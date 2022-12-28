/*-----------------------------------------------------------------------------
 Copyright (C) 2020-2021 ETH Zurich, Switzerland, University of Bologna, Italy.
 All rights reserved.

 File:    main.c
 Author:  Vlad Niculescu      <vladn@iis.ee.ethz.ch>
 Date:    15.03.2021
-------------------------------------------------------------------------------*/

#include <string.h>
#include <stdint.h>
#include <stdbool.h>


#include "app.h"
#include "FreeRTOS.h"
#include "task.h"

#include "debug.h"
#include "log.h"
#include "param.h"
#include "commander.h"
#include "uart_dma_setup.h"

#define DEBUG_MODULE "EXPLORER"

#define AI_STEER
//#define SIMPLE_STEER
//#define FILTER_TEST
#define DEBUG_ON

#define BUFFERSIZE 2
uint8_t aideckRxBuffer[BUFFERSIZE];
volatile uint8_t dma_flag = 0;

#define ABS(a) (a>0.0f?a:-a)
#define DESIRED_HEIGHT 1.0f
#define FORWARD_VEL 0.5
#define ROTATION_VEL 15.0f
#define TOL 0.1f
#define ARENA_EDGES 1.5f
#define SIDEWAYS_VEL 0.25F

float desiredScanAngle = 0;
double integralComponent = 0;

int8_t steer;
uint8_t collision;


double steerFilter(double newSteer, double oldSteer){//Applies low pass filter to steer prediction 
  return 0.7*FORWARD_VEL*newSteer + (1.0 - 0.7)*oldSteer; 
}

double collFilter(double newColl, double oldColl){//Applies low pass filter + other stuff to collision prediction
    integralComponent = (integralComponent + (newColl - 0.3));
    if (integralComponent >= 3)
        integralComponent = 3;
    else if (integralComponent <= 0)
        integralComponent = 0;
    newColl = newColl + 0.2*integralComponent;
    double unfilterVel = FORWARD_VEL*((1 - newColl)*(1-newColl));
    return 0.6*unfilterVel + (1 - 0.6)*oldColl;
}



static void setPositionSetPoint(setpoint_t *setpoint, float x, float y, float z){//Absolute xyz, no rotation
  setpoint->mode.yaw = modeVelocity;
  setpoint->attitudeRate.yaw = 0;
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;
  setpoint->mode.x = modeAbs;
  setpoint->mode.y = modeAbs;
  setpoint->position.x = x;
  setpoint->position.y = y;
  setpoint->velocity_body = true;// Velocity based on global coords... not that it matters much in this case
}

static void setVelocitySetPoint(setpoint_t *setpoint, float x, float y, float z, float yawRate){//xy velocity, absolute z, no rotation
  setpoint->mode.yaw = modeVelocity;
  setpoint->attitudeRate.yaw = yawRate;
  setpoint->mode.z = modeVelocity;
  setpoint->velocity.z = z;
  setpoint->mode.x = modeVelocity;
  setpoint->mode.y = modeVelocity;
  setpoint->velocity.x = x;
  setpoint->velocity.y = y;
  setpoint->velocity_body = true;// Velocity based on relative coords, since now it matters.

}

static void setAttitudeRateSetpoint(setpoint_t *setpoint,float x, float y, float z, float yawRate){//Velocity rotation
  setpoint->mode.yaw = modeVelocity;
  setpoint->attitudeRate.yaw = yawRate;
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;
  setpoint->mode.x = modeAbs;
  setpoint->mode.y = modeAbs;
  setpoint->position.x = x;
  setpoint->position.y = y;
  setpoint->velocity_body = true;
}

#ifdef AI_STEER
// States
typedef enum
{
    TAKEOFF,
    FORWARD,
    EDGE_ROTATE,
    LAND,
    IDLE
} ControllerState;


float edgeFinder(float x, float y){
    if (x >= ARENA_EDGES){//Maybe add a tolerance?
        if (y >= ARENA_EDGES){//corner 0
            return -90;
        }
        else if (y < -ARENA_EDGES){ //corner 1
            return  180;
        }
        else{// line 5
            return -90;
        }
    }
    else if (x < -ARENA_EDGES){
       if (y >= ARENA_EDGES){ //corner 3
            return 0;
        }
        else if (y < -ARENA_EDGES){//corner 2

            return  90;
        }
        else{
            //line 6
            return  90;
        }
    }
    else if (y >= ARENA_EDGES){     // region 4
        return  0;
    }
    else{       //region 7

        return 180;
    }
}

