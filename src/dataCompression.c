#include "dataCompression.h"
#include "sz.h"
#include "ByteToolkit.h"

float computeRangeSize_float(float* oriData, size_t size, float* valueRangeSize, float* medianValue)
{
	size_t i = 0;
	float min = oriData[0];
	float max = min;
	for(i=1;i<size;i++)
	{
		float data = oriData[i];
		if(min>data)
			min = data;
		else if(max<data)
			max = data;
	}

	*valueRangeSize = max - min;
	*medianValue = min + *valueRangeSize/2;
	return min;
}
void compressSingleFloatValue(FloatValueCompressElement *vce, float tgtValue, float precision, float medianValue,
		int reqLength, int reqBytesLength, int resiBitsLength)
{
	float normValue = tgtValue - medianValue;//normalized value, data_i - median value

	lfloat lfBuf;
	lfBuf.value = normValue;

	int ignBytesLength = 32 - reqLength;
	if(ignBytesLength<0)
		ignBytesLength = 0;

	int tmp_int = lfBuf.ivalue;
	intToBytes_bigEndian(vce->curBytes, tmp_int);

	lfBuf.ivalue = (lfBuf.ivalue >> ignBytesLength) << ignBytesLength;

	//float tmpValue = lfBuf.value;

	vce->data = lfBuf.value+medianValue; //value after binary representation analysis
	vce->curValue = tmp_int; //value before binary representation analysis
	vce->reqBytesLength = reqBytesLength;
	vce->resiBitsLength = resiBitsLength;
}

void addExactData(DynamicByteArray *exactMidByteArray, DynamicIntArray *exactLeadNumArray,
		DynamicIntArray *resiBitArray, LossyCompressionElement *lce)
{
	int i;
	int leadByteLength = lce->leadingZeroBytes;// the number of same bytes as the former
	addDIA_Data(exactLeadNumArray, leadByteLength);
	unsigned char* intMidBytes = lce->integerMidBytes;//the different bytes
	int integerMidBytesLength = lce->integerMidBytes_Length;
	int resMidBitsLength = lce->resMidBitsLength;
	if(intMidBytes!=NULL||resMidBitsLength!=0)
	{
		if(intMidBytes!=NULL)
			for(i = 0;i<integerMidBytesLength;i++)
				addDBA_Data(exactMidByteArray, intMidBytes[i]);
		if(resMidBitsLength!=0)
			addDIA_Data(resiBitArray, lce->residualMidBits);
	}
}
int compIdenticalLeadingBytesCount_float(unsigned char* preBytes, unsigned char* curBytes)
{
	int i, n = 0;
	for(i=0;i<4;i++)
		if(preBytes[i]==curBytes[i])
			n++;
		else
			break;
	if(n>3) n = 3;
	return n;
}
