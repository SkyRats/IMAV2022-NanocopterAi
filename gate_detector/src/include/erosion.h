#ifndef __EROSION__
#define __EROSION__
#include <stdint.h>
#include "vector.h"
#include "imageIO.h"

void erosion(PGMImage *img, Vector *kernel);

#endif
