

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
#define TOL 0.1f
#define PI_OVER_4 0.785398f


static void setHoverSetpoint(setpoint_t *setpoint, float x, float y, float z, float yawrate){
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;

  //setpoint->mode.yaw = modeVelocity;
  setpoint->mode.yaw = modeAbs;

  //setpoint->attitudeRate.yaw = yawrate;
  setpoint->attitude.yaw = yawrate;


  setpoint->mode.x = modeAbs;
  setpoint->mode.y = modeAbs;
  setpoint->position.x = x;
  setpoint->position.y = y;

  setpoint->velocity_body = true;
}


//static void setYawSetpoint(setpoint_t *setpoint, float x, float y, float z, float yawrate)
//{
//  setpoint->mode.z = modeAbs;
//  setpoint->position.z = z;
//
//
//  setpoint->mode.yaw = modeVelocity;
//  setpoint->attitudeRate.yaw = yawrate;
//
//
//  setpoint->mode.x = modeAbs;
//  setpoint->mode.y = modeAbs;
//  setpoint->position.x = x;
//  setpoint->position.y = y;
//
//  setpoint->velocity_body = true;
//}


void appMain() {
  logVarId_t idZEstimate = logGetVarId("stateEstimate", "z");
  //paramVarId_t idMotorPowerSet = paramGetVarId("motorPowerSet", "motorPowerSet.enable");
  paramVarId_t idPositioningDeck = paramGetVarId("deck", "bcFlow2");
  //logVarId_t idStabilizerYaw = logGetVarId("stabilizer", "yaw");
  /*
  paramSetInt(idMotorPowerSet, 1);
  while(paramGetInt(idMotorPowerSet) >= 1){
    DEBUG_PRINT("Waiting for activation ...\n");
    vTaskDelay(M2T(200));
  }
  */
  static setpoint_t setpoint;

  vTaskDelay(M2T(2000));
  memset(&setpoint, 0, sizeof(setpoint_t));
  commanderSetSetpoint(&setpoint, 3);
  bool reached_height = false;
  bool down_half = false;
  bool down_quarter = false;
  int counter = 0;

  //ASCENDING
  while (reached_height == false){
    uint8_t positioningInit = paramGetUint(idPositioningDeck);
    vTaskDelay(M2T(10));
    if (positioningInit)
    {
      float zEstimate = logGetFloat(idZEstimate);
      vTaskDelay(M2T(10));
      setHoverSetpoint(&setpoint, 0, 0, 1.0, 0);
      commanderSetSetpoint(&setpoint, 3);
      vTaskDelay(M2T(50));
      DEBUG_PRINT("Decolou?\n");
      if(ABS(zEstimate - 1.0f) <= TOL){
        reached_height = true;
      }
    }

  }
//MOVING HORIZONTALLY
  while (reached_height == true &&  counter <= 400){
    vTaskDelay(M2T(10));
    setHoverSetpoint(&setpoint, 1, 1, 1, 0);
    commanderSetSetpoint(&setpoint, 3);
    vTaskDelay(M2T(25));
    counter++;
    DEBUG_PRINT("Voou?\n");

 }

 //ROTATING AFTER MOVING
  //while (reached_height == true && counter <=400){
  //  vTaskDelay(M2T(30));
  //  setYawSetpoint(&setpoint, 1, 1, 1, 0.5);
  //  commanderSetSetpoint(&setp  oint, 3);
  //  vTaskDelay(M2T(30));
  //  DEBUG_PRINT("Girou?\n");
  //  counter++;
  //}
//
/*
//ROTATING FOR A PERIOD
while (reached_height == true && counter <=100){
    vTaskDelay(M2T(30));
    setHoverSetpoint(&setpoint, 0, 0, 1, 1);
    commanderSetSetpoint(&setpoint, 3);
    vTaskDelay(M2T(30));
    DEBUG_PRINT("Girou?\n");
    counter++;
}
*/

//ROTATING TIL AN ANGLE
//float initial_yaw = logGetFloat(idStabilizerYaw);
//while(reached_height == true && logGetFloat(idStabilizerYaw)<= initial_yaw + (float)PI_OVER_4){
//    //vTaskDelay(M2T(30));
//    setYawSetpoint(&setpoint, 0, 0, 1, 0.5);
//    commanderSetSetpoint(&setpoint, 3);
//}


//DESCENDING
 while(reached_height == true){
   vTaskDelay(M2T(50));
   if(down_half == false){
     DEBUG_PRINT("Chegou na Metade?\n");
     setHoverSetpoint(&setpoint, 1, 1, 0.5, 0);

     commanderSetSetpoint(&setpoint, 3);
     vTaskDelay(M2T(30));
     float zEstimate = logGetFloat(idZEstimate);
     if (ABS(zEstimate - 0.5f) <= TOL){
       down_half = true;
     }
   }
   else if(down_quarter == false){
     setHoverSetpoint(&setpoint, 1, 1, 0.25, 0);
     commanderSetSetpoint(&setpoint, 3);
     vTaskDelay(M2T(30));
     float zEstimate = logGetFloat(idZEstimate);
     if (ABS(zEstimate - 0.25f) <= TOL){
       down_quarter = true;
     }
     DEBUG_PRINT("Chegou no um quarto?\n");
   }
   else{
    setHoverSetpoint(&setpoint, 1,1,0,0);
    commanderSetSetpoint(&setpoint, 3);
    vTaskDelay(M2T(50));
    DEBUG_PRINT("Pousou?\n");
    reached_height = false;
   /*
   if (ABS(idZEstimate - 0.1f) <= TOL){
      DEBUG_PRINT("Pousou?\n");
      paramSetInt(idMotorPowerSet, 1);
     }
  */
  }
 }
}
