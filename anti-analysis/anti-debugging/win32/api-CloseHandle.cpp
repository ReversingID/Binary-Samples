/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Ketika sebuah proses sedang di-debug, pemanggilan terhadap ZwClose() 
    serta turunannya (NtClose, CloseHandle) dengan invalid handle akan 
    menghasilkan STATUS_INVALID_HANDLE (0xC0000008) exception.


Compile:
    (msvc)
    $ cl api-CloseHandle.cpp
*/

#include <windows.h>

/*
    Prototype dari kernel32!CloseHandle():
        BOOL WINAPI CloseHandle(
            _In_ HANDLE hObject
        );

    Prototype dari ntdll!NtClose():
        __kernel_entry NTSYSCALLAPI NTSTATUS NtClose(
            HANDLE Handle
        );


    Prototype dari ntdll!ZwClose():
        __kernel_entry NTSYSCALLAPI NTSTATUS ZwClose(
            HANDLE Handle
        );

*/

/**
 * API CloseHandle()
 * kernel32!CloseHandle
 *
 * API CloseHandle() merupakan wrapper dari NtClose().
 *
 * Teknik ini dapat diatasi dengan memodifikasi syscall data dari rin3 sebelum
 * pemanggilan dilakukan, atau dapat dengan melakukan kernel hook.
 */
bool antidebug_CloseHandle_InvalidHandle()
{
    // Menggunakan wrapper API, CloseHandle()
    __try 
    {
        CloseHandle((HANDLE) 0x99999999ULL);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) 
    {
        return TRUE;
    }
    return FALSE;
}


/**
 * API NtClose()
 * ntdll!NtClose
 * 
 * ntdll!ZwClose dan ntdll!NtClose adalah identik dari sisi user-mode.
 */
bool antidebug_NtClose_InvalidHandle()
{
    typedef NTSTATUS(WINAPI * pNtClose)(HANDLE);

    __try 
    {
        auto dll = LoadLibraryA("ntdll.dll");
        auto fnNtClose = reinterpret_cast<pNtClose>(GetProcAddress(dll, "NtClose"));
        fnNtClose(reinterpret_cast<HANDLE>(0x99999999ULL));
    }
    __except (EXCEPTION_EXECUTE_HANDLER) 
    {
        return true;
    }
    return false;
}


/**
 * API ZwClose() 
 * ntdll!ZwClose
 * 
 * ntdll!ZwClose dan ntdll!NtClose adalah identik dari sisi user-mode.
 */
bool antidebug_ZwClose_InvalidHandle()
{
    typedef NTSTATUS(WINAPI * pZwClose)(HANDLE);

    __try 
    {
        auto dll = LoadLibraryA("ntdll.dll");
        auto fnZwClose = reinterpret_cast<pZwClose>(GetProcAddress(dll, "ZwClose"));
        fnZwClose(reinterpret_cast<HANDLE>(0x99999999ULL));
    }
    __except (EXCEPTION_EXECUTE_HANDLER) 
    {
        return true;
    }
    return false;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_CloseHandle_InvalidHandle, "Calling kernel32!CloseHandle");
    check(antidebug_NtClose_InvalidHandle,     "Calling ntdll!NtClose");
    check(antidebug_ZwClose_InvalidHandle,     "Calling ntdll!ZwClose");
    
    getchar();
    return 0;
}