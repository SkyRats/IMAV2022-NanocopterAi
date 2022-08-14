#include <stdio.h>
#include <stdint.h>

/* PMSIS includes. */
#include "pmsis.h"

/* Autotiler includes. */
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

#define STACK_SIZE      2048

#ifndef MIN_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES
#define MIN_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES 2000
#endif

#ifndef MAX_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES
#define MAX_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES 40000
#endif

#ifndef MAX_GRAYSHADE_DIFF
#define MAX_GRAYSHADE_DIFF 10
#endif

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


unsigned int PI_L2 Performance[4];
unsigned char index_perf = 0;

unsigned long int PI_L1 Elapsed[10];

void edgesLabelAndCheckNeighbour(PGMImage * img, uint8_t label, uint16_t pixelIndex, Stack * pixelStack, uint16_t * pixelCount)
{
    if(img->data[pixelIndex] == label)
        return;
    else
    {
        img->data[pixelIndex] = label;
        *pixelCount += 1;
    }

    const uint8_t imageWidth = img->x, imageHeight = img->y;
    const uint16_t imageSize = imageWidth * imageHeight;

    uint8_t pixel;
    uint16_t pixelNeighbours[8];


    if( pixelIndex <= imageWidth || pixelIndex >= imageSize - imageWidth || pixelIndex % imageWidth == 0 || (pixelIndex + 1) % (imageWidth) == 0)
        return;

    pixelNeighbours[0] = pixelIndex - imageWidth - 1; /* upper-left */
    pixelNeighbours[1] = pixelIndex - 1;              /* left */
    pixelNeighbours[2] = pixelIndex + imageWidth - 1; /* bottom-left */
    pixelNeighbours[3] = pixelIndex + imageWidth;     /* bottom */
    pixelNeighbours[4] = pixelIndex + imageWidth + 1; /* bottom-right */
    pixelNeighbours[5] = pixelIndex + 1;              /* right */
    pixelNeighbours[6] = pixelIndex - imageWidth + 1; /* upper-right */
    pixelNeighbours[7] = pixelIndex - imageWidth;     /* upper */

    for(uint8_t i = 0; i < 8; ++i)
    {

        pixel = img->data[pixelNeighbours[i]];

        if(pixel == MAX_PIXEL_VALUE)
            push(pixelStack, pixelNeighbours[i]);
    }

}

PQueue* edgeSegmentation(PGMImage * img)
{
    const uint8_t imageWidth = 200, imageHeight = 200;
    const uint16_t imageSize = imageWidth * imageHeight;

    uint8_t y, pixel, label = 1;
    uint16_t line, pixelIndex, pixelCount;
    Stack* pixelStack = createStack();
    PQueue* labelPQueue = createPQueue();

    for(y = 1; y < imageHeight - 1; ++y)
    {
        line = y * imageWidth;
        for(pixelIndex = line + 1; pixelIndex < line + imageWidth - 1; ++pixelIndex)
        {
            pixel = img->data[pixelIndex];

            if(pixel == MAX_PIXEL_VALUE)
            {
                pixelCount = 0;
                edgesLabelAndCheckNeighbour(img, label, pixelIndex, pixelStack, &pixelCount);

                while(!isStackEmpty(pixelStack))
                    edgesLabelAndCheckNeighbour(img, label, pop(pixelStack), pixelStack, &pixelCount);

                if(pixelCount >= MIN_PIXEL_AMOUNT_EDGES_ONLY)
                    pEnqueue(labelPQueue, (PQueueNode){label, imageSize - pixelCount, NULL});


                #ifdef DEBUG_ON
                printf("label %-3u has %-5u pixels.%c", label, pixelCount,label%3==0?'\n':'\t');
                #endif
                label++;
            }
        }
    }

    destroyStack(pixelStack);

    return labelPQueue; /* valid regions found */
}



