/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Menjalankan instruksi 0xF1 (ICEBP) atau INT 1 untuk menghasilkan
    Single-Step exception.

Compile:
    (msvc)
    $ cl interrupt-IceBreakpoint.cpp intf1-x64.obj
    atau
    $ cl interrupt-IceBreakpoint.cpp intf1-x86.obj
*/

#include <windows.h>

/*
    ICEBP (Ice Breakpoint) merupakan instruksi yang tidak terdokumentasikan.
    Opcode 0xF1 digunakan untuk mendeteksi program tracer.

    Eksekusi 0xF1 akan menghasilkan exception SINGLE_STEP.
*/

extern "C" void __intf1();

static bool SwallowedException = false;

/**
 *  Eksekusi 0xF1 yang didefinisikan di assembly.
 */
bool antidebug_IceBreakpoint()
{
    __try
    {
        __intf1();
    }
    __except(1)
    {
        return false;
    }
    return true;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_IceBreakpoint, "Interrupt Ice Breakpoint");

    getchar();
    return 0;
}