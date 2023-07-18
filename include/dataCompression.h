
#ifndef _DataCompression_H
#define _DataCompression_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include "CompressElement.h"
#include "DynamicByteArray.h"
#include "DynamicIntArray.h"

float computeRangeSize_float(float* oriData, size_t size, float* valueRangeSize, float* medianValue);
void compressSingleFloatValue(FloatValueCompressElement *vce, float tgtValue, float precision, float medianValue,
		int reqLength, int reqBytesLength, int resiBitsLength);
void addExactData(DynamicByteArray *exactMidByteArray, DynamicIntArray *exactLeadNumArray,
		DynamicIntArray *resiBitArray, LossyCompressionElement *lce);

int compIdenticalLeadingBytesCount_float(unsigned char* preBytes, unsigned char* curBytes);

#ifdef __cplusplus
}
#endif

#endif