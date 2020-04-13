#pragma once
#define _UNICODE

#include <windows.h>

/******************************** DECLARATION ********************************/

bool memsearch (PBYTE needle, size_t nlength, PBYTE haystack, size_t hlength);

/******************************* IMPLEMENTATION *******************************/

/**
 *  Mencari kemunculan "needle" di "haystack"
 */
bool memsearch (PBYTE needle, size_t nlength, PBYTE haystack, size_t hlength)
{
    for (size_t i = 0; i < hlength - nlength; i++)
    {
        if (memcmp(&haystack[i], needle, nlength) == 0)
            return true;
    }

    return false;
}
