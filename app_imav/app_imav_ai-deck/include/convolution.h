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

void cl_convolution3by3(void * args);
void cl_sobelOperator(void * args);

#endif

