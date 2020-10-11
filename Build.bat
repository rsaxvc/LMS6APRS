cxst7 -l +warn +modms +debug -itsip_parser adc.c afsk_mod.c aprs.c ax25.c cc1050.c crts.s delay.c gps.c init.c interrupt.c lms6aprs.c morse.c morse_table.c putchar.s spilib.c stdio.c tasks.c vector.c tsip_parser\tsip_conversions.c tsip_parser\tsip_parser.c
clnk -o lms6aprs.st7 lms6aprs.lkf
cvdwarf lms6aprs.st7
chex -o lms6aprs.s19 lms6aprs.st7
