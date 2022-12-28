
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "app.h"
#include "commander.h"
#include "FreeRTOS.h"
#include "log.h"
#include "param.h"
#include <math.h>
#include "usec_time.h"
#include "crazyflie_controller.h"
#define TOL 0.5
#define SIZE 4.0
#define HEIGHT 2.0
#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)
#define ABS(a)    ((a>0.0f) ? a:-a)


setpoint    
static void setYawSetpoint(setpoint_t *setpoint, float vx, float vy, float z, float yawrate)
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

//States 
typedef enum{
    forward,
    back,
    rotateAdjust,
    rotateScan,
    rotateObstacle
} CrazyflieSM;

static CrazyflieSM state = forward;

void ControllerInit(float maxForwardSpeed_ref, StateLoop initState)
{
  maxForwardSpeed = maxForwardSpeed_ref;
  firstRun = true;
  stateWF = initState;
}


float maxForwardSpeed = 0.5f;

float cmdVelX = 0.0f;
float cmdVelY = 0.0f;
float cmdYaw = 0.0f;



void appMain()
{
  vTaskDelay(M2T(200));
  // Getting variable ids
  logVarId_t idxpose = logGetVarId("stateEstimate", "x");
  logVarId_t idypose = logGetVarId("stateEStimate", "y");       
  logVarId_t idzpose = logGetVarId("stateEstimate", "z");
  logVarId_t idxvelocity = logGetVarId("stateEstimate", "vx");
  logVarId_t idyvelocity = logGetVarId("stateEstimate", "vy");
  logVarId_t idyaw = logGetVarId("stabilizer", "yaw");
  logVarId_t idpitch = logGetVarId("stabilizer", "pitch");
  logVarId_t idroll = logGetVarId("stabilizer", "roll");
  




   setpoint_t setpoint;


}


static void CrazyflieSM transition(StateWF newState)
{
    return newState;
}



CrazyflieSM Controller(float xpose, float ypose, float yaw, float distance_to_obstacle){
    switch(state){
        case forward:
            cmdVelX = maxForwardSpeed;
            if(ABS(xpose)-TOL || ABS(ypose)>SIZE-TOL ){
                state = transition(rotateAdjust);
            }
            break;
        case back:
            



    }
    setVelocitySetpoint(&setpoint, cmdVelX, cmdVelY, HEIGHT, cmdAngWDeg);
    commanderSetSetpoint(&setpoint, 3);


                     }


//first we fix a height - pegar do Igor

while(1){
    float estYawDeg = logGetFloat(idyaw);
    float xpose = logGetFloat(idxpose);
    float ypose = logGetFloat(idypose);
    Controller(xpose, ypose, estYawDeg, cmdHeight);        //state machine will control our drone in every timestep


    
    vTaskDelay(M2T(10));
    
}