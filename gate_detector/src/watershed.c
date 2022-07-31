#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "imageIO.h"
#include "PQueue.h"
#include "queue.h"
#include "vector.h"

/*
1- Recebe imagem e um vetor de pontos (marcadores) -Check
2- Estabelece cores aleatorias para cada marcador, usando um vetor de cores
3- adiciona os marcadores em uma lista de prioridades
4- Agora, precisamos criar um mapa de marcadores:
5- Enquanto a lista de prioridades ter elementos, pegar o primeiro elem da fila
6- Somar o valor de X e de Y com os 4 primeiros elementos de um vetor de pesos (dx e dy)
7- Se o valor estiver dentro dos limites da imagem, usar esses novos valores para criar o proximo ponto a ser checado
8- Verificar a label dos vizinhos desse ponto no mapa de marcadores
7- Se a label dos vizinhos for igual ao do ponto next, adicionar ele a fila, e adicionar a label dos vizinhos ao ponto top no mapa de marcadores.
9- Transformar o mapa de marcadores em uma imagem, retornar a imagem.
*/

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

void distanceTransform(PGMImage *img, Vector * markers)
{
    Queue* pixelIndexQueue = createQueue(((img->x)*(img->y)) >> 4);
    int8_t dx[8] = {-1, 1, 0, 0, -1, -1, 1, 1},
           dy[8] = {0, 0, -1, 1, -1, 1, 1, -1};

    bool* visited = calloc(img->x * img->y, sizeof(bool));
    if( visited == NULL)
    {
        #ifdef DEBUG_ON
        printf("Allocation of \"visited\" array failed (distanceTransform). Exiting.");
        #endif
        return;
    }

    for(uint8_t y = 1 ; y < img->y -1; ++y)
        for( uint8_t x = 1 ; x < img->x -1; ++x)
        {
            uint16_t pixelIndex = y * img->x + x;

            if( img->data[pixelIndex].gray != 0)
                continue;

            for( uint8_t h = 0 ; h < 4 ; ++h)
            {
                uint16_t nextPixelIndex = pixelIndex + dy[h]*img->x + dx[h];

                if(!visited[nextPixelIndex] && img->data[nextPixelIndex].gray == 254)
                {
                    visited[nextPixelIndex] = true;
                    img->data[nextPixelIndex].gray = 50;
                    enqueue(pixelIndexQueue, nextPixelIndex);
                }
            }

        }

    free(visited);

    while(!queueIsEmpty(pixelIndexQueue))
    {
        uint16_t currentPixelIndex = dequeue(pixelIndexQueue);
        uint16_t nextPixelIndex;
        uint8_t currentPixel = img->data[currentPixelIndex].gray;
        uint8_t nextPixel;
        bool isBigger = true;

        for(uint8_t h = 0; h < 8 ; ++h)
        {
            nextPixelIndex = currentPixelIndex + dy[h]*img->x + dx[h];
            nextPixel = img->data[nextPixelIndex].gray;

            if(nextPixel == 0)
                continue;

            if(currentPixel <= nextPixel)
            {
                isBigger = false;
                if(currentPixel + 1 < nextPixel)
                {
                    img->data[nextPixelIndex].gray = currentPixel + 1 < 254 ? currentPixel + 1 : 254;
                    enqueue(pixelIndexQueue, nextPixelIndex);
                }
            }
        }

        if(isBigger)
            vector_add(markers, currentPixelIndex);
    }
}

PGMImage* watershed(PGMImage *image, Vector *Markers){

    PQueue *pq = NULL;
    PGMImage *markerImage = malloc(sizeof(PGMImage));
    markerImage->x = image->x;
    markerImage->y = image->y;
    markerImage->data = malloc(sizeof(PGMPixel)*(markerImage->x)*(markerImage->y));

    Vector *colors = malloc(sizeof(Vector));
    vector_init(colors);

    int black = 0;
    vector_add(colors, black);
    for(int i = 1; i <= (Markers->total) + 5; i++){
        vector_add(colors, rand()%255);
    }

    PGMImage *markerMap = calloc((image->y)*(image->x), sizeof(PGMImage));
    markerMap->data = malloc(sizeof(PGMPixel)*(image->x)*(image->y));
    PGMImage *inpq = calloc((image->y)*(image->x), sizeof(PGMImage));
    inpq->data = malloc(sizeof(PGMPixel)*(image->x)*(image->y));

    int8_t dx[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int8_t dy[8] = {0, 0, -1, 1, -1, 1, 1, -1};


    //Put markers in priority Queue
    int id = 2;
    for(int i = 0; i <= Markers -> total; i++){
       int marker = vector_get(Markers, i);
        set_PGM(markerMap, marker, id);
        for(int j = 0; j< 4; j++){
            int newIndex = marker + dx[j] + dy[j]*(image->x);
            if (newIndex < 0 || newIndex >= (image->x)*(image->y)){
                continue;
            }

            if (pq == NULL){
                pq = newPQueue(newIndex, get_PGM(image, newIndex).gray);
            }
            else{
                push(&pq, newIndex, 255 - get_PGM(image, newIndex).gray);
            }
        }
        id++;
    }

    while(!isEmpty(&pq)){
        int top = peek(&pq);pop(&pq);

        bool canLabel = true;
        int neighboursLabel = 0;
        for(int i = 0; i < 4; i++) {
                int nextIndex = top + dx[i] + dy[i]*(image->x);
                if(nextIndex < 0 || nextIndex >= (image->x)*(image->y)) {
                    continue;
                }
                PGMPixel nextPixel = get_PGM(image, nextIndex);
                 // Must check if all surrounding marked have the same color
                if (get_PGM(markerMap, nextIndex).gray !=0 && nextPixel.gray != 0){
                    if (neighboursLabel == 0){
                        neighboursLabel = get_PGM(markerMap, nextIndex).gray;
                    }
                    else if (get_PGM(markerMap, nextIndex).gray != neighboursLabel){
                        canLabel = false;
                    }
                    else if (get_PGM(inpq, nextIndex).gray == 0){// Remember, 0=False, 1=True
                        set_PGM(inpq, nextIndex, 1);
                        push(&pq, nextIndex, 255 - nextPixel.gray);
                    }
                }
    }
        if(canLabel){
            set_PGM(markerMap, top, neighboursLabel);
        }
   }
    // Transform markerMap into an image
//    for (int i = 0; i < (image->x)*(image->y); i++){
        //int marker = get_PGM(markerMap, i).gray;
        //int newColor = vector_get(colors, marker);
        //set_PGM(markerImage, i, newColor);
    //}

    return markerMap;
}
