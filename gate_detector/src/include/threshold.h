#ifndef __THRESHOLD__
#define __THRESHOLD__
#include <stdint.h>
#include "ppmReader.h"

/* You can tweak these values as you find fitting */
#define MAX_PIXEL_VALUE 255
#define MIN_PIXEL_VALUE 0

void thresholdImage(PPMGRAYImage *img, uint8_t thresholdValue);

#endif
