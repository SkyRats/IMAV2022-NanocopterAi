#ifndef __DISTANCE_TRANSFORM__
#define __DISTANCE_TRANSFORM__
#include <stdint.h>
#include "vector.h"
#include "imageIO.h"

void distanceTransform(PGMImage *img, Vector * markers);

#endif
