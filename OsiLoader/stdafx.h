// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#include <string>

[[noreturn]]
void Fail(char const * reason);

std::string ToUTF8(std::wstring const & s);
std::wstring FromUTF8(std::string const & s);
std::string GetLastErrorString(DWORD lastError);
