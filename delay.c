#include "asmtools.h"
#include "delay.h"
//If the optimizer messes with these move to assembly.

//This one is roughly correct
void delay_seconds( unsigned char sec )
{
while( sec )
	{
	delay_millis( 250 );
	delay_millis( 250 );
	delay_millis( 250 );
	delay_millis( 250 );
	sec--;
	}
}

//This one is roughly correct
void delay_millis( unsigned char ms )
{
while( ms )
	{
	//Adjust for the delay_micros() drift*/
	delay_micros( 180 );
	delay_micros( 180 );
	delay_micros( 180 );
	delay_micros( 180 );
	ms--;
	}
}

//This delay loop takes roughly 1.4us/us
void delay_micros( unsigned char us )
{
while( us )
	{
	us--;
	}
}

//This delay loop is faster, and eventuall
//we should switch to it instead.
void delay_micros2( unsigned char us )
{
while( --us );
}