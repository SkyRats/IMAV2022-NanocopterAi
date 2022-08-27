#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "app.h"

#include "commander.h"

#include "FreeRTOS.h"
#include "task.h"

#include "debug.h"

#include "log.h"
#include "param.h"

#define DEBUG_MODULE "TEST_CRAZYFLIE"
#define AUTONOMOUS_TEST_FLIGHT
#define TOL 0.1f
#define MAX(a,b) ((a>b)?a:b)
#define MIN(a,b) ((a<b)?a:b)
#define ABS(a) ((a>0.0f?a:-a))

static const float velMax = 1.0f;

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

#ifdef AUTONOMOUS_TEST_FLIGHT
void appMain()
{
    vTaskDelay(M2T(3000));
    static setpoint_t setpoint;

    //logVarId_t idXEstimate = logGetVarId("stateEstimate", "x");
    //logVarId_t idYEstimate = logGetVarId("stateEstimate", "y");
    logVarId_t idZEstimate = logGetVarId("stateEstimate", "z");
    //logVarId_t idStabilizerYaw = logGetVarId("stabilizer", "yaw");

    vTaskDelay(M2T(10));
    //Get position esimate
    //float xEstimate = logGetFloat(idXEstimate);
    //float yEstimate = logGetFloat(idYEstimate);

    // Get Height estimate
    float heightEstimate = logGetFloat(idZEstimate);
    //setHoverSetpoint(&setpoint, 0, 0, 1.0f, 0);

    while(ABS(heightEstimate - 1.0f) > TOL)
    {
        setHoverSetpoint(&setpoint, 0, 0, 1.0f, 0);
        commanderSetSetpoint(&setpoint, 3);
        heightEstimate = logGetFloat(idZEstimate);
    }
    vTaskDelay(M2T(5000));

    memset(&setpoint, 0, sizeof(setpoint_t));
    while(ABS(heightEstimate) > TOL)
    {
        setHoverSetpoint(&setpoint, 0, 0, 0.0f, 0);
        commanderSetSetpoint(&setpoint, 3);
        heightEstimate = logGetFloat(idZEstimate);
    }
    vTaskDelay(M2T(5000));

    while(1)
    {
        setHoverSetpoint(&setpoint, 0, 0, 0.0f, 0);
        commanderSetSetpoint(&setpoint, 3);
    }
}
#endif

