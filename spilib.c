/*	Simple SPI management
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */

#include "spilib.h"
#include <io72324.h>

void SPI_init( void )
{
SPICSR = 0;
SPICR = 0x5A;//SPiEnable | MstrMode | 125KHz | CPOL = 1 | CPHA = 0
SPICR = 0x7A;//SPiEnable | MstrMode | 4.0MHz | CPOL = 1 | CPHA = 0
}

void SPI_tx_byte( unsigned char byte )
{
SPICSR; //Read SPICSR to reset SPIF
SPIDR; //Read SPIDR to clear it out
SPIDR = byte;
while( SPICSR & ( 1<<7) ); //Wait for SPIF to signal TX complete
}