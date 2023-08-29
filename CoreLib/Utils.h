#pragma once

#include <CoreLib/Base/Base.h>
#include <CoreLib/Console.h>

BEGIN_SE()

template <typename... Args>
void Debug(DebugMessageType type, wchar_t const * fmt, Args... args)
{
	if (gCoreLibPlatformInterface.GlobalConsole) {
		wchar_t buf[1024];
		_snwprintf_s(buf, std::size(buf), _TRUNCATE, fmt, args...);
		gCoreLibPlatformInterface.GlobalConsole->Print(type, buf);
	}
}

template <typename... Args>
void Debug(DebugMessageType type, char const * fmt, Args... args)
{
	if (gCoreLibPlatformInterface.GlobalConsole) {
		char buf[1024];
		_snprintf_s(buf, std::size(buf), _TRUNCATE, fmt, args...);
		gCoreLibPlatformInterface.GlobalConsole->Print(type, buf);
	}
}

#define DEBUG(msg, ...) Debug(DebugMessageType::Debug, msg, __VA_ARGS__)
#define INFO(msg, ...) Debug(DebugMessageType::Info, msg, __VA_ARGS__)
#define WARN(msg, ...) Debug(DebugMessageType::Warning, msg, __VA_ARGS__)
#define ERR(msg, ...) Debug(DebugMessageType::Error, msg, __VA_ARGS__)

[[noreturn]]
void Fail(TCHAR const * reason);

[[noreturn]]
void Fail(char const * reason);

bool TryCreateDirectory(std::wstring const& path);
bool SaveFile(std::wstring const& path, std::vector<uint8_t> const& body);
bool SaveFile(std::wstring const& path, std::string const& body);
bool LoadFile(std::wstring const& path, std::vector<uint8_t>& body);
bool LoadFile(std::wstring const& path, std::string& body);

std::optional<std::string> GetExeResource(int resourceId);

END_SE()
