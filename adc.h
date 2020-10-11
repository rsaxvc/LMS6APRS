/*	Example of ADC read
 *	Copyright (c) 2000 by COSMIC Software
 */
#ifndef ADC_H
#define ADC_H

#include <stdint.h>

enum adc_channel
	{
	ADC_CHANNEL_0,
	ADC_CHANNEL_1,
	ADC_CHANNEL_2,
	ADC_CHANNEL_3,
	ADC_CHANNEL_4,
	ADC_CHANNEL_5,
	ADC_CHANNEL_6,
	ADC_CHANNEL_7
	};
 
uint8_t read_adc8( enum adc_channel channel );
uint16_t read_adc10( enum adc_channel channel );

#endif