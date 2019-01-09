/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Memeriksa flag Flags di ProcessHeap.

Compile:
    (msvc)
    $ cl flag-ProcessHeap-Flags.cpp
*/

#include <windows.h>
#include "../../version.win32.h"

/*
    Di dalam array Reserved4 di PEB terdapat ProcessHeap yang berisi lokasi dari
    heap pertama dari process. First heap dialokasikan oleh loader.

    Ketika program berjalan di atas debugger dan diciptakan dengan debug process
    creation flag, nilai heap flags akan berubah. Flag ini berada di lokasi
    yang berbeda tergantung pada veri operating system yang digunakan.

    - Pada Windows XP, flag ini berada di offset 0x0C dari heap base pada
      system 32-bit dan 0x14 pada system 64-bit.
    - Pada Windows 7, flag ini berada pada offset 0x40 dari heap base pada
      system 32-bit dan 0x70 pada system 64-bit.

    Jika Flags tidak memiliki HEAP_GROWABLE (0x2) maka dipastikan process sedang di-debug.
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
 *  Dapatkan alamat dari Flags di ProcessHeap kemudian periksa nilainya.
 */
bool antidebug_ProcessHeap_Flags()
{
    PUINT32 pHeapFlags = NULL;

#if defined (ENV64BIT)
    PINT64  pProcessHeap = (PINT64)(__readgsqword(0x60) + 0x30);
    if (isWindowsVistaOrGreater())
        pHeapFlags = (PUINT32)(*pProcessHeap + 0x70);
    else 
        pHeapFlags = (PUINT32)(*pProcessHeap + 0x14);

#elif define (ENV32BIT)
    PUINT32 pProcessHeap = (PUINT32)(__readgsqword(0x30) + 0x18);
    if (isWindowsVistaOrGreater())
        pHeapFlags = (PUINT32)(*pProcessHeap + 0x40);
    else 
        pHeapFlags = (PUINT32)(*pProcessHeap + 0x0C);

#endif

    return (*pHeapFlags > 2);
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_ProcessHeap_Flags, "Checking ProcessHeap.Flags");

    getchar();
    return 0;
}