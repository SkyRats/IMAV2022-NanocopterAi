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

#ifndef MIN_EDGE_INTENSITY_CANNY
/* TODO: test different values */
#define MIN_EDGE_INTENSITY_CANNY 100
#endif

typedef enum __attribute__((packed))
{
    NO_ANGLE = 0,
    ZERO_DEGREES,
    FOURTY_FIVE_DEGREES,
    NINETY_DEGREES,
    A_HUNDRED_AND_THIRTY_FIVE_DEGREES
} angle_t;

PGMImage* convolution3by3(PGMImage const *img, const int8_t mask[9]);
PGMImage* sobel_convolution(PGMImage const *img, const int8_t x_filter[9], const int8_t y_filter[9]);
PGMImage* cl_cannyOperator(PGMImage *img);

#endif

