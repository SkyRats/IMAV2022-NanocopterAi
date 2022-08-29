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
#include "uart_dma_setup.h"
#include "log.h"
#include "param.h"

#define DEBUG_MODULE "EXPLORER"
#define BUFFERSIZE 8

#define TEST_COM
#define DEBUG_ON


// States
typedef enum
{
    WAITING_FOR_FIRST_MSG, // IDLE, HOVER, TAKEOFF
    EXPLORING, // GO_FORWARD, TURN_BACK_TO_ARENA, CHANGE_HEADING
    SEARCHING_FOR_GATE, // FIND_GATE
} ControllerState;

int32_t aideckRxBuffer[BUFFERSIZE/4]; /* 8 bytes of data == 2 32-bit integers */
volatile uint8_t dma_flag = 0;

#ifndef TEST_COM

static void setVelocitySetpoint(setpoint_t *setpoint, float vx, float vy, float z, float yawrate);
#include "explorer.h"
uint32_t squareX = 0;
uint32_t squareY = 0;

float maxForwardSpeed = 0.5f;

float cmdVelX = 0.0f;
float cmdVelY = 0.0f;
float cmdAngWRad = 0.0f;
float cmdAngWDeg = 0.0f;

bool reinit = false;

ControllerState controllerState = WAITING_FOR_FIRST_MSG;
ExplorerState explorerState;

void appMain()
{
	DEBUG_PRINT("Explorer started!\n");
	USART_DMA_Start(115200, (int8_t *)aideckRxBuffer, BUFFERSIZE);
    vTaskDelay(M2T(3000));

    // Getting the Logging IDs of the state estimates
    logVarId_t idXEstimate = logGetVarId("stateEstimate", "x");
    logVarId_t idYEstimate = logGetVarId("stateEstimate", "y");
    logVarId_t idZEstimate = logGetVarId("stateEstimate", "z");
    logVarId_t idStabilizerYaw = logGetVarId("stabilizer", "yaw");

    // Initialize crazyflie position struct
    CfPosition crazyfliePosition = {.x = logGetFloat(idXEstimate), .y = logGetFloat(idYEstimate)};

    // Getting Param IDs of the deck driver initialization
    paramVarId_t idPositioningDeck = paramGetVarId("deck", "bcFlow2");
    paramVarId_t idAIDeck = paramGetVarId("deck", "bcAI");

    // initialize state machine
    explorerState = explorerInit(crazyfliePosition, maxForwardSpeed, TAKEOFF);

    // Intialize the setpoint structure
    setpoint_t setpoint;

	while(1)
    {
		vTaskDelay(M2T(10));

        // initialize state machine
        if(reinit)
            explorerState = explorerInit(crazyfliePosition, maxForwardSpeed, TAKEOFF);

        // Check if decks are properly mounted
        uint8_t positioningInit = paramGetUint(idPositioningDeck);
        uint8_t aiDeckInit = paramGetUint(idAIDeck);

        //Get position esimate
        float xEstimate = logGetFloat(idXEstimate);
        float yEstimate = logGetFloat(idYEstimate);

        // Get Height estimate
        float heightEstimate = logGetFloat(idHeightEstimate);

		if (dma_flag == 1)
		{
		    dma_flag  = 0;  // clear the flag

            int32_t squareX = aideckRxBuffer[0]; /* load pulp-dronet-v2 NEMO/DORY results */
            int32_t squareY = aideckRxBuffer[1];

            #ifdef DEBUG_ON
			DEBUG_PRINT("output 1: %u\n", squareX);
			DEBUG_PRINT("output 2: %u\n", squareY);
            #endif

            switch(ControllerState)
            {
                case WAITING_FOR_FIRST_MSG:
                    break;
                case EXPLORING:
                    break;
                case TRAVERSING_GATE:
                    break;
                default:
                    DEBUG_PRINT("Controller state does not exist!\n");
                    break;
            }


			memset(aideckRxBuffer, 0, BUFFERSIZE);
		}


	}
}
#endif

#ifdef TEST_COM
int32_t squareX;
int32_t squareY;

void appMain()
{
	DEBUG_PRINT("Gate detection started!\n");
    vTaskDelay(M2T(3000));
	USART_DMA_Start(115200, (int8_t *)aideckRxBuffer, BUFFERSIZE);

	while(1) {
		vTaskDelay(M2T(100));

		if (dma_flag == 1)
		{
			dma_flag = 0;  // clear the flag
            squareX = aideckRxBuffer[0];
            squareY = aideckRxBuffer[1];

            #ifdef DEBUG_ON
			DEBUG_PRINT("output 1: %ld\n", squareX);
			DEBUG_PRINT("output 2: %ld\n\n", squareY);
            #endif

            // maybe remove this?
            aideckRxBuffer[0] = aideckRxBuffer[1] = 0;
            squareX = squareY = 0;
		}
	}
}
#endif

//static void setVelocitySetpoint(setpoint_t *setpoint, float vx, float vy, float z, float yawrate)
//{
  //setpoint->mode.z = modeAbs;
  //setpoint->position.z = z;
  //setpoint->mode.yaw = modeVelocity;
  //setpoint->attitudeRate.yaw = yawrate;
  //setpoint->mode.x = modeVelocity;
  //setpoint->mode.y = modeVelocity;
  //setpoint->velocity.x = vx;
  //setpoint->velocity.y = vy;
//
  //setpoint->velocity_body = true;
//}


void __attribute__((used)) DMA1_Stream1_IRQHandler(void)
{
    DMA_ClearFlag(DMA1_Stream1, UART3_RX_DMA_ALL_FLAGS);
    //ControllerState =
    dma_flag = 1;
}
