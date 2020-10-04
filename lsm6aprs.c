/*	APRS Tracker
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */

#include "asmtools.h"
#include "tasks.h"
#include "spilib.h"
#include "tsip_parser.h"

/*	main program
 */
static unsigned char tasks;

void pend_task( unsigned char new_tasks)
{
tasks |= new_tasks;
}

static const char example_packet[] = { 0x10, 0x01, 'H','E','L','L','O',0,0x10,0x03 };

void tsip_process_packet( unsigned char id, const unsigned char * ptr, unsigned char len)
{
puts("GOT TSIP PACKET:");
puts(example_packet);
}

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
	for( i = 0; i < sizeof(example_packet); ++i )
		{
		tsip_parser_push( example_packet[0] );
		}
		
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

		SIM();
		if( !tasks )
			{
			WFI();
			}
		RIM();
		}
	}

