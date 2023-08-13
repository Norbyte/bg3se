#include "stdafx.h"
#include "Version.h"
#include <GameDefinitions/Base/Base.h>
#include <Extender/ScriptExtender.h>
#include <rpc.h>
#include <rpcdce.h>
#include <thread>

namespace bg3se
{
	STDString ToUTF8(WStringView s)
	{
		int size = WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0, NULL, NULL);
		STDString converted;
		converted.resize(size);
		WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), converted.data(), (int)converted.size(), NULL, NULL);
		return converted;
	}

	STDWString FromUTF8(StringView s)
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0);
		STDWString converted;
		converted.resize(size);
		MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), converted.data(), (int)converted.size());
		return converted;
	}
}

std::string ToStdUTF8(std::wstring_view s)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0, NULL, NULL);
	std::string converted;
	converted.resize(size);
	WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), converted.data(), (int)converted.size(), NULL, NULL);
	return converted;
}

std::wstring FromStdUTF8(std::string_view s)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0);
	std::wstring converted;
	converted.resize(size);
	MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), converted.data(), (int)converted.size());
	return converted;
}

std::optional<std::string> GetExeResource(int resourceId)
{
	auto hResource = FindResource(gThisModule, MAKEINTRESOURCE(resourceId), L"SCRIPT_EXTENDER");

	if (hResource) {
		auto hGlobal = LoadResource(gThisModule, hResource);
		if (hGlobal) {
			auto resourceData = LockResource(hGlobal);
			if (resourceData) {
				DWORD resourceSize = SizeofResource(gThisModule, hResource);
				std::string contents;
				contents.resize(resourceSize);
				memcpy(contents.data(), resourceData, resourceSize);
				return contents;
			}
		}
	}

	ERR("Could not get bootstrap resource %d!", resourceId);
	return {};
}

[[noreturn]]
void Fail(TCHAR const * reason)
{
#if defined(_DEBUG)
	DebugBreak();
#endif
	ERR(L"%s", reason);
	MessageBoxW(NULL, reason, L"BG3 Script Extender Error", MB_OK | MB_ICONERROR);
	TerminateProcess(GetCurrentProcess(), 1);
}

[[noreturn]]
void Fail(char const * reason)
{
	ERR("%s", reason);
#if defined(_DEBUG)
	DebugBreak();
#endif
	MessageBoxA(NULL, reason, "BG3 Script Extender Error", MB_OK | MB_ICONERROR);
	TerminateProcess(GetCurrentProcess(), 1);
}

void LogLuaError(std::string_view msg)
{
	bg3se::gExtender->LogLuaError(msg);
}

void LogOsirisError(std::string_view msg)
{
	bg3se::gExtender->LogOsirisError(msg);
}

void LogOsirisWarning(std::string_view msg)
{
	bg3se::gExtender->LogOsirisWarning(msg);
}

void LogOsirisMsg(std::string_view msg)
{
	bg3se::gExtender->LogOsirisMsg(msg);
}
