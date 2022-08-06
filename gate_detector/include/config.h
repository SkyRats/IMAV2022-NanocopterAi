#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CREATOR "POLI-USP SKYRATS"
#define VECTOR_INIT_CAPACITY 4
#define STACK_INITIAL_SIZE 100
#define PEAK_SPACE 50
#define MAX_PIXEL_VALUE 255
#define MIN_PIXEL_VALUE 0
/* for a region to be considered valid */
#define MIN_PIXEL_AMOUNT 2500
/* tolerance for gate finding, in pixels */
#define TOL 10

/**
  * 0: Histogram Peak Technique
  * 1: Histogram Valley Technique
  * else: Adaptive Technique
  */

#define THRESHOLDING_SEGMENTATION_METHOD 2
#define DEBUG_ON

/* if you wish to use the values defined
 * at each header
 */
//#undef CREATOR
//#undef VECTOR_INIT_CAPACITY
//#undef STACK_INITIAL_SIZE
//#undef PEAK_SPACE
//#undef MAX_PIXEL_VALUE
//#undef MIN_PIXEL_VALUE

#endif
