#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "filter.h"

PGMImage * lowPixelFilter(PGMImage const * img)
{
    if(img)
    {
        /* loop variables */
        uint8_t x, y, filteredPixel;

        uint16_t idx, line;
        uint16_t idxr[8];
        const uint8_t imageWidth = img->x;
        const uint8_t imageHeight = img->y;

        PGMImage* outImg = (PGMImage *)malloc(sizeof(PGMImage));

        if(outImg == NULL)
            return NULL;

        outImg->x = imageWidth;
        outImg->y = imageHeight;
        outImg->data = (PGMPixel*)malloc(imageWidth * imageHeight * sizeof(PGMPixel));

        if(outImg->data == NULL)
        {
            #ifdef DEBUG_ON
            printf("Allocation of memory for result image of low pixel filtering failed. Exiting.");
            #endif
            return NULL;
        }

        for(y = 0; y < imageHeight; ++y)
        {
            line = y * imageWidth;
            for(x = 0, idx = line; x < imageWidth; ++idx, ++x)
            {
                if(x == 0 || y == 0 || x == imageWidth - 1 || y == imageHeight -1)
                    outImg->data[idx].gray = img->data[idx].gray;
                else
                {
                    /* finding position of each pixel around idx*/
                    idxr[0] = line - imageWidth + (x - 1);
                    idxr[1] = line + (x - 1);
                    idxr[2] = line + imageWidth + (x - 1);
                    idxr[3] = line + imageWidth + (x);
                    idxr[4] = line + imageWidth + (x + 1);
                    idxr[5] = line + (x + 1);
                    idxr[6] = line - imageWidth + (x + 1);
                    idxr[7] = line - imageWidth + (x);

                    /* filtering */
                    filteredPixel = img->data[idx].gray;
                    filteredPixel = img->data[idxr[0]].gray < filteredPixel ? img->data[idxr[0]].gray : filteredPixel;
                    filteredPixel = img->data[idxr[1]].gray < filteredPixel ? img->data[idxr[1]].gray : filteredPixel;
                    filteredPixel = img->data[idxr[2]].gray < filteredPixel ? img->data[idxr[2]].gray : filteredPixel;
                    filteredPixel = img->data[idxr[3]].gray < filteredPixel ? img->data[idxr[3]].gray : filteredPixel;
                    filteredPixel = img->data[idxr[4]].gray < filteredPixel ? img->data[idxr[4]].gray : filteredPixel;
                    filteredPixel = img->data[idxr[5]].gray < filteredPixel ? img->data[idxr[5]].gray : filteredPixel;
                    filteredPixel = img->data[idxr[6]].gray < filteredPixel ? img->data[idxr[6]].gray : filteredPixel;
                    filteredPixel = img->data[idxr[7]].gray < filteredPixel ? img->data[idxr[7]].gray : filteredPixel;

                    outImg->data[idx].gray = filteredPixel;
                }
            }
        }

        return outImg;
    }

    return NULL;
}

void pi_cl_lowPixelFilter(void * args)
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
                outImg->data[idx].gray = img->data[idx].gray;
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
                filteredPixel = img->data[idx].gray;
                filteredPixel = img->data[idxr[0]].gray < filteredPixel ? img->data[idxr[0]].gray : filteredPixel;
                filteredPixel = img->data[idxr[1]].gray < filteredPixel ? img->data[idxr[1]].gray : filteredPixel;
                filteredPixel = img->data[idxr[2]].gray < filteredPixel ? img->data[idxr[2]].gray : filteredPixel;
                filteredPixel = img->data[idxr[3]].gray < filteredPixel ? img->data[idxr[3]].gray : filteredPixel;
                filteredPixel = img->data[idxr[4]].gray < filteredPixel ? img->data[idxr[4]].gray : filteredPixel;
                filteredPixel = img->data[idxr[5]].gray < filteredPixel ? img->data[idxr[5]].gray : filteredPixel;
                filteredPixel = img->data[idxr[6]].gray < filteredPixel ? img->data[idxr[6]].gray : filteredPixel;
                filteredPixel = img->data[idxr[7]].gray < filteredPixel ? img->data[idxr[7]].gray : filteredPixel;

                outImg->data[idx].gray = filteredPixel;
            }
        }
    }
}
