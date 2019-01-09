/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Mendapatkan nilai kembalian dari API CheckRemoteDebuggerPresent()
    yang menandakan kondisi process apakah sedang di-debug atau tidak.
    

Compile:
    (msvc)
    $ cl api-CheckRemoteDebuggerPresent.cpp
*/

#include <windows.h>

/*
    CheckRemoteDebugger() umumnya digunakan untuk memeriksa apakah
    remote process sedang di-debug. Dengan memberikan handle ke diri sendiri,
    kita dapa tmemeriksa apakah kita sedang di-debug.
*/

/**
 *  API CheckRemoteDebuggerPresent()
 *  kernel32!CheckRemoteDebuggerPresent
 * 
 *  API ini memanggil ntdll!NtQueryInformationProcess() dengan 
 *  SYSTEM_INFORMATION_CLASS bernilai 7 (ProcessDebugPort).
 */
bool antidebug_CheckRemoteDebuggerPresentAPI()
{
    BOOL IsDebuggerPresent = FALSE;
    CheckRemoteDebuggerPresent(GetCurrentProcess(), &IsDebuggerPresent);
    return IsDebuggerPresent;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_CheckRemoteDebuggerPresentAPI, "Calling kernel32!CheckRemoteDebuggerPresent");

    getchar();
    return 0;
}