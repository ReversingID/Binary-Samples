;
;   Archive of Reversing.ID
;   Sample Code for Anti Debugging
;
; Assemble (use in MSVC - 64 bit):
;   $ nasm -fwin64 intf1-x64.asm
;
    global __intf1
section .text

__intf1:
    db 0xF1
    nop
	ret
