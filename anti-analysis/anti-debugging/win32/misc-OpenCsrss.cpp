/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Memeriksa sesi debugging dengan membuka Csrss.

Compile:
    (msvc)
    $ cl misc-OpenCsrss.cpp
*/

#include <windows.h>

/*
    Jika sesi debugging terjadi dengan SeDebugPrivileges maka OpenProcess akan 
    berhasil dilakukan. Namun kondisi administrator privileges diperlukan ketika
    debugging terjadi.

    Di WIndows XP, Vista, dan 7, pemanggilen OpenProcess() dengan PROCESS_ALL_ACCESS
    akan gagal meskipun SeDebugPrivileges aktif. Karena itu digunakan 
    PROCESS_QUERY_LIMITED_INFORMATION
*/

bool antidebug_OpenCsrss()
{
    typedef DWORD(NTAPI * pCsrGetId)(VOID);

    HANDLE csrss = NULL;

    /* Dapatkan address ke function */
    auto   dll      = GetModuleHandle("ntdll.dll");
    void * procaddr = GetProcAddress(dll, "CsrGetProcessId");
    auto fnCsrGetId = static_cast<pCsrGetId>(procaddr);
    if (fnCsrGetId == nullptr)
        return false;

    csrss = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, fnCsrGetId());
    if (csrss != NULL)
    {
        CloseHandle(csrss);
        return true;
    }
    return false;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_OpenCsrss, "Opening Csrss");
    
    getchar();
    return 0;
}