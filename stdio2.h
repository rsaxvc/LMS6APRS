/*	Extensions to STDIO
 *	Copyright (c) 2020 by SECKC Software
 */
#ifndef STDIO2_H
#define STDIO2_H

#include <stdint.h>

/* Utilities to avoid linking in printf */
int puts_int_u8( uint8_t i );
int puts_int_s8( int8_t  i );
int puts_hex_u4( uint8_t i );
int puts_hex_u8( uint8_t i );
int puts_hex_u16( uint16_t i );
#define puts_reg(NAME)  puts(#NAME);puts(":0x");puts_hex_u8(NAME)

#endif
