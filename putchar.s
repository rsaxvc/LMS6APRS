;	Soft UART-TX
;	Copyright (c) 2020 by SecKC
;

xdef	_putchar
;
;	putc routine to bit-bang an 8N1 software UART
;
;This algorithm is highly ST7-specific
;
;SENDBIT MACROS
;All sendBit/sendStartBit/sendStopBit invocations must
;take the same number of cycles so the waveform stays
;stays stable over the byte(10 bits for 8N1). The baud
;rate is determined by Fcpu / (number of cycles per bit)
;
;GPIO-write must always occur on the same cycle
;(currently assumed to be instruction 3) to ensure
;no jitter between bit-edges.
;
;This implementation assumes that each instruction takes
;2 cycles, if this is not true the algorithm will need
;to be adjusted once hardware is available. Of note,
;the available documentation says instructions take
;2-12 cycle, which is a pretty large spread.
;
;This has not been tested.

sendBit: macro ;ARGS: BankRegister, BankPin, BitNum
        bcp     a, #\3
        jreq    \@pos
        bset    \1, #\2
        jra     \@macroret
\@pos:
        bres    \1, #\2
        nop
\@macroret:
	endm

sendStartBit: macro ;ARGS: BankRegister, BankPin
	nop
	nop
	bres \1, #\2
	nop
	endm
sendStopBit: macro ;ARGS: BankRegister, BankPin
	nop
	nop
	bset \1, #\2
	nop
	endm

	switch	.text

_putchar:

	ld      a, x             ;Load first COSMIC-C parameter into accumulator(it's passed in x)
	sim                      ;Disable interrupts while we transmit a byte, this is why baud should be as high as possible
	sendStartBit 0, 0        ;Drop GPIO low to send start bit
	sendBit 0, 0, 0          ;Send Bit0
	sendBit 0, 0, 1          ;Send Bit1
	sendBit 0, 0, 2          ;Send Bit2
	sendBit 0, 0, 3          ;Send Bit3
	sendBit 0, 0, 4          ;Send Bit4
	sendBit 0, 0, 5          ;Send Bit5
	sendBit 0, 0, 6          ;Send Bit6
	sendBit 0, 0, 7          ;Send Bit7
	sendStopBit 0, 0	 ;Raise GPIO high to send end bit
	rim                      ;Restore interrupts
	ret

	end
