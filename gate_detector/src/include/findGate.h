#ifndef __FIND_GATE_H__
#define __FIND_GATE_H__
#include "imageIO.h"

#define TOL 10

Point findGate(PGMImage* img, uint8_t grayShade);

#endif /* __FIND_GATE_H__ */

