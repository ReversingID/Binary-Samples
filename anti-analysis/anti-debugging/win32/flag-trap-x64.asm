;
;   Archive of Reversing.ID
;   Sample Code for Anti Debugging
;
; Assemble (use in MSVC - 64 bit):
;   $ nasm -fwin64 flag-trap-x64.asm
;
    global __tfset
section .text

__tfset:
    pushfq
    or word [esp], 0x100
    popfq
    nop
	ret
