/*	GPS Driver
 *	Copyright (c) 2020 by SecKC Software
 */
#include "gps.h"
#include "tsip_parser.h"
#include <io72324.h>
#include <string.h>
#include <stdio.h>
#include "stdio2.h"
#include "tasks.h"

enum
	{
	SCISR_TDRE_BIT = 7, //Transmit data register empty
	SCISR_TC_BIT = 6, // Transmission complete
	SCISR_RDRF_BIT = 5, //RX Data Ready Flag
	SCISR_IDLE_BIT = 4, //Idle line detected
	SCISR_OR_BIT = 3, //OR overrun error
	SCISR_NF_BIT = 2, //Noise Flag
	SCISR_FE_BIT = 1, //Framing error
	SCISR_PE_BIT = 0, //Parity error

	SCICR1_R8_BIT = 7, //9th RX bit
	SCICR1_T8_BIT = 6, //9th TX bit
	SCICR1_SCID_BIT = 5, //SCI Disable
	SCICR1_M_BIT = 4, //word length 0=8N1, 1=9N1
	SCICR1_WAKE_BIT = 3, //wake
	SCICR1_PCE_BIT = 2, //parity control enable
	SCICR1_PS_BIT = 1, //parity selection
	SCICR1_PIE_BIT = 0, //parity interrupt enable
	
	SCICR2_TIE_BIT = 7, //Transmit Interrupt Enable
	SCICR2_TCIE_BIT = 6,//Transmission Complete Interrupt Enable
	SCICR2_RIE_BIT = 5, //Receive Interrupt Enable
	SCICR2_ILIE_BIT = 4,//Idle Line Interrupt Enable
	SCICR2_TE_BIT = 3,  //Transmitter Enable
	SCICR2_RE_BIT = 2,  //RX enable
	SCICR2_RWU_BIT = 1, //receiver mute
	SCICR2_SBK_BIT = 0, //send break
	
	SCISR_TDRE_MASK = (1<<SCISR_TDRE_BIT),
	SCISR_RDRF_MASK = (1<<SCISR_RDRF_BIT),
	SCISR_OR_MASK   = (1<<SCISR_OR_BIT),
	
	SCICR2_RIE_MASK = (1<<SCICR2_RIE_BIT),
	SCICR2_TE_MASK = (1<<SCICR2_TE_BIT),
	SCICR2_RE_MASK = (1<<SCICR2_RE_BIT)
	};

static float  gps_lla_packet[5];
static uint8_t gps_pkt_len;
static uint8_t gps_pkt_id;
static uint8_t gps_pkt_super;
static uint8_t gps_config_done;/*GPS configuration state machine*/

/*Init the UART for RX interrupts and 38400 baud*/
static void uart_init( void )
{
SCICR1 = 0;
SCICR2 = SCICR2_RIE_MASK|SCICR2_TE_MASK|SCICR2_RE_MASK;
SCIBRR = 0xC0; //BaudRateDivider=13, so BaudRate = FCPU/16/13 = 38400 baud
}

/*
Wait for transmitter ready.
Load a byte into the transmitter
Do not wait for transmission complete
*/
static void uart_tx( unsigned char byte )
{
//Wait for TX empty
while( !(SCISR & SCISR_TDRE_MASK) );

//Load the shift register
SCIDR = byte;
}

/*Transmit a TSIP frame. Handle start of packet and escaping*/
static void tsip_tx( unsigned char len, const unsigned char * packet )
{
uart_tx( 0x10 ); //Start of frame as long as not followed by another DLE/0x10
while( len )
	{
	unsigned char byte = *packet;
	if( byte == 0x10 )
		{
		uart_tx( 0x10 );uart_tx( 0x10 ); //DLE escape the DLE
		}
	else
		{
		uart_tx( byte ); //Transmit regular byte
		}
	len--;
	packet++;
	}
uart_tx( 0x10 );uart_tx( 0x03 );//End of frame
}

