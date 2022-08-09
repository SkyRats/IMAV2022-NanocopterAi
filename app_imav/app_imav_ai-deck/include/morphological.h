#ifndef __MORPHOLOGICAL_H__
#define __MORPHOLOGICAL_H__

#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "imageIO.h"

PGMImage* maskErosion(PGMImage *img, bool const mask[8]);
PGMImage* maskDilation(PGMImage *img, bool const mask[8]);
void pi_cl_maskErosion(void * args);
void pi_cl_maskDilation(void * args);

#endif

