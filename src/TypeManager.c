#include "TypeManager.h"
#include "DynamicByteArray.h"
#include <stdlib.h>
inline int getLeftMovingSteps(size_t k, unsigned char resiBitLength)
{
	return 8 - k%8 - resiBitLength;
}
size_t convertIntArray2ByteArray_fast_2b(unsigned char* timeStepType, size_t timeStepTypeLength, unsigned char **result)
{
	size_t i, j, byteLength = 0;
	if(timeStepTypeLength%4==0)
		byteLength = timeStepTypeLength*2/8;
	else
		byteLength = timeStepTypeLength*2/8+1;
	if(byteLength>0)
		*result = (unsigned char*)malloc(byteLength*sizeof(unsigned char));
	else
		*result = NULL;
	size_t n = 0;
	for(i = 0;i<byteLength;i++)
	{
		int tmp = 0;
		for(j = 0;j<4&&n<timeStepTypeLength;j++)
		{
			int type = timeStepType[n];
			switch(type)
			{
			case 0:

				break;
			case 1:
				tmp = (tmp | (1 << (6-j*2)));
				break;
			case 2:
				tmp = (tmp | (2 << (6-j*2)));
				break;
			case 3:
				tmp = (tmp | (3 << (6-j*2)));
				break;
			default:
				printf("Error: wrong timestep type...: type[%zu]=%d\n", n, type);
				exit(0);
			}
			n++;
		}
		(*result)[i] = (unsigned char)tmp;
	}
	return byteLength;
}
size_t convertIntArray2ByteArray_fast_dynamic(unsigned char* timeStepType, unsigned char resiBitLength, size_t nbEle, unsigned char **bytes)
{
	size_t i = 0, j = 0, k = 0;
	int value;
	DynamicByteArray* dba;
	new_DBA(&dba, 1024);
	int tmp = 0, leftMovSteps = 0;
	for(j = 0;j<nbEle;j++)
	{
		if(resiBitLength==0)
			continue;
		value = timeStepType[i];
		leftMovSteps = getLeftMovingSteps(k, resiBitLength);//8 - k%8 - resiBitLength;
		if(leftMovSteps < 0)
		{
			tmp = tmp | (value >> (-leftMovSteps));
			addDBA_Data(dba, (unsigned char)tmp);
			tmp = 0 | (value << (8+leftMovSteps));
		}
		else if(leftMovSteps > 0)
		{
			tmp = tmp | (value << leftMovSteps);
		}
		else //==0
		{
			tmp = tmp | value;
			addDBA_Data(dba, (unsigned char)tmp);
			tmp = 0;
		}
		i++;
		k += resiBitLength;
	}
	if(leftMovSteps != 0)
		addDBA_Data(dba, (unsigned char)tmp);
	convertDBAtoBytes(dba, bytes);
	size_t size = dba->size;
	free_DBA(dba);
	return size;
}