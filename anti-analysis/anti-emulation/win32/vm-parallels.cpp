/*
    Archive of Reversing.ID
    Sample Code for Anti Emulation

Description:
    Deteksi VM Parallels berdasarkan artifak.

Compile:
    (msvc)
    $ cl vm-parallels.cpp
*/

#include <windows.h>
#include "../../utils.process.hpp"
#include "../../utils.adapter.hpp"

#define MAX_MESSAGE 256

/**
 *  Periksa keberadaan nama-nama process yang terkait dengan Parallels.
 */
bool antivm_parallels_process ()
{
    wchar_t * processes [] = {
        L"prl_cc.exe",
        L"prl_tools.exe",
    };

    int length = sizeof(processes) / sizeof(processes[0]);
    for (int i = 0; i < length; i++)
    {
        if (getPidFromName(processes[i]))
            return true;
    }
    return false;
}


/**
 *  Periksa MAC address yang digunakan.
 *  Jika tidak ada perubahan, maka MAC address akan menggunakan address
 *  yang termasuk ke dalam vendor Paralles, Inc.
 */
bool antivm_parallels_macaddr ()
{
    // Parallels, Inc.
    return checkMacAddress(L"\x00\x1C\x42");
}

/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antivm_parallels_process, "VM Parallels : Process");
    check(antivm_parallels_macaddr, "VM Parallels : MAC Address");
    
    getchar();
    return 0;
}