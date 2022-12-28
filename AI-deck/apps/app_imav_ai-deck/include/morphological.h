#ifndef __MORPHOLOGICAL_H__
#define __MORPHOLOGICAL_H__

#include <stdint.h>
#include <stdbool.h>

#include "config.h"
#include "imageIO.h"

void cl_maskErosion(void * args);
void cl_maskDilation(void * args);

#endif

