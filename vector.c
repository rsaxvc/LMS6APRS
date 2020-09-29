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
extern void _stext(); /*Reset/startup entry point*/
#define UNUSED	bad_interrupt /* map unhandled interrupts to a loop for debugging*/

void (* const _vectab[])() = {
	UNUSED,			/* AVD     */
	UNUSED,			/* SCI     */
	UNUSED,			/* Timer B */
	UNUSED,			/* Timer A */
	UNUSED,			/* SPI     */
	UNUSED,			/* unused  */
	UNUSED,			/* EI3     */
	UNUSED,			/* EI2     */
	UNUSED,			/* EI1     */
	UNUSED,			/* EI0     */
	UNUSED,			/* MCC/RCC */
	UNUSED,			/* unused  */
	UNUSED,			/* TRAP    */
	_stext,			/* RESET   */
	};
