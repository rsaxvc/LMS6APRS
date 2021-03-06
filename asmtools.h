/*	Assembly tools for C 
 *	Copyright (c) 2020 by SECKC Software
 */
#ifndef ASMTOOLS_H
#define ASMTOOLS_H

/*	Macros
 */
#define NOP() _asm("NOP") //No-op, 2 cycle on ST7
#define SIM() _asm("SIM") //Block interrupts
#define RIM() _asm("RIM") //Unblock interrupts
#define WFI() _asm("WFI") //Wait for interrupt

#define NOP4() NOP();NOP();NOP();NOP()
#define NOP16() NOP4();NOP4();NOP4();NOP4()
#define NOP64() NOP16();NOP16();NOP16();NOP16()
#define NOP256() NOP64();NOP64();NOP64();NOP64()
#endif
