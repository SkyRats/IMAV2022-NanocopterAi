#ifndef __WATERSHED_H__
#define __WATERSHED_H__
#include <stdint.h>
#include "vector.h"
#include "imageIO.h"

void erosion(PGMImage *img, Vector *kernel);
void distanceTransform(PGMImage *img, Vector * markers);
PGMImage* watershed(PGMImage *image, Vector *Markers);

#endif
