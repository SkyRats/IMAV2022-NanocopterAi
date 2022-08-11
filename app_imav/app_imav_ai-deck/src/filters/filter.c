/* PMSIS includes. */
#include "pmsis.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "filter.h"

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
    pi_cl_team_barrier(0);
}
