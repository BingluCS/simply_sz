#include <stdio.h>
#include <stdlib.h>

#include "rw.h"
#include "sz.h"
#include "defines.h"
#include "sz_float.h"

sz_params *confparams_cpr = NULL; //used for compression
sz_params *confparams_dec = NULL; //used for decompression

sz_exedata *exe_params = NULL;

void Init_sz(){
    confparams_cpr = (sz_params*)malloc(sizeof(sz_params));
    exe_params = (sz_exedata*)malloc(sizeof(sz_exedata));
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
// int main(int argc, char* argv[]) {
//     char *inPath="../data/baryon_density.f32";
//     size_t nbEle;    
//     float *data = readFloatData(inPath, &nbEle);
//     return 0;
// }