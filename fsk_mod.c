#include "config.h"
#if MOD_FSK
#include <stdint.h>
#include <stdio.h>
#if defined(__CSMC__)
	#include "asmtools.h"
	#include "gpio_def.h"
	#include "interrupt.h"
	#include "stdio2.h"
	#define INTERRUPT @interrupt
#else
	#error "unsupported compiler"
#endif

#pragma space extern [] @tiny

static uint8_t *txBuffer;
static uint8_t  txBitMask;
static uint16_t txBitsToSend;

void afsk_send(const uint8_t *message, uint16_t lengthInBits)
{
txBuffer = message;
txBitsToSend = lengthInBits;

txBitMask = 1;  /* LSB first */

GPIO_CLR( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN );

#if defined(__CSMC__)
SIM();
EICR = 0x10;  //Enable external Interrupt 0, Falling Edge
PAOR |= 0x08; //Turn PortA.3 into an interrupt pin
ITSPR2 = 0xC0;//ei0, the CC1050 modulation interrupt, must be highest to prevent GPS IRQ from causing timing jitter
RIM();
#endif
}

uint8_t afsk_busy( void )
{
return (uint8_t)(txBitsToSend > 0);
}

INTERRUPT void ei0_interrupt(void)
{
/* Load new symbol (bit) to transmit */
if( *txBuffer & txBitMask )
	{
	GPIO_SET( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN );
	}
else
	{
	GPIO_CLR( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN );
	}

/* Update bit extraction mask */
if (txBitMask == 0x80)
	{
	/* Whole byte was processed, move to the next one */
	txBitMask = 1;
	txBuffer++;
	}
else
	{
	/* LSB first */
	txBitMask <<= 1;
	}

txBitsToSend--;
if( txBitsToSend == 0 )
	{
	//Shut off the interrupt
	PAOR &= (uint8_t)~0x08; //Turn PortA.3 into not an interrupt pin
	}
}
#endif