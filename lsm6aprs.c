/*	APRS Tracker
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */

#include "asmtools.h"
#include "tasks.h"

/*	main program
 */
static unsigned char tasks;

void pend_task( unsigned char new_tasks)
{
tasks |= new_tasks;
}

void main(void)
	{
	port_init();
	timer_init();
	puts("APRS Tracker\r\n");
	puts("By SecKC\r\n");
	
	pend_task( TASK_A );

	for (;;)
		{
		SIM();
		if( tasks & TASK_A )
			{
			tasks &= ~TASK_A;
			RIM();
			task_a();
			}
		else
			{
			RIM();
			}

		SIM();
		if( tasks & TASK_B )
			{
			tasks &= ~TASK_B;
			RIM();
			task_b();
			}
		else
			{
			RIM();
			}

		SIM();
		if( !tasks )
			{
			WFI();
			}
		RIM();
		}
	}

