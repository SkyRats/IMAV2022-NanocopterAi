#ifndef __SOBEL_EDGE_DETECTOR__
#define __SOBEL_EDGE_DETECTOR__
#include "imageIO.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

PGMImage* convolution3by3(PGMImage const *img, const int8_t mask[9]);

#endif

