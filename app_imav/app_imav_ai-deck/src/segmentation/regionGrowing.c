#include "pmsis.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "config.h"
#include "stack.h"
#include "imageIO.h"
#include "regionGrowing.h"
#include "queue.h"

PQueue * edgeAndGrayShadeSegmentation(PGMImage * img, PGMImage * thresholdedEdgeDetectorOutput, PGMImage * outputImg)
{
    const uint8_t imageWidth = 200, imageHeight = 200;
    const uint16_t imageSize = imageWidth * imageHeight;

    uint8_t y, averageGrayShade, label = 1;
    uint16_t line, pixelIndex, pixelCount;
    uint32_t sum;
    printf("antes da stack e queue");
    Stack* pixelStack = createStack();
    PQueue* labelPQueue = createPQueue();
    printf("depois da stack e queue");

    for(y = 1; y < imageHeight - 1; ++y)
    {
        line = y * imageWidth;
        for(pixelIndex = line + 1; pixelIndex < line + imageWidth - 1; ++pixelIndex)
        {
            if(thresholdedEdgeDetectorOutput->data[pixelIndex] != MAX_PIXEL_VALUE && outputImg->data[pixelIndex] == 0)
            {
                sum = 0;
                pixelCount = 0;
                edgesAndGrayShadesLabelAndCheckNeighbour(img, thresholdedEdgeDetectorOutput, outputImg, label, pixelIndex, pixelStack, &pixelCount, &sum, &averageGrayShade);

                while(!isStackEmpty(pixelStack))
                    edgesAndGrayShadesLabelAndCheckNeighbour(img, thresholdedEdgeDetectorOutput, outputImg, label, pop(pixelStack), pixelStack, &pixelCount, &sum, &averageGrayShade);

                if(MIN_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES <= pixelCount && pixelCount <= MAX_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES)
                {
                    pEnqueue(labelPQueue, (PQueueNode){label, imageSize - pixelCount, NULL});
                    label++;
                }
                else
                    for(uint16_t i = 0; i < imageSize; ++i)
                        if(outputImg->data[i] == label)
                        {
                            outputImg->data[i] = 0;
                            thresholdedEdgeDetectorOutput->data[i] = MAX_PIXEL_VALUE;
                        }
            }

        }
    }

    destroyStack(pixelStack);

    return labelPQueue;
}

void edgesAndGrayShadesLabelAndCheckNeighbour(PGMImage * img, PGMImage * thresholdedEdgeDetectorOutput, PGMImage * outputImg, uint8_t label, uint16_t pixelIndex, Stack * pixelStack, uint16_t * pixelCount, uint32_t * sum, uint8_t * averageGrayShade)
{

    if(outputImg->data[pixelIndex] == label)
        return;
    else
    {
        outputImg->data[pixelIndex] = label;
        *pixelCount += 1;
        *sum += img->data[pixelIndex];
        *averageGrayShade = (*sum)/(*pixelCount);
    }

    const uint8_t imageWidth = 200, imageHeight = 200;
    const uint16_t imageSize = imageWidth * imageHeight;

    if(pixelIndex <= imageWidth || pixelIndex >= imageSize - imageWidth || pixelIndex % imageWidth == 0 || (pixelIndex + 1) % (imageWidth) == 0)
        return;

    uint8_t neighbourPixel;
    uint16_t pixelNeighbours[8];
    int16_t diff;

    pixelNeighbours[0] = pixelIndex - imageWidth - 1; /* upper-left */
    pixelNeighbours[1] = pixelIndex - 1;              /* left */
    pixelNeighbours[2] = pixelIndex + imageWidth - 1; /* bottom-left */
    pixelNeighbours[3] = pixelIndex + imageWidth;     /* bottom */
    pixelNeighbours[4] = pixelIndex + imageWidth + 1; /* bottom-right */
    pixelNeighbours[5] = pixelIndex + 1;              /* right */
    pixelNeighbours[6] = pixelIndex - imageWidth + 1; /* upper-right */
    pixelNeighbours[7] = pixelIndex - imageWidth;     /* upper */

    for(uint8_t i = 0; i < 8; ++i)
    {

        neighbourPixel = img->data[pixelNeighbours[i]];

        diff = *averageGrayShade - neighbourPixel;

        if(thresholdedEdgeDetectorOutput->data[pixelNeighbours[i]] != MAX_PIXEL_VALUE
                && outputImg->data[pixelNeighbours[i]] == 0
                && -MAX_GRAYSHADE_DIFF <= diff && diff <= MAX_GRAYSHADE_DIFF)
            push(pixelStack, pixelNeighbours[i]);
    }

}

