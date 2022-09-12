/* PMSIS includes. */
#include "pmsis.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "convolution.h"

/* Autotiler includes. */
//#include "Gap8.h"
#include "Gap.h"

#define ABS(x) (x>0?x:(-x))
#define MAX(x,y) (x>y?x:y)
#define MIN(x,y) (x>y?y:x)

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
    uint8_t end = MIN(beginning + linesPerCore, imageHeight);


    for(y = beginning, line = beginning * imageWidth; y < end; ++y, line += imageWidth)
        for(x = 0, pixelIndex = line; x < imageWidth; ++x, ++pixelIndex)
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

void __attribute__((noinline)) cl_sobelOperator(void * args)
{
    uint8_t coreId = pi_core_id();
    clusterCallArgs * restrict realArgs = (clusterCallArgs *)args;
    PGMImage * restrict img = realArgs->inputImage;
    PGMImage * restrict sobelImg = realArgs->outputImage;
    uint8_t nOfCores = realArgs->numOfCores;

    const uint8_t imageWidth = img->x, imageHeight = img->y;

    /* loop variables */
    int16_t xGradient;
    int16_t yGradient;
    uint16_t maxGradient, minGradient, sqrtApprox;
    uint8_t linesPerCore = (imageHeight + nOfCores - 1)/nOfCores; /* rounded up */
    uint8_t beginning = coreId*linesPerCore;
    uint8_t end = MIN(beginning + linesPerCore, imageHeight);
    uint8_t x, y;

    uint16_t pixelIndex, line;
    uint16_t pixelNeighbours[8];

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

                xGradient = ABS(xGradient);
                yGradient = ABS(yGradient);

                maxGradient = MAX(xGradient, yGradient);
                minGradient = MIN(xGradient, yGradient);

                sqrtApprox = maxGradient - (maxGradient >> 3) + (minGradient >> 1);
                sqrtApprox = MAX(sqrtApprox, maxGradient);

                sobelImg->data[pixelIndex] = MIN(sqrtApprox, MAX_PIXEL_VALUE);

            }
}

