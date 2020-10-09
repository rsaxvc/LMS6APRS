/*	Simple SPI management
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */

#include "spilib.h"
#include "gpio_def.h"

void SPI_init( void )
{
SPICSR = 0;
SPICR = SPICR_INIT;
}

void SPI_tx_byte( uint8_t byte )
{
SPICSR; //Read SPICSR to reset SPIF
SPIDR; //Read SPIDR to clear it out
SPIDR = byte;
while( ( SPICSR & ( 1<<7 ) ) == 0 ); //Wait for SPIF to signal TX complete
}
