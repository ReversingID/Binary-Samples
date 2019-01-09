/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Menggunakan NtQueryObject() untuk mencari debug object.

Compile:
    (msvc)
    $ cl api-NtQueryObject.cpp
*/

#include <windows.h>
#include <Shlwapi.h>

/*
    Windows XP memperkenalkan "debug object".
    Ketika sesi debugging dimulai, debug object diciptakan dengan sebuah handle
    khusus untuknya.
*/

#define DEBUG_READ_EVENT 0x0001
#define DEBUG_PROCESS_ASSIGN 0x0002
#define DEBUG_SET_INFORMATION 0x0004
#define DEBUG_QUERY_INFORMATION 0x0008
#define DEBUG_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | \
    DEBUG_READ_EVENT | DEBUG_PROCESS_ASSIGN | DEBUG_SET_INFORMATION | \
    DEBUG_QUERY_INFORMATION)

typedef enum _OBJECT_INFORMATION_CLASS {
    ObjectBasicInformation,
    ObjectTypeInformation,
    ObjectAllTypesInformation
} OBJECT_INFORMATION_CLASS;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG           Length;
    HANDLE          RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG           Attributes;
    PVOID           SecurityDescriptor;
    PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef struct _OBJECT_TYPE_INFORMATION {
    UNICODE_STRING TypeName;
    ULONG TotalNumberOfHandles;
    ULONG TotalNumberOfObjects;
} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

typedef struct _OBJECT_ALL_INFORMATION {
	ULONG NumberOfObjects;
	OBJECT_TYPE_INFORMATION ObjectTypeInformation[1];
} OBJECT_ALL_INFORMATION, *POBJECT_ALL_INFORMATION;

typedef NTSTATUS(WINAPI * pNtQueryObject)(HANDLE, UINT, PVOID, ULONG, PULONG);
typedef NTSTATUS(WINAPI * pNtCreateDebugObject)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, ULONG);

#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
}

/*
    NtQueryObject()
    ntdll!NtQueryObject

    prototype:
        __kernel_entry NTSYSCALLAPI NTSTATUS NtQueryObject(
            HANDLE                   Handle,
            OBJECT_INFORMATION_CLASS ObjectInformationClass,
            PVOID                    ObjectInformation,
            ULONG                    ObjectInformationLength,
            PULONG                   ReturnLength
        );

    NtCreateDebugObject()
    ntdll!NtCreateDebugObject

    prototype:
        NTSYSAPI NTSTATUS NTAPI
        NtCreateDebugObject(
            OUT PHANDLE             DebugObjectHandle,
            IN ACCESS_MASK          DesiredAccess,
            IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
            IN BOOLEAN              KillProcessOnExit 
        );
*/

/*
    NtQueryObject() dengan ObjectAllTypesInformation class akan mengembalikan
    informasi tentang host system dan current process. Informasi penting yang diperlukan
    adalah DebugObject di lingkungan.

    Di Windows XP dan Vista, DebugObject disimpan di daftar object tersebut. Jumlah
    object yang ada menandakan adanya sesi debug yang sedang berlangsung.
*/
bool antidebug_NtQueryObject_AllTypesInformation()
{
	ULONG size;
	PVOID memory = NULL;
	POBJECT_ALL_INFORMATION objinfo = NULL;
	NTSTATUS status;
    UCHAR * location;
    ULONG objcount;

    /* Dapatkan address ke function */
    auto   dll      = GetModuleHandle("ntdll.dll");
    void * procaddr = GetProcAddress(dll, "NtQueryObject");
    auto fnNtQueryObject = static_cast<pNtQueryObject>(procaddr);

    if (fnNtQueryObject == nullptr)
        return false;

    // Dapatkan ukuran informasi yang diperlukan
    status = fnNtQueryObject(NULL, ObjectAllTypesInformation, &size, sizeof(ULONG), &size);

    // Alokasikan memori untuk mendapatkan daftar object
    memory = VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (memory == nullptr)
        return false;

    // Dapatkan daftar object
    status = fnNtQueryObject((HANDLE)-1, ObjectAllTypesInformation, memory, size, NULL);
    if (status != 0x00000000)
    {
        VirtualFree(memory, 0, MEM_RELEASE);
        return false;
    }

    // Parse informasi yang ada
    objinfo  = static_cast<POBJECT_ALL_INFORMATION>(memory);
    location = reinterpret_cast<UCHAR*>(objinfo->ObjectTypeInformation);
    objcount = objinfo->NumberOfObjects; 

    for (UINT i = 0; i < objcount; i++)
    {
        auto type = reinterpret_cast<POBJECT_TYPE_INFORMATION>(location);

        if (wcscmp(L"DebugObject", type->TypeName.Buffer) == 0)
        {
            bool result = (type->TotalNumberOfObjects > 0);
            VirtualFree(memory, 0, MEM_RELEASE);
            return result;
        }

        // Dapatkan lokasi berikutnya
        location  = (UCHAR *) type->TypeName.Buffer;
        location += type->TypeName.MaximumLength;
        ULONG_PTR tmp = ((ULONG_PTR) location) & -(int)sizeof(void*);

        if ((ULONG_PTR)tmp != (ULONG_PTR) location)
            tmp += sizeof(void*);
        location = ((UCHAR*) tmp);
    }

    VirtualFree(memory, 0, MEM_RELEASE);
    return false;
}


/**
 *  Menggunakan NtQueryObject dengan ObjectTypeInformation class akan mengembalikan
 *  informasi dari handle yang diberikan.
 *  
 *  Normalnya setelah CreateObject, hanya terdapat 1 handle untuk process ini.
 */
bool antidebug_NtQueryObject_ObjectTypeInformation()
{
	HANDLE DebugObjectHandle;
	OBJECT_ATTRIBUTES ObjectAttributes;
	BYTE memory[0x1000] = { 0 };
	POBJECT_TYPE_INFORMATION ObjectInformation = reinterpret_cast<POBJECT_TYPE_INFORMATION>(memory);
	NTSTATUS status;

    /* Dapatkan address ke function */
    auto   dll      = GetModuleHandle("ntdll.dll");
    void * procaddr = nullptr;

    procaddr = GetProcAddress(dll, "NtQueryObject");
    auto fnNtQueryObject       = static_cast<pNtQueryObject>(procaddr);

    procaddr = GetProcAddress(dll, "NtCreateDebugObject");
    auto fnNtCreateDebugObject = static_cast<pNtCreateDebugObject>(procaddr);

    if (fnNtQueryObject == nullptr || fnNtCreateDebugObject == nullptr)
        return false;

	InitializeObjectAttributes(&ObjectAttributes, 0, 0, 0, 0);

    // Ciptakan satu buah debug object dan periksa apakah debug object tersebut valid.
	status = fnNtCreateDebugObject(&DebugObjectHandle, DEBUG_ALL_ACCESS, &ObjectAttributes, FALSE);
    status = fnNtQueryObject(DebugObjectHandle, ObjectTypeInformation, ObjectInformation, sizeof(memory), 0);
    CloseHandle(DebugObjectHandle);

    if (status >= 0)
        // Harusnya terdapat 1 debug object yang baru saja dibuat.
        return (ObjectInformation->TotalNumberOfObjects != 1);
    else 
        return false;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_NtQueryObject_AllTypesInformation,   "Calling NtQueryObject (ObjectAllTypeInformation)");
    check(antidebug_NtQueryObject_ObjectTypeInformation, "Calling NtQueryObject (ObjectTypeInformation)");

    getchar();
    return 0;
}