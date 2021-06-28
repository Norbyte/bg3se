#include "stdafx.h"
#include "Version.h"
#include <GameDefinitions/BaseTypes.h>
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

std::string ToUTF8(std::wstring_view s)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0, NULL, NULL);
	std::string converted;
	converted.resize(size);
	WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), converted.data(), (int)converted.size(), NULL, NULL);
	return converted;
}

std::wstring FromUTF8(std::string_view s)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0);
	std::wstring converted;
	converted.resize(size);
	MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), converted.data(), (int)converted.size());
	return converted;
}

std::optional<UUID> ParseUuid(std::string_view s)
{
	UUID uuid;
	if (UuidFromStringA((RPC_CSTR)s.data(), &uuid) == RPC_S_OK) {
		// BG3 parses UUIDs into a weird byte swapped format
		auto v1 = uuid.Val[1];
		uuid.Val[1] = (((v1 >> 56) & 0xff) << 48) |
			(((v1 >> 48) & 0xff) << 56) |
			(((v1 >> 40) & 0xff) << 32) |
			(((v1 >> 32) & 0xff) << 40) |
			(((v1 >> 24) & 0xff) << 16) |
			(((v1 >> 16) & 0xff) << 24) |
			(((v1 >> 8) & 0xff) << 0) |
			(((v1 >> 0) & 0xff) << 8);

		return uuid;
	} else {
		return {};
	}
}

std::optional<UUID> ParseGuidString(std::string_view nameGuid)
{
	if (!bg3se::IsValidGuidString(nameGuid.data())) {
		OsiError("GUID (" << nameGuid << ") malformed!");
		return {};
	}

	auto guid = nameGuid.data() + nameGuid.size() - 36;
	return ParseUuid(std::string_view(guid, 36));
}

std::string FormatUuid(UUID const& uuid)
{
	uint8_t const* p = reinterpret_cast<uint8_t const*>(&uuid);
	char s[100];
	sprintf_s(s, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x", 
		p[3], p[2], p[1], p[0],
		p[5], p[4],
		p[7], p[6],
		p[9], p[8],
		p[11], p[10], p[13], p[12], p[15], p[14]
	);
	return s;
}

std::optional<std::string> GetResource(int resourceId)
{
	auto hResource = FindResource(gThisModule, MAKEINTRESOURCE(resourceId),
		L"LUA_SCRIPT");

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
