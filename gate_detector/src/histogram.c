#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "histogram.h"
#include "threshold.h"

void findValley(uint16_t restrict* histogram, uint8_t firstPeak, uint8_t secondPeak, uint8_t restrict* valley)
{
    uint8_t i,
            max = firstPeak < secondPeak ? secondPeak : firstPeak,
            valley;

    for(i = firstPeak < secondPeak ? firstPeak + 1 : secondPeak + 1; i < max; ++i)
        *valley = histogram[*valley] > histogram[i] ? i : *valley;
}

void peaksBoundsValleyTechnique(uint16_t restrict* histogram, uint16_t firstPeak, uint16_t secondPeak, uint8_t restrict* upperBound,uint8_t restrict* lowerBound)
{
    uint8_t i, valley;
    uint32_t sum1 = 0, sum2 = 0;

    findValley(histogram, firstPeak, secondPeak, &valley);

    for(i = 0; i < valley; ++i)
        sum1 += histogram[i];

    for(i=midPoint; i<255; ++i)
        sum2 += histogram[i];
    sum2 += histogram[255];

    if(sum1 >= sum2)
    {
        *lowerBound = histogram[valley];
        *upperBound = 255;
    }
    else
    {
        *lowerBound = 0;
        *upperBound = histogram[valley];
    }
}

void peaksBoundsPeakTechnique(uint16_t restrict* histogram, uint8_t firstPeak, uint8_t secondPeak, uint8_t restrict* upperBound,uint8_t restrict* lowerBound)
{
    uint8_t i, midPoint = (firstPeak - secondPeak > 0) ? ((firstPeak - secondPeak)>>1) + secondPeak : ((secondPeak - firstPeak)>>1) + firstPeak;
    uint32_t sum1 = 0, sum2 = 0;

    for(i = 0; i < midPoint; ++i)
        sum1 += histogram[i];

    for(i=midPoint; i<255; ++i)
        sum2 += histogram[i];
    sum2 += histogram[255];

    if(sum1 >= sum2)
    {
        *lowerBound = histogram[midPoint];
        *upperBound = 255;
    }
    else
    {
        *lowerBound = 0;
        *upperBound = histogram[midPoint];
    }
}

void findPeaks(uint16_t restrict* histogram, uint8_t restrict *firstPeak, uint8_t restrict *secondPeak)
{
    uint8_t i;
    int16_t diff;

    for(i = 0; i < 255; ++i)
        if(histogram[i] > histogram[*firstPeak])
            *firstPeak = i;
        else
        {
            diff = *firstPeak - *secondPeak;
            diff = diff > 0 ? diff : -diff;
            if(histogram[i] > histogram[*secondPeak] && diff > PEAK_SPACE)
                *secondPeak = i;
        }

    if(histogram[255] > histogram[*firstPeak])
        *firstPeak = 255;
    else
    {
        diff = *firstPeak - *secondPeak;
        diff = diff > 0 ? diff : -diff;
        if(histogram[255] > histogram[*secondPeak] && diff > PEAK_SPACE)
            *secondPeak = 255;
    }
}

void calculateHistogram(PGMImage const* img, uint16_t* histogram)
{
    uint16_t i, size = img->x * img->y;
    uint8_t index;

    for(i = 0; i < size; i++)
    {
        index = img->data[i].gray;
        histogram[index] += 1;
    }
}

void smoothHistogram(uint16_t restrict* histogram)
{
    uint8_t i = 1;
    uint16_t *newHistogram = calloc(256, sizeof(uint16_t));

    newHistogram[0] = (histogram[0] + histogram[1]) >> 1;/*divide by 2*/
    newHistogram[255] = (histogram[255] + histogram[254]) >> 1;

    do
    {
        newHistogram[i] = (histogram[i-1] + histogram[i] + histogram[i+1])/3;
        ++i;
    }while(i != 255);

    for(i = 0; i < 255; ++i)
        histogram[i] = newHistogram[i];

    histogram[255] = newHistogram[255];

    free(newHistogram);
}


void histogramPeakTechnique(PGMImage* img)
{
    uint8_t firstPeak = 1, secondPeak = 0;
    uint8_t upperBound, lowerBound;

    uint16_t *histogram = calloc(256, sizeof(uint16_t));

    calculateHistogram(img, histogram);
    smoothHistogram(histogram);
    findPeaks(histogram, &firstPeak, &secondPeak);
    peaksBoundsPeakTechnique(histogram, firstPeak, secondPeak, &upperBound, &lowerBound);
    rangeThresholdImage(img, lowerBound, upperBound);
    free(histogram);
}

void histogramValleyTechnique(PGMImage* img)
{
    uint8_t firstPeak = 1, secondPeak = 0;
    uint8_t upperBound, lowerBound;

    uint16_t *histogram = calloc(256, sizeof(uint16_t));

    calculateHistogram(img, histogram);
    smoothHistogram(histogram);
    findPeaks(histogram, &firstPeak, &secondPeak);
    peaksBoundsValleyTechnique(histogram, firstPeak, secondPeak, &upperBound, &lowerBound);
    rangeThresholdImage(img, lowerBound, upperBound);
    free(histogram);
}


void adaptativeHistogramTechnique(PGMImage* img)
{
    uint8_t firstPeak = 1, secondPeak = 0;
    uint8_t upperBound, lowerBound, object, background;

    uint16_t *histogram = calloc(256, sizeof(uint16_t));

    calculateHistogram(img, histogram);
    smoothHistogram(histogram);
    findPeaks(histogram, &firstPeak, &secondPeak);
    peaksBoundsPeakTechnique(histogram, firstPeak, secondPeak, &upperBound, &lowerBound);
    thresholdAndFindMeans(img, upperBound, lowerBound, &object, &background);
    peaksBoundsPeakTechnique(histogram, object, background, &upperBound, &lowerBound);
    rangeThresholdImage(img, lowerBound, upperBound);
    free(histogram);
}
