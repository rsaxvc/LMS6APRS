/*	Example of ADC read
 *	Copyright (c) 2000 by COSMIC Software
 */
#include <io7flit0.h>

#define ADON	0x20	// AD Converter On
#define COCO	0x80	// Conversion Complete

/*	Read input value on Channel 3 (PC3)
 */
unsigned char read_adc(void)
	{
	ADCCSR = ADON | 3;		// enable input from PB3 
	while (!(ADCCSR & COCO))	// wait for conversion completed
		;
	return (ADCDAT);		// return value
	}

