#ifndef TSIP_PARSER_H
#define TSIP_PARSER_H

enum
	{
	TSIP_PACKET_LLA_DOUBLE = 0x84,
	TSIP_PACKET_LLA_FLOAT  = 0x4A
	};

void tsip_process_packet( unsigned char id, const unsigned char * packet, unsigned char len );
void tsip_parser_push( unsigned char ch );
void tsip_parser_reset( void );

float tsip_float_to_native_float( const unsigned char * dptr );

#endif
