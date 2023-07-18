#include <stdio.h>
#include <stdint.h>

#include "rw.h"

typedef union lfloat
{
    float value;
    unsigned int ivalue;
    unsigned char byte[4];
} lfloat;  


float *readFloatData(char *srcFilePath, size_t *nbEle) 
{

 	size_t inSize;
	FILE *pFile = fopen(srcFilePath, "rb");
    if (pFile == NULL)
    {
        printf("Failed to open input file. 1\n");
        return NULL;
    }
	fseek(pFile, 0, SEEK_END);
    inSize = ftell(pFile);
    *nbEle = inSize/4;
    fclose(pFile);

    if(inSize<=0)
    {
		printf("Error: input file is wrong!\n");
	}

    float *daBuf = (float *)malloc(inSize);

    pFile = fopen(srcFilePath, "rb");
    if (pFile == NULL)
    {
        printf("Failed to open input file. 2\n");
        return NULL;
    }
    fread(daBuf, 4, *nbEle, pFile);
    fclose(pFile);
    return daBuf;
}