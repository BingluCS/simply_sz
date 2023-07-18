#include "CompressElement.h"
#include "dataCompression.h"

void updateLossyCompElement_Float(unsigned char* curBytes, unsigned char* preBytes, 
		int reqBytesLength, int resiBitsLength,  LossyCompressionElement *lce)
{
	int resiIndex, intMidBytes_Length = 0;
	int leadingNum = compIdenticalLeadingBytesCount_float(preBytes, curBytes); //calculate the number of same bytes
	int fromByteIndex = leadingNum;		// orginal code of SZ, (pointless)
	int toByteIndex = reqBytesLength; // orginal code of SZ, (pointless)
	if(fromByteIndex < toByteIndex)
	{
		intMidBytes_Length = reqBytesLength - leadingNum;// the number of different bytes
		memcpy(lce->integerMidBytes, &(curBytes[fromByteIndex]), intMidBytes_Length);// copy the different bytes
	}
	int resiBits = 0;
	if(resiBitsLength!=0)
	{
		resiIndex = reqBytesLength;
		if(resiIndex < 8)
			resiBits = (curBytes[resiIndex] & 0xFF) >> (8-resiBitsLength);
	}
	lce->leadingZeroBytes = leadingNum;
	lce->integerMidBytes_Length = intMidBytes_Length;
	lce->resMidBitsLength = resiBitsLength;
	lce->residualMidBits = resiBits;
}

void listAdd_float(float last3CmprsData[3], float value)
{//predicted buffer is shifted forward by one bit
	last3CmprsData[2] = last3CmprsData[1];
	last3CmprsData[1] = last3CmprsData[0];
	last3CmprsData[0] = value;
}
