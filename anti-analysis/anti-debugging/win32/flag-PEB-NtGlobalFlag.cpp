/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Memeriksa nilai flag NtGlobal di PEB.

Compile:
    (msvc)
    $ cl flag-PEB-NtGlobalFlag.cpp
*/

#include <windows.h>
#include "../../utils.win32.h"

/*
    Di PEB terdapat flag NtGlobalFlag yang berisi informasi bagaimana system
    menciptakan struktur heap.

    Field NtGlobalFlag berada di PEB pada offset 0x68 untuk process 32-bit
    dan 0xBC untuk process 64-bit. Nilai flag ini merupakan kombinasi beberapa
    flags yang diset oleh OS  untuk memengaruhi process yang sedang di-debug.
    Flag tersebut antara lain:

        - FLG_HEAP_ENABLE_TAIL_CHECK (0x10)
        - FLG_HEAP_ENABLE_FREE_CHECK (0x20)
        - FLG_HEAP_VALIDATE_PARAMETERS (0x40)

    Jika kombinasi FLG_HEAP_ENABLE_TAIL_CHECK, FLG_HEAP_ENABLE_FREE_CHECK, dan 
    FLG_HEAP_VALIDATE_PARAMETERS aktif maka process sedang di-debug.
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
 *  Dapatkan PEB dan periksa nilai NtGlobalFlag.
 *  
 *  Terdapat tiga case aplikasi berjalan:
 *      1. aplikasi 64-bit berjalan di OS 64-bit
 *      2. aplikasi 32-bit berjalan di OS 32-bit
 *      3. aplikasi 32-bit berjalan di OS 64-bit menggunakan Wow64
 * 
 *  Di Wow64, terdapat PEB terpisah untuk bagian 32-bit dan 64-bit sehingga perlu
 *  dilakukan pengecekan ekstra.
 */
bool antidebug_NtGlobalFlag()
{
    PDWORD pNtGlobalFlag = NULL, pNtGlobalFlagWow64 = NULL;

#if defined (ENV64BIT)
    /* 1. process 64-bit berjalan di lingkungan 64-bit windows */
    pNtGlobalFlag = (PDWORD)(__readgsqword(0x60) + 0xBC);

#elif defined (ENV32BIT)
    /* 2. process 32-bit berjalan di lingkungan 32-bit windows (real) */
    BYTE * _teb32 = (BYTE*)__readfsdword(0x18);     // Thread Execution Block
    DWORD  _peb32 = *(DWORD*)(_teb32 + 0x30);       // Process Execution Block
    pNtGlobalFlag = (PDWORD)(_peb32 + 0x68);        // NtGlobalFlag

    /* 3. process 32-bit berjalan di lingkungan 64-bit windows. */
    if (isWow64())
    {
        BYTE *  _teb64 = (BYTE*)__readfsdword(0x18) - 0x2000;
        DWORD64 _peb64 = *(DWORD64*)(_teb64 + 0x60);
        pNtGlobalFlagWow64 = (PDWORD)(_peb64 + 0xBC);
    }
#endif 

    bool normalDetected = pNtGlobalFlag && (*pNtGlobalFlag & 0x00000070);
    bool wow64Detected  = pNtGlobalFlagWow64 && (*pNtGlobalFlagWow64 & 0x00000070);

    return (normalDetected || wow64Detected);
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_NtGlobalFlag, "Checking PEB.NtGlobalFlag");

    getchar();
    return 0;
}