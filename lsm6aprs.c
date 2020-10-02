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
	tasks = 0;
	port_init();
	timer_init();
	puts("APRS Tracker\r\n");
	puts("By SecKC\r\n");
	
	pend_task( TASK_A );

	for (;;)
		{
		unsigned char new_tasks;
		
		SIM();
		new_tasks = tasks;
		tasks = 0;
		RIM();
		
		if( new_tasks & TASK_A )
			{
			task_a();
			}

		if( new_tasks & TASK_B )
			{
			task_b();
			}

		SIM();
		if( !tasks )
			{
			WFI();
			}
		RIM();
		}
	}

