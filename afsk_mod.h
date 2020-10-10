/*	Example Interrupt Handlers
 *	Copyright (c) 2020 by SECKC Software
 */
#ifndef AFSK_MOD_H
#define AFSK_MOD_H
@interrupt void ei0_interrupt(void);

void afsk_send(const uint8_t *message, uint16_t lengthInBits);
uint8_t afsk_busy( void );

#endif
