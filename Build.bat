cxst7 -l +modm +debug crts.s lsm6aprs.c init.c adc.c interrupt.c vector.c stdio.c tasks.c
clnk -o lsm6aprs.st7 lsm6aprs.lkf
cvdwarf lsm6aprs.st7
