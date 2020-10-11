/*	CC1050 management
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */

#include "cc1050.h"
#include "delay.h"
#include "gpio_def.h"
#include "spilib.h"
#include <stdio.h>
#include <stdint.h>
#include "stdio2.h"

enum
	{
	REG_MAIN    = 0x00,
	REG_FREQ_2A = 0x01,
	REG_FREQ_1A = 0x02,
	REG_FREQ_0A = 0x03,
	REG_FREQ_2B = 0x04,
	REG_FREQ_1B = 0x05,
	REG_FREQ_0B = 0x06,
	REG_FSEP1   = 0x07,
	REG_FSEP0   = 0x08,
	REG_CURRENT = 0x09,
	REG_XOSC    = 0x0A,
	REG_PA_POW  = 0x0B,
	REG_PLL     = 0x0C,
	REG_LOCK    = 0x0D,
	REG_CAL     = 0x0E,
	REG_MODEM0  = 0x11,
	REG_FSCTRL  = 0x13,
	REG_PRESCALER = 0x1C,
	REG_TEST6   = 0x40,
	REG_TEST5   = 0x41,
	REG_TEST4   = 0x42,
	REG_TEST3   = 0x43,
	REG_TEST2   = 0x44,
	REG_TEST1   = 0x45,
	REG_TEST0   = 0x46
	};
	
enum
	{
	REG_MAIN_FREG_OFST = 6,
	REG_MAIN_TX_PD_OFST = 4,
	REG_MAIN_FS_PD_OFST = 3,
	REG_MAIN_CORE_PD_OFST = 2,
	REG_MAIN_BIAS_PD_OFST = 1,
	REG_MAIN_RESET_OFST = 0,
	
	REG_MAIN_FREG    = 1 << REG_MAIN_FREG_OFST,
	REG_MAIN_TX_PD   = 1 << REG_MAIN_TX_PD_OFST,
	REG_MAIN_FS_PD   = 1 << REG_MAIN_FS_PD_OFST,
	REG_MAIN_CORE_PD = 1 << REG_MAIN_CORE_PD_OFST,
	REG_MAIN_BIAS_PD = 1 << REG_MAIN_BIAS_PD_OFST,
	REG_MAIN_RESET   = 1 << REG_MAIN_RESET_OFST
	};

#define DUMP_REG(NAME)  putchar('\t');putstr(#NAME);putstr(":0x");put_hex_u8(CC1050_reg_read(NAME));putstr("\n");
void CC1050_print_regs( void )
{
puts("Starting CC1050 register dump");
	DUMP_REG(REG_MAIN); 
	DUMP_REG(REG_FREQ_2A); 
	DUMP_REG(REG_FREQ_1A); 
	DUMP_REG(REG_FREQ_0A); 
	DUMP_REG(REG_FREQ_2B); 
	DUMP_REG(REG_FREQ_1B); 
	DUMP_REG(REG_FREQ_0B);
	DUMP_REG(REG_FSEP1);   
	DUMP_REG(REG_FSEP0); 
	DUMP_REG(REG_CURRENT);
	DUMP_REG(REG_XOSC);  
	DUMP_REG(REG_PA_POW);
	DUMP_REG(REG_PLL);    
	DUMP_REG(REG_LOCK);    
	DUMP_REG(REG_CAL);  
	DUMP_REG(REG_MODEM0);  
	DUMP_REG(REG_FSCTRL);  
	DUMP_REG(REG_PRESCALER);
	DUMP_REG(REG_TEST6);
	DUMP_REG(REG_TEST5);
	DUMP_REG(REG_TEST4);
	DUMP_REG(REG_TEST3);
	DUMP_REG(REG_TEST2);
	DUMP_REG(REG_TEST1);
	DUMP_REG(REG_TEST0);
puts("CC1050 register dump complete");
}


//This serves as an CPU-side shadow
//of the CC1050 main register so that
//we can check which frequency channel(A/B)
//is in use without reading it back, as
//reads are slow.
static uint8_t reg_main;

void CC1050_hop( uint8_t f2, uint8_t f1, uint8_t f0 )
{
//Program next frequency
if( reg_main & REG_MAIN_FREG )
	{
	CC1050_reg_set( REG_FREQ_2A, f2 );
	CC1050_reg_set( REG_FREQ_1A, f1 );
	CC1050_reg_set( REG_FREQ_0A, f0 );
	}
else
	{
	CC1050_reg_set( REG_FREQ_2B, f2 );
	CC1050_reg_set( REG_FREQ_1B, f1 );
	CC1050_reg_set( REG_FREQ_0B, f0 );
	}
//Hop to the new channel
reg_main ^= REG_MAIN_FREG;
CC1050_reg_set( REG_MAIN, reg_main );
}

