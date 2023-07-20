#ifndef _SZ_H
#define _SZ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

#define SZ_FLOAT 0
#define SZ_DOUBLE 1

#define SZ_COMPUTE_1D_NUMBER_OF_BLOCKS( COUNT, NUM_BLOCKS, BLOCK_SIZE ) \
    if (COUNT <= BLOCK_SIZE){                  \
        NUM_BLOCKS = 1;             \
    }                                   \
    else{                               \
        NUM_BLOCKS = COUNT / BLOCK_SIZE;       \
    }                                   \

#define SZ_COMPUTE_2D_NUMBER_OF_BLOCKS( COUNT, NUM_BLOCKS, BLOCK_SIZE ) \
    if (COUNT <= BLOCK_SIZE){                   \
        NUM_BLOCKS = 1;             \
    }                                   \
    else{                               \
        NUM_BLOCKS = COUNT / BLOCK_SIZE;        \
    }                                   \

#define SZ_COMPUTE_3D_NUMBER_OF_BLOCKS( COUNT, NUM_BLOCKS, BLOCK_SIZE ) \
    if (COUNT <= BLOCK_SIZE){                   \
        NUM_BLOCKS = 1;             \
    }                                   \
    else{                               \
        NUM_BLOCKS = COUNT / BLOCK_SIZE;        \
    }                                   \

#define SZ_COMPUTE_BLOCKCOUNT( COUNT, NUM_BLOCKS, SPLIT_INDEX,       \
                                       EARLY_BLOCK_COUNT, LATE_BLOCK_COUNT ) \
    EARLY_BLOCK_COUNT = LATE_BLOCK_COUNT = COUNT / NUM_BLOCKS;               \
    SPLIT_INDEX = COUNT % NUM_BLOCKS;                                        \
    if (0 != SPLIT_INDEX) {                                                  \
        EARLY_BLOCK_COUNT = EARLY_BLOCK_COUNT + 1;                           \
    }                                                                        \

typedef union ldouble
{
    double value;
    uint64_t lvalue;
    unsigned char byte[8];
} ldouble;

typedef union lfloat
{
    float value;
    unsigned int ivalue;
    unsigned char byte[4];
} lfloat;

typedef struct sz_params
{
	int dataType;
	unsigned int max_quant_intervals; //max number of quantization intervals for quantization
	unsigned int quantization_intervals;
	unsigned int maxRangeRadius;
	int sol_ID;// it's SZ or SZ_Transpose, unless the setting is PASTRI compression mode (./configure --enable-pastri)
	int losslessCompressor;
	int sampleDistance; //2 bytes
	float predThreshold;  // 2 bytes
	int szMode; //* 0 (best speed) or 1 (better compression with Zstd/Gzip) or 3 temporal-dimension based compression
	int gzipMode; //* four options: Z_NO_COMPRESSION, or Z_BEST_SPEED, Z_BEST_COMPRESSION, Z_DEFAULT_COMPRESSION
	int  errorBoundMode; //4bits (0.5byte), //ABS, REL, ABS_AND_REL, or ABS_OR_REL, PSNR, or PW_REL, PSNR
	double absErrBound; //absolute error bound
	double relBoundRatio; //value range based relative error bound ratio
	double psnr; //PSNR
	double normErr;
	double pw_relBoundRatio; //point-wise relative error bound
	int segment_size; //only used for 2D/3D data compression with pw_relBoundRatio (deprecated)
	int pwr_type; //only used for 2D/3D data compression with pw_relBoundRatio

	int protectValueRange; //0 or 1
	float fmin, fmax;
	double dmin, dmax;

	int snapshotCmprStep; //perform single-snapshot-based compression if time_step == snapshotCmprStep
	int predictionMode;

	int accelerate_pw_rel_compression;
	int plus_bits;

	int randomAccess;
	int withRegression;

} sz_params;

typedef struct sz_exedata
{
	char optQuantMode;	//opt Quantization (0: fixed ; 1: optimized)
	int intvCapacity; // the number of intervals for the linear-scaling quantization
	int intvRadius;  // the number of intervals for the radius of the quantization range (intvRadius=intvCapacity/2)
	unsigned int SZ_SIZE_TYPE; //the length (# bytes) of the size_t in the system at runtime //4 or 8: sizeof(size_t)
} sz_exedata;

void Init_sz(size_t quant_intervals,size_t max_quant);
unsigned char* SZ_compress(int dataType, void *data, size_t *outSize, size_t r5, size_t r4, size_t r3, size_t r2, size_t r1);

unsigned char* SZ_compress_args(int dataType, void *data, size_t *outSize, int errBoundMode, double absErrBound,
double relBoundRatio, double pwrBoundRatio, size_t r5, size_t r4, size_t r3, size_t r2, size_t r1);

extern sz_params *confparams_cpr;
extern sz_exedata *exe_params;
extern char* filename;

#ifdef __cplusplus
}
#endif

#endif