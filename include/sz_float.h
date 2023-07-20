
#include <stdio.h>
#include "TightDataPointStorageF.h"

void computeReqLength_float(double realPrecision, short radExpo, int* reqLength, float* medianValue);
unsigned int optimize_intervals_float_1D_opt(float *oriData, size_t dataLength, double realPrecision);
int SZ_compress_args_float(int cmprType, int withRegression, unsigned char** newByteData, float *oriData, 
size_t r5, size_t r4, size_t r3, size_t r2, size_t r1, size_t *outSize, 
int errBoundMode, double absErr_Bound, double relBoundRatio, double pwRelBoundRatio);


char SZ_compress_args_float_NoCkRngeNoGzip_1D(int cmprType, unsigned char** newByteData, float *oriData, 
size_t dataLength, double realPrecision, size_t *outSize, float valueRangeSize, float medianValue_f);

TightDataPointStorageF* SZ_compress_float_1D_MDQ(float *oriData, 
size_t dataLength, double realPrecision, float valueRangeSize, float medianValue_f);
extern TightDataPointStorageF* tdps;
// unsigned char * SZ_compress_float_3D_MDQ_nonblocked_with_blocked_regression(float *oriData,size_t r1,
//  size_t r2, size_t r3, float realPrecision, size_t * comp_size);

// unsigned int optimize_intervals_float_1D_opt(float *oriData, size_t dataLength, double realPrecision);

// char SZ_compress_args_float_NoCkRngeNoGzip_1D(int cmprType, unsigned char** newByteData, float *oriData,
// size_t dataLength, double realPrecision, size_t *outSize, float valueRangeSize, float medianValue_f);
