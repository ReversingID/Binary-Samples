#pragma once
#define _UNICODE

#include <windows.h>
#include <sysinfoapi.h>

/******************************** DECLARATION ********************************/

PBYTE getFirmwareTable(DWORD signature, DWORD table, PDWORD size);
UINT  enumFirmwareTables(DWORD signature, PVOID table, DWORD length);

/******************************* IMPLEMENTATION *******************************/

/**
 *  Dapatkan firmware table tertentu berdasarkan signature firmware table provider.
 */
PBYTE getFirmwareTable(DWORD signature, DWORD table, PDWORD size)
{
    typedef UINT (*pGetSystemFirmwareTable)(DWORD, DWORD, PVOID, DWORD);

    /* dapatkan address ke function */
    auto dll  = GetModuleHandle("kernel32.dll");
    auto func = reinterpret_cast<pGetSystemFirmwareTable>(GetProcAddress(dll, "GetSystemFirmwareTable"));

    DWORD  buffer_size = 4096, result;
    LPBYTE buffer = NULL;
    UINT   table_length = 0;

    if (func)
    {
        buffer = static_cast<PBYTE>(malloc(buffer_size));
        SecureZeroMemory(buffer, buffer_size);

        /* 
            dapatkan firmware table
            sesuaikan ukuran buffer jika diperlukan.
        */
        result = func(signature, table, buffer, buffer_size);
        if (result == 0)
        {
            free(buffer);
            return NULL;
        }

        if (result > buffer_size)
        {
            buffer = static_cast<PBYTE>(realloc(buffer, result));
            SecureZeroMemory(buffer, result);

            if (func(signature, table, buffer, result) == 0)
            {
                free(buffer);
                return NULL;
            }
        }

        *size = result;
    }
    
    return buffer;
}


/**
 *  Enumerasi daftar firmware-table di sistem 
 */
UINT  enumFirmwareTables(DWORD signature, PVOID table, DWORD length)
{
    typedef UINT (*pEnumSystemFirmwareTables)(DWORD, PVOID, DWORD);

    /* dapatkan address ke function */
    auto dll = GetModuleHandle("kernel32.dll");
    auto func = reinterpret_cast<pEnumSystemFirmwareTables>(GetProcAddress(dll, "EnumSystemFirmwareTables"));

    if (func) 
        return func(signature, table, length);

    return -1;
}