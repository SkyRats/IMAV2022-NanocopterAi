#include "threshold.h"


void thresholdImage(PGMImage *img, uint8_t thresholdValue)
{
    int i;

    if(img)
         for(i = 0; i < img->x * img->y ; ++i)
              img->data[i].gray = img->data[i].gray >= thresholdValue ? MAX_PIXEL_VALUE : MIN_PIXEL_VALUE;
}


