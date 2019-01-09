/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Menyisipkan instruksi INT 3 ke serangkaian instruksi yang valid
    untuk memicu breakpoint.

Compile:
    (msvc)
    $ cl interrupt-0x03.cpp
*/

#include <windows.h>

/*
    Menyisipkan instruksi INT 3 ke serangkaian instruksi yang valid.
    Pemanggilan akan menyebabkan kontrol dialihkan ke exception handler, utamanya
    debugger ketika process sedang di-debug. Jika tidak terdapat debugger,
    exception akan diteruskan ke SEH (Structured Exception Handler).

    Opcode umum: 0xCC dan 0xCD 0x03
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
bool antidebug_Interrupt03_VectorHandler()
{
    // Menambah handler ke Interrupt Vector
    PVOID handle = AddVectoredExceptionHandler(1, VectoredHandler);
    SwallowedException = true;
    __debugbreak();
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
        __debugbreak();
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
    check(antidebug_Interrupt03_VectorHandler, "Interrupt 3 (Vectored Handler)");
    check(antidebug_Interrupt2D_TryExcept,     "Interrupt 3 (__try/__except)");

    getchar();
    return 0;
}