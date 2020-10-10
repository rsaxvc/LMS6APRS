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
	while(1)
		{
		GPIO_SET(GPIO_SOFTUART_TX_PORT, GPIO_SOFTUART_TX_PIN);
		GPIO_CLR(GPIO_SOFTUART_TX_PORT, GPIO_SOFTUART_TX_PIN);
		}
	}
