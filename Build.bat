cxst7 -l +warn +modms +debug -itsip_parser adc.c afsk_mod.c cc1050.c crts.s delay.c gps.c putchar.s puts.c lms6aprs.c init.c interrupt.c morse.c morse_table.c vector.c spilib.c tasks.c tsip_parser\tsip_conversions.c tsip_parser\tsip_parser.c
clnk -o lms6aprs.st7 lms6aprs.lkf
cvdwarf lms6aprs.st7
chex -o lms6aprs.s19 lms6aprs.st7
