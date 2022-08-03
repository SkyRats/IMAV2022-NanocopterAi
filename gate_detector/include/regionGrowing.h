#ifndef __REGION_GROWING_H__
#define __REGION_GROWING_H__

#include <stdint.h>

#include "config.h"
#include "imageIO.h"
#include "stack.h"
#include "queue.h"

#ifndef MIN_PIXEL_AMOUNT
#define MIN_PIXEL_AMOUNT 3000
#endif

PQueue* edgeSegmentation(PGMImage * img);
void labelAndCheckNeighbour(PGMImage * img, uint8_t label, uint16_t pixelIndex, Stack * pixelStack);

#endif
