#include "stdafx.h"
#include <CoreLib/Base/Base.h>
#include <Shlwapi.h>

BEGIN_SE()

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
	auto hResource = FindResource(gCoreLibPlatformInterface.ThisModule, MAKEINTRESOURCE(resourceId), L"SCRIPT_EXTENDER");

	if (hResource) {
		auto hGlobal = LoadResource(gCoreLibPlatformInterface.ThisModule, hResource);
		if (hGlobal) {
			auto resourceData = LockResource(hGlobal);
			if (resourceData) {
				DWORD resourceSize = SizeofResource(gCoreLibPlatformInterface.ThisModule, hResource);
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
	if (IsDebuggerPresent()) {
		DebugBreak();
	}
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
	if (IsDebuggerPresent()) {
		DebugBreak();
	}
#endif
	MessageBoxA(NULL, reason, "BG3 Script Extender Error", MB_OK | MB_ICONERROR);
	TerminateProcess(GetCurrentProcess(), 1);
}

bool TryCreateDirectory(std::wstring const& path)
{
	if (!PathFileExistsW(path.c_str())) {
		return CreateDirectoryW(path.c_str(), NULL) == TRUE;
	} else {
		return true;
	}
}

bool SaveFile(std::wstring const& path, std::vector<uint8_t> const& body)
{
	std::ofstream f(path, std::ios::binary | std::ios::out);
	if (!f.good()) {
		return false;
	}

	f.write(reinterpret_cast<char const*>(body.data()), body.size());
	return f.good();
}

bool SaveFile(std::wstring const& path, std::string const& body)
{
	std::ofstream f(path, std::ios::binary | std::ios::out);
	if (!f.good()) {
		return false;
	}

	f.write(body.data(), body.size());
	return f.good();
}

bool LoadFile(std::wstring const& path, std::vector<uint8_t>& body)
{
	std::ifstream f(path, std::ios::in | std::ios::binary);
	if (f.good()) {
		f.seekg(0, std::ios::end);
		auto size = f.tellg();
		f.seekg(0, std::ios::beg);

		body.resize(size);
		f.read(reinterpret_cast<char*>(body.data()), size);
		return f.good();
	}

	return false;
}

bool LoadFile(std::wstring const& path, std::string& body)
{
	std::ifstream f(path, std::ios::in | std::ios::binary);
	if (f.good()) {
		f.seekg(0, std::ios::end);
		auto size = f.tellg();
		f.seekg(0, std::ios::beg);

		body.resize(size);
		f.read(body.data(), size);
		return f.good();
	}

	return false;
}

END_SE()
