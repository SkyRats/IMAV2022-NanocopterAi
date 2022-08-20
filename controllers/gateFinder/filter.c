
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "filter.h"
#include "imageIO.h"


PGMImage* cl_vectorial_gaussianFilter(PGMImage* img)
{
    
    uint8_t gaussianMask[] = {2, 4, 5, 4, 2, 
                                4, 9, 12, 9, 4, 
                                5, 12, 15, 12, 5, 
                                4, 9, 12, 9, 4,
                                2, 4, 5, 4, 2};

  
    uint8_t normFactor = 32767/159;

    uint8_t pixelVector[25];


    PGMImage * convolutedImg = (PGMImage *)malloc(sizeof(PGMImage));
    const uint8_t imageWidth = img->x, imageHeight = img->y;
    if(convolutedImg == NULL)
            return NULL;

    convolutedImg->x = imageWidth;
    convolutedImg->y = imageHeight;
    convolutedImg->data = (PGMPixel*)malloc(imageWidth * imageHeight * sizeof(PGMPixel));
    uint16_t pixelIndex, line;

    /* loop variables */
    uint8_t x, y;
    uint32_t convolutedPixel;
    
    for(y = 0, line = 0; y < imageHeight; ++y, line += imageWidth)
        for(x = 0, pixelIndex = line; x < imageWidth; ++x, ++pixelIndex)
            if(x <= 2 || y <= 2 || x >= imageWidth -2 || y >= imageHeight -2)
            {   
                convolutedImg->data[pixelIndex].gray = img->data[pixelIndex].gray;
                
               
            }
            else
            {
                
                //Gets 5x5 indexes around the current pixel index
                pixelVector[0] = line - 2*imageWidth + (x-2);
                pixelVector[1] = line - 1*imageWidth + (x-2);
                pixelVector[2] = line + (x-2);
                pixelVector[3] = line + imageWidth + (x-2);
                pixelVector[4] = line + 2*imageWidth + (x-2);
                pixelVector[5] = line + 2*imageWidth + (x-1);
                pixelVector[6] = line + 2*imageWidth + (x);
                pixelVector[7] = line + 2*imageWidth + (x +1);
                pixelVector[8] = line + 2*imageWidth + (x+2);
                pixelVector[9] = line + 1*imageWidth + (x+2);
                pixelVector[10] = line  + (x+2);
                pixelVector[11] = line  - imageWidth + (x+2);
                pixelVector[12] = line  - 2*imageWidth + (x+2);
                pixelVector[13] = line  - 2*imageWidth + (x+1);
                pixelVector[14] = line  - 2*imageWidth + (x);
                pixelVector[15] = line  - 2*imageWidth + (x-1);
                pixelVector[16] = line  - 1*imageWidth + (x-1);
                pixelVector[17] = line + (x-1);
                pixelVector[18] = line  + 1*imageWidth + (x-1);
                pixelVector[19] = line  + 1*imageWidth + (x);
                pixelVector[20] = line  + 1*imageWidth + (x+ 1);
                pixelVector[21] = line  + (x+ 1);
                pixelVector[22] = line  - 1*imageWidth + (x+ 1);
                pixelVector[23] = line  - 1*imageWidth + (x);
                pixelVector[24] = line + x;
                
                
                /*
                convolutedPixel = img->data[pixelVector[0]].gray*gaussianMask[0];
                convolutedPixel += img->data[pixelVector[1]].gray*gaussianMask[1];
                convolutedPixel += img->data[pixelVector[2]].gray*gaussianMask[2];
                convolutedPixel += img->data[pixelVector[3]].gray*gaussianMask[3];
                convolutedPixel += img->data[pixelVector[4]].gray*gaussianMask[4];
                convolutedPixel += img->data[pixelVector[5]].gray*gaussianMask[5];
                convolutedPixel += img->data[pixelVector[6]].gray*gaussianMask[6];
                convolutedPixel += img->data[pixelVector[7]].gray*gaussianMask[7];
                convolutedPixel += img->data[pixelVector[8]].gray*gaussianMask[8];
                convolutedPixel += img->data[pixelVector[9]].gray*gaussianMask[9];
                convolutedPixel += img->data[pixelVector[10]].gray*gaussianMask[10];
                convolutedPixel += img->data[pixelVector[11]].gray*gaussianMask[11];
                convolutedPixel += img->data[pixelVector[12]].gray*gaussianMask[13];
                convolutedPixel += img->data[pixelVector[13]].gray*gaussianMask[14];
                convolutedPixel += img->data[pixelVector[14]].gray*gaussianMask[15];
                convolutedPixel += img->data[pixelVector[15]].gray*gaussianMask[16];
                convolutedPixel += img->data[pixelVector[16]].gray*gaussianMask[17];
                convolutedPixel += img->data[pixelVector[17]].gray*gaussianMask[18];
                convolutedPixel += img->data[pixelVector[18]].gray*gaussianMask[19];
                convolutedPixel += img->data[pixelVector[19]].gray*gaussianMask[20];
                convolutedPixel += img->data[pixelVector[20]].gray*gaussianMask[21];
                convolutedPixel += img->data[pixelVector[21]].gray*gaussianMask[22];
                convolutedPixel += img->data[pixelVector[22]].gray*gaussianMask[23];
                convolutedPixel += img->data[pixelVector[23]].gray*gaussianMask[24];
                convolutedPixel += img->data[pixelVector[24]].gray*gaussianMask[12];
                */
                
                convolutedPixel  = img->data[pixelVector[0]].gray * gaussianMask[0];
                convolutedPixel += img->data[pixelVector[1]].gray * gaussianMask[5];
                convolutedPixel += img->data[pixelVector[2]].gray * gaussianMask[10];
                convolutedPixel += img->data[pixelVector[3]].gray * gaussianMask[15];
                convolutedPixel += img->data[pixelVector[4]].gray * gaussianMask[20];
                convolutedPixel += img->data[pixelVector[5]].gray * gaussianMask[21];
                convolutedPixel += img->data[pixelVector[6]].gray * gaussianMask[22];
                convolutedPixel += img->data[pixelVector[7]].gray * gaussianMask[23];
                convolutedPixel += img->data[pixelVector[8]].gray * gaussianMask[24];
                convolutedPixel += img->data[pixelVector[9]].gray * gaussianMask[19];
                convolutedPixel += img->data[pixelVector[10]].gray * gaussianMask[14];
                convolutedPixel += img->data[pixelVector[11]].gray * gaussianMask[9];
                convolutedPixel += img->data[pixelVector[12]].gray * gaussianMask[4];
                convolutedPixel += img->data[pixelVector[13]].gray * gaussianMask[3];
                convolutedPixel += img->data[pixelVector[14]].gray * gaussianMask[2];
                convolutedPixel += img->data[pixelVector[15]].gray * gaussianMask[1];
                convolutedPixel += img->data[pixelVector[16]].gray * gaussianMask[6];
                convolutedPixel += img->data[pixelVector[17]].gray * gaussianMask[11];
                convolutedPixel += img->data[pixelVector[18]].gray * gaussianMask[16];
                convolutedPixel += img->data[pixelVector[19]].gray * gaussianMask[17];
                convolutedPixel += img->data[pixelVector[20]].gray * gaussianMask[18];
                convolutedPixel += img->data[pixelVector[21]].gray * gaussianMask[13];
                convolutedPixel += img->data[pixelVector[22]].gray * gaussianMask[8];
                convolutedPixel += img->data[pixelVector[23]].gray * gaussianMask[7];
                convolutedPixel += img->data[pixelVector[24]].gray * gaussianMask[12];
                
                uint32_t finalPixel = (convolutedPixel*normFactor)>>15;
                convolutedImg->data[pixelIndex].gray = MIN_PIXEL_VALUE <= finalPixel && finalPixel <= MAX_PIXEL_VALUE ? finalPixel : finalPixel < MIN_PIXEL_VALUE ? -finalPixel : MAX_PIXEL_VALUE;
                //convolutedImg->data[pixelIndex].gray = (convolutedPixel*normFactor)>>15;
                
            }
    return convolutedImg;
}

