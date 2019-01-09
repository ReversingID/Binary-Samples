/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Memeriksa apakah UnhandledExceptionFilter() dipanggil.

Compile:
    (msvc)
    $ cl exception-PageException-BreakpointCheck.cpp
*/

#include <windows.h>

/*
    Ketika exception terjadi dan tidak ada exception handler yang terdaftar
    (baik Structured maupun vectored) atau tidak ada handler yang menangani
    exception, maka fungsi UnhandledExceptionFilter() di kernel32 akan
    dipanggil.
*/

bool SwallowedException = true;
LONG WINAPI UnhandledExceptFilter(PEXCEPTION_POINTERS pExceptPointers)
{
    /* Jika debugger ada, fungsi ini tak akan pernah dipanggil */
    SwallowedException = false;
    return EXCEPTION_CONTINUE_EXECUTION;
}
bool antidebug_UnhandledExceptionFilter_Test()
{
    LPTOP_LEVEL_EXCEPTION_FILTER top = SetUnhandledExceptionFilter(UnhandledExceptFilter);
    RaiseException(EXCEPTION_FLT_DIVIDE_BY_ZERO, 0, 0, NULL);
    SetUnhandledExceptionFilter(top);
    return SwallowedException;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_UnhandledExceptionFilter_Test, "Checking UnhandledExceptionFilter");

    getchar();
    return 0;
}