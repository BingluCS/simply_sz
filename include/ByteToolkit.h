
#ifndef _ByteToolkit_H
#define _ByteToolkit_H

#ifdef __cplusplus
extern "C" {
#endif
#include "sz.h"

short getExponent_float(float value);


//inline short getPrecisionReqLength_float(float precision);

void intToBytes_bigEndian(unsigned char *b, unsigned int num);
short getPrecisionReqLength_double(double precision);


#ifdef __cplusplus
}
#endif

#endif /* ----- #ifndef _ByteToolkit_H  ----- */

