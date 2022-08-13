#include "pmsis.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "imageIO.h"
#include "morphological.h"

#define ABS(x) (x>0?x:(-x))
#define MAX(x,y) (x>y?x:y)
#define MIN(x,y) (x>y?y:x)


void cl_maskErosion(void * args)
{
    #if SEGMENTATION_METHOD == 0
    const bool mask[8] = {true, true, true, true, true, true, true, true };
    #else
    const bool mask[8] = {false, true, false, true, false, true, false, true };
    #endif

    uint8_t coreId = pi_core_id();
    clusterCallArgs * restrict realArgs = (clusterCallArgs *)args;
    PGMImage * restrict img = realArgs->inputImage;
    PGMImage * restrict erodedImg = realArgs->outputImage;
    uint8_t nOfCores = realArgs->numOfCores;

    const uint16_t imageWidth = img->x, imageHeight = img->y;

    uint16_t pixel, line;
    uint16_t pixelNeighbours[8];
    uint8_t i, j;
    uint8_t linesPerCore = (imageHeight + nOfCores - 1)/nOfCores; /* rounded up */
    uint8_t beginning = coreId*linesPerCore;
    uint8_t end = MIN(beginning + linesPerCore, imageHeight);


    for(j = beginning, line = beginning * imageWidth; j < end; ++j, line += imageWidth)
        for(i = 0, pixel = line; i < imageWidth; ++i, ++pixel)
            if(i == 0 || j == 0 || i == imageWidth -1 || j == imageHeight -1)
                erodedImg->data[pixel] = MIN_PIXEL_VALUE;
            else if(img->data[pixel] != MIN_PIXEL_VALUE)
                erodedImg->data[pixel] = img->data[pixel];
            else
            {
                pixelNeighbours[0] = pixel - imageWidth - 1; /* upper-left */
                pixelNeighbours[1] = pixel - 1;              /* left */
                pixelNeighbours[2] = pixel + imageWidth - 1; /* bottom-left */
                pixelNeighbours[3] = pixel + imageWidth;     /* bottom */
                pixelNeighbours[4] = pixel + imageWidth + 1; /* bottom-right */
                pixelNeighbours[5] = pixel + 1;              /* right */
                pixelNeighbours[6] = pixel - imageWidth + 1; /* upper-right */
                pixelNeighbours[7] = pixel - imageWidth;     /* upper */

                /* eroding */
                erodedImg->data[pixelNeighbours[0]] = mask[0] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[0]];
                erodedImg->data[pixelNeighbours[1]] = mask[1] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[1]];
                erodedImg->data[pixelNeighbours[2]] = mask[2] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[2]];
                erodedImg->data[pixelNeighbours[3]] = mask[3] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[3]];
                erodedImg->data[pixelNeighbours[4]] = mask[4] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[4]];
                erodedImg->data[pixelNeighbours[5]] = mask[5] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[5]];
                erodedImg->data[pixelNeighbours[6]] = mask[6] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[6]];
                erodedImg->data[pixelNeighbours[7]] = mask[7] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[7]];
            }
}

void cl_maskDilation(void * args)
{
    const bool mask[8] = {true, true, true, true, true, true, true, true };
    uint8_t coreId = pi_core_id();
    clusterCallArgs * realArgs = (clusterCallArgs *)args;
    PGMImage * img = realArgs->inputImage;
    PGMImage * dilatedImg = realArgs->outputImage;
    uint8_t nOfCores = realArgs->numOfCores;

    const uint16_t imageWidth = img->x, imageHeight = img->y;

    uint16_t pixel, line;
    uint16_t pixelNeighbours[8];
    uint8_t i, j;
    uint8_t linesPerCore = (imageHeight + nOfCores - 1)/nOfCores; /* rounded up */
    uint8_t beginning = coreId*linesPerCore;
    uint8_t end = MIN(beginning + linesPerCore, imageHeight);


    for(j = beginning; j < end; ++j)
    {
        line = j * imageWidth;
        for(i = 0, pixel = line; i < imageWidth; ++i, ++pixel)
            if(i == 0 || j == 0 || i == imageWidth -1 || j == imageHeight -1)
                dilatedImg->data[pixel] = MIN_PIXEL_VALUE;
            else if(img->data[pixel] != MAX_PIXEL_VALUE)
                dilatedImg->data[pixel] = img->data[pixel];
            else
            {
                pixelNeighbours[0] = pixel - imageWidth - 1; /* upper-left */
                pixelNeighbours[1] = pixel - 1;              /* left */
                pixelNeighbours[2] = pixel + imageWidth - 1; /* bottom-left */
                pixelNeighbours[3] = pixel + imageWidth;     /* bottom */
                pixelNeighbours[4] = pixel + imageWidth + 1; /* bottom-right */
                pixelNeighbours[5] = pixel + 1;              /* right */
                pixelNeighbours[6] = pixel - imageWidth + 1; /* upper-right */
                pixelNeighbours[7] = pixel - imageWidth;     /* upper */

                /* dilating */
                dilatedImg->data[pixelNeighbours[0]] = mask[0] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[0]];
                dilatedImg->data[pixelNeighbours[1]] = mask[1] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[1]];
                dilatedImg->data[pixelNeighbours[2]] = mask[2] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[2]];
                dilatedImg->data[pixelNeighbours[3]] = mask[3] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[3]];
                dilatedImg->data[pixelNeighbours[4]] = mask[4] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[4]];
                dilatedImg->data[pixelNeighbours[5]] = mask[5] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[5]];
                dilatedImg->data[pixelNeighbours[6]] = mask[6] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[6]];
                dilatedImg->data[pixelNeighbours[7]] = mask[7] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[7]];
            }
    }
}
