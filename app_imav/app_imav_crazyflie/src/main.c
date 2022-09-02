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

#define DEBUG_MODULE "EXPLORER"
#define BUFFERSIZE 8

//#define TEST_COM
//#define TEST_AI
#define DEBUG_ON

int32_t aideckRxBuffer[BUFFERSIZE/4]; /* 8 bytes of data == 2 32-bit integers */
volatile uint8_t dma_flag = 0;

#ifdef TEST_COM
int32_t squareX;
int32_t squareY;

void appMain()
{
  DEBUG_PRINT("Gate detection started!\n");
  vTaskDelay(M2T(3000));
  USART_DMA_Start(115200, (int8_t *)aideckRxBuffer, BUFFERSIZE);

  while(1)
  {
  	vTaskDelay(M2T(10));

  	if (dma_flag == 1)
  	{
  		  dma_flag = 0;  // clear the flag
          squareX = *(aideckRxBuffer);
          squareY = *(aideckRxBuffer + 1);

          #ifdef DEBUG_ON
  		  DEBUG_PRINT("output 1: %ld\n", squareX);
  		  DEBUG_PRINT("output 2: %ld\n\n", squareY);
          #endif

          memset(aideckRxBuffer, 0, BUFFERSIZE);
          squareX = 0;
          squareY = 0;
  	}
  }
}
#elsif TEST_AI
int32_t collision;
int32_t steer;

void appMain()
{
	DEBUG_PRINT("Obstacle detection started!\n");
    vTaskDelay(M2T(3000));
	USART_DMA_Start(115200, (int8_t *)aideckRxBuffer, BUFFERSIZE);

	while(1) {
		vTaskDelay(M2T(100));

		if (dma_flag == 1)
		{
			dma_flag = 0;  // clear the flag
            steer = aideckRxBuffer[0];
            collision = aideckRxBuffer[1];

            #ifdef DEBUG_ON
			DEBUG_PRINT("output 1: %f\n", steer/2147483647.0);
			DEBUG_PRINT("output 2: %f\n\n", ((uint32_t)collision/4294967295.0));
            #endif

            // maybe remove this?
            memset(aideckRxBuffer, 0, BUFFERSIZE);
		}
	}
}

void __attribute__((used)) DMA1_Stream1_IRQHandler(void)
{
    DMA_ClearFlag(DMA1_Stream1, UART3_RX_DMA_ALL_FLAGS);
    //ControllerState =
    dma_flag = 1;
}

#else
#define DESIRED_HEIGHT 1.0f
#define FORWARD_VEL 0.1f
#define ROTATION_VEL 15
#define TOL 0.1f
#define ARENA_EDGES 0.5f

#define ABS(a) (a>0?a:-a)
uint8_t rotationDir = 1;

