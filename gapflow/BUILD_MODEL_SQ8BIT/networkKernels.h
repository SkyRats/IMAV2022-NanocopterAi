#ifndef __NETWORKKERNEL_H__
#define __NETWORKKERNEL_H__

#include "AutoTilerLibTypes.h"
#include "nntool_extra_kernels.h"
#include "CNN_BasicKernels_SQ8.h"
#include "network.h"
#define _network_L1_Memory_SIZE 46400
#define _network_L2_Memory_SIZE 0
extern char *network_L1_Memory; /* Size given for generation: 46736 bytes, used: 46400 bytes */
extern char *network_L2_Memory; /* Size used for generation: 0 bytes */
extern void S1_Op_input_1_formatter(
		unsigned char * __restrict__ In,
		signed char * __restrict__ Out);
extern void S4_Conv2d_32x1x5x5_MaxPool_2x2(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos);
extern void S7_Conv2d_32x32x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos);
extern void S10_Conv2d_32x32x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos);
extern void S13_Conv2d_32x32x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos);
extern void S14_MatAdd_32x25x25(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos);
extern void S17_Conv2d_64x32x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos);
extern void S20_Conv2d_64x64x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos);
extern void S23_Conv2d_64x32x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos);
extern void S24_MatAdd_64x13x13(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos);
extern void S27_Conv2d_128x64x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos);
extern void S30_Conv2d_128x128x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos);
extern void S33_Conv2d_128x64x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos);
extern void S34_MatAdd_128x7x7_Relu6(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos);
extern void S38_Linear_2x6272(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos);
extern void S40_Op_Gemm_25_split_copy_0(
		signed char * __restrict__ In,
		signed char * __restrict__ Out);
extern void S42_Act_Hsigmoid(
		signed char * __restrict__ In,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos);
extern int networkCNN_Construct();
extern int networkCNN_Destruct();
extern int networkCNN(
		unsigned char * __restrict__ S0_Input_1,
		signed char * __restrict__ Output_1,
		signed char * __restrict__ Output_2);
extern unsigned int AT_GraphPerf[17];
extern char * AT_GraphNodeNames[17];
extern unsigned int AT_GraphOperInfosNames[17];
#endif
