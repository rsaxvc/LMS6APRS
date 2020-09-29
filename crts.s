;	C STARTUP FOR ST7
;	WITHOUT ANY DATA INITIALIZATION
;	Copyright (c) 1998 by COSMIC Software
;
	xref	_main
	xdef	__stext
;
;	startup routine from reset vector
;
	switch	.text
__stext:
	rsp			; reset stack pointer
;
;	execute main() function
;
	jp	_main		; execute main
;
	end
