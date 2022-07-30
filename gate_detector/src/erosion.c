#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <erosion.h>
#include <stdbool.h>
#include "imageIO.h"
#include "queue.h"


void erosion(PGMImage *img, Vector *kernel){
        uint8_t n = 3;    //check kernel size
        uint8_t m = 3;

        bool* shouldBeZeroImage = calloc(img->x * img->y, sizeof(bool));

        for(uint8_t i = n >> 1; i < img->y - (n >> 1); i++) {
            for(uint8_t j = m >> 1; j < img->x - (m >> 1); j++) {

                // Loop the kernel
                if( img->data[i*(img->x)+j].gray == 254) {
                    bool shouldBeZero = false;
                    for(uint8_t crtX = i - (n >> 1), x = 0; crtX <= i + (n >> 1); crtX++, x++) {
                        for(uint8_t crtY = j - m / 2, y = 0; crtY <= j + (m >> 1); crtY++, y++) {
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
        for(uint8_t i = 0; i < img->y; i++) {
            for(uint8_t j = 0; j < img->x; j++) {
                if(shouldBeZeroImage[i*(img->x)+j]) {
                    img->data[i*(img->x)+j].gray = 0;
                }
            }
        }

}
