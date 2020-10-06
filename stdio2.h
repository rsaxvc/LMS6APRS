/*	Extensions to STDIO
 *	Copyright (c) 2020 by SECKC Software
 */
#ifndef STDIO2_H
#define STDIO2_H

/* Utilities to avoid linking in printf */
int puts_int_u8( unsigned char i );
int puts_hex_u4( unsigned char i );
int puts_hex_u8( unsigned char i );
#define puts_reg(NAME)  puts(#NAME);puts(":0x");puts_hex_u8(NAME)

#endif
