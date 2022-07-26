#include <stdio.h>
#include <stdlib.h>
#include "../include/ppmReader.h"

int main(){
    PPMGRAYImage *image;
    image = readGrayscalePPM("../images/can_bottom.ppm");
    changeColorPPM(image);
    writeGrayscalePPM("../images/can_bottom2.ppm",image);
    printf("Press any key...");
    getchar();

    return 0;
}
