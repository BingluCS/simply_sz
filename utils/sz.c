#include <stdio.h>
#include "rw.h"
#include "sz.h"
#include "defines.h"
#include "sz_float.h"
#include <string.h>

void usage(){
	printf("	-M <error bound mode> : 10 options as follows. \n");
	printf("		ABS (absolute error bound)\n");
	printf("		REL (value range based error bound, so a.k.a., VR_REL)\n");
	printf("	-A <absolute error bound>: specifying absolute error bound\n");
	printf("	-R <value_range based relative error bound>: specifying relative error bound\n");
	printf("	-i <original data file> : original data file\n");
	printf("	-1 <nx> : dimension for 1D data such as data[nx]\n");
	printf("	-2 <nx> <ny> : dimensions for 2D data such as data[ny][nx]\n");
	printf("	-3 <nx> <ny> <nz> : dimensions for 3D data such as data[nz][ny][nx] \n");
	printf("	-4 <nx> <ny> <nz> <np>: dimensions for 4D data such as data[np][nz][ny][nx] \n");
	printf("	-4 <nx> <ny> <nz> <np>: dimensions for 4D data such as data[np][nz][ny][nx] \n");
	printf("	-O <quantization intervals>: \nThe number of quantization intervals should be always set to an \"even\" number!\n");
	printf("	   If it is set to 0, SZ will autamatically search for an optimized setting\n"); 
	printf("	   Otherwise, it has be to no less than 4 and no greater than 65536, such as 256\n"); 	
	printf("	-q <maximum quantization interval>: The maximumquantization intervals is valid only when quantization_intervals=0\n");
	printf("	-p <output file>: The output file path\n"); 	
	exit(0);
}

int main(int argc, char* argv[]) {
    char* inPath;
    size_t nbEle;    
    
	size_t outSize;
	unsigned char *bytes = NULL;
	size_t r1,r2,r3,r4,r5,quant_intervals=0,max_quant=65536;
	char* errBoundMode=NULL;
	char* absErrorBound=NULL;
	char* relErrorBound=NULL;
	for(int i=1;i<argc;i++) {
		if (argv[i][0] != '-' || argv[i][2])
			usage();
		switch (argv[i][1]) {
			case 'i':
				if (++i == argc)
					usage();
				inPath = argv[i];		
				break;	
			case '1': 
				if (++i == argc || sscanf(argv[i], "%zu", &r1) != 1)
					usage();
				break;
			case '2':
				if (++i == argc || sscanf(argv[i], "%zu", &r1) != 1 ||
					++i == argc || sscanf(argv[i], "%zu", &r2) != 1)
					usage();
				break;
			case '3':
				if (++i == argc || sscanf(argv[i], "%zu", &r1) != 1 ||
					++i == argc || sscanf(argv[i], "%zu", &r2) != 1 ||
					++i == argc || sscanf(argv[i], "%zu", &r3) != 1)
					usage();		
				break;
			case '4':
				if (++i == argc || sscanf(argv[i], "%zu", &r1) != 1 ||
					++i == argc || sscanf(argv[i], "%zu", &r2) != 1 ||
					++i == argc || sscanf(argv[i], "%zu", &r3) != 1 ||
					++i == argc || sscanf(argv[i], "%zu", &r4) != 1)
					usage();
				break;
			case 'M':
				if (++i == argc)
					usage();
				errBoundMode = argv[i];
				break;
			case 'A':
				if (++i == argc)
					usage();
				absErrorBound = argv[i];
				break;
			case 'R':
				if (++i == argc)
					usage();
				relErrorBound = argv[i];
				break;
			case 'O':
				if (++i == argc)
					usage();
				quant_intervals = atoi(argv[i]);
				break;
			case 'q':
				if (++i == argc)
					usage();
				max_quant = atoi(argv[i]);
				break;
			case 'p':
				if (++i == argc)
					usage();
				filename =argv[i];
				break;
		}
		
	}
	float* data = readFloatData(inPath, &nbEle);

	size_t byteLength = 0; 
	if(errBoundMode != NULL)
	{
		int errorBoundMode = 0;
		if(strcmp(errBoundMode, "ABS")==0) {
			if(absErrorBound != NULL)
				confparams_cpr->absErrBound = atof(absErrorBound);
			else {
				printf("Error: wrong error bound without using the option '-A'\n");
				exit(0);
			}
			confparams_cpr->errorBoundMode = ABS;
		}
		else if(strcmp(errBoundMode, "REL")==0||strcmp(errBoundMode, "VR_REL")==0) {
			errorBoundMode = REL;
			if(relErrorBound != NULL)
				confparams_cpr->relBoundRatio = atof(relErrorBound);
			else {
				printf("Error: wrong error bound without using the option '-R'\n");
				exit(0);
			}
			confparams_cpr->errorBoundMode = REL;
		}	
		else
		{
			printf("Error: wrong error bound mode setting by using the option '-M'\n");
			//usage();
			exit(0);
		}	
	}

	Init_sz(quant_intervals,max_quant);
	bytes = SZ_compress(SZ_FLOAT, data, &byteLength, r5, r4, r3, r2, r1);

    return 0;
}