/*	APRS Tracker
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */

#include <stdio.h>
#include <stdio2.h>

/*We use a custom puts, because the default appends a newline*/
int puts( const char * s )
{
while( *s )
	{
	putchar( *s );
	s++;
	}
return 0;
}

/*put a string formatted from a uint8*/
int puts_int_u8( uint8_t i )
{
if( i >= 200 )
	{
	putchar( '2' );
	i -= 200;
	}
else if( i >= 100 )
	{
	putchar( '1' );
	i -= 100;
	}

if( i >= 10 )
	{
	unsigned char m = (unsigned char)(i / 10);
	putchar( (unsigned char)('0' + m) );
	i -= (uint8_t)(10 * m);
	}

putchar( (unsigned char)('0' + i) );
return 0;
}

/*put a hex string formatted from a uint4*/
int puts_hex_u4( uint8_t i )
{
if( i > 9 )
	{
    putchar( (char)('A' + i - 10) );
	}
else
	{
	putchar( (char)('0' + i) );
	}
return 0;
}

/*put a hex string formatted from a uint8*/
int puts_hex_u8( uint8_t i )
{
puts_hex_u4( (uint8_t)(i >> 4 ) );
puts_hex_u4( (uint8_t)(i & 0xF) );
return 0;
}

/*put a hex string formatted from a uint16*/
int puts_hex_u16( uint16_t i )
{
puts_hex_u4( (uint8_t)( (i >>12) & 0xF ) );
puts_hex_u4( (uint8_t)( (i >> 8) & 0xF ) );
puts_hex_u4( (uint8_t)( (i >> 4) & 0xF ) );
puts_hex_u4( (uint8_t)( (i >> 0) & 0xF ) );
return 0;
}

/*put a hex string formatted from a uint16*/
int puts_hex_u32( uint32_t i )
{
puts_hex_u4( (uint8_t)( (i >>28) & 0xF ) );
puts_hex_u4( (uint8_t)( (i >>24) & 0xF ) );
puts_hex_u4( (uint8_t)( (i >>20) & 0xF ) );
puts_hex_u4( (uint8_t)( (i >>16) & 0xF ) );
puts_hex_u4( (uint8_t)( (i >>12) & 0xF ) );
puts_hex_u4( (uint8_t)( (i >> 8) & 0xF ) );
puts_hex_u4( (uint8_t)( (i >> 4) & 0xF ) );
puts_hex_u4( (uint8_t)( (i >> 0) & 0xF ) );
return 0;
}

int puts_int_s8( int8_t i )
{
if( i == -128 )
	{
	puts("-128");
	return 0;
	}

if( i < 0 )
	{
	putchar( '-' );
	i = (int8_t)-i;
	}

if( i > 100 )
	{
	putchar('1');
	i -= 100;
	}
if( i > 9 )
	{
    putchar( (char)('0' + (i/10)) );
	i -= 10*(i/10);
	}

putchar( (char)('0' + i) );
	
return 0;
}
