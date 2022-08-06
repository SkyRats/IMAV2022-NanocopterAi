#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "imageIO.h"
#include "convolution.h"
#include "erosion.h"
#include "filter.h"
#include "histogram.h"
#include "regionGrowing.h"
#include "queue.h"
#include "findGate.h"

int main(int argc, char** argv)
{
    int8_t const quickMask[9] =
       { -1, 0, -1,
        0, 4, 0,
       -1, 0, -1 };

    bool const erosionMask[8] =
       {true, true, true,
        true,      true,
       true, true, true };

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
    free(image);
    if(filteredImg == NULL || filteredImg->data == NULL)
    {
        printf("Allocation of memory failed. Exiting.\n");
        exit(-1);
    }

    PGMImage *convolutedImg =  convolution3by3(filteredImg, quickMask);
    free(filteredImg);
    if(convolutedImg == NULL || convolutedImg->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }

    PGMImage * erodedImg = maskErosion(convolutedImg, erosionMask);
    free(convolutedImg);
    if(erodedImg == NULL || erodedImg->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }

    #if THRESHOLDING_SEGMENTATION_METHOD == 0
    histogramPeakTechnique(erodedImg);
    #elif THRESHOLDING_SEGMENTATION_METHOD == 1
    histogramValleyTechnique(erodedImg);
    #else
    adaptiveHistogramTechnique(erodedImg);
    #endif

    PQueue * labels = edgeSegmentation(erodedImg);

    #ifdef DEBUG_ON
    printf("\nfound %u valid region%s\n", labels->size,labels->size>1?"s.":".");
    #endif
    Point squareCenter;
    while(!pQueueIsEmpty(labels))
    {
        PQueueNode label = pDequeue(labels);
        #ifdef DEBUG_ON
        printf("\tamount of pixels: %-5u label: %-5u priority: %-5u\n", erodedImg->x * erodedImg->y - label.priority, label.pQueueItem, label.priority);
        #endif

        squareCenter = findGate(erodedImg, label.pQueueItem);
        if(squareCenter.x == 0 && squareCenter.y == 0 && squareCenter.grayShade == 0)
            printf("found no squares.\n");
        else
            printf("found square with center at (%-3u, %-3u)\n", squareCenter.x, squareCenter.y);
    }

    writePGM(argv[2], erodedImg);
    free(erodedImg);
    destroyPQueue(labels);
    printf("Done.\n");

    return 0;
}
