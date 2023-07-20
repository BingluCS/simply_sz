#ifndef _TypeManager_H
#define _TypeManager_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

inline int getLeftMovingSteps(size_t k, unsigned char resiBitLength);
size_t convertIntArray2ByteArray_fast_2b(unsigned char* timeStepType, size_t timeStepTypeLength, unsigned char **result);
size_t convertIntArray2ByteArray_fast_dynamic(unsigned char* timeStepType, unsigned char resiBitLength, size_t nbEle, unsigned char **bytes);



#ifdef __cplusplus
}
#endif

#endif /* ----- #ifndef _TypeManager_H  ----- */

