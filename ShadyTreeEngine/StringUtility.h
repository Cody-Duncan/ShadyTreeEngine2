#pragma once
#include <Windows.h>

#ifndef STRING_UTILITY_H
#define STRING_UTILITY_H

//Convert a string to that pesky WCHAR format.
inline WCHAR* convertMultiByteToWCHAR(const char* data)
{
    WCHAR *pwcsName;
    int nChars = MultiByteToWideChar(CP_ACP, 0, data, -1, NULL, 0);
    pwcsName = new WCHAR[nChars];
    MultiByteToWideChar(CP_ACP, 0, data, -1, (LPWSTR)pwcsName, nChars);
    return pwcsName;
}

#endif