/*
    Archive of Reversing.ID
    Sample Code for Anti Dumping

Description:
   Hapus PE header dari module executable di memory.

   Ketika executable dimuat ke memory, OS loader akan melakukan pemetaan executable serta library pendukung
   sebagai module. Proses dumping dilakukan dengan melakukan rekonstruksi executable berdasarkan informasi 
   dari PE header.


Compile:
    (msvc)
    $ cl erase-pe-header.cpp
*/
#include <windows.h>


/**
 * Langkah:
 *  - mendapatkan alamat awal (base) dari module executable (main module)
 *  - memodifikasi permission dari page agar dapat ditulisi (writeable)
 *  - menimpa page dengan 0 (atau nilai lain)
 */
VOID antidump_ErasePEHeaderFromMemory()
{
    DWORD protect = 0;

    /* Dapatkan base address dari module EXE */
    char * base_address = (char*) GetModuleHandle(NULL);

    /* Ubah proteksi agar dapat ditulisi */
    VirtualProtect(base_address, 4096, PAGE_READWRITE, &protect);

    /* Timpa header dengan 0 */
    SecureZeroMemory(base_address, 4096);
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include <stdio.h>

int main()
{
    printf("[*] Removing the PE Header ...");
    getchar();

    antidump_ErasePEHeaderFromMemory();

    printf("Removal done ...");
    getchar();
    return 0;
}