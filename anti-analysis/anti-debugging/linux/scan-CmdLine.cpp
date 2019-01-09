/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Parse cmdline di /proc/PPID/cmdline dan deteksi keberadaan debugger.

Compile:
    (gcc)
    $ g++ api-CmdLine.cpp -std=c++11
*/

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

/**
 * Scan terhadap /proc/PPID/cmdline akan memberikan string gdb/ltrace/strace
 * sesuai dengan tools yang digunakan untuk melakukan debugging
 */
bool antidebug_CmdLine_Parse()
{
    char fname[24], buffer[16];
    FILE * f;

    snprintf(fname, 24, "/proc/%d/cmdline", getppid());

    f = fopen(fname, "r");
    fgets(buffer, 16, f);
    fclose(f);

    return strstr(buffer, "gdb") ||  strstr(buffer, "strace") || strstr(buffer, "ltrace");
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.linux.h"

int main(int argc, char* argv[])
{
    check(antidebug_CmdLine_Parse, "Parsing /proc/PPID/cmdline");
    
    getchar();
    return 0;
}