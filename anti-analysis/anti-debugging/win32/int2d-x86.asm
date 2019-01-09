;
;   Archive of Reversing.ID
;   Sample Code for Anti Debugging
;
; Assemble (use in MSVC - 32 bit):
;   $ nasm -fwin32 int2d-x86.asm
; 
    global ___int2d
section .text 

___int2d:
    int 2dh
    nop
    ret
