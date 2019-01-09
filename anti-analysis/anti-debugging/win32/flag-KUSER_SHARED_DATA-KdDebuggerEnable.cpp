/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Mendeteksi keberadaan kernel debugger dengan memeriksa
    struktur KUSER_SHARED_DATA

Compile:
    (msvc)
    $ cl flag-KUSER_SHARED_DATA-KdDebuggerEnable.cpp
*/

#include <windows.h>

/*
    Struktur KUSER_SHARED_DATA terdapat di alamat (usermode) 0x7FFE0000 di 
    semua versi Windows (32/64 bit). Di kernel mode, struktur ini terletak di
    0xFFDF0000 (32-bit) serta 0xFFFFF78000000000 (64-bit).

    Terdapat dua flag yang dapat dibaca di struktur tersebut untuk mengetahui
    keberadaan kernel debugger, yaitu DebuggerEnabled dan DebuggerNotPresent.
*/

/**
 *  Mendapatkan KUSER_SHARED_DATA di alamat 0x7FFE0000.
 * 
 *  Terdapat sebuah masked flag di offset 0x2D4 yang merupakan gabungan antara
 *  KdDebuggerEnabled dan kdDebuggerNotPresent.
 */
bool antidebug_KUSER_SHARED_DATA_CheckPresence()
{
    /* Gunakan alamat KUSER_SHARED_DATA */
    const ULONG_PTR addrUserSharedData = 0x7FFE0000;

    /* offset 0x2D4 */
    const UCHAR KdDebuggerEnabledByte = * reinterpret_cast<UCHAR*>(addrUserSharedData + 0x2D4);

    /* Ektrak flag */
    const bool KdDebuggerEnabled    = ((KdDebuggerEnabledByte & 0x01) == 0x1);
    const bool KdDebuggerNotPresent = ((KdDebuggerEnabledByte & 0x02) == 0x0);

    return (KdDebuggerEnabled || !KdDebuggerNotPresent);
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_KUSER_SHARED_DATA_CheckPresence, "Checking SharedUserData->KdDebuggerEnabled");
    
    getchar();
    return 0;
}