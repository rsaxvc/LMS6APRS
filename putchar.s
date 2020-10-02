;	Soft UART-TX
;	Copyright (c) 2020 by SecKC
;

;	switch	.ubsct
;value: dc.b 1 ; fills .data section

xdef	_putchar
;
;	putc routine to bit-bang an 8N1 software UART
;
;This algorithm is highly ST7-specific
;
;This implementation relies on the following instruction timings:
;    NOP2 - 2 cycles - Regular NOP
;    NOP3 - 3 cycles - Custom slower NOP
;    NOP5 - 5 cycles - Combo NOP
;    NOP6 - 6 cycles - Combo NOP
;    BRES - 5 cycles - Bit Reset
;    BSET - 5 cycles - Bit Set
;    BCP  - 2 cycles - Bit Compare
;    JREQ - 3 cycles - Conditional Jump
;    JRT  - 3 cycles - Jump
;

;Utility NOPs
nop2: macro
        nop
	endm

nop3: macro
        jra \@macroret
\@macroret:
        endm

nop5: macro
        nop2
	nop3
        endm

nop6: macro
        nop3
	nop3
        endm


;SENDBIT MACROS
;All sendBit/sendStartBit/sendStopBit invocations must
;take the same number of cycles so the waveform stays
;stays stable over the byte(10 bits for 8N1). The baud
;rate is determined by Fcpu / (number of cycles per bit)
;
;GPIO-write must always occur on the same cycle
;(currently assumed to start on cycle 6 of each
;bit) to ensure no jitter between bit-edges.
;
;This particular implementation implements 500kBaud
;running on an 8MHz ST7.
;
sendBit: macro ;ARGS: BankRegister, BankPin, BitNum
        bcp     a, #\3      ;2 cycles
        jreq    \@pos       ;3 cycles
        bset    \1, #\2     ;5 cycles
	nop3                ;3 cycles 
        jra     \@macroret  ;3 cycles
\@pos:
        bres    \1, #\2     ;5 cycles
        nop6                ;6 cycles
\@macroret:
	endm

sendStartBit: macro ;ARGS: BankRegister, BankPin
	nop5                ;5 cycles
	bres \1, #\2        ;5 cycles
	nop6                ;6 cycles
	endm
sendStopBit: macro ;ARGS: BankRegister, BankPin
	nop5                ;5 cycles
	bset \1, #\2        ;5 cycles
	nop6                ;6 cycles
	endm

#define PADR 0x0
#define PBDR 0x3
#define PCDR 0x6
#define PDADR 0x9
#define PEDR 0xC
#define PFDR 0xF

#define PORT_REG PBDR
#define PIN 4

	switch	.text

_putchar:

;	ld           value,a                   ;Load first COSMIC-C parameter into accumulator(it's passed in x)
	sim                                    ;Disable interrupts while we transmit a byte, this is why baud should be as high as possible
	sendStartBit PORT_REG, PIN             ;Drop GPIO low to send start bit
	sendBit      PORT_REG, PIN, 0x01       ;Send Bit0
	sendBit      PORT_REG, PIN, 0x02       ;Send Bit1
	sendBit      PORT_REG, PIN, 0x04       ;Send Bit2
	sendBit      PORT_REG, PIN, 0x08       ;Send Bit3
	sendBit      PORT_REG, PIN, 0x10       ;Send Bit4
	sendBit      PORT_REG, PIN, 0x20       ;Send Bit5
	sendBit      PORT_REG, PIN, 0x40       ;Send Bit6
	sendBit      PORT_REG, PIN, 0x80       ;Send Bit7
	sendStopBit  PORT_REG, PIN             ;Raise GPIO high to send end bit
	rim                                    ;Restore interrupts
	ret

	end
