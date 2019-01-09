/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Memeriksa flag BeingDebugged di PEB untuk menentukan apakah process
    sedang di-debug.

Compile:
    (msvc)
    $ cl flag-PEB-BeingDebugged.cpp
*/

#include <windows.h>
#include <winternl.h>

/*
    Process Execution Block (PEB) menyimpan flag yang menandakan apakah
    process sedang di-debug.

    Lokasi PEB dapat dirujuk melalui fs:[30h] di 32-bit Windows maupun 
    gs:[60h] di 64-bit Windows. Field kedua di struct PEB adalah byte 
    BeingDebugged. API IsDebuggerPresent() menggunakan nilai yang ada di
    field tersebut sebagai kembalian.
*/

/* Menentukan lingkungan (64/32 bit) */
#if _WIN32 || _WIN64
#   if _WIN64
#       define ENV64BIT
#   else
#       define ENV32BIT
#   endif
#endif

/**
 *  Mendapatkan lokasi BeingDebugged di PEB dan melakukan pembacaan 
 *  terhadapnya.
 * 
 *   Teknik ini dapat diatasi dengan set nilai 0 ke PEB!BeingDebugged.
 */
bool antidebug_BeingDebugged()
{
    PPEB peb;
    
#if defined (ENV64BIT)
    peb = (PPEB) __readgsqword(0x60);

#elif defined (ENV32BIT)
    peb = (PPEB) __readfsdword(0x30);
#endif 

    return (peb->BeingDebugged == 1);
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_BeingDebugged, "Checking PEB.BeingDebugged");
    
    getchar();
    return 0;
}