/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Mendeteksi keberadaan memory breakpoint melalui alokasi page dengan page guard.

Compile:
    (msvc)
    $ cl exception-PageGuardViolation.cpp
*/

#include <windows.h>

/*
    Breakpoint pada alamat tertentu akan mengubah proteksi dari suatu page dengan penambahan
    PAGE_GUARD. Beberapa debugger menangani exception STATUS_GUARD_PAGE_VIOLATION yang dipicu
    oleh aplikasi.

    Exception STATUS_GUARD_PAGE_VIOLATION dapat dipicu secara manual dengan cara mengalokasikan
    page dan menandainya dengan PAGE_GUARD. Eksekusi terhadap page tersebut akan menyebabkan
    STATUS_PAGE_GUARD_VIOLATION.

    Jika tidak ada debugger, ketika eksekusi instruksi RET maka exception STATUS_GUARD_PAGE_VIOLATION
    akan dihasilkan. Sementara jika terdapat debugger, utamanya OllyDbg, eksekusi akan terus berlangsung.
*/

/**
 *  Alokasikan buffer secara dinamis dan tulis instruksi RET ke dalam buffer.
*/
bool antidebug_PageGuardViolation()
{
    SYSTEM_INFO sysinfo = { 0 };
    DWORD oldProtect = 0;
    PVOID buffer = NULL;

    // Dapatkan informasi tentang system
    GetSystemInfo(&sysinfo);

    // Alokasikan memory dengan RWX
    buffer = VirtualAlloc(NULL, sysinfo.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (buffer == NULL)
        return false;
    
    // Tuliskan instruksi RET ke buffer
    RtlFillMemory(buffer, 1, 0xC3);

    // Tandai dengan page guard
    if (VirtualProtect(buffer, sysinfo.dwPageSize, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &oldProtect) == 0)
        return false;
    
    __try 
    {
        // Eksekusi instruksi di buffer
        ((void(*)())buffer)();
    }
    __except (GetExceptionCode() == STATUS_GUARD_PAGE_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
    {
        VirtualFree(buffer, NULL, MEM_RELEASE);
        return false;
    }
    
    VirtualFree(buffer, NULL, MEM_RELEASE);
    return true;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_PageGuardViolation, "Checking Memory Breakpoints (Page Guard)");

    getchar();
    return 0;
}