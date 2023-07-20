#include "sz_float.h"
#include "sz.h"
#include "rw.h"
#include "TightDataPointStorageF.h"
#include "ByteToolkit.h"
#include "dataCompression.h"
#include "DynamicIntArray.h"
#include "DynamicByteArray.h"
#include "defines.h"
#include "CompressElement.h"
#include "conf.h"
#include <string.h>
#include <math.h>

void computeReqLength_float(double realPrecision, short radExpo, int* reqLength, float* medianValue)
{
	short reqExpo = getPrecisionReqLength_double(realPrecision);
	*reqLength = 9+radExpo - reqExpo+1; //radExpo-reqExpo == reqMantiLength, 9 is the sign and exponent bit
	if(*reqLength<9)
		*reqLength = 9;
	if(*reqLength>32)
	{
		*reqLength = 32;
		*medianValue = 0;
	}
}

unsigned int optimize_intervals_float_1D_opt(float *oriData, size_t dataLength, double realPrecision)
{
	size_t i = 0, radiusIndex;
	float pred_value = 0, pred_err;
	size_t *intervals = (size_t*)malloc(confparams_cpr->maxRangeRadius*sizeof(size_t));
	memset(intervals, 0, confparams_cpr->maxRangeRadius*sizeof(size_t));
	size_t totalSampleSize = 0;//dataLength/confparams_cpr->sampleDistance;

	float * data_pos = oriData + 2;
	while(data_pos - oriData < dataLength){
		totalSampleSize++;
		pred_value = data_pos[-1];
		pred_err = fabs(pred_value - *data_pos);
		radiusIndex = (uint64_t)((pred_err/realPrecision+1)/2);
		if(radiusIndex>=confparams_cpr->maxRangeRadius)
			radiusIndex = confparams_cpr->maxRangeRadius - 1;
		intervals[radiusIndex]++;

		data_pos += confparams_cpr->sampleDistance;
	}
	//compute the appropriate number
	size_t targetCount = totalSampleSize*confparams_cpr->predThreshold;
	size_t sum = 0;
	for(i=0;i<confparams_cpr->maxRangeRadius;i++)
	{
		sum += intervals[i];
		if(sum>targetCount)
			break;
	}
	if(i>=confparams_cpr->maxRangeRadius)
		i = confparams_cpr->maxRangeRadius-1;

	unsigned int accIntervals = 2*(i+1);
	unsigned int powerOf2 = roundUpToPowerOf2(accIntervals);

	if(powerOf2<32)
		powerOf2 = 32;

	free(intervals);
	return powerOf2;
}

int SZ_compress_args_float(int cmprType, int withRegression, unsigned char** newByteData, float *oriData,
size_t r5, size_t r4, size_t r3, size_t r2, size_t r1, size_t *outSize,
int errBoundMode, double absErr_Bound, double relBoundRatio, double pwRelBoundRatio) {
	// confparams_cpr->dataType = SZ_FLOAT;
	// confparams_cpr->errorBoundMode = errBoundMode

	size_t dataLength = r1;
	float min = 0, max, valueRangeSize = 0, medianValue = 0;
	min = computeRangeSize_float(oriData, dataLength, &valueRangeSize, &medianValue);
	max = min+valueRangeSize;

	confparams_cpr->fmin = min;
	confparams_cpr->fmax = max;

	size_t tmpOutSize = 0;
	unsigned char* tmpByteData;
	double realPrecision = confparams_cpr->absErrBound;

	SZ_compress_args_float_NoCkRngeNoGzip_1D(cmprType, &tmpByteData, oriData, r1, realPrecision, &tmpOutSize, valueRangeSize, medianValue);
	//printf("%f %f\n",min,max);
	// if(confparams_cpr->errorBoundMode==ABS) {

	// }
	return 0;
}