void __attribute__((noinline)) cl_cannyOperator(void * args)
{
    const uint8_t coreId = pi_core_id();
    clusterCallArgs * restrict realArgs = (clusterCallArgs *)args;
    PGMImage * restrict img = realArgs->inputImage;
    PGMImage * restrict cannyImg = realArgs->outputImage;
    const uint8_t nOfCores = realArgs->numOfCores;

    const uint8_t imageWidth = img->x, imageHeight = img->y;

    /* loop variables */
    uint16_t maxGradient, minGradient, sqrtApprox;
    const uint8_t linesPerCore = (imageHeight + nOfCores - 1)/nOfCores; /* rounded up */
    const uint8_t beginning = coreId*linesPerCore;
    const uint16_t firstLine = beginning * imageWidth;
    const uint8_t end = MIN(beginning + linesPerCore, imageHeight);
    uint8_t x, y, pixel;

    uint16_t pixelIndex, line;
    uint16_t pixelNeighbours[8];

    int16_t xGradient;
    int16_t yGradient;
    uint16_t absXGradient;
    uint16_t absYGradient;

    pi_cl_alloc_req_t alloc_req;
    pi_cl_free_req_t free_req;
    /* vector that will store the angle of each edge */
    pi_cl_l2_malloc((linesPerCore * imageWidth)*sizeof(angle_t), &alloc_req);
    angle_t * edgeDirection = pi_cl_l2_malloc_wait(&alloc_req);

    /** Section 1:
      * Edge detection
    **/
    for(y = beginning, line = firstLine ; y < end ; ++y, line += imageWidth)
        for(x = 0, pixelIndex = line ; x < imageWidth ; ++x, ++pixelIndex)
            if(x == 0 || y == 0 || x == imageWidth -1 || y == imageHeight -1)
            {
                cannyImg->data[pixelIndex] = MIN_PIXEL_VALUE;
                edgeDirection[pixelIndex - firstLine] = NO_ANGLE;
            }
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
                xGradient  = -img->data[pixelNeighbours[0]];
                xGradient -= (img->data[pixelNeighbours[1]] << 1); /* one left shift == multiplication by 2 */
                xGradient -= img->data[pixelNeighbours[2]];
                xGradient += img->data[pixelNeighbours[4]];
                xGradient += (img->data[pixelNeighbours[5]] << 1);
                xGradient += img->data[pixelNeighbours[6]];

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

                absXGradient = ABS(xGradient);
                absYGradient = ABS(yGradient);

                maxGradient = MAX(absXGradient, absYGradient);
                minGradient = MIN(absXGradient, absYGradient);

                sqrtApprox = maxGradient - (maxGradient >> 3) + (minGradient >> 1);
                sqrtApprox = MAX(sqrtApprox, maxGradient);

                if(sqrtApprox >= MIN_EDGE_INTENSITY_CANNY)
                {
                    cannyImg->data[pixelIndex] = MIN(sqrtApprox, MAX_PIXEL_VALUE);
                    //cannyImg->data[pixelIndex] = MAX_PIXEL_VALUE;

                    /* finding pixel gradient direction */
                    if((absYGradient << 1) <= absXGradient)
                        edgeDirection[pixelIndex - firstLine] = ZERO_DEGREES;
                    else if ((absXGradient << 1) <= absYGradient)
                        edgeDirection[pixelIndex - firstLine] = NINETY_DEGREES;
                    else if ((xGradient < 0 && yGradient < 0) || (xGradient > 0 && yGradient > 0))
                        edgeDirection[pixelIndex - firstLine] = FOURTY_FIVE_DEGREES;
                    else
                        edgeDirection[pixelIndex - firstLine] = A_HUNDRED_AND_THIRTY_FIVE_DEGREES;
                }
                else
                {
                    cannyImg->data[pixelIndex] = MIN_PIXEL_VALUE;
                    edgeDirection[pixelIndex - firstLine] = NO_ANGLE;
                }
            }

    pi_cl_team_barrier(0);

    if(coreId == 0)
    {
        //pi_cl_dma_cmd_t dmaCopyStatus;

        //pi_cl_dma_cmd((uint32_t) cannyImg->data, (uint32_t) img->data, 40000*sizeof(uint8_t), PI_CL_DMA_DIR_EXT2LOC, &dmaCopyStatus);

        /* wait for transfer to end */
        //pi_cl_dma_cmd_wait(&dmaCopyStatus);
        memcpy(img->data, cannyImg->data, 40000*sizeof(uint8_t));

    }

    pi_cl_team_barrier(0);

    /** Section 2:
      * Gradient magnitude thresholding
    **/
    for(y = beginning, line = firstLine ; y < end ; ++y, line += imageWidth)
        for(x = 0, pixelIndex = line ; x < imageWidth ; ++x, ++pixelIndex)
            if(x == 0 || y == 0 || x == imageWidth -1 || y == imageHeight -1)
                continue;
            else
                switch(edgeDirection[pixelIndex - firstLine])
                {
                    case NO_ANGLE:
                        continue;
                        break;

                    case ZERO_DEGREES:
                        pixel = img->data[pixelIndex];
                        /* if the pixel's value is greater than its neighbours in its gradient direction*/
                        if(pixel < img->data[pixelIndex - 1] || pixel < img->data[pixelIndex + 1])
                            cannyImg->data[pixelIndex] = MIN_PIXEL_VALUE;
                        else
                            cannyImg->data[pixelIndex] = MAX_PIXEL_VALUE;

                        break;

                    case FOURTY_FIVE_DEGREES:
                        pixel = img->data[pixelIndex];
                        /* if the pixel's value is greater than its neighbours in its gradient direction*/
                        if(pixel < img->data[pixelIndex - imageWidth + 1] || pixel < img->data[pixelIndex + imageWidth - 1])
                            cannyImg->data[pixelIndex] = MIN_PIXEL_VALUE;
                        else
                            cannyImg->data[pixelIndex] = MAX_PIXEL_VALUE;

                        break;

                    case NINETY_DEGREES:
                        pixel = img->data[pixelIndex];
                        /* if the pixel's value is greater than its neighbours in its gradient direction*/
                        if(pixel < img->data[pixelIndex - imageWidth] || pixel < img->data[pixelIndex + imageWidth])
                            cannyImg->data[pixelIndex] = MIN_PIXEL_VALUE;
                        else
                            cannyImg->data[pixelIndex] = MAX_PIXEL_VALUE;

                        break;

                    case A_HUNDRED_AND_THIRTY_FIVE_DEGREES:
                        pixel = img->data[pixelIndex];
                        /* if the pixel's value is greater than its neighbours in its gradient direction*/
                        if(pixel < img->data[pixelIndex - imageWidth - 1] || pixel < img->data[pixelIndex + imageWidth + 1])
                            cannyImg->data[pixelIndex] = MIN_PIXEL_VALUE;
                        else
                            cannyImg->data[pixelIndex] = MAX_PIXEL_VALUE;

                        break;

                    default:
                        /* never reached */
                        break;
                }


    pi_cl_l2_free(edgeDirection, (linesPerCore * imageWidth)*sizeof(angle_t), &free_req);
    pi_cl_l2_free_wait(&free_req);
}