void CC1050_hop2( uint32_t f )
{
CC1050_hop( (uint8_t)((f>>16) & 0xFF), (uint8_t)((f>>8)&0xFF), (uint8_t)(f&0xFF));
}

void CC1050_init( uint8_t f2, uint8_t f1, uint8_t f0 )
{
delay_millis(100);
CC1050_reg_set( REG_MAIN, 0x1A ); //Put it in reset, most modules in powerdown
CC1050_reg_set( REG_MAIN, 0x1A ); //Do this twice on first boot
CC1050_reg_set( REG_MAIN, 0x1B ); //Take it out of reset
delay_millis(2); //Wait for crystal startup
CC1050_reg_set( REG_FREQ_2A, f2 );
CC1050_reg_set( REG_FREQ_1A, f1 );
CC1050_reg_set( REG_FREQ_0A, f0 );
CC1050_reg_set( REG_FREQ_2B, f2 );
CC1050_reg_set( REG_FREQ_1B, f1 );
CC1050_reg_set( REG_FREQ_0B, f0 );
CC1050_reg_set( REG_CURRENT, 0x81 );
CC1050_reg_set( REG_XOSC, 0 );
CC1050_reg_set( REG_PA_POW, 0 );
CC1050_reg_set( REG_LOCK, 0x10 );
CC1050_reg_set( REG_CAL, 0x66 );
//CC1050_reg_set( REG_MODEM0, 0x13 ); //1200 baud, NRZ, 14.7456MHz xtal
//CC1050_reg_set( REG_MODEM0, 0x23 ); //2400 baud, NRZ, 14.7456MHz xtal
//CC1050_reg_set( REG_MODEM0, 0x33 ); //4800 baud, NRZ, 14.7456MHz xtal
//CC1050_reg_set( REG_MODEM0, 0x43 ); //9600 baud, NRZ, 14.7456MHz xtal
CC1050_reg_set( REG_MODEM0, 0x53 ); //19200 baud, NRZ, 14.7456MHz xtal
//CC1050_reg_set( REG_MODEM0, 0x73 ); //76800 baud, NRZ, 14.7456MHz xtal
//CC1050_reg_set( REG_MODEM0, 0x18 ); //1200 baud, UART, 14.7456MHz xtal
CC1050_reg_set( REG_FSCTRL, 0x1 );
//CC1050_reg_set( REG_PRESCALER, 0x80 ); //Default
CC1050_reg_set( REG_PRESCALER, 0x60 );

CC1050_reg_set( REG_TEST6, 0x10 );
CC1050_reg_set( REG_TEST5, 0x08 );
//CC1050_reg_set( REG_TEST4, 0x3F ); //For Baud Rates up to 76800
//CC1050_reg_set( REG_TEST4, 0x25 ); //For Baud Rates up to 19200
CC1050_reg_set( REG_TEST4, 0x12 ); //Undocumented but should be lower bandwidth
//CC1050_reg_set( REG_TEST4, 0x09 ); //Undocumented but should be lower bandwidth
//CC1050_reg_set( REG_TEST4, 0x05 ); //Undocumented but should be lower bandwidth
//CC1050_reg_set( REG_TEST4, 0x03 ); //Undocumented but should be lower bandwidth
CC1050_reg_set( REG_TEST3, 0x04 );
CC1050_reg_set( REG_TEST2, 0x00 );
CC1050_reg_set( REG_TEST1, 0x00 );
CC1050_reg_set( REG_TEST0, 0x00 );
CC1050_reg_set( REG_FSEP1, 0x00 );
CC1050_reg_set( REG_FSEP0, 26 ); //Roughly 3kHz deviation
CC1050_reg_set( REG_PLL,   0x40 );//REFDIV=8

//Initial programming complete

//Do Calibration Here
CC1050_reg_set( REG_CAL, 0x66 ); //Set CAL-DUAL
CC1050_reg_set( REG_MAIN, 1 );   
CC1050_reg_set( REG_CURRENT, 0x81 );
CC1050_reg_set( REG_PA_POW, 0 );
CC1050_reg_set( REG_CAL, 0xE6 ); //Set CAL-START
delay_millis(26);
CC1050_reg_set( REG_CAL, 0x66 ); //Clear CAL-START

CC1050_reg_set( REG_MAIN, 0x1F); //Powerdown
}

