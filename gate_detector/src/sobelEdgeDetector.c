#include "sobelEdgeDetector.h"
#include <stdint.h>
#include <stdlib.h>

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

PGMImage* sobelEdgeContrast(PGMImage *img, const uint8_t sobelMask[9])
{
    if(!img)
        return NULL;

    int idx;
    int idxr[8];

    PGMImage* sobelImg = (PGMImage *)malloc(sizeof(PGMImage));
    if(sobelImg == NULL)
        return NULL;
    sobelImg->x = img->x;
    sobelImg->y = img->y;
    sobelImg->data = (PGMPixel*)malloc(sobelImg->x * sobelImg->y * sizeof(PGMPixel));
    if(sobelImg->data == NULL)
        return NULL;

    for(int y = 0; y < img->y; ++y)
    {
        for(int x = 0; x < img->x;++x)
        {
            /* pixel to be convoluted */
            idx = y * img->x + x;

            if(x == 0 || y == 0 || x == img->x -1 || y == img->y -1)
                sobelImg->data[idx].gray = 0;
            else
            {
                /* finding position of each pixel around idx*/
                idxr[0] = (y - 1)* img->x + (x - 1);
                idxr[1] = (y)* img->x + (x - 1);
                idxr[2] = (y + 1)* img->x + (x - 1);
                idxr[3] = (y + 1)* img->x + (x);
                idxr[4] = (y + 1)* img->x + (x + 1);
                idxr[5] = (y)* img->x + (x + 1);
                idxr[6] = (y - 1)* img->x + (x + 1);
                idxr[7] = (y - 1)* img->x + (x);

                /* applying mask */
                sobelImg->data[idx].gray = img->data[idxr[0]].gray*sobelMask[0];
                sobelImg->data[idx].gray+= img->data[idxr[1]].gray*sobelMask[1];
                sobelImg->data[idx].gray+= img->data[idxr[2]].gray*sobelMask[2];
                sobelImg->data[idx].gray+= img->data[idxr[3]].gray*sobelMask[3];
                sobelImg->data[idx].gray+= img->data[idxr[5]].gray*sobelMask[5];
                sobelImg->data[idx].gray+= img->data[idxr[6]].gray*sobelMask[6];
                sobelImg->data[idx].gray+= img->data[idxr[7]].gray*sobelMask[7];
                sobelImg->data[idx].gray+= img->data[idxr[8]].gray*sobelMask[8];

                /* value of the pixel itself*/
                sobelImg->data[idx].gray+= img->data[idxr[4]].gray*sobelMask[4];
            }
        }
    }

    return sobelImg;
}
