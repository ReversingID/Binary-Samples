/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Menggunakan ptrace untuk mendeteksi keberadaan debugger.

Compile:
    (gcc)
    $ g++ api-Ptrace-Self.cpp -std=c++11
*/

#include <cstdlib>
#include <sys/ptrace.h>

/*
    System call ptrace digunakan oleh process (tracer) untuk mengamati (observe)
    dan mengendalikan (control) eksekusi dari process lain (tracee). Namun hanya
    boleh ada satu tracer yang mengendalikan tracee dalam satu waktu.

    Semua debugger dan program tracer menggunakan ptrace untuk attach dan debug
    process. Jika process melakukan ptrace kepada diri sendiri dengan PTRACE_TRACEME,
    maka ptrace akan melakukan setup parent process sebagai tracer. Jika debugger
    sudah attach ke debuggee, maka pemanggilan ptrace akan gagal.
  
    Terdapat dua alternatif cara untuk mengatasi teknik ini:
        - patch pemanggilan ptrace(2) dengan NOP
        - overwrite fungsi ptrace(2) dengan preload ELF shared binary 
          (menggunakan LD_PRELOAD).
*/

/**
 * Lakukan ptrace sekali untuk mendeteksi adanya ptrace.
 */
bool antidebug_SelfPtrace()
{
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
        return true;
    
    ptrace(PTRACE_DETACH, 0, 0, 0);
    return false;
}


/**
 * Lakukan ptrace dua kali untuk mendeteksi adanya ptrace.
 * 
 * Normalnya jika tidak ada ptrace yang terjadi maka pemanggilan pertama
 * akan berhasil sementara pemanggilan kedua akan gagal.
 */
bool antidebug_SelfPtrace_Twice()
{
    int result = 0;

    /* pemanggilan pertama */
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == 0)
    {
        result = 2;
    }

    /* pemanggilan kedua */
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
    {
        result *= 3;
        ptrace(PTRACE_DETACH, 0, 0, 0);
    }
    
    /* eksekusi normal terjadi jika kedua pemanggilan ptrace berhasil */
    return (result != 2 * 3);
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.linux.h"

int main(int argc, char* argv[])
{
    // check(antidebug_SelfPtrace, "Checking Self Ptrace (1)");
    check(antidebug_SelfPtrace_Twice, "Checking Self Ptrace (2)");
    
    getchar();
    return 0;
}