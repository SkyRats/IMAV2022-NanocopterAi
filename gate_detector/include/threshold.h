#ifndef __THRESHOLD__
#define __THRESHOLD__

#include <stdint.h>

#include "config.h"
#include "imageIO.h"

/* You can tweak these values as you find fitting */
#ifndef MAX_PIXEL_VALUE
#define MAX_PIXEL_VALUE 255
#endif

#ifndef MIN_PIXEL_VALUE
#define MIN_PIXEL_VALUE 0
#endif

void thresholdImage(PGMImage *img, uint8_t thresholdValue);
void rangeThresholdImage(PGMImage *img, uint8_t lowerThresholdValue, uint8_t upperThresholdValue);
void thresholdAndFindMeans(PGMImage* img, uint8_t upperBound, uint8_t lowerBound, uint8_t *objectMean, uint8_t* backgroundMean);

#endif