// States
typedef enum
{
    TAKEOFF,
    FORWARD,
    LOOK_INWARDS,
    FIND_GATE,
    //AVOID_OBSTACLE,
    LAND,
    IDLE
} ControllerState;
/*
float edgeFinder(float x, float y, float yaw){
    if (x >= ARENA_EDGES){
        if (y >= ARENA_EDGES){
            rotationDir = yaw >= 45  ? 1: -1;
            return yaw <= 45 ? 180: -90;
        }
        else if (y < -ARENA_EDGES){
            rotationDir = yaw >= -45 ? 1: -1 ;
            return yaw >= -45 ? 90: -180;
        }
        else{
            rotationDir = yaw >= 0 ? 1 : -1;
            return yaw >= 0? 90 : -90;
        }
    }
    else if (x < -ARENA_EDGES){
       if (y >= ARENA_EDGES){
            rotationDir = yaw >= 135? 1 : -1;
            return yaw >=135 ? -90 : 0;
        }
        else if (y < -ARENA_EDGES){
            rotationDir = yaw >= -135? 1 : -1;
            return yaw >= -135 ? 0: 90;
        }
        else{
            rotationDir = yaw >= -180 && yaw <= -90 ? 1 : -1;
            return yaw >= -180 && yaw <= -90? -90 : 90;
        }
    }
    else if (y >= ARENA_EDGES){
        rotationDir = yaw >= 90? 1 : -1;
        return yaw >= 90? 180 : 0;
    }
    else{
        rotationDir = yaw >= -90? 1 : -1;
        return yaw >= -90? 0: -180;
    }
}
*/
float edgeFinder(float x, float y, float yaw){
    if (x >= ARENA_EDGES){
        if (y >= ARENA_EDGES){
            rotationDir = yaw >= 45 ? 1: -1; //corner 0
            desiredScanAngle = (yaw >= 45) ? -90:-180;   //tanto faz -pi e pi?
            return yaw >= 45 ? 180: -90;
        }
        else if (y < -ARENA_EDGES){
            rotationDir = yaw >= -45 ? 1: -1 ;      //corner 1
            desiredScanAngle = yaw >= -45 ? 180: 90 
            return yaw >= -45 ? 90: -180;
        }
        else{
            rotationDir = (yaw >= 0)? 1 : -1;   // line 5
            desiredScanAngle = (yaw >= 0)? -90 : 90;
            return (yaw >= 0)? 90 : -90;
        }
    }
    else if (x < -ARENA_EDGES){
       if (y >= ARENA_EDGES){
            rotationDir = yaw >= 135? 1 : -1;   //corner 3
            desiredScanAngle = (yaw >= 135)? 0 : -90;
            return yaw >= 135 ? -90: 0;
        }
        else if (y < -ARENA_EDGES){
            rotationDir = yaw >= -135? 1 : -1;   //corner 2
            desiredScanAngle = (yaw >= -135)? 90 : 0;
            return yaw >= -135 ? 0: 90;
        }
        else{
            rotationDir = (yaw <= 180 && yaw>0)? -1 : 1;       //line 6   
            desiredScanAngle = (yaw <= 180 && yaw>0)? -90 : 90; 
            return (yaw <= 180 && yaw>0)? 90 : -90;
        } 
    }
    else if (y >= ARENA_EDGES){     // region 4
        rotationDir = yaw >= 90? 1 : -1;
        desiredScanAngle = 0:180;
        return yaw >= 90? 180 : 0;
    }
    else{       //region 7
        rotationDir = yaw >= -90? 1 : -1;
        desiredScanAngle = 180:0;
        return yaw >= -90? 0: -180; 
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
  setpoint->velocity_body = false;// Velocity based on global coords... not that it matters much in this case
}

static void setVelocitySetPoint(setpoint_t *setpoint, float x, float y, float z){//xy velocity, absolute z, no rotation
  setpoint->mode.yaw = modeVelocity;
  setpoint->attitudeRate.yaw = 0;
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;
  setpoint->mode.x = modeVelocity;
  setpoint->mode.y = modeVelocity;
  setpoint->velocity.x = x;
  setpoint->velocity.y = y;
  setpoint->velocity_body = true;// Velocity based on relative coords, since now it matters.

}
static void setAttitudeSetpoint(setpoint_t *setpoint,float x, float y, float z, float yaw){//Absolute rotation
  setpoint->mode.yaw = modeAbs;
  setpoint->attitude.yaw = yaw;
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;
  setpoint->mode.x = modeAbs;
  setpoint->mode.y = modeAbs;
  setpoint->position.x = x;
  setpoint->position.y = y;
  setpoint->velocity_body = true;
}
static void setAttitudeRateSetpoint(setpoint_t *setpoint,float x, float y, float z, float yawRate){//Velocity rotation
  setpoint->mode.yaw = modeVelocity;
  setpoint->attitudeRate.yaw = yawRate;
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;
  setpoint->mode.x = modeVelocity;
  setpoint->mode.y = modeVelocity;
  setpoint->velocity.x = x;
  setpoint->velocity.y = y;
  setpoint->velocity_body = false;
}
void appMain(){

    //Initialize Sensors
    paramVarId_t idPositioningDeck = paramGetVarId("deck", "bcFlow2");
    uint8_t positioningInit = paramGetUint(idPositioningDeck);

    //Initialize Estimators
    logVarId_t idStabilizerYaw = logGetVarId("stabilizer", "yaw");
    logVarId_t idXEstimate = logGetVarId("stateEstimate", "x");
    logVarId_t idYEstimate = logGetVarId("stateEstimate", "y");
    logVarId_t idZEstimate = logGetVarId("stateEstimate", "z");

    //Initialize Variables
    static setpoint_t setpoint;
    ControllerState currentState = TAKEOFF;
    bool turnOff = false;// If set to true, the main loop closes, and the motors turn off.
    bool down_half = false;
    bool down_quarter = false;
    float edgeRotation = 0;
    float desiredScanAngle = 0;
    int idleCounter = 0;
    int inwardsCounter = 0;
    int takeoffCounter = 0;
    float idleX = 0;
    float idleY = 0;
    memset(&setpoint, 0, sizeof(setpoint_t));
    commanderSetSetpoint(&setpoint, 3);

    vTaskDelay(M2T(3000));//After every task, especially those relating to movement, an appropriate delay must be set
    while(positioningInit && turnOff == false){
        float xEstimate = logGetFloat(idXEstimate);
        float yEstimate = logGetFloat(idYEstimate);
        float zEstimate = logGetFloat(idZEstimate);
        float yawEstimate = logGetFloat(idStabilizerYaw);

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
            DEBUG_PRINT("Takeoff\n");
            if(zEstimate >= DESIRED_HEIGHT - TOL && takeoffCounter >= 10){
                currentState = FORWARD;
            }
            break;
        case FORWARD: //Goes forward until it reaches arena edges.
            if(xEstimate <= ARENA_EDGES && xEstimate >= -ARENA_EDGES && yEstimate <= ARENA_EDGES && yEstimate >= -ARENA_EDGES){
                vTaskDelay(M2T(10));
                setVelocitySetPoint(&setpoint, FORWARD_VEL, 0, DESIRED_HEIGHT);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(200));
            }
            else{
                edgeRotation = edgeFinder(xEstimate, yEstimate, yawEstimate);
                currentState = LOOK_INWARDS;
            }
            DEBUG_PRINT("Forwards\n");
            break;
        case LOOK_INWARDS:
            vTaskDelay(M2T(10));
            setAttitudeSetpoint(&setpoint, xEstimate, yEstimate, DESIRED_HEIGHT, edgeRotation);
            commanderSetSetpoint(&setpoint, 3);
            vTaskDelay(M2T(100));
            yawEstimate = logGetFloat(idStabilizerYaw);
            vTaskDelay(M2T(50));
            inwardsCounter++;
            DEBUG_PRINT("Look Inwards, %f\n", (double)edgeRotation);
            if (ABS(yawEstimate - edgeRotation) <= 10 && inwardsCounter >= 10){
                /*
                if((xEstimate >= ARENA_EDGES || xEstimate <= -ARENA_EDGES) && (yEstimate >= ARENA_EDGES || yEstimate <= -ARENA_EDGES)){
                    if (rotationDir > 0){
                        desiredScanAngle = (yawEstimate + 90) >= 180? (yawEstimate + 90) -360 : yawEstimate + 90;
                    }
                    else{
                        desiredScanAngle = (yawEstimate - 90) < -180? (yawEstimate - 90)  + 360 : yawEstimate - 90;
                    }
                }
                else{
                    if (rotationDir > 0){
                        desiredScanAngle = (yawEstimate + 180) >= 180? (yawEstimate + 180) -360 : yawEstimate + 180;
                    }
                    else{
                        desiredScanAngle = (yawEstimate - 180) < -180? (yawEstimate - 180)  + 360 : yawEstimate - 180;
                    }
                }
                */
                currentState = FIND_GATE;
                idleX = xEstimate; idleY = yEstimate;
            }
            break;
        case FIND_GATE:
            vTaskDelay(M2T(10));
            setAttitudeRateSetpoint(&setpoint,0, 0, DESIRED_HEIGHT, rotationDir*ROTATION_VEL);
            commanderSetSetpoint(&setpoint, 3);
            vTaskDelay(M2T(300));
            yawEstimate = logGetFloat(idStabilizerYaw);
            vTaskDelay(M2T(10));
            DEBUG_PRINT("Finding gate... %f, %f\n", (double)yawEstimate, (double) desiredScanAngle);
            if(yawEstimate < desiredScanAngle + 10 && yawEstimate > desiredScanAngle - 10){// Ele cai quando chega no IDLE, mas so quando roda pra direita?
                currentState = IDLE;
            }
            break;
        case LAND:
            vTaskDelay(M2T(50));
            DEBUG_PRINT("Landing\n");
            if(down_half == false){
                setVelocitySetPoint(&setpoint, 0, 0, DESIRED_HEIGHT/2 );
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(30));
                float zEstimate = logGetFloat(idZEstimate);
                if (ABS(zEstimate - DESIRED_HEIGHT/2) <= TOL){
                down_half = true;
                }
            }
            else if(down_quarter == false){
                setVelocitySetPoint(&setpoint, 0, 0, DESIRED_HEIGHT/4);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(30));
                float zEstimate = logGetFloat(idZEstimate);
                if (ABS(zEstimate - DESIRED_HEIGHT/4) <= TOL){
                    down_quarter = true;
                }
            }
            else{
                setVelocitySetPoint(&setpoint, 0,0,0);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(50));
                turnOff = true;
            }
            break;
        case IDLE:
            vTaskDelay(M2T(10));
            setVelocitySetPoint(&setpoint, 0, 0, DESIRED_HEIGHT);
            commanderSetSetpoint(&setpoint, 3);
            vTaskDelay(M2T(300));
            idleCounter++;
            DEBUG_PRINT("Idle\n");
            if (idleCounter >= 20){
                currentState = LAND;
            }
            break;
        }


    }
}
#endif
