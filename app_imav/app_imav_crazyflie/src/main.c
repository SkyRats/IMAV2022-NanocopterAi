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
#define DESIRED_HEIGHT 1.0f
#define FORWARD_VEL 0.1f
#define ROTATION_VEL 15
#define TOL 0.1f
#define ARENA_EDGES 0.5f

#define ABS(a) ((a>0.0f?a:-a))
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

float edgeFinder(float x, float y, float yaw){
    if (x >= ARENA_EDGES){
        if (y >= ARENA_EDGES){
            rotationDir = yaw >= 45 ? 1: -1;
            return yaw >= 45 ? 180: 270;
        }
        else if (y < -ARENA_EDGES){
            rotationDir = yaw >= 315 ? 1: -1 ;
            return yaw >= 315 ? 90: 180;
        }
        else{
            rotationDir = (yaw >= 0 && yaw < 90)? 1 : -1;
            return (yaw >= 0 && yaw < 90)? 90 : 270;
        }
    }
    else if (x < -ARENA_EDGES){
       if (y >= ARENA_EDGES){
            rotationDir = yaw >= 120? 1 : -1;
            return yaw >= 120 ? 270: 0;
        }
        else if (y < -ARENA_EDGES){
            rotationDir = yaw >= 225? 1 : -1;
            return yaw >= 225 ? 0: 90;
        }
        else{
            rotationDir = yaw >= 180? 1 : -1;
            return yaw >= 180? 270 : 90;
        } 
    }
    else if (y >= ARENA_EDGES){
        rotationDir = yaw >= 90? 1 : -1;
        return yaw >= 90? 180 : 0;
    }
    else{
        rotationDir = yaw >= 270? 1 : -1;
        return yaw >= 270? 0: 180;
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
  setpoint->mode.x = modeAbs;
  setpoint->mode.y = modeAbs;
  setpoint->position.x = x;
  setpoint->position.y = y;
  setpoint->velocity_body = true;
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
    int takeoffCounter = 0;
    float idleX = 0;
    float idleY = 0;
    memset(&setpoint, 0, sizeof(setpoint_t));
    commanderSetSetpoint(&setpoint, 3);

    vTaskDelay(M2T(3000));//After every task, especially those relatig to movement, an appropriate delay must be set
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
            if(zEstimate >= DESIRED_HEIGHT - TOL && takeoffCounter >= 5){
                currentState = FORWARD;
            }
            break;
        case FORWARD: //Goes forward until it reaches arena edges.
            if(xEstimate <= ARENA_EDGES && xEstimate >= -ARENA_EDGES && yEstimate <= ARENA_EDGES && yEstimate >= -ARENA_EDGES){
                vTaskDelay(M2T(10));
                setVelocitySetPoint(&setpoint, FORWARD_VEL, 0, DESIRED_HEIGHT);
                commanderSetSetpoint(&setpoint, 3);
                vTaskDelay(M2T(100));
            }
            else{
                edgeRotation = edgeFinder(xEstimate, yEstimate, yawEstimate);
                currentState = LOOK_INWARDS;
            }
            break;
        case LOOK_INWARDS:
            vTaskDelay(M2T(10));
            setAttitudeSetpoint(&setpoint, xEstimate, yEstimate, DESIRED_HEIGHT, edgeRotation);
            commanderSetSetpoint(&setpoint, 3);
            vTaskDelay(M2T(100));
            yawEstimate = logGetFloat(idStabilizerYaw);
            vTaskDelay(M2T(10));
            if (ABS(yawEstimate - edgeRotation) <= 10 ){
                if((xEstimate >= ARENA_EDGES || xEstimate <= -ARENA_EDGES) && (yEstimate >= ARENA_EDGES || yEstimate <= -ARENA_EDGES)){
                    if (rotationDir > 0){
                        desiredScanAngle = (yawEstimate + 90) >= 360? (yawEstimate + 90) -360 : yawEstimate + 90;
                    }
                    else{
                        desiredScanAngle = (yawEstimate - 90) < 0? (yawEstimate - 90)  + 360 : yawEstimate - 90;
                    }
                }
                else{
                    if (rotationDir > 0){
                        desiredScanAngle = (yawEstimate + 180) >= 360? (yawEstimate + 180) -360 : yawEstimate + 180;
                    }
                    else{
                        desiredScanAngle = (yawEstimate - 180) < 0? (yawEstimate - 180)  + 360 : yawEstimate - 180;
                    }
                }
                currentState = FIND_GATE;
                idleX = xEstimate; idleY = yEstimate;
            }
            break;
        case FIND_GATE:
            vTaskDelay(M2T(10));
            setAttitudeRateSetpoint(&setpoint, idleX, idleY, DESIRED_HEIGHT, rotationDir*ROTATION_VEL);
            commanderSetSetpoint(&setpoint, 3);
            vTaskDelay(M2T(100));
            yawEstimate = logGetFloat(idStabilizerYaw);
            vTaskDelay(M2T(30));
            if(yawEstimate > desiredScanAngle - 10 && yawEstimate < desiredScanAngle + 10){
                currentState = IDLE;
            }
            break;
        case LAND:
            vTaskDelay(M2T(50));
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
            if (idleCounter >= 20){
                currentState = LAND;
            }
            break;
        }


    }
}