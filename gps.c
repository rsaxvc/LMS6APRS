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

static uint8_t gps_pkt_len;
static uint8_t gps_pkt_id;
static uint8_t gps_pkt_super;
static uint8_t gps_config_done;/*GPS configuration state machine*/
static uint8_t gps_compact_fix_packet[29];

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
	case TSIP_PACKET_SUPER:
		gps_pkt_super = ptr[0];
		if( ptr[0] == 0x23 && len == 29 )
			{
			memcpy( gps_compact_fix_packet, ptr, 29 );
			new_tasks |= TASK_GPS_FIX;
			}
		break;
	}
pend_task_irq( new_tasks );
}

/*Configure the GPS chipset. This is called at roughly 1Hz when the GPS is powered,
send one packet each time until all packets are sent*/
static void gps_configure(void)
{
//Enable 32-bit fixed point packets.
//Once we have a parser for either remove the other.
static const unsigned char config_packet_0x35[] = { 0x35, 0x20, 0x00, 0x01, 0x00 }; 

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
uart_init();
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

//BELOW HERE FEATHERHAB BUFFERS
static char latitude_formatted[9];//4903.50N is 49 degrees 3 minutes 30 seconds north.
static char longitude_formatted[10];//07201.75W is 72 degrees 1 minute 45 seconds west.
static char altitude_formatted[7];

//BELOW HERE FEATHERHAB HACKS
char* get_timestamp() 
{
	return "003100.00";
}

char* get_latitudeTrimmed(){return latitude_formatted;}
char* get_longitudeTrimmed(){return longitude_formatted;}
char* get_gpsaltitude(){return altitude_formatted;}

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

//BELOW HERE WE POPULATE THE FeatherHAB APIs
//4903.50N is 49 degrees 3 minutes 30 seconds north.
static void parse_latitude_into_aprs( int32_t latitude )
{
static const uint32_t scales[]=
    {
    119304648,
    11930465,
    1988411,
    198842,
    0,
    19885,
    1989
    };
	
uint8_t i;

if( latitude > 0 )
    {
    latitude_formatted[7] = 'N';
    }
else
    {
    latitude = -latitude;
    latitude_formatted[7] = 'S';
    }

//Latitude is now positive
for( i = 0; i < sizeof(scales)/sizeof(scales[0]); ++i )
    {
    if( i == 4 )
        {
        latitude_formatted[4] = '.';
        }
    else
        {
        int16_t latdiv = (int16_t)(latitude / scales[i]);
        latitude_formatted[i] = (char)('0' + latdiv); latitude -= latdiv * scales[i];
        }
    }
latitude_formatted[8]='\0';
}

static void parse_longitude_into_aprs( uint32_t longitude )
{
static const uint32_t scales[]=
    {
    1193046471,
    119304647,
    11930465,
    1988411,
    198842,
    0,
    19885,
    1989
    };

uint8_t i;

if( longitude >= 0x80000000 )
    {
    longitude_formatted[8] = 'W';
    longitude = 0xffffffff - longitude;
    }
else
    {
    longitude_formatted[8] = 'E';
    }

for( i = 0; i < sizeof(scales)/sizeof(scales[0]); ++i )
    {
    if( i == 5 )
        {
        longitude_formatted[5] = '.';
        }
    else
        {
        int16_t londiv = (int16_t)(longitude / scales[i]);
        longitude_formatted[i] = (char)('0' + londiv); longitude -= londiv * scales[i];
        }
    }
longitude_formatted[9]='\0';
}

static int32_t mm_to_feet( int32_t input )
{
//There are 1524 mm in 5 feet
//There are 381 mm in 1.25 feet
input /= 4; //Hack off two bits so we have some room
input *= 5; //This now fits, and represents an overall scaling of 1.25x
input /= 381;//complete the scaling
return input;
}

static void parse_altitude_into_aprs( int32_t altitude )
{
altitude = mm_to_feet( altitude );
if( altitude < 0 )
    {
    strcpy( altitude_formatted, "000000");
    }
else if( altitude > 999999 )
    {
    strcpy( altitude_formatted, "999999" );
    }
else
    {
    signed char i;
    for( i = 5; i >= 0; --i )
        {
        altitude_formatted[i] = (char)('0' + (int8_t)(altitude % 10)); altitude /= 10;
        }
    altitude_formatted[6] = '\0';
    }
}

//BELOW HERE ARE GPS-RELATED TASKS

/*This task processes GPS fixes*/
void task_gps_fix( void )
{
int32_t latitude;
uint32_t longitude;
int32_t altitude;
uint8_t fix_byte = gps_compact_fix_packet[8];

puts("TaskGpsFix:");
//Check for a fix and make sure it's a 3D one since we need altitude
if( (fix_byte & ((1<<0)|(1<<2))) == 0 ) 
	{
	if( fix_byte & (1<<5) )
		{
		puts("\tVelScale=0.02m/s");
		}
	else
		{
		puts("\tVelScale=0.005m/s");
		}
		
	memcpy( &latitude,  gps_compact_fix_packet+9,  sizeof(latitude) );
	parse_latitude_into_aprs( latitude );
	memcpy( &longitude, gps_compact_fix_packet+13, sizeof(longitude) );
	parse_longitude_into_aprs( longitude );
	memcpy( &altitude,  gps_compact_fix_packet+17, sizeof(altitude) );
	parse_altitude_into_aprs( altitude );

	putstr("\tLat:");puts(latitude_formatted);
	putstr("\tLon:");puts(longitude_formatted);
	putstr("\tAlt:");puts(altitude_formatted);
	GPS_ready();
	}
else
	{
	puts("\tNo fix available");	
	}
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
putstr("\n");

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
