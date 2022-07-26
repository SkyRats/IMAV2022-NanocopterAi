#ifndef __PPM_READER_H__
#define __PPM_READER_H__
#define CREATOR "SKYRATS"
#define MAXIMUM_VALUE 255

typedef struct {
     unsigned char red,green,blue;
} PPMPixel;

typedef struct {
     unsigned char gray;
} PGMPixel;

typedef struct {
     int x, y;
     PPMPixel *data;
} PPMImage;

typedef struct {
     int x, y;
     PGMPixel *data;
} PGMImage;

void writePGM(const char *filename, PGMImage *img);
PGMImage *readPGM(const char *filename);
void changeContrastPGM(PGMImage *img);
#endif
