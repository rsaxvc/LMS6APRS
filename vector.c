/*	INTERRUPT VECTORS TABLE FOR ST72F324J6T6
 *	Copyright (c) 2020 by SECKC Software
 *
 *  THIS FILE IS MAGIC - it is located
 *  at address 0xffe4 by the linker. So,
 *  it is important that this file contain
 *  a single _vectab, as anything else could
 *  cause _vectab to be located at the wrong
 *  address
 */

#include "interrupt.h"
#include "gps.h"
extern void _stext(); /*Reset/startup entry point*/
#define UNUSED	bad_interrupt /* map unhandled interrupts to a loop for debugging*/

void (* const _vectab[])() = {
	UNUSED,			/* 0xFFE4 - AVD     */
	GPS_interrupt,	/* 0xFFE6 - SCI     */
	UNUSED,			/* 0xFFE8 - Timer B */
	UNUSED,			/* 0xFFEA - Timer A */
	UNUSED,			/* 0xFFEC - SPI     */
	UNUSED,			/* 0xFFEE - unused  */
	UNUSED,			/* 0xFFF0 - EI3     */
	UNUSED,			/* 0xFFF2 - EI2     */
	UNUSED,			/* 0xFFF4 - EI1     */
	ei0_interrupt,	/* 0xFFF6 - EI0     */
	UNUSED,			/* 0xFFF8 - MCC/RCC */
	UNUSED,			/* 0xFFFA - unused  */
	UNUSED,			/* 0xFFFC - TRAP    */
	_stext,			/* 0xFFFE - RESET   */
	};
