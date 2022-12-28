#include <stdio.h>
#include <stdlib.h>

#include "imageIO.h"


PGMImage *readPGM(const char *filename)
{
         char buff[16];
         PGMImage *img;
         FILE *fp;
         int c, rgb_comp_color;
         //open PPM file for reading
         fp = fopen(filename, "rb");
         if (!fp) {
              fprintf(stderr, "Unable to open file '%s'\n", filename);
              exit(1);
         }

         //read image format
         if (!fgets(buff, sizeof(buff), fp)) {
              perror(filename);
              exit(1);
         }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '5') {
         fprintf(stderr, "Invalid image format (must be 'P6')\n");
         exit(1);
    }

    //alloc memory form image
    img = (PGMImage *)malloc(sizeof(PGMImage));
    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
    while (getc(fp) != '\n')
        ;
         c = getc(fp);
    }

    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
         fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
         exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
         fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
         exit(1);
    }

    //check rgb component depth
    if (rgb_comp_color!= MAX_PIXEL_VALUE) {
         fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
         exit(1);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->data = (PGMPixel*)malloc(img->x * img->y * sizeof(PGMPixel));

    if (!img) {
         fprintf(stderr, "Unable to allocate memory\n");
         exit(1);
    }

    //read pixel data from file
    if (fread(img->data, img->x, img->y, fp) != img->y) {
         fprintf(stderr, "Error loading image '%s'\n", filename);
         exit(1);
    }

    fclose(fp);
    return img;
}
void writePGM(const char *filename, PGMImage *img)
{
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", filename);
         exit(1);
    }

    //write the header file
    //image format
    fprintf(fp, "P5\n");

    //comments
    fprintf(fp, "# Created by %s\n",CREATOR);

    //image size
    fprintf(fp, "%d %d\n",img->x,img->y);

    // rgb component depth
    fprintf(fp, "%d\n",MAX_PIXEL_VALUE);

    // pixel data
    fwrite(img->data, img->x, img->y, fp);
    fclose(fp);
}

void changeContrastPGM(PGMImage *img)
{
    int i;
    if(img){

         for(i=0;i<img->x*img->y;i++)
              img->data[i].gray = MAX_PIXEL_VALUE - img->data[i].gray;
    }
}


PGMPixel get_PGM(PGMImage *img, int index){
     if (index >= 0 && index < (img->x)*(img->y)){
          return img->data[index];
     }
     return img->data[0];

}

void set_PGM(PGMImage *img, int index, int newData){
     if (index >= 0 && index < (img->x)*(img->y)){
          img->data[index].gray = newData;
     }

}
