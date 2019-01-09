;
;   Archive of Reversing.ID
;   Sample Code for Anti Debugging
;
; Assemble (use in MSVC - 64 bit):
;   $ nasm -fwin64 int2d-x64.asm
;
    global __int2d
section .text

__int2d:
    int 2dh
    nop
	ret
