#include <stdio.h>
#include <stdlib.h>
#include "imageIO.h"
#include "histogram.h"
#include "filter.h"

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        printf("\tUsage: testHistogramTechniques [input file path] [output file path]\n");
        exit(-1);
    }

    PGMImage *image;
    image = readPGM(argv[1]);
    if(image == NULL)
    {
        printf("Allocation of memory failed.");
        exit(-1);
    }

    /* applying filter */
    PGMImage *filteredImg =  lowPixelFilter(image);
    free(image); /* freeing space allocated to original image */

    //histogramPeakTechnique(filteredImg);
    //histogramValleyTechnique(filteredImg);
    adaptativeHistogramTechnique(filteredImg);
    writePGM(argv[2],filteredImg);
    //free(image);
    free(filteredImg);
    printf("Done.\n");

    return 0;

}
