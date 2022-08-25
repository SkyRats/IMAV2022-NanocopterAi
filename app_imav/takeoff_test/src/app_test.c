

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

static void setHoverSetpoint(setpoint_t *setpoint, float vx, float vy, float z, float yawrate)
{
  setpoint->mode.z = modeAbs;
  setpoint->position.z = z;


  setpoint->mode.yaw = modeVelocity;
  setpoint->attitudeRate.yaw = yawrate;


  setpoint->mode.x = modeVelocity;
  setpoint->mode.y = modeVelocity;
  setpoint->velocity.x = vx;
  setpoint->velocity.y = vy;

  setpoint->velocity_body = true;
}


void appMain() {
  DEBUG_PRINT("Waiting for activation ...\n");
  static setpoint_t setpoint;
  vTaskDelay(M2T(2000));
  memset(&setpoint, 0, sizeof(setpoint_t));
  commanderSetSetpoint(&setpoint, 3);
  
  while (1)
  {
    vTaskDelay(M2T(10));
    setHoverSetpoint(&setpoint, 0, 0, 0.3, 0);
    commanderSetSetpoint(&setpoint, 3);
    DEBUG_PRINT("Voou?\n");
  }
}
