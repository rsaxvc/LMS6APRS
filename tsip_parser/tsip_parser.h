#ifndef TSIP_PARSER_H
#define TSIP_PARSER_H

#define TSIP_MAX_PACKET_LEN 100

enum
	{
	TSIP_PACKET_LLA_DOUBLE = 0x84,
	TSIP_PACKET_LLA_FLOAT  = 0x4A,
	TSIP_PACKET_SBAS_STATUS= 0x82,
	TSIP_PACKET_SUPER      = 0x8F
	};

void tsip_process_packet( unsigned char id, const unsigned char * packet, unsigned char len );
void tsip_parser_push( unsigned char ch );
void tsip_parser_reset( void );

float tsip_float_to_native_float( const unsigned char * dptr );

#endif
