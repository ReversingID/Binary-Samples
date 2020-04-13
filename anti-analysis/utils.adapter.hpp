#pragma once
#define _UNICODE

#include <windows.h>
#include <iptypes.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")

/******************************** DECLARATION ********************************/

void  lowercase(char * str);
void  lowercase(wchar_t * str);

bool  checkMacAddress(wchar_t * mac);
bool  checkAdapterName(wchar_t * name);

/******************************* IMPLEMENTATION *******************************/

void lowercase(char * str)
{
    for (size_t i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

void lowercase(wchar_t * str)
{
    for (size_t i = 0; str[i]; i++)
        str[i] = towlower(str[i]);
}

// ----------------------------------------------------------------------------

/**
 *  Periksa apakah terdapat adapter dengan MAC address tertentu
 */
bool checkMacAddress(wchar_t * mac)
{
    bool    result = false;
    PIP_ADAPTER_INFO adapter, iter;
    ULONG   length = sizeof(IP_ADAPTER_INFO);
    char    cmac[4];

    /*
        Alokasikan ruang yang cukup untuk menampung adapter.
        Jika adapter lebih dari satu maka perlu dilakukan penyesuaian
        dengan mengubah ukuran buffer.
    */
    adapter = (PIP_ADAPTER_INFO) malloc(sizeof(IP_ADAPTER_INFO));
    if (adapter == NULL)
        return false;
    
    if (GetAdaptersInfo(adapter, &length) == ERROR_BUFFER_OVERFLOW)
    {
        adapter = (PIP_ADAPTER_INFO) realloc(adapter, length);
        if (adapter == NULL)
            return false;
    }

    if (GetAdaptersInfo(adapter, &length) == ERROR_SUCCESS)
    {
        iter = adapter;
        while (iter)
        {
            wcstombs (cmac, mac, 3);
            if (iter->AddressLength == 6 && !memcmp(cmac, iter->Address, 3))
            {
                result = true;
                break;
            }
            iter = iter->Next;
        }

        free(adapter);
    }

    return result;
}


/**
 *  Periksa apakah terdapat adapter dengan nama tertentu.
 */
bool checkAdapterName(wchar_t * name)
{
    bool    result = false;
    PIP_ADAPTER_INFO adapter, iter;
    ULONG   length = sizeof(IP_ADAPTER_INFO);
    char    cname[32];

    /*
        Alokasikan ruang yang cukup untuk menampung adapter.
        Jika adapter lebih dari satu maka perlu dilakukan penyesuaian
        dengan mengubah ukuran buffer.
    */
    adapter = (PIP_ADAPTER_INFO) malloc(sizeof(IP_ADAPTER_INFO));
    if (adapter == NULL)
        return false;
    
    if (GetAdaptersInfo(adapter, &length) == ERROR_BUFFER_OVERFLOW)
    {
        adapter = (PIP_ADAPTER_INFO) realloc(adapter, length);
        if (adapter == NULL)
            return false;
    }

    if (GetAdaptersInfo(adapter, &length) == ERROR_SUCCESS)
    {
        iter = adapter;
        while (iter)
        {
            wcstombs(cname, name, sizeof(cname));
            lowercase(iter->Description);
            lowercase(cname);

            if (strstr(iter->Description, cname) != NULL)
            {
                result = true;
                break;
            }
            iter = iter->Next;
        }

        free(adapter);
    }

    return result;
}