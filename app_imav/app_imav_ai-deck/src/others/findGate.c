#include "pmsis.h"
#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "imageIO.h"
#include "findGate.h"

#define ABS(x) ((x<0?-x:x))

typedef enum __attribute__((packed))
{
    BEGIN,
    FIND_FIRST,
    FIND_SECOND,
    END
} gateFinderStates;

Point findGate(PGMImage* img, uint8_t Shade)
{
    uint8_t imageWidth = img->x, imageHeight = img->y;
    uint16_t imageSize = imageWidth * imageHeight;

    uint8_t firstPixelH, firstPixelV, secondPixelH, secondPixelV, distanceH = 0, distanceV = 0;
    uint16_t line = 0, column, firstPixelIndex, secondPixelIndex;
    int16_t diff;

    gateFinderStates gateFinderState = FIND_FIRST;

    while(gateFinderState != END)
    {
        switch(gateFinderState)
        {
            /* unnecessary
            case BEGIN:
                line = 0;
                distanceH = 0;
                gateFinderState = FIND_FIRST;
                break;
            */

            case FIND_FIRST:
                firstPixelIndex = line;
                while(firstPixelIndex < imageSize && img->data[firstPixelIndex] !=Shade)
                    firstPixelIndex++;

                if(firstPixelIndex == imageSize)
                {
                    gateFinderState = END;
                    break;
                }

                firstPixelH = firstPixelIndex % imageWidth;
                line = firstPixelIndex - firstPixelH;

                gateFinderState = FIND_SECOND;
                break;

            case FIND_SECOND:
                secondPixelIndex = line + imageWidth - 1;
                while(secondPixelIndex != firstPixelIndex && img->data[secondPixelIndex] !=Shade)
                    secondPixelIndex--;

                if(firstPixelIndex != secondPixelIndex)
                {
                    secondPixelH = secondPixelIndex % imageWidth;
                    diff = secondPixelH - firstPixelH;
                    distanceH = diff > distanceH ? diff : distanceH;
                }

                if(imageSize - line <= imageWidth)
                    gateFinderState = END;
                else
                {
                    gateFinderState = FIND_FIRST;
                    line += imageWidth;
                }

                break;

            /*case END:
                unreachable
                break;*/

            default:
                #ifdef DEBUG_ON
                printf("State does not exist!!!");
                #endif
                gateFinderState = END;
                break;
        }
    }

    #ifdef DEBUG_ON
    printf("\tgreatest horizontal distance: %-3u ", distanceH);
    #endif
    gateFinderState = BEGIN;

    while(gateFinderState != END)
    {
        switch(gateFinderState)
        {
            case BEGIN:
                column = 0;
                distanceV = 0;
                gateFinderState = FIND_FIRST;
                break;

            case FIND_FIRST:
                firstPixelIndex = column;
                while(firstPixelIndex < imageSize && img->data[firstPixelIndex] !=Shade)
                    if(imageSize - firstPixelIndex <= imageWidth)
                    {
                        firstPixelIndex = firstPixelIndex % imageWidth + 1;
                        if(firstPixelIndex == imageWidth)
                            firstPixelIndex = imageSize;
                    }
                    else
                        firstPixelIndex += imageWidth;


                if(firstPixelIndex >= imageSize)
                {
                    gateFinderState = END;
                    break;
                }

                firstPixelV = firstPixelIndex / imageWidth;
                column = firstPixelIndex % imageWidth;

                gateFinderState = FIND_SECOND;
                break;

            case FIND_SECOND:
                secondPixelIndex = column + imageSize - imageWidth;
                while(secondPixelIndex != firstPixelIndex && img->data[secondPixelIndex] !=Shade)
                        secondPixelIndex -= imageWidth;

                if(firstPixelIndex != secondPixelIndex)
                {
                    secondPixelV = secondPixelIndex / imageWidth;
                    diff = secondPixelV - firstPixelV;
                    distanceV =  diff > distanceV ? diff : distanceV;
                }

                if(column == imageWidth - 1)
                    gateFinderState = END;
                else
                {
                    gateFinderState = FIND_FIRST;
                    column++;
                }

                break;

            /*
            case END:
                unreachable
                break;
            */

            default:
                #ifdef DEBUG_ON
                printf("State does not exist!!!");
                #endif
                gateFinderState = END;
                break;
        }
    }

    #ifdef DEBUG_ON
    printf("greatest vertical distance: %-3u\n", distanceV);
    #endif

    //if(-TOL <= diff && diff <= TOL && )
    if(distanceH <= MAX_SQUARE_SIDE_SIZE && distanceV <= MAX_SQUARE_SIDE_SIZE)
    {
        diff = distanceH - distanceV;
        if(ABS(diff) <= TOL)
        {
            uint32_t sumH = 0, sumV = 0;
            uint16_t pixelIndex, counter = 0;
            for(pixelIndex = 0; pixelIndex < imageSize; ++pixelIndex)
                if(img->data[pixelIndex] == Shade)
                {
                    sumH += pixelIndex % imageWidth;
                    sumV += pixelIndex / imageWidth;
                    counter++;
                }
            #ifdef DEBUG_ON
            printf("sumH/counter: %d\n", sumH/counter);
            printf("sumV/counter: %d\n", sumV/counter);
            #endif

            return (Point){sumH / counter, sumV / counter, 1};
        }
    }

    return (Point){0, 0, 0};
}
