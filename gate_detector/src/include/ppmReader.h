#define CREATOR "RPFELGUEIRAS"
#define MAXIMUM_VALUE 255

typedef struct {
     unsigned char red,green,blue;
} PPMRGBPixel;

typedef struct {
     unsigned char gray;
} PPMGRAYPixel;

typedef struct {
     int x, y;
     PPMRGBPixel *data;
} PPMRGBImage;

typedef struct {
     int x, y;
     PPMGRAYPixel *data;
} PPMGRAYImage;

void writeGrayscalePPM(const char *filename, PPMGRAYImage *img);
PPMGRAYImage *readGrayscalePPM(const char *filename);
void changeColorPPM(PPMGRAYImage *img);
