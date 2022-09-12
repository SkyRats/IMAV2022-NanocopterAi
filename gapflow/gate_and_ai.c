
/*-----------------------------------------------------------------------------
 Copyright (C) 2020-2021 ETH Zurich, Switzerland, University of Bologna, Italy.
 All rights reserved.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 See LICENSE.apache.md in the top directory for details.
 You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 File:    main.c
 Authors: Vlad Niculescu   	<vladn@iis.ee.ethz.ch>
          Lorenzo Lamberti 	<lorenzo.lamberti@unibo.it>
          Daniele Palossi  <dpalossi@iis.ee.ethz.ch> <daniele.palossi@idsia.ch>
 Date:    15.03.2021
-------------------------------------------------------------------------------*/


#include "pmsis.h"
#include "bsp/flash/hyperflash.h"
#include "bsp/bsp.h"
#include "bsp/buffer.h"
#include "bsp/camera/himax.h"
#include "bsp/ram.h"
#include "bsp/ram/hyperram.h"
#include "bsp/display/ili9341.h"
#include "bsp/fs.h"
#include "bsp/flash/hyperflash.h"
#include "bsp/fs/readfs.h"

#include "config.h"
#include "main.h"
#include "networkKernels.h"
#include "imageIO.h"
#include "gateFinder.h"

uint8_t toSend[4];


/* Defines */
#define NUM_CLASSES 	2
#define AT_INPUT_SIZE 	(AT_INPUT_WIDTH*AT_INPUT_HEIGHT*AT_INPUT_COLORS)

#define __XSTR(__s) __STR(__s)
#define __STR(__s) #__s
#define NULL_CHECK(val)                                  \
    if (val == NULL)                                     \
    {                                                    \
        printf("Allocation of memory failed. Exiting."); \
        exit(-1);                                        \
    }

void run_gateFinder();

typedef signed char NETWORK_OUT_TYPE;

// Global Variables
static volatile struct pi_device camera_dev;
static struct pi_device cluster_dev;
struct pi_device HyperRam;

L2_MEM NETWORK_OUT_TYPE *ResOut;
static uint32_t l3_buff;
AT_HYPERFLASH_FS_EXT_ADDR_TYPE AT_L3_ADDR = 0;

static volatile struct pi_himax_conf camera_conf;

static volatile uint8_t asyncImgTransfFlag = 1;
static uint8_t * Input_1;

static void handle_transfer_end(void *arg)
{
    pi_camera_control(&camera_dev, PI_CAMERA_CMD_STOP, 0);
    asyncImgTransfFlag = 1;
}

static int8_t open_camera()
{
    printf("initiating camera conf\n");
    pi_himax_conf_init(&camera_conf);

    camera_conf.format = PI_CAMERA_QVGA; /* 320 x 240 */
    camera_conf.roi.h = 200;
    camera_conf.roi.slice_en = 1;
    camera_conf.roi.w = 200;
    camera_conf.roi.x = 60; /* 320 / 2 - 100 */
    camera_conf.roi.y = 40; /* 240 - 200 */

    printf("opening from conf\n");
    pi_open_from_conf(&camera_dev, &camera_conf);
    if(pi_camera_open(&camera_dev))
    {
        printf("failed to open camera.\n");
        return -1;
    }

    /* rotate image -- image is upside down by default */
    uint8_t set_value = 3;
    uint8_t reg_value;
    printf("setting registers\n");
    pi_camera_reg_set(&camera_dev, IMG_ORIENTATION, &set_value);
    pi_camera_reg_get(&camera_dev, IMG_ORIENTATION, &reg_value);
    if(set_value != reg_value)
    {
        printf("failed to rotate image.\n");
        return -1;
    }
    pi_camera_control(&camera_dev, PI_CAMERA_CMD_AEG_INIT, 0);
    return 0;
}

static void RunNetwork()
{
  printf("Running on cluster\n");
  printf("Start timer\n");
  gap_cl_starttimer();
  gap_cl_resethwtimer();
  AT_CNN(l3_buff, &ResOut[0], &ResOut[1]);
  printf("Runner completed\n");
}


