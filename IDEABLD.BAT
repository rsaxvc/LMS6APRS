REM       COMMAND FILE BUILT BY IDEA 
REM       Wed Sep 30 23:26:34 2020
REM
cast7 -xx -x -i"C:\Program Files (x86)\COSMIC\EVAL_ST7\HST7" "putchar.s"
cast7 -xx -x -i"C:\Program Files (x86)\COSMIC\EVAL_ST7\HST7" "crts.s"
cxst7 -vl +debug +modm -wa196994 -wb1224 -i"C:\Program Files (x86)\COSMIC\EVAL_ST7\HST7" "tasks.c" "adc.c" "interrupt.c" "init.c" "vector.c" "lsm6aprs.c"
clnk -o "lsm6aprs.st7" -m "lsm6aprs.map" -l"C:\Program Files (x86)\COSMIC\EVAL_ST7\Lib" "lsm6aprs.lkf"
clabs "lsm6aprs.st7"
cvdwarf "lsm6aprs.st7"
