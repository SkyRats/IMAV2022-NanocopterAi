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
#include "uart_dma_setup.h"

#define DEBUG_MODULE "EXPLORER"
#define BUFFERSIZE 8

#define TEST_COM
//#define TEST_AI
#define DEBUG_ON

int32_t aideckRxBuffer[BUFFERSIZE/4]; /* 8 bytes of data == 2 32-bit integers */
volatile uint8_t dma_flag = 0;

#ifdef TEST_COM
uint32_t squareX;
uint32_t squareY;

void appMain()
{
  vTaskDelay(M2T(3000));
  DEBUG_PRINT("Gate detection starting!\n");
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
  		  DEBUG_PRINT("output 1: %ld\n", squareX>>24);
  		  DEBUG_PRINT("output 2: %ld\n\n", squareY>>24);
          #endif

          memset(aideckRxBuffer, 0, BUFFERSIZE);
          squareX = 0;
          squareY = 0;
  	}
  }
}
#endif

void __attribute__((used)) DMA1_Stream1_IRQHandler(void)
{
    DMA_ClearFlag(DMA1_Stream1, UART3_RX_DMA_ALL_FLAGS);
    dma_flag = 1;
}
