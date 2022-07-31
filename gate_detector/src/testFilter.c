#include <stdio.h>
#include <stdlib.h>
#include "filter.h"

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        printf("\tUsage: testLowPixelFilter [input file path] [output file path]\n");
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

    writePGM(argv[2],filteredImg);
    free(image);
    free(filteredImg);
    printf("Done.\n");

    return 0;
}
