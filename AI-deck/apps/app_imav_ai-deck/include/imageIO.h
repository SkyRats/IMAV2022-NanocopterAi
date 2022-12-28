#ifndef __PPM_READER_H__
#define __PPM_READER_H__
#include <stdint.h>
#include "config.h"

#ifndef CREATOR
#define CREATOR "POLI_USP SKYRATS"
#endif

#ifndef MAX_PIXEL_VALUE
#define MAX_PIXEL_VALUE 255
#endif

#ifndef MIN_PIXEL_VALUE
#define MIN_PIXEL_VALUE 0
#endif

struct PGMPoint
{
    uint8_t x;
    uint8_t y;
    uint8_t grayShade;
}__attribute__((packed));

typedef struct PGMPoint Point;

struct PGMImage {
     uint8_t x, y; //width, height
     uint8_t *data;
}__attribute__((packed));

typedef struct PGMImage PGMImage;

struct clusterCallArgs{
    PGMImage * inputImage;
    PGMImage * outputImage;
    uint8_t numOfCores;
}__attribute__((packed));

typedef struct clusterCallArgs clusterCallArgs;
#endif
