#ifndef __EROSION__
#define __EROSION__

#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "vector.h"
#include "imageIO.h"

PGMImage* maskErosion(PGMImage *img, bool const mask[8]);

#endif
