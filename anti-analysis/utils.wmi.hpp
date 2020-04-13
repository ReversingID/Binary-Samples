#pragma once
#define _UNICODE

#include <windows.h>
#include <WbemCli.h>

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "OleAut32.lib")
#pragma comment(lib, "wbemuuid.lib")

/******************************** DECLARATION ********************************/

bool InitWMI (IWbemServices ** svc, IWbemLocator ** loc, wchar_t * netresource);
bool ExecWMIQuery (IWbemServices ** svc, IWbemLocator ** loc, IEnumWbemClassObject **enumerator, wchar_t * query);

/******************************* IMPLEMENTATION *******************************/

/**
 *  Inisialisasi WMI sebelum Query
 */
bool InitWMI (IWbemServices ** svc, IWbemLocator ** loc, wchar_t * netresource)
{
    // inisialisasi COM
    HRESULT result;
    
    result = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(result))
        return false;
    
    // Set COM security level
    result = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    if (FAILED(result))
    {
        CoUninitialize();
        return false;
    }

    // dapatkan locator WMI
    result = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(loc));
    if (FAILED(result))
    {
        CoUninitialize();
        return false;
    }

    // connect ke namespace root\cimv2 
    result = (*loc)->ConnectServer(netresource, NULL, NULL, 0, NULL, 0, 0, svc);
    if (FAILED(result))
    {
        (*loc)->Release();
        CoUninitialize();
        return false;
    }

    // set security level pada proxy
    result = CoSetProxyBlanket(*svc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    if (FAILED(result))
    {
        (*svc)->Release();
        (*loc)->Release();
        CoUninitialize();
        return false;
    }

    return true;
}


/**
 *  Eksekusi WMI Query
 */
bool ExecWMIQuery (IWbemServices ** svc, IWbemLocator ** loc, IEnumWbemClassObject **enumerator, wchar_t * query)
{
    // eksekusi WMI Query
    HRESULT result;

    result = (*svc)->ExecQuery(L"WQL", query, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, enumerator);
    if (FAILED(result))
    {
        (*svc)->Release();
        (*loc)->Release();
        CoUninitialize();
        return false;
    }

    return true;
}
