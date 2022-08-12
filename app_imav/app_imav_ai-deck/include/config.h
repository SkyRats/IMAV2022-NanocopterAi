#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CREATOR "POLI-USP SKYRATS"
#define VECTOR_INIT_CAPACITY 4
#define STACK_INITIAL_SIZE 100
#define PEAK_SPACE 50
#define MAX_PIXEL_VALUE 255
#define MIN_PIXEL_VALUE 0


/* tolerance of square side max size difference for gate finding, in pixels */
#define TOL 10

/**
  * 0: Histogram Peak Technique
  * 1: Histogram Valley Technique
  * else: Adaptive Technique
  */
#define THRESHOLDING_SEGMENTATION_METHOD 2

/**
  * 0: Only input image edges will be taken into account
  * else: Edges and gray shades of the input image will be considered
  */
#define SEGMENTATION_METHOD 1

/* for a region to be considered valid (potential square) */
#define MIN_PIXEL_AMOUNT_EDGES_ONLY 3000
#define MIN_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES 2000
#define MAX_PIXEL_AMOUNT_EDGES_AND_GRAY_SHADES 40000
#define MAX_GRAYSHADE_DIFF 10

#define DEBUG_ON

/*
 * if you wish to use the values defined
 * at each header
 */
//#undef CREATOR
//#undef VECTOR_INIT_CAPACITY
//#undef STACK_INITIAL_SIZE
//#undef PEAK_SPACE
//#undef MAX_PIXEL_VALUE
//#undef MIN_PIXEL_VALUE

#endif
