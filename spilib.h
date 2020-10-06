/*	Simple SPI management
 *	Copyright (c) 2020 by SECKC Software
 */
#ifndef SPILIB_H
#define SPILIB_H

//#define SPICR_INIT 0x5A //SPiEnable | MstrMode | 125KHz | CPOL = 1 | CPHA = 0
//#define SPICR_INIT 0x7A //SPiEnable | MstrMode | 250KHz | CPOL = 1 | CPHA = 0
#define SPICR_INIT 0x58 //SPiEnable | MstrMode | 1.0MHz | CPOL = 1 | CPHA = 0
//#define SPICR_INIT 0x78 //SPiEnable | MstrMode | 2.0MHz | CPOL = 1 | CPHA = 0

void SPI_init( void );
void SPI_tx_byte( unsigned char byte );
unsigned char SPI_rx_byte( void );

#endif
