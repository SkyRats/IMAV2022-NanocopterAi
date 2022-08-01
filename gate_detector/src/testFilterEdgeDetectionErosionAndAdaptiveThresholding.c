#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "imageIO.h"
#include "convolution.h"
#include "erosion.h"
#include "filter.h"
#include "histogram.h"
#include "regionGrowing.h"

int main(int argc, char** argv)
{
    int8_t const quickMask[9] =
       { -1, 0, -1,
        0, 4, 0,
       -1, 0, -1 };

    bool const erosionMask[8] =
       {true, true, true,
        true,      true,
       true, true, true };

    if(argc < 3)
    {
        printf("\tUsage: testErosion [input file path] [output file path]\n");
        exit(-1);
    }

    PGMImage *image;
    image = readPGM(argv[1]);
    if(image == NULL)
    {
        printf("File does not exist.\n");
        exit(-1);
    }

    PGMImage *filteredImg = lowPixelFilter(image);
    free(image);
    if(filteredImg == NULL || filteredImg->data == NULL)
    {
        printf("Allocation of memory failed. Exiting.\n");
        exit(-1);
    }

    PGMImage *convolutedImg =  convolution3by3(filteredImg, quickMask);
    free(filteredImg);
    if(convolutedImg == NULL || convolutedImg->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }

    PGMImage * erodedImg = maskErosion(convolutedImg, erosionMask);
    free(convolutedImg);
    if(erodedImg == NULL || erodedImg->data == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }

    adaptiveHistogramTechnique(erodedImg);

    printf("found %u regions.\n", edgeSegmentation(erodedImg));

    writePGM(argv[2], erodedImg);
    free(erodedImg);
    printf("Done.\n");

    return 0;
}
