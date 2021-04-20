/*
    Archive of Reversing.ID
    Sample Code for Anti Emulation

Description:
    Deteksi VM VirtualBox berdasarkan artifak.
    
Compile:
    (msvc)
    $ cl vm-virtualbox.cpp
*/
#include <windows.h> 

#include "../../utils.common.hpp"
#include "../../utils.registry.hpp"
#include "../../utils.fs.hpp"
#include "../../utils.adapter.hpp"
#include "../../utils.process.hpp"
#include "../../utils.wmi.hpp"
#include "../../utils.firmware.hpp"

// FindWindowW
#pragma comment(lib, "user32.lib")

// WNetGetProviderNameW
#pragma comment(lib, "Mpr.lib")



/**
 *  Inspeksi nilai (value) dari registry key tertentu
 */
bool antivm_vbox_regkeyval ()
{
    /* Daftar registry key values yang terdapat di lingkungan VirtualBox */
    wchar_t * entries[][3] = {
        // SUBKEY                          VALUE NAME           VALUE
        { L"HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", L"Identifier", L"VBOX" },
        { L"HARDWARE\\Description\\System", L"SystemBiosVersion", L"VBOX" },
        { L"HARDWARE\\Description\\System", L"VideoBiosVersion",  L"VIRTUALBOX" },
        { L"HARDWARE\\Description\\System", L"SystemBiosDate",    L"06/23/99" },
    };

    size_t length = sizeof(entries) / sizeof(entries[0]);
    for (size_t i = 0; i < length; i++)
    {
        if (isRegKeyValExists(HKEY_LOCAL_MACHINE, entries[i][0], entries[i][1], entries[i][2]))
            return true;
    }

    return false;
}


/**
 *  Deteksi keberadaan registry keys tertentu
 */
bool antivm_vbox_regkey()
{
    /* Daftar registry key yang terdapat di lingkungan VirtualBox */
    wchar_t * entries[] = {
        L"HARDWARE\\ACPI\\DSDT\\VBOX__",
        L"HARDWARE\\ACPI\\FADT\\VBOX__",
        L"HARDWARE\\ACPI\\RSDT\\VBOX__",
        L"SOFTWARE\\Oracle\\VirtualBox Guest Additions",
        L"SYSTEM\\ControlSet001\\Services\\VBoxGuest",
        L"SYSTEM\\ControlSet001\\Services\\VBoxMouse",
        L"SYSTEM\\ControlSet001\\Services\\VBoxService",
        L"SYSTEM\\ControlSet001\\Services\\VBoxSF",
        L"SYSTEM\\ControlSet001\\Services\\VBoxVideo"
    };

    size_t length = sizeof(entries) / sizeof(entries[0]);
    for (size_t i = 0; i < length; i++)
    {
        if (isRegKeyExists(HKEY_LOCAL_MACHINE, entries[i]))
            return true;
    }

    return false;
}


/**
 *  Deteksi keberadaan files default VirtualBox.
 */
bool antivm_vbox_files()
{
    /* Daftar path yang terdapat di lingkungan VirtualBox */
    wchar_t * entries[] = {
        L"system32\\drivers\\VBoxMouse.sys",
        L"system32\\drivers\\VBoxGuest.sys",
        L"system32\\drivers\\VBoxSF.sys",
        L"system32\\drivers\\VBoxVideo.sys",
        L"system32\\vboxdisp.dll",
        L"system32\\vboxhook.dll",
        L"system32\\vboxmrxnp.dll",
        L"system32\\vboxogl.dll",
        L"system32\\vboxoglarrayspu.dll",
        L"system32\\vboxoglcrutil.dll",
        L"system32\\vboxoglerrorspu.dll",
        L"system32\\vboxoglfeedbackspu.dll",
        L"system32\\vboxoglpackspu.dll",
        L"system32\\vboxoglpassthroughspu.dll",
        L"system32\\vboxservice.exe",
        L"system32\\vboxtray.exe",
        L"system32\\VBoxControl.exe",
    };

    size_t length = sizeof(entries) / sizeof(entries[0]);

    wchar_t windir[MAX_PATH];
    wchar_t path[MAX_PATH];

    /* Dapatkan direktori windows */
    GetWindowsDirectoryW(windir, MAX_PATH);

    for (size_t i = 0; i < length; i++)
    {
        PathCombineW(path, windir, entries[i]);
        if (isFileExists(path))
            return true;
    }

    return false;
}