char SZ_compress_args_float_NoCkRngeNoGzip_1D(int cmprType, unsigned char** newByteData, float *oriData, 
size_t dataLength, double realPrecision, size_t *outSize, float valueRangeSize, float medianValue_f)
{
	char compressionType = 0;
	TightDataPointStorageF* tdps = NULL;

	tdps = SZ_compress_float_1D_MDQ(oriData, dataLength, realPrecision, valueRangeSize, medianValue_f);

	//convertTDPStoFlatBytes_float(tdps, newByteData, outSize);

	// if(*outSize>3 + MetaDataByteLength + exe_params->SZ_SIZE_TYPE + 1 + sizeof(float)*dataLength)
	// 	SZ_compress_args_float_StoreOriData(oriData, dataLength, newByteData, outSize);

	// free_TightDataPointStorageF(tdps);
	return compressionType;
}
TightDataPointStorageF* SZ_compress_float_1D_MDQ(float *oriData,
size_t dataLength, double realPrecision, float valueRangeSize, float medianValue_f)
{
	unsigned int quantization_intervals;

	if(exe_params->optQuantMode==1)
		quantization_intervals = optimize_intervals_float_1D_opt(oriData, dataLength, realPrecision);
	else
		quantization_intervals = exe_params->intvCapacity;

	int intvRadius = quantization_intervals/2;
	size_t i;
	int reqLength;
	float medianValue = medianValue_f;
	short radExpo = getExponent_float(valueRangeSize/2);
	computeReqLength_float(realPrecision, radExpo, &reqLength, &medianValue);
	int* type = (int*) malloc(dataLength*sizeof(int));
	float* spaceFillingValue = oriData; //

	DynamicIntArray *exactLeadNumArray;// the numbers of same bytes of each unpredictable value as the former unpredictable value 
	new_DIA(&exactLeadNumArray, DynArrayInitLen);

	DynamicByteArray *exactMidByteArray;// the different bytes
	new_DBA(&exactMidByteArray, DynArrayInitLen);

	DynamicIntArray *resiBitArray;// the remaining bits
	new_DIA(&resiBitArray, DynArrayInitLen);

	unsigned char preDataBytes[4];
	intToBytes_bigEndian(preDataBytes, 0);

	int reqBytesLength = reqLength/8;
	int resiBitsLength = reqLength%8;
	float last3CmprsData[3] = {0};

	FloatValueCompressElement *vce = (FloatValueCompressElement*)malloc(sizeof(FloatValueCompressElement));
	LossyCompressionElement *lce = (LossyCompressionElement*)malloc(sizeof(LossyCompressionElement));

	//add the first data, the first two data are unpredictable
	type[0] = 0;
	compressSingleFloatValue(vce, spaceFillingValue[0], realPrecision, medianValue, reqLength, reqBytesLength, resiBitsLength);
	updateLossyCompElement_Float(vce->curBytes, preDataBytes, reqBytesLength, resiBitsLength, lce);
	memcpy(preDataBytes,vce->curBytes,4);
	addExactData(exactMidByteArray, exactLeadNumArray, resiBitArray, lce);
	listAdd_float(last3CmprsData, vce->data);

		//add the second data
	type[1] = 0;
	compressSingleFloatValue(vce, spaceFillingValue[1], realPrecision, medianValue, reqLength, reqBytesLength, resiBitsLength);
	updateLossyCompElement_Float(vce->curBytes, preDataBytes, reqBytesLength, resiBitsLength, lce);
	memcpy(preDataBytes,vce->curBytes,4);
	addExactData(exactMidByteArray, exactLeadNumArray, resiBitArray, lce);
	listAdd_float(last3CmprsData, vce->data);


	int state;
	float checkRadius;
	float curData;
	float pred = last3CmprsData[0];
	float predAbsErr;
	checkRadius = (quantization_intervals-1)*realPrecision;
	float interval = 2*realPrecision;

	float recip_precision = 1/realPrecision;

	for(i=2;i<dataLength;i++)
	{
		curData = spaceFillingValue[i];
		//pred = 2*last3CmprsData[0] - last3CmprsData[1];
		//pred = last3CmprsData[0];
		predAbsErr = fabsf(curData - pred);
		if(predAbsErr<checkRadius)
		{
			state = ((int)(predAbsErr*recip_precision+1))>>1;
			if(curData>=pred)
			{
				type[i] = intvRadius+state;
				pred = pred + state*interval;
			}
			else //curData<pred
			{
				type[i] = intvRadius-state;
				pred = pred - state*interval;
			}

			//double-check the prediction error in case of machine-epsilon impact
			if(fabs(curData-pred)>realPrecision)
			{
				type[i] = 0;
				compressSingleFloatValue(vce, curData, realPrecision, medianValue, reqLength, reqBytesLength, resiBitsLength);
				updateLossyCompElement_Float(vce->curBytes, preDataBytes, reqBytesLength, resiBitsLength, lce);
				memcpy(preDataBytes,vce->curBytes,4);
				addExactData(exactMidByteArray, exactLeadNumArray, resiBitArray, lce);

				//listAdd_float(last3CmprsData, vce->data);
				pred = vce->data;

			}
			continue;
		}

		//unpredictable data processing
		type[i] = 0;
		compressSingleFloatValue(vce, curData, realPrecision, medianValue, reqLength, reqBytesLength, resiBitsLength);
		updateLossyCompElement_Float(vce->curBytes, preDataBytes, reqBytesLength, resiBitsLength, lce);
		memcpy(preDataBytes,vce->curBytes,4);
		addExactData(exactMidByteArray, exactLeadNumArray, resiBitArray, lce);
		pred = vce->data;

	}//end of for

	size_t exactDataNum = exactLeadNumArray->size;
	TightDataPointStorageF* tdps;

	new_TightDataPointStorageF(&tdps, dataLength, exactDataNum,
		type, exactMidByteArray->array, exactMidByteArray->size,
		exactLeadNumArray->array,
		resiBitArray->array, resiBitArray->size,
		resiBitsLength,
		realPrecision, medianValue, (char)reqLength, quantization_intervals, NULL, 0, 0);
	for(i=0;i<20;i++)
		printf("%d %d\n",i,type[i]);
	free_DIA(exactLeadNumArray);
	free_DIA(resiBitArray);
	free(type);
	free(vce);
	free(lce);
	free(exactMidByteArray);
	return tdps;
}