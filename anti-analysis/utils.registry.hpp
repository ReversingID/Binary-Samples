#pragma once
#define _UNICODE

#include <windows.h>

#pragma comment(lib, "advapi32.lib")

/******************************** DECLARATION ********************************/

bool  isRegKeyValExists (HKEY hive, wchar_t * subkey, wchar_t * valname, wchar_t * value);
bool  isRegKeyExists    (HKEY hive, wchar_t * subkey);

/******************************* IMPLEMENTATION *******************************/

/**
 *  Periksa apakah sebuah value terdapat di key tertentu. 
 */
bool isRegKeyValExists (HKEY hive, wchar_t * subkey, wchar_t * valname, wchar_t * value)
{
    HKEY    key;
    BYTE    data[1024] = { 0 };
    wchar_t data2[1024];
    DWORD   datalen = MAX_PATH;
    bool    found = false;

    if (RegOpenKeyExW(hive, subkey, NULL, KEY_READ, &key) == ERROR_SUCCESS)
    {
        if (RegQueryValueExW(key, valname, NULL, NULL, data, &datalen) == ERROR_SUCCESS)
        {
            mbstowcs(data2, (char*)data, sizeof(data));
            found = (wcsstr(data2, value) != NULL);
        }
        
        RegCloseKey(key);
    }

    return found;
}


/**
 *  Periksa apakah sebuah key tertentu ada.
 */
bool isRegKeyExists (HKEY hive, wchar_t * subkey)
{
    HKEY key;
    BYTE data[1024] = { 0 };
    DWORD datalen = MAX_PATH;
    bool found = false;

    if (RegOpenKeyExW(hive, subkey, NULL, KEY_READ, &key) == ERROR_SUCCESS)
    {        
        RegCloseKey(key);
        found = true;
    }

    return found;
}