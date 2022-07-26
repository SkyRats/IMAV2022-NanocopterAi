#include <stdio.h>
#include <stdlib.h>
#include "imageIO.h"

int main(){
    PGMImage *image;
    image = readPGM("../images/can_bottom.ppm");
    changeContrastPGM(image);
    writePGM("../images/can_bottom2.ppm",image);
    printf("Press any key...");
    getchar();

    return 0;
}