void appMain(){

    vTaskDelay(M2T(1000));//After every task, especially those relating to movement, an appropriate delay must be set
    USART_DMA_Start(115200, (int8_t *)aideckRxBuffer, BUFFERSIZE);

    //Initialize Sensors
    paramVarId_t idAiDeck = paramGetVarId("deck", "bcAI");
    uint8_t aiInit = paramGetUint(idAiDeck);
    paramVarId_t idPositioningDeck = paramGetVarId("deck", "bcFlow2");
    uint8_t positioningInit = paramGetUint(idPositioningDeck);
    logVarId_t idBattery = logGetVarId("pm", "state");

    //Initialize Estimators
    logVarId_t idStabilizerYaw = logGetVarId("stabilizer", "yaw");
    logVarId_t idXEstimate = logGetVarId("stateEstimate", "x");
    logVarId_t idYEstimate = logGetVarId("stateEstimate", "y");
    logVarId_t idZEstimate = logGetVarId("stateEstimate", "z");

    //Initialize Variables
    static setpoint_t setpoint;
    ControllerState currentState = TAKEOFF;
    bool turnOff = false;// If set to true, the main loop closes, and the motors turn off.
    bool down_half = false;//Part of the landing procedure.
    bool down_quarter = false;
    float edgeRotation = 0;
    float radSteer = 0;
    float steerPred = 0;
    float newSteer = 0;
    float velocityPred = 0;
    float newColl = 0;

    int idleCounter = 0;

    int takeoffCounter = 0;
    float idleX = 0;
    float idleY = 0;
    memset(&setpoint, 0, sizeof(setpoint_t));
    commanderSetSetpoint(&setpoint, 3);

    vTaskDelay(M2T(3000));//After every task, especially those relating to movement, an appropriate delay must be set

    while(positioningInit && turnOff == false && aiInit){
        if (dma_flag == 1){

            dma_flag = 0;  // clear the flag
            
            
            steer = *(aideckRxBuffer);
            newSteer = steer*0.2460539;
            collision = *(aideckRxBuffer + 1);
            newColl = collision*0.00787402;
            radSteer = steerFilter(newSteer, radSteer);
            steerPred = radSteer*57.2957795131f;
            velocityPred = collFilter(newColl, velocityPred);

            DEBUG_PRINT("Velocity pred: %f  Steer pred: %f \n\n", (double)velocityPred, (double)steerPred);
            steer = 0;
            collision = 0;

        }


            float xEstimate = logGetFloat(idXEstimate);
            float yEstimate = logGetFloat(idYEstimate);
            float zEstimate = logGetFloat(idZEstimate);
            float yawEstimate = logGetFloat(idStabilizerYaw);
            if (logGetInt(idBattery) == 3){
                DEBUG_PRINT("Low battery\n");
                if (currentState == TAKEOFF){
                    turnOff = true;
                }
                else{
                    currentState = LAND;
                }

            }

            #ifdef DEBUG_ON
  		        //DEBUG_PRINT("xEstimate: %f\n", (double)xEstimate);
  		        //DEBUG_PRINT("yEstimate: %f\n", (double)yEstimate);
                //DEBUG_PRINT("zEstimate: %f\n\n ", (double)zEstimate);
            #endif

            switch (currentState)
            {
            case TAKEOFF:
                vTaskDelay(M2T(10));
                setPositionSetPoint(&setpoint, 0, 0, DESIRED_HEIGHT);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(400));
                zEstimate = logGetFloat(idZEstimate);
                vTaskDelay(M2T(10));
                takeoffCounter++;
                //DEBUG_PRINT("Takeoff\n");
                if(zEstimate >= DESIRED_HEIGHT - TOL && takeoffCounter >= 10){
                    currentState = FORWARD;
                    commanderRelaxPriority();
                    vTaskDelay(M2T(10));
                }
                break;
            case FORWARD: //Goes forward until it reaches arena edges.
                if(xEstimate <= ARENA_EDGES && xEstimate >= -ARENA_EDGES && yEstimate <= ARENA_EDGES && yEstimate >= -ARENA_EDGES){
                    vTaskDelay(M2T(10));
                    setVelocitySetPoint(&setpoint, velocityPred, 0, 0 , steerPred);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(200));
                    //DEBUG_PRINT("Forwards: %f, %f\n", (double)velocityPred, (double)steerPred);
                }
                else{
                    vTaskDelay(M2T(50));
                    edgeRotation = edgeFinder(xEstimate, yEstimate);
                    currentState = EDGE_ROTATE;
                    commanderRelaxPriority();
                    setVelocitySetPoint(&setpoint, -FORWARD_VEL, 0, 0 , 0);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(200));
                    idleX = xEstimate; idleY = yEstimate;
                }
                
                break;
            case EDGE_ROTATE:
                vTaskDelay(M2T(10));
                setAttitudeRateSetpoint(&setpoint, idleX, idleY, DESIRED_HEIGHT, ROTATION_VEL);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(300));
                yawEstimate = logGetFloat(idStabilizerYaw);
                vTaskDelay(M2T(50));
                DEBUG_PRINT("Look Inwards, %f, %f\n", (double)edgeRotation, (double)yawEstimate);
                float gamma = yawEstimate - edgeRotation;
                if (ABS(gamma) <= 10.0f){
                    commanderRelaxPriority();
                    vTaskDelay(M2T(10));
                    setVelocitySetPoint(&setpoint, 0, 0, 0, 0);
                    vTaskDelay(M2T(1000));
                    currentState = FORWARD;
                    idleX = xEstimate; idleY = yEstimate;
                }
                break;
            case LAND:
                vTaskDelay(M2T(50));
                DEBUG_PRINT("Landing\n");
                if(down_half == false){
                    setVelocitySetPoint(&setpoint, 0, 0, -0.2f, 0 );
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(100));
                    float zEstimate = logGetFloat(idZEstimate);
                    if (ABS(zEstimate - DESIRED_HEIGHT/2) <= TOL){
                    down_half = true;
                    }
                }
                else if(down_quarter == false){
                    setVelocitySetPoint(&setpoint, 0, 0, -0.2f, 0);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(100));
                    float zEstimate = logGetFloat(idZEstimate);
                    if (ABS(zEstimate - DESIRED_HEIGHT/4) <= TOL){
                        down_quarter = true;
                    }
                }
                else{
                    setVelocitySetPoint(&setpoint, 0,0,-0.2f, 0);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(100));
                    turnOff = true;
                }
                break;
            case IDLE:
                vTaskDelay(M2T(10));
                setVelocitySetPoint(&setpoint, 0,0, 0, 0);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(300));
                idleCounter++;
                DEBUG_PRINT("Idle\n");

                if (idleCounter >= 20){
                    currentState = LAND;
                    commanderRelaxPriority();
                    vTaskDelay(M2T(10));
                }

                break;
            }
    }
}
#elif  defined SIMPLE_STEER
// States
typedef enum
{
    TAKEOFF,
    FORWARD,
    EDGE_ROTATE,
    OBSTACLE_ROTATE,
    LAND,
    IDLE
} ControllerState;

