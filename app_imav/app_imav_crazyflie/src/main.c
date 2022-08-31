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
#endif

#ifdef TEST_AI
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
#endif

void __attribute__((used)) DMA1_Stream1_IRQHandler(void)
{
    DMA_ClearFlag(DMA1_Stream1, UART3_RX_DMA_ALL_FLAGS);
    //ControllerState =
    dma_flag = 1;
}
