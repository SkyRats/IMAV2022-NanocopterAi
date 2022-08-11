/* PMSIS includes. */
#include "pmsis.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "convolution.h"

/* masks for sobel operator */
/*

const uint8_t sobel_mask_0[9] =
   { 1, 2, 1,
    0, 0, 0,
   -1, -2, -1 };


const uint8_t sobel_mask_1[9] =
   { 2, 1, 0,
    1, 0, -1,
    0, -1, -2 };


const uint8_t sobel_mask_2[9] =
   { 1, 0, -1,
    2, 0, -2,
    1, 0, -1 };


const uint8_t sobel_mask_3[9] =
   { 0, -1, -2,
    1, 0, -1,
    2, 1, 0 };


const uint8_t sobel_mask_4[9] =
   {-1, -2, -1,
    0, 0, 0,
    1, 2, 1 };


const uint8_t sobel_mask_5[9] =
   {-2, -1, 0,
   -1, 0, 1,
    0, 1, 2 };


const uint8_t sobel_mask_6[9] =
   {-1, 0, 1,
   -2, 0, 2,
   -1, 0, 1 };


const uint8_t sobel_mask_7[9] =
    { 0, 1, 2,
    -1, 0, 1,
    -2, -1, 0 };
*/


void cl_convolution3by3(void * args)
{
    const int8_t mask[9] = { -1, 0, -1, 0, 4, 0, -1, 0, -1 };
    uint8_t coreId = pi_core_id();
    clusterCallArgs * realArgs = (clusterCallArgs *)args;
    PGMImage * img = realArgs->inputImage;
    PGMImage * convolutedImg = realArgs->outputImage;
    uint8_t nOfCores = realArgs->numOfCores;

    const uint8_t imageWidth = img->x, imageHeight = img->y;

    uint16_t pixelIndex, line;
    uint16_t pixelNeighbours[8];

    /* loop variables */
    uint8_t x, y;
    int32_t convolutedPixel;
    uint8_t linesPerCore = (imageHeight + nOfCores - 1)/nOfCores; /* rounded up */
    uint8_t beginning = coreId*linesPerCore;
    uint8_t end = beginning + linesPerCore;

    if(end > imageHeight)
        end = imageHeight;

    for(y = beginning; y < end; ++y)
    {
        line = y * imageWidth;
        for(x = 0, pixelIndex = line; x < imageWidth; ++x, ++pixelIndex)
        {
            if(x == 0 || y == 0 || x == imageWidth -1 || y == imageHeight -1)
                convolutedImg->data[pixelIndex] = MIN_PIXEL_VALUE;
            else
            {
                /* finding position of each pixel around pixelIndex*/
                pixelNeighbours[0] = pixelIndex - imageWidth - 1;
                pixelNeighbours[1] = pixelIndex - 1;
                pixelNeighbours[2] = pixelIndex + imageWidth - 1;
                pixelNeighbours[3] = pixelIndex + imageWidth;
                pixelNeighbours[4] = pixelIndex + imageWidth + 1;
                pixelNeighbours[5] = pixelIndex + 1;
                pixelNeighbours[6] = pixelIndex - imageWidth + 1;
                pixelNeighbours[7] = pixelIndex - imageWidth;

                /* applying mask */
                convolutedPixel  = img->data[pixelNeighbours[0]] * mask[0];
                convolutedPixel += img->data[pixelNeighbours[1]] * mask[1];
                convolutedPixel += img->data[pixelNeighbours[2]] * mask[2];
                convolutedPixel += img->data[pixelNeighbours[3]] * mask[3];
                convolutedPixel += img->data[pixelNeighbours[4]] * mask[5];
                convolutedPixel += img->data[pixelNeighbours[5]] * mask[6];
                convolutedPixel += img->data[pixelNeighbours[6]] * mask[7];
                convolutedPixel += img->data[pixelNeighbours[7]] * mask[8];

                /* value of the pixel itself*/
                convolutedPixel += img->data[pixelIndex] * mask[4];
                convolutedPixel = convolutedPixel > 0 ? convolutedPixel : -convolutedPixel;
                convolutedImg->data[pixelIndex] = convolutedPixel <= MAX_PIXEL_VALUE ? convolutedPixel : MAX_PIXEL_VALUE;
            }
        }
    }
    pi_cl_team_barrier(0);
}

