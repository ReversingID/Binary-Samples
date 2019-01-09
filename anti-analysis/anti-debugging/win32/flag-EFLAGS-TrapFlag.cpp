/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Set bit TF (Trap Flag) di EFLAGS

Compile:
    (msvc)
    $ cl flag-EFLAGS-TrapFlag.cpp
*/

#include <windows.h>

/*
    Bit ke-8 di register EFLAGS adalah Trap Flag (TF). Ketika diset, 
    exception SINGLE-STEP akan dijalankan
*/

extern "C" void __tfset();

bool antidebug_EFLAGS_TrapFlagSet()
{
    __try
    {
        __tfset();
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
    check(antidebug_EFLAGS_TrapFlagSet, "Set Trap Flag");
    
    getchar();
    return 0;
}