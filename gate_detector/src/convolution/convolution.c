#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "convolution.h"

/* masks for sobel operator */
/*

const uint8_t sobel_mask_0[9] =
   { 1, 2, 1,
    0, 0, 0,
   -1, -2, -1 };


const uint8_t sobel_mask_1[9] =
   { 2, 1, 0,
    1, 0, -1,
    0, -1, -2 };


const uint8_t sobel_mask_2[9] =
   { 1, 0, -1,
    2, 0, -2,
    1, 0, -1 };


const uint8_t sobel_mask_3[9] =
   { 0, -1, -2,
    1, 0, -1,
    2, 1, 0 };


const uint8_t sobel_mask_4[9] =
   {-1, -2, -1,
    0, 0, 0,
    1, 2, 1 };


const uint8_t sobel_mask_5[9] =
   {-2, -1, 0,
   -1, 0, 1,
    0, 1, 2 };


const uint8_t sobel_mask_6[9] =
   {-1, 0, 1,
   -2, 0, 2,
   -1, 0, 1 };


const uint8_t sobel_mask_7[9] =
    { 0, 1, 2,
    -1, 0, 1,
    -2, -1, 0 };
*/

PGMImage* convolution3by3(PGMImage const *img, const int8_t mask[9])
{
    const uint8_t imageWidth = img->x, imageHeight = img->y;
    const uint16_t imageSize = imageWidth * imageHeight;

    uint8_t x, y;
    int32_t convolutedPixel;

    uint16_t idx, line;
    uint16_t idxr[8];

    PGMImage* convolutedImg = (PGMImage *)malloc(sizeof(PGMImage));

    if(convolutedImg == NULL)
        return NULL;

    convolutedImg->x = imageWidth;
    convolutedImg->y = imageHeight;
    convolutedImg->data = (PGMPixel*)malloc(imageSize * sizeof(PGMPixel));

    if(convolutedImg->data == NULL)
        return NULL;

    for(y = 0; y < imageHeight; ++y)
    {
        line = y * imageWidth;
        for(x = 0, idx = line; x < imageWidth; ++x, ++idx)
        {
            if(x == 0 || y == 0 || x == imageWidth -1 || y == imageHeight -1)
                convolutedImg->data[idx].gray = MIN_PIXEL_VALUE;
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

                /* applying mask */
                convolutedPixel  = img->data[idxr[0]].gray * mask[0];
                convolutedPixel += img->data[idxr[1]].gray * mask[1];
                convolutedPixel += img->data[idxr[2]].gray * mask[2];
                convolutedPixel += img->data[idxr[3]].gray * mask[3];
                convolutedPixel += img->data[idxr[4]].gray * mask[5];
                convolutedPixel += img->data[idxr[5]].gray * mask[6];
                convolutedPixel += img->data[idxr[6]].gray * mask[7];
                convolutedPixel += img->data[idxr[7]].gray * mask[8];

                /* value of the pixel itself*/
                convolutedPixel += img->data[idx].gray * mask[4];
                if(convolutedPixel<0){
                    convolutedPixel = -convolutedPixel;
                }
                convolutedImg->data[idx].gray = convolutedPixel;
                //convolutedImg->data[idx].gray = MIN_PIXEL_VALUE <= convolutedPixel && convolutedPixel <= MAX_PIXEL_VALUE ? convolutedPixel : convolutedPixel < MIN_PIXEL_VALUE ? -convolutedPixel : MAX_PIXEL_VALUE;
            }
        }
    }

    return convolutedImg;
}


PGMImage* sobel_convolution(PGMImage const *img, const int8_t x_filter[9], const int8_t y_filter[9]){
    const uint8_t imageWidth = img->x, imageHeight = img->y;
    const uint16_t imageSize = imageWidth * imageHeight;

    PGMImage* gradient_x = convolution3by3(img, x_filter); 
    PGMImage* gradient_y = convolution3by3(img, y_filter); 

    PGMImage* sobelImg = (PGMImage *)malloc(sizeof(PGMImage));
    sobelImg->x = imageWidth;
    sobelImg->y = imageHeight;
    sobelImg->data = (PGMPixel*)malloc(imageSize * sizeof(PGMPixel));


    for(uint16_t i=0; i<imageHeight*imageWidth; i++){
        if(gradient_x->data[i].gray > gradient_y->data[i].gray){
            sobelImg->data[i].gray = gradient_x->data[i].gray;
        }
        else{
            sobelImg->data[i].gray = gradient_x->data[i].gray;
        }
    }
    free(gradient_x->data);
    free(gradient_y->data);
    free(gradient_x);
    free(gradient_y);
    return sobelImg;

}