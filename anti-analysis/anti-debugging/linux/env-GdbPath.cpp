/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Parse environment variable dan mendeteksi gdb.

Compile:
    (gcc)
    $ g++ env-GdbPath.cpp -std=c++11
*/

#include <cstdlib>
#include <cstring>

/*
    environment variable "_" menyimpan path dari file atau program yang
    terlibat dalam command sebelumnya.
*/

/**
 * Deteksi keberadaan environment variable
 */
bool antidebug_GdbEnv_Path()
{
    /* Dapatkan env variable */
    char * gdb_path = getenv("_");

    if (gdb_path == nullptr)
        return false;
    
    return (strstr(gdb_path, "gdb") != nullptr);
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.linux.h"

int main(int argc, char* argv[])
{
    check(antidebug_GdbEnv_Path, "Checking env GDB Path");
    
    getchar();
    return 0;
}