/**
 *  Deteksi keberadaan direktori default VirtualBox (guest addition)
 */
bool antivm_vbox_dirs()
{
    wchar_t programfiles[MAX_PATH];
    wchar_t path[MAX_PATH];
    wchar_t target[MAX_PATH] = L"oracle\\virtualbox guest additions\\";

    if (isWow64())
        ExpandEnvironmentStringsW(L"%ProgramW6432%", programfiles, MAX_PATH);
    else 
        SHGetSpecialFolderPathW(NULL, programfiles, CSIDL_PROGRAM_FILES, FALSE);

    PathCombineW(path, programfiles, target);
    return isDirExists(path);
}


/**
 *  Periksa MAC address yang digunakan.
 *  Jika tidak ada perubahan, maka MAC address akan menggunakan address
 *  yang termasuk ke dalam vendor VirtualBox
 */
bool antivm_vbox_macaddr ()
{
    // PCS Systemtechnik CmbH (VirtualBox)
    return checkMacAddress(L"\x08\x00\x27");
}


/**
 *  Periksa keberadaan pseudo-devices
 */
bool antivm_vbox_devices ()
{
    wchar_t * entries[] = {
        L"\\\\.\\VBoxMiniRdrDN",
        L"\\\\.\\VBoxGuest",
        L"\\\\.\\pipe\\VBoxMiniRdDN",
        L"\\\\.\\VBoxTrayIPC",
        L"\\\\.\\pipe\\VBoxTrayIPC"
    };
    bool found = false;

    size_t length = sizeof(entries) / sizeof(entries[0]);
    for (size_t i = 0; i < length; i++)
    {
        HANDLE f = CreateFileW(entries[i], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (f != INVALID_HANDLE_VALUE)
        {
            CloseHandle(f);
            found = true;
            break;
        }
    }

    return found;
}


/**
 *  Periksa keberadaan Window Class
 */
bool antivm_vbox_window_class ()
{
    HWND wclass = FindWindowW(L"VBoxTrayToolWndClass", NULL);
    HWND window = FindWindowW(NULL, L"VBoxTrayToolWnd");

    return (wclass || window);
}


/**
 *  Periksa keberadaan shared folders
 */
bool antivm_vbox_network_share ()
{
    wchar_t provider[MAX_PATH];
    DWORD   bufsize;

    if (WNetGetProviderNameW(WNNC_NET_RDR2SAMPLE, provider, &bufsize) == NO_ERROR)
        return (wcscmp(provider, L"VirtualBox Shared Folders") == 0);
    
    return false;
}


/**
 *  Periksa keberadaan nama-nama process yang terkait dengan VirtualBox.
 */
bool antivm_vbox_process ()
{
    wchar_t * entries [] = {
        L"vboxservice.exe",
        L"vboxtray.exe"
    };

    size_t length = sizeof(entries) / sizeof(entries[0]);
    for (size_t i = 0; i < length; i++)
    {
        if (getPidFromName(entries[i]))
            return true;
    }
    return false;
}


/**
 *  Periksa device dengan WMI
 */
bool antivm_vbox_wmi ()
{
    IWbemServices * svc = NULL;
    IWbemLocator  * loc = NULL;
    IEnumWbemClassObject * enumerator = NULL;
    HRESULT result;
    bool    found = false;

    // Inisialisasi WMI
    if (InitWMI (&svc, &loc, L"ROOT\\CIMV2"))
    {
        // eksekusi Query
        if (ExecWMIQuery(&svc, &loc, &enumerator, L"SELECT * FROM Win32_PnPEntity"))
        {
            IWbemClassObject * iter = NULL;
            ULONG   retval = 0;
            VARIANT prop;

            while (enumerator)
            {
                result = enumerator->Next(WBEM_INFINITE, 1, &iter, &retval);
                if (retval == 0) break;

                // dapatkan nilai dari Name Property
                result = iter->Get(L"DeviceId", 0, &prop, 0, 0);

                // bandingkan 
                if (wcsstr(prop.bstrVal, L"PCI\\VEN_80EE&DEV_CAFE") != 0)
                {
                    found = true;
                    break;
                }

                VariantClear(&prop);
                iter->Release();
            }

            // cleanup
            enumerator->Release();
            svc->Release();
            loc->Release();
            CoUninitialize();
        }
    }

    return found;
}


/**
 *  Periksa MAC Address dengan WMI
 */
bool antivm_vbox_wmi_mac ()
{
    IWbemServices * svc = NULL;
    IWbemLocator  * loc = NULL;
    IEnumWbemClassObject * enumerator = NULL;
    HRESULT result;
    bool    found = false;

    // Inisialisasi WMI
    if (InitWMI (&svc, &loc, L"ROOT\\CIMV2"))
    {
        // eksekusi Query
        if (ExecWMIQuery(&svc, &loc, &enumerator, L"SELECT * FROM Win32_NetworkAdapterConfiguration"))
        {
            IWbemClassObject * iter = NULL;
            ULONG   retval = 0;
            VARIANT prop;

            while (enumerator)
            {
                result = enumerator->Next(WBEM_INFINITE, 1, &iter, &retval);
                if (retval == 0) break;

                // dapatkan nilai dari Name Property
                result = iter->Get(L"MACAddress", 0, &prop, 0, 0);
                if (V_VT(&prop) != VT_NULL)
                {
                    if (wcsstr(prop.bstrVal, L"08:00:27") != 0)
                    {
                        found = true;
                        break;
                    }

                    VariantClear(&prop);
                    iter->Release();
                }
            }

            // cleanup
            enumerator->Release();
            svc->Release();
            loc->Release();
            CoUninitialize();
        }
    }

    return found;
}


/**
 *  Periksa Event Log dengan WMI
 */
bool antivm_vbox_wmi_evtlog ()
{
    IWbemServices * svc = NULL;
    IWbemLocator  * loc = NULL;
    IEnumWbemClassObject * enumerator = NULL;
    HRESULT result;
    bool    found = false;

    // Inisialisasi WMI
    if (InitWMI (&svc, &loc, L"ROOT\\CIMV2"))
    {
        // eksekusi Query
        if (ExecWMIQuery(&svc, &loc, &enumerator, L"SELECT * FROM Win32_NTEventlogFile"))
        {
            IWbemClassObject * iter = NULL;
            ULONG   retval = 0;
            VARIANT prop;

            while (enumerator && !found)
            {
                result = enumerator->Next(WBEM_INFINITE, 1, &iter, &retval);
                if (retval == 0) break;

                // dapatkan nilai dari Name Property
                result = iter->Get(L"FileName", 0, &prop, 0, 0);
                if (V_VT(&prop) != VT_NULL)
                {
                    if (wcsstr(prop.bstrVal, L"System") != 0)
                    {
                        // dapatkan Source Property
                        VariantClear(&prop);
                        result = iter->Get(L"Sources", 0, &prop, 0, 0);

                        // dapatkan jumlah elemen dari SAFEARRAY
                        SAFEARRAY * sources = prop.parray;
                        LONG * vals;
                        HRESULT  hres;

                        // akses langsung ke SA memory
                        hres = SafeArrayAccessData(sources, (VOID**) &vals);
                        if (SUCCEEDED(hres))
                        {
                            LONG lbound, ubound;
                            SafeArrayGetLBound(sources, 1, &lbound);
                            SafeArrayGetUBound(sources, 1, &ubound);
                            LONG length = ubound - lbound + 1;

                            wchar_t * item;
                            for (LONG idx = 0; idx < length; idx++)
                            {
                                SafeArrayGetElement(sources, &idx, (void*) &item);
                                if (wcscmp(item, L"vboxvideo") == 0)
                                {
                                    found = true;
                                    break;
                                }
                            } 
                        }
                    }

                    VariantClear(&prop);
                    iter->Release();
                }
            }

            // cleanup
            enumerator->Release();
            svc->Release();
            loc->Release();
            CoUninitialize();
        }
    }

    return found;
}

/**
 *  Periksa PnP (Plug and Play) entity untuk PCI Device
 */
bool antivm_vbox_wmi_pnpentity_pcideviceid()
{
    IWbemServices * svc = NULL;
    IWbemLocator  * loc = NULL;
    IEnumWbemClassObject * enumerator = NULL;
    HRESULT result;
    bool    found = false;

    // Inisialisasi WMI
    if (InitWMI(&svc, &loc, L"ROOT\\CIMV2"))
    {
        // eksekusi Query
        if (ExecWMIQuery(&svc, &loc, &enumerator, L"SELECT * FROM Win32_PnPEntity"))
        {
            IWbemClassObject * iter = NULL;
            ULONG   retval = 0;
            VARIANT vtProp;

            while (enumerator && !found)
            {
                result = enumerator->Next(WBEM_INFINITE, 1, &iter, &retval);
                if (retval == 0) break;

                // dapatkan value dari property Name
                result = iter->Get(L"DeviceId", 0, &vtProp, 0, 0);

                if (SUCCEEDED(result)) 
                {
                    if (vtProp.vt == VT_BSTR) 
                    {
                        if (wcsstr(vtProp.bstrVal, L"PCI\\VEN_80EE&DEV_CAFE") != 0)
                            found = true;
                    }
                    VariantClear(&vtProp);
                }
                
                iter->Release();
            }

            // cleanup
            enumerator->Release();
            svc->Release();
            loc->Release();
            CoUninitialize();
        }
    }

    return found;
}

/**
 *  Periksa PnP (Plug n Play) entity untuk hardware VirtualBox
 */
bool antivm_vbox_wmi_pnpentity_controllers()
{
    IWbemServices * svc = NULL;
    IWbemLocator  * loc = NULL;
    IEnumWbemClassObject * enumerator = NULL;
    HRESULT result;
    bool    found = false;

    // Inisialisasi WMI
    if (InitWMI(&svc, &loc, L"ROOT\\CIMV2"))
    {
        // eksekusi Query
        if (ExecWMIQuery(&svc, &loc, &enumerator, L"SELECT * FROM Win32_PnPEntity"))
        {
            // Get the data from the query
            IWbemClassObject * iter = NULL;
            ULONG   retval = 0;
            VARIANT vtProp;

            int findCount = 0;
            const int findThreshold = 3;

            while (enumerator)
            {
                result = enumerator->Next(WBEM_INFINITE, 1, &iter, &retval);
                if (retval == 0) break;

                // dapatkan value dari property Name
                result = iter->Get(L"Name", 0, &vtProp, 0, 0);
                if (SUCCEEDED(result)) 
                {
                    if (V_VT(&vtProp) != VT_NULL)
                    {
                        if ((vtProp.vt & VT_BSTR) == VT_BSTR)
                        {
                            // naikkan counter jika instance cocok dengan nama VBox
                            if (wcsstr(vtProp.bstrVal, L"82801FB") != 0)
                                findCount++;

                            else if (wcsstr(vtProp.bstrVal, L"82441FX") != 0)
                                findCount++;
                                
                            else if (wcsstr(vtProp.bstrVal, L"82371SB") != 0)
                                findCount++;

                            else if (wcsstr(vtProp.bstrVal, L"OpenHCD") != 0)
                                findCount++;
                        }

                        VariantClear(&vtProp);
                    }
                }
                iter->Release();
            }

            if (findCount >= findThreshold)
                found = true;

            // Cleanup
            enumerator->Release();
            svc->Release();
            loc->Release();
            CoUninitialize();
        }
    }

    return found;
}

/**
 *  periksa PnP (Plug n Play) untuk device VBOX
 */
bool antivm_vbox_wmi_pnpentity_vboxname()
{
    IWbemServices * svc = NULL;
    IWbemLocator  * loc = NULL;
    IEnumWbemClassObject * enumerator = NULL;
    HRESULT result;
    bool    found = false;

    // Inisialisasi WMI
    if (InitWMI(&svc, &loc, L"ROOT\\CIMV2"))
    {
        // eksekusi Query
        if (ExecWMIQuery(&svc, &loc, &enumerator, L"SELECT * FROM Win32_PnPDevice"))
        {
            IWbemClassObject * iter = NULL;
            ULONG   retval = 0;
            VARIANT vtProp;

            while (enumerator && !found)
            {
                result = enumerator->Next(WBEM_INFINITE, 1, &iter, &retval);
                if (retval == 0) break;

                // dapatkan value dari property Name
                result = iter->Get(L"Name", 0, &vtProp, 0, 0);
                if (SUCCEEDED(result))
                {
                    if (V_VT(&vtProp) != VT_NULL)
                    {
                        if ((vtProp.vt & VT_BSTR) == VT_BSTR)
                        {
                            if (wcsstr(vtProp.bstrVal, L"VBOX") != 0)
                                found = true;
                        }

                        VariantClear(&vtProp);
                    }
                }

                // dapatkan value dari property Caption
                result = iter->Get(L"Caption", 0, &vtProp, 0, 0);
                if (SUCCEEDED(result))
                {
                    if (V_VT(&vtProp) != VT_NULL)
                    {
                        if ((vtProp.vt & VT_BSTR) == VT_BSTR)
                        {
                            if (wcsstr(vtProp.bstrVal, L"VBOX") != 0)
                                found = true;
                        }

                        VariantClear(&vtProp);
                    }
                }

                // dapatkan value dari property PNPDeviceID
                result = iter->Get(L"PNPDeviceID", 0, &vtProp, 0, 0);
                if (SUCCEEDED(result))
                {
                    if (V_VT(&vtProp) != VT_NULL)
                    {
                        if ((vtProp.vt & VT_BSTR) == VT_BSTR)
                        {
                            if (wcsstr(vtProp.bstrVal, L"VEN_VBOX") != 0)
                                found = true;
                        }

                        VariantClear(&vtProp);
                    }
                }

                iter->Release();
            }

            // Cleanup
            enumerator->Release();
            svc->Release();
            loc->Release();
            CoUninitialize();
        }
    }

    return found;
}

/**
 *  periksa bus apakah hanya terdapat ACPIBus_BUS_0, PCI_BUS_0, PNP_BUS_0
 */
bool antivm_vbox_bus()
{
    IWbemServices * svc = NULL;
    IWbemLocator  * loc = NULL;
    IEnumWbemClassObject * enumerator = NULL;
    HRESULT result;
    bool    found = false;

    // Inisialisasi WMI
    if (InitWMI(&svc, &loc, L"ROOT\\CIMV2"))
    {
        // eksekusi Query
        if (ExecWMIQuery(&svc, &loc, &enumerator, L"SELECT * FROM Win32_Bus"))
        {
            IWbemClassObject * iter = NULL;
            ULONG   retval = 0;
            VARIANT vtProp;
            
            int count = 0;
            int findCount = 0;
            const int findThreshold = 3;

            while (enumerator)
            {
                result = enumerator->Next(WBEM_INFINITE, 1, &iter, &retval);
                if (retval == 0) break;

                count++;

                // dapatkan value dari property Name
                result = iter->Get(L"Name", 0, &vtProp, 0, 0);
                if (SUCCEEDED(result)) 
                {
                    if (V_VT(&vtProp) != VT_NULL)
                    {
                        if ((vtProp.vt & VT_BSTR) == VT_BSTR)
                        {
                            if (wcsstr(vtProp.bstrVal, L"ACPIBus_BUS_0") != 0)
                                findCount++;

                            else if (wcsstr(vtProp.bstrVal, L"PCI_BUS_0") != 0)
                                findCount++;

                            else if (wcsstr(vtProp.bstrVal, L"PNP_BUS_0") != 0)
                                findCount++;
                        }
                        
                        VariantClear(&vtProp);
                    }
                }
                iter->Release();
            }

            // periksa jika terdapat N instance yang cocok dengan string di atas
            if (count == findThreshold && findCount == findThreshold)
                found = true;

            // Cleanup
            enumerator->Release();
            svc->Release();
            loc->Release();
            CoUninitialize();
        }
    }

    return found;
}

/**
 *  Periksa baseboard
 */
bool antivm_vbox_wmi_baseboard()
{
    IWbemServices * svc = NULL;
    IWbemLocator  * loc = NULL;
    IEnumWbemClassObject * enumerator = NULL;
    HRESULT result;
    bool    found = false;

    // Inisialisasi WMI
    if (InitWMI(&svc, &loc, L"ROOT\\CIMV2"))
    {
        // eksekusi Query
        if (ExecWMIQuery(&svc, &loc, &enumerator, L"SELECT * FROM Win32_BaseBoard"))
        {
            IWbemClassObject * iter = NULL;
            ULONG   retval = 0;
            VARIANT vtProp = { 0 };

            while (enumerator && !found)
            {
                result = enumerator->Next(WBEM_INFINITE, 1, &iter, &retval);
                if (retval == 0) break;

                // dapatkan value dari property Product
                result = iter->Get(L"Product", 0, &vtProp, 0, 0);
                if (SUCCEEDED(result))
                {
                    if (V_VT(&vtProp) != VT_NULL)
                    {
                        if ((vtProp.vt & VT_BSTR) == VT_BSTR)
                        {
                            if (wcsstr(vtProp.bstrVal, L"VirtualBox") != 0)
                                found = true;
                        }

                        VariantClear(&vtProp);
                    }
                }

                vtProp = { 0 };

                // dapatkan value dari property Manufacturer
                result = iter->Get(L"Manufacturer", 0, &vtProp, 0, 0);
                if (SUCCEEDED(result))
                {
                    if (V_VT(&vtProp) != VT_NULL)
                    {
                        if ((vtProp.vt & VT_BSTR) == VT_BSTR)
                        {
                            if (wcsstr(vtProp.bstrVal, L"Oracle Corporation") != 0)
                                found = true;
                        }

                        VariantClear(&vtProp);
                    }
                }

                iter->Release();
            }

            // Cleanup
            enumerator->Release();
            svc->Release();
            loc->Release();
            CoUninitialize();
        }
    }

    return found;
}


/**
 *  Periksa firmware SMBIOS
 */
bool antivm_vbox_firmware_smbios ()
{
    bool result = false;

    DWORD size = 0;
    PBYTE smbios = getFirmwareTable(static_cast<DWORD>('RSMB'), 0x0000, &size);
    if (smbios != NULL)
    {
        if (memsearch((PBYTE) "VirtualBox", 10, smbios, size) || 
            memsearch((PBYTE) "vbox", 4, smbios, size) ||
            memsearch((PBYTE) "VBOX", 4, smbios, size) )
            result = true;

        free(smbios);
    }

    return result;
}


/**
 *  Periksa firmware ACPI
 */
bool antivm_vbox_firmware_acpi ()
{
    bool result = false;

    PDWORD names = static_cast<PDWORD>(malloc(4096));
    SecureZeroMemory(names, 4096);
    DWORD  size = enumFirmwareTables(static_cast<DWORD>('ACPI'), names, 4096);

    if (size == -1)
        return false;
    
    DWORD count = size / 4;
    if (size < 4 || count == 0)
        result = true;
    else 
    {
        for (DWORD i = 0; i < count && !result; i++)
        {
            DWORD tblsize = 0;
            PBYTE table = getFirmwareTable(static_cast<DWORD>('ACPI'), names[i], &tblsize);

            if (memsearch((PBYTE) "VirtualBox", 10, table, tblsize) || 
                memsearch((PBYTE) "vbox", 4, table, tblsize) ||
                memsearch((PBYTE) "VBOX", 4, table, tblsize) )
                result = true;
            
            free(table);
        }
    }

    free(names);
    return result;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antivm_vbox_regkeyval,        "VM VBox : Registry Key Value");
    check(antivm_vbox_regkey,           "VM VBox : Registry Key");
    check(antivm_vbox_files,            "VM VBox : Default Files");
    check(antivm_vbox_dirs,             "VM VBox : Default Dirs");
    check(antivm_vbox_devices,          "VM VBox : Special Devices");
    check(antivm_vbox_macaddr,          "VM VBox : MAC Address");
    check(antivm_vbox_window_class,     "VM VBox : Window Class");
    check(antivm_vbox_network_share,    "VM VBox : Network Shared Folder");
    check(antivm_vbox_process,          "VM VBox : Process");
    check(antivm_vbox_wmi,              "VM VBox : WMI (Device)");
    check(antivm_vbox_wmi_mac,          "VM VBox : WMI (MAC Address)");
    check(antivm_vbox_wmi_evtlog,       "VM VBox : WMI (Event Log)");
    check(antivm_vbox_wmi_pnpentity_pcideviceid, "VM VBox : WMI PnPEntity PCIDeviceID");
    check(antivm_vbox_wmi_pnpentity_controllers, "VM VBox : WMI PnPEntity Controllers");
    check(antivm_vbox_wmi_pnpentity_vboxname,    "VM VBox : WMI PnPEntity VBoxName");
    check(antivm_vbox_bus,              "VM VBox : BUS name");
    check(antivm_vbox_wmi_baseboard,    "VM VBox : BaseBoard");
    check(antivm_vbox_firmware_smbios,  "VM VBox : Firmware BIOS");
    check(antivm_vbox_firmware_acpi,    "VM VBox : Firmware ACPI");
    
    getchar();
    return 0;
}