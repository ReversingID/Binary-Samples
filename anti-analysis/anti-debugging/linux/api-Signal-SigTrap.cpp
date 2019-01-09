/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Deteksi GDB dengan SIGTRAP

Compile:
    (gcc)
    $ g++ api-Signal-SigTrap.cpp -std=c++11
*/

#include <signal.h>

/*
    GDB menangani signal SIGTRAP.

    Jika SIGTRAP diberikan namun tidak dapat ditangani oleh handle di
    dalam aplikasi, maka dipastikan bahwa GDB atau debugger lain yang
    telah menangkap SIGTRAP tersebut.
*/

bool SwallowedException;

/**
 * Picu SIGTRAP dan lihat apakah penanganan SIGTRAP dilakukan oleh handle
 * di dalam aplikasi atau tidak.
 */
static void sigtrap_handler(int signum)
{ 
    SwallowedException = false; 
}
bool antidebug_SigTrap_RaiseHandle()
{
    SwallowedException = true;

    signal(SIGTRAP, sigtrap_handler);
    raise(SIGTRAP);
    signal(SIGTRAP, SIG_IGN);

    return SwallowedException;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.linux.h"

int main(int argc, char* argv[])
{
    check(antidebug_SigTrap_RaiseHandle, "Raising and handling SIGTRAP");
    
    getchar();
    return 0;
}