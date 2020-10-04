/*	LSM6APRS Port Initialization
 */
#include <io72324.h>
#include "gpio_def.h"

#define PORT_B_OUTPUTS (GPIO_SOFTUART_TX_BIT)
#define PORT_C_OUTPUTS (GPIO_CC1050_PALE_BIT)

void port_init(void)
	{
	//DO NOT SET ANYTHING TO OUTPUT UNTIL WE HAVE A PLAUSIBLE SCHEMATIC
	//Example code from COSMIC:
	//PADDR = 0x1F;	// PORTA: bits 0..4 output
	//PAOR  = 0x1F;	//        push-pull
	//PADR  = 0x00;	//        clear output
	PBDDR = PORT_B_OUTPUTS;	// PORTB: bits 0..2 output
	PBOR  = PORT_B_OUTPUTS;	//        push-pull
	//PBDR  = 0x00;	//        clear output
	PCDDR = PORT_C_OUTPUTS;
	PCOR  = PORT_C_OUTPUTS;
	}

void timer_init(void)
	{
	//TBD: What will we use these timers for?
	//TBD: How will we use them? Module-specific or centralized?
	//Example code from COSMIC:
	//ATCSR = 0x13;		// enable interrupts
	//ATR = 0;		// auto-reload value
	}
