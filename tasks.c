/*	Simple example tasks
 *	Copyright (c) 2020 by SECKC Software
 */
#include <stdio.h>
#include "stdio2.h"
#include "tasks.h"
#include "gps.h"
#include "tsip_parser.h"
#include "cc1050.h"
#include "aprs.h"
#include "afsk_mod.h"

void task_gps_fix( void )
{
puts("TaskGpsFix\r\n");
pend_task(TASK_APRS_TX);
}

void task_gps_pkt( void )
{
puts("TaskGpsPkt:");
if( gps_pkt_id == TSIP_PACKET_SUPER )
	{
	puts_hex_u8(gps_pkt_id);
	puts(" SuperId:");
	puts_hex_u8(gps_pkt_super);
	}
else
	{
	puts_hex_u8(gps_pkt_id);
	}
puts("\r\n");

if( gps_pkt_id == TSIP_PACKET_SBAS_STATUS )
	{
	GPS_configure();
	}
}

void task_aprs_tx( void )
{
puts("Preparing AFSK\r\n");
aprs_send();
CC1050_tx_enable();
while( afsk_busy() );
puts("Done\r\n");
CC1050_tx_disable();
}