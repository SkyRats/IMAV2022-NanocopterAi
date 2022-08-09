#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "imageIO.h"
#include "morphological.h"

PGMImage* maskErosion(PGMImage *img, bool const mask[8])
{

    const uint16_t imageWidth = img->x, imageHeight = img->y;
    const uint16_t imageSize = imageWidth*imageHeight;

    uint16_t pixel, line;
    uint16_t pixelNeighbours[8];
    uint8_t i, j;

    PGMImage* erodedImg = (PGMImage *)malloc(sizeof(PGMImage));
    erodedImg->x = imageWidth;
    erodedImg->y = imageHeight;
    erodedImg->data = (PGMPixel*)malloc(imageSize * sizeof(PGMPixel));

    if(erodedImg == NULL || erodedImg->data == NULL)
    {
        #ifdef DEBUG_ON
        printf("Allocation of memory for image failed. Exiting.");
        #endif
        return NULL;
    }

    for(j = 0; j < imageHeight; ++j)
    {
        line = j * imageWidth;
        for(i = 0, pixel = line; i < imageWidth; ++i, ++pixel)
            if(i == 0 || j == 0 || i == imageWidth -1 || j == imageHeight -1)
                erodedImg->data[pixel].gray = MIN_PIXEL_VALUE;
            else if(img->data[pixel].gray != MIN_PIXEL_VALUE)
                erodedImg->data[pixel].gray = img->data[pixel].gray;
            else
            {
                pixelNeighbours[0] = line - imageWidth + (i - 1); /* upper-left */
                pixelNeighbours[1] = line + (i - 1);              /* left */
                pixelNeighbours[2] = line + imageWidth + (i - 1); /* bottom-left */
                pixelNeighbours[3] = line + imageWidth + (i);     /* bottom */
                pixelNeighbours[4] = line + imageWidth + (i + 1); /* bottom-right */
                pixelNeighbours[5] = line + (i + 1);              /* right */
                pixelNeighbours[6] = line - imageWidth + (i + 1); /* upper-right */
                pixelNeighbours[7] = line - imageWidth + (i);     /* upper */

                /* eroding */
                erodedImg->data[pixelNeighbours[0]].gray = mask[0] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[0]].gray;
                erodedImg->data[pixelNeighbours[1]].gray = mask[1] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[1]].gray;
                erodedImg->data[pixelNeighbours[2]].gray = mask[2] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[2]].gray;
                erodedImg->data[pixelNeighbours[3]].gray = mask[3] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[3]].gray;
                erodedImg->data[pixelNeighbours[4]].gray = mask[4] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[4]].gray;
                erodedImg->data[pixelNeighbours[5]].gray = mask[5] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[5]].gray;
                erodedImg->data[pixelNeighbours[6]].gray = mask[6] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[6]].gray;
                erodedImg->data[pixelNeighbours[7]].gray = mask[7] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[7]].gray;
            }
    }

    return erodedImg;
}

void pi_cl_maskErosion(void * args)
{
    const bool mask[8] = {false, true, false, true, true, false, true, false };
    uint8_t coreId = pi_core_id();
    clusterCallArgs * realArgs = (clusterCallArgs *)args;
    PGMImage * img = realArgs->inputImage;
    PGMImage * erodedImg = realArgs->outputImage;
    uint8_t nOfCores = realArgs->numOfCores;

    const uint16_t imageWidth = img->x, imageHeight = img->y;

    uint16_t pixel, line;
    uint16_t pixelNeighbours[8];
    uint8_t i, j;
    uint8_t linesPerCore = (imageHeight + nOfCores - 1)/nOfCores; /* rounded up */
    uint8_t beginning = coreId*linesPerCore;
    uint8_t end = beginning + linesPerCore;

    if(end > imageHeight)
        end = imageHeight;

    for(j = beginning; j < end; ++j)
    {
        line = j * imageWidth;
        for(i = 0, pixel = line; i < imageWidth; ++i, ++pixel)
            if(i == 0 || j == 0 || i == imageWidth -1 || j == imageHeight -1)
                erodedImg->data[pixel].gray = MIN_PIXEL_VALUE;
            else if(img->data[pixel].gray != MIN_PIXEL_VALUE)
                erodedImg->data[pixel].gray = img->data[pixel].gray;
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
                erodedImg->data[pixelNeighbours[0]].gray = mask[0] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[0]].gray;
                erodedImg->data[pixelNeighbours[1]].gray = mask[1] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[1]].gray;
                erodedImg->data[pixelNeighbours[2]].gray = mask[2] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[2]].gray;
                erodedImg->data[pixelNeighbours[3]].gray = mask[3] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[3]].gray;
                erodedImg->data[pixelNeighbours[4]].gray = mask[4] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[4]].gray;
                erodedImg->data[pixelNeighbours[5]].gray = mask[5] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[5]].gray;
                erodedImg->data[pixelNeighbours[6]].gray = mask[6] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[6]].gray;
                erodedImg->data[pixelNeighbours[7]].gray = mask[7] ? MIN_PIXEL_VALUE : img->data[pixelNeighbours[7]].gray;
            }
    }
}

