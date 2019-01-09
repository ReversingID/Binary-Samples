/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Mendeteksi keberadaan LD_PRELOAD di environment.

Compile:
    (gcc)
    $ g++ env-LdPreload.cpp -std=c++11
*/

#include <cstdlib>

/**
 * Deteksi keberadaan environment variable LD_PRELOAD
 */
bool antidebug_DetectLdPreload_Simple()
{
    return (getenv("LD_PRELOAD") != nullptr);
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.linux.h"

int main(int argc, char* argv[])
{
    check(antidebug_DetectLdPreload_Simple, "Checking env LD_PRELOAD");
    
    getchar();
    return 0;
}