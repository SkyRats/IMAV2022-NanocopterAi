#include <stdio.h>
#include <stdlib.h>
#include "imageIO.h"
#include "sobelEdgeDetector.h"

const uint8_t sobel_mask_5[9] =
   {-2, -1, 0,
   -1, 0, 1,
    0, 1, 2 };

int main(int argc, char** argv)
{

    if(argc < 3)
    {
        printf("\tUsage: testSobel [input file path] [output file path]\n");
        exit(-1);
    }

    PGMImage *image;
    image = readPGM(argv[1]);
    PGMImage *sobelTestImg =  sobelEdgeContrast(image, sobel_mask_5);
    if(sobelTestImg == NULL || sobelTestImg->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }

    writePGM(argv[2],sobelTestImg);
    printf("Done.\n");

    return 0;
}