float edgeFinder(float x, float y){
    if (x >= ARENA_EDGES){//Maybe add a tolerance?
        if (y >= ARENA_EDGES){//corner 0

            return 180;
        }
        else if (y < -ARENA_EDGES){ //corner 1

            return  90;
        }
        else{// line 5
            return 90;
        }
    }
    else if (x < -ARENA_EDGES){
       if (y >= ARENA_EDGES){ //corner 3
            return  -90;
        }
        else if (y < -ARENA_EDGES){//corner 2
            return  0;
        }
        else{
            //line 6
            return  -90;
        }
    }
    else if (y >= ARENA_EDGES){     // region 4

        return  180;
    }
    else{       //region 7
        return 0;
    }
}

void appMain(){

    vTaskDelay(M2T(1000));//After every task, especially those relating to movement, an appropriate delay must be set
    USART_DMA_Start(115200, (int8_t *)aideckRxBuffer, BUFFERSIZE);
 
    //Initialize Sensors
    paramVarId_t idAiDeck = paramGetVarId("deck", "bcAI");
    uint8_t aiInit = paramGetUint(idAiDeck);
    paramVarId_t idPositioningDeck = paramGetVarId("deck", "bcFlow2");
    uint8_t positioningInit = paramGetUint(idPositioningDeck);
    logVarId_t idBattery = logGetVarId("pm", "state");

    //Initialize Estimators
    logVarId_t idStabilizerYaw = logGetVarId("stabilizer", "yaw");
    logVarId_t idXEstimate = logGetVarId("stateEstimate", "x");
    logVarId_t idYEstimate = logGetVarId("stateEstimate", "y");
    logVarId_t idZEstimate = logGetVarId("stateEstimate", "z");

    //Initialize Variables
    static setpoint_t setpoint;
    ControllerState currentState = TAKEOFF;
    bool turnOff = false;// If set to true, the main loop closes, and the motors turn off.
    bool down_half = false;//Part of the landing procedure.
    bool down_quarter = false;
    
    float edgeRotation = 0;
    float steerPred = 0;
    float newSteer = 0;
    float velocityPred = 0;
    float newColl = 0;

    float desiredAngle = 0;
    uint8_t rotationDir = 0;
    int idleCounter = 0;

    int takeoffCounter = 0;
    float idleX = 0;
    float idleY = 0;
    memset(&setpoint, 0, sizeof(setpoint_t));
    commanderSetSetpoint(&setpoint, 3);

    vTaskDelay(M2T(3000));//After every task, especially those relating to movement, an appropriate delay must be set

    while(positioningInit && turnOff == false && aiInit){
        if (dma_flag == 1){

            dma_flag = 0;  // clear the flag
            steer = *(aideckRxBuffer);
            newSteer = steer*0.2460539;
            collision = *(aideckRxBuffer + 1);
            newColl = collision*0.00787402;
            steerPred = steerFilter(newSteer, steerPred);
            velocityPred = collFilter(newColl, velocityPred);

            steer = 0;
            collision = 0;

        }


            float xEstimate = logGetFloat(idXEstimate);
            float yEstimate = logGetFloat(idYEstimate);
            float zEstimate = logGetFloat(idZEstimate);
            float yawEstimate = logGetFloat(idStabilizerYaw);

            if (logGetInt(idBattery) == 3){
                DEBUG_PRINT("Low battery\n");
                if (currentState == TAKEOFF){
                    turnOff = true;
                }
                else{
                    currentState = LAND;
                }

            }

            #ifdef DEBUG_ON
  		        //DEBUG_PRINT("xEstimate: %f\n", (double)xEstimate);
  		        //DEBUG_PRINT("yEstimate: %f\n", (double)yEstimate);
                //DEBUG_PRINT("zEstimate: %f\n\n ", (double)zEstimate);
            #endif

            switch (currentState)
            {
            case TAKEOFF:
                vTaskDelay(M2T(10));
                setPositionSetPoint(&setpoint, 0, 0, DESIRED_HEIGHT);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(400));
                zEstimate = logGetFloat(idZEstimate);
                vTaskDelay(M2T(10));
                takeoffCounter++;
                //DEBUG_PRINT("Takeoff\n");
                if(zEstimate >= DESIRED_HEIGHT - TOL && takeoffCounter >= 10){
                    currentState = FORWARD;
                    commanderRelaxPriority();
                    vTaskDelay(M2T(10));
                }
                break;
            case FORWARD: //Goes forward until it reaches arena edges.
                if (velocityPred < 0.3f && dma_flag == 1){//Obstacle detected
                    vTaskDelay(M2T(50));
                    currentState = OBSTACLE_ROTATE;
                    commanderRelaxPriority();
                    setVelocitySetPoint(&setpoint, -FORWARD_VEL, 0, 0 , 0);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(200));
                    idleX = xEstimate; idleY = yEstimate;
                    rotationDir = steerPred >= 0? 1:-1;
                    desiredAngle = yawEstimate + 90 > 180? (yawEstimate + 90) -360 : yawEstimate +90;
                }
                else if(xEstimate <= ARENA_EDGES && xEstimate >= -ARENA_EDGES && yEstimate <= ARENA_EDGES && yEstimate >= -ARENA_EDGES){//No Obstacle, Inside Edges
                    vTaskDelay(M2T(10));
                    setVelocitySetPoint(&setpoint, FORWARD_VEL, 0, 0 , 0);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(500));
                }
                else{
                    vTaskDelay(M2T(50));
                    edgeRotation = edgeFinder(xEstimate, yEstimate);
                    currentState = EDGE_ROTATE;
                    commanderRelaxPriority();
                    setVelocitySetPoint(&setpoint, -FORWARD_VEL, 0, 0 , 0);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(200));
                    idleX = xEstimate; idleY = yEstimate;
                }
                DEBUG_PRINT("Forwards\n");
                break;
            case OBSTACLE_ROTATE:
                vTaskDelay(M2T(10));
                setAttitudeRateSetpoint(&setpoint, idleX, idleY, DESIRED_HEIGHT, rotationDir*ROTATION_VEL);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(300));
                yawEstimate = logGetFloat(idStabilizerYaw);
                vTaskDelay(M2T(50));
                DEBUG_PRINT("Look Inwards, %f, %f\n", (double)edgeRotation, (double)yawEstimate);
                float delta = yawEstimate - desiredAngle;
                if (ABS(delta) <= 10.0f && velocityPred > 0.5f){
                    commanderRelaxPriority();
                    vTaskDelay(M2T(10));
                    setVelocitySetPoint(&setpoint, 0, 0, 0, 0);
                    vTaskDelay(M2T(1000));
                    currentState = FORWARD;
                }
                break;
            case EDGE_ROTATE:
                vTaskDelay(M2T(10));
                setAttitudeRateSetpoint(&setpoint, idleX, idleY, DESIRED_HEIGHT, ROTATION_VEL);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(300));
                yawEstimate = logGetFloat(idStabilizerYaw);
                vTaskDelay(M2T(50));
                DEBUG_PRINT("Look Inwards, %f, %f\n", (double)edgeRotation, (double)yawEstimate);
                float gamma = yawEstimate - edgeRotation;
                if (ABS(gamma) <= 10.0f){
                    commanderRelaxPriority();
                    vTaskDelay(M2T(10));
                    setVelocitySetPoint(&setpoint, 0, 0, 0, 0);
                    vTaskDelay(M2T(1000));
                    currentState = FORWARD;
                    idleX = xEstimate; idleY = yEstimate;
                }
                break;
            case LAND:
                vTaskDelay(M2T(50));
                DEBUG_PRINT("Landing\n");
                if(down_half == false){
                    setVelocitySetPoint(&setpoint, 0, 0, -0.2f, 0);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(100));
                    float zEstimate = logGetFloat(idZEstimate);
                    if (ABS(zEstimate - DESIRED_HEIGHT/2) <= TOL){
                    down_half = true;
                    }
                }
                else if(down_quarter == false){
                    setVelocitySetPoint(&setpoint, 0, 0, -0.2f, 0);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(100));
                    float zEstimate = logGetFloat(idZEstimate);
                    if (ABS(zEstimate - DESIRED_HEIGHT/4) <= TOL){
                        down_quarter = true;
                    }
                }
                else{
                    setVelocitySetPoint(&setpoint, 0,0,-0.2f, 0);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(100));
                    turnOff = true;
                }
                break;
            case IDLE:
                vTaskDelay(M2T(10));
                setVelocitySetPoint(&setpoint, 0,0, 0, 0);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(300));
                idleCounter++;
                DEBUG_PRINT("Idle\n");

                if (idleCounter >= 20){
                    currentState = LAND;
                    commanderRelaxPriority();
                    vTaskDelay(M2T(10));
                }

                break;
            }

    }
}
#elif defined FILTER_TEST
void appMain(){
    vTaskDelay(M2T(3000));
    double velocityPred = 0;
    double steerPred = 0;
    
    DEBUG_PRINT("Obstacle detection starting!\n");
    USART_DMA_Start(115200, (int8_t *)aideckRxBuffer, BUFFERSIZE);
    while(1){
        vTaskDelay(M2T(10));
        if (dma_flag == 1){
            dma_flag = 0;  // clear the flag
            steer = *(aideckRxBuffer);
            double newSteer = steer*0.2460539;
            collision = *(aideckRxBuffer + 1);
            double newColl = collision*0.00787402;
            steerPred = steerFilter(newSteer, steerPred);
            velocityPred = collFilter(newColl, velocityPred);

            DEBUG_PRINT("New Steer: %f\n", newSteer);
  		    DEBUG_PRINT("New Coll: %f\n", newColl);
            DEBUG_PRINT("Steer Prediction: %f \n", steerPred);
  		    DEBUG_PRINT("Velocity Prediction: %f \n\n", velocityPred);
            
            vTaskDelay(M2T(100));
            steer = 0;
            collision = 0;
            
        }
        
    }
}
#endif
void __attribute__((used)) DMA1_Stream1_IRQHandler(void)
{
    //static bool first = false;
    DMA_ClearFlag(DMA1_Stream1, UART3_RX_DMA_ALL_FLAGS);
    dma_flag = 1;

}
