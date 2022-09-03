/*
 * test_template.c
 * Alessio Burrello <alessio.burrello@unibo.it>
 *
 * Copyright (C) 2019-2020 University of Bologna
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <stdint.h>

#include "network.h"
#include "pmsis.h"
#include "stdio.h"
#include "gateFinder.h"

/* Autotiler includes. */
#include "Gap8.h"

/* Gap_lib includes. */
#include "gaplib/ImgIO.h"

#include "imageIO.h"
#include "bsp/bsp.h"
#include "bsp/camera.h"
#include "bsp/camera/himax.h"

#define STACK_SIZE      2048
#define NULL_CHECK(val)                                  \
    if (val == NULL)                                     \
    {                                                    \
        printf("Allocation of memory failed. Exiting."); \
        exit(-1);                                        \
    }

/* Variables used. */
int32_t * dronetOutput;
int32_t toSend[4];

uint8_t * run_gateFinder(struct pi_device *);

void test_uart_dronet(void)
{
    printf("Entering main controller\n");

    uint32_t errors = 0;
    struct pi_device uart;
    struct pi_uart_conf conf;
    static struct pi_device * camera_dev;

    /* Init & open uart. */
    pi_uart_conf_init(&conf);
    conf.enable_tx = 1;
    conf.enable_rx = 0;
    conf.baudrate_bps = 115200;
    pi_open_from_conf(&uart, &conf);
    if (pi_uart_open(&uart))
    {
        printf("Uart open failed !\n");
        pmsis_exit(-1);
    }
    dronetOutput = network_setup();

    while(1)
    {
      struct pi_device * camera_dev = network_run_FabricController();
      toSend[0] = dronetOutput[0];
      toSend[1] = dronetOutput[1];

      if(((uint32_t)dronetOutput[1])/4294967295.0f < 0.8f)
          run_gateFinder(camera_dev);

      pi_uart_write(&uart, toSend, 16);
      memset(toSend, 0, 4*sizeof(uint32_t));
    }

    pi_uart_close(&uart);

    pmsis_exit(errors);
}

uint8_t * run_gateFinder(struct pi_device * camera_dev)
{
      PGMImage * originalImage = pmsis_l2_malloc(sizeof(PGMImage));
      NULL_CHECK(originalImage);
      printf("originalImage: %p\n", originalImage);
      originalImage->x = 200;
      originalImage->y = 200;
      originalImage->data = pmsis_l2_malloc(40000*sizeof(uint8_t));
      NULL_CHECK(originalImage->data);

      printf("Acquiring image\n");

      pi_camera_control(camera_dev, PI_CAMERA_CMD_START, 0);
      pi_camera_capture(camera_dev, originalImage->data , 40000);
      pi_camera_control(camera_dev, PI_CAMERA_CMD_STOP, 0);

      PGMImage * outputImage = pmsis_l2_malloc(sizeof(PGMImage));
      NULL_CHECK(outputImage);
      outputImage->x = 200;
      outputImage->y = 200;
      outputImage->data = pmsis_l2_malloc(40000*sizeof(uint8_t));
      NULL_CHECK(outputImage->data);

      PGMImage * args[2];
      args[0] = originalImage;
      args[1] = outputImage;

      /* opening and configuring cluster */
      printf("Calling cluster.\n");
      // Activate the Cluster
      struct pi_device cluster_dev;
      struct pi_cluster_conf cl_conf;
      cl_conf.id = 0;
      pi_open_from_conf(&cluster_dev, (void *) &cl_conf);
      if (pi_cluster_open(&cluster_dev))
      {
          printf("Cluster open failed !\n");
          pmsis_exit(-1);
      }

      struct pi_cluster_task *task = pmsis_l2_malloc(sizeof(struct pi_cluster_task));
      NULL_CHECK(task);
      memset(task, 0, sizeof(struct pi_cluster_task));
      task->entry = masterFindGate;
      task->arg = (void *)args;
      task->stack_size = (uint32_t) STACK_SIZE;

      /* Synchronous call to cluster, execution will start only on Master Core (core 0) */
      pi_cluster_send_task_to_cl(&cluster_dev, task);
      toSend[2] = outputImage->data[0]; /* x position */
      toSend[3] = outputImage->data[1]; /* y position */

      pmsis_l2_malloc_free(task, sizeof(struct pi_cluster_task));
      pmsis_l2_malloc_free(originalImage->data, 40000*sizeof(uint8_t));
      pmsis_l2_malloc_free(originalImage, sizeof(PGMImage));

      pmsis_l2_malloc_free(outputImage->data, 40000*sizeof(uint8_t));
      pmsis_l2_malloc_free(outputImage, sizeof(PGMImage));
}

/* Program Entry. */
int main(void)
{
    printf("\n\n\t *** PMSIS Uart dronet test ***\n\n");
    return pmsis_kickoff((void *) test_uart_dronet);
}
