/*	Simple example tasks
 *	Copyright (c) 2020 by SECKC Software
 */
#include <stdio.h>
#include "stdio2.h"
#include "tasks.h"
#include "gps.h"
#include "tsip_parser.h"

void task_a( void )
{
//puts("TaskA\r\n");
pend_task( TASK_B );
}

void task_b( void )
{
//puts("TaskB\r\n");
pend_task( TASK_A );
}

void task_gps_fix( void )
{
puts("TaskGpsFix\r\n");
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