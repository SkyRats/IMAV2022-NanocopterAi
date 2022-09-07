#include "networkKernels.h"
L1_CL_MEM AT_L1_POINTER network_L1_Memory;
L2_MEM AT_L2_POINTER network_L2_Memory;
AT_HYPERRAM_POINTER network_L3_Memory;
extern AT_HYPERRAM_T HyperRam;
static AT_HYPERFLASH_FS_T HyperFlash;
void S1_Op_input_1_formatter(
		unsigned char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 46400 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	KerNormBW_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast, T0Ind_NextNextLast;
	/* User kernel arguments related variables */
	unsigned int _NN_In;
	unsigned int _SN_In, _SNN_In;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 4]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 2
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 40000 [Tile0, 4:[200x58, 2:200x58, 200x26], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 4:[200x58, 2:200x58, 200x26], 1]
		Tile0: [0, 11600, 11600], Tile1: [11600, 11600, 11600], Tile2; [23200, 11600, 11600]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 40000 [Tile0, 4:[200x58, 2:200x58, 200x26], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 4:[200x58, 2:200x58, 200x26], 1]
		Tile0: [0, 11600, 11600], Tile1: [11600, 11600, 11600], Tile2; [23200, 11600, 11600]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned short int) (200);
	/*================================= Read Tiles Prolog ===============================*/
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+0), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory+28404+0), 11600, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+11600), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory+28404+11600), 11600, 0, &UchanHR1);
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L2_Memory+28404+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+0), 11600, 0, &DmaR_Evt1);
	_NN_In=11600; _SN_In=11600;
	_C_Out=0; _SC_Out=11600;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	for (T0Ind=0; T0Ind<4; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
		int T0Ind_Last = (T0Ind==3), T0Ind_NextLast = ((T0Ind+1)==3), T0Ind_NextNextLast = ((T0Ind+2)==3);
		/*================================= Prepare Tiles ===================================*/
		_SNN_In = 0;
		if (!(T0Ind_Last)) {
			if (!(T0Ind_NextLast)) {
				_NN_In = _NN_In + (11600); _SNN_In = ((T0Ind_NextNextLast)?5200:11600); 
			}
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read In */
		if (_SNN_In) {
			AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) In+_NN_In), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory+28404+11600*((T0Ind_Total)%2)),
					_SNN_In, 0, &UchanHR1);
		}
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
		if (_SN_In) {
			AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L2_Memory+28404+11600*((T0Ind_Total+1)%2)), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+11600*((T0Ind_Total+1)%2)),
					_SN_In, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		/*====================== Call Kernel LOC_LOOP =========================*/
		KerArg0->In = (unsigned char *__restrict__) (network_L1_Memory+0+11600*((T0Ind_Total)%2));
		KerArg0->Out = (signed char *__restrict__) (network_L1_Memory+23200+11600*((T0Ind_Total)%2));
		KerArg0->H = (unsigned short int) (T0Ind_Last?26:58);
		AT_FORK(gap_ncore(), (void *) CNN_NormBW_shift_fps, (void *) KerArg0);
		__CALL(CNN_NormBW_shift_fps, KerArg0);
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+23200+11600*((T0Ind_Total)%2)),
				_SC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SN_In = _SNN_In;
		_SP_Out = _SC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T0Ind_Last)) {
			_C_Out = _C_Out + (11600); _SC_Out = ((T0Ind_NextLast)?5200:11600); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S4_Conv2d_32x1x5x5_MaxPool_2x2(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 33404 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerConvLinReduct_SQ8_T S_KerArg2, *KerArg2 = &S_KerArg2;
	KerPool_SQ8_T S_KerArg3, *KerArg3 = &S_KerArg3;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Total=0, D1Ind_Last, D1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _N_In;
	unsigned int _SN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: Init: 32, Tiled: 1][Tile0 Dim: 50][D0 Dim: Init: 1, Tiled: 1]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 50 logical tiles, 50 physical tiles
			Total Size: 80000 [D1, [0 x 80000, 80000]][Tile0, 50:[50x1, 48:50x1, 50x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 80000, 80000]][Tile0, 50:[50x1, 48:50x1, 50x1], 1]
		Tile0: [0, 1600, 50], Tile1: [50, 1600, 50], Tile2; [100, 1600, 50]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [D1, [0 x 128, 128]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 128, 128]]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [D1, [0 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 32, 32]]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [D1, [0 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 32, 32]]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: Filter, Tiled Space: D1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 50 logical tiles, 1 physical tiles
			Total Size: 800 [D1, [0 x 800, 800]][D0, [0 x 800, 800]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 800, 800]][D0, [0 x 800, 800]]
		Tile0: [0, 800, 25], Tile1: [0, 800, 25], Tile2; [0, 800, 25]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 50 logical tiles, 50 physical tiles
			Total Size: 40000 [D0, [0 x 40000, 40000]][Tile0, 50:[200x5, 48:200x7, 200x6], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 50:[200x5, 48:200x7, 200x6], 1][D0, [0 x 40000, 40000]]
		Tile0: [0, 1000, 1000], Tile1: [400, 1400, 1400], Tile2; [1200, 1400, 1400]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 50 logical tiles, 1 physical tiles
			Total Size: 1280000 [D1, [0 x 1280000, 1280000]][Tile0, 50:[100x2, 48:100x2, 100x2], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 1280000, 1280000]][Tile0, 50:[100x2, 48:100x2, 100x2], 4]
		Tile0: [0, 25600, 800], Tile1: [0, 25600, 800], Tile2; [0, 25600, 800]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 50 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 50:[9x1, 48:9x1, 9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 50:[9x1, 48:9x1, 9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (network_L1_Memory+7792);
	KerArg0->W = (unsigned short int) (100);
	KerArg0->H = (unsigned short int) (2);
	KerArg0->Feat = (unsigned short int) (32);
	KerArg0->Bias = (void * __restrict__) (network_L1_Memory+2800);
	KerArg1->W = (unsigned short int) (200);
	KerArg1->UsedW = (unsigned short int) (200);
	KerArg1->InFeatures = (unsigned short int) (1);
	KerArg1->OutFeatures = (unsigned short int) (32);
	KerArg1->TotalInFeatures = (unsigned short int) (1);
	KerArg1->Out = (int * __restrict__) (network_L1_Memory+7792);
	KerArg2->In = (int *__restrict__) (network_L1_Memory+7792);
	KerArg2->Out = (void *__restrict__) (network_L1_Memory+7792);
	KerArg2->Feat = (unsigned short int) (32);
	KerArg2->W = (unsigned short int) (100);
	KerArg2->H = (unsigned short int) (2);
	KerArg2->Scale = (unsigned char *__restrict__) (network_L1_Memory+2928);
	KerArg2->ScaleN = (unsigned char *__restrict__) (network_L1_Memory+2960);
	KerArg2->Infos = (signed char *__restrict__) (network_L1_Memory+33392);
	KerArg3->In = (signed char * __restrict__) (network_L1_Memory+7792);
	KerArg3->W = (unsigned short int) (100);
	KerArg3->UsedW = (unsigned short int) (100);
	KerArg3->H = (unsigned short int) (2);
	KerArg3->UsedH = (unsigned short int) (2);
	KerArg3->Feat = (unsigned short int) (32);
	KerArg3->Pad = (v4s) 0;
	KerArg3->PoolMax = (unsigned char) (1);
	KerArg3->DoScale = (unsigned char) (1);
	KerArg3->Infos = (signed char * __restrict__) (network_L1_Memory+33392);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=1600; _LC_Out=50;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+2800), 128, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+2928), 32, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+2960), 32, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+2992+0), 800, 0, &DmaR_Evt4);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+0), 1000, 0, &DmaR_Evt5);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+33392), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1, D1Ind_NextLast = 1;
		/*================================= Prepare Tiles ===================================*/
		_SN_Filter = 0;
		
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+2992+800*((D1Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<50; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==49), T0Ind_NextLast = ((T0Ind+1)==49);
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->NormBias = (unsigned char) (((char *)(network_L1_Memory+33392))[5]);
			AT_FORK(gap_ncore(), (void *) KerParSetBiasB32_SQ8, (void *) KerArg0);
			__CALL(KerParSetBiasB32_SQ8, KerArg0);
			{ /* Single iteration on D0 */
				int D0Ind_Last = 1, D0Ind_NextLast = 1;
				/*================================= Prepare Tiles ===================================*/
				_SN_In = 0;
				if (!(T0Ind_Last)) {
					_N_In = _N_In + (800-(400*(T0Ind==0))); _SN_In = ((T0Ind_NextLast)?1200:1400); 
				} else if (!(1)) {
					_N_In = _N_In + (-38800); _SN_In = (1000); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+1400*((D0Ind_Total+1)%2)),
							_SN_In, 0, &DmaR_Evt5);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (network_L1_Memory+0+1400*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (7-2*(T0Ind==0)-1*(T0Ind_Last));
				KerArg1->UsedH = (unsigned short int) (7-2*(T0Ind==0)-1*(T0Ind_Last));
				KerArg1->Filter = (signed char * __restrict__) (network_L1_Memory+2992+800*((D1Ind_Total)%2));
				KerArg1->Pad = (v4s) ((v4s){2,1,2*(T0Ind==0),1*(T0Ind_Last)});
				AT_FORK(gap_ncore(), (void *) KerParConv5x5Stride2_SQ8, (void *) KerArg1);
				__CALL(KerParConv5x5Stride2_SQ8, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				/*============================= End Update Arg Pipeline =============================*/
				D0Ind_Total++;
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			AT_FORK(gap_ncore(), (void *) KerParReductIO_CC_SQ8, (void *) KerArg2);
			__CALL(KerParReductIO_CC_SQ8, KerArg2);
			KerArg3->Out = (signed char * __restrict__) (network_L1_Memory+4592+1600*((T0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParPool2x2Stride2_SQ8, (void *) KerArg3);
			__CALL(KerParPool2x2Stride2_SQ8, KerArg3);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+4592+1600*((T0Ind_Total)%2)),
					_SC_Out, 2500, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (50); _LC_Out = (50); _SC_Out = (32*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		/*============================= End Update Arg Pipeline =============================*/
		D1Ind_Total++;
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S7_Conv2d_32x32x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 45036 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerConvLinReduct_SQ8_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Total=0, D1Ind_Last, D1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: Init: 32, Tiled: 1][Tile0 Dim: 7][D0 Dim: Init: 32, Tiled: 4]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 7 physical tiles
			Total Size: 20000 [D1, [0 x 20000, 20000]][Tile0, 7:[25x4, 5:25x4, 25x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 20000, 20000]][Tile0, 7:[25x4, 5:25x4, 25x1], 1]
		Tile0: [0, 3200, 100], Tile1: [100, 3200, 100], Tile2; [200, 3200, 100]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [D1, [0 x 128, 128]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 128, 128]]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [D1, [0 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 32, 32]]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [D1, [0 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 32, 32]]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: Filter, Tiled Space: D1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 28 logical tiles, 1 physical tiles
			Total Size: 9216 [D1, [0 x 9216, 9216]][D0, [3 x 2304, 2304]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 9216, 9216]][D0, [3 x 2304, 2304]]
		Tile0: [0, 9216, 288], Tile1: [0, 9216, 288], Tile2; [0, 9216, 288]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 28 logical tiles, 28 physical tiles
			Total Size: 80000 [D0, [3 x 20000, 20000]][Tile0, 7:[50x8, 5:50x9, 50x3], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 7:[50x8, 5:50x9, 50x3], 1][D0, [3 x 20000, 20000]]
		Tile0: [0, 3200, 400], Tile1: [20000, 3200, 400], Tile2; [40000, 3200, 400]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 80000 [D1, [0 x 80000, 80000]][Tile0, 7:[25x4, 5:25x4, 25x1], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 80000, 80000]][Tile0, 7:[25x4, 5:25x4, 25x1], 4]
		Tile0: [0, 12800, 400], Tile1: [0, 12800, 400], Tile2; [0, 12800, 400]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 7 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 7:[9x1, 5:9x1, 9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 7:[9x1, 5:9x1, 9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (network_L1_Memory+32224);
	KerArg0->W = (unsigned short int) (25);
	KerArg0->Feat = (unsigned short int) (32);
	KerArg0->Bias = (void * __restrict__) (network_L1_Memory+7200);
	KerArg1->W = (unsigned short int) (50);
	KerArg1->UsedW = (unsigned short int) (50);
	KerArg1->InFeatures = (unsigned short int) (8);
	KerArg1->OutFeatures = (unsigned short int) (32);
	KerArg1->TotalInFeatures = (unsigned short int) (32);
	KerArg1->Out = (int * __restrict__) (network_L1_Memory+32224);
	KerArg2->In = (int *__restrict__) (network_L1_Memory+32224);
	KerArg2->Feat = (unsigned short int) (32);
	KerArg2->W = (unsigned short int) (25);
	KerArg2->Scale = (unsigned char *__restrict__) (network_L1_Memory+7328);
	KerArg2->ScaleN = (unsigned char *__restrict__) (network_L1_Memory+7360);
	KerArg2->Infos = (signed char *__restrict__) (network_L1_Memory+45024);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=3200; _LC_Out=100;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+7200), 128, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+7328), 32, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+7360), 32, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+7392+0), 9216, 0, &DmaR_Evt4);
	_N_Filter=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+0), 3200, 2500, 400, 0, &DmaR_Evt5);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+45024), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1, D1Ind_NextLast = 1;
		/*================================= Prepare Tiles ===================================*/
		_SN_Filter = 0;
		
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+7392+9216*((D1Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<7; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==6), T0Ind_NextLast = ((T0Ind+1)==6);
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->H = (unsigned short int) (T0Ind_Last?1:4);
			KerArg0->NormBias = (unsigned char) (((char *)(network_L1_Memory+45024))[5]);
			AT_FORK(gap_ncore(), (void *) KerParSetBiasB32_SQ8, (void *) KerArg0);
			__CALL(KerParSetBiasB32_SQ8, KerArg0);
			for (D0Ind=0; D0Ind<4; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==3), D0Ind_NextLast = ((D0Ind+1)==3);
				/*================================= Prepare Tiles ===================================*/
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (20000); _LN_In = ((T0Ind_Last)?150:(450-50*(T0Ind==0))); _SN_In = (8*_LN_In); 
				} else if (!(T0Ind_Last)) {
					_N_In = _N_In + (400-(50*(T0Ind==0)))+(-60000); _LN_In = ((T0Ind_NextLast)?150:450); _SN_In = (8*_LN_In); 
				} else if (!(1)) {
					_N_In = _N_In + (-2350)+(-60000); _LN_In = (400); _SN_In = (8*_LN_In); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+3600*((D0Ind_Total+1)%2)),
							_SN_In, 2500, _LN_In, 0, &DmaR_Evt5);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (network_L1_Memory+0+3600*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (((T0Ind_Last)?3:9)-1*(T0Ind==0));
				KerArg1->UsedH = (unsigned short int) (((T0Ind_Last)?3:9)-1*(T0Ind==0));
				KerArg1->Filter = (signed char * __restrict__) (network_L1_Memory+7392+((D0Ind)*72)+9216*((D1Ind_Total)%2));
				KerArg1->Pad = (v4s) ((v4s){1,0,1*(T0Ind==0),0*(T0Ind_Last)});
				AT_FORK(gap_ncore(), (void *) KerParConv3x3Stride2_SQ8, (void *) KerArg1);
				__CALL(KerParConv3x3Stride2_SQ8, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->Out = (void *__restrict__) (network_L1_Memory+25824+3200*((T0Ind_Total)%2));
			KerArg2->H = (unsigned short int) (T0Ind_Last?1:4);
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg2);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg2);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+25824+3200*((T0Ind_Total)%2)),
					_SC_Out, 625, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (100); _LC_Out = ((T0Ind_NextLast)?25:100); _SC_Out = (32*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		/*============================= End Update Arg Pipeline =============================*/
		D1Ind_Total++;
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S10_Conv2d_32x32x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 45436 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerConvLinReduct_SQ8_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Total=0, D1Ind_Last, D1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: Init: 32, Tiled: 1][Tile0 Dim: 5][D0 Dim: Init: 32, Tiled: 4]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 20 logical tiles, 20 physical tiles
			Total Size: 20000 [D0, [3 x 5000, 5000]][Tile0, 5:[25x6, 3:25x7, 25x6], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 5:[25x6, 3:25x7, 25x6], 1][D0, [3 x 5000, 5000]]
		Tile0: [0, 1200, 150], Tile1: [5000, 1200, 150], Tile2; [10000, 1200, 150]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [D1, [0 x 128, 128]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 128, 128]]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [D1, [0 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 32, 32]]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [D1, [0 x 32, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 32, 32]]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: Filter, Tiled Space: D1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 20 logical tiles, 1 physical tiles
			Total Size: 9216 [D1, [0 x 9216, 9216]][D0, [3 x 2304, 2304]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 9216, 9216]][D0, [3 x 2304, 2304]]
		Tile0: [0, 9216, 288], Tile1: [0, 9216, 288], Tile2; [0, 9216, 288]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 5 physical tiles
			Total Size: 20000 [D1, [0 x 20000, 20000]][Tile0, 5:[25x5, 3:25x5, 25x5], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 20000, 20000]][Tile0, 5:[25x5, 3:25x5, 25x5], 1]
		Tile0: [0, 4000, 125], Tile1: [125, 4000, 125], Tile2; [250, 4000, 125]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 80000 [D1, [0 x 80000, 80000]][Tile0, 5:[25x5, 3:25x5, 25x5], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 80000, 80000]][Tile0, 5:[25x5, 3:25x5, 25x5], 4]
		Tile0: [0, 16000, 500], Tile1: [0, 16000, 500], Tile2; [0, 16000, 500]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 5:[9x1, 3:9x1, 9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 5:[9x1, 3:9x1, 9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (network_L1_Memory+29424);
	KerArg0->W = (unsigned short int) (25);
	KerArg0->H = (unsigned short int) (5);
	KerArg0->Feat = (unsigned short int) (32);
	KerArg0->Bias = (void * __restrict__) (network_L1_Memory+2800);
	KerArg1->W = (unsigned short int) (25);
	KerArg1->UsedW = (unsigned short int) (25);
	KerArg1->InFeatures = (unsigned short int) (8);
	KerArg1->OutFeatures = (unsigned short int) (32);
	KerArg1->TotalInFeatures = (unsigned short int) (32);
	KerArg1->Out = (int * __restrict__) (network_L1_Memory+29424);
	KerArg2->In = (int *__restrict__) (network_L1_Memory+29424);
	KerArg2->Feat = (unsigned short int) (32);
	KerArg2->W = (unsigned short int) (25);
	KerArg2->H = (unsigned short int) (5);
	KerArg2->Scale = (unsigned char *__restrict__) (network_L1_Memory+2928);
	KerArg2->ScaleN = (unsigned char *__restrict__) (network_L1_Memory+2960);
	KerArg2->Infos = (signed char *__restrict__) (network_L1_Memory+45424);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+0), 1200, 625, 150, 0, &DmaR_Evt1);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+2800), 128, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+2928), 32, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+2960), 32, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+2992+0), 9216, 0, &DmaR_Evt5);
	_N_Filter=0;
	_C_Out=0; _SC_Out=4000; _LC_Out=125;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+45424), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1, D1Ind_NextLast = 1;
		/*================================= Prepare Tiles ===================================*/
		_SN_Filter = 0;
		
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+2992+9216*((D1Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt5);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<5; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==4), T0Ind_NextLast = ((T0Ind+1)==4);
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->NormBias = (unsigned char) (((char *)(network_L1_Memory+45424))[5]);
			AT_FORK(gap_ncore(), (void *) KerParSetBiasB32_SQ8, (void *) KerArg0);
			__CALL(KerParSetBiasB32_SQ8, KerArg0);
			for (D0Ind=0; D0Ind<4; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==3), D0Ind_NextLast = ((D0Ind+1)==3);
				/*================================= Prepare Tiles ===================================*/
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (5000); _LN_In = ((T0Ind_Last)?150:(175-25*(T0Ind==0))); _SN_In = (8*_LN_In); 
				} else if (!(T0Ind_Last)) {
					_N_In = _N_In + (125-(25*(T0Ind==0)))+(-15000); _LN_In = ((T0Ind_NextLast)?150:175); _SN_In = (8*_LN_In); 
				} else if (!(1)) {
					_N_In = _N_In + (-475)+(-15000); _LN_In = (150); _SN_In = (8*_LN_In); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+1400*((D0Ind_Total+1)%2)),
							_SN_In, 625, _LN_In, 0, &DmaR_Evt1);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (network_L1_Memory+0+1400*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (7-1*(T0Ind==0)-1*(T0Ind_Last));
				KerArg1->UsedH = (unsigned short int) (7-1*(T0Ind==0)-1*(T0Ind_Last));
				KerArg1->Filter = (signed char * __restrict__) (network_L1_Memory+2992+((D0Ind)*72)+9216*((D1Ind_Total)%2));
				KerArg1->Pad = (v4s) ((v4s){1,1,1*(T0Ind==0),1*(T0Ind_Last)});
				AT_FORK(gap_ncore(), (void *) KerParConv3x3Stride1_SQ8, (void *) KerArg1);
				__CALL(KerParConv3x3Stride1_SQ8, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->Out = (void *__restrict__) (network_L1_Memory+21424+4000*((T0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg2);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg2);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+21424+4000*((T0Ind_Total)%2)),
					_SC_Out, 625, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (125); _LC_Out = (125); _SC_Out = (32*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		/*============================= End Update Arg Pipeline =============================*/
		D1Ind_Total++;
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S13_Conv2d_32x32x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 41260 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Total=0, T1Ind_Last, T1Ind_NextLast;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 5][Tile0 Dim: 1]
	Ker Arg: In2, Tiled Space: Tile1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 5 logical tiles, 5 physical tiles
			Total Size: 80000 [Tile1, 5:[32x500, 3:32x500, 32x500], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 5:[32x500, 3:32x500, 32x500], 1]
		Tile0: [0, 16000, 500], Tile1: [500, 16000, 500], Tile2; [1000, 16000, 500]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1024 [Tile0, 1:[32x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[32x32], 1]
		Tile0: [0, 1024, 1024], Tile1: [0, 1024, 1024], Tile2; [0, 1024, 1024]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 32 [Tile1, 5:[32x1, 3:32x1, 32x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 5:[32x1, 3:32x1, 32x1], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [Tile0, 1:[1x32], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 4]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Out, Tiled Space: Tile1
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 5 physical tiles
			Total Size: 20000 [Tile1, 5:[32x125, 3:32x125, 32x125], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 5:[32x125, 3:32x125, 32x125], 1]
		Tile0: [0, 4000, 125], Tile1: [125, 4000, 125], Tile2; [250, 4000, 125]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile0, 1:[1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile0, 1:[1x32], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x32], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 5 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 5:[1x1, 3:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 5:[1x1, 3:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (network_L1_Memory+40044);
	KerArg0->W_In1 = (unsigned short int) (32);
	KerArg0->H_In1 = (unsigned short int) (32);
	KerArg0->W_In2 = (unsigned short int) (500);
	KerArg0->Bias = (void * __restrict__) (network_L1_Memory+41068);
	KerArg0->Scale = (unsigned char * __restrict__) (network_L1_Memory+41196);
	KerArg0->ScaleN = (unsigned char * __restrict__) (network_L1_Memory+41228);
	KerArg0->W_Out = (unsigned short int) (125);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (network_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Sx = (unsigned char) (2);
	KerArg0->Sy = (unsigned char) (2);
	KerArg0->W = (unsigned short int) (50);
	KerArg0->H = (unsigned short int) (50);
	KerArg0->Infos = (signed char *__restrict__) (network_L1_Memory+40032);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+32+0), 16000, 2500, 500, 0, &DmaR_Evt1);
	_N_In2=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+40044), 1024, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+41068), 128, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	_C_Out=0; _SC_Out=4000; _LC_Out=125;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+41196), 32, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+41228), 32, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+40032), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (T1Ind=0; T1Ind<5; T1Ind++, T1Ind_Total++) { /* Iteration on Tile1 */
		int T1Ind_Last = (T1Ind==4), T1Ind_NextLast = ((T1Ind+1)==4);
		/*================================= Prepare Tiles ===================================*/
		_SN_In2 = 0;
		if (!(T1Ind_Last)) {
			_N_In2 = _N_In2 + (500); _LN_In2 = (500); _SN_In2 = (32*_LN_In2); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In2 */
		if (_SN_In2) {
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+32+16000*((T1Ind_Total+1)%2)),
					_SN_In2, 2500, _LN_In2, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In2 = (signed char * __restrict__) (network_L1_Memory+32+16000*((T1Ind_Total)%2));
			KerArg0->Out = (signed char * __restrict__) (network_L1_Memory+32032+4000*((T1Ind_Total)%2));
			KerArg0->OutFirstCol = (unsigned short int) ((0)*32);
			KerArg0->NormBias = (unsigned char) (((char *)(network_L1_Memory+40032))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulSxSyB32_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulSxSyB32_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
		/*================================= Write Tiles =====================================*/
		if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
		AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+32032+4000*((T1Ind_Total)%2)),
				_SC_Out, 625, _LC_Out, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		_SP_Out = _SC_Out;_LP_Out = _LC_Out;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_Out = 0;
		if (!(T1Ind_Last)) {
			_C_Out = _C_Out + (125); _LC_Out = (125); _SC_Out = (32*_LC_Out); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S14_MatAdd_32x25x25(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 43212 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last, D0Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In1;
	unsigned int _SN_In1;
	unsigned int _LN_In1;
	unsigned int _N_In2;
	unsigned int _SN_In2;
	unsigned int _LN_In2;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 32, Tiled: 1][Tile0 Dim: 3]
	Ker Arg: In1, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 20000 [D0, [0 x 20000, 20000]][Tile0, 3:[25x9, 1:25x9, 25x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 20000, 20000]][Tile0, 3:[25x9, 1:25x9, 25x7], 1]
		Tile0: [0, 7200, 225], Tile1: [225, 7200, 225], Tile2; [450, 5600, 175]
	Ker Arg: In2, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 20000 [D0, [0 x 20000, 20000]][Tile0, 3:[25x9, 1:25x9, 25x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 20000, 20000]][Tile0, 3:[25x9, 1:25x9, 25x7], 1]
		Tile0: [0, 7200, 225], Tile1: [225, 7200, 225], Tile2; [450, 5600, 175]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 20000 [D0, [0 x 20000, 20000]][Tile0, 3:[25x9, 1:25x9, 25x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 20000, 20000]][Tile0, 3:[25x9, 1:25x9, 25x7], 1]
		Tile0: [0, 7200, 225], Tile1: [225, 7200, 225], Tile2; [450, 5600, 175]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 3:[1x1, 1:1x1, 1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[1x1, 1:1x1, 1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Feat = (unsigned short int) (32);
	KerArg0->W = (unsigned short int) (25);
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (network_L1_Memory+43200);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+0), 7200, 625, 225, 0, &DmaR_Evt1);
	_N_In1=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+14400+0), 7200, 625, 225, 0, &DmaR_Evt2);
	_N_In2=0;
	_C_Out=0; _SC_Out=7200; _LC_Out=225;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+43200), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1, D0Ind_NextLast = 1;
		for (T0Ind=0; T0Ind<3; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==2), T0Ind_NextLast = ((T0Ind+1)==2);
			/*================================= Prepare Tiles ===================================*/
			_SN_In1 = 0;
			if (!(T0Ind_Last)) {
				_N_In1 = _N_In1 + (225); _LN_In1 = ((T0Ind_NextLast)?175:225); _SN_In1 = (32*_LN_In1); 
			}
			_SN_In2 = 0;
			if (!(T0Ind_Last)) {
				_N_In2 = _N_In2 + (225); _LN_In2 = ((T0Ind_NextLast)?175:225); _SN_In2 = (32*_LN_In2); 
			}
			/*============================= End Prepare Tiles ===================================*/
			/*================================= Read Tiles ======================================*/
			AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
			if (_SN_In1) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In1+_N_In1), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+7200*((T0Ind_Total+1)%2)),
						_SN_In1, 625, _LN_In1, 0, &DmaR_Evt1);
			}
			AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
			if (_SN_In2) {
				AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In2+_N_In2), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+14400+7200*((T0Ind_Total+1)%2)),
						_SN_In2, 625, _LN_In2, 0, &DmaR_Evt2);
			}
			/*============================= End Read Tiles ======================================*/
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->In1 = (signed char *__restrict__) (network_L1_Memory+0+7200*((T0Ind_Total)%2));
			KerArg0->In2 = (signed char *__restrict__) (network_L1_Memory+14400+7200*((T0Ind_Total)%2));
			KerArg0->Out = (signed char *__restrict__) (network_L1_Memory+28800+7200*((T0Ind_Total)%2));
			KerArg0->H = (unsigned short int) (T0Ind_Last?7:9);
			AT_FORK(gap_ncore(), (void *) KerParMatAdd_SQ8, (void *) KerArg0);
			__CALL(KerParMatAdd_SQ8, KerArg0);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+28800+7200*((T0Ind_Total)%2)),
					_SC_Out, 625, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (225); _LC_Out = ((T0Ind_NextLast)?175:225); _SC_Out = (32*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S17_Conv2d_64x32x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 45852 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerConvLinReduct_SQ8_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Total=0, D1Ind_Last, D1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: Init: 64, Tiled: 1][Tile0 Dim: 13][D0 Dim: Init: 32, Tiled: 2]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 13 logical tiles, 13 physical tiles
			Total Size: 10816 [D1, [0 x 10816, 10816]][Tile0, 13:[13x1, 11:13x1, 13x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 10816, 10816]][Tile0, 13:[13x1, 11:13x1, 13x1], 1]
		Tile0: [0, 832, 13], Tile1: [13, 832, 13], Tile2; [26, 832, 13]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 256 [D1, [0 x 256, 256]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 256, 256]]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [D1, [0 x 64, 64]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 64, 64]]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [D1, [0 x 64, 64]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 64, 64]]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Filter, Tiled Space: D1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 26 logical tiles, 1 physical tiles
			Total Size: 18432 [D1, [0 x 18432, 18432]][D0, [1 x 13824, 4608]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 18432, 18432]][D0, [1 x 13824, 4608]]
		Tile0: [0, 18432, 288], Tile1: [0, 18432, 288], Tile2; [0, 18432, 288]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 26 logical tiles, 26 physical tiles
			Total Size: 20000 [D0, [1 x 15000, 5000]][Tile0, 13:[25x2, 11:25x3, 25x2], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 13:[25x2, 11:25x3, 25x2], 1][D0, [1 x 15000, 5000]]
		Tile0: [0, 1200, 50], Tile1: [15000, 400, 50], Tile2; [25, 1800, 75]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 13 logical tiles, 1 physical tiles
			Total Size: 43264 [D1, [0 x 43264, 43264]][Tile0, 13:[13x1, 11:13x1, 13x1], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [0 x 43264, 43264]][Tile0, 13:[13x1, 11:13x1, 13x1], 4]
		Tile0: [0, 3328, 52], Tile1: [0, 3328, 52], Tile2; [0, 3328, 52]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 13 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 13:[9x1, 11:9x1, 9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 13:[9x1, 11:9x1, 9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (network_L1_Memory+42512);
	KerArg0->W = (unsigned short int) (13);
	KerArg0->H = (unsigned short int) (1);
	KerArg0->Feat = (unsigned short int) (64);
	KerArg0->Bias = (void * __restrict__) (network_L1_Memory+3600);
	KerArg1->W = (unsigned short int) (25);
	KerArg1->UsedW = (unsigned short int) (25);
	KerArg1->OutFeatures = (unsigned short int) (64);
	KerArg1->TotalInFeatures = (unsigned short int) (32);
	KerArg1->Out = (int * __restrict__) (network_L1_Memory+42512);
	KerArg2->In = (int *__restrict__) (network_L1_Memory+42512);
	KerArg2->Feat = (unsigned short int) (64);
	KerArg2->W = (unsigned short int) (13);
	KerArg2->H = (unsigned short int) (1);
	KerArg2->Scale = (unsigned char *__restrict__) (network_L1_Memory+3856);
	KerArg2->ScaleN = (unsigned char *__restrict__) (network_L1_Memory+3920);
	KerArg2->Infos = (signed char *__restrict__) (network_L1_Memory+45840);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=832; _LC_Out=13;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+3600), 256, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+3856), 64, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+3920), 64, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+3984+0), 18432, 0, &DmaR_Evt4);
	_N_Filter=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+0), 1200, 625, 50, 0, &DmaR_Evt5);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+45840), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1, D1Ind_NextLast = 1;
		/*================================= Prepare Tiles ===================================*/
		_SN_Filter = 0;
		
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+3984+18432*((D1Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt4);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<13; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==12), T0Ind_NextLast = ((T0Ind+1)==12);
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->NormBias = (unsigned char) (((char *)(network_L1_Memory+45840))[5]);
			AT_FORK(gap_ncore(), (void *) KerParSetBiasB32_SQ8, (void *) KerArg0);
			__CALL(KerParSetBiasB32_SQ8, KerArg0);
			for (D0Ind=0; D0Ind<2; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==1), D0Ind_NextLast = ((D0Ind+1)==1);
				/*================================= Prepare Tiles ===================================*/
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (15000); _LN_In = ((T0Ind_Last)?50:(75-25*(T0Ind==0))); _SN_In = (((1)?8:24)*_LN_In); 
				} else if (!(T0Ind_Last)) {
					_N_In = _N_In + (50-(25*(T0Ind==0)))+(-15000); _LN_In = ((T0Ind_NextLast)?50:75); _SN_In = (24*_LN_In); 
				} else if (!(1)) {
					_N_In = _N_In + (-575)+(-15000); _LN_In = (50); _SN_In = (24*_LN_In); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+1800*((D0Ind_Total+1)%2)),
							_SN_In, 625, _LN_In, 0, &DmaR_Evt5);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (network_L1_Memory+0+1800*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (3-1*(T0Ind==0)-1*(T0Ind_Last));
				KerArg1->UsedH = (unsigned short int) (3-1*(T0Ind==0)-1*(T0Ind_Last));
				KerArg1->InFeatures = (unsigned short int) (D0Ind_Last?8:24);
				KerArg1->Filter = (signed char * __restrict__) (network_L1_Memory+3984+((D0Ind)*216)+18432*((D1Ind_Total)%2));
				KerArg1->Pad = (v4s) ((v4s){1,1,1*(T0Ind==0),1*(T0Ind_Last)});
				AT_FORK(gap_ncore(), (void *) KerParConv3x3Stride2_SQ8, (void *) KerArg1);
				__CALL(KerParConv3x3Stride2_SQ8, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->Out = (void *__restrict__) (network_L1_Memory+40848+832*((T0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg2);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg2);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+40848+832*((T0Ind_Total)%2)),
					_SC_Out, 169, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (13); _LC_Out = (13); _SC_Out = (64*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		/*============================= End Update Arg Pipeline =============================*/
		D1Ind_Total++;
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S20_Conv2d_64x64x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 45188 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerConvLinReduct_SQ8_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Last, D1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _LN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: Init: 64, Tiled: 2][Tile0 Dim: 1][D0 Dim: Init: 64, Tiled: 16]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 10816 [D0, [15 x 676, 676]][Tile0, 1:[13x13], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[13x13], 1][D0, [15 x 676, 676]]
		Tile0: [0, 676, 169], Tile1: [676, 676, 169], Tile2; [1352, 676, 169]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 256 [D1, [1 x 160, 96]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 160, 96]]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 64 [D1, [1 x 40, 24]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 40, 24]]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 64 [D1, [1 x 40, 24]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 40, 24]]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 32 logical tiles, 32 physical tiles
			Total Size: 36864 [D1, [1 x 23040, 13824]][D0, [15 x 1440, 1440]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 23040, 13824]][D0, [15 x 1440, 1440]]
		Tile0: [0, 1440, 36], Tile1: [1440, 1440, 36], Tile2; [2880, 1440, 36]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 2 physical tiles
			Total Size: 10816 [D1, [1 x 6760, 4056]][Tile0, 1:[13x13], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 6760, 4056]][Tile0, 1:[13x13], 1]
		Tile0: [0, 6760, 169], Tile1: [6760, 4056, 169], Tile2; [0, 6760, 169]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 43264 [D1, [1 x 27040, 16224]][Tile0, 1:[13x13], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 27040, 16224]][Tile0, 1:[13x13], 4]
		Tile0: [0, 27040, 676], Tile1: [0, 27040, 676], Tile2; [0, 27040, 676]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (network_L1_Memory+18136);
	KerArg0->W = (unsigned short int) (13);
	KerArg0->H = (unsigned short int) (13);
	KerArg1->W = (unsigned short int) (13);
	KerArg1->UsedW = (unsigned short int) (13);
	KerArg1->InFeatures = (unsigned short int) (4);
	KerArg1->TotalInFeatures = (unsigned short int) (4);
	KerArg1->Out = (int * __restrict__) (network_L1_Memory+18136);
	KerArg1->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg2->In = (int *__restrict__) (network_L1_Memory+18136);
	KerArg2->W = (unsigned short int) (13);
	KerArg2->H = (unsigned short int) (13);
	KerArg2->Infos = (signed char *__restrict__) (network_L1_Memory+45176);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+0), 676, 169, 169, 0, &DmaR_Evt1);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+1352), 256, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+1608), 64, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+1672), 64, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+1736+0), 1440, 0, &DmaR_Evt5);
	_N_Filter=0;
	_C_Out=0; _SC_Out=6760; _LC_Out=169;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+45176), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D1Ind=0; D1Ind<2; D1Ind++) { /* Iteration on D1 */
		int D1Ind_Last = (D1Ind==1), D1Ind_NextLast = ((D1Ind+1)==1);
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->Feat = (unsigned short int) (D1Ind_Last?24:40);
			KerArg0->Bias = (void * __restrict__) (network_L1_Memory+1352+((D1Ind)*160));
			KerArg0->NormBias = (unsigned char) (((char *)(network_L1_Memory+45176))[5]);
			AT_FORK(gap_ncore(), (void *) KerParSetBiasB32_SQ8, (void *) KerArg0);
			__CALL(KerParSetBiasB32_SQ8, KerArg0);
			for (D0Ind=0; D0Ind<16; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==15), D0Ind_NextLast = ((D0Ind+1)==15);
				/*================================= Prepare Tiles ===================================*/
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (676); _LN_In = (169); _SN_In = (4*_LN_In); 
				} else if (!(D1Ind_Last)) {
					_N_In = _N_In + (-10140); _LN_In = (169); _SN_In = (4*_LN_In); 
				}
				_SN_Filter = 0;
				if (!(D0Ind_Last)) {
					_N_Filter = _N_Filter + (((D1Ind_Last)?(864):(1440))); _LN_Filter = (36); _SN_Filter = (((D1Ind_Last)?24:40)*_LN_Filter); 
				} else if (!((1))) {
					_N_Filter = _N_Filter + (((D1Ind_Last)?(-12960):(-21600))); _LN_Filter = (36); _SN_Filter = (((D1Ind_Last)?24:40)*_LN_Filter); 
				} else if (!(D1Ind_Last)) {
					_N_Filter = _N_Filter + (23040)+(-21600); _LN_Filter = (36); _SN_Filter = (((1)?24:40)*_LN_Filter); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+676*((D0Ind_Total+1)%2)),
							_SN_In, 169, _LN_In, 0, &DmaR_Evt1);
				}
				AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
				if (_SN_Filter) {
					AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+1736+1440*((D0Ind_Total+1)%2)),
							_SN_Filter, 0, &DmaR_Evt5);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (network_L1_Memory+0+676*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (15-1*(1)-1*(1));
				KerArg1->UsedH = (unsigned short int) (15-1*(1)-1*(1));
				KerArg1->OutFeatures = (unsigned short int) (D1Ind_Last?24:40);
				KerArg1->Filter = (signed char * __restrict__) (network_L1_Memory+1736+1440*((D0Ind_Total)%2));
				AT_FORK(gap_ncore(), (void *) KerParConv3x3Stride1_SQ8, (void *) KerArg1);
				__CALL(KerParConv3x3Stride1_SQ8, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->Out = (void *__restrict__) (network_L1_Memory+4616+6760*((T0Ind_Total)%2));
			KerArg2->Feat = (unsigned short int) (D1Ind_Last?24:40);
			KerArg2->Scale = (unsigned char *__restrict__) (network_L1_Memory+1608+((D1Ind)*40));
			KerArg2->ScaleN = (unsigned char *__restrict__) (network_L1_Memory+1672+((D1Ind)*40));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg2);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg2);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+4616+6760*((T0Ind_Total)%2)),
					_SC_Out, 169, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D1Ind_Last)) {
				_C_Out = _C_Out + (6760); _LC_Out = (169); _SC_Out = (((1)?24:40)*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S23_Conv2d_64x32x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 33292 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [Tile1, 1:[32x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[32x1], 1]
		Tile0: [0, 32, 32], Tile1: [0, 32, 32], Tile2; [0, 32, 32]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2048 [Tile0, 1:[32x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[32x64], 1]
		Tile0: [0, 2048, 2048], Tile1: [0, 2048, 2048], Tile2; [0, 2048, 2048]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 20000 [Tile1, 1:[32x625], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[32x625], 1]
		Tile0: [0, 20000, 20000], Tile1: [0, 20000, 20000], Tile2; [0, 20000, 20000]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 256 [Tile0, 1:[1x64], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x64], 4]
		Tile0: [0, 256, 256], Tile1: [0, 256, 256], Tile2; [0, 256, 256]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 10816 [Tile1, 1:[64x169], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[64x169], 1]
		Tile0: [0, 10816, 10816], Tile1: [0, 10816, 10816], Tile2; [0, 10816, 10816]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile0, 1:[1x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x64], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile0, 1:[1x64], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x64], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (network_L1_Memory+30860);
	KerArg0->W_In1 = (unsigned short int) (32);
	KerArg0->H_In1 = (unsigned short int) (64);
	KerArg0->In2 = (signed char * __restrict__) (network_L1_Memory+32);
	KerArg0->W_In2 = (unsigned short int) (625);
	KerArg0->Bias = (void * __restrict__) (network_L1_Memory+32908);
	KerArg0->Scale = (unsigned char * __restrict__) (network_L1_Memory+33164);
	KerArg0->ScaleN = (unsigned char * __restrict__) (network_L1_Memory+33228);
	KerArg0->Out = (signed char * __restrict__) (network_L1_Memory+20032);
	KerArg0->W_Out = (unsigned short int) (169);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (network_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Sx = (unsigned char) (2);
	KerArg0->Sy = (unsigned char) (2);
	KerArg0->W = (unsigned short int) (25);
	KerArg0->H = (unsigned short int) (25);
	KerArg0->Infos = (signed char *__restrict__) (network_L1_Memory+30848);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+30860), 2048, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+32), 20000, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+32908), 256, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+33164), 64, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+33228), 64, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+30848), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutFirstCol = (unsigned short int) ((0)*64);
			KerArg0->NormBias = (unsigned char) (((char *)(network_L1_Memory+30848))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulSxSyB32_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulSxSyB32_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+20032), 10816, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S24_MatAdd_64x13x13(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 32460 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 64, Tiled: 1][Tile0 Dim: 1]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 10816 [D0, [0 x 10816, 10816]][Tile0, 1:[13x13], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 10816, 10816]][Tile0, 1:[13x13], 1]
		Tile0: [0, 10816, 10816], Tile1: [0, 10816, 10816], Tile2; [0, 10816, 10816]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 10816 [D0, [0 x 10816, 10816]][Tile0, 1:[13x13], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 10816, 10816]][Tile0, 1:[13x13], 1]
		Tile0: [0, 10816, 10816], Tile1: [0, 10816, 10816], Tile2; [0, 10816, 10816]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 10816 [D0, [0 x 10816, 10816]][Tile0, 1:[13x13], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 10816, 10816]][Tile0, 1:[13x13], 1]
		Tile0: [0, 10816, 10816], Tile1: [0, 10816, 10816], Tile2; [0, 10816, 10816]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char *__restrict__) (network_L1_Memory+0);
	KerArg0->In2 = (signed char *__restrict__) (network_L1_Memory+10816);
	KerArg0->Out = (signed char *__restrict__) (network_L1_Memory+21632);
	KerArg0->Feat = (unsigned short int) (64);
	KerArg0->W = (unsigned short int) (13);
	KerArg0->H = (unsigned short int) (13);
	KerArg0->DoScale = (unsigned char) (1);
	KerArg0->Infos = (signed char *__restrict__) (network_L1_Memory+32448);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0), 10816, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+10816), 10816, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+32448), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			AT_FORK(gap_ncore(), (void *) KerParMatAdd_SQ8, (void *) KerArg0);
			__CALL(KerParMatAdd_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+21632), 10816, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S27_Conv2d_128x64x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 43124 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerConvLinReduct_SQ8_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Last, D1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _LN_Filter;
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: Init: 128, Tiled: 2][Tile0 Dim: 1][D0 Dim: Init: 64, Tiled: 16]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 2 physical tiles
			Total Size: 6272 [D1, [1 x 5488, 784]][Tile0, 1:[7x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 5488, 784]][Tile0, 1:[7x7], 1]
		Tile0: [0, 5488, 49], Tile1: [5488, 784, 49], Tile2; [0, 5488, 49]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 512 [D1, [1 x 448, 64]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 448, 64]]
		Tile0: [0, 512, 512], Tile1: [0, 512, 512], Tile2; [0, 512, 512]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 128 [D1, [1 x 112, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 112, 16]]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 128 [D1, [1 x 112, 16]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 112, 16]]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 32 logical tiles, 32 physical tiles
			Total Size: 73728 [D1, [1 x 64512, 9216]][D0, [15 x 4032, 4032]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 64512, 9216]][D0, [15 x 4032, 4032]]
		Tile0: [0, 4032, 36], Tile1: [4032, 4032, 36], Tile2; [8064, 4032, 36]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 10816 [D0, [15 x 676, 676]][Tile0, 1:[13x13], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[13x13], 1][D0, [15 x 676, 676]]
		Tile0: [0, 676, 169], Tile1: [676, 676, 169], Tile2; [1352, 676, 169]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 25088 [D1, [1 x 21952, 3136]][Tile0, 1:[7x7], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 21952, 3136]][Tile0, 1:[7x7], 4]
		Tile0: [0, 21952, 196], Tile1: [0, 21952, 196], Tile2; [0, 21952, 196]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (network_L1_Memory+21160);
	KerArg0->W = (unsigned short int) (7);
	KerArg0->H = (unsigned short int) (7);
	KerArg1->W = (unsigned short int) (13);
	KerArg1->UsedW = (unsigned short int) (13);
	KerArg1->InFeatures = (unsigned short int) (4);
	KerArg1->TotalInFeatures = (unsigned short int) (4);
	KerArg1->Out = (int * __restrict__) (network_L1_Memory+21160);
	KerArg1->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg2->In = (int *__restrict__) (network_L1_Memory+21160);
	KerArg2->W = (unsigned short int) (7);
	KerArg2->H = (unsigned short int) (7);
	KerArg2->Infos = (signed char *__restrict__) (network_L1_Memory+43112);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=5488; _LC_Out=49;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+1352), 512, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+1864), 128, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+1992), 128, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+2120+0), 4032, 0, &DmaR_Evt4);
	_N_Filter=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+0), 676, 169, 169, 0, &DmaR_Evt5);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+43112), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D1Ind=0; D1Ind<2; D1Ind++) { /* Iteration on D1 */
		int D1Ind_Last = (D1Ind==1), D1Ind_NextLast = ((D1Ind+1)==1);
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->Feat = (unsigned short int) (D1Ind_Last?16:112);
			KerArg0->Bias = (void * __restrict__) (network_L1_Memory+1352+((D1Ind)*448));
			KerArg0->NormBias = (unsigned char) (((char *)(network_L1_Memory+43112))[5]);
			AT_FORK(gap_ncore(), (void *) KerParSetBiasB32_SQ8, (void *) KerArg0);
			__CALL(KerParSetBiasB32_SQ8, KerArg0);
			for (D0Ind=0; D0Ind<16; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==15), D0Ind_NextLast = ((D0Ind+1)==15);
				/*================================= Prepare Tiles ===================================*/
				_SN_Filter = 0;
				if (!(D0Ind_Last)) {
					_N_Filter = _N_Filter + (((D1Ind_Last)?(576):(4032))); _LN_Filter = (36); _SN_Filter = (((D1Ind_Last)?16:112)*_LN_Filter); 
				} else if (!((1))) {
					_N_Filter = _N_Filter + (((D1Ind_Last)?(-8640):(-60480))); _LN_Filter = (36); _SN_Filter = (((D1Ind_Last)?16:112)*_LN_Filter); 
				} else if (!(D1Ind_Last)) {
					_N_Filter = _N_Filter + (64512)+(-60480); _LN_Filter = (36); _SN_Filter = (((1)?16:112)*_LN_Filter); 
				}
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (676); _LN_In = (169); _SN_In = (4*_LN_In); 
				} else if (!(D1Ind_Last)) {
					_N_In = _N_In + (-10140); _LN_In = (169); _SN_In = (4*_LN_In); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Filter */
				if (_SN_Filter) {
					AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+2120+4032*((D0Ind_Total+1)%2)),
							_SN_Filter, 0, &DmaR_Evt4);
				}
				AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+676*((D0Ind_Total+1)%2)),
							_SN_In, 169, _LN_In, 0, &DmaR_Evt5);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (network_L1_Memory+0+676*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (15-1*(1)-1*(1));
				KerArg1->UsedH = (unsigned short int) (15-1*(1)-1*(1));
				KerArg1->OutFeatures = (unsigned short int) (D1Ind_Last?16:112);
				KerArg1->Filter = (signed char * __restrict__) (network_L1_Memory+2120+4032*((D0Ind_Total)%2));
				AT_FORK(gap_ncore(), (void *) KerParConv3x3Stride2_SQ8, (void *) KerArg1);
				__CALL(KerParConv3x3Stride2_SQ8, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->Out = (void *__restrict__) (network_L1_Memory+10184+5488*((T0Ind_Total)%2));
			KerArg2->Feat = (unsigned short int) (D1Ind_Last?16:112);
			KerArg2->Scale = (unsigned char *__restrict__) (network_L1_Memory+1864+((D1Ind)*112));
			KerArg2->ScaleN = (unsigned char *__restrict__) (network_L1_Memory+1992+((D1Ind)*112));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg2);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg2);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+10184+5488*((T0Ind_Total)%2)),
					_SC_Out, 49, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D1Ind_Last)) {
				_C_Out = _C_Out + (5488); _LC_Out = (49); _SC_Out = (((1)?16:112)*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S30_Conv2d_128x128x3x3_Relu6(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 45092 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_SQ8_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerConvLinReduct_SQ8_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Last, D1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _LN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: Init: 128, Tiled: 2][Tile0 Dim: 1][D0 Dim: Init: 128, Tiled: 32]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 32 logical tiles, 32 physical tiles
			Total Size: 6272 [D0, [31 x 196, 196]][Tile0, 1:[7x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[7x7], 1][D0, [31 x 196, 196]]
		Tile0: [0, 196, 49], Tile1: [196, 196, 49], Tile2; [392, 196, 49]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 512 [D1, [1 x 480, 32]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 480, 32]]
		Tile0: [0, 512, 512], Tile1: [0, 512, 512], Tile2; [0, 512, 512]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 128 [D1, [1 x 120, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 120, 8]]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 128 [D1, [1 x 120, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 120, 8]]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 64 logical tiles, 64 physical tiles
			Total Size: 147456 [D1, [1 x 138240, 9216]][D0, [31 x 4320, 4320]]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 138240, 9216]][D0, [31 x 4320, 4320]]
		Tile0: [0, 4320, 36], Tile1: [4320, 4320, 36], Tile2; [8640, 4320, 36]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 2 physical tiles
			Total Size: 6272 [D1, [1 x 5880, 392]][Tile0, 1:[7x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 5880, 392]][Tile0, 1:[7x7], 1]
		Tile0: [0, 5880, 49], Tile1: [5880, 392, 49], Tile2; [0, 5880, 49]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 2 logical tiles, 1 physical tiles
			Total Size: 25088 [D1, [1 x 23520, 1568]][Tile0, 1:[7x7], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, [1 x 23520, 1568]][Tile0, 1:[7x7], 4]
		Tile0: [0, 23520, 196], Tile1: [0, 23520, 196], Tile2; [0, 23520, 196]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (network_L1_Memory+21560);
	KerArg0->W = (unsigned short int) (7);
	KerArg0->H = (unsigned short int) (7);
	KerArg1->W = (unsigned short int) (7);
	KerArg1->UsedW = (unsigned short int) (7);
	KerArg1->InFeatures = (unsigned short int) (4);
	KerArg1->TotalInFeatures = (unsigned short int) (4);
	KerArg1->Out = (int * __restrict__) (network_L1_Memory+21560);
	KerArg1->Pad = (v4s) ((v4s){1,1,1,1});
	KerArg2->In = (int *__restrict__) (network_L1_Memory+21560);
	KerArg2->W = (unsigned short int) (7);
	KerArg2->H = (unsigned short int) (7);
	KerArg2->Infos = (signed char *__restrict__) (network_L1_Memory+45080);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+0), 196, 49, 49, 0, &DmaR_Evt1);
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+392), 512, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+904), 128, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+1032), 128, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+1160+0), 4320, 0, &DmaR_Evt5);
	_N_Filter=0;
	_C_Out=0; _SC_Out=5880; _LC_Out=49;
	_SP_Out=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+45080), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	for (D1Ind=0; D1Ind<2; D1Ind++) { /* Iteration on D1 */
		int D1Ind_Last = (D1Ind==1), D1Ind_NextLast = ((D1Ind+1)==1);
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->Feat = (unsigned short int) (D1Ind_Last?8:120);
			KerArg0->Bias = (void * __restrict__) (network_L1_Memory+392+((D1Ind)*480));
			KerArg0->NormBias = (unsigned char) (((char *)(network_L1_Memory+45080))[5]);
			AT_FORK(gap_ncore(), (void *) KerParSetBiasB32_SQ8, (void *) KerArg0);
			__CALL(KerParSetBiasB32_SQ8, KerArg0);
			for (D0Ind=0; D0Ind<32; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==31), D0Ind_NextLast = ((D0Ind+1)==31);
				/*================================= Prepare Tiles ===================================*/
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (196); _LN_In = (49); _SN_In = (4*_LN_In); 
				} else if (!(D1Ind_Last)) {
					_N_In = _N_In + (-6076); _LN_In = (49); _SN_In = (4*_LN_In); 
				}
				_SN_Filter = 0;
				if (!(D0Ind_Last)) {
					_N_Filter = _N_Filter + (((D1Ind_Last)?(288):(4320))); _LN_Filter = (36); _SN_Filter = (((D1Ind_Last)?8:120)*_LN_Filter); 
				} else if (!((1))) {
					_N_Filter = _N_Filter + (((D1Ind_Last)?(-8928):(-133920))); _LN_Filter = (36); _SN_Filter = (((D1Ind_Last)?8:120)*_LN_Filter); 
				} else if (!(D1Ind_Last)) {
					_N_Filter = _N_Filter + (138240)+(-133920); _LN_Filter = (36); _SN_Filter = (((1)?8:120)*_LN_Filter); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0+196*((D0Ind_Total+1)%2)),
							_SN_In, 49, _LN_In, 0, &DmaR_Evt1);
				}
				AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read Filter */
				if (_SN_Filter) {
					AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+1160+4320*((D0Ind_Total+1)%2)),
							_SN_Filter, 0, &DmaR_Evt5);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (network_L1_Memory+0+196*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (9-1*(1)-1*(1));
				KerArg1->UsedH = (unsigned short int) (9-1*(1)-1*(1));
				KerArg1->OutFeatures = (unsigned short int) (D1Ind_Last?8:120);
				KerArg1->Filter = (signed char * __restrict__) (network_L1_Memory+1160+4320*((D0Ind_Total)%2));
				AT_FORK(gap_ncore(), (void *) KerParConv3x3Stride1_SQ8, (void *) KerArg1);
				__CALL(KerParConv3x3Stride1_SQ8, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->Out = (void *__restrict__) (network_L1_Memory+9800+5880*((T0Ind_Total)%2));
			KerArg2->Feat = (unsigned short int) (D1Ind_Last?8:120);
			KerArg2->Scale = (unsigned char *__restrict__) (network_L1_Memory+904+((D1Ind)*120));
			KerArg2->ScaleN = (unsigned char *__restrict__) (network_L1_Memory+1032+((D1Ind)*120));
			AT_FORK(gap_ncore(), (void *) KerParReduct_CC_ReLU_SQ8, (void *) KerArg2);
			__CALL(KerParReduct_CC_ReLU_SQ8, KerArg2);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+9800+5880*((T0Ind_Total)%2)),
					_SC_Out, 49, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D1Ind_Last)) {
				_C_Out = _C_Out + (5880); _LC_Out = (49); _SC_Out = (((1)?8:120)*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S33_Conv2d_128x64x1x1_Relu6(
		signed char * __restrict__ In2,
		signed char * __restrict__ In1,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 26124 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	KerMatMul_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T1Ind, T1Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile1 Dim: 1][Tile0 Dim: 1]
	Ker Arg: KerBuff, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [Tile1, 1:[64x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[64x1], 1]
		Tile0: [0, 64, 64], Tile1: [0, 64, 64], Tile2; [0, 64, 64]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 8192 [Tile0, 1:[64x128], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[64x128], 1]
		Tile0: [0, 8192, 8192], Tile1: [0, 8192, 8192], Tile2; [0, 8192, 8192]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 10816 [Tile1, 1:[64x169], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[64x169], 1]
		Tile0: [0, 10816, 10816], Tile1: [0, 10816, 10816], Tile2; [0, 10816, 10816]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 512 [Tile0, 1:[1x128], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x128], 4]
		Tile0: [0, 512, 512], Tile1: [0, 512, 512], Tile2; [0, 512, 512]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6272 [Tile1, 1:[128x49], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[128x49], 1]
		Tile0: [0, 6272, 6272], Tile1: [0, 6272, 6272], Tile2; [0, 6272, 6272]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [Tile0, 1:[1x128], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x128], 1]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [Tile0, 1:[1x128], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x128], 1]
		Tile0: [0, 128, 128], Tile1: [0, 128, 128], Tile2; [0, 128, 128]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile1, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile1, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char * __restrict__) (network_L1_Memory+17164);
	KerArg0->W_In1 = (unsigned short int) (64);
	KerArg0->H_In1 = (unsigned short int) (128);
	KerArg0->In2 = (signed char * __restrict__) (network_L1_Memory+64);
	KerArg0->W_In2 = (unsigned short int) (169);
	KerArg0->Bias = (void * __restrict__) (network_L1_Memory+25356);
	KerArg0->Scale = (unsigned char * __restrict__) (network_L1_Memory+25868);
	KerArg0->ScaleN = (unsigned char * __restrict__) (network_L1_Memory+25996);
	KerArg0->Out = (signed char * __restrict__) (network_L1_Memory+10880);
	KerArg0->W_Out = (unsigned short int) (49);
	KerArg0->BufferColIn2 = (signed char * __restrict__) (network_L1_Memory+0);
	KerArg0->ColFirst = (unsigned char) (1);
	KerArg0->Sx = (unsigned char) (2);
	KerArg0->Sy = (unsigned char) (2);
	KerArg0->W = (unsigned short int) (13);
	KerArg0->H = (unsigned short int) (13);
	KerArg0->Infos = (signed char *__restrict__) (network_L1_Memory+17152);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+17164), 8192, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+64), 10816, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+25356), 512, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+25868), 128, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+25996), 128, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+17152), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile1 */
		int T1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->OutFirstCol = (unsigned short int) ((0)*128);
			KerArg0->NormBias = (unsigned char) (((char *)(network_L1_Memory+17152))[5]);
			AT_FORK(gap_ncore(), (void *) KerParMatMulSxSyB32_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatMulSxSyB32_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on Tile1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+10880), 6272, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S34_MatAdd_128x7x7_Relu6(
		signed char * __restrict__ In1,
		signed char * __restrict__ In2,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 18828 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerMat3_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 128, Tiled: 1][Tile0 Dim: 1]
	Ker Arg: In1, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6272 [D0, [0 x 6272, 6272]][Tile0, 1:[7x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 6272, 6272]][Tile0, 1:[7x7], 1]
		Tile0: [0, 6272, 6272], Tile1: [0, 6272, 6272], Tile2; [0, 6272, 6272]
	Ker Arg: In2, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6272 [D0, [0 x 6272, 6272]][Tile0, 1:[7x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 6272, 6272]][Tile0, 1:[7x7], 1]
		Tile0: [0, 6272, 6272], Tile1: [0, 6272, 6272], Tile2; [0, 6272, 6272]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6272 [D0, [0 x 6272, 6272]][Tile0, 1:[7x7], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 6272, 6272]][Tile0, 1:[7x7], 1]
		Tile0: [0, 6272, 6272], Tile1: [0, 6272, 6272], Tile2; [0, 6272, 6272]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In1 = (signed char *__restrict__) (network_L1_Memory+0);
	KerArg0->In2 = (signed char *__restrict__) (network_L1_Memory+6272);
	KerArg0->Out = (signed char *__restrict__) (network_L1_Memory+12544);
	KerArg0->Feat = (unsigned short int) (128);
	KerArg0->W = (unsigned short int) (7);
	KerArg0->H = (unsigned short int) (7);
	KerArg0->DoScale = (unsigned char) (0);
	KerArg0->Infos = (signed char *__restrict__) (network_L1_Memory+18816);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In1+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0), 6272, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In1 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In2+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+6272), 6272, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read In2 */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+18816), 9, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			AT_FORK(gap_ncore(), (void *) KerParMatAdd_ReLU_SQ8, (void *) KerArg0);
			__CALL(KerParMatAdd_ReLU_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+12544), 6272, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S38_Linear_2x6272(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		int * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned char * __restrict__ Scale,
		unsigned char * __restrict__ ScaleN,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 18848 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaR_Evt4;
	AT_L2_EVENT DmaR_Evt5;
	AT_L2_EVENT DmaR_Evt6;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	KerLinear_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 2, Tiled: 1][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 6272 [Tile0, 1:[1x1], 6272]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 6272]
		Tile0: [0, 6272, 6272], Tile1: [0, 6272, 6272], Tile2; [0, 6272, 6272]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 12544 [D0, [0 x 12544, 12544]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 12544, 12544]]
		Tile0: [0, 12544, 12544], Tile1: [0, 12544, 12544], Tile2; [0, 12544, 12544]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 8 [D0, [0 x 8, 8]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 8, 8]]
		Tile0: [0, 8, 8], Tile1: [0, 8, 8], Tile2; [0, 8, 8]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2 [D0, [0 x 2, 2]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 2, 2]]
		Tile0: [0, 2, 2], Tile1: [0, 2, 2], Tile2; [0, 2, 2]
	Ker Arg: Scale, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2 [D0, [0 x 2, 2]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 2, 2]]
		Tile0: [0, 2, 2], Tile1: [0, 2, 2], Tile2; [0, 2, 2]
	Ker Arg: ScaleN, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2 [D0, [0 x 2, 2]]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 2, 2]]
		Tile0: [0, 2, 2], Tile1: [0, 2, 2], Tile2; [0, 2, 2]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[1x1], 9]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 9]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char * __restrict__) (network_L1_Memory+0);
	KerArg0->Weights = (signed char * __restrict__) (network_L1_Memory+6272);
	KerArg0->Bias = (void * __restrict__) (network_L1_Memory+18816);
	KerArg0->Out = (void * __restrict__) (network_L1_Memory+18824);
	KerArg0->InDim = (unsigned short int) (6272);
	KerArg0->TotalInDim = (unsigned short int) (6272);
	KerArg0->OutDim = (unsigned short int) (2);
	KerArg0->Scale = (unsigned char *__restrict__) (network_L1_Memory+18828);
	KerArg0->ScaleN = (unsigned char *__restrict__) (network_L1_Memory+18832);
	KerArg0->Infos = (signed char *__restrict__) (network_L1_Memory+18836);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0), 6272, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Filter+0), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory+34680+0), 12544, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read Filter */
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L2_Memory+34680+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+6272), 12544, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+18816), 8, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Scale+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+18828), 2, 0, &DmaR_Evt4);
	AT_L2_WAIT(0, &DmaR_Evt4); /* Wait previous DMA read Scale */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ScaleN+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+18832), 2, 0, &DmaR_Evt5);
	AT_L2_WAIT(0, &DmaR_Evt5); /* Wait previous DMA read ScaleN */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Infos+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+18836), 9, 0, &DmaR_Evt6);
	AT_L2_WAIT(0, &DmaR_Evt6); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			AT_FORK(gap_ncore(), (void *) KerParLinearLayerFullFeatB32_SQ8, (void *) KerArg0);
			__CALL(KerParLinearLayerFullFeatB32_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+18824), 2, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S40_Op_Gemm_25_split_copy_0(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 8 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerCopy_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1 [Tile0, 1:[1x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 1]
		Tile0: [0, 1, 1], Tile1: [0, 1, 1], Tile2; [0, 1, 1]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1 [Tile0, 1:[1x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 1]
		Tile0: [0, 1, 1], Tile1: [0, 1, 1], Tile2; [0, 1, 1]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char *__restrict__) (network_L1_Memory+0);
	KerArg0->Out = (signed char *__restrict__) (network_L1_Memory+4);
	KerArg0->W = (unsigned int) (1);
	KerArg0->H = (unsigned int) (1);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0), 1, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		AT_FORK(gap_ncore(), (void *) CNN_Copy_fps, (void *) KerArg0);
		__CALL(CNN_Copy_fps, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+4), 1, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S42_Act_Hsigmoid(
		signed char * __restrict__ In,
		signed char * __restrict__ Out,
		signed char * __restrict__ Infos)

{
	/* Shared L1: 20 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaW_Evt1;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	KerActivation_SQ8_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: Init: 1, Tiled: 1][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1 [D0, [0 x 1, 1]][Tile0, 1:[1x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 1, 1]][Tile0, 1:[1x1], 1]
		Tile0: [0, 1, 1], Tile1: [0, 1, 1], Tile2; [0, 1, 1]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1 [D0, [0 x 1, 1]][Tile0, 1:[1x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, [0 x 1, 1]][Tile0, 1:[1x1], 1]
		Tile0: [0, 1, 1], Tile1: [0, 1, 1], Tile2; [0, 1, 1]
	Ker Arg: Infos, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9 [Tile0, 1:[9x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[9x1], 1]
		Tile0: [0, 9, 9], Tile1: [0, 9, 9], Tile2; [0, 9, 9]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char *__restrict__) (network_L1_Memory+0);
	KerArg0->Out = (signed char *__restrict__) (network_L1_Memory+4);
	KerArg0->Feat = (unsigned short int) (1);
	KerArg0->W = (unsigned short int) (1);
	KerArg0->H = (unsigned short int) (1);
	KerArg0->Infos = (signed char *__restrict__) (network_L1_Memory+8);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+0), 1, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) Infos+0), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory+28408+0), 9, 0, &UchanHR1);
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1); /* Wait previous uDMA read Infos */
	AT_L2_COPY(0, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L2_Memory+28408+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+8), 9, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Infos */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			AT_FORK(gap_ncore(), (void *) KerPar_HSigmoid_SQ8, (void *) KerArg0);
			__CALL(KerPar_HSigmoid_SQ8, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) network_L1_Memory+4), 1, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
int networkCNN_Construct()

{
	AT_HYPERFLASH_FS_FC_EVENT UchanHF1;
	AT_HYPERRAM_FC_EVENT UchanHR2;
	AT_HYPERFLASH_FS_CONF_T HyperFlashConf;
	int Error;
	AT_HYPERFLASH_FS_CONF_INIT(&HyperFlashConf, AT_MEM_L3_HFLASH, 0);
	AT_HYPERFLASH_FS_OPEN(&HyperFlash, &HyperFlashConf, "network_L3_Flash_Const.dat", &Error);
	if (Error) return 1;
	network_L3_Memory = (AT_HYPERRAM_POINTER) AT_HYPERRAM_ALLOC(&HyperRam, 322934);
	if (network_L3_Memory == 0) return 2;
	network_L2_Memory = (AT_L2_POINTER) AT_L2_ALLOC(0, 200000);
	if (network_L2_Memory == 0) return 3;
	network_L1_Memory = (AT_L1_POINTER) AT_L1_ALLOC(0, 46400);
	if (network_L1_Memory == 0) return 4;
	/* Moving S5_Conv_2_weights, size 9216 from HyperFlash at 289024 to (size 9216) HyperRam at 289024..298239 */
	{
		int Size = 9216, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 289024+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 289024+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S6_Constant_92, size 128 from HyperFlash at 321952 to (size 128) HyperRam at 321024..321151 */
	{
		int Size = 128, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 321952+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321024+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S7_Infos, size 9 from HyperFlash at 323756 to (size 9) HyperRam at 322752..322760 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323756+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322752+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S7_Mul_scale, size 32 from HyperFlash at 323552 to (size 32) HyperRam at 322560..322591 */
	{
		int Size = 32, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323552+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322560+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S7_Mul_shift, size 32 from HyperFlash at 323584 to (size 32) HyperRam at 322592..322623 */
	{
		int Size = 32, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323584+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322592+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S8_Conv_4_weights, size 9216 from HyperFlash at 298240 to (size 9216) HyperRam at 298240..307455 */
	{
		int Size = 9216, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 298240+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 298240+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S9_Constant_95, size 128 from HyperFlash at 322080 to (size 128) HyperRam at 321152..321279 */
	{
		int Size = 128, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 322080+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321152+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S10_Infos, size 9 from HyperFlash at 323768 to (size 9) HyperRam at 322764..322772 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323768+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322764+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S10_Mul_scale, size 32 from HyperFlash at 323616 to (size 32) HyperRam at 322624..322655 */
	{
		int Size = 32, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323616+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322624+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S10_Mul_shift, size 32 from HyperFlash at 323648 to (size 32) HyperRam at 322656..322687 */
	{
		int Size = 32, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323648+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322656+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S11_Conv_6_weights, size 1024 from HyperFlash at 317696 to (size 1024) HyperRam at 317696..318719 */
	{
		int Size = 1024, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 317696+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 317696+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S12_Constant_resblock1.bypass.bias, size 128 from HyperFlash at 322208 to (size 128) HyperRam at 321280..321407 */
	{
		int Size = 128, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 322208+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321280+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S13_Infos, size 9 from HyperFlash at 323780 to (size 9) HyperRam at 322776..322784 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323780+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322776+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S13_Mul_scale, size 32 from HyperFlash at 323680 to (size 32) HyperRam at 322688..322719 */
	{
		int Size = 32, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323680+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322688+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S13_Mul_shift, size 32 from HyperFlash at 323712 to (size 32) HyperRam at 322720..322751 */
	{
		int Size = 32, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323712+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322720+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S14_Infos, size 9 from HyperFlash at 323792 to (size 9) HyperRam at 322788..322796 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323792+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322788+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S17_Infos, size 9 from HyperFlash at 323804 to (size 9) HyperRam at 322800..322808 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323804+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322800+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S17_Mul_scale, size 64 from HyperFlash at 323104 to (size 64) HyperRam at 322176..322239 */
	{
		int Size = 64, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323104+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322176+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S17_Mul_shift, size 64 from HyperFlash at 323168 to (size 64) HyperRam at 322240..322303 */
	{
		int Size = 64, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323168+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322240+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S18_Conv_11_weights, size 36864 from HyperFlash at 221184 to (size 36864) HyperRam at 221184..258047 */
	{
		int Size = 36864, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 221184+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 221184+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S19_Constant_101, size 256 from HyperFlash at 321312 to (size 256) HyperRam at 320512..320767 */
	{
		int Size = 256, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 321312+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 320512+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S20_Infos, size 9 from HyperFlash at 323816 to (size 9) HyperRam at 322812..322820 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323816+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322812+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S20_Mul_scale, size 64 from HyperFlash at 323232 to (size 64) HyperRam at 322304..322367 */
	{
		int Size = 64, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323232+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322304+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S20_Mul_shift, size 64 from HyperFlash at 323296 to (size 64) HyperRam at 322368..322431 */
	{
		int Size = 64, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323296+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322368+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S21_Conv_13_weights, size 2048 from HyperFlash at 315648 to (size 2048) HyperRam at 315648..317695 */
	{
		int Size = 2048, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 315648+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 315648+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S22_Constant_resblock2.bypass.bias, size 256 from HyperFlash at 321568 to (size 256) HyperRam at 320768..321023 */
	{
		int Size = 256, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 321568+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 320768+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S23_Infos, size 9 from HyperFlash at 323828 to (size 9) HyperRam at 322824..322832 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323828+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322824+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S23_Mul_scale, size 64 from HyperFlash at 323360 to (size 64) HyperRam at 322432..322495 */
	{
		int Size = 64, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323360+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322432+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S23_Mul_shift, size 64 from HyperFlash at 323424 to (size 64) HyperRam at 322496..322559 */
	{
		int Size = 64, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323424+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322496+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S24_Infos, size 9 from HyperFlash at 323840 to (size 9) HyperRam at 322836..322844 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323840+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322836+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S25_Conv_16_weights, size 73728 from HyperFlash at 147456 to (size 73728) HyperRam at 147456..221183 */
	{
		int Size = 73728, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 147456+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 147456+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S27_Infos, size 9 from HyperFlash at 323852 to (size 9) HyperRam at 322848..322856 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323852+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322848+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S27_Mul_scale, size 128 from HyperFlash at 322336 to (size 128) HyperRam at 321408..321535 */
	{
		int Size = 128, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 322336+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321408+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S27_Mul_shift, size 128 from HyperFlash at 322464 to (size 128) HyperRam at 321536..321663 */
	{
		int Size = 128, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 322464+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321536+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S28_Conv_18_weights, size 147456 from HyperFlash at 0 to (size 147456) HyperRam at 0..147455 */
	{
		int Size = 147456, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 0+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 0+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S29_Constant_107, size 512 from HyperFlash at 320032 to (size 512) HyperRam at 319232..319743 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 320032+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 319232+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S30_Infos, size 9 from HyperFlash at 323864 to (size 9) HyperRam at 322860..322868 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323864+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322860+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S30_Mul_scale, size 128 from HyperFlash at 322592 to (size 128) HyperRam at 321664..321791 */
	{
		int Size = 128, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 322592+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321664+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S30_Mul_shift, size 128 from HyperFlash at 322720 to (size 128) HyperRam at 321792..321919 */
	{
		int Size = 128, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 322720+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321792+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S32_Constant_resblock3.bypass.bias, size 512 from HyperFlash at 320544 to (size 512) HyperRam at 319744..320255 */
	{
		int Size = 512, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 320544+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 319744+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S33_Infos, size 9 from HyperFlash at 323876 to (size 9) HyperRam at 322872..322880 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323876+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322872+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S33_Mul_scale, size 128 from HyperFlash at 322848 to (size 128) HyperRam at 321920..322047 */
	{
		int Size = 128, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 322848+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321920+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S33_Mul_shift, size 128 from HyperFlash at 322976 to (size 128) HyperRam at 322048..322175 */
	{
		int Size = 128, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 322976+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322048+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S34_Infos, size 9 from HyperFlash at 323888 to (size 9) HyperRam at 322884..322892 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323888+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322884+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S36_Gemm_25_weights, size 12544 from HyperFlash at 276480 to (size 12544) HyperRam at 276480..289023 */
	{
		int Size = 12544, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 276480+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 276480+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S38_Infos, size 9 from HyperFlash at 323900 to (size 9) HyperRam at 322896..322904 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323900+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322896+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S38_Mul_scale, size 2 from HyperFlash at 323932 to (size 2) HyperRam at 322928..322929 */
	{
		int Size = 2, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323932+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322928+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S38_Mul_shift, size 2 from HyperFlash at 323936 to (size 2) HyperRam at 322932..322933 */
	{
		int Size = 2, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323936+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322932+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S42_Infos, size 9 from HyperFlash at 323912 to (size 9) HyperRam at 322908..322916 */
	{
		int Size = 9, Base = 0;
		while (Size) {
			int Chunk = Min(Size, 1024);
			AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323912+Base), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 0, &UchanHF1);
			AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
			AT_HYPERRAM_FC_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322908+Base), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 0), Chunk, 1, &UchanHR2);
			AT_HYPERRAM_FC_WAIT(&HyperRam, &UchanHR2);
			Base += Chunk;
			Size -= Chunk;
		}
	}
	/* Moving S2_Conv_0_weights, size 800 from HyperFlash at 318720 to (size 800) L2 at 0..799 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 318720), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 0), 800, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S3_Constant_layer1.bias, size 128 from HyperFlash at 321824 to (size 128) L2 at 800..927 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 321824), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 800), 128, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S4_Infos, size 9 from HyperFlash at 323744 to (size 9) L2 at 992..1000 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323744), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 992), 9, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S4_Mul_scale, size 32 from HyperFlash at 323488 to (size 32) L2 at 928..959 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323488), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 928), 32, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S4_Mul_shift, size 32 from HyperFlash at 323520 to (size 32) L2 at 960..991 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323520), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 960), 32, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S15_Conv_9_weights, size 18432 from HyperFlash at 258048 to (size 18432) L2 at 1004..19435 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 258048), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 1004), 18432, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S16_Constant_98, size 256 from HyperFlash at 321056 to (size 256) L2 at 28140..28395 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 321056), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 28140), 256, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S26_Constant_104, size 512 from HyperFlash at 319520 to (size 512) L2 at 27628..28139 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 319520), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 27628), 512, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S31_Conv_20_weights, size 8192 from HyperFlash at 307456 to (size 8192) L2 at 19436..27627 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 307456), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 19436), 8192, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	/* Moving S37_Gemm_25_biases, size 8 from HyperFlash at 323924 to (size 8) L2 at 28396..28403 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) network_L3_Flash + 323924), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) network_L2_Memory + 28396), 8, 0, &UchanHF1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &UchanHF1);
	return 0;
}
int networkCNN_Destruct()

{
	AT_HYPERRAM_FREE(&HyperRam, network_L3_Memory, 322934);
	AT_L2_FREE(0, network_L2_Memory, 200000);
	AT_L1_FREE(0, network_L1_Memory, 46400);
	AT_HYPERFLASH_FS_CLOSE(&HyperFlash);
	return 0;
}
unsigned int AT_GraphPerf[17];
unsigned int AT_GraphOperInfosNames[17] = {
	40000,
	8320000,
	5780000,
	5780000,
	640000,
	20000,
	3125824,
	6240832,
	346112,
	10816,
	3618944,
	7231616,
	401408,
	6272,
	12544,
	0,
	1,
};
char *AT_GraphNodeNames[17] = {
	"S1_Op_input_1_formatter",
	"S4_Conv2d_32x1x5x5_MaxPool_2x2",
	"S7_Conv2d_32x32x3x3_Relu6",
	"S10_Conv2d_32x32x3x3_Relu6",
	"S13_Conv2d_32x32x1x1_Relu6",
	"S14_MatAdd_32x25x25",
	"S17_Conv2d_64x32x3x3_Relu6",
	"S20_Conv2d_64x64x3x3_Relu6",
	"S23_Conv2d_64x32x1x1_Relu6",
	"S24_MatAdd_64x13x13",
	"S27_Conv2d_128x64x3x3_Relu6",
	"S30_Conv2d_128x128x3x3_Relu6",
	"S33_Conv2d_128x64x1x1_Relu6",
	"S34_MatAdd_128x7x7_Relu6",
	"S38_Linear_2x6272",
	"S40_Op_Gemm_25_split_copy_0",
	"S42_Act_Hsigmoid",
};
int networkCNN(
		unsigned char * __restrict__ S0_Input_1,
		signed char * __restrict__ Output_1,
		signed char * __restrict__ Output_2)

{
	AT_HYPERRAM_CL_EVENT UchanHR0;
	AT_HYPERRAM_CL_EVENT UchanHR1;
	AT_HYPERRAM_CL_EVENT UchanHR2;
	AT_HYPERRAM_CL_EVENT UchanHR3;
	AT_HYPERRAM_CL_EVENT UchanHR4;
	AT_HYPERRAM_CL_EVENT UchanHR5;
	AT_HYPERRAM_CL_EVENT UchanHR6;
	AT_HYPERRAM_CL_EVENT UchanHR7;
	AT_HYPERRAM_CL_EVENT UchanHR8;
	AT_HYPERRAM_CL_EVENT UchanHR9;
	AT_HYPERRAM_CL_EVENT UchanHR10;
	AT_HYPERRAM_CL_EVENT UchanHR11;
	AT_HYPERRAM_CL_EVENT UchanHR12;
	AT_HYPERRAM_CL_EVENT UchanHR13;
	AT_HYPERRAM_CL_EVENT UchanHR14;
	AT_HYPERRAM_CL_EVENT UchanHR15;
	AT_HYPERRAM_CL_EVENT UchanHR16;
	AT_HYPERRAM_CL_EVENT UchanHR17;
	/* Moving S5_Conv_2_weights, size 9216 from HyperRam at 289024 to (size 9216) L2 at 148404 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 289024), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 148404), 9216, 0, &UchanHR0);
	/* Moving S6_Constant_92, size 128 from HyperRam at 321024 to (size 128) L2 at 166836 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321024), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 166836), 128, 0, &UchanHR1);
	/* Moving S7_Infos, size 9 from HyperRam at 322752 to (size 9) L2 at 166964 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322752), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 166964), 9, 0, &UchanHR2);
	/* Moving S8_Conv_4_weights, size 9216 from HyperRam at 298240 to (size 9216) L2 at 157620 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 298240), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 157620), 9216, 0, &UchanHR3);
	AT_GraphPerf[0] = gap_cl_readhwtimer();
	S1_Op_input_1_formatter(
		((unsigned char * __restrict__) S0_Input_1), /* In */
		((signed char * __restrict__) (network_L2_Memory+108404)) /* Out */
	);
	AT_GraphPerf[0] = gap_cl_readhwtimer() - AT_GraphPerf[0];
	AT_GraphPerf[1] = gap_cl_readhwtimer();
	S4_Conv2d_32x1x5x5_MaxPool_2x2(
		((signed char * __restrict__) (network_L2_Memory+108404)), /* In */
		((signed char * __restrict__) (network_L2_Memory+0)), /* Filter */
		((signed int * __restrict__) (network_L2_Memory+800)), /* Bias */
		((signed char * __restrict__) (network_L2_Memory+28404)), /* Out */
		((unsigned char * __restrict__) (network_L2_Memory+928)), /* Scale */
		((signed char * __restrict__) (network_L2_Memory+960)), /* ScaleN */
		((signed char * __restrict__) (network_L2_Memory+992)) /* Infos */
	);
	AT_GraphPerf[1] = gap_cl_readhwtimer() - AT_GraphPerf[1];
	/* Moving S7_Mul_scale, size 32 from HyperRam at 322560 to (size 32) L2 at 128404 using event 4 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322560), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 128404), 32, 0, &UchanHR4);
	/* Moving S7_Mul_shift, size 32 from HyperRam at 322592 to (size 32) L2 at 128436 using event 5 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322592), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 128436), 32, 0, &UchanHR5);
	/* Waiting completion of transfer of S5_Conv_2_weights using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR0);
	/* Waiting completion of transfer of S6_Constant_92 using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1);
	/* Waiting completion of transfer of S7_Infos using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2);
	/* Waiting completion of transfer of S7_Mul_scale using event 4 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR4);
	/* Waiting completion of transfer of S7_Mul_shift using event 5 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR5);
	AT_GraphPerf[2] = gap_cl_readhwtimer();
	S7_Conv2d_32x32x3x3_Relu6(
		((signed char * __restrict__) (network_L2_Memory+28404)), /* In */
		((signed char * __restrict__) (network_L2_Memory+148404)), /* Filter */
		((signed int * __restrict__) (network_L2_Memory+166836)), /* Bias */
		((signed char * __restrict__) (network_L2_Memory+108404)), /* Out */
		((unsigned char * __restrict__) (network_L2_Memory+128404)), /* Scale */
		((signed char * __restrict__) (network_L2_Memory+128436)), /* ScaleN */
		((signed char * __restrict__) (network_L2_Memory+166964)) /* Infos */
	);
	AT_GraphPerf[2] = gap_cl_readhwtimer() - AT_GraphPerf[2];
	/* Moving S9_Constant_95, size 128 from HyperRam at 321152 to (size 128) L2 at 149428 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321152), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 149428), 128, 0, &UchanHR0);
	/* Moving S10_Infos, size 9 from HyperRam at 322764 to (size 9) L2 at 149620 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322764), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 149620), 9, 0, &UchanHR1);
	/* Moving S10_Mul_scale, size 32 from HyperRam at 322624 to (size 32) L2 at 149556 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322624), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 149556), 32, 0, &UchanHR2);
	/* Moving S10_Mul_shift, size 32 from HyperRam at 322656 to (size 32) L2 at 149588 using event 4 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322656), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 149588), 32, 0, &UchanHR4);
	/* Moving S11_Conv_6_weights, size 1024 from HyperRam at 317696 to (size 1024) L2 at 148404 using event 5 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 317696), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 148404), 1024, 0, &UchanHR5);
	/* Waiting completion of transfer of S8_Conv_4_weights using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR3);
	/* Waiting completion of transfer of S9_Constant_95 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR0);
	/* Waiting completion of transfer of S10_Infos using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1);
	/* Waiting completion of transfer of S10_Mul_scale using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2);
	/* Waiting completion of transfer of S10_Mul_shift using event 4 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR4);
	AT_GraphPerf[3] = gap_cl_readhwtimer();
	S10_Conv2d_32x32x3x3_Relu6(
		((signed char * __restrict__) (network_L2_Memory+108404)), /* In */
		((signed char * __restrict__) (network_L2_Memory+157620)), /* Filter */
		((signed int * __restrict__) (network_L2_Memory+149428)), /* Bias */
		((signed char * __restrict__) (network_L2_Memory+128404)), /* Out */
		((unsigned char * __restrict__) (network_L2_Memory+149556)), /* Scale */
		((signed char * __restrict__) (network_L2_Memory+149588)), /* ScaleN */
		((signed char * __restrict__) (network_L2_Memory+149620)) /* Infos */
	);
	AT_GraphPerf[3] = gap_cl_readhwtimer() - AT_GraphPerf[3];
	/* Moving S12_Constant_resblock1.bypass.bias, size 128 from HyperRam at 321280 to (size 128) L2 at 149428 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321280), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 149428), 128, 0, &UchanHR0);
	/* Moving S13_Infos, size 9 from HyperRam at 322776 to (size 9) L2 at 149620 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322776), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 149620), 9, 0, &UchanHR1);
	/* Moving S13_Mul_scale, size 32 from HyperRam at 322688 to (size 32) L2 at 149556 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322688), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 149556), 32, 0, &UchanHR2);
	/* Moving S13_Mul_shift, size 32 from HyperRam at 322720 to (size 32) L2 at 149588 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322720), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 149588), 32, 0, &UchanHR3);
	/* Waiting completion of transfer of S11_Conv_6_weights using event 5 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR5);
	/* Waiting completion of transfer of S12_Constant_resblock1.bypass.bias using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR0);
	/* Waiting completion of transfer of S13_Infos using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1);
	/* Waiting completion of transfer of S13_Mul_scale using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2);
	/* Waiting completion of transfer of S13_Mul_shift using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR3);
	AT_GraphPerf[4] = gap_cl_readhwtimer();
	S13_Conv2d_32x32x1x1_Relu6(
		((signed char * __restrict__) (network_L2_Memory+28404)), /* In2 */
		((signed char * __restrict__) (network_L2_Memory+148404)), /* In1 */
		((signed int * __restrict__) (network_L2_Memory+149428)), /* Bias */
		((signed char * __restrict__) (network_L2_Memory+108404)), /* Out */
		((unsigned char * __restrict__) (network_L2_Memory+149556)), /* Scale */
		((signed char * __restrict__) (network_L2_Memory+149588)), /* ScaleN */
		((signed char * __restrict__) (network_L2_Memory+149620)) /* Infos */
	);
	AT_GraphPerf[4] = gap_cl_readhwtimer() - AT_GraphPerf[4];
	/* Moving S14_Infos, size 9 from HyperRam at 322788 to (size 9) L2 at 48404 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322788), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 48404), 9, 0, &UchanHR0);
	/* Moving S17_Infos, size 9 from HyperRam at 322800 to (size 9) L2 at 59604 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322800), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 59604), 9, 0, &UchanHR1);
	/* Moving S17_Mul_scale, size 64 from HyperRam at 322176 to (size 64) L2 at 59476 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322176), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 59476), 64, 0, &UchanHR2);
	/* Moving S17_Mul_shift, size 64 from HyperRam at 322240 to (size 64) L2 at 59540 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322240), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 59540), 64, 0, &UchanHR3);
	/* Moving S18_Conv_11_weights, size 36864 from HyperRam at 221184 to (size 36864) L2 at 70036 using event 4 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 221184), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 70036), 36864, 0, &UchanHR4);
	/* Waiting completion of transfer of S14_Infos using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR0);
	AT_GraphPerf[5] = gap_cl_readhwtimer();
	S14_MatAdd_32x25x25(
		((signed char * __restrict__) (network_L2_Memory+128404)), /* In1 */
		((signed char * __restrict__) (network_L2_Memory+108404)), /* In2 */
		((signed char * __restrict__) (network_L2_Memory+28404)), /* Out */
		((signed char * __restrict__) (network_L2_Memory+48404)) /* Infos */
	);
	AT_GraphPerf[5] = gap_cl_readhwtimer() - AT_GraphPerf[5];
	/* Moving S19_Constant_101, size 256 from HyperRam at 320512 to (size 256) L2 at 108948 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 320512), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 108948), 256, 0, &UchanHR0);
	/* Moving S20_Infos, size 9 from HyperRam at 322812 to (size 9) L2 at 109716 using event 5 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322812), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 109716), 9, 0, &UchanHR5);
	/* Moving S20_Mul_scale, size 64 from HyperRam at 322304 to (size 64) L2 at 109460 using event 6 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322304), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 109460), 64, 0, &UchanHR6);
	/* Moving S20_Mul_shift, size 64 from HyperRam at 322368 to (size 64) L2 at 109524 using event 7 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322368), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 109524), 64, 0, &UchanHR7);
	/* Moving S21_Conv_13_weights, size 2048 from HyperRam at 315648 to (size 2048) L2 at 106900 using event 8 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 315648), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 106900), 2048, 0, &UchanHR8);
	/* Moving S22_Constant_resblock2.bypass.bias, size 256 from HyperRam at 320768 to (size 256) L2 at 109204 using event 9 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 320768), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 109204), 256, 0, &UchanHR9);
	/* Moving S23_Infos, size 9 from HyperRam at 322824 to (size 9) L2 at 109728 using event 10 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322824), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 109728), 9, 0, &UchanHR10);
	/* Moving S23_Mul_scale, size 64 from HyperRam at 322432 to (size 64) L2 at 109588 using event 11 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322432), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 109588), 64, 0, &UchanHR11);
	/* Moving S23_Mul_shift, size 64 from HyperRam at 322496 to (size 64) L2 at 109652 using event 12 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322496), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 109652), 64, 0, &UchanHR12);
	/* Moving S24_Infos, size 9 from HyperRam at 322836 to (size 9) L2 at 143764 using event 13 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322836), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 143764), 9, 0, &UchanHR13);
	/* Moving S29_Constant_107, size 512 from HyperRam at 319232 to (size 512) L2 at 199220 using event 14 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 319232), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 199220), 512, 0, &UchanHR14);
	/* Moving S30_Infos, size 9 from HyperRam at 322860 to (size 9) L2 at 199991 using event 15 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322860), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 199991), 9, 0, &UchanHR15);
	/* Moving S30_Mul_scale, size 128 from HyperRam at 321664 to (size 128) L2 at 199732 using event 16 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321664), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 199732), 128, 0, &UchanHR16);
	/* Moving S30_Mul_shift, size 128 from HyperRam at 321792 to (size 128) L2 at 199860 using event 17 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321792), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 199860), 128, 0, &UchanHR17);
	/* Waiting completion of transfer of S17_Infos using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1);
	/* Waiting completion of transfer of S17_Mul_scale using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2);
	/* Waiting completion of transfer of S17_Mul_shift using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR3);
	AT_GraphPerf[6] = gap_cl_readhwtimer();
	S17_Conv2d_64x32x3x3_Relu6(
		((signed char * __restrict__) (network_L2_Memory+28404)), /* In */
		((signed char * __restrict__) (network_L2_Memory+1004)), /* Filter */
		((signed int * __restrict__) (network_L2_Memory+28140)), /* Bias */
		((signed char * __restrict__) (network_L2_Memory+48404)), /* Out */
		((unsigned char * __restrict__) (network_L2_Memory+59476)), /* Scale */
		((signed char * __restrict__) (network_L2_Memory+59540)), /* ScaleN */
		((signed char * __restrict__) (network_L2_Memory+59604)) /* Infos */
	);
	AT_GraphPerf[6] = gap_cl_readhwtimer() - AT_GraphPerf[6];
	/* Waiting completion of transfer of S18_Conv_11_weights using event 4 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR4);
	/* Waiting completion of transfer of S19_Constant_101 using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR0);
	/* Waiting completion of transfer of S20_Infos using event 5 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR5);
	/* Waiting completion of transfer of S20_Mul_scale using event 6 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR6);
	/* Waiting completion of transfer of S20_Mul_shift using event 7 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR7);
	AT_GraphPerf[7] = gap_cl_readhwtimer();
	S20_Conv2d_64x64x3x3_Relu6(
		((signed char * __restrict__) (network_L2_Memory+48404)), /* In */
		((signed char * __restrict__) (network_L2_Memory+70036)), /* Filter */
		((signed int * __restrict__) (network_L2_Memory+108948)), /* Bias */
		((signed char * __restrict__) (network_L2_Memory+59220)), /* Out */
		((unsigned char * __restrict__) (network_L2_Memory+109460)), /* Scale */
		((signed char * __restrict__) (network_L2_Memory+109524)), /* ScaleN */
		((signed char * __restrict__) (network_L2_Memory+109716)) /* Infos */
	);
	AT_GraphPerf[7] = gap_cl_readhwtimer() - AT_GraphPerf[7];
	/* Waiting completion of transfer of S21_Conv_13_weights using event 8 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR8);
	/* Waiting completion of transfer of S22_Constant_resblock2.bypass.bias using event 9 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR9);
	/* Waiting completion of transfer of S23_Infos using event 10 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR10);
	/* Waiting completion of transfer of S23_Mul_scale using event 11 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR11);
	/* Waiting completion of transfer of S23_Mul_shift using event 12 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR12);
	AT_GraphPerf[8] = gap_cl_readhwtimer();
	S23_Conv2d_64x32x1x1_Relu6(
		((signed char * __restrict__) (network_L2_Memory+28404)), /* In2 */
		((signed char * __restrict__) (network_L2_Memory+106900)), /* In1 */
		((signed int * __restrict__) (network_L2_Memory+109204)), /* Bias */
		((signed char * __restrict__) (network_L2_Memory+48404)), /* Out */
		((unsigned char * __restrict__) (network_L2_Memory+109588)), /* Scale */
		((signed char * __restrict__) (network_L2_Memory+109652)), /* ScaleN */
		((signed char * __restrict__) (network_L2_Memory+109728)) /* Infos */
	);
	AT_GraphPerf[8] = gap_cl_readhwtimer() - AT_GraphPerf[8];
	/* Moving S25_Conv_16_weights, size 73728 from HyperRam at 147456 to (size 73728) L2 at 70036 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 147456), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 70036), 73728, 0, &UchanHR0);
	/* Moving S27_Infos, size 9 from HyperRam at 322848 to (size 9) L2 at 46260 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322848), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 46260), 9, 0, &UchanHR1);
	/* Moving S27_Mul_scale, size 128 from HyperRam at 321408 to (size 128) L2 at 46004 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321408), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 46004), 128, 0, &UchanHR2);
	/* Moving S27_Mul_shift, size 128 from HyperRam at 321536 to (size 128) L2 at 46132 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321536), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 46132), 128, 0, &UchanHR3);
	/* Waiting completion of transfer of S24_Infos using event 13 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR13);
	AT_GraphPerf[9] = gap_cl_readhwtimer();
	S24_MatAdd_64x13x13(
		((signed char * __restrict__) (network_L2_Memory+59220)), /* In1 */
		((signed char * __restrict__) (network_L2_Memory+48404)), /* In2 */
		((signed char * __restrict__) (network_L2_Memory+28404)), /* Out */
		((signed char * __restrict__) (network_L2_Memory+143764)) /* Infos */
	);
	AT_GraphPerf[9] = gap_cl_readhwtimer() - AT_GraphPerf[9];
	/* Waiting completion of transfer of S25_Conv_16_weights using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR0);
	/* Waiting completion of transfer of S27_Infos using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1);
	/* Waiting completion of transfer of S27_Mul_scale using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2);
	/* Waiting completion of transfer of S27_Mul_shift using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR3);
	AT_GraphPerf[10] = gap_cl_readhwtimer();
	S27_Conv2d_128x64x3x3_Relu6(
		((signed char * __restrict__) (network_L2_Memory+28404)), /* In */
		((signed char * __restrict__) (network_L2_Memory+70036)), /* Filter */
		((signed int * __restrict__) (network_L2_Memory+27628)), /* Bias */
		((signed char * __restrict__) (network_L2_Memory+39220)), /* Out */
		((unsigned char * __restrict__) (network_L2_Memory+46004)), /* Scale */
		((signed char * __restrict__) (network_L2_Memory+46132)), /* ScaleN */
		((signed char * __restrict__) (network_L2_Memory+46260)) /* Infos */
	);
	AT_GraphPerf[10] = gap_cl_readhwtimer() - AT_GraphPerf[10];
	/* Moving S28_Conv_18_weights, size 147456 from HyperRam at 0 to (size 147456) L2 at 51764 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 0), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 51764), 147456, 0, &UchanHR0);
	/* Waiting completion of transfer of S28_Conv_18_weights using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR0);
	/* Waiting completion of transfer of S29_Constant_107 using event 14 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR14);
	/* Waiting completion of transfer of S30_Infos using event 15 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR15);
	/* Waiting completion of transfer of S30_Mul_scale using event 16 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR16);
	/* Waiting completion of transfer of S30_Mul_shift using event 17 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR17);
	AT_GraphPerf[11] = gap_cl_readhwtimer();
	S30_Conv2d_128x128x3x3_Relu6(
		((signed char * __restrict__) (network_L2_Memory+39220)), /* In */
		((signed char * __restrict__) (network_L2_Memory+51764)), /* Filter */
		((signed int * __restrict__) (network_L2_Memory+199220)), /* Bias */
		((signed char * __restrict__) (network_L2_Memory+45492)), /* Out */
		((unsigned char * __restrict__) (network_L2_Memory+199732)), /* Scale */
		((signed char * __restrict__) (network_L2_Memory+199860)), /* ScaleN */
		((signed char * __restrict__) (network_L2_Memory+199991)) /* Infos */
	);
	AT_GraphPerf[11] = gap_cl_readhwtimer() - AT_GraphPerf[11];
	/* Moving S32_Constant_resblock3.bypass.bias, size 512 from HyperRam at 319744 to (size 512) L2 at 59956 using event 0 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 319744), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 59956), 512, 0, &UchanHR0);
	/* Moving S33_Infos, size 9 from HyperRam at 322872 to (size 9) L2 at 60724 using event 1 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322872), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 60724), 9, 0, &UchanHR1);
	/* Moving S33_Mul_scale, size 128 from HyperRam at 321920 to (size 128) L2 at 60468 using event 2 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 321920), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 60468), 128, 0, &UchanHR2);
	/* Moving S33_Mul_shift, size 128 from HyperRam at 322048 to (size 128) L2 at 60596 using event 3 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322048), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 60596), 128, 0, &UchanHR3);
	/* Moving S34_Infos, size 9 from HyperRam at 322884 to (size 9) L2 at 60736 using event 4 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322884), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 60736), 9, 0, &UchanHR4);
	/* Moving S38_Infos, size 9 from HyperRam at 322896 to (size 9) L2 at 51764 using event 5 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322896), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 51764), 9, 0, &UchanHR5);
	/* Moving S38_Mul_scale, size 2 from HyperRam at 322928 to (size 2) L2 at 51784 using event 6 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322928), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 51784), 2, 0, &UchanHR6);
	/* Moving S38_Mul_shift, size 2 from HyperRam at 322932 to (size 2) L2 at 51788 using event 7 */
	AT_HYPERRAM_CL_COPY(&HyperRam, ((AT_HYPERRAM_EXT_ADDR_TYPE) network_L3_Memory + 322932), ((AT_HYPERRAM_INT_ADDR_TYPE) network_L2_Memory + 51788), 2, 0, &UchanHR7);
	/* Waiting completion of transfer of S32_Constant_resblock3.bypass.bias using event 0 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR0);
	/* Waiting completion of transfer of S33_Infos using event 1 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR1);
	/* Waiting completion of transfer of S33_Mul_scale using event 2 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR2);
	/* Waiting completion of transfer of S33_Mul_shift using event 3 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR3);
	AT_GraphPerf[12] = gap_cl_readhwtimer();
	S33_Conv2d_128x64x1x1_Relu6(
		((signed char * __restrict__) (network_L2_Memory+28404)), /* In2 */
		((signed char * __restrict__) (network_L2_Memory+19436)), /* In1 */
		((signed int * __restrict__) (network_L2_Memory+59956)), /* Bias */
		((signed char * __restrict__) (network_L2_Memory+39220)), /* Out */
		((unsigned char * __restrict__) (network_L2_Memory+60468)), /* Scale */
		((signed char * __restrict__) (network_L2_Memory+60596)), /* ScaleN */
		((signed char * __restrict__) (network_L2_Memory+60724)) /* Infos */
	);
	AT_GraphPerf[12] = gap_cl_readhwtimer() - AT_GraphPerf[12];
	/* Waiting completion of transfer of S34_Infos using event 4 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR4);
	AT_GraphPerf[13] = gap_cl_readhwtimer();
	S34_MatAdd_128x7x7_Relu6(
		((signed char * __restrict__) (network_L2_Memory+45492)), /* In1 */
		((signed char * __restrict__) (network_L2_Memory+39220)), /* In2 */
		((signed char * __restrict__) (network_L2_Memory+28408)), /* Out */
		((signed char * __restrict__) (network_L2_Memory+60736)) /* Infos */
	);
	AT_GraphPerf[13] = gap_cl_readhwtimer() - AT_GraphPerf[13];
	/* Waiting completion of transfer of S38_Infos using event 5 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR5);
	/* Waiting completion of transfer of S38_Mul_scale using event 6 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR6);
	/* Waiting completion of transfer of S38_Mul_shift using event 7 */
	AT_HYPERRAM_CL_WAIT(&HyperRam, &UchanHR7);
	AT_GraphPerf[14] = gap_cl_readhwtimer();
	S38_Linear_2x6272(
		((signed char * __restrict__) (network_L2_Memory+28408)), /* In */
		((signed char * __restrict__) (network_L3_Memory+276480)), /* Filter */
		((signed int * __restrict__) (network_L2_Memory+28396)), /* Bias */
		((signed char * __restrict__) (network_L2_Memory+28404)), /* Out */
		((unsigned char * __restrict__) (network_L2_Memory+51784)), /* Scale */
		((signed char * __restrict__) (network_L2_Memory+51788)), /* ScaleN */
		((signed char * __restrict__) (network_L2_Memory+51764)) /* Infos */
	);
	AT_GraphPerf[14] = gap_cl_readhwtimer() - AT_GraphPerf[14];
	AT_GraphPerf[15] = gap_cl_readhwtimer();
	S40_Op_Gemm_25_split_copy_0(
		((signed char * __restrict__) (network_L2_Memory+28404)), /* In */
		((signed char * __restrict__) Output_1) /* Out */
	);
	AT_GraphPerf[15] = gap_cl_readhwtimer() - AT_GraphPerf[15];
	AT_GraphPerf[16] = gap_cl_readhwtimer();
	S42_Act_Hsigmoid(
		((signed char * __restrict__) ((network_L2_Memory+28404) + 1)), /* In */
		((signed char * __restrict__) Output_2), /* Out */
		((signed char * __restrict__) (network_L3_Memory+322908)) /* Infos */
	);
	AT_GraphPerf[16] = gap_cl_readhwtimer() - AT_GraphPerf[16];
	return 0;
}