PGMImage* maskDilation(PGMImage *img, bool const mask[8])
{

    const uint16_t imageWidth = img->x, imageHeight = img->y;
    const uint16_t imageSize = imageWidth*imageHeight;

    uint16_t pixel, line;
    uint16_t pixelNeighbours[8];
    uint8_t i, j;

    PGMImage* dilatedImg = (PGMImage *)malloc(sizeof(PGMImage));

    dilatedImg->x = imageWidth;
    dilatedImg->y = imageHeight;
    dilatedImg->data = (PGMPixel*)malloc(imageSize * sizeof(PGMPixel));

    if(dilatedImg == NULL || dilatedImg->data == NULL)
    {
        #ifdef DEBUG_ON
        printf("Allocation of memory for image failed. Exiting.");
        #endif
        return NULL;
    }

    for(j = 0; j < imageHeight; ++j)
    {
        line = j * imageWidth;
        for(i = 0, pixel = line; i < imageWidth; ++i, ++pixel)
            if(i == 0 || j == 0 || i == imageWidth -1 || j == imageHeight -1)
                dilatedImg->data[pixel].gray = MIN_PIXEL_VALUE;
            else if(img->data[pixel].gray != MAX_PIXEL_VALUE)
                dilatedImg->data[pixel].gray = img->data[pixel].gray;
            else
            {
                pixelNeighbours[0] = line - imageWidth + (i - 1); /* upper-left */
                pixelNeighbours[1] = line + (i - 1);              /* left */
                pixelNeighbours[2] = line + imageWidth + (i - 1); /* bottom-left */
                pixelNeighbours[3] = line + imageWidth + (i);     /* bottom */
                pixelNeighbours[4] = line + imageWidth + (i + 1); /* bottom-right */
                pixelNeighbours[5] = line + (i + 1);              /* right */
                pixelNeighbours[6] = line - imageWidth + (i + 1); /* upper-right */
                pixelNeighbours[7] = line - imageWidth + (i);     /* upper */

                /* dilating */
                dilatedImg->data[pixelNeighbours[0]].gray = mask[0] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[0]].gray;
                dilatedImg->data[pixelNeighbours[1]].gray = mask[1] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[1]].gray;
                dilatedImg->data[pixelNeighbours[2]].gray = mask[2] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[2]].gray;
                dilatedImg->data[pixelNeighbours[3]].gray = mask[3] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[3]].gray;
                dilatedImg->data[pixelNeighbours[4]].gray = mask[4] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[4]].gray;
                dilatedImg->data[pixelNeighbours[5]].gray = mask[5] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[5]].gray;
                dilatedImg->data[pixelNeighbours[6]].gray = mask[6] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[6]].gray;
                dilatedImg->data[pixelNeighbours[7]].gray = mask[7] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[7]].gray;
            }
    }

    return dilatedImg;
}

void pi_cl_maskDilation(void * args)
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
    uint8_t end = beginning + linesPerCore;

    if(end > imageHeight)
        end = imageHeight;

    for(j = beginning; j < end; ++j)
    {
        line = j * imageWidth;
        for(i = 0, pixel = line; i < imageWidth; ++i, ++pixel)
            if(i == 0 || j == 0 || i == imageWidth -1 || j == imageHeight -1)
                dilatedImg->data[pixel].gray = MIN_PIXEL_VALUE;
            else if(img->data[pixel].gray != MAX_PIXEL_VALUE)
                dilatedImg->data[pixel].gray = img->data[pixel].gray;
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
                dilatedImg->data[pixelNeighbours[0]].gray = mask[0] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[0]].gray;
                dilatedImg->data[pixelNeighbours[1]].gray = mask[1] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[1]].gray;
                dilatedImg->data[pixelNeighbours[2]].gray = mask[2] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[2]].gray;
                dilatedImg->data[pixelNeighbours[3]].gray = mask[3] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[3]].gray;
                dilatedImg->data[pixelNeighbours[4]].gray = mask[4] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[4]].gray;
                dilatedImg->data[pixelNeighbours[5]].gray = mask[5] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[5]].gray;
                dilatedImg->data[pixelNeighbours[6]].gray = mask[6] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[6]].gray;
                dilatedImg->data[pixelNeighbours[7]].gray = mask[7] ? MAX_PIXEL_VALUE : img->data[pixelNeighbours[7]].gray;
            }
    }
}
