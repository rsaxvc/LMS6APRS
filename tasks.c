/*	Simple example tasks
 *	Copyright (c) 2020 by SECKC Software
 */
#include <stdio.h>
#include "stdio2.h"
#include "tasks.h"
#include "gps.h"
#include "tsip_parser.h"
#include "cc1050.h"
#include "aprs.h"
#include "afsk_mod.h"


void task_aprs_tx( void )
{
puts("Preparing AFSK");
aprs_send();
CC1050_tx_enable();
//At this point ei0, the modulation interrupt, is enabled.
//We must not disabled interrupts until afsk_busy() returns
//false. Of note, the standard output functions like puts
//also disable interrupts for about 20 microseconds per
//character, which may goober up the transmit interrupt timing,
//so make sure not to call those here either.
while( afsk_busy() );
puts("Done");
CC1050_tx_disable();
}
