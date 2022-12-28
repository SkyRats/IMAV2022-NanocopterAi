#include "pmsis.h"
/* PMSIS includes. */
#include "pmsis.h"

#include "threshold.h"
#include <stdlib.h>


void thresholdImage(PGMImage * restrict img, uint8_t thresholdValue)
{
    int i;

    if(img)
         for(i = 0; i < img->x * img->y ; ++i)
              img->data[i] = img->data[i] >= thresholdValue ? MAX_PIXEL_VALUE : MIN_PIXEL_VALUE;
}


void rangeThresholdImage(PGMImage *img, uint8_t lowerThresholdValue, uint8_t upperThresholdValue)
{
    uint16_t i;
    uint8_t pixel;

    if(img)
        for(i = 0; i < img->x * img->y ; ++i)
        {
            pixel = img->data[i];
            img->data[i] = (pixel >= lowerThresholdValue && pixel <= upperThresholdValue) ? MAX_PIXEL_VALUE : MIN_PIXEL_VALUE;
        }
}

void thresholdAndFindMeans(PGMImage* img, uint8_t upperBound, uint8_t lowerBound, uint8_t *restrict objectMean, uint8_t * restrict backgroundMean)
{
    uint32_t object = 0, background = 0;
    uint8_t imageWidth = img->x, imageHeight = img->y, pixel;
    uint16_t imageSize = imageWidth*imageHeight, counter = 0;

    for(uint16_t i = 0; i < imageSize; ++i)
    {
        pixel = img->data[i];
        if(pixel >= lowerBound && pixel <= upperBound)
        {
            img->data[i] = MAX_PIXEL_VALUE;
            counter++;
            object += pixel;
        }
        else
        {
            img->data[i] = MIN_PIXEL_VALUE;
            background += pixel;
        }
    }

    *objectMean = object/counter;
    *backgroundMean = background/(imageSize - counter);
}

