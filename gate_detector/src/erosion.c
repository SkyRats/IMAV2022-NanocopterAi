#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <erosion.h>
#include <stdbool.h>
#include "imageIO.h"

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
                erodedImg->data[pixel].gray = 0;
            else if(img->data[pixel].gray != 0)
                erodedImg->data[pixel].gray = img->data[pixel].gray;
            else
            {
                pixelNeighbours[0] = line - imageWidth + (i - 1);
                pixelNeighbours[1] = line + (i - 1);
                pixelNeighbours[2] = line + imageWidth + (i - 1);
                pixelNeighbours[3] = line + imageWidth + (i);
                pixelNeighbours[4] = line + imageWidth + (i + 1);
                pixelNeighbours[5] = line + (i + 1);
                pixelNeighbours[6] = line - imageWidth + (i + 1);
                pixelNeighbours[7] = line - imageWidth + (i);

                /* eroding */
                erodedImg->data[pixelNeighbours[0]].gray = mask[0] ? 0 : img->data[pixelNeighbours[0]].gray;
                erodedImg->data[pixelNeighbours[1]].gray = mask[1] ? 0 : img->data[pixelNeighbours[1]].gray;
                erodedImg->data[pixelNeighbours[2]].gray = mask[2] ? 0 : img->data[pixelNeighbours[2]].gray;
                erodedImg->data[pixelNeighbours[3]].gray = mask[3] ? 0 : img->data[pixelNeighbours[3]].gray;
                erodedImg->data[pixelNeighbours[4]].gray = mask[4] ? 0 : img->data[pixelNeighbours[4]].gray;
                erodedImg->data[pixelNeighbours[5]].gray = mask[5] ? 0 : img->data[pixelNeighbours[5]].gray;
                erodedImg->data[pixelNeighbours[6]].gray = mask[6] ? 0 : img->data[pixelNeighbours[6]].gray;
                erodedImg->data[pixelNeighbours[7]].gray = mask[7] ? 0 : img->data[pixelNeighbours[7]].gray;
            }
    }

    return erodedImg;
}
