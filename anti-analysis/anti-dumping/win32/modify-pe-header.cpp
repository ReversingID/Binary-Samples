/*
    Archive of Reversing.ID
    Sample Code for Anti Dumping

Description:
    Ubah field di PE Header.

    Proses dumping dilakukan dengan melakukan rekonstruksi executable berdasarkan informasi 
    dari PE header.
    Memodifikasi PE header menyebabkan process dump menghasilkan property yang keliru.

References:
    LDR_DATA_TABLE_ENTRY menyimpan informasi module yang dmuat di proses.
    https://docs.microsoft.com/en-us/windows/desktop/api/winternl/ns-winternl-_peb_ldr_data


Compile:
    (msvc)
    $ cl modify-pe-header.cpp
*/
#include <windows.h>
#include <winternl.h>


/** Menentukan versi Windows (32/64 bit) **/
/* MSVC */
#if _WIN32 || _WIN64
#   if _WIN64 
#       define ENV64BIT
#   else 
#       define ENV32BIT
#   endif 
#endif 

/**
 * Modifikasi bagian dari 
 * 
 * Langkah:
 *  - dapatkan alamat PEB
 *  - dapatkan entry dari table
 *  - modifikasi base address.
 */
VOID antidump_ModifyPEHeader_SizeofImage()
{
    PPEB peb;
    
#if defined (ENV64BIT)
    peb = (PPEB) __readgsqword(0x60);
#elif defined (ENV32BIT)
    peb = (PPEB) __readfsdword(0x30);
#endif  

    /* Mendapatkan entry dari table */
    PLDR_DATA_TABLE_ENTRY entry = (PLDR_DATA_TABLE_ENTRY)(peb->Ldr->InMemoryOrderModuleList.Flink);
    
    /* Modifikasi ukuran image */
    entry->DllBase = (PVOID)((INT_PTR)entry->DllBase + 0x100000);
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include <stdio.h>

int main(int argc, char* argv[])
{
    printf("Modify the PE Header ...");
    getchar();

    printf("[*] SizeOfImage");
    antidump_ModifyPEHeader_SizeofImage();

    printf("Removal done ...");
    getchar();
    return 0;
}