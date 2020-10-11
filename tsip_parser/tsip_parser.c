/*
 * tsip_parser.c, based on:
 * clk_trimtsip.c,v 4.17 2005/04/16 17:32:10 kardel RELEASE_20050508_A
 *
 * Trimble TSIP support
 * Thanks to Sven Dietrich for providing test hardware
 *
 * Copyright (c) 2020 by SecKC
 * Copyright (c) 1995-2005 by Frank Kardel <kardel <AT> ntp.org>
 * Copyright (c) 1989-1994 by Frank Kardel, Friedrich-Alexander Universität Erlangen-Nürnberg, Germany
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#include "tsip_parser.h"

static unsigned char parse_data[TSIP_MAX_PACKET_LEN];
static unsigned char parse_index;

static void start_packet( void )
{
parse_index = 0;
}

static void append_packet( unsigned char ch )
{
if( parse_index < TSIP_MAX_PACKET_LEN )
    {
    parse_data[parse_index] = ch;
    }
parse_index++;
}

static void end_packet( void )
{
if( parse_index > 0 ) {
	tsip_process_packet( parse_data[0], parse_data+1, (unsigned char)(parse_index-1));
	}
}

static char t_in_pkt = 0;
static char t_dle = 0;

#define DLE 0x10
#define ETX 0x03
void tsip_parser_push( unsigned char ch )
{
	if (!t_in_pkt && ch != DLE) {
		/* wait for start of packet */
		return;
	}

	switch (ch) {
		case DLE:
		if (!t_in_pkt) {
			t_dle = 0;
			t_in_pkt = 1;
			start_packet();
		} else if (t_dle) {
			/* Double DLE -> insert a DLE */
			t_dle = 0;
			append_packet(DLE);
		} else {
			t_dle = 1;
		}
		break;

		case ETX:
			if (t_dle) {
				/* DLE,ETX -> end of packet */
				t_in_pkt = t_dle = 0;
				end_packet();
				return;
			}

		default:/* collect data */
		t_dle = 0;
		append_packet(ch);
	}
}

void tsip_parser_reset( void )
{
t_dle = 0;
t_in_pkt = 0;
}