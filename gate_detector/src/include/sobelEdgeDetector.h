#ifndef __SOBEL_EDGE_DETECTOR__
#define __SOBEL_EDGE_DETECTOR__
#include "imageIO.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

PGMImage* sobelEdgeContrast(PGMImage *img, const uint8_t sobelMask[9]);

#endif

