#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "app.h"

#include "commander.h"

#include "FreeRTOS.h"
#include "task.h"

#include "log.h"
#include "param.h"


#define DEBUG_MODULE "TEST"
#include "debug.h"
#define TOL 0.05f
#define ABS(a) ((a>0.0f?a:-a))
#define MAX_VEL 1
static setpoint_t setpoint;
static void setVelocitySetpoint(setpoint_t *setpoint, float vx, float vy, float vz, float yawrate)
{
  setpoint->mode.z = modeVelocity;
  setpoint->position.z = vz;


  setpoint->mode.yaw = modeVelocity;
  setpoint->attitudeRate.yaw = yawrate;


  setpoint->mode.x = modeVelocity;
  setpoint->mode.y = modeVelocity;
  setpoint->position.x = vx;
  setpoint->position.y = vy;

  setpoint->velocity_body = true;
}


static void Proportional(float actual_x, float actual_y, float actual_z, float desired_x, float desired_y, float desired_z){
  setVelocitySetpoint(&setpoint, desired_x - actual_x, desired_y - actual_y, desired_z - actual_z, 0);
  commanderSetSetpoint(&setpoint, 3);
}
static bool reached(float actual_x, float actual_y, float actual_z, float desired_x, float desired_y, float desired_z){
    if(ABS(desired_x - actual_x) <= TOL && ABS(desired_y - actual_y)<=TOL && ABS(desired_y-actual_y) <= TOL){
        return 1;
    }
    return 0;
}


void appMain() {
    paramVarId_t idPositioningDeck = paramGetVarId("deck", "bcFlow2");
    logVarId_t idXEstimate = logGetVarId("stateEstimate", "x");
    logVarId_t idYEstimate = logGetVarId("stateEstimate", "y");
    logVarId_t idZEstimate = logGetVarId("stateEstimate", "z");
    bool reached_pose = false;
    uint8_t positioningInit = paramGetUint(idPositioningDeck);
    if(positioningInit){
        memset(&setpoint, 0, sizeof(setpoint_t));
        commanderSetSetpoint(&setpoint, 3);
        while(reached_pose == false){
            float x_actual = logGetFloat(idXEstimate);
            float y_actual = logGetFloat(idYEstimate);
            float z_actual = logGetFloat(idZEstimate);
            Proportional(x_actual, y_actual, z_actual, 0, 0, 1);
            if (reached(x_actual, y_actual, z_actual, 0, 0, 1)){
                reached_pose = true;
            }
        }
        reached_pose = false;
        while(reached_pose  == false){
            float x_actual = logGetFloat(idXEstimate);
            float y_actual = logGetFloat(idYEstimate);
            float z_actual = logGetFloat(idZEstimate);
            Proportional(x_actual, y_actual, z_actual,0, 0, 0);
            if (reached(x_actual, y_actual, z_actual, 0, 0, 0)){
                reached_pose = true;
            }
        }
    }





}
