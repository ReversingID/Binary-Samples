/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Memeriksa keberadaan debugger dengan mencoba menampilkan string dengan
    OutputDebugString().

Compile:
    (msvc)
    $ cl api-OutputDebugString.cpp
*/

#include <windows.h>
#include "../../version.win32.h"

/*
    API OutputDebugString() umumnya digunakan untuk menampilkan string ke 
    debugging data stream. String ini ditampilkan di debugger. Dengan kata lain,  
    hasil yang dberikan akan berbeda bergantung kepada keberadaan debugger.

    Jika process berada di atas debugger maka eksekusi akan berjalan normal dan
    tidak ada error state yang di-register. Sebaliknya, LastError akan berisi
    informasi bahwa tidak ada debugger (debugger free)

    Berlaku untuk Windows XP dan 2000
*/

/**
 *  API OutputDebugString
 *  kernel32!OutputDebugStringA || kernel32!OutputDebugStringW
 * 
 *  Ketika pemanggilan terjadi dengan string yang valid, API akan mengembalikan
 *  alamat dari string yang menjadi parameter. Jika process berjalan tanpa debug
 *  maka normalnya kembalian akan bernilai 1.
 */
bool antidebug_OutputDebugString_Print()
{
    bool  result = false;
    DWORD value  = 0x29A;

    /* Hanya berjalan di Windows XP/2000 */
    if (isWindowsXpOr2k())
    {
        SetLastError(value);
        OutputDebugString("Reversind.ID");

        if (GetLastError() == value)
            result = true;
    }
    return result;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_OutputDebugString_Print, "Calling kernel32!OutputDebugStringA");

    getchar();
    return 0;
}