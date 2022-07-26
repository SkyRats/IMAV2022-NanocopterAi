#include <stdio.h>
#include <stdlib.h>
#include "ppmReader.h"
#include "threshold.h"

int main(int argc, char** argv)
{

    if(argc < 4)
    {
        printf("\tUsage: testThreshold [input file path] [output file path] [threshold value]\n");
        exit(-1);
    }

    char** placeholder = NULL;

    PPMGRAYImage *image;
    image = readGrayscalePPM(argv[1]);
    thresholdImage(image, strtol(argv[3], placeholder, 10));
    writeGrayscalePPM(argv[2],image);
    printf("Done.\n");

    return 0;
}
