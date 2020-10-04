#include <string.h>

float tsip_float_to_native_float( const unsigned char * dptr )
{
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	float retn;
	uint32_t temp;
	
	memcpy( &temp, ptr, sizeof(temp) );
	temp = __builtin_bswap32( temp );
	memcpy( &retn, &temp, sizeof(temp) );
	
	return retn;
#else
	float retn;
	memcpy( &retn, dptr, sizeof(float) );
	return retn;
#endif
}
