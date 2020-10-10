/*	Example Interrupt Handlers
 *	Copyright (c) 2020 by SECKC Software
 */
#include <stdint.h>
#include <stdio.h>
#if defined(__CSMC__)
	#include "asmtools.h"
	#include "gpio_def.h"
	#include "interrupt.h"
	#include "stdio2.h"
	#define INTERRUPT @interrupt
#else
	#include <stdio.h>
	#define INTERRUPT
#endif

#pragma space extern [] @tiny

/* Generated by the following python script:
import math
for t in xrange(0,256):
	print int(64 * math.sin(t * 2 * math.pi / 256 )),",",
*/
#define WAVETABLE_AMPLITUDE 64
static const int8_t wavetable[]=
{
#include "wavetable.csv"
};


#define SYMBOL_RATE 1200        /**< AFSK symbol rate (baud) */
#define FREQ_MARK   1200        /**< AFSK mark tone frequency (Hz) */
#define FREQ_SPACE  2200        /**< AFSK space tone frequency (Hz) */
#define SAMPLES_PER_SYMBOL  16

/** Number of fractional bits for phase computation */
#define FRAC                6

/* Compute useful constants */
#define PHASE_MAX           ((uint16_t)sizeof(wavetable) << FRAC)
#define PHASE_DELTA_MARK    (uint16_t)((uint32_t)PHASE_MAX * FREQ_MARK / SYMBOL_RATE / SAMPLES_PER_SYMBOL)
#define PHASE_DELTA_SPACE   (uint16_t)((uint32_t)PHASE_MAX * FREQ_SPACE / SYMBOL_RATE / SAMPLES_PER_SYMBOL) 

#if defined(__CSMC__)
    #define OUTPUT1() GPIO_SET( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN )
    #define OUTPUT0() GPIO_CLR( GPIO_CC1050_DI_PORT, GPIO_CC1050_DI_PIN )
#else
    #define OUTPUT1() puts("1,")
    #define OUTPUT0() puts("0,")
#endif

static uint8_t *txBuffer;
static uint8_t  txBitMask;
static uint16_t txBitsToSend;
static uint16_t txSampleInSymbol;
static uint16_t txPhase;
static uint16_t txPhaseDelta;
static int8_t  txQuantError;

void afsk_send(const uint8_t *message, uint16_t lengthInBits)
{
txBuffer = message;
txBitsToSend = lengthInBits;

txPhase = 0;
txPhaseDelta = PHASE_DELTA_SPACE;

txBitMask = 1;  /* LSB first */
txSampleInSymbol = 0;

txQuantError = wavetable[0];

#if defined(__CSMC__)
SIM();
//EICR = 0x10;  //Enable external Interrupt 0, Falling Edge
EICR = 0x08;  //Enable external Interrupt 0, Rising Edge
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
int8_t sample;
uint16_t txPhaseTemp;

/* We get here 19200 times per second or 16 times per symbol */
if (txSampleInSymbol == 0) {
	/* Load new symbol (bit) to transmit */
	if (0 == (*txBuffer & txBitMask)) {
		/* Logical 0 - toggle the AFSK frequency */
		txPhaseDelta ^= (PHASE_DELTA_MARK ^ PHASE_DELTA_SPACE);
	}
	
	/* Update bit extraction mask */
	if (txBitMask == 0x80) {
		/* Whole byte was processed, move to the next one */
		txBitMask = 1;
		txBuffer++;
	} else {
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

txSampleInSymbol++;
if (txSampleInSymbol >= SAMPLES_PER_SYMBOL) {
	txSampleInSymbol = 0;
}

/* Update PWM amount and increment phase */
sample = (int8_t)(wavetable[txPhase >> FRAC]);
sample += txQuantError;
txPhaseTemp = (uint16_t)(txPhase + txPhaseDelta);
if( txPhaseTemp >= PHASE_MAX )
	{
	txPhase = txPhaseTemp - PHASE_MAX;
	}
else
	{
	txPhase = txPhaseTemp;
	}

if( sample > 0 )
	{
	OUTPUT1();
	txQuantError = (int8_t)(sample - WAVETABLE_AMPLITUDE);
	}
else
	{
	OUTPUT0();
	txQuantError = (int8_t)(sample + WAVETABLE_AMPLITUDE);
    }
}