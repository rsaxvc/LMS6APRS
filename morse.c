/*	CC1050 management
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */

#include <stdio.h>

#include "cc1050.h"
#include "delay.h"
#include "morse.h"

static void long_delay_millis( unsigned int delay )
{
while( delay > 255 )
	{
	delay_millis(255);
	delay -= 255;
	}
delay_millis((unsigned char)delay);
}

#define BASE_DELAY (100ul)
static void delay_dash( void ){long_delay_millis(3*BASE_DELAY);}
static void delay_dot( void ){long_delay_millis(BASE_DELAY);}
static void delay_between_dot_dash( void ){long_delay_millis(BASE_DELAY);}
static void delay_character( void ){long_delay_millis(3*BASE_DELAY);}
static void delay_word( void ){ long_delay_millis( 7*BASE_DELAY);}

void morse_transmit_word( const char * str )
{
putstr("MORSE TX:");
while( *str )
	{
	const char * morse = morse_code_from_asc(*str);
	putchar(*str);putchar('(');
	while( *morse )
		{
		putchar(*morse);
		CC1050_tx_enable();
		if( *morse == '-' )
			{
			delay_dash();
			}
		else if( *morse == '.' )
			{
			delay_dot();
			}
		CC1050_tx_disable();
		delay_between_dot_dash();
		morse++;
		}
	delay_character();
	putchar(')');
	str++;
	}
delay_word();
putstr("\n");
}
