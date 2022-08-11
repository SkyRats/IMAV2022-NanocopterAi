#include "pmsis.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "histogram.h"
#include "threshold.h"

void findValley(uint16_t *restrict histogram, uint8_t firstPeak, uint8_t secondPeak, uint8_t *restrict valley)
{
    uint8_t i,
            max = firstPeak < secondPeak ? secondPeak : firstPeak;

    for(i = firstPeak < secondPeak ? firstPeak + 2 : secondPeak + 2; i < max; ++i)
        *valley = histogram[*valley] > histogram[i] ? i : *valley;
}

void peaksBoundsValleyTechnique(uint16_t *restrict histogram, uint8_t firstPeak, uint8_t secondPeak, uint8_t *restrict upperBound,uint8_t *restrict lowerBound)
{
    uint8_t i, valley = firstPeak < secondPeak ? firstPeak + 1 : secondPeak + 1;
    uint32_t sum1 = 0, sum2 = 0;

    findValley(histogram, firstPeak, secondPeak, &valley);
    #ifdef DEBUG_ON
    printf("valley: %u\n", valley);
    #endif

    for(i = 0; i < valley; ++i)
        sum1 += histogram[i];

    for(i=valley; i<MAX_PIXEL_VALUE - MIN_PIXEL_VALUE; ++i)
        sum2 += histogram[i];
    sum2 += histogram[MAX_PIXEL_VALUE - MIN_PIXEL_VALUE];

    if(sum1 >= sum2)
    {
        *lowerBound = valley;
        *upperBound = MAX_PIXEL_VALUE;
    }
    else
    {
        *lowerBound = MIN_PIXEL_VALUE;
        *upperBound = valley;
    }
}

void peaksBoundsPeakTechnique(uint16_t *restrict histogram, uint8_t firstPeak, uint8_t secondPeak, uint8_t *restrict upperBound,uint8_t *restrict lowerBound)
{
    uint8_t i, midPoint = (firstPeak - secondPeak > 0) ? ((firstPeak + secondPeak)>>1) : ((secondPeak + firstPeak)>>1);
    uint32_t sum1 = 0, sum2 = 0;

    for(i = 0; i < midPoint; ++i)
        sum1 += histogram[i];

    for(i=midPoint; i<MAX_PIXEL_VALUE - MIN_PIXEL_VALUE; ++i)
        sum2 += histogram[i];
    sum2 += histogram[MAX_PIXEL_VALUE - MIN_PIXEL_VALUE];

    if(sum1 >= sum2)
    {
        *lowerBound = midPoint;
        *upperBound = MAX_PIXEL_VALUE;
    }
    else
    {
        *lowerBound = MIN_PIXEL_VALUE;
        *upperBound = midPoint;
    }
}

void findPeaks(uint16_t * restrict histogram, uint16_t  *restrict firstPeak, uint16_t  * restrict secondPeak)
{
    uint8_t i;
    int16_t diff;

    for(i = 0; i < MAX_PIXEL_VALUE - MIN_PIXEL_VALUE; ++i)
        if(histogram[i] > histogram[*firstPeak])
            *firstPeak = i - MIN_PIXEL_VALUE;

    if(histogram[MAX_PIXEL_VALUE - MIN_PIXEL_VALUE] > histogram[*firstPeak])
        *firstPeak = MAX_PIXEL_VALUE - MIN_PIXEL_VALUE;

    for(i = 0; i < MAX_PIXEL_VALUE - MIN_PIXEL_VALUE; ++i)
    {
        diff = *firstPeak - i;
        diff = diff > 0 ? diff : -diff;
        if(histogram[i] > histogram[*secondPeak] && diff > PEAK_SPACE)
            *secondPeak = i - MIN_PIXEL_VALUE;
    }

    diff = *firstPeak - i;
    diff = diff > 0 ? diff : -diff;
    if(histogram[MAX_PIXEL_VALUE - MIN_PIXEL_VALUE] > histogram[*secondPeak] && diff > PEAK_SPACE)
        *secondPeak = MAX_PIXEL_VALUE - MIN_PIXEL_VALUE;
}

void calculateHistogram(PGMImage const* img, uint16_t* histogram)
{
    uint16_t i, size = img->x * img->y;
    uint8_t index, pixel;

    for(i = 0; i < size; i++)
    {
        pixel = img->data[i];
        index =  (pixel <= MAX_PIXEL_VALUE ? pixel : MAX_PIXEL_VALUE) - MIN_PIXEL_VALUE;
        histogram[index < 0 ? 0 : index] += 1;
    }
}

void smoothHistogram(uint16_t *restrict histogram)
{
    uint8_t i = 1;
    uint16_t *newHistogram = pmsis_l2_malloc(((MAX_PIXEL_VALUE + 1) - MIN_PIXEL_VALUE) * sizeof(uint16_t));
    memset(newHistogram, 0, ((MAX_PIXEL_VALUE + 1) - MIN_PIXEL_VALUE) * sizeof(uint16_t));

    newHistogram[0] = (histogram[0] + histogram[1]) >> 1;/*divide by 2*/
    newHistogram[MAX_PIXEL_VALUE - MIN_PIXEL_VALUE] = (histogram[MAX_PIXEL_VALUE - MIN_PIXEL_VALUE] + histogram[MAX_PIXEL_VALUE - 1 - MIN_PIXEL_VALUE]) >> 1;

    do
    {
        newHistogram[i] = (histogram[i-1] + histogram[i] + histogram[i+1])/3;
        ++i;
    }while(i != MAX_PIXEL_VALUE - MIN_PIXEL_VALUE);

    for(i = 0; i < MAX_PIXEL_VALUE - MIN_PIXEL_VALUE; ++i)
        histogram[i] = newHistogram[i];

    histogram[MAX_PIXEL_VALUE - MIN_PIXEL_VALUE] = newHistogram[MAX_PIXEL_VALUE - MIN_PIXEL_VALUE];

    pmsis_l2_malloc_free(newHistogram, ((MAX_PIXEL_VALUE + 1) -MIN_PIXEL_VALUE) * sizeof(uint16_t));
}