void __attribute__((noinline)) edgesAndGrayShadesLabelAndCheckNeighbour(PGMImage * img, PGMImage * thresholdedEdgeDetectorOutput, PGMImage * outputImg, uint8_t label, uint16_t pixelIndex, Stack * pixelStack, uint16_t * pixelCount, uint32_t * sum, uint8_t * averageGrayShade)
{

    if(outputImg->data[pixelIndex] == label)
        return;
    else
    {
        outputImg->data[pixelIndex] = label;
        *pixelCount += 1;
        *sum += img->data[pixelIndex];
        *averageGrayShade = (*sum)/(*pixelCount);
    }

    const uint8_t imageWidth = 200, imageHeight = 200;
    const uint16_t imageSize = imageWidth * imageHeight;

    if(pixelIndex <= imageWidth || pixelIndex >= imageSize - imageWidth || pixelIndex % imageWidth == 0 || (pixelIndex + 1) % (imageWidth) == 0)
        return;

    uint8_t neighbourPixel;
    uint16_t pixelNeighbours[8];
    int16_t diff;

    pixelNeighbours[0] = pixelIndex - imageWidth - 1; /* upper-left */
    pixelNeighbours[1] = pixelIndex - 1;              /* left */
    pixelNeighbours[2] = pixelIndex + imageWidth - 1; /* bottom-left */
    pixelNeighbours[3] = pixelIndex + imageWidth;     /* bottom */
    pixelNeighbours[4] = pixelIndex + imageWidth + 1; /* bottom-right */
    pixelNeighbours[5] = pixelIndex + 1;              /* right */
    pixelNeighbours[6] = pixelIndex - imageWidth + 1; /* upper-right */
    pixelNeighbours[7] = pixelIndex - imageWidth;     /* upper */

    for(uint8_t i = 0; i < 8; ++i)
    {

        neighbourPixel = img->data[pixelNeighbours[i]];

        diff = *averageGrayShade - neighbourPixel;

        if(thresholdedEdgeDetectorOutput->data[pixelNeighbours[i]] != MAX_PIXEL_VALUE
                && outputImg->data[pixelNeighbours[i]] == 0
                && -MAX_GRAYSHADE_DIFF <= diff && diff <= MAX_GRAYSHADE_DIFF)
            push(pixelStack, pixelNeighbours[i]);
    }

}

PQueue * __attribute__((noinline)) edgeAndGrayShadeSegmentation(PGMImage * img, PGMImage * thresholdedEdgeDetectorOutput, PGMImage * outputImg)
{
    const uint8_t imageWidth = 200, imageHeight = 200;
    const uint16_t imageSize = imageWidth * imageHeight;

    uint8_t y, averageGrayShade, label = 1;
    uint16_t line, pixelIndex, pixelCount;
    uint32_t sum;
    Stack* pixelStack = createStack();
    PQueue* labelPQueue = createPQueue();

    for(y = 1; y < imageHeight - 1; ++y)
    {
        line = y * imageWidth;
        for(pixelIndex = line + 1; pixelIndex < line + imageWidth - 1; ++pixelIndex)
        {
            if(thresholdedEdgeDetectorOutput->data[pixelIndex] != MAX_PIXEL_VALUE && outputImg->data[pixelIndex] == 0)
            {
                sum = 0;
                pixelCount = 0;
                edgesAndGrayShadesLabelAndCheckNeighbour(img, thresholdedEdgeDetectorOutput, outputImg, label, pixelIndex, pixelStack, &pixelCount, &sum, &averageGrayShade);

                while(!isStackEmpty(pixelStack))
                    edgesAndGrayShadesLabelAndCheckNeighbour(img, thresholdedEdgeDetectorOutput, outputImg, label, pop(pixelStack), pixelStack, &pixelCount, &sum, &averageGrayShade);

                if(MIN_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES <= pixelCount && pixelCount <= MAX_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES)
                {
                    pEnqueue(labelPQueue, (PQueueNode){label, imageSize - pixelCount, NULL});
                    label++;
                }
                else
                    for(uint16_t i = 0; i < imageSize; ++i)
                        if(outputImg->data[i] == label)
                        {
                            outputImg->data[i] = 0;
                            thresholdedEdgeDetectorOutput->data[i] = MAX_PIXEL_VALUE;
                        }
            }

        }
    }

    destroyStack(pixelStack);

    return labelPQueue;
}

