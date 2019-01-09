/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Memeriksa siapa parent process.

Compile:
    (msvc)
    $ cl misc-ParentIsExplorer.cpp user32.lib
*/

#include <windows.h>

/*
    Memeriksa parent process. 

    Asumsinya adalah jika parent process bukan explorer maka sesi debugging sedang
    berlangsung.

    Teknik ini bisa membandingkan parent process dengan nama process dari setiap debugger.
*/

/* Process Basic Information untuk process 32-bit dan 64-bit (murni) */
struct PROCESS_BASIC_INFORMATION
{
	PVOID Reserved1;
	void* PebBaseAddress;
	PVOID Reserved2[2];
	ULONG_PTR UniqueProcessId;
	ULONG_PTR ParentProcessId;
};

bool antidebug_ParentIsExplorer()
{
    typedef NTSTATUS(WINAPI * pNtQueryInformationProcess)(HANDLE, UINT, PVOID, ULONG, PULONG); 

    DWORD expid = 0;

    NTSTATUS status = 0;
    PROCESS_BASIC_INFORMATION pbi;

    /* Dapatkan PID explorer dari windows handle */
    GetWindowThreadProcessId(GetShellWindow(), &expid);
    
    /* Dapatkan address ke function */
    auto   dll      = GetModuleHandle("ntdll.dll");
    void * procaddr = GetProcAddress(dll, "NtQueryInformationProcess");
    auto fnNtQueryInfoProcess = static_cast<pNtQueryInformationProcess>(procaddr);
    if (fnNtQueryInfoProcess == nullptr)
        return false;
    
    /* Dapatkan ProcessBasicInformation dengan set param kedua dengan 0 */
    SecureZeroMemory(&pbi, sizeof(PROCESS_BASIC_INFORMATION));
    status = fnNtQueryInfoProcess(GetCurrentProcess(), 0, (PVOID)&pbi, sizeof(PROCESS_BASIC_INFORMATION), 0);

    return (status == 0 && pbi.ParentProcessId != expid);

}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_ParentIsExplorer, "Checking Parent (is Explorer?)");
    
    getchar();
    return 0;
}