void CC1050_init2( uint32_t f )
{
CC1050_init( (uint8_t)((f>>16) & 0xFF), (uint8_t)((f>>8)&0xFF), (uint8_t)(f&0xFF));
}

void CC1050_tx_enable( void )
{
CC1050_reg_set( REG_MAIN, 0x1B );
delay_millis(2);//Wait for crystal startup
CC1050_reg_set( REG_MAIN, 0x19 );
delay_micros(200); //Wait for bias generator
CC1050_reg_set( REG_PA_POW, 0 );
CC1050_reg_set( REG_MAIN, 0x11 );
delay_micros(250); //Wait for PLL to lock
CC1050_reg_set( REG_PA_POW, 0xFF ); //PA to full power
delay_micros(20); //Wait for PA to stabilize
CC1050_reg_set( REG_MAIN, 0x01 );
}

void CC1050_tx_disable( void )
{
CC1050_reg_set( REG_PA_POW, 0 );
CC1050_reg_set( REG_MAIN, 0x1F );
}

void CC1050_reg_set( uint8_t reg_addr, uint8_t byte )
{
//putstr("Setting 0x");put_hex_u8(reg_addr);putstr(" to 0x");put_hex_u8(byte);putstr("\n");
GPIO_CLR( GPIO_CC1050_PALE_PORT, GPIO_CC1050_PALE_PIN );
SPI_tx_byte( (uint8_t)(( reg_addr << 1 ) | 1 ) );//1 LSB means WRITE
GPIO_SET( GPIO_CC1050_PALE_PORT, GPIO_CC1050_PALE_PIN );
if( reg_addr == REG_MAIN )
	{
	reg_main = byte;
	}
SPI_tx_byte( byte );
}


static uint8_t readbit( void )
{
uint8_t retn = GPIO_GET( GPIO_CC1050_PDATA_PORT, GPIO_CC1050_PDATA_PIN );
GPIO_CLR( GPIO_CC1050_PCLK_PORT, GPIO_CC1050_PCLK_PIN );
delay_micros(1);
GPIO_SET( GPIO_CC1050_PCLK_PORT, GPIO_CC1050_PCLK_PIN );
delay_micros(1);
return retn;
}

static void writebit( uint8_t byte, uint8_t bit )
{
if( byte & (1<<bit))
	{
	GPIO_SET( GPIO_CC1050_PDATA_PORT, GPIO_CC1050_PDATA_PIN );
	}
else
	{
	GPIO_CLR( GPIO_CC1050_PDATA_PORT, GPIO_CC1050_PDATA_PIN );
	}
delay_micros(1);
GPIO_CLR( GPIO_CC1050_PCLK_PORT, GPIO_CC1050_PCLK_PIN );
delay_micros(1);
GPIO_SET( GPIO_CC1050_PCLK_PORT, GPIO_CC1050_PCLK_PIN );
delay_micros(1);
}


uint8_t CC1050_reg_read( uint8_t reg_addr )
{
uint8_t retn;
signed char bit;
reg_addr <<= 1;//Form 8-bit address for a read

SPICR &= (uint8_t)~(1<<6);//Disable the SPI, PDATA/PCLK are now GPIO outputs

GPIO_CLR( GPIO_CC1050_PALE_PORT, GPIO_CC1050_PALE_PIN );
delay_micros(1);
for( bit = 7; bit >= 0; --bit )
	{
	writebit( reg_addr, bit);
	}


//Set PDATA as an input
PCDDR &= (uint8_t)~GPIO_CC1050_PDATA_BIT;
//PCOR  &= ~GPIO_CC1050_PDATA_BIT;

GPIO_SET( GPIO_CC1050_PALE_PORT, GPIO_CC1050_PALE_PIN );

delay_micros(1);
retn = 0;
for( bit = 7; bit >= 0; --bit )
	{
	retn |= (uint8_t)( readbit() << bit );
	}

//Set PDATA back to an output
PCDDR |= GPIO_CC1050_PDATA_BIT;
//PCOR  |= GPIO_CC1050_PDATA_BIT;

SPICR = SPICR_INIT;//Re-enable the SPI

return retn;
}

