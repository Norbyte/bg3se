#pragma once

#include <cstdint>
#include <string>

namespace bg3se {

struct ExtenderConfig
{
#if defined(OSI_EXTENSION_BUILD)
	bool CreateConsole{ false };
	bool EnableDebugger{ false };
	bool EnableLuaDebugger{ false };
#else
	bool CreateConsole{ true };
	bool EnableDebugger{ true };
	bool EnableLuaDebugger{ true };
#endif

	bool EnableExtensions{ true };
#if defined(OSI_EOCAPP)
	bool LogFailedCompile{ true };
#else
	bool LogFailedCompile{ false };
#endif
	bool EnableLogging{ false };
	bool LogCompile{ false };
	bool LogRuntime{ false };
	bool SendCrashReports{ true };
	bool EnableAchievements{ true };

#if defined(OSI_EXTENSION_BUILD)
	bool DisableModValidation{ true };
#if defined(_DEBUG)
	bool DeveloperMode{ true };
#else
	bool DeveloperMode{ false };
#endif // defined(_DEBUG)
#else
	bool DisableModValidation{ false };
	bool DeveloperMode{ true };
#endif // defined(OSI_EXTENSION_BUILD)

	bool ShowPerfWarnings{ false };
	uint32_t DebuggerPort{ 9999 };
	uint32_t LuaDebuggerPort{ 9998 };
	uint32_t DebugFlags{ 0 };
	std::wstring LogDirectory;
};

}
