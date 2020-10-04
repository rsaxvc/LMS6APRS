/*	CC1050 management
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */

#include "cc1050.h"
#include "spilib.h"
#include "gpio_def.h"
#include <io72324.h>

enum
	{
	REG_MAIN    = 0,
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

static unsigned char reg_main;

void CC1050_hop( unsigned char f2, unsigned char f1, unsigned char f0 )
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
//Hop
CC1050_reg_set( REG_MAIN, reg_main ^ REG_MAIN_FREG );
}

void CC1050_init( unsigned f2, unsigned f1, unsigned f0 )
{
GPIO_SET( GPIO_CC1050_PALE_PORT, GPIO_CC1050_PALE_PIN );
delay_millis(10);
CC1050_reg_set( REG_MAIN, 0x1A );   //Put it in reset, most modules in powerdown
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
//CC1050_reg_set( REG_MODEM0, 0x33 ); //4800 baud, NRZ, 14.7456MHz xtal
CC1050_reg_set( REG_MODEM0, 0x43 ); //9600 baud, NRZ, 14.7456MHz xtal
//CC1050_reg_set( REG_MODEM0, 0x53 ); //19200 baud, NRZ, 14.7456MHz xtal
CC1050_reg_set( REG_FSCTRL, 0x1 );
CC1050_reg_set( REG_PRESCALER, 0x80 );
CC1050_reg_set( REG_TEST6, 0x10 );
CC1050_reg_set( REG_TEST5, 0x08 );
CC1050_reg_set( REG_TEST4, 0x25 );
CC1050_reg_set( REG_TEST3, 0x04 );
CC1050_reg_set( REG_TEST2, 0x00 );
CC1050_reg_set( REG_TEST1, 0x00 );
CC1050_reg_set( REG_TEST0, 0x00 );
CC1050_reg_set( REG_FSEP1, 0x00 );
CC1050_reg_set( REG_FSEP0, 0x00 );
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

void CC1050_tx_enable( void )
{
CC1050_reg_set( REG_MAIN, 0x1B );
delay_millis(2);
CC1050_reg_set( REG_MAIN, 0x19 );
delay_micros(200);
CC1050_reg_set( REG_PA_POW, 0 );
CC1050_reg_set( REG_MAIN, 0x11 );
delay_micros(250);
CC1050_reg_set( REG_MAIN, 0x01 );
CC1050_reg_set( REG_PA_POW, 0xFF );
delay_micros(20);
}

void CC1050_tx_disable( void )
{
CC1050_reg_set( REG_PA_POW, 0 );
CC1050_reg_set( REG_MAIN, 0x1F );
}

void CC1050_reg_set( unsigned char reg_addr, unsigned byte )
{
GPIO_CLR( GPIO_CC1050_PALE_PORT, GPIO_CC1050_PALE_PIN );
SPI_tx_byte( ( reg_addr << 7 ) | 1 );
GPIO_SET( GPIO_CC1050_PALE_PORT, GPIO_CC1050_PALE_PIN );
if( reg_addr = REG_MAIN )
	{
	reg_main = byte;
	}
SPI_tx_byte( byte );
}
