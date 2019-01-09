/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
   Menyembunyikan thread dari debugger.

Compile:
    (msvc)
    $ cl api-CloseHandle-invalid-handle.cpp
*/

#include <windows.h>

/*
    API NtSetInformationThread()
    ntdll!NtSetInformationThread

    API NtSetInformationThread() merupakan wrapper dari syscall
    ZwSetInformationThread.

    Prototype API ini adalah:

    NTSYSAPI NTSTATUS NTAPI NtSetInformationThread(
        IN HANDLE ThreadHandle,
        IN THREAD_INFORMATION_CLASS ThreadInformationClass,
        IN PVOID ThreadInformation,
        IN ULONG ThreadInformationLength
    );

    Teknik ini dapat diatasi dengan memodifikasi nilai parameter sebelum pemanggilan terjadi.
*/

/**
 *  API NtSetInformationThread() dengan ThreadHideFromDebugger
 *  ntdll!NtSetInformationThread
 * 
 *  Ketika pemanggilan terjadi dengan ThreadInformationClass bernilai 0x11
 *  (ThreadHideFromDebugger) maka thread akan di-detach dari debugger.
 * 
 *  Dengan memberikan NULL pada parameter ThreadHandle maka thread yang sedang berjalan
 *  akan disembunyikan. Aplikasi akan berjalan namun debugger tidak mendapatkan event
 *  terkait thread tersebut.
 * 
 *  Fungsi akan mengembalikan FALSE ketika gagal dan TRUE ketika sukses.
 */
bool antidebug_NtSetInformationThread_ThreadHideFromDebugger()
{
    typedef NTSTATUS(WINAPI * pNtSetInformationThread)(HANDLE, UINT, PVOID, ULONG);
    typedef NTSTATUS(WINAPI * pNtQueryInformationThread)(HANDLE, UINT, PVOID, ULONG, PULONG);
    
    auto   dll           = GetModuleHandle("ntdll.dll");
    void * setinfoproc   = GetProcAddress(dll, "NtSetInformationThread");
    void * queryinfoproc = GetProcAddress(dll, "NtQueryInformationThread");
    auto fnNtSetInformationThread   = static_cast<pNtSetInformationThread>(setinfoproc);
    auto fnNtQueryInformationThread = static_cast<pNtQueryInformationThread>(queryinfoproc);
    
    if (fnNtSetInformationThread == nullptr || fnNtQueryInformationThread == nullptr)
        return false;

    /* Set ThreadInformationClass menjadi 0x11 (ThreadHideFromDebugger) */
    const int ThreadInformationClass = 0x11;

    /* Status */ 
    NTSTATUS status;
    bool     ThreadInformation = false;

    /* 
    Pengecekan #1:  jika parameter ThreadInformationLength diberikan sembarang nilai dan fungsi berhasil maka 
                    dipastikan terdapat hook ke NtSetInformationThread()
    */
    status = fnNtSetInformationThread(GetCurrentThread(), ThreadInformationClass, &ThreadInformation, 135100);
    if (status == 0) return true;
    
    /* 
    Pengecekan #2:  jika parameter ThreadHandle diberikan sembarang nilai dan fungsi berhasil maka 
                    dipastikan terdapat hook ke NtSetInformationThread()
    */
    status = fnNtSetInformationThread((HANDLE) 0xFFFF, ThreadInformationClass, NULL, 0);
    if (status == 0) return true;
    
    /* Pengecekan sesungguhnya */
    status = fnNtSetInformationThread(GetCurrentThread(), ThreadInformationClass, NULL, 0);
    if (status == 0)
    {
        status = fnNtQueryInformationThread(GetCurrentThread(), ThreadInformationClass, &ThreadInformation, 1, NULL);
        if (status == 0)
            return ! ThreadInformation;
    }
    return false;
}

/**
 *  Alternatif: ZwSetInformationThread
 *  ntdll!ZwSetInformationThread
 */
bool antidebug_ZwSetInformationThread_ThreadHideFromDebugger()
{
    typedef NTSTATUS(WINAPI * pZwSetInformationThread)(HANDLE, UINT, PVOID, ULONG);
    typedef NTSTATUS(WINAPI * pZwQueryInformationThread)(HANDLE, UINT, PVOID, ULONG, PULONG);
    
    auto   dll           = GetModuleHandle("ntdll.dll");
    void * setinfoproc   = GetProcAddress(dll, "ZwSetInformationThread");
    void * queryinfoproc = GetProcAddress(dll, "ZwQueryInformationThread");
    auto fnZwSetInformationThread   = static_cast<pZwSetInformationThread>(setinfoproc);
    auto fnZwQueryInformationThread = static_cast<pZwQueryInformationThread>(queryinfoproc);
    
    if (fnZwSetInformationThread == nullptr || fnZwQueryInformationThread == nullptr)
        return false;

    /* Set ThreadInformationClass menjadi 0x11 (ThreadHideFromDebugger) */
    const int ThreadInformationClass = 0x11;

    /* Status */ 
    NTSTATUS status;
    bool     ThreadInformation = false;

    /* 
    Pengecekan #1:  jika parameter ThreadInformationLength diberikan sembarang nilai dan fungsi berhasil maka 
                    dipastikan terdapat hook ke ZwSetInformationThread()
    */
    status = fnZwSetInformationThread(GetCurrentThread(), ThreadInformationClass, &ThreadInformation, 135100);
    if (status == 0) return true;
    
    /* 
    Pengecekan #2:  jika parameter ThreadHandle diberikan sembarang nilai dan fungsi berhasil maka 
                    dipastikan terdapat hook ke ZwSetInformationThread()
    */
    status = fnZwSetInformationThread((HANDLE) 0xFFFF, ThreadInformationClass, NULL, 0);
    if (status == 0) return true;
    
    /* Pengecekan sesungguhnya */
    status = fnZwSetInformationThread(GetCurrentThread(), ThreadInformationClass, NULL, 0);
    if (status == 0)
    {
        status = fnZwQueryInformationThread(GetCurrentThread(), ThreadInformationClass, &ThreadInformation, 1, NULL);
        if (status == 0)
            return ! ThreadInformation;
    }
    return false;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_NtSetInformationThread_ThreadHideFromDebugger, "Calling ntdll!NtSetInformationThread");
    check(antidebug_ZwSetInformationThread_ThreadHideFromDebugger, "Calling ntdll!ZwSetInformationThread");

    getchar();
    return 0;
}