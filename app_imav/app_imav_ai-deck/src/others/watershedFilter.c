#include "pmsis.h"
#include <stdio.h>
#include <stdlib.h>
#include "watershed.h"
#include "threshold.h"
#include "vector.h"


int main(int argc, char** argv)
{
    Vector * erosionTargetThree = malloc(sizeof(Vector));
    vector_init(erosionTargetThree);
    for(int i = 0; i < 9; ++i)
        vector_add(erosionTargetThree, 1);

    if(argc < 3)
    {
        printf("\tUsage: watershedFilter [input file path] [output file path] \n");
        exit(-1);
    }


    PGMImage *image;
    image = readPGM(argv[1]);

    thresholdImage(image, 100);
    erosion(image, erosionTargetThree);
    Vector* markers = malloc(sizeof(Vector));
    vector_init(markers);
//
    distanceTransform(image, markers);
    PGMImage* outputImage = watershed(image, markers);
    writePGM(argv[2],outputImage);

    printf("Done.");
    return 0;
}
