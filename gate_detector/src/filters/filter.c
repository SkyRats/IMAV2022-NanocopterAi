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
