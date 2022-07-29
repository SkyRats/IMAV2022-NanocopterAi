#include <stdio.h>
#include <stdlib.h>
#include "imageIO.h"
#include "convolution.h"

const uint8_t sobel_mask_0[9] =
   { 1, 0, -1,
    2, 0, -2,
   1, 0, -1 };

int main(int argc, char** argv)
{

    if(argc < 3)
    {
        printf("\tUsage: testSobel [input file path] [output file path]\n");
        exit(-1);
    }

    PGMImage *image;
    image = readPGM(argv[1]);
    PGMImage *sobelTestImg =  convolution3by3(image, sobel_mask_0);
    free(image);
    if(sobelTestImg == NULL || sobelTestImg->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }

    writePGM(argv[2],sobelTestImg);
    free(sobelTestImg);
    printf("Done.\n");

    return 0;
}
