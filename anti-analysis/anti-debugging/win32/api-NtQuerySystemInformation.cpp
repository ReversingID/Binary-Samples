/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Menggunakan NtQuerySystemInformation untuk mendeteksi keberadaan
    kernel debugger.

Compile:
    (msvc)
    $ cl api-NtQuerySystemInformation.cpp
*/

#include <windows.h>

/*
    Ketika NtQuerySystemInformation dipanggil dengan SystemKernelDebugger class,
    fungsi akan mengembalikan struktur SYSTEM_KERNEL_DEBUGGER_INFORMATION
    yang dapat digunakan untuk memberitahu keberadaan kernel debugger.

    Prototype NtQuerySystemInformation:
        __kernel_entry NTSTATUS NtQuerySystemInformation(
            IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
            OUT PVOID                   SystemInformation,
            IN ULONG                    SystemInformationLength,
            OUT PULONG                  ReturnLength
        );

    Prototype ZwQuerySystemInformation:
        NTSTATUS WINAPI ZwQuerySystemInformation(
            IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
            OUT PVOID                   SystemInformation,
            IN ULONG                    SystemInformationLength,
            OUT PULONG                  ReturnLength
        );
*/

typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION
{
    BOOLEAN KernelDebuggerEnabled;
    BOOLEAN KernelDebuggerNotPresent;
} SYSTEM_KERNEL_DEBUGGER_INFORMATION, *PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

/**
 *  NtQuerySystemInformation() dengan SystemKernelDebuggerInformation class.
 *  ntdll!NtQuerySystemInformation
 */
bool antidebug_NtQuerySystemInformation()
{
    typedef NTSTATUS(WINAPI * pNtQuerySystemInformation)(UINT, PVOID, ULONG, PULONG);

    SYSTEM_KERNEL_DEBUGGER_INFORMATION kdbginfo;
    NTSTATUS status;

    /* Dapatkan address ke function */
    auto   dll      = GetModuleHandle("ntdll.dll");
    void * procaddr = GetProcAddress(dll, "NtQuerySystemInformation");
    auto fnNtQuerySystemInformation = static_cast<pNtQuerySystemInformation>(procaddr);
    if (fnNtQuerySystemInformation == nullptr)
        return false;
    
    /* Set SystemInformationClass menjadi 0x23 (SystemKernelDebuggerInformation) */
    const int SystemInformationClass = 0x23;

    status = fnNtQuerySystemInformation(SystemInformationClass, &kdbginfo, sizeof(SYSTEM_KERNEL_DEBUGGER_INFORMATION), NULL);
    if (status >= 0)
        return (kdbginfo.KernelDebuggerEnabled || !kdbginfo.KernelDebuggerNotPresent); 

    return false;
}


/**
 *  ZwQuerySystemInformation() dengan SystemKernelDebuggerInformation class.
 *  ntdll!ZwQuerySystemInformation
 */
bool antidebug_ZwQuerySystemInformation()
{
    typedef NTSTATUS(WINAPI * pZwQuerySystemInformation)(UINT, PVOID, ULONG, PULONG);

    SYSTEM_KERNEL_DEBUGGER_INFORMATION kdbginfo;
    NTSTATUS status;

    /* Dapatkan address ke function */
    auto   dll      = GetModuleHandle("ntdll.dll");
    void * procaddr = GetProcAddress(dll, "ZwQuerySystemInformation");
    auto fnZwQuerySystemInformation = static_cast<pZwQuerySystemInformation>(procaddr);
    if (fnZwQuerySystemInformation == nullptr)
        return false;
    
    /* Set SystemInformationClass menjadi 0x23 (SystemKernelDebuggerInformation) */
    const int SystemInformationClass = 0x23;

    status = fnZwQuerySystemInformation(SystemInformationClass, &kdbginfo, sizeof(SYSTEM_KERNEL_DEBUGGER_INFORMATION), NULL);
    if (status >= 0)
        return (kdbginfo.KernelDebuggerEnabled || !kdbginfo.KernelDebuggerNotPresent); 

    return false;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_NtQuerySystemInformation, "Calling NtQuerySystemInformation (SystemKernelDebuggerInformation)");
    check(antidebug_ZwQuerySystemInformation, "Calling ZwQuerySystemInformation (SystemKernelDebuggerInformation)");

    getchar();
    return 0;
}