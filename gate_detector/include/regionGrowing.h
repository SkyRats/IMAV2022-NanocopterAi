#ifndef __REGION_GROWING_H__
#define __REGION_GROWING_H__

#include <stdint.h>

#include "config.h"
#include "imageIO.h"
#include "stack.h"
#include "queue.h"

#ifndef MIN_PIXEL_AMOUNT_EDGES_ONLY
#define MIN_PIXEL_AMOUNT_EDGES_ONLY 3000
#endif

#ifndef MIN_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES
#define MIN_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES 0
#endif

#ifndef MAX_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES
#define MAX_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES 40000
#endif

#ifndef MAX_GRAYSHADE_DIFF
#define MAX_GRAYSHADE_DIFF 10
#endif

PQueue* edgeSegmentation(PGMImage * img);
void edgesLabelAndCheckNeighbour(PGMImage * img, uint8_t label, uint16_t pixelIndex, Stack * pixelStack, uint16_t * pixelCount);
PQueue * edgeAndGrayShadeSegmentation(PGMImage * img, PGMImage * thresholdedEdgeDetectorOutput, PGMImage * outputImg);
void edgesAndGrayShadesLabelAndCheckNeighbour(PGMImage * img, PGMImage * thresholdedEdgeDetectorOutput, PGMImage * outputImg, uint8_t label, uint16_t pixelIndex, Stack * pixelStack, uint16_t * pixelCount, uint32_t * sum, uint8_t * averageGrayShade);

#endif
