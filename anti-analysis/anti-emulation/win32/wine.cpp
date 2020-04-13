/*
    Archive of Reversing.ID
    Sample Code for Anti Emulation

Description:
    Deteksi WINE berdasarkan artifak.

Compile:
    (msvc)
    $ cl wine.cpp
*/

#include <windows.h>
#include "../../utils.registry.hpp"

/**
 *  Kernel32.dll di WINE memiliki sebuah export tambahan.
 */
bool antivm_wine_exports ()
{
    HMODULE kernel32;

    kernel32 = GetModuleHandleW(L"kernel32.dll");
    if (kernel32)
        return (GetProcAddress(kernel32, "wine_get_unix_file_name") != NULL);
    
    return false;
}


/**
 *  Deteksi keberadaan registry keys tertentu
 */
bool antivm_wine_regkey()
{
    /* Daftar registry key yang terdapat di lingkungan VirtualBox */
    wchar_t * entries[] = {
		L"SOFTWARE\\Wine"
    };

    size_t length = sizeof(entries) / sizeof(entries[0]);
    for (size_t i = 0; i < length; i++)
    {
        if (isRegKeyExists(HKEY_LOCAL_MACHINE, entries[i]))
            return true;
    }

    return false;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antivm_wine_exports,  "VM WINE : Export Function (kernel32.dll)");
    check(antivm_wine_regkey,   "VM WINE : Registry Key");
    
    getchar();
    return 0;
}