

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "convolution.h"
#include "config.h"


#define ABS(x) (x>0?x:(-x))
#define MAX(x,y) (x>y?x:y)
#define MIN(x,y) (x>y?y:x)

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
                convolutedImg->data[idx].gray = MIN_PIXEL_VALUE <= convolutedPixel && convolutedPixel <= MAX_PIXEL_VALUE ? convolutedPixel : convolutedPixel < MIN_PIXEL_VALUE ? -convolutedPixel : MAX_PIXEL_VALUE;
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
            sobelImg->data[i].gray = gradient_y->data[i].gray;
        }
    }
    free(gradient_x->data);
    free(gradient_y->data);
    free(gradient_x);
    free(gradient_y);
    return sobelImg;

}


PGMImage* cl_cannyOperator(PGMImage *img)
{
   
    PGMImage* cannyImg= (PGMImage *)malloc(sizeof(PGMImage));
    

    const uint8_t imageWidth = img->x, imageHeight = img->y;
    if(cannyImg == NULL)
            return NULL;

    cannyImg->x = imageWidth;
    cannyImg->y = imageHeight;
    cannyImg->data = (PGMPixel*)malloc(imageWidth * imageHeight * sizeof(PGMPixel));
    /* loop variables */
    uint16_t maxGradient, minGradient, sqrtApprox;
    uint8_t x, y, pixel;

    uint16_t pixelIndex, line;
    uint16_t pixelNeighbours[8];

    int16_t xGradient;
    int16_t yGradient;
    uint16_t absXGradient;
    uint16_t absYGradient;

    /* vector that will store the angle of each edge */
    angle_t * edgeDirection = malloc(sizeof(angle_t));

    for(y = 0, line = 0 ; y < imageHeight ; ++y, line += imageWidth)
        for(x = 0, pixelIndex = line ; x < imageWidth ; ++x, ++pixelIndex)
            if(x == 0 || y == 0 || x == imageWidth -1 || y == imageHeight -1)
            {
                cannyImg->data[pixelIndex].gray = MIN_PIXEL_VALUE;
                edgeDirection[pixelIndex] = NO_ANGLE;
                
            }
            else
            {
                /* finding position of each pixel around pixelIndex*/
                
                printf("1\n");
                pixelNeighbours[0] = pixelIndex - imageWidth - 1; /* upper-left */
                pixelNeighbours[1] = pixelIndex - 1;              /* left */
                pixelNeighbours[2] = pixelIndex + imageWidth - 1; /* bottom-left */
                pixelNeighbours[3] = pixelIndex + imageWidth;     /* bottom */
                pixelNeighbours[4] = pixelIndex + imageWidth + 1; /* bottom-right */
                pixelNeighbours[5] = pixelIndex + 1;              /* right */
                pixelNeighbours[6] = pixelIndex - imageWidth + 1; /* upper-right */
                pixelNeighbours[7] = pixelIndex - imageWidth;     /* upper */
                
                printf("2\n");
                /* derivating on x */
                xGradient  = -img->data[pixelNeighbours[0]].gray;
                xGradient -= (img->data[pixelNeighbours[1]].gray << 1); /* one left shift == multiplication by 2 */
                xGradient -= img->data[pixelNeighbours[2]].gray;
                xGradient += img->data[pixelNeighbours[4]].gray;
                xGradient += (img->data[pixelNeighbours[5]].gray << 1);
                xGradient += img->data[pixelNeighbours[6]].gray;
                printf("3\n");
                /* derivating on y */
                yGradient  = img->data[pixelNeighbours[0]].gray;
                yGradient -= img->data[pixelNeighbours[2]].gray;
                yGradient -= (img->data[pixelNeighbours[3]].gray << 1);
                yGradient -= img->data[pixelNeighbours[4]].gray;
                yGradient += img->data[pixelNeighbours[6]].gray;
                yGradient += (img->data[pixelNeighbours[7]].gray << 1);
                printf("4\n");
                /* SRA algorithm -- Square-Root Approximation */
                /** sqrt(xGradient ^2 + yGradient ^2)
                  * approx eq to
                  * max( 0.875 * max(xGradient, yGradient) + 0.5 * min(xGradient, yGradient), max(xGradient, yGradient))
                  */
                 
                absXGradient = ABS(xGradient);
                absYGradient = ABS(yGradient);
                
                maxGradient = MAX(absXGradient, absYGradient);
                minGradient = MIN(absXGradient, absYGradient);
                
                sqrtApprox = maxGradient - (maxGradient >> 3) + (minGradient >> 1);
                sqrtApprox = MAX(sqrtApprox, maxGradient);

                if(sqrtApprox >= MIN_EDGE_INTENSITY_CANNY)
                {
                    printf("5\n");
                    cannyImg->data[pixelIndex].gray = MIN(sqrtApprox, MAX_PIXEL_VALUE);
                    
                    /* finding pixel gradient direction */
                    if((absYGradient << 1) <= absXGradient)
                        edgeDirection[pixelIndex] = ZERO_DEGREES;
                    else if ((absXGradient << 1) <= absYGradient)
                        edgeDirection[pixelIndex] = NINETY_DEGREES;
                    else if ((xGradient < 0 && yGradient < 0) || (xGradient > 0 && yGradient > 0))
                        edgeDirection[pixelIndex ] = FOURTY_FIVE_DEGREES;
                    else
                        edgeDirection[pixelIndex ] = A_HUNDRED_AND_THIRTY_FIVE_DEGREES;
                    printf("6\n");
                }
                else
                {   
                    printf("7\n");
                    cannyImg->data[pixelIndex].gray = MIN_PIXEL_VALUE;
                    edgeDirection[pixelIndex] = NO_ANGLE;
                }
            }

    printf("8\n");
    for(y = 0, line = 0 ; y < imageHeight ; ++y, line += imageWidth)
        for(x = 0, pixelIndex = line ; x < imageWidth ; ++x, ++pixelIndex)
            if(x == 0 || y == 0 || x == imageWidth -1 || y == imageHeight -1)
                continue;
            else
                switch(edgeDirection[pixelIndex ])
                {
                    case NO_ANGLE:
                        continue;
                        break;

                    case ZERO_DEGREES:
                       
                        /* if the pixel's value is greater than its neighbours in its gradient direction*/
                        if(img->data[pixelIndex].gray < img->data[pixelIndex - 1].gray ||img->data[pixelIndex].gray < img->data[pixelIndex + 1].gray)
                            cannyImg->data[pixelIndex].gray = MIN_PIXEL_VALUE;

                        break;

                    case FOURTY_FIVE_DEGREES:
                        
                        /* if the pixel's value is greater than its neighbours in its gradient direction*/
                        if(img->data[pixelIndex].gray < img->data[pixelIndex - imageWidth + 1].gray || img->data[pixelIndex].gray < img->data[pixelIndex + imageWidth - 1].gray)
                            cannyImg->data[pixelIndex].gray = MIN_PIXEL_VALUE;

                        break;

                    case NINETY_DEGREES:
                        
                        /* if the pixel's value is greater than its neighbours in its gradient direction*/
                        if(img->data[pixelIndex].gray < img->data[pixelIndex - imageWidth].gray ||img->data[pixelIndex].gray < img->data[pixelIndex + imageWidth].gray)
                            cannyImg->data[pixelIndex].gray = MIN_PIXEL_VALUE;

                        break;

                    case A_HUNDRED_AND_THIRTY_FIVE_DEGREES:
                        
                        /* if the pixel's value is greater than its neighbours in its gradient direction*/
                        if(img->data[pixelIndex].gray < img->data[pixelIndex - imageWidth - 1].gray || img->data[pixelIndex].gray < img->data[pixelIndex + imageWidth + 1].gray)
                            cannyImg->data[pixelIndex].gray = MIN_PIXEL_VALUE;

                        break;

                    default:
                        /* never reached */
                        break;
                }
    free(edgeDirection);
    return cannyImg;
}