int body(void)
{
    pi_time_wait_us(7000000);
    // Voltage-Frequency settings
    uint32_t voltage =1200;
    pi_freq_set(PI_FREQ_DOMAIN_FC, FREQ_FC*1000*1000);
    pi_freq_set(PI_FREQ_DOMAIN_CL, FREQ_CL*1000*1000);
    PMU_set_voltage(voltage, 0);
    printf("Set VDD voltage as %.2f, FC Frequency as %d MHz, CL Frequency = %d MHz\n",
		(float)voltage/1000, FREQ_FC, FREQ_CL);

    pi_fs_file_t *file;
    struct pi_device fs;
    struct pi_device flash;
    struct pi_hyperflash_conf flash_conf;
    struct pi_readfs_conf conf0;

    pi_hyperflash_conf_init(&flash_conf);
    pi_open_from_conf(&flash, &flash_conf);
    if (pi_flash_open(&flash))
    {
        printf("Error flash open ! \n");
        pmsis_exit(-3);
    }

    // Open filesystem on flash
    pi_readfs_conf_init(&conf0);
    conf0.fs.flash = &flash;
    pi_open_from_conf(&fs, &conf0);
    if (pi_fs_mount(&fs))
    {
        printf("Error FS mounting ! \n");
        pmsis_exit(-2);
    }


	// Initialize the ram
  	struct pi_hyperram_conf hyper_conf;
  	pi_hyperram_conf_init(&hyper_conf);
  	pi_open_from_conf(&HyperRam, &hyper_conf);
	if (pi_ram_open(&HyperRam))
	{
		printf("Error ram open !\n");
		pmsis_exit(-3);
	}

	// Allocate L3 buffer to store input data
	if (pi_ram_alloc(&HyperRam, &l3_buff, (uint32_t) AT_INPUT_SIZE))
	{
		printf("Ram malloc failed !\n");
		pmsis_exit(-4);
	}

	// Allocate temp buffer for image data
	Input_1 = (uint8_t*) pmsis_l2_malloc(AT_INPUT_SIZE*sizeof(char));
	if(!Input_1){
		printf("Failed allocation!\n");
		pmsis_exit(1);
	}

    printf("opening camera\n");
    if(open_camera())
    {
        printf("Failed to open camera.\n");
        return NULL;
    }
    printf("initiating camera\n");
    pi_camera_control(&camera_dev, PI_CAMERA_CMD_START, 0);
    pi_camera_capture(&camera_dev, Input_1 , 40000);
    pi_camera_control(&camera_dev, PI_CAMERA_CMD_STOP, 0);

    struct pi_cluster_task *cl_task = pmsis_l2_malloc(sizeof(struct pi_cluster_task));
    if(cl_task==NULL) {
      printf("pi_cluster_task alloc Error!\n");
      pmsis_exit(-1);
    }
    printf("Stack size is %d and %d\n",STACK_SIZE,SLAVE_STACK_SIZE );
    // Task setup
    memset(cl_task, 0, sizeof(struct pi_cluster_task));
    cl_task->entry = &RunNetwork;
    cl_task->stack_size = STACK_SIZE;
    cl_task->slave_stack_size = SLAVE_STACK_SIZE;
    cl_task->arg = NULL;


    // Allocate the output tensor
    ResOut = (NETWORK_OUT_TYPE *) AT_L2_ALLOC(0, NUM_CLASSES*sizeof(NETWORK_OUT_TYPE));
    if (ResOut==0) {
        printf("Failed to allocate Memory for Result (%ld bytes)\n", 2*sizeof(char));
        return 1;
    }

    // Open the cluster
    struct pi_cluster_conf conf;
    pi_cluster_conf_init(&conf);
    pi_open_from_conf(&cluster_dev, (void *)&conf);
    pi_cluster_open(&cluster_dev);

    pi_task_t task;


    struct pi_device uart;
    struct pi_uart_conf uart_conf;
    /* Init & open uart. */
    pi_uart_conf_init(&uart_conf);
    uart_conf.baudrate_bps = 115200;
    uart_conf.enable_rx = 0;
    uart_conf.enable_tx = 1;
    pi_open_from_conf(&uart, &uart_conf);
    if (pi_uart_open(&uart))
    {
        printf("Uart open failed !\n");
        pmsis_exit(-1);
    }

	// Network Constructor
        int err_const = AT_CONSTRUCT();
        pi_time_wait_us(10000);
	    if (err_const)
	    {
		    printf("Graph constructor exited with error: %d\n", err_const);
	        return 1;
	    }
	    printf("Network Constructor was OK!\n");

    while(1)
        //if(asyncImgTransfFlag == 1)
    {
        while(asyncImgTransfFlag == 0)
            pi_yield();


        // Write greyscale image to RAM
        pi_ram_write(&HyperRam, (l3_buff), Input_1, (uint32_t) AT_INPUT_SIZE);

        asyncImgTransfFlag = 0;
        pi_camera_capture_async(&camera_dev, Input_1, 40000, pi_task_callback(&task, handle_transfer_end, NULL));
        pi_camera_control(&camera_dev, PI_CAMERA_CMD_START, 0);

        // Dispatch task on the cluster
        pi_cluster_send_task_to_cl(&cluster_dev, cl_task);

        // Task setup
        memset(cl_task, 0, sizeof(struct pi_cluster_task));
        cl_task->entry = &RunNetwork;
        cl_task->stack_size = STACK_SIZE;
        cl_task->slave_stack_size = SLAVE_STACK_SIZE;
        cl_task->arg = NULL;

        printf("Model:\t%s\n\n", __XSTR(AT_MODEL_PREFIX));
        double out1 = 0.2460539 * (double)ResOut[0];
        double out2 = 0.00787402 * (double)ResOut[1];

	    printf("ResOut[0]: %d\n", ResOut[0]);
	    printf("ResOut[1]: %d\n", ResOut[1]);

        printf("With quantization: \n");
        printf("Output 1:\t%.6f\n", out1);
        printf("Output 2:\t%.6f\n", out2);


        toSend[0] = ResOut[0];
        toSend[1] = ResOut[1];

        pi_time_wait_us(10000);

	if(out2 <= 0.8)
	{
		while(asyncImgTransfFlag != 1)
		    pi_yield();

		run_gateFinder();
	}
	else
	{
		toSend[2] = 0;
		toSend[3] = 0;
	}

	printf("toSend[0]: %d\ntoSend[1]: %d\ntoSend[2]: %d\ntoSend[3]: %d\n", toSend[0], toSend[1], toSend[2], toSend[3]);

        pi_time_wait_us(5000);
        pi_uart_write(&uart, (uint8_t*)toSend    , 1);
        pi_time_wait_us(10000);
        pi_uart_write(&uart, (((uint8_t*)toSend) + 1), 1);
        pi_time_wait_us(10000);
        pi_uart_write(&uart, (((uint8_t*)toSend) + 2), 1);
        pi_time_wait_us(10000);
        pi_uart_write(&uart, (((uint8_t*)toSend) + 3), 1);
        pi_time_wait_us(1000000);
    }

	// Netwrok Destructor
	AT_DESTRUCT();

    pi_uart_close(&uart);
    pmsis_exit(0);
    return 0;
}