void cl_sobelOperator(void * args)
{
    uint8_t coreId = pi_core_id();
    clusterCallArgs * realArgs = (clusterCallArgs *)args;
    PGMImage * img = realArgs->inputImage;
    PGMImage * sobelImg = realArgs->outputImage;
    uint8_t nOfCores = realArgs->numOfCores;

    const uint8_t imageWidth = img->x, imageHeight = img->y;

    /* loop variables */
    int16_t xGradient;
    int16_t yGradient;
    uint16_t maxGradient, minGradient, sqrtApprox;
    uint8_t linesPerCore = (imageHeight + nOfCores - 1)/nOfCores; /* rounded up */
    uint8_t beginning = coreId*linesPerCore;
    uint8_t end = beginning + linesPerCore;
    uint8_t x, y;

    uint16_t pixelIndex, line;
    uint16_t pixelNeighbours[8];

    if(end > imageHeight)
        end = imageHeight;

    for(y = beginning, line = beginning * imageWidth ; y < end ; ++y, line += imageWidth)
        for(x = 0, pixelIndex = line ; x < imageWidth ; ++x, ++pixelIndex)
            if(x == 0 || y == 0 || x == imageWidth -1 || y == imageHeight -1)
                sobelImg->data[pixelIndex] = MIN_PIXEL_VALUE;
            else
            {
                /* finding position of each pixel around pixelIndex*/
                pixelNeighbours[0] = pixelIndex - imageWidth - 1; /* upper-left */
                pixelNeighbours[1] = pixelIndex - 1;              /* left */
                pixelNeighbours[2] = pixelIndex + imageWidth - 1; /* bottom-left */
                pixelNeighbours[3] = pixelIndex + imageWidth;     /* bottom */
                pixelNeighbours[4] = pixelIndex + imageWidth + 1; /* bottom-right */
                pixelNeighbours[5] = pixelIndex + 1;              /* right */
                pixelNeighbours[6] = pixelIndex - imageWidth + 1; /* upper-right */
                pixelNeighbours[7] = pixelIndex - imageWidth;     /* upper */

                /* derivating on x */
                xGradient  = img->data[pixelNeighbours[0]];
                xGradient += (img->data[pixelNeighbours[1]] << 1); /* one left shift == multiplication by 2 */
                xGradient += img->data[pixelNeighbours[2]];
                xGradient -= img->data[pixelNeighbours[4]];
                xGradient -= (img->data[pixelNeighbours[5]] << 1);
                xGradient -= img->data[pixelNeighbours[6]];

                /* derivating on y */
                yGradient  = img->data[pixelNeighbours[0]];
                yGradient -= img->data[pixelNeighbours[2]];
                yGradient -= (img->data[pixelNeighbours[3]] << 1);
                yGradient -= img->data[pixelNeighbours[4]];
                yGradient += img->data[pixelNeighbours[6]];
                yGradient += (img->data[pixelNeighbours[7]] << 1);

                /* SRA algorithm -- Square-Root Approximation */
                /** sqrt(xGradient ^2 + yGradient ^2)
                  * approx eq to
                  * max( 0.875 * max(xGradient, yGradient) + 0.5 * min(xGradient, yGradient), max(xGradient, yGradient))
                  */

                xGradient = xGradient > 0 ? xGradient : -xGradient;
                yGradient = yGradient > 0 ? yGradient : -yGradient;

                maxGradient = xGradient > yGradient ? xGradient : yGradient;
                minGradient = xGradient > yGradient ? yGradient : xGradient;

                sqrtApprox = maxGradient - (maxGradient >> 3) + (minGradient >> 1);
                sqrtApprox = sqrtApprox > maxGradient ? sqrtApprox : maxGradient;

                sobelImg->data[pixelIndex] = sqrtApprox <= MAX_PIXEL_VALUE ? sqrtApprox : MAX_PIXEL_VALUE;

            }
    pi_cl_team_barrier(0);
}
