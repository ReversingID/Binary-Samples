/*
    Archive of Reversing.ID
    Sample Code for Anti Emulation

Description:
    Deteksi VM QEMU berdasarkan artifak.

Compile:
    (msvc)
    $ cl vm-qemu.cpp
*/

#include <windows.h>
#include "../../utils.common.hpp"
#include "../../utils.process.hpp"
#include "../../utils.registry.hpp"
#include "../../utils.firmware.hpp"

/**
 *  Periksa keberadaan nama-nama process yang terkait dengan QEMU.
 */
bool antivm_qemu_process ()
{
    wchar_t * processes [] = {
        L"qemu-ga.exe",
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
 *  Periksa Value dari Regisry Key
 */
bool antivm_qemu_regkeyval ()
{
    wchar_t * entries[][3] = {
        { L"HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", L"Identifier", L"QEMU" },
        { L"HARDWARE\\Description\\System", L"SystemBiosVersion", L"QEMU" },
    };

    int length = sizeof(entries) / sizeof(entries[0]);
    for (int i = 0; i < length; i++)
    {
        if (isRegKeyValExists(HKEY_LOCAL_MACHINE, entries[i][0], entries[i][1], entries[i][2]))
            return true;
    }

    return false;
}


/**
 *  Periksa firmware SMBIOS
 */
bool antivm_qemu_firmware_smbios ()
{
    bool result = false;

    DWORD size = 0;
    PBYTE smbios = getFirmwareTable(static_cast<DWORD>('RSMB'), 0x0000, &size);
    if (smbios != NULL)
    {
        if (memsearch((PBYTE) "qemu", 4, smbios, size) || 
            memsearch((PBYTE) "QEMU", 4, smbios, size) )
            result = true;

        free(smbios);
    }

    return result;
}


/**
 *  Periksa firmware ACPI
 */
bool antivm_qemu_firmware_acpi ()
{
    bool result = false;

    PDWORD names = static_cast<PDWORD>(malloc(4096));
    SecureZeroMemory(names, 4096);
    DWORD  size = enumFirmwareTables(static_cast<DWORD>('ACPI'), names, 4096);

    if (size == -1)
        return false;
    
    DWORD count = size / 4;
    if (size < 4 || count == 0)
        result = true;
    else 
    {
        for (DWORD i = 0; i < count && !result; i++)
        {
            DWORD tblsize = 0;
            PBYTE table = getFirmwareTable(static_cast<DWORD>('ACPI'), names[i], &tblsize);

            if (memsearch((PBYTE) "BOCHS", 5, table, tblsize) || 
                memsearch((PBYTE) "BXPC", 4, table,  tblsize) )
                result = true;
            
            free(table);
        }
    }

    free(names);
    return result;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antivm_qemu_process, "VM QEMU : Process");
    check(antivm_qemu_regkeyval,  "VM QEMU : Registry Key Value");
    check(antivm_qemu_firmware_smbios,  "VM QEMU : Firmware BIOS");
    check(antivm_qemu_firmware_acpi,    "VM QEMU : Firmware ACPI");
    
    getchar();
    return 0;
}