/*Process a received TSIP packet. This is called on an interrupt so no heavy processing here*/
void tsip_process_packet( unsigned char id, const unsigned char * ptr, unsigned char len)
{
unsigned char new_tasks = TASK_GPS_PKT;
gps_pkt_id = id;
gps_pkt_len = len;
switch( id ) 
	{
	case TSIP_PACKET_LLA_FLOAT:
		if( len == 20 )
			{
			memcpy( gps_lla_packet, ptr, 20 );
			new_tasks |= TASK_GPS_FIX;
			}
		break;
	case TSIP_PACKET_SUPER:
		gps_pkt_super = ptr[0];
		break;
	}
pend_task_irq( new_tasks );
}

/*Configure the GPS chipset. This is called at roughly 1Hz when the GPS is powered,
send one packet each time until all packets are sent*/
static void gps_configure(void)
{
//Enable 32-bit float and 32-bit fixed point packets.
//Once we have a parser for either remove the other.
static const unsigned char config_packet_0x35[] = { 0x35, 0x22, 0x00, 0x01, 0x00 }; 

//Enable 0x8F-23 packets
static const unsigned char config_packet_0x8e_23[] = { 0x8E, 0x23, 0x01 };

//Disable 0x8F-2A packets
static const unsigned char config_packet_0x8e_2a[] = { 0x8E, 0x2A, 0x00 };

//Disable 0x8F-2B packets
static const unsigned char config_packet_0x8e_2b[] = { 0x8E, 0x2B, 0x00 };

switch( gps_config_done )
	{
	case 0:
		tsip_tx( sizeof(config_packet_0x35), config_packet_0x35 );
		gps_config_done++;
		break;

	case 1:
		tsip_tx( sizeof(config_packet_0x8e_23), config_packet_0x8e_23 );
		gps_config_done++;
		break;

	case 2:
		tsip_tx( sizeof(config_packet_0x8e_2a), config_packet_0x8e_2a );
		gps_config_done++;
		break;

	case 3:
		tsip_tx( sizeof(config_packet_0x8e_2b), config_packet_0x8e_2b );
		gps_config_done++;
		break;
	}
}

void GPS_init(void)
{
gps_config_done = 0;
puts("GPS_init called");
tsip_parser_reset();
puts("parser reset");
uart_init();
puts("UART init complete");
puts("Done with GPS_init");
}

/*Handle GPS UART interrupts*/
@interrupt void GPS_interrupt(void)
{
uint8_t scisr = SCISR;
if( scisr & SCISR_OR_MASK )
	{
	/*Data overrun occurred*/
	SCIDR;/*Drop the data*/
	pend_task_irq( TASK_GPS_OVR );
	tsip_parser_reset();
	}
else if( scisr & SCISR_RDRF_MASK )
	{
	tsip_parser_push( SCIDR );
	}
}

//BELOW HERE FEATHERHAB HACKS
char* get_timestamp() 
{
	return "003000.00";
}
	
char* get_latitudeTrimmed() 
{
	return "00123.6";
}
char* get_latitudeLSBs()
{
	return "12";
}

char* get_longitudeTrimmed() 
{
	return "00345.6";
}
char* get_longitudeLSBs()
{
	return "34";
}

char* get_hdop() 
{
	return "00.0";
}

char* get_gpsaltitude()
{
	return "000000.0";
}

char* get_speedKnots() 
{
	return "000.0";
}

char* get_course() 
{
	return "000.0";
}
	
char* get_dayofmonth() 
{
	return "101020";
}

//BELOW HERE ARE GPS-RELATED TASKS

/*This task processes GPS fixes*/
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

/*this task prints GPS packet IDs for debugging*/
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

/*This packet is the last in a batch,
The first one means the GPS is ready to
accept new configurations*/
if( gps_pkt_id == TSIP_PACKET_SBAS_STATUS )
	{
	gps_configure();
	}
}

/*this task prints a warning when the UART is not serviced often enough*/
void task_gps_ovr( void )
{
puts("WARNING: GPS Overrun occurred");
}
