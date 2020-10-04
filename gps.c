/*	GPS Driver
 *	Copyright (c) 2020 by SecKC Software
 */
#include "tsip_parser.h"
#include <io72324.h>
#include <string.h>
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
	SCISR1_WAKE_BIT = 3, //wake
	SCISR1_PCE_BIT = 2, //parity control enable
	SCISR1_PS_BIT = 1, //parity selection
	SCISR1_PIE_BIT = 0, //parity interrupt enable
	
	SCICR2_TIE_BIT = 7, //Transmit Interrupt Enable
	SCICR2_TCIE_BIT = 6,//Transmission Complete Interrupt Enable
	SCICR2_RIE_BIT = 5, //Receive Interrupt Enable
	SCICR2_ILIE_BIT = 4,//Idle Line Interrupt Enable
	SCICR2_TE_BIT = 3,  //Transmitter Enable
	SCICR2_RE_BIT = 2,  //RX enable
	SCICR2_RWU_BIT = 1, //receiver mute
	SCICR2_SBK_BIT = 0, //send break
	
	SCISR_TDRE_MASK = (1<<SCISR_TDRE_MASK),
	SCISR_RDRF_MASK = (1<<SCISR_RDRF_BIT),
	
	SCICR2_RIE_MASK = (1<<SCICR2_RIE_BIT),
	SCICR2_TE_MASK = (1<<SCICR2_TE_BIT),
	SCICR2_RE_MASK = (1<<SCICR2_RE_BIT)
	};

/*Init the UART for RX interrupts and 38400 baud*/
static void uart_init( void )
{
SCICR1 = 0;
SCICR2 = SCICR2_RIE_MASK|SCICR2_TE_MASK|SCICR2_RE_MASK;
SCIBRR = 0xC0; //BaudRateDivider=13, so BaudRate = FCPU/16/13 = 38400 baud
}

/*queue a byte for TX, don't wait for it to go out*/
static void uart_tx( unsigned char byte )
{
//Wait for TX empty
while( (SCISR & SCISR_TDRE_MASK)==0 );

//Load the shift register
SCIDR = byte;
}

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

float gps_lla_packet[5];

void tsip_process_packet( unsigned char id, const unsigned char * ptr, unsigned char len)
{
switch( id ) 
	{
	case TSIP_PACKET_LLA_FLOAT:
		if( len == 20 )
			{
			memcpy( gps_lla_packet, ptr, 20 );
			pend_task_irq( TASK_ID_GPS );
			}
		break;
	}
}

void GPS_init(void)
{
static const unsigned char config_packet[] = { 0x35, 0x02, 0x00, 0x01 };
uart_init();
tsip_tx( sizeof(config_packet), config_packet );
}

/*Handle GPS UART interrupts*/
@interrupt void GPS_interrupt(void)
{
if( SCISR & SCISR_RDRF_MASK )
	{
	tsip_parser_push( SCIDR );
	}
}