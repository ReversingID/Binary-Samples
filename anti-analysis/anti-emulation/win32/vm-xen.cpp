/*
    Archive of Reversing.ID
    Sample Code for Anti Emulation

Description:
    Deteksi VM XEN berdasarkan artifak.

Compile:
    (msvc)
    $ cl vm-xen.cpp
*/

#include <windows.h>
#include "../../utils.process.hpp"
#include "../../utils.adapter.hpp"

/**
 *  Periksa keberadaan nama-nama process yang terkait dengan XEN.
 */
bool antivm_xen_process ()
{
    wchar_t * processes [] = {
        L"xenservice.exe",
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
 */
bool antivm_xen_macaddr ()
{
    // PCS Systemtechnik CmbH (VirtualBox)
    return checkMacAddress(L"\x00\x16\x3E");
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antivm_xen_process,  "VM XEN : Process");
    check(antivm_xen_macaddr,  "VM XEN : Mac Address");
    
    getchar();
    return 0;
}