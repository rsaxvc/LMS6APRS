/*	LSM6APRS Port Initialization
 */
#include <io7flit0.h>

void port_init(void)
	{
	//DO NOT SET ANYTHING TO OUTPUT UNTIL WE HAVE A PLAUSIBLE SCHEMATIC
	//Example code from COSMIC:
	//PADDR = 0x1F;	// PORTA: bits 0..4 output
	//PAOR  = 0x1F;	//        push-pull
	//PADR  = 0x00;	//        clear output
	//PBDDR = 0x07;	// PORTB: bits 0..2 output
	//PBOR  = 0x07;	//        push-pull
	//PBDR  = 0x00;	//        clear output
	}

void timer_init(void)
	{
	//TBD: What will we use these timers for?
	//TBD: How will we use them? Module-specific or centralized?
	//Example code from COSMIC:
	//ATCSR = 0x13;		// enable interrupts
	//ATR = 0;		// auto-reload value
	}

