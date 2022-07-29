#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "distanceTransform.h"
#include "queue.h"
#include "imageIO.h"

void distanceTransform(PGMImage *img, Vector * markers)
{
    Queue* pixelIndexQueue = createQueue(((img->x)*(img->y)) >> 4);
    int8_t dx[8] = {-1, 1, 0, 0, -1, -1, 1, 1},
           dy[8] = {0, 0, -1, 1, -1, 1, 1, -1};

    bool* visited = calloc(img->x * img->y, sizeof(bool));
    if( visited == NULL)
    {
        #ifdef DEBUG_ON
        printf("Allocation of \"visited\" array failed (distanceTransform). Exiting.");
        #endif
        return;
    }

    for(uint8_t y = 1 ; y < img->y -1; ++y)
        for( uint8_t x = 1 ; x < img->x -1; ++x)
        {
            uint16_t pixelIndex = y * img->x + x;

            if( img->data[pixelIndex].gray != 0)
                continue;

            for( uint8_t h = 0 ; h < 4 ; ++h)
            {
                uint16_t nextPixelIndex = pixelIndex + dy[h]*img->x + dx[h];

                if(!visited[nextPixelIndex] && img->data[nextPixelIndex].gray == 255)
                {
                    visited[nextPixelIndex] = true;
                    img->data[nextPixelIndex].gray = 50;
                    enqueue(pixelIndexQueue, nextPixelIndex);
                }
            }

        }

    free(visited);

    while(!queueIsEmpty(pixelIndexQueue))
    {
        uint16_t currentPixelIndex = dequeue(pixelIndexQueue);
        uint16_t nextPixelIndex;
        uint8_t currentPixel = img->data[currentPixelIndex].gray;
        uint8_t nextPixel;
        bool isBigger = true;

        for(uint8_t h = 0; h < 8 ; ++h)
        {
            nextPixelIndex = currentPixelIndex + dy[h]*img->x + dx[h];
            nextPixel = img->data[nextPixelIndex].gray;

            if(nextPixel == 0)
                continue;

            if(currentPixel <= nextPixel)
            {
                isBigger = false;
                if(currentPixel + 1 < nextPixel)
                {
                    img->data[nextPixelIndex].gray = currentPixel + 1 < 254 ? currentPixel + 1 : 254;
                    enqueue(pixelIndexQueue, nextPixelIndex);
                }
            }
        }

        if(isBigger)
            vector_add(markers, currentPixelIndex);
    }
}
