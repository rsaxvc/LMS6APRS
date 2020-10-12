/*	APRS Tracker
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */

#include <stdio.h>
#include "stdio2.h"

#include "aprs.h"
#include "afsk_mod.h"
#include "cc1050.h"
#include "config.h"
#include "asmtools.h"
#include "tasks.h"
#include "spilib.h"
#include "tsip_parser.h"
#include "gpio_def.h"

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

uint8_t gps_counter;
void GPS_ready( void )
{
gps_counter++;
if( gps_counter >= APRS_TRANSMIT_PERIOD )
	{
	pend_task( TASK_APRS_TX );
	gps_counter = 0;
	}
}


void main(void)
	{
	unsigned char i;

	gps_counter = APRS_TRANSMIT_PERIOD;
	tasks = 0;
	port_init();
	timer_init();
	SPI_init();
	tsip_parser_reset();
	GPS_init();
	
	puts("APRS Tracker");
	puts("By SecKC");

//	CC1050_init2( 3957808 );//446.175 on paper - low FSK
//	CC1050_init2( 3957764 );//446.175 on paper, 446.185 on specan - center
	CC1050_init2( 3957675 );//446.165 on paper, 446.175 on specan - center
	
	for (;;)
		{
		unsigned char new_tasks;
		
		SIM();
		new_tasks = tasks;
		tasks = 0;
		RIM();
		
		if( new_tasks & TASK_GPS_FIX )
			{
			task_gps_fix();
			}
		
		if( new_tasks & TASK_GPS_PKT )
			{
			task_gps_pkt();
			}

		if( new_tasks & TASK_APRS_TX )
			{
			task_aprs_tx();
			}
		
		if( new_tasks & TASK_GPS_OVR )
			{
			task_gps_ovr();
			}

		SIM();
		if( !tasks )
			{
			//This lines causes us to drop tasks.
			//Need to understand why.
			//WFI();
			}
		RIM();
		}
	}

