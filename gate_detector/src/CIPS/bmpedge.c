



#include "cips.h"

int fix_edges();

short quick_mask[3][3] =  {
       {-1,  0, -1},
       { 0,  4,  0},
       {-1,  0, -1} };

int quick_edge(the_image, out_image,
           threshold, high, rows, cols, bits_per_pixel)
   int    high, threshold;
   long   rows, cols, bits_per_pixel;
   short  **the_image, **out_image;

{
   short  a, b, i, j, k,
          length, max, new_hi, new_low,
          sum, width;


   new_hi  = 250;
   new_low = 16;
   if(bits_per_pixel == 4){
       new_hi  = 10;
       new_low = 3;
   }

   max = 255;
   if(bits_per_pixel == 4)
      max = 16;

         /* Do convolution over image array */
   printf("\n");
   for(i=1; i<rows-1; i++){
      if( (i%10) == 0) printf("%d ", i);
      for(j=1; j<cols-1; j++){
         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum +
                     the_image[i+a][j+b] *
                     quick_mask[a+1][b+1];
            }
         }
         if(sum < 0)   sum = 0;
         if(sum > max) sum = max;
         out_image[i][j]   = sum;

      }  /* ends loop over j */
   }  /* ends loop over i */

     /* if desired, threshold the output image */
   if(threshold == 1){
       for(i=0; i<rows; i++){
          for(j=0; j<cols; j++){
             if(out_image[i][j] > high){
                  out_image[i][j] = new_hi;
             }
             else{
                  out_image[i][j] = new_low;
             }
          }
       }
   }  /* ends if threshold == 1 */

   fix_edges(out_image, 1,
             rows-1, cols-1);
return(1);
}  /* ends quick_edge */

int main(argc, argv)
   int argc;
   char *argv[];
{
   char  image_name[MAX_NAME_LENGTH];
   char  image_name2[MAX_NAME_LENGTH];
   char  response[MAX_NAME_LENGTH];
   int   i, j;
   short **the_image, **out_image;
   struct   tiff_header_struct image_header;


   struct bmpfileheader file_header;
   struct bitmapheader  bmheader;
   struct ctstruct      rgb[256];

   long   height, width;


      /******************************************
      *
      *   Ensure the command line is correct.
      *
      ******************************************/

   if(argc < 3){
    printf("\n   usage: bmedge input-image output-image\n");
    exit(0);
   }
   else
      printf("\n%s %s %s", argv[0], argv[1], argv[2]);

   strcpy(image_name, argv[1]);
   strcpy(image_name2, argv[2]);

   read_bmp_file_header(image_name, &file_header);
   read_bm_header(image_name, &bmheader);

   width = bmheader.width;
   if(bmheader.height < 0)
      height   = bmheader.height * (-1);
   else
      height = bmheader.height;

   the_image = allocate_image_array(height, width);
   out_image = allocate_image_array(height, width);
   read_bmp_image(image_name, the_image);


   create_bmp_file_if_needed(image_name, image_name2, the_image);

   read_bmp_image(image_name, the_image);
printf("\nMAIN> read %s", image_name);

   quick_edge(the_image,
              out_image,
              1,         /* threshold */
              90,        /* high      */
              bmheader.height,
              bmheader.width,
              bmheader.bitsperpixel);


printf("\nMAIN> Back from the edge detector");


   write_bmp_image(image_name2, out_image);
printf("\nMAIN> wrote %s", image_name2);

   free_image_array(the_image,
      image_header.image_length);
   free_image_array(out_image,
      image_header.image_length);

}  /* ends main */


























   /*********************************************
   *
   *   create_allocate_bmp_file(...
   *
   *   The calling routine must set the
   *   height and width.  This routine will set
   *   everything else.
   *
   **********************************************/










