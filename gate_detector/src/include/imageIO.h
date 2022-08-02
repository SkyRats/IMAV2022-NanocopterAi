#ifndef __PPM_READER_H__
#define __PPM_READER_H__
#include <stdint.h>

#define CREATOR "POLI_USP SKYRATS"
#define MAXIMUM_VALUE 255

typedef struct {
     unsigned char red,green,blue;
} PPMPixel;

typedef struct {
     unsigned char gray;
} PGMPixel;

struct PGMPoint
{
    uint8_t x;
    uint8_t y;
    uint8_t grayShade;
}__attribute__((packed));

typedef struct PGMPoint Point;

typedef struct {
     int x, y;
     PPMPixel *data;
} PPMImage;

typedef struct {
     int x, y; //width, height
     PGMPixel *data;
} PGMImage;

void writePGM(const char *filename, PGMImage *img);
PGMImage *readPGM(const char *filename);
void changeContrastPGM(PGMImage *img);
PGMPixel get_PGM(PGMImage *img, int index);
void set_PGM(PGMImage *img, int index, int newData);
#endif
