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



#define DEBUG_ON


#define BUFFERSIZE 8
uint32_t aideckRxBuffer[BUFFERSIZE/4]; /* 8 bytes of data == 2 32-bit integers */


volatile uint8_t dma_flag = 0;


#define DESIRED_HEIGHT 1.0f
#define FORWARD_VEL 1.0f
#define ROTATION_VEL 15.0f
#define TOL 0.1f
#define ARENA_EDGES 3.5f
#define SIDEWAYS_VEL 0.5f

#define ABS(a) (a>0.0f?a:-a)
float desiredScanAngle = 0;
uint32_t squareX;
uint32_t squareY;
// States
typedef enum
{
    TAKEOFF,
    FORWARD,
    GATE_FOUND,
    LOOK_INWARDS,
    FIND_GATE,
    LAND,
    IDLE
} ControllerState;

float edgeFinder(float x, float y){
    if (x >= 0.5f){//Maybe add a tolerance?
        if (y >= ARENA_EDGES){//corner 0

            desiredScanAngle = -90;
            return 180;
        }
        else if (y < -ARENA_EDGES){ //corner 1

            desiredScanAngle = 180;
            return  90;
        }
        else{// line 5

            desiredScanAngle =  -90;
            return 90;
        }
    }
    else if (x < -ARENA_EDGES){
       if (y >= ARENA_EDGES){ //corner 3

            desiredScanAngle =  0;
            return  -90;
        }
        else if (y < -ARENA_EDGES){//corner 2

            desiredScanAngle =  90;
            return  0;
        }
        else{
            //line 6
            desiredScanAngle = 90;
            return  -90;
        }
    }
    else if (y >= ARENA_EDGES){     // region 4

        desiredScanAngle = 0;
        return  180;
    }
    else{       //region 7

        desiredScanAngle = 180;
        return 0;
    }
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

static void setVelocitySetPoint(setpoint_t *setpoint, float x, float y, float z){//xy velocity, absolute z, no rotation
  setpoint->mode.yaw = modeVelocity;
  setpoint->attitudeRate.yaw = 0;
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
            squareX = (*((uint32_t*)aideckRxBuffer));
            squareY = (*((uint32_t*)aideckRxBuffer + 1));


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
                    currentState = IDLE;
                    commanderRelaxPriority();
                    vTaskDelay(M2T(10));
                }
                break;
            case FORWARD: //Goes forward until it reaches arena edges.
                if(xEstimate <= ARENA_EDGES && xEstimate >= -ARENA_EDGES && yEstimate <= ARENA_EDGES && yEstimate >= -ARENA_EDGES){
                    if (squareX != 0 && squareY != 0){
                        if (squareX >= 110){
                            vTaskDelay(M2T(10));
                            setVelocitySetPoint(&setpoint, FORWARD_VEL, -SIDEWAYS_VEL, 0);
                            commanderSetSetpoint(&setpoint, 3);
                            vTaskDelay(M2T(500));
                        }
                        else if(squareX <= 90){
                            vTaskDelay(M2T(10));
                            setVelocitySetPoint(&setpoint, FORWARD_VEL, SIDEWAYS_VEL, 0);
                            commanderSetSetpoint(&setpoint, 3);
                            vTaskDelay(M2T(500));
                        }
                        else{
                        vTaskDelay(M2T(10));
                        setVelocitySetPoint(&setpoint, FORWARD_VEL, 0, 0);
                        commanderSetSetpoint(&setpoint, 3);
                        vTaskDelay(M2T(500));
                        }
                    }
                    else{
                        vTaskDelay(M2T(10));
                        setVelocitySetPoint(&setpoint, FORWARD_VEL, 0, 0);
                        commanderSetSetpoint(&setpoint, 3);
                        vTaskDelay(M2T(500));
                    }
                }
                else{
                    vTaskDelay(M2T(50));
                    edgeRotation = edgeFinder(xEstimate, yEstimate);
                    currentState = LOOK_INWARDS;
                    commanderRelaxPriority();
                    vTaskDelay(M2T(10));
                    idleX = xEstimate; idleY = yEstimate;
                }
                DEBUG_PRINT("Forwards\n");
                break;
            case GATE_FOUND:
                if(xEstimate >= ARENA_EDGES|| xEstimate <= -ARENA_EDGES || yEstimate >= ARENA_EDGES || yEstimate <= -ARENA_EDGES){
                    if (squareX != 0 && squareY != 0){
                        if (squareX >= 110){
                            vTaskDelay(M2T(10));
                            setVelocitySetPoint(&setpoint, FORWARD_VEL, -SIDEWAYS_VEL, 0);
                            commanderSetSetpoint(&setpoint, 3);
                            vTaskDelay(M2T(200));
                        }
                        else if(squareX <= 90){
                            vTaskDelay(M2T(10));
                            setVelocitySetPoint(&setpoint, FORWARD_VEL, SIDEWAYS_VEL, 0);
                            commanderSetSetpoint(&setpoint, 3);
                            vTaskDelay(M2T(200));
                        }
                        else{
                        vTaskDelay(M2T(10));
                        setVelocitySetPoint(&setpoint, FORWARD_VEL, 0, 0);
                        commanderSetSetpoint(&setpoint, 3);
                        vTaskDelay(M2T(200));
                        }
                    }
                    else{
                        vTaskDelay(M2T(10));
                        setVelocitySetPoint(&setpoint, FORWARD_VEL, 0, 0);
                        commanderSetSetpoint(&setpoint, 3);
                        vTaskDelay(M2T(200));
                    }
                    DEBUG_PRINT("gate found!\n");

                }
                else{
                    currentState = FORWARD;
                    commanderRelaxPriority();
                    vTaskDelay(M2T(10));
                }
                break;
            case LOOK_INWARDS:
                vTaskDelay(M2T(10));
                setAttitudeRateSetpoint(&setpoint, idleX, idleY, DESIRED_HEIGHT, ROTATION_VEL);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(300));
                yawEstimate = logGetFloat(idStabilizerYaw);
                vTaskDelay(M2T(50));
                DEBUG_PRINT("Look Inwards, %f, %f\n", (double)edgeRotation, (double)yawEstimate);
                float gamma = yawEstimate - edgeRotation;
                if (ABS(gamma) <= 10.0f){
                    //DEBUG_PRINT("Entrou1\n");
                    commanderRelaxPriority();
                    vTaskDelay(M2T(10));
                    setVelocitySetPoint(&setpoint, 0, 0, 0);
                    vTaskDelay(M2T(1000));
                    currentState = FIND_GATE;
                    idleX = xEstimate; idleY = yEstimate;
                }
                break;
            case FIND_GATE:
                if ((squareX != 0 && squareY != 0)){
                    DEBUG_PRINT("Achou!\n");
                    commanderRelaxPriority();
                     vTaskDelay(M2T(10));
                    if (squareX > 100)
                        setAttitudeRateSetpoint(&setpoint,idleX, idleY, DESIRED_HEIGHT, -ROTATION_VEL);
                    else
                        setVelocitySetPoint(&setpoint, FORWARD_VEL, 0, DESIRED_HEIGHT);

                    //setPositionSetPoint(&setpoint, xEstimate, yEstimate, DESIRED_HEIGHT);
                    vTaskDelay(M2T(500));
                    commanderSetSetpoint(&setpoint, 3);
                    currentState = GATE_FOUND;

                }
                else{
                    vTaskDelay(M2T(10));
                    setAttitudeRateSetpoint(&setpoint,idleX, idleY, DESIRED_HEIGHT, ROTATION_VEL);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(300));
                    yawEstimate = logGetFloat(idStabilizerYaw);
                    vTaskDelay(M2T(50));
                    float delta = yawEstimate - desiredScanAngle;
                    #ifdef DEBUG_ON
  		            DEBUG_PRINT("output 1: %ld\n", squareX);
  		            DEBUG_PRINT("output 2: %ld\n\n", squareY);
                    DEBUG_PRINT("Finding gate..");
                    #endif

                    if (ABS(delta) <= 10.0f){
                        commanderRelaxPriority();
                        vTaskDelay(M2T(10));
                        currentState = IDLE;
                    }
                }

                break;
            case LAND:
                vTaskDelay(M2T(50));
                DEBUG_PRINT("Landing\n");
                if(down_half == false){
                    setVelocitySetPoint(&setpoint, 0, 0, -0.2f );
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(100));
                    float zEstimate = logGetFloat(idZEstimate);
                    if (ABS(zEstimate - DESIRED_HEIGHT/2) <= TOL){
                    down_half = true;
                    }
                }
                else if(down_quarter == false){
                    setVelocitySetPoint(&setpoint, 0, 0, -0.2f);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(100));
                    float zEstimate = logGetFloat(idZEstimate);
                    if (ABS(zEstimate - DESIRED_HEIGHT/4) <= TOL){
                        down_quarter = true;
                    }
                }
                else{
                    setVelocitySetPoint(&setpoint, 0,0,-0.2f);
                    commanderSetSetpoint(&setpoint, 3);
                    vTaskDelay(M2T(100));
                    turnOff = true;
                }
                break;
            case IDLE:
                vTaskDelay(M2T(10));
                setVelocitySetPoint(&setpoint, 0,0, 0);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(300));
                idleCounter++;
                
                if (idleCounter >= 20){
                    currentState = LAND;
                    commanderRelaxPriority();
                    vTaskDelay(M2T(10));
                }
                break;
            }
        squareX = 0;
        squareY = 0;

    }
}
void __attribute__((used)) DMA1_Stream1_IRQHandler(void)
{
    //static bool first = false;
    DMA_ClearFlag(DMA1_Stream1, UART3_RX_DMA_ALL_FLAGS);
    dma_flag = 1;

}
