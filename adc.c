/*	Example of ADC read
 *	Copyright (c) 2000 by COSMIC Software
 */
#include "adc.h"
#include "asmtools.h"
#include "delay.h"
#include "gpio_def.h"

#define ADON	0x20	// AD Converter On
#define COCO	0x80	// Conversion Complete

static void configure_trigger_and_convert( enum adc_channel channel )
{
uint8_t temp;
SIM();
temp = PBDR;
temp &= (uint8_t)(~(GPIO_ADC_MUX_SEL0_BIT|GPIO_ADC_MUX_SEL1_BIT|GPIO_ADC_MUX_SEL2_BIT));
temp |= channel;
PBDR = temp;
RIM();
delay_micros(100); //Allow Analog mux to settle, this time not measured on scope
ADCCSR = ADON | 8;		// enable input from PF0/AIN8, this appears to be the only used ST7 ADC channel, all muxing done by external mux
while (!(ADCCSR & COCO));	// wait for conversion completed
}

uint8_t read_adc8( enum adc_channel channel )
{
configure_trigger_and_convert(channel);
return (ADCDRH);
}

uint16_t read_adc10( enum adc_channel channel )
{
configure_trigger_and_convert(channel);
return (uint16_t)ADCDRH << 2 | ADCDRL;
}

