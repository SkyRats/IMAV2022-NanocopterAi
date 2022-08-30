
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "app.h"
#include "commander.h"
#include "FreeRTOS.h"
#include "task.h"
#include "log.h"
#include "param.h"
#define TOL 0.1f
#define DEBUG_MODULE "TEST"
#include "debug.h"
#define ABS(a) ((a>0.0f?a:-a))



static void setPositionSetPoint(setpoint_t *setpoint, float x, float y, float z, float yaw){//Absolute xyz, no rotation
  setpoint->mode.yaw = modeAbs;
  setpoint->attitude.yaw = yaw;
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;
  setpoint->mode.x = modeAbs;
  setpoint->mode.y = modeAbs;
  setpoint->position.x = x;
  setpoint->position.y = y;
  setpoint->velocity_body = false;
}

static void setVelocitySetPoint(setpoint_t *setpoint, float x, float y, float z, float yaw){//xy velocity, absolute z, no rotation
  setpoint->mode.yaw = modeAbs;
  setpoint->attitude.yaw = yaw;
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;
  setpoint->mode.x = modeVelocity;
  setpoint->mode.y = modeVelocity;
  setpoint->velocity.x = x;
  setpoint->velocity.y = y;
  setpoint->velocity_body = true;

}

/*
static void setAttitudeSetpoint(setpoint_t *setpoint,float x, float y, float z, float yaw){//Absolute rotation
  setpoint->mode.yaw = modeAbs;
  setpoint->attitude.yaw = yaw;
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;
  setpoint->mode.x = modeAbs;
  setpoint->mode.y = modeAbs;
  setpoint->position.x = x;
  setpoint->position.y = y;
  setpoint->velocity_body = false;
}
*/

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

void appMain() {
  logVarId_t idXEstimate = logGetVarId("stateEstimate", "x");
  logVarId_t idYEstimate = logGetVarId("stateEstimate", "y");
  logVarId_t idZEstimate = logGetVarId("stateEstimate", "z");
  paramVarId_t idPositioningDeck = paramGetVarId("deck", "bcFlow2");
  logVarId_t idStabilizerYaw = logGetVarId("stabilizer", "yaw");
  static setpoint_t setpoint;
  uint8_t positioningInit = paramGetUint(idPositioningDeck);
  vTaskDelay(M2T(4000));
  memset(&setpoint, 0, sizeof(setpoint_t));
  commanderSetSetpoint(&setpoint, 3);
  
  bool reached_height = false;
  bool down_half = false;
  bool down_quarter = false;

  if(positioningInit){
  
    //ASCENDING
    while (reached_height == false){
      float zEstimate = logGetFloat(idZEstimate);
      vTaskDelay(M2T(10));
      setPositionSetPoint(&setpoint, 0, 0, 1, 0);
      commanderSetSetpoint(&setpoint, 3);
      vTaskDelay(M2T(100));
      DEBUG_PRINT("Decolou?\n");
      zEstimate = logGetFloat(idZEstimate);
      vTaskDelay(M2T(10));
      if(zEstimate >= 1.0f - TOL){
        reached_height = true;
      }
    }
    
    //ROTATING 
    while(logGetFloat(idStabilizerYaw) >= 90 + 10 || logGetFloat(idStabilizerYaw) <= 90 - 10 ){// TODO: Rotation dir selector like on the simulation.
        vTaskDelay(M2T(30));
        setAttitudeRateSetpoint(&setpoint,0,0,1,15);//degree/s
        commanderSetSetpoint(&setpoint, 3);
        vTaskDelay(M2T(100));
        DEBUG_PRINT("Girou?\n");
    }
    

    //MOVING HORIZONTALLY
    while (logGetFloat(idXEstimate) <= 0.5f && logGetFloat(idXEstimate) >= -0.5f && logGetFloat(idYEstimate) <= 0.5f && logGetFloat(idYEstimate) >= -0.5f){
      vTaskDelay(M2T(10));
      //setPositionSetpoint(&setpoint, 0.5, 0.5, 1, 0);
      setVelocitySetPoint(&setpoint, 0.1, 0, 1, 90);
      commanderSetSetpoint(&setpoint, 3);
      vTaskDelay(M2T(100));
      DEBUG_PRINT("Voou?\n");
    }
    
    //DESCENDING 
    while(reached_height == true){
      vTaskDelay(M2T(50));
      if(down_half == false){
        DEBUG_PRINT("Chegou na Metade?\n");
        setVelocitySetPoint(&setpoint, 0, 0, 0.5, 90);
        commanderSetSetpoint(&setpoint, 3);
        vTaskDelay(M2T(30));
        float zEstimate = logGetFloat(idZEstimate);
        if (ABS(zEstimate - 0.5f) <= TOL){
          down_half = true;
        }
      }
      else if(down_quarter == false){
        setVelocitySetPoint(&setpoint, 0, 0, 0.25, 90);
        commanderSetSetpoint(&setpoint, 3);
        vTaskDelay(M2T(30));
        float zEstimate = logGetFloat(idZEstimate);
        if (ABS(zEstimate - 0.25f) <= TOL){
          down_quarter = true;
        }
        DEBUG_PRINT("Chegou no um quarto?\n");
      }
      else{
        setVelocitySetPoint(&setpoint, 0,0,0, 90);
        commanderSetSetpoint(&setpoint, 3);
        vTaskDelay(M2T(50));
        DEBUG_PRINT("Pousou?\n");
        reached_height = false;
      }

    }
  } 
}
