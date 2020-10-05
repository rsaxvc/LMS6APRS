/*	APRS Tracker
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */

#include <stdio.h>
#include "stdio2.h"

#include "asmtools.h"
#include "tasks.h"
#include "spilib.h"
#include "tsip_parser.h"

/*	main program
 */
static unsigned char tasks;

/*This is used from the main loop and tasks*/
void pend_task( unsigned char new_tasks)
{
SIM();
tasks |= new_tasks;
RIM();
}

/*This is used from inside an interrupt handler*/
void pend_task_irq( unsigned char new_tasks)
{
tasks |= new_tasks;
}

static const char example_packet[] = { 0x10, 0x01, 'H','E','L','L','O',0,0x10,0x03 };

void main(void)
	{
	unsigned char i;

	tasks = 0;
	port_init();
	timer_init();
	SPI_init();
	tsip_parser_reset();
	
	puts("APRS Tracker\r\n");
	puts("By SecKC\r\n");

	CC1050_init( 0x3c, 0x64, 0x30 );
	CC1050_tx_enable();
	while(1)
		{
		CC1050_hop( 0x3b, 0xb6, 0x94 ); //KCAPRS SECONDARY 
		delay_millis(250);
		CC1050_hop( 0x3c, 0x64, 0x30 ); //KCAPRS PRIMARY
		delay_millis(250);
		}

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
		
		if( new_tasks & TASK_GPS_FIX )
			{
			task_gps_fix();
			}
		
		if( new_tasks & TASK_GPS_PKT )
			{
			task_gps_pkt();
			}

		SIM();
		if( !tasks )
			{
			WFI();
			}
		RIM();
		}
	}

