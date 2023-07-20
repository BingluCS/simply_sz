#include "TightDataPointStorageF.h"
#include "defines.h"
#include "sz.h"
#include "TypeManager.h"
#include <stdlib.h>
void new_TightDataPointStorageF(TightDataPointStorageF **this_,
		size_t dataSeriesLength, size_t exactDataNum, 
		int* type, unsigned char* exactMidBytes, size_t exactMidBytes_size,
		unsigned char* leadNumIntArray,  //leadNumIntArray contains readable numbers....
		unsigned char* resiMidBits, size_t resiMidBits_size,
		unsigned char resiBitLength, 
		double realPrecision, float medianValue, char reqLength, unsigned int intervals, 
		unsigned char* pwrErrBoundBytes, size_t pwrErrBoundBytes_size, unsigned char radExpo) {
			
	*this_ = (TightDataPointStorageF *)malloc(sizeof(TightDataPointStorageF));
	(*this_)->allSameData = 0;
	(*this_)->realPrecision = realPrecision;
	(*this_)->medianValue = medianValue;
	(*this_)->reqLength = reqLength;

	(*this_)->dataSeriesLength = dataSeriesLength;
	(*this_)->exactDataNum = exactDataNum;

	(*this_)->rtypeArray = NULL;
	(*this_)->rtypeArray_size = 0;

	int stateNum = 2*intervals;
	// HuffmanTree* huffmanTree = createHuffmanTree(stateNum);
	// if(confparams_cpr->errorBoundMode == PW_REL && confparams_cpr->accelerate_pw_rel_compression)
	// 	(*this_)->max_bits = encode_withTree_MSST19(huffmanTree, type, dataSeriesLength, &(*this_)->typeArray, &(*this_)->typeArray_size);
	// else
	// 	encode_withTree(huffmanTree, type, dataSeriesLength, &(*this_)->typeArray, &(*this_)->typeArray_size);
	// SZ_ReleaseHuffman(huffmanTree);
		
	(*this_)->exactMidBytes = exactMidBytes;
	(*this_)->exactMidBytes_size = exactMidBytes_size;
	//futher compression the number of same bytes
	(*this_)->leadNumArray_size = convertIntArray2ByteArray_fast_2b(leadNumIntArray, exactDataNum, &((*this_)->leadNumArray));

	(*this_)->residualMidBits_size = convertIntArray2ByteArray_fast_dynamic(resiMidBits, resiBitLength, exactDataNum, &((*this_)->residualMidBits));
	
	(*this_)->intervals = intervals;
	
	(*this_)->isLossless = 0;
	
	if(confparams_cpr->errorBoundMode>=PW_REL)
		(*this_)->pwrErrBoundBytes = pwrErrBoundBytes;
	else
		(*this_)->pwrErrBoundBytes = NULL;
		
	(*this_)->radExpo = radExpo;
	
	(*this_)->pwrErrBoundBytes_size = pwrErrBoundBytes_size;
}