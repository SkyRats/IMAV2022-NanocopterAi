#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "config.h"
#include "imageIO.h"
#include "convolution.h"
#include "morphological.h"
#include "filter.h"
#include "histogram.h"
#include "regionGrowing.h"
#include "queue.h"
#include "findGate.h"

int main(int argc, char** argv)
{
    /* mask pixel order
       upper-left, left, bottom-left, bottom, (center pixel if mask is 9-bit wide), bottom-right, right, upper-right and upper pixel */
    int8_t const quickMask[9] = { -1, 0, -1, 0, 4, 0, -1, 0, -1 };

    bool const erosionMask[8] = {false, true, false, true, true, false, true, false };

    bool const dilationMask[8] = {true, true, true, true, true, true, true, true };

    if(argc < 3)
    {
        printf("\tUsage: testErosion [input file path] [output file path]\n");
        exit(-1);
    }

    PGMImage *image;
    image = readPGM(argv[1]);
    if(image == NULL)
    {
        printf("File does not exist.\n");
        exit(-1);
    }

    PGMImage *filteredImg = lowPixelFilter(image);

    #if SEGMENTATION_METHOD == 0
    free(image);
    #endif
    if(filteredImg == NULL || filteredImg->data == NULL)
    {
        printf("Allocation of memory failed. Exiting.\n");
        exit(-1);
    }

    /* the following convolution mask detects horizontal and vertical edges simultaneously
       with certain accuraccy. Another option would be to convolute the image twice: one
       with a vertical-edge detector mask and another with a horizontal-edge detector mask
       -- then, sum both images into one. This will be left as an exercise for the reader :) */
    PGMImage *convolutedImg =  convolution3by3(filteredImg, quickMask);
    free(filteredImg);
    if(convolutedImg == NULL || convolutedImg->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }


    #if THRESHOLDING_SEGMENTATION_METHOD == 0

    histogramPeakTechnique(convolutedImg);

    #elif THRESHOLDING_SEGMENTATION_METHOD == 1

    histogramValleyTechnique(convolutedImg);

    #else

    adaptiveHistogramTechnique(convolutedImg);

    #endif


    /* we will moderately erode the image twice */
    PGMImage * halfEroded = maskErosion(convolutedImg, erosionMask);
    if(halfEroded == NULL || halfEroded->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }

    PGMImage * erodedImg = maskErosion(halfEroded, erosionMask);
    free(halfEroded);
    free(convolutedImg);
    if(erodedImg == NULL || erodedImg->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }

    /* then we apply a strong dilation */
    PGMImage * dilatedImg = maskDilation(erodedImg, dilationMask);
    //free(erodedImg);
    if(dilatedImg == NULL || dilatedImg->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }

    /* finally, we segment the image and proceed to look for square regions (potential gates) */
    #if SEGMENTATION_METHOD == 0

    PQueue * labels = edgeSegmentation(dilatedImg);

    #else

    PGMImage * outputImg = malloc(sizeof(PGMImage));
    outputImg->x = image->x;
    outputImg->y = image->y;
    outputImg->data = calloc(image->x * image->y, sizeof(PGMPixel));
    if(outputImg == NULL || outputImg->data == NULL)
    {
        printf("Allocation of memory failed. Exiting.");
        exit(-1);
    }

    PQueue * labels = edgeAndGrayShadeSegmentation(image, dilatedImg, outputImg);

    #endif

    #ifdef DEBUG_ON
    printf("\nfound %u valid region%s\n", labels->size,labels->size>1?"s.":".");
    #endif

    Point squareCenter;
    PQueueNode label;
    while(!pQueueIsEmpty(labels))
    {
        label = pDequeue(labels);
        #ifdef DEBUG_ON
        printf("\tamount of pixels: %-5u label: %-5u priority: %-5u\n", (uint16_t)(dilatedImg->x * dilatedImg->y) - label.priority, label.pQueueItem, label.priority);
        #endif

        #if SEGMENTATION_METHOD == 0
        squareCenter = findGate(dilatedImg, label.pQueueItem);
        #else
        squareCenter = findGate(outputImg, label.pQueueItem);
        #endif

        if(squareCenter.x == 0 && squareCenter.y == 0 && squareCenter.grayShade == 0)
            printf("\tregion is not a square.\n\n");
        else
            printf("\tregion is approximately a square with center at (%-3u, %-3u)\n\n", squareCenter.x, squareCenter.y);
    }

    #if SEGMENTATION_METHOD == 0
    writePGM(argv[2], dilatedImg);
    #else
    writePGM(argv[2], outputImg);
    free(outputImg);
    #endif

    free(dilatedImg);
    destroyPQueue(labels);

    printf("Done.\n");

    return 0;
}
