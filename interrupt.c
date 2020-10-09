/*	Example Interrupt Handlers
 *	Copyright (c) 2020 by SECKC Software
 */
#include "gpio_def.h"
#include "interrupt.h"
/*
@interrupt void it_example(void)
	{
	//Clear interrupt generating flag
	//Do something with data
	//maybe pend a task
	}
*/

/*Handle accidental interrupts*/
@interrupt void bad_interrupt(void)
	{
	while(1);
	}

static const unsigned char dither_me_timbers[]=
{
0 , 1 , 1 , 1 , 1 , 0 , 1 , 1 , 1 , 1 , 0 , 1 , 1 , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 1 , 0 , 1 , 1 , 0 , 1 , 1 , 1 , 1 , 0 , 1 , 1 , 1
};

unsigned char iterator;

@interrupt void ei0_interrupt(void)
{
iterator++;
if( iterator >= sizeof( dither_me_timbers ) )
	{
	iterator = 0;
	}

if( dither_me_timbers[iterator] )
	{
	GPIO_SET( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN );
	}
else
	{
	GPIO_CLR( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN );
	}
}