/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Mendeteksi keberadaan software breakpoints dan melakukan sesuatu
    terhadapnya.

Compile:
    (gcc)
    $ g++ scan-SoftwareBreakpoints.cpp -std=c++11
*/

#include <cstddef>

extern unsigned char * _start;
extern unsigned char * __etext;

/*
    Software breakpoints direpresentasikan sebagai instruksi INT 3 atau interrupt
    dengan opcode 0xCC.

    Proteksi dan teknik anti analisis ini bertujuan untuk mendeteksi kemunculan
    instruksi 0xCC pada rentang alamat memori tertentu. Komparasi dapat dilakukan
    secara langsung (membandingkan tiap byte dengan 0xCC) maupun tidak langsung
    (membandingkan hasil dari xor byte dengan nilai tertentu).

    Meskipun tidak lazim, terdapat pula bentuk lain dari 0xCC yaitu 0xCD 0x03
*/

/* Sample bagian kode yang diproteksi. */
void CriticalFunction()
{
    int a = 135;
    int b = 10;
    int c = 30;
    int d = (a * 100 + b) * 1000 + c; 
}

void NextFunction()
{
    CriticalFunction();
}


/**
 *  Mendeteksi software breakpoint di alamat memori pada rentang tertentu.
 * 
 *  Tidak ada jaminan untuk menentukan ukuran dari fungsi saat runtime.
 * 
 *  Pendekatan ini menggunakan fungsi lain sebagai batas akhir pencarian.
 *  Fungsi yang dipilih haruslah fungsi yang berada setelah fungsi yang diinginkan.
 * 
 *  Ada kemungkinan false positive jika terdapat instruksi dengan byte 0xCC sebagai argument.
 */
bool antidebug_DetectBreakpoint_FuncBorder()
{
    size_t AddressLength = reinterpret_cast<size_t>(NextFunction) - reinterpret_cast<size_t>(CriticalFunction);
    auto   pCriticalFunction = reinterpret_cast<unsigned char *>(CriticalFunction);

    for (auto i = 0; i < AddressLength; i++)
    {
        if (pCriticalFunction[i] == 0xCC)
            return true;
    }
    return false;
}


/**
 *  Mendeteksi software breakpoint di alamat memory sampai ditemukan instruksi RET.
 * 
 *  Terdapat beberapa representasi instruksi RET di x86.
 *       C3      => near return
 *       CB      => far return
 *       C2 iw   => near return dengan pop iw bytes dari stack
 *       CA iw   => far return dengan pop iw bytes dari stack
 * 
 *  Sesuaikan dengan kondisi fungsi.
 * 
 *  Ada kemungkinan false positive jika terdapat instruksi dengan byte 0xCC sebagai argument.
 */
bool antidebug_DetectBreakpoint_ScanUntilRet()
{
    auto   pCriticalFunction = reinterpret_cast<unsigned char *>(CriticalFunction);
    size_t i = 0;

    while (pCriticalFunction[i] != 0xC3 && pCriticalFunction[i] != 0xCB)
    {
        if (pCriticalFunction[i] == 0xCC)
            return true;
        i++;
    }
    return false;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.linux.h"

int main(int argc, char* argv[])
{
    check(antidebug_DetectBreakpoint_FuncBorder, "Checking Software Breakpoints (Function-Bound)");
    check(antidebug_DetectBreakpoint_ScanUntilRet, "Checking Software Breakpoints (Scan until RET)");

    getchar();
    return 0;
}