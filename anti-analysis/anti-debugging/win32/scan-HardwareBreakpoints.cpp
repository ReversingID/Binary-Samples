/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Mendeteksi keberadaan hardware breakpoints dan melakukan sesuatu
    terhadapnya. 

Compile:
    (msvc)
    $ cl scan-HardwareBreakpoints.cpp
*/

#include <windows.h>

/*    
    Hardware Breakpoints adalah teknik yang diimplementasikan oleh Intel 
    untuk melakukan debugging dengan bantuan processor (debugging di level mesin).
    Register DR0 - DR7 akan berisi alamat breakpoint jika hardware breakpoint
    dilakukan.

    Umumnya proteksi dilakukan dengan cara melakukan deteksi terhadap keberadaan
    hardware breakpoints di debug register.
*/


/**
 *  Mendeteksi hardware breakpoint melalui isi debug register.
 * 
 *  Debug register dapat diakses melalui thread context.
 */
bool antidebug_DetectDebugRegisters_GetThreadContext()
{
    /* Struktur yang menyimpan thread context */
    CONTEXT ctx;
    ZeroMemory(&ctx, sizeof(CONTEXT));

    /* set flags sebelum dapat melakukan pembacaan debug registers */
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

    /* dapatkan kondisi register saat ini dan simpan ke ctx */
    if (GetThreadContext(GetCurrentThread(), &ctx) == 0)
        return false;
    
    /* Cek DR0 - DR3 yang digunakan untuk hardware breakpoint */
    return (ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0);
}


/**
 *  Reset nilai dari debug register.
 * 
 *  Dapatkan thread context dan isi semua debug register dengna 0. 
 */
void antidebug_ResetDebugRegisters_SetThreadContext()
{
    CONTEXT ctx;
    ZeroMemory(&ctx, sizeof(CONTEXT));

    /* set flags sebelum dapat melakukan pembacaan debug registers */
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

    /* dapatkan kondisi register saat ini dan simpan ke ctx */
    if (GetThreadContext(GetCurrentThread(), &ctx))
    {
        /* Reset nilai di DR0 - DR3 */
        ctx.Dr0 = ctx.Dr1 = ctx.Dr2 = ctx.Dr3 = 0;

        ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;
        SetThreadContext(GetCurrentThread(), &ctx);
    }
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_DetectDebugRegisters_GetThreadContext, "Checking Hardware Breakpoints (GetThreadContext");

    getchar();
    return 0;
}