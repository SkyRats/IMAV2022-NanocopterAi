#ifndef ___HISTOGRAM_H__
#define ___HISTOGRAM_H__

#include <stdint.h>

#include "config.h"
#include "imageIO.h"

#ifndef PEAK_SPACE
#define PEAK_SPACE 50
#endif

void findValley(uint16_t *restrict , uint8_t , uint8_t , uint8_t *restrict );
void peaksBoundsPeakTechnique(uint16_t*,uint8_t,uint8_t,uint8_t *restrict,uint8_t *restrict);
void peaksBoundsValleyTechnique(uint16_t *restrict, uint8_t , uint8_t , uint8_t *restrict ,uint8_t *restrict );
void findPeaks(uint16_t * restrict,uint16_t *restrict,uint16_t *restrict);
void calculateHistogram(PGMImage const*,uint16_t*);
void smoothHistogram(uint16_t*);
void histogramPeakTechnique(PGMImage*);
void histogramValleyTechnique(PGMImage*);
void adaptiveHistogramTechnique(PGMImage*);

#endif