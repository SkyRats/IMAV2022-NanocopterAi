/* PMSIS includes. */
#include "pmsis.h"

/* Autotiler includes */
#include "Gap8.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "filter.h"

#define ABS(x) (x>0?x:(-x))
#define MAX(x, y) (x>y?x:y)
#define MIN(x, y) (x>y?y:x)

void __attribute__((noinline)) cl_vectorial_gaussianFilter(void * args)
{
    v4u gaussianMask0 = {2, 4, 5, 4},
        gaussianMask1 = {4, 9, 12, 9},
        gaussianMask2 = {5, 12, 15, 12};
    uint8_t normFactor = 32767/159;

    v4u * pixelVectorRetriever;
    v4u   pixelVector0, pixelVector1, pixelVector2,
          pixelVector3, pixelVector4, pixelVector5;

    uint8_t coreId = pi_core_id();
    clusterCallArgs * realArgs = (clusterCallArgs *)args;
    PGMImage * restrict img = realArgs->inputImage;
    PGMImage * restrict convolutedImg = realArgs->outputImage;
    uint8_t nOfCores = realArgs->numOfCores;

    const uint8_t imageWidth = img->x, imageHeight = img->y;

    uint16_t pixelIndex, line;

    /* loop variables */
    uint8_t x, y;
    uint32_t convolutedPixel;
    uint8_t linesPerCore = (imageHeight + nOfCores - 1)/nOfCores; /* rounded up */
    uint8_t beginning = coreId*linesPerCore;
    uint8_t end = MIN(beginning + linesPerCore, imageHeight);

    for(y = beginning, line = beginning * imageWidth; y < end; ++y, line += imageWidth)
        for(x = 0, pixelIndex = line; x < imageWidth; ++x, ++pixelIndex)
            if(x <= 1 || y <= 1 || x >= imageWidth -2 || y >= imageHeight -2)
                convolutedImg->data[pixelIndex] = img->data[pixelIndex];
            else
            {
                /* address of the first pixel under the gaussian filter mask */
                pixelVectorRetriever = (v4u *)(img->data + pixelIndex - (imageWidth << 1) - 2);

                /* copy 4 pixels into the vector and point pixelVectorRetriever to the next row */
                pixelVector0 = *pixelVectorRetriever; pixelVectorRetriever += (imageWidth>>2);
                pixelVector1 = *pixelVectorRetriever; pixelVectorRetriever += (imageWidth>>2);
                pixelVector2 = *pixelVectorRetriever; pixelVectorRetriever += (imageWidth>>2);
                pixelVector3 = *pixelVectorRetriever; pixelVectorRetriever += (imageWidth>>2);
                pixelVector4 = *pixelVectorRetriever;
                pixelVector5 = (v4u){*(img->data + pixelIndex - (imageWidth << 1) + 2), /* upper-right */
                                *(img->data + pixelIndex - (imageWidth)      + 2), /* pixel under */
                                *(img->data + pixelIndex                     + 2), /* right */
                                *(img->data + pixelIndex + (imageWidth)      + 2)}; /* pixel under */

                convolutedPixel = gap8_dotpu4(pixelVector0, gaussianMask0);
                convolutedPixel = gap8_sumdotpu4(pixelVector1, gaussianMask1, convolutedPixel);
                convolutedPixel = gap8_sumdotpu4(pixelVector2, gaussianMask2, convolutedPixel);
                convolutedPixel = gap8_sumdotpu4(pixelVector3, gaussianMask1, convolutedPixel);
                convolutedPixel = gap8_sumdotpu4(pixelVector4, gaussianMask0, convolutedPixel);
                convolutedPixel = gap8_sumdotpu4(pixelVector5, gaussianMask0, convolutedPixel);
                convolutedPixel += ((*(img->data + pixelIndex + (imageWidth << 1)      + 2))<<1);

                convolutedImg->data[pixelIndex] = (convolutedPixel*normFactor)>>15;

            }
}

void cl_lowPixelFilter(void * args)
{
    uint8_t coreId = pi_core_id();
    clusterCallArgs * realArgs = (clusterCallArgs *)args;
    PGMImage * img = realArgs->inputImage;
    PGMImage * outImg = realArgs->outputImage;
    uint8_t nOfCores = realArgs->numOfCores;


    uint16_t idx, line;
    uint16_t idxr[8];
    const uint8_t imageWidth = img->x;
    const uint8_t imageHeight = img->y;

    /* loop variables */
    uint8_t x, y, filteredPixel;
    uint8_t linesPerCore = (imageHeight + nOfCores - 1)/nOfCores; /* rounded up */
    uint8_t beginning = coreId*linesPerCore;
    uint8_t end = beginning + linesPerCore;

    if(end > imageHeight)
        end = imageHeight;

    for(y = beginning; y < end; ++y)
    {
        line = y * imageWidth;
        for(x = 0, idx = line; x < imageWidth; ++idx, ++x)
        {
            if(x == 0 || y == 0 || x == imageWidth - 1 || y == imageHeight -1)
                outImg->data[idx] = img->data[idx];
            else
            {
                /* finding position of each pixel around idx*/
                idxr[0] = idx - imageWidth - 1;
                idxr[1] = idx - 1;
                idxr[2] = idx + imageWidth - 1;
                idxr[3] = idx + imageWidth;
                idxr[4] = idx + imageWidth + 1;
                idxr[5] = idx + 1;
                idxr[6] = idx - imageWidth + 1;
                idxr[7] = idx - imageWidth;

                /* filtering */
                filteredPixel = img->data[idx];
                filteredPixel = img->data[idxr[0]] < filteredPixel ? img->data[idxr[0]] : filteredPixel;
                filteredPixel = img->data[idxr[1]] < filteredPixel ? img->data[idxr[1]] : filteredPixel;
                filteredPixel = img->data[idxr[2]] < filteredPixel ? img->data[idxr[2]] : filteredPixel;
                filteredPixel = img->data[idxr[3]] < filteredPixel ? img->data[idxr[3]] : filteredPixel;
                filteredPixel = img->data[idxr[4]] < filteredPixel ? img->data[idxr[4]] : filteredPixel;
                filteredPixel = img->data[idxr[5]] < filteredPixel ? img->data[idxr[5]] : filteredPixel;
                filteredPixel = img->data[idxr[6]] < filteredPixel ? img->data[idxr[6]] : filteredPixel;
                filteredPixel = img->data[idxr[7]] < filteredPixel ? img->data[idxr[7]] : filteredPixel;

                outImg->data[idx] = filteredPixel;
            }
        }
    }
}
