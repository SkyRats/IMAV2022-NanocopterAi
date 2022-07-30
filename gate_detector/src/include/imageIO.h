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
     int x, y; //largura, altura
     PGMPixel *data;
} PGMImage;

void writePGM(const char *filename, PGMImage *img);
PGMImage *readPGM(const char *filename);
void changeContrastPGM(PGMImage *img);
PGMPixel get_PGM(PGMImage *img, int index);
void set_PGM(PGMImage *img, int index, int newData);
#endif
