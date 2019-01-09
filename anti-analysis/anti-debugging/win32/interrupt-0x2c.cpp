/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Menyisipkan instruksi INT 0x2C ke serangkaian instruksi yang valid
    untuk memicu exception.

Compile:
    (msvc)
    $ cl interrupt-0x03.cpp
*/

#include <windows.h>
#include "../../version.win32.h"

/*
    Menyisipkan instruksi INT 2C ke serangkaian instruksi yang valid.

    Interrupt 2C akan memicu debug assertion exception. Exception ini ditangani oleh
    WinDbg (namun tidak debugger lain).
    
    Interrupt ini hanya berlaku untuk versi Windows Vista dan setelahnya.
*/

static bool SwallowedException = false;

/**
 *  Membuat Vector Handler secara manual untuk menelusuri SEH.
 *
 *  Handler akan dieksekusi jika debugger tidak menangani (swallowing) 
 *  exception yang dipicu.
 */
static LONG CALLBACK 
VectoredHandler (_In_ PEXCEPTION_POINTERS ep)
{
    SwallowedException = false;

    if (ep->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
    {
#ifdef _WIN64
        ep->ContextRecord->Rip++;
#else 
        ep->ContextRecord->Eip++;
#endif
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}
bool antidebug_Interrupt2C_VectorHandler()
{
    SwallowedException = true;

    if (isWindowsVistaOrGreater())
    {
        // Menambah handler ke Interrupt Vector
        PVOID handle = AddVectoredExceptionHandler(1, VectoredHandler);
        __int2c();
        RemoveVectoredExceptionHandler(handle);
    }
    return SwallowedException;
}


/**
 *  Menggunakan __try/__except untuk memicu interrupt.
 *
 *  Exception handler akan dieksekusi jika debugger tidak menangani (swallowing)
 *  exception.
 */
bool antidebug_Interrupt2C_TryExcept()
{
    if (isWindowsVistaOrGreater())
    {
        __try
        {
            __int2c();
        }
        __except(1)
        {
            return false;
        }
        return true;
    }
    return false;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_Interrupt2C_VectorHandler, "Interrupt 2C (Vectored Handler)");
    check(antidebug_Interrupt2C_TryExcept,     "Interrupt 2C (__try/__except)");

    getchar();
    return 0;
}