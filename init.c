/*	LSM6APRS Port Initialization
 */
#include <stdio.h>
#include "stdio2.h"
#include "gpio_def.h"

#define PORT_B_OUTPUTS (0)
#define PORT_C_OUTPUTS (GPIO_CC1050_PALE_BIT|GPIO_CC1050_PCLK_BIT|GPIO_CC1050_PDATA_BIT)
#define PORT_D_OUTPUTS (GPOI_CC1050_LDO_BIT)
#define PORT_F_OUTPUTS (GPIO_SOFTUART_TX_BIT|GPIO_CC1050_DI_BIT)

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
	PDDDR = PORT_D_OUTPUTS;
	//PDOR unprogrammed just like original firmware
	
	PFDDR = PORT_F_OUTPUTS;
	PFOR  = PORT_F_OUTPUTS;
	
	//TODO: split these up into modules and control as needed.
	GPIO_CLR( GPIO_CC1050_LDO_PORT,  GPOI_CC1050_LDO_BIT );
	GPIO_SET( GPIO_SOFTUART_TX_PORT, GPIO_SOFTUART_TX_PIN );
	GPIO_SET( GPIO_CC1050_PALE_PORT, GPIO_CC1050_PALE_PIN );
	GPIO_SET( GPIO_CC1050_PCLK_PORT, GPIO_CC1050_PCLK_PIN );
	}

void timer_init(void)
	{
	//TBD: What will we use these timers for?
	//TBD: How will we use them? Module-specific or centralized?
	//Example code from COSMIC:
	//ATCSR = 0x13;		// enable interrupts
	//ATR = 0;		// auto-reload value
	}

void print_gpio_states( void )
{
puts("Dumping GPIO states\r\n");
#define DUMP_REG(NAME)  putchar('\t');puts(#NAME);puts(":0x");puts_hex_u8(NAME);puts("\r\n");
DUMP_REG(PADDR);DUMP_REG(PAOR);DUMP_REG(PADR);
DUMP_REG(PBDDR);DUMP_REG(PBOR);DUMP_REG(PBDR);
DUMP_REG(PCDDR);DUMP_REG(PCOR);DUMP_REG(PCDR);
DUMP_REG(PDDDR);DUMP_REG(PDOR);DUMP_REG(PDDR);
DUMP_REG(PEDDR);DUMP_REG(PEOR);DUMP_REG(PEDR);
DUMP_REG(PFDDR);DUMP_REG(PFOR);DUMP_REG(PFDR);
puts("Done dumping GPIO states\r\n");
}