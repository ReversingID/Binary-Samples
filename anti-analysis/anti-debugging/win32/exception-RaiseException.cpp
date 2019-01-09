/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Memicu beberapa macam exception untuk memeriksa keberadaan debugger
    melalui penanganan exception.
   

Compile:
    (msvc)
    $ cl exception-RaiseException.cpp
*/

#include <windows.h>

/*
    Beberapa jenis exception dihasilkan dengan fungsi RaiseException akan ditangani
    (swallowed) oleh debugger. Beberapa exception yang dapat digunakan antara lain:

    STATUS_BREAKPOINT               (0x80000003)
    STATUS_SINGLE_STEP              (0x80000004) 
    DBG_PRINTEXCEPTION_C            (0x40010006)
    DBG_RIPEXCEPTION                (0x40010007)
    DBG_CONTROL_C                   (0x40010005)
    DBG_CONTROL_BREAK               (0x40010008)
    DBG_COMMAND_EXCEPTION           (0x40010009)
    ASSERTION_FAILURE               (0xC0000420)
    STATUS_GUARD_PAGE_VIOLATION     (0x80000001)
    SEGMENT_NOTIFICATION            (0x40000005)
    EXCEPTION_WX86_SINGLE_STEP      (0x4000001E)
    EXCEPTION_WX86_BREAKPOINT       (0x4000001F)
*/


template <DWORD code>
bool antidebug_RaiseException()
{
    __try 
    {
        RaiseException(code, 0, 0, 0);
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
    check(antidebug_RaiseException<STATUS_BREAKPOINT>, "Raising Exception: STATUS_BREAKPOINT");
    check(antidebug_RaiseException<STATUS_SINGLE_STEP>, "Raising Exception: STATUS_SINGLE_STEP");
    check(antidebug_RaiseException<DBG_PRINTEXCEPTION_C>, "Raising Exception: DBG_PRINTEXCEPTION_C");
    check(antidebug_RaiseException<DBG_RIPEXCEPTION>, "Raising Exception: DBG_RIPEXCEPTION");
    check(antidebug_RaiseException<DBG_CONTROL_C>, "Raising Exception: DBG_CONTROL_C");
    check(antidebug_RaiseException<DBG_CONTROL_BREAK>, "Raising Exception: DBG_CONTROL_BREAK");
    check(antidebug_RaiseException<DBG_COMMAND_EXCEPTION>, "Raising Exception: DBG_COMMAND_EXCEPTION");
    check(antidebug_RaiseException<STATUS_ASSERTION_FAILURE>, "Raising Exception: STATUS_ASSERTION_FAILURE");
    check(antidebug_RaiseException<STATUS_GUARD_PAGE_VIOLATION>, "Raising Exception: STATUS_GUARD_PAGE_VIOLATION");
    check(antidebug_RaiseException<STATUS_SEGMENT_NOTIFICATION>, "Raising Exception: STATUS_SEGMENT_NOTIFICATION");

    getchar();
    return 0;
}