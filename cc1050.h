/*	CC1050 management
 *	Copyright (c) 2020 by SECKC Software
 *
 *	This program monitors GPS and the weather and
 *	transmits reports over APRS over AX.25
 */
#ifndef CC1050_H
#define CC1050_h

void CC1050_reg_set( unsigned char reg_addr, unsigned char byte );
unsigned char CC1050_reg_read( unsigned char reg_addr );
void CC1050_init( unsigned char f2, unsigned char f1, unsigned char f0 );
void CC1050_hop( unsigned char f2, unsigned char f1, unsigned char f0 );
void CC1050_hop2( unsigned long f );
void CC1050_tx_enable( void );
void CC1050_tx_disable( void );
void CC1050_print_regs( void );
#endif