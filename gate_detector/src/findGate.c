#include <stdlib.h>
#include <stdio.h>
#include "imageIO.h"
#include "findGate.h"

typedef enum
{
    BEGIN,
    FIND_FIRST,
    FIND_SECOND,
    END
} gateFinderStates;

Point findGate(PGMImage* img, uint8_t grayShade)
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
                while(firstPixelIndex < imageSize && img->data[firstPixelIndex].gray != grayShade)
                    firstPixelIndex++;

                if(firstPixelIndex == imageSize)
                {
                    gateFinderState = END;
                    break;
                }
                #ifdef DEBUG_ON
                printf("first x: %-5u ", firstPixelIndex % imageWidth);
                #endif

                firstPixelH = firstPixelIndex % imageWidth;
                line = firstPixelIndex - firstPixelH;

                gateFinderState = FIND_SECOND;
                break;

            case FIND_SECOND:
                secondPixelIndex = line + imageWidth - 1;
                while(secondPixelIndex != firstPixelIndex && img->data[secondPixelIndex].gray != grayShade)
                    secondPixelIndex--;

                if(firstPixelIndex != secondPixelIndex)
                {
                    #ifdef DEBUG_ON
                    printf("second x: %-5u\n", secondPixelIndex % imageWidth);
                    #endif
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
                while(firstPixelIndex < imageSize && img->data[firstPixelIndex].gray != grayShade)
                    if(imageSize - firstPixelIndex <= imageWidth)
                    {
                        firstPixelIndex = firstPixelIndex % imageWidth + 1;
                        if(firstPixelIndex == imageWidth)
                            firstPixelIndex = imageSize;
                    }
                    else
                        firstPixelIndex += imageWidth;

                #ifdef DEBUG_ON
                printf("first y: %-5u ", firstPixelIndex / imageWidth);
                #endif

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
                while(secondPixelIndex != firstPixelIndex && img->data[secondPixelIndex].gray != grayShade)
                        secondPixelIndex -= imageWidth;

                if(firstPixelIndex != secondPixelIndex)
                {
                    #ifdef DEBUG_ON
                    printf("second y: %-5u\n", secondPixelIndex / imageWidth);
                    #endif
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

    diff = distanceH - distanceV;
    if(-TOL <= diff && diff <= TOL)
    {
        uint32_t sum = 0, counter;
        uint16_t pixelIndex;
        for(pixelIndex = 0; pixelIndex < imageSize; ++pixelIndex)
            if(img->data[pixelIndex].gray == grayShade)
            {
                sum += pixelIndex;
                counter++;
            }
        pixelIndex = sum/counter;
        return (Point){pixelIndex % imageWidth, pixelIndex / imageWidth, img->data[pixelIndex].gray};
    }

    return (Point){0, 0, 0};
}
