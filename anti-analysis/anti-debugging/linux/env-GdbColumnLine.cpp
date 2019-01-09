/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Parse environment variable dan mendeteksi gdb.

Compile:
    (gcc)
    $ g++ env-GdbColumnLine.cpp -std=c++11
*/

#include <cstdlib>

/**
 * Deteksi keberadaan environment variable
 */
bool antidebug_GdbEnv_ColumnLine()
{
    return ((getenv("COLUMNS") != nullptr) || (getenv("LINES") != nullptr));
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.linux.h"

int main(int argc, char* argv[])
{
    check(antidebug_GdbEnv_ColumnLine, "Checking env GDB COLUMNS-LINES");
    
    getchar();
    return 0;
}