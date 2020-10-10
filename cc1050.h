/*	CC1050 management
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */
#ifndef CC1050_H
#define CC1050_h

#include <stdint.h>

void CC1050_reg_set( uint8_t reg_addr, uint8_t byte );
uint8_t CC1050_reg_read( uint8_t reg_addr );
void CC1050_init( uint8_t f2, uint8_t f1, uint8_t f0 );
void CC1050_init2( uint32_t f );
void CC1050_hop( uint8_t f2, uint8_t f1, uint8_t f0 );
void CC1050_hop2( uint32_t f );
void CC1050_tx_enable( void );
void CC1050_tx_disable( void );
void CC1050_print_regs( void );
#endif