#include "asmtools.h"

//
void delay_millis( unsigned int ms )
{
while( ms )
	{
	delay_micros( 1000 );
	ms--;
	}
}

void delay_micros( unsigned int us )
{
while( us )
	{
	_asm("NOP");
	_asm("NOP");
	us--;
	}
}