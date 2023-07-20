#include <stdio.h>
#include <stdlib.h>

#include "rw.h"
#include "sz.h"
#include "defines.h"
#include "sz_float.h"
char* filename=NULL;
sz_params *confparams_cpr = (sz_params*)malloc(sizeof(sz_params)); //used for compression
sz_params *confparams_dec; //used for decompression

sz_exedata *exe_params = (sz_exedata*)malloc(sizeof(sz_exedata));

void Init_sz(size_t quant_intervals,size_t max_quant) {
    // confparams_cpr = (sz_params*)malloc(sizeof(sz_params));
    // exe_params = (sz_exedata*)malloc(sizeof(sz_exedata));

	confparams_cpr->dataType=SZ_FLOAT;
	// confparams_cpr->absErrBound=115.862236; //absoluted error bound
	// confparams_cpr->relBoundRatio=0.001;	//relative error bound based on value range
	//confparams_cpr->pw_relBoundRatio=0.1;	//point-wise relative error bound
										
	confparams_cpr->quantization_intervals=quant_intervals;//autamatically search for an optimized setting if it is 0

	if(confparams_cpr->quantization_intervals==0)
		exe_params->optQuantMode=1;
	else  {
		exe_params->intvCapacity = quant_intervals;
		exe_params->intvRadius = quant_intervals/2;
	}

	confparams_cpr->max_quant_intervals = max_quant;// maximum quantization interval is valid only when quantization_intervals=0
	confparams_cpr->maxRangeRadius=confparams_cpr->max_quant_intervals/2;

	confparams_cpr->sampleDistance=100;// sampleDistance determins the number of samples used to optimize the quantization intervals
										//For example, sampleDistance=50 means 1/50=2% of data points are sample points

	confparams_cpr->predThreshold=0.99;	//the threshold to determine the ratio of predictable data over all data
	confparams_cpr->withRegression=SZ_WITH_LINEAR_REGRESSION;
}

unsigned char* SZ_compress(int dataType, void *data, size_t *outSize, size_t r5, size_t r4, size_t r3, size_t r2, size_t r1)
{
    unsigned char *newByteData = SZ_compress_args(dataType, data, outSize, confparams_cpr->errorBoundMode, 
    confparams_cpr->absErrBound, confparams_cpr->relBoundRatio,confparams_cpr->pw_relBoundRatio, r5, r4, r3, r2, r1);
	return newByteData;
}


unsigned char* SZ_compress_args(int dataType, void *data, size_t *outSize, int errBoundMode, double absErrBound,
double relBoundRatio, double pwrBoundRatio, size_t r5, size_t r4, size_t r3, size_t r2, size_t r1)
{
    // if(dataType==SZ_FLOAT)
	// {
		unsigned char *newByteData = NULL;
		SZ_compress_args_float(-1, confparams_cpr->withRegression, &newByteData, (float *)data, r5, r4, r3, r2, r1,
		outSize, errBoundMode, absErrBound, relBoundRatio, pwrBoundRatio);
		return newByteData;
	// }
    //printf("---------\n");
    //return newByteData;
}