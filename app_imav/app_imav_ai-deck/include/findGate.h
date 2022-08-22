#ifndef __FIND_GATE_H__
#define __FIND_GATE_H__

#include "config.h"
#include "imageIO.h"

#ifndef TOL
#define TOL 10
#endif

Point findGate(PGMImage* img, uint8_t grayShade);
void clusterMain(void * args);

#endif /* __FIND_GATE_H__ */

