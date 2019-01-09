;
;   Archive of Reversing.ID
;   Sample Code for Anti Debugging
;
; Assemble (use in MSVC - 32 bit):
;   $ nasm -fwin32 flag-trap-x86.asm
; 
    global ___tfset
section .text 

___tfset:
    pushfd
    or word [esp], 0x100
    popfd
    nop
    ret
