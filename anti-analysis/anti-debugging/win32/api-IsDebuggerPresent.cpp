/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Mendapatkan nilai kembalian dari API IsDebuggerPresent()
    yang menandakan kondisi process apakah sedang di-debug atau tidak.

Compile:
    (msvc)
    $ cl api-IsDebuggerPresent.cpp
*/

#include <windows.h>

/**
 *  API IsDebuggerPresent()
 *  kernel32!IsDebuggerPresent
 * 
 *  IsDebuggerPresent mengembalikan 1 jika process sedang di-debug, 0 jika
 *  sebaliknya. API ini membaca flag PEB!BeingDebugged yang terletak pada
 *  offset 2 di struktur PEB.
 * 
 *  Teknik ini dapat diatasi dengan set nilai 0 ke PEB!BeingDebugged.
 */
bool antidebug_IsDebuggerPresent()
{
    return IsDebuggerPresent();
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_IsDebuggerPresent, "Calling kernel32!IsDebuggerPresent");
    
    getchar();
    return 0;
}