#pragma once

#include <CoreLib/Base/Base.h>
#include <CoreLib/Console.h>

BEGIN_SE()

void Debug(DebugMessageType type, _In_z_ _Printf_format_string_ char const* fmt, ...);
void DebugLocal(DebugMessageType type, _In_z_ _Printf_format_string_ char const* fmt, ...);

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
bool SaveFile(std::wstring const& path, std::string_view body);
bool LoadFile(std::wstring const& path, std::vector<uint8_t>& body);
bool LoadFile(std::wstring const& path, std::string& body);

std::optional<std::string> GetExeResource(int resourceId);

END_SE()
