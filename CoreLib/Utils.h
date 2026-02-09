#pragma once

#include <CoreLib/Base/Base.h>
#include <CoreLib/Console.h>

BEGIN_SE()

template <typename... Args>
void Debug(DebugMessageType type, _Printf_format_string_ char const * fmt, Args... args)
{
    if (gCoreLibPlatformInterface.GlobalConsole) {
        char buf[1024];
        _snprintf_s(buf, std::size(buf), _TRUNCATE, fmt, args...);
        gCoreLibPlatformInterface.GlobalConsole->Print(type, buf);
    }
}

template <typename... Args>
void DebugLocal(DebugMessageType type, _Printf_format_string_ char const * fmt, Args... args)
{
    if (gCoreLibPlatformInterface.GlobalConsole) {
        char buf[1024];
        _snprintf_s(buf, std::size(buf), _TRUNCATE, fmt, args...);
        gCoreLibPlatformInterface.GlobalConsole->LocalPrint(type, buf);
    }
}

#define DEBUG(msg, ...) Debug(DebugMessageType::Debug, msg, __VA_ARGS__)
#define INFO(msg, ...) Debug(DebugMessageType::Info, msg, __VA_ARGS__)
#define WARN(msg, ...) Debug(DebugMessageType::Warning, msg, __VA_ARGS__)
#define ERR(msg, ...) Debug(DebugMessageType::Error, msg, __VA_ARGS__)

#define DEBUG_LOCAL(msg, ...) DebugLocal(DebugMessageType::Debug, msg, __VA_ARGS__)
#define INFO_LOCAL(msg, ...) DebugLocal(DebugMessageType::Info, msg, __VA_ARGS__)
#define WARN_LOCAL(msg, ...) DebugLocal(DebugMessageType::Warning, msg, __VA_ARGS__)
#define ERR_LOCAL(msg, ...) DebugLocal(DebugMessageType::Error, msg, __VA_ARGS__)

#define WARN_ONCE(msg, ...) { static bool _warned{false}; if (!_warned) { _warned = true; WARN(msg, __VA_ARGS__); } }

void TryDebugBreak();

[[noreturn]]
void Fail(char const * reason);

bool TryCreateDirectory(std::wstring const& path);
bool SaveFile(std::wstring const& path, std::vector<uint8_t> const& body);
bool SaveFile(std::wstring const& path, std::string const& body);
bool LoadFile(std::wstring const& path, std::vector<uint8_t>& body);
bool LoadFile(std::wstring const& path, std::string& body);

std::optional<std::string> GetExeResource(int resourceId);

END_SE()
