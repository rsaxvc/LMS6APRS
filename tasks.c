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
#define ICD_GPS_200_PI 3.1415926535898f
#define ICD_GPS_200_RAD2DEG (180.0f/ICD_GPS_200_PI)
putstr("TaskGpsFix:0x");
put_hex_u32( *(uint32_t*)&gps_lla_packet[0]);
putstr(",0x");
put_hex_u32( *(uint32_t*)&gps_lla_packet[1]);
putstr("\r\n");
//pend_task(TASK_APRS_TX);
}

void task_gps_pkt( void )
{
putstr("TaskGpsPkt:");
if( gps_pkt_id == TSIP_PACKET_SUPER )
	{
	put_hex_u8(gps_pkt_id);
	putstr(" SuperId:");
	put_hex_u8(gps_pkt_super);
	}
else
	{
	put_hex_u8(gps_pkt_id);
	}
putstr(" len:");put_int_u8(gps_pkt_len);
putstr("\r\n");

if( gps_pkt_id == TSIP_PACKET_SBAS_STATUS )
	{
	GPS_configure();
	}
}

void task_aprs_tx( void )
{
puts("Preparing AFSK");
aprs_send();
CC1050_tx_enable();
//At this point ei0, the modulation interrupt, is enabled.
//We must not disabled interrupts until afsk_busy() returns
//false. Of note, the standard output functions like puts
//also disable interrupts for about 20 microseconds per
//character, which may goober up the transmit interrupt timing,
//so make sure not to call those here either.
while( afsk_busy() );
puts("Done");
CC1050_tx_disable();
}

void task_gps_ovr( void )
{
puts("WARNING: GPS Overrun occurred");
}