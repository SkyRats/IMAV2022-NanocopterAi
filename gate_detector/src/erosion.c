#include <stdio.h>
#include <stdlib.h>
#include <erosion.h>
#include <stdbool.h>
#include "imageIO.h"
#include "queue.h"

 
void erosion(PGMImage *img, Vector *kernel){
        int n = 3;    //check kernel size
        int m = 3;

        bool* shouldBeZeroImage = calloc(img->x * img->y, sizeof(bool));
       
        for(int i = n / 2; i < img->y - n / 2; i++) {
            for(int j = m / 2; j < img->x - m / 2; j++) {

                // Loop the kernel
                if( img->data[i*(img->x)+j].gray == 254) {
                    bool shouldBeZero = false;
                    for(int crtX = i - n / 2, x = 0; crtX <= i + n / 2; crtX++, x++) {
                        for(int crtY = j - m / 2, y = 0; crtY <= j + m / 2; crtY++, y++) {
                            if(img->data[crtX*(img->x)+crtY].gray == 0 && kernel->items[x*(img->x)+y] == 1) {
                                shouldBeZero = true;
                                break;
                            }
                        }
                    }

                    if(shouldBeZero) {
                        shouldBeZeroImage[i*(img->x)+j] = true;
                    }
                }
            }
        }
        for(int i = 0; i < img->y; i++) {
            for(int j = 0; j < img->x; j++) {
                if(shouldBeZeroImage[i*(img->x)+j]) {
                    img->data[i*(img->x)+j].gray = 0;
                }
            }
        }

}