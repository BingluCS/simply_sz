#include <stdio.h>
#include "rw.h"
#include "sz.h"
#include "defines.h"
#include "sz_float.h"


int main(int argc, char* argv[]) {
    char* inPath;
    size_t nbEle;    
    
	size_t outSize;
	unsigned char *bytes = NULL;
	int r1,r2,r3,r4,r5;
	inPath=argv[2];
	r1=atoi(argv[3]);

	float* data = readFloatData(inPath, &nbEle);

	size_t byteLength = 0; 

	Init_sz();

	// for(int i=0;i<20;i++)
	// 	printf("%d %f\n",i,data[i]);
	bytes = SZ_compress(SZ_FLOAT, data, &byteLength, r5, r4, r3, r2, r1);
	//cout<<0x40f<<endl;
    return 0;
}