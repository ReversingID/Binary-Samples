/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Mendeteksi keberadaan file descriptor untuk mengetahui keberadaan
    GDB.

Compile:
    (gcc)
    $ g++ scan-GdbFileDescriptor.cpp -std=c++11
*/

#include <cstdio>

/*
    Ini merupakan bug lawas (#7912).

    GDB leak 2 file descriptor ketika GDB membuka program untuk di-debug.
    Kedua file descriptor mengarah kepada file debugee.
*/

/**
 * Mencoba membuka file dan melihat file descriptor yang diberikan.
 */
bool antidebug_DetectFileDescriptor()
{
    FILE * fd  = fopen("/", "r");
    int  nb_fd = fileno(fd);
    fclose(fd);

    return (nb_fd > 3);
}

/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.linux.h"

int main(int argc, char* argv[])
{
    check(antidebug_DetectFileDescriptor, "Checking GDB file descriptor leak");
    
    getchar();
    return 0;
}