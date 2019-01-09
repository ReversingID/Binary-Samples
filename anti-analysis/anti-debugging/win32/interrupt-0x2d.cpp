/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Menyisipkan instruksi INT 2D ke serangkaian instruksi yang valid
    untuk memicu exception.

Compile:
    (msvc)
    $ cl interrupt-0x2d.cpp int2d-x64.obj
    atau
    $ cl interrupt-0x2d.cpp int2d-x86.obj
*/

#include <windows.h>

/*
    Menyisipkan instruksi INT 2D ke serangkaian instruksi yang valid.
    Eksekusi interrupt akan menghasilkan breakpoint exception jika program
    tidak sedang di-debug.
    
    Jika process sedang di-debug dan instruksi tidak dieksekusi dengan flag trace, 
    exception tidak akan dipicu dan eksekusi akan berjalan dengan normal. Jika
    process di-debug dan instruksi sedang di-trace, byte berikutnya akan dilewati
    sehingga eksekusi akan berlanjut.
*/

extern "C" void __int2d();

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
        return EXCEPTION_CONTINUE_EXECUTION;

    return EXCEPTION_CONTINUE_SEARCH;
}
bool antidebug_Interrupt2D_VectorHandler()
{
    /* Menambah handler ke Interrupt Vector */
    PVOID handle = AddVectoredExceptionHandler(1, VectoredHandler);
    SwallowedException = true;
    __int2d();
    RemoveVectoredExceptionHandler(handle);
    return SwallowedException;
}


/**
 *  Menggunakan __try/__except untuk memicu interrupt.
 *
 *  Exception handler akan dieksekusi jika debugger tidak menangani (swallowing)
 *  exception.
 */
bool antidebug_Interrupt2D_TryExcept()
{
    __try
    {
        __int2d();
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
    check(antidebug_Interrupt2D_VectorHandler, "Interrupt 0x2D (Vectored Handler)");
    check(antidebug_Interrupt2D_TryExcept,     "Interupt 0x2D (__try/except)");

    getchar();
    return 0;
}