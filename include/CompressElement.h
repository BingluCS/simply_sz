/**
 *  @file CompressElement.h
 *  @author Sheng Di
 *  @date April, 2016
 *  @brief Header file for Compress Elements such as DoubleCompressELement.
 *  (C) 2016 by Mathematics and Computer Science (MCS), Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include <stdint.h>

#ifndef _CompressElement_H
#define _CompressElement_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>

typedef struct DoubleValueCompressElement
{
	double data;
	int64_t curValue;
	unsigned char curBytes[8]; //big_endian
	int reqBytesLength;
	int resiBitsLength;
} DoubleValueCompressElement;

typedef struct FloatValueCompressElement
{
	float data;// Value after optimization
	int curValue; //Value before optimization
	unsigned char curBytes[4]; //big_endian
	int reqBytesLength; //valid length /8
	int resiBitsLength;	//valid length %8
} FloatValueCompressElement;

typedef struct LossyCompressionElement
{
	int leadingZeroBytes; //0,1,2,or 3, which means the number of same bytes
	unsigned char integerMidBytes[8];// the different bytes
	int integerMidBytes_Length; //they are mid_bits actually

	int resMidBitsLength; //the number of different bytes
	int residualMidBits; //the remain bits
} LossyCompressionElement;

void listAdd_float(float last3CmprsData[3], float value);



// void new_LossyCompressionElement(LossyCompressionElement *lce, int leadingNum, unsigned char* intMidBytes,
// 		int intMidBytes_Length, int resiMidBitsLength, int resiBits);
// void updateLossyCompElement_Double(unsigned char* curBytes, unsigned char* preBytes,
// 		int reqBytesLength, int resiBitsLength,  LossyCompressionElement *lce);
void updateLossyCompElement_Float(unsigned char* curBytes, unsigned char* preBytes,
		int reqBytesLength, int resiBitsLength,  LossyCompressionElement *lce);

#ifdef __cplusplus
}
#endif

#endif /* ----- #ifndef _CompressElement_H  ----- */
