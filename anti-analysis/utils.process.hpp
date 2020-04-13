#pragma once
#define _UNICODE

#include <windows.h>
#include <Psapi.h>

// #pragma comment(lib, "Mpr.lib")

/******************************** DECLARATION ********************************/

bool  isWow64();

DWORD getPidFromName (wchar_t * process);

/******************************* IMPLEMENTATION *******************************/

/**
 *  Periksa apakah process merupakan process 32-bit yang berjalan di OS 64-bit.
 */
bool isWow64()
{
    typedef BOOL (*pIsWow64Process)(HANDLE, PBOOL);

    BOOL result = FALSE;

    /* dapatkan address ke function */
    auto dll  = GetModuleHandle("kernel32.dll");
    auto func = reinterpret_cast<pIsWow64Process>(GetProcAddress(dll, "IsWow64Process"));

    // jika pemanggilan menghasilkan error, maka operasi keseluruhan false. 
    if (func && func(GetCurrentProcess(), &result))
        result = FALSE;

    return result;
}


/**
 *  Dapatkan PID dari process dengan nama tertentu.
 */
DWORD getPidFromName (wchar_t * name)
{
    DWORD   pids[1024];
    DWORD   nbytes;
    BOOL    status;
    HMODULE module;
    DWORD   reserved;
    wchar_t procname[MAX_PATH] = L"";
    size_t  count, i;     

    // dapatkan daftar PID
    status = EnumProcesses(pids, sizeof(pids), &nbytes);

    // hitung berapa banyak PID yang dikembalikan
    count = nbytes / sizeof(DWORD);

    for (i = 0; i < count; i++)
    {
        // dapatkan handle ke process
        HANDLE handle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pids[i]);

        // dapatkan process name
        if (handle != NULL)
        {
            K32EnumProcessModules(handle, &module, sizeof(module), &reserved);
            K32GetModuleBaseNameW(handle, module, procname, MAX_PATH);

            CloseHandle(handle);

            // lakukan perbandingan
            if (wcscmp(procname, name) == 0)
                return pids[i];
        }
    }

    return 0;
}
