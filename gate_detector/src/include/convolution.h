#ifndef __SOBEL_EDGE_DETECTOR__
#define __SOBEL_EDGE_DETECTOR__

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "config.h"
#include "imageIO.h"

#ifndef MAX_PIXEL_VALUE
#define MAX_PIXEL_VALUE 255
#endif

#ifndef MIN_PIXEL_VALUE
#define MIN_PIXEL_VALUE 0
#endif

PGMImage* convolution3by3(PGMImage const *img, const int8_t mask[9]);

#endif

