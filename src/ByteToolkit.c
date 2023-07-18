#include "ByteToolkit.h"

short getExponent_float(float value)
{
	//int ivalue = floatToBigEndianInt(value);

	lfloat lbuf;
	lbuf.value = value;
	int ivalue = lbuf.ivalue;
	
	int expValue = (ivalue & 0x7F800000) >> 23;
	expValue -= 127;
	return (short)expValue;
}


short getPrecisionReqLength_double(double precision)
{
	ldouble lbuf;
	lbuf.value = precision;
	int64_t lvalue = lbuf.lvalue;
	
	int expValue = (int)((lvalue & 0x7FF0000000000000) >> 52);
	expValue -= 1023;
//	unsigned char the1stManBit = (unsigned char)((lvalue & 0x0008000000000000) >> 51);
//	if(the1stManBit==1)
//		expValue--;
	return (short)expValue;
}
void intToBytes_bigEndian(unsigned char *b, unsigned int num)
{
	b[0] = (unsigned char)(num >> 24);	//high
	b[1] = (unsigned char)(num >> 16);	
	b[2] = (unsigned char)(num >> 8);	
	b[3] = (unsigned char)(num);	//low
	
	//note: num >> xxx already considered endian_type...
//if(dataEndianType==LITTLE_ENDIAN_DATA)
//		symTransform_4bytes(*b); //change to BIG_ENDIAN_DATA
}