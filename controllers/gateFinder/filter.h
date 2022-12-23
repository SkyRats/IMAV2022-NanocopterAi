#ifndef __FILTERING_H__
#define __FILTERING_H__

#include <stdint.h>

#include "config.h"
#include "imageIO.h"


PGMImage* cl_vectorial_gaussianFilter(PGMImage* img);
PGMImage * lowPixelFilter(PGMImage const * img);

#endif
