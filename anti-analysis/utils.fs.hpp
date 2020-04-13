#pragma once
#define _UNICODE

#include <windows.h>
#include <Shlobj.h>
#include <Shlwapi.h>

#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Shlwapi.lib")

/******************************** DECLARATION ********************************/

bool  isFileExists(wchar_t * path);
bool  isDirExists(wchar_t * path);

/******************************* IMPLEMENTATION *******************************/

/**
 *  Periksa apakah sebuah file ada di system.
 */
bool isFileExists(wchar_t * path)
{
    DWORD attrib = GetFileAttributesW(path);
    return (attrib != INVALID_FILE_ATTRIBUTES) && !(attrib & FILE_ATTRIBUTE_DIRECTORY);
}


/**
 *  Periksa apakah sebuah direktori ada di system.
 */
bool isDirExists(wchar_t * path)
{
    DWORD attrib = GetFileAttributesW(path);
    return (attrib != INVALID_FILE_ATTRIBUTES) && (attrib & FILE_ATTRIBUTE_DIRECTORY);
}