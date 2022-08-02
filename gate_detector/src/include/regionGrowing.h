#ifndef __REGION_GROWING_H__
#define __REGION_GROWING_H__
#include "imageIO.h"
#include "stack.h"
#include "queue.h"

PQueue* edgeSegmentation(PGMImage * img);
void labelAndCheckNeighbour(PGMImage * img, uint8_t label, uint16_t pixelIndex, Stack * pixelStack);

#endif
