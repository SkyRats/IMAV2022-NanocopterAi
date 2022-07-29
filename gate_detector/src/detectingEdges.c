#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "imageIO.h"
#include "sobelEdgeDetector.h"
#include "threshold.h"

const uint8_t sobel_mask_0[9] =
   { -1, 0, -1,
    0, 4, 0,
   -1, 0, -1 };

int main(int argc, char** argv)
{

    if(argc < 4)
    {
        printf("\tUsage: detectingEdges [input file path] [output file path] [threshold value]\n");
        exit(-1);
    }

    char** placeholder = NULL;

    PGMImage *image;
    image = readPGM(argv[1]);
    thresholdImage(image, strtol(argv[3], placeholder, 10));
    PGMImage *sobelTestImg =  sobelEdgeContrast(image, sobel_mask_0);
    if(sobelTestImg == NULL || sobelTestImg->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }

    writePGM(argv[2],sobelTestImg);
    free(image);
    free(sobelTestImg);
    printf("Done.\n");

    return 0;
}
