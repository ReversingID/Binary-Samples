/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Parse status di /proc/PPID/status dan deteksi keberadaan debugger.

Compile:
    (gcc)
    $ g++ scan-Status.cpp -std=c++11
*/

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>


/**
 * Scan terhadap /proc/PPID/status akan memberikan string gdb/ltrace/strace
 * sesuai dengan tools yang digunakan untuk melakukan debugging
 */
bool antidebug_Status_Parse()
{
    char fname[24], buffer[16];
    FILE * f;

    snprintf(fname, 24, "/proc/%d/status", getppid());

    f = fopen(fname, "r");
    fgets(buffer, 16, f);
    fclose(f);

    return strstr(buffer, "gdb") ||  strstr(buffer, "strace") || strstr(buffer, "ltrace");
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.linux.h"

int main(int argc, char* argv[])
{
    check(antidebug_Status_Parse, "Parsing /proc/PPID/status");
    
    getchar();
    return 0;
}