void run_gateFinder()
{
      PGMImage * originalImage = pmsis_l2_malloc(sizeof(PGMImage));
      NULL_CHECK(originalImage);
      printf("originalImage: %p\n", originalImage);
      originalImage->x = 200;
      originalImage->y = 200;
      originalImage->data = Input_1;
      NULL_CHECK(originalImage->data);

      printf("Acquiring image\n");

      PGMImage * outputImage = pmsis_l2_malloc(sizeof(PGMImage));
      NULL_CHECK(outputImage);
      outputImage->x = 200;
      outputImage->y = 200;
      outputImage->data = pmsis_l2_malloc(40000*sizeof(uint8_t));
      NULL_CHECK(outputImage->data);

      PGMImage * args[2];
      args[0] = originalImage;
      args[1] = outputImage;

      struct pi_cluster_task *task = pmsis_l2_malloc(sizeof(struct pi_cluster_task));
      NULL_CHECK(task);
      memset(task, 0, sizeof(struct pi_cluster_task));
      task->entry = masterFindGate;
      task->arg = (void *)args;
      task->stack_size = (uint32_t) 2048;

      /* Synchronous call to cluster, execution will start only on Master Core (core 0) */
      pi_cluster_send_task_to_cl(&cluster_dev, task);
      toSend[2] = outputImage->data[0]; /* x position */
      toSend[3] = outputImage->data[1]; /* y position */

      pmsis_l2_malloc_free(task, sizeof(struct pi_cluster_task));
      //pmsis_l2_malloc_free(originalImage->data, 40000*sizeof(uint8_t));
      pmsis_l2_malloc_free(originalImage, sizeof(PGMImage));

      pmsis_l2_malloc_free(outputImage->data, 40000*sizeof(uint8_t));
      pmsis_l2_malloc_free(outputImage, sizeof(PGMImage));
      return;
}

int main(void)
{
    printf("\n\n\t *** ImageNet classification on GAP ***\n");
    return pmsis_kickoff((void *) body);
}

