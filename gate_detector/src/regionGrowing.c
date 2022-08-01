#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "stack.h"
#include "imageIO.h"
#include "regionGrowing.h"

uint8_t edgeSegmentation(PGMImage * img)
{
    const uint8_t imageWidth = img->x, imageHeight = img->y;

    uint8_t y, pixel, label = 1;
    uint16_t line, pixelIndex;
    Stack* pixelStack = createStack();

    for(y = 1; y < imageHeight - 1; ++y)
    {
        line = y * imageWidth;
        for(pixelIndex = line + 1; pixelIndex < line + imageWidth - 1; ++pixelIndex)
        {
            pixel = img->data[pixelIndex].gray;

            if(pixel == 255)
            {
                labelAndCheckNeighbour(img, label, pixelIndex, pixelStack);

                while(!isStackEmpty(pixelStack))
                    labelAndCheckNeighbour(img, label, pop(pixelStack), pixelStack);

                label++;
            }
        }
    }

    return label - 1; /* amount of regions found */
}


void labelAndCheckNeighbour(PGMImage * img, uint8_t label, uint16_t pixelIndex, Stack * pixelStack)
{
    const uint8_t imageWidth = img->x, imageHeight = img->y;
    const uint16_t imageSize = imageWidth * imageHeight;

    uint8_t pixel;
    uint16_t pixelNeighbours[8];

    img->data[pixelIndex].gray = label;

    if( pixelIndex <= imageWidth || pixelIndex >= imageSize - imageWidth || pixelIndex % imageWidth == 0 || pixelIndex % (imageSize - 1) == 0)
        return;

    pixelNeighbours[0] = pixelIndex - imageWidth - 1;
    pixelNeighbours[1] = pixelIndex - 1;
    pixelNeighbours[2] = pixelIndex + imageWidth - 1;
    pixelNeighbours[3] = pixelIndex + imageWidth;
    pixelNeighbours[4] = pixelIndex + imageWidth + 1;
    pixelNeighbours[5] = pixelIndex + 1;
    pixelNeighbours[6] = pixelIndex - imageWidth + 1;
    pixelNeighbours[7] = pixelIndex - imageWidth;

    for(uint8_t i = 0; i < 8; ++i)
    {

        pixel = img->data[pixelNeighbours[i]].gray;

        if(pixel == 255)
            push(pixelStack, pixelNeighbours[i]);
    }

}
