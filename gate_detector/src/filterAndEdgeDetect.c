#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "imageIO.h"
#include "filter.h"
#include "convolution.h"
#include "threshold.h"

const uint8_t sobel_mask_0[9] =
   { -1, 0, -1,
    0, 4, 0,
   -1, 0, -1 };

int main(int argc, char** argv)
{

    if(argc < 4)
    {
        printf("\tUsage: filterAndEdgeDetect [input file path] [output file path] [threshold value]\n");
        exit(-1);
    }

    PGMImage *image;
    image = readPGM(argv[1]);
    PGMImage *filteredImg =  lowPixelFilter(image);
    if(filteredImg == NULL || filteredImg->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }
    char ** placeholder = NULL;
    free(image);
    thresholdImage(filteredImg, strtol(argv[3], placeholder, 10));
    PGMImage *sobelTestImg =  convolution3by3(filteredImg, sobel_mask_0);
    if(sobelTestImg == NULL || sobelTestImg->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }
    free(filteredImg);
    writePGM(argv[2],sobelTestImg);
    free(sobelTestImg);
    printf("Done.\n");
}