PQueue* edgeSegmentation(PGMImage * img)
{
    const uint8_t imageWidth = img->x, imageHeight = img->y;
    const uint16_t imageSize = imageWidth * imageHeight;

    uint8_t y, pixel, label = 1;
    uint16_t line, pixelIndex, pixelCount;
    Stack* pixelStack = createStack();
    PQueue* labelPQueue = createPQueue();

    for(y = 1; y < imageHeight - 1; ++y)
    {
        line = y * imageWidth;
        for(pixelIndex = line + 1; pixelIndex < line + imageWidth - 1; ++pixelIndex)
        {
            pixel = img->data[pixelIndex];

            if(pixel == MAX_PIXEL_VALUE)
            {
                pixelCount = 0;
                edgesLabelAndCheckNeighbour(img, label, pixelIndex, pixelStack, &pixelCount);

                while(!isStackEmpty(pixelStack))
                    edgesLabelAndCheckNeighbour(img, label, pop(pixelStack), pixelStack, &pixelCount);

                if(pixelCount >= MIN_PIXEL_AMOUNT_EDGES_ONLY)
                    pEnqueue(labelPQueue, (PQueueNode){label, imageSize - pixelCount, NULL});


                #ifdef DEBUG_ON
                printf("label %-3u has %-5u pixels.%c", label, pixelCount,label%3==0?'\n':'\t');
                #endif
                label++;
            }
        }
    }

    destroyStack(pixelStack);

    return labelPQueue; /* valid regions found */
}


void edgesLabelAndCheckNeighbour(PGMImage * img, uint8_t label, uint16_t pixelIndex, Stack * pixelStack, uint16_t * pixelCount)
{
    if(img->data[pixelIndex] == label)
        return;
    else
    {
        img->data[pixelIndex] = label;
        *pixelCount += 1;
    }

    const uint8_t imageWidth = img->x, imageHeight = img->y;
    const uint16_t imageSize = imageWidth * imageHeight;

    uint8_t pixel;
    uint16_t pixelNeighbours[8];


    if( pixelIndex <= imageWidth || pixelIndex >= imageSize - imageWidth || pixelIndex % imageWidth == 0 || (pixelIndex + 1) % (imageWidth) == 0)
        return;

    pixelNeighbours[0] = pixelIndex - imageWidth - 1; /* upper-left */
    pixelNeighbours[1] = pixelIndex - 1;              /* left */
    pixelNeighbours[2] = pixelIndex + imageWidth - 1; /* bottom-left */
    pixelNeighbours[3] = pixelIndex + imageWidth;     /* bottom */
    pixelNeighbours[4] = pixelIndex + imageWidth + 1; /* bottom-right */
    pixelNeighbours[5] = pixelIndex + 1;              /* right */
    pixelNeighbours[6] = pixelIndex - imageWidth + 1; /* upper-right */
    pixelNeighbours[7] = pixelIndex - imageWidth;     /* upper */

    for(uint8_t i = 0; i < 8; ++i)
    {
        pixel = img->data[pixelNeighbours[i]];

        if(pixel == MAX_PIXEL_VALUE)
            push(pixelStack, pixelNeighbours[i]);
    }

}
