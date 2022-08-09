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

#define NULL_CHECK(val)                                  \
    if (val == NULL)                                     \
    {                                                    \
        printf("Allocation of memory failed. Exiting."); \
        exit(-1);                               \
    }

int main(int argc, char** argv)
{
    /* mask pixel order
       upper-left, left, bottom-left, bottom, (center pixel if mask is 9-bit wide), bottom-right, right, upper-right and upper pixel */
    int8_t const quickMask[9] = { -1, 0, -1, 0, 4, 0, -1, 0, -1 };
    const int8_t sobel_mask_y[9] =
   { 1, 2, 1,
    0, 0, 0,
   -1, -2, -1 };
    const int8_t sobel_mask_x[9] =
   {-1, 0, 1,
   -2, 0, 2,
   -1, 0, 1 };

    const int8_t prewitt_mask_y[9] =
   { 1, 1, 1,
    0, 0, 0,
   -1, -1, -1 };
    const int8_t prewitt_mask_x[9] =
   {-1, 0, 1,
   -1, 0, 1,
   -1, 0, 1 };


    bool const erosionMask[8] = {false, true, false, true, true, false, true, false };

    bool const dilationMask[8] = {true, true, true, true, true, true, true, true };

    if(argc < 3)
    {
        printf("\tUsage: testErosion [input file path] [output file path]\n");
        exit(-1);
    }

    PGMImage *image;
    image = readPGM(argv[1]);
    NULL_CHECK(image);

    PGMImage *filteredImg = lowPixelFilter(image);

    #if SEGMENTATION_METHOD == 0
    free(image);
    #endif
    NULL_CHECK(filteredImg);
    NULL_CHECK(filteredImg->data);

    /* the following convolution mask detects horizontal and vertical edges simultaneously
       with certain accuraccy. Another option would be to convolute the image twice: one
       with a vertical-edge detector mask and another with a horizontal-edge detector mask
       -- then, sum both images into one. This will be left as an exercise for the reader :) */

    PGMImage *sobelImg = sobel_convolution(filteredImg,prewitt_mask_x, prewitt_mask_y);
    free(filteredImg);
    NULL_CHECK(sobelImg);
    NULL_CHECK(sobelImg->data);








    #if THRESHOLDING_SEGMENTATION_METHOD == 0

    histogramPeakTechnique(sobelImg);

    #elif THRESHOLDING_SEGMENTATION_METHOD == 1

    histogramValleyTechnique(sobelImg);

    #else

    adaptiveHistogramTechnique(sobelImg);

    #endif


    /* we will moderately erode the image twice */
    PGMImage * halfEroded = maskErosion(sobelImg, erosionMask);
    NULL_CHECK(halfEroded);
    NULL_CHECK(halfEroded->data);

    PGMImage * erodedImg = maskErosion(halfEroded, erosionMask);
    NULL_CHECK(erodedImg);
    free(halfEroded);
    free(sobelImg);
    NULL_CHECK(erodedImg->data);

    /* then we apply a strong dilation */
    PGMImage * dilatedImg = maskDilation(erodedImg, dilationMask);
    NULL_CHECK(dilatedImg);
    free(erodedImg);
    NULL_CHECK(dilatedImg->data);

    /* finally, we segment the image and proceed to look for square regions (potential gates) */
    #if SEGMENTATION_METHOD == 0

    PQueue * labels = edgeSegmentation(dilatedImg);

    #else

    PGMImage * outputImg = malloc(sizeof(PGMImage));
    NULL_CHECK(outputImg);
    outputImg->x = image->x;
    outputImg->y = image->y;
    /* using calloc here is crucial */
    outputImg->data = calloc(image->x * image->y, sizeof(PGMPixel));
    NULL_CHECK(outputImg->data);

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