void masterFindGate(PGMImage * originalImage, PGMImage * outputImage)
{
    static int FullReport = 1;
    unsigned long int Time, TimeIndex = 0;
    pi_cl_dma_cmd_t dmaCopyStatus;
    imageProcessingState state = FILTERING;
    uint8_t erosionCount = 1;
    PQueue * labels;
    bool copy = false;

    if (FullReport)
    {
        printf("Entering masterFindGate, W=%d, H=%d, %d iterations\n", 200, 200, sizeof(imageProcessingState));
    }
    pi_perf_reset();
    pi_perf_start();
    Time = pi_perf_read(PI_PERF_ACTIVE_CYCLES);

    /* initial allocation and copy of image to L1 memory */
    PGMImage * inputImage = pmsis_l1_malloc(sizeof(PGMImage));
    NULL_CHECK(inputImage);
    inputImage->x = 200;
    inputImage->y = 200;
    inputImage->data = pmsis_l1_malloc(40000*sizeof(uint8_t));
    NULL_CHECK(inputImage->data);

    /* copy of first input from L2 to L1 */
    pi_cl_dma_cmd((uint32_t) originalImage->data, (uint32_t) inputImage->data, 40000*sizeof(uint8_t), PI_CL_DMA_DIR_EXT2LOC, &dmaCopyStatus);

    /* initializing cluster argument structure */
    clusterCallArgs * clusterArgs = pmsis_l1_malloc(sizeof(clusterCallArgs));
    NULL_CHECK(clusterArgs);
    clusterArgs->inputImage = inputImage;
    clusterArgs->outputImage = outputImage;
    clusterArgs->numOfCores = gap_ncore();

    /* wait for transfer to end */
    pi_cl_dma_cmd_wait(&dmaCopyStatus);

    while(state != END)
    {
        switch(state)
        {
            case FILTERING:
                pi_cl_team_fork(clusterArgs->numOfCores, (void *)cl_vectorial_gaussianFilter, (void *)clusterArgs);
                copy = true;
                state = EDGE_DETECTING;
                break;

            case EDGE_DETECTING:
                #if EDGE_DETECTION_METHOD == 0
                pi_cl_team_fork(clusterArgs->numOfCores, (void *)cl_sobelOperator, (void *)clusterArgs);
                #else
                pi_cl_team_fork(clusterArgs->numOfCores, (void *)cl_cannyOperator, (void *)clusterArgs);
                #endif
                copy = true;
                state = THRESHOLDING;
                break;

            case THRESHOLDING:
                adaptiveHistogramTechnique(clusterArgs->inputImage);

                copy = false;
                #if EDGE_DETECTION_METHOD == 0
                state = ERODING;
                #else
                state = DILATING;
                #endif
                break;

            case ERODING:
                pi_cl_team_fork(clusterArgs->numOfCores, (void *)cl_maskErosion, (void *)clusterArgs);
                #if SEGMENTATION_METHOD == 0

                state = DILATING;
                #else

                if(erosionCount++ == 2)
                    state = DILATING;

                #endif

                copy = true;
                break;

            case DILATING:
                pi_cl_team_fork(clusterArgs->numOfCores, (void *)cl_maskDilation, (void *)clusterArgs);
                copy = true;
                state = SEGMENTING;
                break;

            case SEGMENTING:
                #if SEGMENTATION_METHOD == 0

                labels = edgeSegmentation(clusterArgs->inputImage);

                copy = false;

                #else

                /* setting every pixel in the output image to 0 */
                memset(clusterArgs->outputImage->data, 0, 40000*sizeof(uint8_t));

                labels = edgeAndGrayShadeSegmentation(originalImage, clusterArgs->inputImage, clusterArgs->outputImage);

                copy = true;

                #endif

                state = END;
                break;

            default:
                printf("State does not exist!\n");
                break;
        }

        if(copy)
        {
            pi_cl_dma_cmd((uint32_t) clusterArgs->outputImage->data, (uint32_t) clusterArgs->inputImage->data, 40000*sizeof(uint8_t), PI_CL_DMA_DIR_EXT2LOC, &dmaCopyStatus);

            /* wait for transfer to end */
            pi_cl_dma_cmd_wait(&dmaCopyStatus);
        }
        Elapsed[TimeIndex++] = pi_perf_read(PI_PERF_ACTIVE_CYCLES)-Time;
        Time = pi_perf_read(PI_PERF_ACTIVE_CYCLES);
    }


    Point squareCenter;
    PQueueNode label;
    while(!pQueueIsEmpty(labels))
    {
        label = pDequeue(labels);
        printf("\tamount of pixels: %-5u label: %-5u priority: %-5u\n", 40000 - label.priority, label.pQueueItem, label.priority);

        Time = pi_perf_read(PI_PERF_ACTIVE_CYCLES);
        squareCenter = findGate(clusterArgs->inputImage, label.pQueueItem);
        Elapsed[TimeIndex] = pi_perf_read(PI_PERF_ACTIVE_CYCLES)-Time;

        if(squareCenter.x == 0 && squareCenter.y == 0 && squareCenter.grayShade == 0)
          printf("\tregion is not a square.\n\n");
        else
            printf("\tregion is approximately a square with center at (%-3u, %-3u)\n\n", squareCenter.x, squareCenter.y);
    }

    /* stop measuring performance */
    pi_perf_stop();
    Time = pi_perf_read(PI_PERF_ACTIVE_CYCLES)-Time;

    if (FullReport) {
        printf("\n\ngaussian filter                     : %10lu Cycles\n", Elapsed[0]);
        #if EDGE_DETECTION_METHOD == 0
        printf("sobel edge detector                 : %10lu Cycles\n", Elapsed[1]);
        #else
        printf("canny edge detector                 : %10lu Cycles\n", Elapsed[1]);
        #endif
        printf("adaptive thresholding               : %10lu Cycles\n", Elapsed[2]);
        #if EDGE_DETECTION_METHOD ==0
        #if SEGMENTATION_METHOD == 0
        printf("erosion                             : %10lu Cycles\n", Elapsed[3]);
        printf("dilation                            : %10lu Cycles\n", Elapsed[4]);
        printf("edge segmentation                   : %10lu Cycles\n", Elapsed[5]);
        printf("last findGate                       : %10lu Cycles\n", Elapsed[6]);
        printf("Total                               : %10lu Cycles\n", Elapsed[0]+ Elapsed[1]+ Elapsed[2]+ Elapsed[3]+ Elapsed[4] + Elapsed[5] + Elapsed[6]);
        #else
        printf("first erosion                       : %10lu Cycles\n", Elapsed[3]);
        printf("second erosion                      : %10lu Cycles\n", Elapsed[4]);
        printf("dilation                            : %10lu Cycles\n", Elapsed[5]);
        printf("edge-and-gray-shade segmentation    : %10lu Cycles\n", Elapsed[6]);
        printf("last findGate                       : %10lu Cycles\n", Elapsed[7]);
        printf("Total                               : %10lu Cycles\n", Elapsed[0]+ Elapsed[1]+ Elapsed[2]+ Elapsed[3]+ Elapsed[4] + Elapsed[5] + Elapsed[6] + Elapsed[7]);
        #endif
        #else
        printf("dilation                            : %10lu Cycles\n", Elapsed[3]);
        printf("edge segmentation                   : %10lu Cycles\n", Elapsed[4]);
        printf("last findGate                       : %10lu Cycles\n", Elapsed[5]);
        printf("Total                               : %10lu Cycles\n", Elapsed[0]+ Elapsed[1]+ Elapsed[2]+ Elapsed[3]+ Elapsed[4]+ Elapsed[5]);
        #endif
    }
    printf("Total with Master                   : %10lu Cycles\n", Time);

    #if SEGMENTATION_METHOD == 0
    /* copying output from L1 to L2 */
    pi_cl_dma_cmd((uint32_t) outputImage->data, (uint32_t) clusterArgs->inputImage->data, 40000*sizeof(uint8_t), PI_CL_DMA_DIR_LOC2EXT, &dmaCopyStatus);

    /* wait for transfer to end */
    pi_cl_dma_cmd_wait(&dmaCopyStatus);
    #endif

}

void cluster_main(void * args)
{
    printf("cluster master start\n");
    PGMImage * realArgs[2];
    realArgs[0] = *((PGMImage **)args);
    realArgs[1] = *((PGMImage **)args + 1);

    // Configure performance counters for counting the cycles
    pi_perf_conf(1 << PI_PERF_ACTIVE_CYCLES);

    printf("Square Gate Detector running on %d cores, Source %s image[W=%d, H=%d]\n", gap_ncore(), "Mono", 200, 200);

    // Start Square Gate Detector on core 0, which will dispatch the algorithm to the other cores.
    printf("originalImage: %p, outputImage: %p\n", *realArgs, *(realArgs + 1));
    masterFindGate(realArgs[0], realArgs[1]);

    // Stop the performance measurement clock.
    pi_perf_stop();
}

void gateDetectorDemo(void)
{
    printf("Start of application\n");

	char *Imagefile = "images/gate_1.pgm";
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
    task->entry = cluster_main;
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

