/*
    Archive of Reversing.ID
    Sample Code for Anti Debugging

Description:
    Menggunakan fitur MEM_WRITE_WATCH di fungsi VirtualAlloc untuk
    memeriksa adanya penulisan ke memory oleh debugger, sandbox, dsb

Compile:
    (msvc)
    $ cl api-CloseHandle-invalid-handle.cpp
*/

#include <windows.h>

/**
 *  Step:
 *      - alokasikan buffer
 *      - tulis sekali
 *      - lihat berapa kali penulisan terjadi.
 *  
 *  Jika penulisan tidak hanya sekali maka terdapat debugger atau tool lain yang menulisi alamat tersebut.
 */
bool antidebug_VirtualAlloc_WriteWatch_BufferOnly()
{
	ULONG_PTR hitcount;
    PVOID * addresses;
	DWORD   granularity;
	BOOL    result;
    int   * buffer;

    addresses = static_cast<PVOID*>(VirtualAlloc(NULL, 4096 * sizeof(PVOID), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
	buffer    = static_cast<int*>(VirtualAlloc(NULL, 4096 * 4096, MEM_RESERVE | MEM_COMMIT | MEM_WRITE_WATCH, PAGE_READWRITE));

    // lakukan akses ke buffer sekali
	buffer[0] = 1234;
	
	hitcount = 4096;
	if (GetWriteWatch(0, buffer, 4096, addresses, &hitcount, &granularity) != 0)
        // Terjadi kegagalan dalam pemanggilan GetWriteWatch()
        result = false;
	else
		// seharusnya terdapat hanya satu kali penulisan di titik ini.
		result = (hitcount != 1);

	VirtualFree(addresses, 0, MEM_RELEASE);
	VirtualFree(buffer, 0, MEM_RELEASE);

	return result;
}


/**
 *  Step:
 *      - alokasikan buffer
 *      - pass ke API yang tak mengubah buffer (ex: call dengan invalid parameter)
 *      - lihat berapa kali penulisan terjadi. 
 *  
 *  Jika terdapat penulisan maka terdapat debugger atau tool lain yang menulisi alamat tersebut.
 */
bool antidebug_VirtualAlloc_WriteWatch_ApiCalls()
{
	ULONG_PTR hitcount;
    PVOID * addresses; 
	DWORD   granularity;
	bool    result, error = false;
    int   * buffer;

    addresses = static_cast<PVOID*>(VirtualAlloc(NULL, 4096 * sizeof(PVOID), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
    buffer    = static_cast<int*>(VirtualAlloc(NULL, 4096 * 4096, MEM_RESERVE | MEM_COMMIT | MEM_WRITE_WATCH, PAGE_READWRITE));

    // lakukan banyak pemanggilan dengan buffer writeable namun tidak pernah ditulisi karena pengunaan parameter yang invalid.
    // hal ini bisa mendeteksi adanya API hooks yang perilaku return-by-parameter berbeda dengan API umumnya.
    
    if (GlobalGetAtomName(INVALID_ATOM, (LPTSTR)buffer, 1) != FALSE)
	{
        // pemanggilan GlobalGetAtomName berhasil padahal seharusnya gagal.
		result = false;
		error  = true;
	}
	if (GetEnvironmentVariableW(L"%ReversingIDSuperImportantVarName?[]<>@\\;*!-{}#:/~%", (LPWSTR)buffer, 4096*4096) != FALSE)
	{
        // pemanggilan GetEnvironmentVariable berhasil padahal seharusnya gagal.
		result = false;
		error  = true;
	}
	if (GetBinaryTypeW(L"%ReversingIDFileName?[]<>@\\;*!-{}#:/~%", (LPDWORD)buffer) != FALSE)
	{
		// pemanggilan GetBinaryType berhasil padahal seharusnya gagal.
		result = false;
		error  = true;
	}
	if (HeapQueryInformation(0, (HEAP_INFORMATION_CLASS)69, buffer, 4096, NULL) != FALSE)
	{
		// pemanggilan HeapQueryInformation berhasil padahal seharusnya gagal.
		result = false;
		error  = true;
	}
	if (ReadProcessMemory(INVALID_HANDLE_VALUE, (LPCVOID)0x69696969, buffer, 4096, NULL) != FALSE)
	{
		// pemanggilan ReadProcessMemory berhasil padahal seharusnya gagal.
		result = false;
		error  = true;
	}
	if (GetThreadContext(INVALID_HANDLE_VALUE, (LPCONTEXT)buffer) != FALSE)
	{
		// pemanggilan GetThreadContext berhasil padahal seharusnya gagal.
		result = false;
		error  = true;
	}
	if (GetWriteWatch(0, &antidebug_VirtualAlloc_WriteWatch_ApiCalls, 0, NULL, NULL, (PULONG)buffer) == 0)
	{
		// pemanggilan GetWriteWatch berhasil padahal seharusnya gagal.
		result = false;
		error  = true;
	}

    /* API seharusnya gagal terpanggil. Jika itu terjadi, maka cek apakah terjadi penulisan */
	if (! error)
	{
		hitcount = 4096;
		if (GetWriteWatch(0, buffer, 4096, addresses, &hitcount, &granularity) != 0)
            // Terjadi kegagalan dalam pemanggilan GetWriteWatch()
            result = false;
        else
            // seharusnya terdapat 0 (tidak ada penulisan) di titik ini.
            result = (hitcount != 0);
	}

	VirtualFree(addresses, 0, MEM_RELEASE);
	VirtualFree(buffer, 0, MEM_RELEASE);

	return result;
}


/**
 *  Step:
 *      - alokasikan buffer
 *      - gunakan untuk menyimpan hasil dari pemanggilan fungsi tertentu.
 *      - lihat berapa kali penulisan terjadi. 
 *  
 *  Jika penulisan tidak hanya sekali maka terdapat debugger atau tool lain yang menulisi alamat tersebut.
 */
bool antidebug_VirtualAlloc_WriteWatch_IsDebuggerPresent()
{
	ULONG_PTR hitcount;
    PVOID * addresses;
    DWORD   granularity;
	bool    result;
    int   * buffer;

	addresses = static_cast<PVOID*>(VirtualAlloc(NULL, 4096 * sizeof(PVOID), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
	buffer    = static_cast<int*>(VirtualAlloc(NULL, 4096 * 4096, MEM_RESERVE | MEM_COMMIT | MEM_WRITE_WATCH, PAGE_READWRITE));
	
    // Simpan hasil dari pemanggilan API
    buffer[0] = IsDebuggerPresent();

	hitcount = 4096;
	if (GetWriteWatch(0, buffer, 4096, addresses, &hitcount, &granularity) != 0)
        // Terjadi kegagalan dalam pemanggilan GetWriteWatch()
        result = false;
    else
        // seharusnya terdapat 1 penulisan
        result = (hitcount != 1) || (buffer[0] == TRUE);

	VirtualFree(addresses, 0, MEM_RELEASE);
	VirtualFree(buffer, 0, MEM_RELEASE);

	return result;
}


/**
 *  Step:
 *      - alokasikan buffer dengan permission writeable.
 *      - salin tertentu
 *      - jalankan fungsi
 *      - lihat apakah terdapat penulisan yang terjadi setelah penyalinan fungsi terjadi.
 */
bool antidebug_VirtualAlloc_WriteWatch_CodeWrite()
{
    ULONG_PTR hitcount;
	PVOID * addresses;
    DWORD   granularity;
	bool    result = false;
    byte  * buffer;

	addresses = static_cast<PVOID*>(VirtualAlloc(NULL, 4096 * sizeof(PVOID), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
	buffer    = static_cast<byte*>(VirtualAlloc(NULL, 4096 * 4096, MEM_RESERVE | MEM_COMMIT | MEM_WRITE_WATCH, PAGE_EXECUTE_READWRITE));
	
    // Konstruksi pemanggilan isDebuggerPresent dalam assembly
	ULONG_PTR isDebuggerPresentAddr = (ULONG_PTR)&IsDebuggerPresent;

#if _WIN64
	/*
	 * 64-bit
	 *
		0:  51                              push   rcx
		1:  48 b9 ef cd ab 90 78 56 34 12   movabs rcx, 0x1234567890abcdef
		b:  ff d1                           call   rcx
		d:  59                              pop    rcx
	    e:  c3                              ret
	 */
	int pos = 0;
	buffer[pos++] = 0x51; // push rcx
	buffer[pos++] = 0x48; // movabs rcx, ...
	buffer[pos++] = 0xB9; // ^ ...
	int offset = 0;
	for (int n = 0; n < 8; n++)
	{
		buffer[pos++] = (isDebuggerPresentAddr >> offset) & 0xFF;
		offset += 8;
	}
	buffer[pos++] = 0xFF; // call rcx
	buffer[pos++] = 0xD1; // ^
	buffer[pos++] = 0x59; // pop rcx
	buffer[pos  ] = 0xC3; // ret

#else
	/*
	 * 32-bit
	 *
	0:  51                      push   ecx
	1:  b9 78 56 34 12          mov    ecx, 0x12345678
	6:  ff d1                   call   ecx
	8:  59                      pop    ecx
	9:  c3                      ret
	*/
	int pos = 0;
	buffer[pos++] = 0x51; // push ecx
	buffer[pos++] = 0xB9; // mov ecx, ...
	int offset = 0;
	for (int n = 0; n < 4; n++)
	{
		buffer[pos++] = (isDebuggerPresentAddr >> offset) & 0xFF;
		offset += 8;
	}
	buffer[pos++] = 0xFF; // call ecx
	buffer[pos++] = 0xD1; // ^
	buffer[pos++] = 0x59; // pop ecx
	buffer[pos] = 0xC3; // ret

#endif

	ResetWriteWatch(buffer, 4096 * 4096);

	// eksekusi code
	BOOL(*foo)(VOID) = (BOOL(*)(VOID)) buffer;
    result = (foo() == TRUE);

    if (! result)
    {
		hitcount = 4096;
		if (GetWriteWatch(0, buffer, 4096, addresses, &hitcount, &granularity) != 0)
            // Terjadi kegagalan dalam pemanggilan GetWriteWatch()
            result = false;
        else
            result = (hitcount != 0);
	}

	VirtualFree(addresses, 0, MEM_RELEASE);
	VirtualFree(buffer, 0, MEM_RELEASE);

	return result;
}


/* ************************ CONTOH PENGGUNAAN ************************ */
#include "../../test.win32.h"

int main(int argc, char* argv[])
{
    check(antidebug_VirtualAlloc_WriteWatch_BufferOnly, "Checking VirtualAlloc write watch (buffer only)"); 
    check(antidebug_VirtualAlloc_WriteWatch_ApiCalls, "Checking VirtualAlloc write watch (API calls)"); 
    check(antidebug_VirtualAlloc_WriteWatch_IsDebuggerPresent, "Checking VirtualAlloc write watch (IsDebuggerPresent)");
    check(antidebug_VirtualAlloc_WriteWatch_CodeWrite, "Checking VirtualAlloc write watch (code write)"); 
    
    getchar();
    return 0;
}