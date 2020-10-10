/*	APRS Tracker
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */

#include <stdio.h>
#include "stdio2.h"

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
puts("PFDDR:");puts_hex_u8(PFDDR);puts("\r\n");
puts("PFOR :");puts_hex_u8(PFOR );puts("\r\n");
puts("PFDR :");puts_hex_u8(PFDR );puts("\r\n");
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
	
	puts("\r\n");
	puts("APRS Tracker\r\n");
	puts("By SecKC\r\n");

	CC1050_init( 0x3c, 0x64, 0x30 );
	#define DELAY_1S delay_millis(250);delay_millis(250);delay_millis(250);delay_millis(250)
//	morse_transmit_word( "KD0LIX" );
	
//	DELAY_1S;
	puts("Preparing AFSK\r\n");
	afsk_send((const void*)0x8000, 0x7FFF);
	CC1050_tx_enable();
	while( afsk_busy() );
	puts("Done\r\n");
	CC1050_tx_disable();
	DELAY_1S;

/*
	while(1)
		{
		DELAY_1S;
		CC1050_tx_enable();
		DELAY_1S;
		CC1050_tx_disable();
		}
*/

/*
	CC1050_tx_enable();
	while(1)
		{
		unsigned char i;
		for( i = 1; i < 254; ++i )
			{
			CC1050_hop( 0x3c, 0x64, i );
			}
		for( i = 254; i > 0; --i )
			{
			CC1050_hop( 0x3c, 0x64, i );			
			}
		}
*/

/*
	CC1050_tx_enable();
	while(1)
		{
		DELAY_1S;
		CC1050_hop2( 3957808 );
		DELAY_1S;
		CC1050_hop2( 3957808+44 );
		}
*/

/*
	while(1)
		{
		puts("Setting reg\r\n");
		CC1050_reg_set( 0x0C, 0x40 );
		puts("Reg:0x");puts_hex_u8( CC1050_reg_read( 0x0C ) );puts("\r\n");
		delay_millis(200);
		}
*/

/*
	CC1050_tx_enable();
	while(1)
		{
		DELAY_1S;
		GPIO_SET( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN );
		DELAY_1S;
		GPIO_CLR( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN );
		}
*/

	CC1050_tx_enable();
	GPIO_CLR( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN );
	DELAY_1S;
	GPIO_SET( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN );
	DELAY_1S;
		
	while(1)
		{
//		GPIO_CLR( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN );
//		GPIO_SET( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN );
		}

/*
	CC1050_hop( 0x3c, 0x64, 0x30 ); //KCAPRS PRIMARY
	CC1050_tx_enable();
	while(1)
		{
		CC1050_hop( 0x3b, 0xb6, 0x94 ); //KCAPRS SECONDARY 
		puts("\r\n");
		delay_millis(250);
//		CC1050_print_regs();

		CC1050_hop( 0x3c, 0x64, 0x30 ); //KCAPRS PRIMARY
		puts("\r\n");
		delay_millis(250);
//		CC1050_print_regs();
		}
*/

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

