/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Mendeteksi keberadaan page exception breakpoint di setiap page yang ada di module.

Compile:
    (msvc)
    $ cl exception-PageException-BreakpointCheck.cpp /EHsc
*/

#include <windows.h>
#include <psapi.h>
#include <vector>

std::vector<PVOID> pages = {};

bool antidebug_PageException_BreakpointCheck();

/**
 * Inisialisasi
 * Kumpulkan daftar semua pages yang ada di module
 */
void PageException_InitList()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    size_t pagesize = sysinfo.dwPageSize;

    HMODULE    main;
    MODULEINFO modinfo;

    MEMORY_BASIC_INFORMATION meminfo = {0};

    /* Dapatkan module utama berdasarkan address dari fungsi di dalam module tersebut */
    if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR) &antidebug_PageException_BreakpointCheck, &main))
    {
        /* dapatkan informasi main module, utamanya ukuran module */
        if (GetModuleInformation(GetCurrentProcess(), main, &modinfo, sizeof(MODULEINFO)))
        {
            /* iterasi per page dan masukkan ke daftar page jika page merupakan executable pages  */
            auto module = reinterpret_cast<unsigned char*>(modinfo.lpBaseOfDll);
            for (auto ofs = 0; ofs < modinfo.SizeOfImage; ofs += pagesize)
            {
                if ((meminfo.Protect & PAGE_EXECUTE)           == PAGE_EXECUTE ||
                    (meminfo.Protect & PAGE_EXECUTE_READ)      == PAGE_EXECUTE_READ ||
                    (meminfo.Protect & PAGE_EXECUTE_WRITECOPY) == PAGE_EXECUTE_WRITECOPY ||
                    (meminfo.Protect & PAGE_EXECUTE_READWRITE) == PAGE_EXECUTE_READWRITE)
                {
                    pages.push_back(module + ofs);
                } 
            }
        }
    }
}


/**
 * Membandingkan executables page yang sudah didapatkan dengan kondisi terkini.
 * Perubahan permission page disebabkan adanya interferensi luar (debugging).
 */
bool antidebug_PageException_BreakpointCheck()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    size_t pagesize = sysinfo.dwPageSize;

    HMODULE    main;
    MODULEINFO modinfo;

    MEMORY_BASIC_INFORMATION meminfo = {0};

    /* ===== periksa jika terdapat pages dengan executable+guard atau noaccess ===== */

    /* Dapatkan module utama berdasarkan address dari fungsi di dalam module tersebut */
    if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR) &antidebug_PageException_BreakpointCheck, &main))
    {
        /* dapatkan informasi main module, utamanya ukuran module */
        if (GetModuleInformation(GetCurrentProcess(), main, &modinfo, sizeof(MODULEINFO)))
        {
            auto module = reinterpret_cast<unsigned char*>(modinfo.lpBaseOfDll);
            for (auto ofs = 0; ofs < modinfo.SizeOfImage; ofs += pagesize)
            {
                if (VirtualQuery(module + ofs, &meminfo, sizeof(MEMORY_BASIC_INFORMATION)) >= sizeof(MEMORY_BASIC_INFORMATION))
                {
                    if ((meminfo.Protect & PAGE_EXECUTE)           == PAGE_EXECUTE ||
                        (meminfo.Protect & PAGE_EXECUTE_READ)      == PAGE_EXECUTE_READ ||
                        (meminfo.Protect & PAGE_EXECUTE_WRITECOPY) == PAGE_EXECUTE_WRITECOPY ||
                        (meminfo.Protect & PAGE_EXECUTE_READWRITE) == PAGE_EXECUTE_READWRITE)
                    {
                        /* page ini executable, check jika terdapat guard */
                        if ((meminfo.Protect & PAGE_GUARD)           == PAGE_GUARD ||
                            (meminfo.AllocationProtect & PAGE_GUARD) == PAGE_GUARD)
                        {
                            /* page ini executable+guard, page exception debuggin terdeteksi */
                            return true;
                        }
                    }

                    if ((meminfo.Protect & PAGE_NOACCESS) == PAGE_NOACCESS)
                    {
                        /* page ini tidak seharusnya NOACCESS (cara lain untuk setup page exception breakpoint) */
                        return true;
                    }
                }
            }
        }

        /* Memeriksa apakah terdapat perubahan proteksi di page */
        for (PVOID page : pages)
        {
            if (VirtualQuery(page, &meminfo, sizeof(MEMORY_BASIC_INFORMATION)) >= sizeof(MEMORY_BASIC_INFORMATION))
            {
                if (!(
                    (meminfo.Protect & PAGE_EXECUTE)           == PAGE_EXECUTE ||
					(meminfo.Protect & PAGE_EXECUTE_READ)      == PAGE_EXECUTE_READ ||
					(meminfo.Protect & PAGE_EXECUTE_WRITECOPY) == PAGE_EXECUTE_WRITECOPY ||
					(meminfo.Protect & PAGE_EXECUTE_READWRITE) == PAGE_EXECUTE_READWRITE))
				{
					/* page awalnya executable namun kini tidak lagi */
					return true;
				}
            }
        }
    }

    return false;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    /* Inisialisasi, daftar semua pages */
    PageException_InitList();

    check(antidebug_PageException_BreakpointCheck, "Checking pages exception breapoints");

    getchar();
    return 0;
}