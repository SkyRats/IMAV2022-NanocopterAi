#include <stdbool.h>
#include <math.h>
#include "explorer.h"


/* main variables */
static float probOfCollision = 0.0f;
static float steeringAngle = 0.0f;
static float maxForwardSpeed = 0.2f;
static float maxTurnRate = 0.5f;
static float direction = 1.0f;
static float prevHeading = 0.0f;
static float stateStartTime;

static ExplorerState explorerState = IDLE;
static CfPosition crazyfliePos;
float timeNow = 0.0f;


ExplorerState explorerInit(CfPosition crazyflieInitPos, float maxForwardSpeed_ref, ExplorerState initState)
{
    crazyfliePos = crazyflieInitPos;
    maxForwardSpeed = maxForwardSpeed_ref;
    explorerState = TAKEOFF;
    return explorerState;
}
