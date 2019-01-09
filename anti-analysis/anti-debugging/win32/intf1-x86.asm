;
;   Archive of Reversing.ID
;   Sample Code for Anti Debugging
;
; Assemble (use in MSVC - 32 bit):
;   $ nasm -fwin32 intf1-x86.asm
; 
    global ___intf1
section .text 

___intf1:
    db 0xF1
    nop
    ret
