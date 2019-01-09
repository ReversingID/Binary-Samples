/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Memeriksa informasi process melalui Win32 API.
    Pemanggilan fungsi ini memerlukan flag spesifik untuk mendapatkan informasi 
    terkait debugging. Setidaknya terdapat tiga hal yang bisa digunakan:

        - ProcessDebugFlags
        - ProcessDebugObject
        - ProcessDebugPort
    
Compile:
    (msvc)
    $ cl api-NtQueryInformationProcess.cpp
*/

#include <windows.h>

/* Menentukan lingkungan (64/32 bit) */
#if _WIN32 || _WIN64
#   if _WIN64
#       define ENV64BIT
#   else
#       define ENV32BIT
#   endif
#endif

/*
    ntdll!NtQueryInformationProcess

    API NtQueryInformationProcess() adalah wrapper dari syscall
    ZwQueryInformationProcess(). 

    Prototype API ini adalah:

    NTSYSAPI NTSTATUS NTAPI NtQueryInformationProcess(
        IN HANDLE ProcessHandle,
        IN PROCESS_INFORMATION_CLASS procinfoclass,
        OUT PVOID procinfo,
        IN ULONG procinfoLength,
        OUT PULONG ReturnLength
    );

    Teknik ini dapat diatasi dengan memodifikasi nilai procinfo. 
*/

typedef NTSTATUS(WINAPI * pNtQueryInformationProcess)(HANDLE, UINT, PVOID, ULONG, PULONG);

/**
 *  API NtQueryInformationProcess() dengan ProcessDebugFlags (0x1F).
 *  ntdll!NtQueryInformationProcess
 * 
 *  Ketika NtQueryInformationProcess() dipanggil dengan class ProcessDebugFlags, 
 *  fungsi akan mengembalikan nilai EPROCESS->NoDebugInherit, artinya
 *  jika terdapat debugger maka fungsi akan mengembalikan FALSE.
 */
bool antidebug_NtQueryInformationProcess_ProcessDebugFlags()
{
    /* Dapatkan address ke function */
    auto   dll      = GetModuleHandle("ntdll.dll");
    void * procaddr = GetProcAddress(dll, "NtQueryInformationProcess");
    auto fnNtQueryInfoProcess = static_cast<pNtQueryInformationProcess>(procaddr);
    if (fnNtQueryInfoProcess == nullptr)
        return false;

    /* Set ProcessInformationClass menjadi 0x1F (ProcessDebugFlags) */
    const int procinfoclass = 0x1F;

    NTSTATUS  status;
    DWORD     procinfo;
    
    status = fnNtQueryInfoProcess(GetCurrentProcess(), procinfoclass, &procinfo, sizeof(procinfo), NULL);
    return (status == 0x00000000 && procinfo == 0);
}


/**
 *  API NtQueryInformationProcess() dengan ProcessDebugObject (0x1E).
 *  ntdll!NtQueryInformationProcess
 * 
 *  Mencoba mendapatkan debug object handle untuk process ini.
 */
bool antidebug_NtQueryInformationProcess_ProcessDebugObject()
{
    /* Dapatkan address ke function */
    auto   dll      = GetModuleHandle("ntdll.dll");
    void * procaddr = GetProcAddress(dll, "NtQueryInformationProcess");
    auto fnNtQueryInfoProcess = static_cast<pNtQueryInformationProcess>(procaddr);
    if (fnNtQueryInfoProcess == nullptr)
        return false;

    /* Set ProcessInformationClass menjadi 0x1E (ProcessDebugObject) */
    const int procinfoclass = 0x1E;

    NTSTATUS status;

#if defined (ENV64BIT)
    DWORD64  procinfo;
#elif defined (ENV32BIT)
    DWORD    procinfo;
#endif 
    
    status = fnNtQueryInfoProcess(GetCurrentProcess(), procinfoclass, &procinfo, sizeof(procinfo), NULL);
    return (status == 0x00000000 && procinfo);
}


/**
 *  API NtQueryInformationProcess() dengan ProcessDebugPort (0x7).
 *  ntdll!NtQueryInformationProcess
 *  
 *  Ketika pemanggilan terjadi dengan class ProcessDebugPort, 
 *  system akan set procinfo menjadi -1 jika process sedang di-debug.
 */
bool antidebug_NtQueryInformationProcess_ProcessDebugPort()
{
    /* Dapatkan address ke function */
    auto   dll      = GetModuleHandle("ntdll.dll");
    void * procaddr = GetProcAddress(dll, "NtQueryInformationProcess");
    auto fnNtQueryInfoProcess = static_cast<pNtQueryInformationProcess>(procaddr);
    if (fnNtQueryInfoProcess == nullptr)
        return false;

    /* Set ProcessInformationClass menjadi 7 (ProcessDebugPort) */
    const int procinfoclass = 7;

    NTSTATUS  status;

#if defined (ENV64BIT)
    DWORD64   procinfo = 0;
#elif defined (ENV32BIT)
    DWORD32   procinfo = 0;
#endif 
    
    status = fnNtQueryInfoProcess(GetCurrentProcess(), procinfoclass, &procinfo, sizeof(procinfo), NULL);
    return (status == 0x00000000 && procinfo != 0);
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_NtQueryInformationProcess_ProcessDebugFlags,  "Checking NtQueryInformationProcess : ProcessDebugFlags");
    check(antidebug_NtQueryInformationProcess_ProcessDebugObject, "Checking NtQueryInformationProcess : ProcessDebugObject");
    check(antidebug_NtQueryInformationProcess_ProcessDebugPort,   "Checking NtQueryInformationProcess : ProcessDebugPort");
    
    getchar();
    return 0;
}