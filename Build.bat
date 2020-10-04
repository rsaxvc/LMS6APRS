cxst7 -l +modms +debug -itsip_parser cc1050.c crts.s delay.c putchar.s lms6aprs.c init.c adc.c interrupt.c vector.c spilib.c tasks.c tsip_parser\tsip_parser.c
clnk -o lms6aprs.st7 lms6aprs.lkf
cvdwarf lms6aprs.st7
