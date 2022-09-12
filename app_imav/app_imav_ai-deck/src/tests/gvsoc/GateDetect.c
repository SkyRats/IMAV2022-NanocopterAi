#include <stdio.h>
#include <stdint.h>

/* PMSIS includes. */
#include "pmsis.h"

/* Autotiler includes. */
//#include "Gap8.h"
#include "Gap.h"

/* Gap_lib includes. */
#include "gaplib/ImgIO.h"

/* gate detector includes */
#include "config.h"
#include "imageIO.h"
#include "convolution.h"
#include "morphological.h"
#include "filter.h"
#include "histogram.h"
#include "stack.h"
#include "queue.h"
#include "findGate.h"
#include "regionGrowing.h"

#define STACK_SIZE      2048

#define Max(a, b) (((a)>(b))?(a):(b))
#define Min(a, b) (((a)<(b))?(a):(b))
#define NULL_CHECK(val)                                  \
    if (val == NULL)                                     \
    {                                                    \
        printf("Allocation of memory failed. Exiting."); \
        exit(-1);                                        \
    }

typedef enum __attribute__((packed))
{
    FILTERING = 0,
    EDGE_DETECTING,
    THRESHOLDING,
    ERODING,
    DILATING,
    SEGMENTING,
    END
} imageProcessingState;

void clusterMain(void * args)
{
    PGMImage * restrict originalImage = *((PGMImage **)args);
    PGMImage * restrict outputImage = *((PGMImage **)args + 1);
    PGMImage * restrict inputImage = *((PGMImage **)args + 2);
    pi_cl_dma_cmd_t dmaCopyStatus;
    imageProcessingState state = FILTERING;

    #if SEGMENTATION_METHOD != 0
    uint8_t erosionCount = 1;
    #endif

    PQueue * labels = NULL;
    bool copy = false;

    /* initializing cluster argument structure */
    clusterCallArgs * clusterArgs = pmsis_l1_malloc(sizeof(clusterCallArgs));
    NULL_CHECK(clusterArgs);
    clusterArgs->inputImage = inputImage;
    clusterArgs->outputImage = outputImage;
    clusterArgs->numOfCores = gap_ncore();

    #ifdef DEBUG_ON
    printf("Entering state machine\n");
    #endif

    while(state != END)
    {
        switch(state)
        {
            case FILTERING:
                cl_vectorial_gaussianFilter((void *)clusterArgs);
                copy = true;
                state = EDGE_DETECTING;
                break;

            case EDGE_DETECTING:

                #if EDGE_DETECTION_METHOD == 0
                cl_sobelOperator((void *)clusterArgs);
                #else
                cl_cannyOperator((void *)clusterArgs);
                #endif

                copy = true;
                state = THRESHOLDING;
                //state = DILATING;
                //state = ERODING;
		//state = END;
                break;

            case THRESHOLDING:
                if(pi_core_id() == 0)
                    adaptiveHistogramTechnique(clusterArgs->inputImage);

                copy = false;
                #if EDGE_DETECTION_METHOD == 0
                state = ERODING;
                #else
                state = DILATING;
                #endif
                break;

            //case ERODING:
                //cl_maskErosion((void *)clusterArgs);
                //#if SEGMENTATION_METHOD == 0
//
                //state = DILATING;
                //#else
//
                //if(erosionCount++ == 2)
                    //state = DILATING;
//
                //#endif
//
                //copy = true;
                //break;

            case DILATING:
                cl_maskDilation((void *)clusterArgs);
                copy = true;
                state = SEGMENTING;
                break;

            case SEGMENTING:
                #if SEGMENTATION_METHOD == 0

                if(pi_core_id() == 0)
                    labels = edgeSegmentation(clusterArgs->inputImage);

                copy = false;

                #else

                if(pi_core_id() == 0)
                {
                    /* setting every pixel in the output image to 0 */
                    memset(clusterArgs->outputImage->data, 0, 40000*sizeof(uint8_t));

                    labels = edgeAndGrayShadeSegmentation(originalImage, clusterArgs->inputImage, clusterArgs->outputImage);
                }

                copy = true;

                #endif

                state = END;
                break;

            default:
                //printf("State does not exist!\n");
                break;
        }

        pi_cl_team_barrier(0);

        if(pi_core_id() == 0 && copy)
        {
            //pi_cl_dma_cmd((uint32_t) clusterArgs->outputImage->data, (uint32_t) clusterArgs->inputImage->data, 40000*sizeof(uint8_t), PI_CL_DMA_DIR_EXT2LOC, &dmaCopyStatus);

            /* wait for transfer to end */
            //pi_cl_dma_cmd_wait(&dmaCopyStatus);
            memcpy(clusterArgs->inputImage->data, clusterArgs->outputImage->data, 40000*sizeof(uint8_t));
	    //for(uint16_t i = 0; i < 40000; ++i)
		    //clusterArgs->inputImage->data[i] = clusterArgs->outputImage->data[i];

        }

        pi_cl_team_barrier(0);

    }


    if(pi_core_id() == 0 && labels != NULL)
    {
        Point squareCenter;
        PQueueNode label;
        bool done = false;
        while(!pQueueIsEmpty(labels) && !done)
        {
            label = pDequeue(labels);

            squareCenter = findGate(clusterArgs->inputImage, label.pQueueItem);

            #ifdef DEBUG_ON
            printf("Square at (%d, %d)\n", squareCenter.x, squareCenter.y);
            #endif

            //if( squareCenter.x >= 50 && squareCenter.x <= 150 &&
	//	        squareCenter.y >= 50 && squareCenter.y <= 150 &&
	 //           squareCenter.grayShade == 1)
            if(squareCenter.grayShade == 1)
                done = true;
        }



        #if SEGMENTATION_METHOD == 0
        /* copying output from L1 to L2 */
        //pi_cl_dma_cmd((uint32_t) outputImage->data, (uint32_t) clusterArgs->inputImage->data, 40000*sizeof(uint8_t), PI_CL_DMA_DIR_LOC2EXT, &dmaCopyStatus);

        /* wait for transfer to end */
        //pi_cl_dma_cmd_wait(&dmaCopyStatus);
	memcpy(outputImage->data, clusterArgs->inputImage->data, 40000*sizeof(uint8_t));
	pmsis_l1_malloc_free(clusterArgs, sizeof(clusterCallArgs));
        #endif

        if(done)
        {
            outputImage->data[0] = squareCenter.x;
            outputImage->data[1] = squareCenter.y;
        }
        else
        {
            outputImage->data[0] = 0;
            outputImage->data[1] = 0;
        }

        destroyPQueue(labels);
    }
}

