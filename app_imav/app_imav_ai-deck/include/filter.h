#ifndef __FILTERING_H__
#define __FILTERING_H__

#include <stdint.h>

#include "config.h"
#include "imageIO.h"

PGMImage * lowPixelFilter(PGMImage const * img);
void pi_cl_lowPixelFilter(void * args);

#endif
