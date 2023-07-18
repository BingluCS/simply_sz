#include <stdio.h>
#include "rw.h"
#include "sz.h"
#include "defines.h"
#include "sz_float.h"

void Init(int* r1, int* r2, int* r3, int* r4, int* r5) {
	int quant_intervals=0;
	*r1=512*512*512;
    *r2=0;
    *r3=0;
	*r4=0;
	*r5=0;
	//*ABSerrorbound=115.862236;
	confparams_cpr->dataType=SZ_FLOAT;
	confparams_cpr->errorBoundMode=ABS;
	confparams_cpr->absErrBound=115.862236; //absoluted error bound
	confparams_cpr->relBoundRatio=0.001;	//relative error bound based on value range
	
	confparams_cpr->pw_relBoundRatio=0.1;	//point-wise relative error bound
											
	confparams_cpr->quantization_intervals=quant_intervals;//autamatically search for an optimized setting if it is 0

	if(confparams_cpr->quantization_intervals==0)
		exe_params->optQuantMode=1;
	else  {
		exe_params->intvCapacity = quant_intervals;
		exe_params->intvRadius = quant_intervals/2;
	}

	confparams_cpr->max_quant_intervals = 65536;// maximum quantization interval is valid only when quantization_intervals=0
	confparams_cpr->maxRangeRadius=confparams_cpr->max_quant_intervals/2;

	confparams_cpr->sampleDistance=100;// sampleDistance determins the number of samples used to optimize the quantization intervals
										//For example, sampleDistance=50 means 1/50=2% of data points are sample points

	confparams_cpr->predThreshold=0.99;	//the threshold to determine the ratio of predictable data over all data
	confparams_cpr->withRegression=SZ_WITH_LINEAR_REGRESSION;
	//confparams_cpr->dmin=3;
}

int main(int argc, char* argv[]) {
    char *inPath="./data/baryon_density.f32";
    size_t nbEle;    
    float* data = readFloatData(inPath, &nbEle);
	size_t outSize;
	unsigned char *bytes = NULL;
	int r1,r2,r3,r4,r5;
	size_t byteLength = 0; 
	Init_sz();
	// for(int i=0;i<20;i++)
	// 	printf("%d %f\n",i,data[i]);
	Init(&r1,&r2,&r3,&r4,&r5);
	bytes = SZ_compress(SZ_FLOAT, data, &byteLength, r5, r4, r3, r2, r1);
	//cout<<0x40f<<endl;
    return 0;
}