void masterFindGate(void * args)
{
    PGMImage * restrict originalImage = *((PGMImage **)args);
    PGMImage * restrict outputImage = *((PGMImage **)args + 1);
    //pi_cl_dma_cmd_t dmaCopyStatus;
    pi_cl_alloc_req_t alloc_req, alloc_reqn;
    pi_cl_free_req_t free_req, free_reqn;
    #ifdef DEBUG_ON
    printf("cluster master start\n");
    #endif

    /* initial allocation and copy of image to L1 memory */
    //PGMImage * inputImage = pmsis_l1_malloc(sizeof(PGMImage));
    pi_cl_l2_malloc(sizeof(PGMImage), &alloc_req);
    PGMImage * inputImage = pi_cl_l2_malloc_wait(&alloc_req);
    NULL_CHECK(inputImage);
    inputImage->x = originalImage->x;
    inputImage->y = originalImage->y;
    //inputImage->data = pmsis_l1_malloc(40000*sizeof(uint8_t));
    pi_cl_l2_malloc(40000*sizeof(uint8_t), &alloc_reqn);
    inputImage->data = pi_cl_l2_malloc_wait(&alloc_reqn);
    NULL_CHECK(inputImage->data);

    /* copy of first input from L2 to L1 */
    printf("dma copy starting\n");
    //pi_cl_dma_cmd((uint32_t) originalImage->data, (uint32_t) inputImage->data, 40000*sizeof(uint8_t), PI_CL_DMA_DIR_EXT2LOC, &dmaCopyStatus);
    memcpy(inputImage->data, originalImage->data, 40000*sizeof(uint8_t));
    //for(uint16_t i = 0; i < 40000; ++i)
	    //inputImage->data[i] = originalImage->data[i];

    /* wait for transfer to end */
    //pi_cl_dma_cmd_wait(&dmaCopyStatus);

    /* cluster call arguments */
    PGMImage * forkArgs[3];
    forkArgs[0] = originalImage;
    forkArgs[1] = outputImage;
    forkArgs[2] = inputImage;

    // Start Square Gate Detector on core 0, which will dispatch the algorithm to the other cores.
    printf("forking\n");
    pi_cl_team_fork(gap_ncore(),(void *)clusterMain, forkArgs);

    //pmsis_l1_malloc_free(inputImage->data, 40000*sizeof(uint8_t));
    //pmsis_l1_malloc_free(inputImage, sizeof(PGMImage));

    pi_cl_l2_free(inputImage->data, 40000*sizeof(uint8_t), &free_req);
    pi_cl_l2_free_wait(&free_req);
    pi_cl_l2_free(inputImage, sizeof(PGMImage), &free_reqn);
    pi_cl_l2_free_wait(&free_reqn);
}

void gateDetectorDemo(void)
{
    printf("Start of application\n");

	char *Imagefile = "test_01.pgm";
    PGMImage * originalImage = pmsis_l2_malloc(sizeof(PGMImage));
    NULL_CHECK(originalImage);
    printf("originalImage: %p\n", originalImage);
    originalImage->x = 200;
    originalImage->y = 200;
	char imageName[64];
	sprintf(imageName, "../../../%s", Imagefile);
	originalImage->data = (uint8_t *) pmsis_l2_malloc(40000*sizeof(uint8_t));

    if (ReadImageFromFile(imageName, 200,200, 1, originalImage->data, 40000*sizeof(uint8_t), 0, 0))
    {
        printf("Failed to load image %s\n", imageName);
        pmsis_exit(-1);
    }

    /* AI comes in here. Instead of opening a file
       we will have access to the original image by means of the pointer
       returned by the network_setup function. Then, we will run the
       network inside an infinite loop and, if the probability
       of collision is low enogh, we send the information to crazyflie
       and proceed to check for gates */

    /* Allocating space for input and output images */

    PGMImage * outputImage = pmsis_l2_malloc(sizeof(PGMImage));
    NULL_CHECK(outputImage);
    outputImage->data = pmsis_l2_malloc(40000*sizeof(uint8_t));
    NULL_CHECK(outputImage->data);
    outputImage->x = 200;
    outputImage->y = 200;

    /* cluster call arguments */
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

    char imgName[50];
    sprintf(imgName, "../../../img_OUT.pgm");
    printf("imgName: %s\n", imgName);
    WriteImageToFile(imgName, 200, 200, 1, outputImage->data, sizeof(uint8_t));

    pi_cluster_close(&cluster_dev);

    pmsis_exit(0);
}


int main(int argc, char *argv[])
{
    printf("\n\n\t *** Gate Detector ***\n\n");
    return pmsis_kickoff((void *) gateDetectorDemo);
}
