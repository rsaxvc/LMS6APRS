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

static void print_portf_registers( void )
{
putstr("PFDDR:");put_hex_u8(PFDDR);putstr("\r\n");
putstr("PFOR :");put_hex_u8(PFOR );putstr("\r\n");
putstr("PFDR :");put_hex_u8(PFDR );putstr("\r\n");
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
	GPS_init();
	
	puts("APRS Tracker");
	puts("By SecKC");

//	CC1050_init2( 3957808 );//446.175 on paper - low FSK
//	CC1050_init2( 3957764 );//446.175 on paper, 446.185 on specan - center
	CC1050_init2( 3957675 );//446.165 on paper, 446.175 on specan - center

	#define DELAY_1S delay_millis(250);delay_millis(250);delay_millis(250);delay_millis(250)
	DELAY_1S;
	morse_transmit_word( "KD0LIX" );
	DELAY_1S;
	
	pend_task(TASK_APRS_TX);
	
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