PGMImage * lowPixelFilter(PGMImage const * img)
{
    if(img)
    {
        /* loop variables */
        uint8_t x, y, filteredPixel;

        uint16_t idx, line;
        uint16_t pixelVector[8];
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
                    pixelVector[0] = line - imageWidth + (x - 1);
                    pixelVector[1] = line + (x - 1);
                    pixelVector[2] = line + imageWidth + (x - 1);
                    pixelVector[3] = line + imageWidth + (x);
                    pixelVector[4] = line + imageWidth + (x + 1);
                    pixelVector[5] = line + (x + 1);
                    pixelVector[6] = line - imageWidth + (x + 1);
                    pixelVector[7] = line - imageWidth + (x);

                    /* filtering */
                    filteredPixel = img->data[idx].gray;
                    filteredPixel = img->data[pixelVector[0]].gray < filteredPixel ? img->data[pixelVector[0]].gray : filteredPixel;
                    filteredPixel = img->data[pixelVector[1]].gray < filteredPixel ? img->data[pixelVector[1]].gray : filteredPixel;
                    filteredPixel = img->data[pixelVector[2]].gray < filteredPixel ? img->data[pixelVector[2]].gray : filteredPixel;
                    filteredPixel = img->data[pixelVector[3]].gray < filteredPixel ? img->data[pixelVector[3]].gray : filteredPixel;
                    filteredPixel = img->data[pixelVector[4]].gray < filteredPixel ? img->data[pixelVector[4]].gray : filteredPixel;
                    filteredPixel = img->data[pixelVector[5]].gray < filteredPixel ? img->data[pixelVector[5]].gray : filteredPixel;
                    filteredPixel = img->data[pixelVector[6]].gray < filteredPixel ? img->data[pixelVector[6]].gray : filteredPixel;
                    filteredPixel = img->data[pixelVector[7]].gray < filteredPixel ? img->data[pixelVector[7]].gray : filteredPixel;

                    outImg->data[idx].gray = filteredPixel;
                }
            }
        }

        return outImg;
    }

    return NULL;
}
