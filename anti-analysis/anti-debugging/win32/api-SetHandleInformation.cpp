/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Menggunakan trik percobaan Close protected handle untuk
    mendeteksi debugger.

Compile:
    (msvc)
    $ cl api-SetHandleInformation.cpp
*/

#include <windows.h>

/*
    Jika sebuah protected handle diberikan ke fungsi CloseHandle(), atau NtClose maupun
    ZwClose, maka EXCEPTION_HANDLE_NOT_CLOSABLE (0xC0000235) akan diberikan. 
    Jika exception diterima maka terdapat indikasi bahwa debugging sedang dilakukan.
*/

/**
 *  SetHandleInformation()
 *  kernel32!SetHandleInformation
 * 
 * 
 */
bool antidebug_SetHandleInformation()
{
    HANDLE mutex;

    /* Ciptakan mutex untuk mendapatkan handle */
    mutex = CreateMutex(NULL, FALSE, "Reversing.ID");

    /* Berikan proteksi agar handle tidak dapat di-close */
    SetHandleInformation(mutex, HANDLE_FLAG_PROTECT_FROM_CLOSE, HANDLE_FLAG_PROTECT_FROM_CLOSE);

    __try
    {
        CloseHandle(mutex);
    }
    __except(HANDLE_FLAG_PROTECT_FROM_CLOSE)
    {
        return true;
    }
    return false;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_SetHandleInformation, "Checking CloseHandle protected handle trick");
    
    getchar();
    return 0;
}