void histogramPeakTechnique(PGMImage* img)
{
    uint16_t firstPeak = MAX_PIXEL_VALUE + 1, secondPeak = MAX_PIXEL_VALUE + 1;
    uint8_t upperBound, lowerBound;

    uint16_t *histogram = pmsis_l2_malloc((MAX_PIXEL_VALUE + 2 -MIN_PIXEL_VALUE) * sizeof(uint16_t));
    memset(histogram, 0, (MAX_PIXEL_VALUE + 2 -MIN_PIXEL_VALUE) * sizeof(uint16_t));

    calculateHistogram(img, histogram);
    smoothHistogram(histogram);
    findPeaks(histogram, &firstPeak, &secondPeak);

    #ifdef DEBUG_ON
    printf("first peak: %u; second peak: %u\n", firstPeak, secondPeak);
    #endif

    peaksBoundsPeakTechnique(histogram, firstPeak, secondPeak, &upperBound, &lowerBound);

    #ifdef DEBUG_ON
    printf("upper bound: %u; lower bound: %u\n", upperBound, lowerBound);
    #endif

    rangeThresholdImage(img, lowerBound, upperBound);
    pmsis_l2_malloc_free(histogram, (MAX_PIXEL_VALUE + 2 -MIN_PIXEL_VALUE) * sizeof(uint16_t));
}

void histogramValleyTechnique(PGMImage* img)
{
    uint16_t firstPeak = MAX_PIXEL_VALUE + 1, secondPeak = MAX_PIXEL_VALUE + 1;
    uint8_t upperBound, lowerBound;

    uint16_t *histogram = pmsis_l2_malloc((MAX_PIXEL_VALUE + 2 -MIN_PIXEL_VALUE) * sizeof(uint16_t));
    memset(histogram, 0, (MAX_PIXEL_VALUE + 2 -MIN_PIXEL_VALUE) * sizeof(uint16_t));

    calculateHistogram(img, histogram);
    smoothHistogram(histogram);
    findPeaks(histogram, &firstPeak, &secondPeak);

    #ifdef DEBUG_ON
    printf("first peak: %u; second peak: %u\n", firstPeak, secondPeak);
    #endif

    peaksBoundsValleyTechnique(histogram, firstPeak, secondPeak, &upperBound, &lowerBound);

    #ifdef DEBUG_ON
    printf("upper bound: %u; lower bound: %u\n", upperBound, lowerBound);
    #endif

    rangeThresholdImage(img, lowerBound, upperBound);
    pmsis_l2_malloc_free(histogram, (MAX_PIXEL_VALUE + 2 -MIN_PIXEL_VALUE) * sizeof(uint16_t));
}


void adaptiveHistogramTechnique(PGMImage* img)
{
    uint16_t firstPeak = MAX_PIXEL_VALUE + 1, secondPeak = MAX_PIXEL_VALUE + 1;
    uint8_t upperBound, lowerBound, object, background;

    /* initializing memory with 0 because some shades may never appear on the image */
    uint16_t *histogram = pmsis_l2_malloc((MAX_PIXEL_VALUE + 2 -MIN_PIXEL_VALUE) * sizeof(uint16_t));
    memset(histogram, 0, (MAX_PIXEL_VALUE + 2 -MIN_PIXEL_VALUE) * sizeof(uint16_t));

    calculateHistogram(img, histogram);
    smoothHistogram(histogram);
    findPeaks(histogram, &firstPeak, &secondPeak);

    #ifdef DEBUG_ON
    printf("first peak: %u; second peak: %u\n", firstPeak, secondPeak);
    #endif

    peaksBoundsPeakTechnique(histogram, firstPeak, secondPeak, &upperBound, &lowerBound);
    #ifdef DEBUG_ON
    printf("1st iteration: upper bound: %u; lower bound: %u\n", upperBound, lowerBound);
    #endif

    thresholdAndFindMeans(img, upperBound, lowerBound, &object, &background);

    #ifdef DEBUG_ON
    printf("object mean: %u; background mean: %u\n", object, background);
    #endif

    peaksBoundsPeakTechnique(histogram, object - MIN_PIXEL_VALUE, background - MIN_PIXEL_VALUE, &upperBound, &lowerBound);

    #ifdef DEBUG_ON
    printf("2nd iteration: upper bound: %u; lower bound: %u\n", upperBound, lowerBound);
    #endif

    rangeThresholdImage(img, lowerBound, upperBound);
    pmsis_l2_malloc_free(histogram, (MAX_PIXEL_VALUE + 2 -MIN_PIXEL_VALUE) * sizeof(uint16_t));
}
