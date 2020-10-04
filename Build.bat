cxst7 -l +modms +debug -itsip_parser cc1050.c crts.s delay.c putchar.s lsm6aprs.c init.c adc.c interrupt.c vector.c spilib.c tasks.c tsip_parser\tsip_parser.c
clnk -o lsm6aprs.st7 lsm6aprs.lkf
cvdwarf lsm6aprs.st7
