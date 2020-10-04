/*	Example Interrupt Handlers
 *	Copyright (c) 2020 by SECKC Software
 */
#include <io72324.h>
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
