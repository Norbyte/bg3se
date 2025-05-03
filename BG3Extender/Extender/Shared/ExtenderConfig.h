#pragma once

#include <cstdint>
#include <string>

namespace bg3se {

struct ExtenderConfig
{
#if defined(OSI_EXTENSION_BUILD)
    bool CreateConsole{ false };
    bool DefaultToClientConsole{ false };
    bool EnableDebugger{ false };
    bool EnableLuaDebugger{ false };
#else
    bool CreateConsole{ true };
    bool DefaultToClientConsole{ false };
    bool EnableDebugger{ true };
    bool EnableLuaDebugger{ true };
#endif

    bool ExtendStory{ false };
    bool LogFailedCompile{ true };
    bool EnableLogging{ false };
    bool LogCompile{ false };
    bool LogRuntime{ false };
    bool SendCrashReports{ true };
    bool ForceCrashReporting{ false };
    bool EnableAchievements{ true };
    bool DisableLauncher{ false };
    bool DisableStoryPatching{ false };
    bool DisableStoryCompilation{ true };
    bool InsanityCheck{ false };

#if defined(OSI_EXTENSION_BUILD)
#if defined(_DEBUG)
    bool DeveloperMode{ true };
#else
    bool DeveloperMode{ false };
#endif // defined(_DEBUG)
#else
    bool DeveloperMode{ true };
#endif // defined(OSI_EXTENSION_BUILD)

    bool ClearOnReset{ true };
    bool ShowPerfWarnings{ false };
    uint32_t DebuggerPort{ 9999 };
    uint32_t LuaDebuggerPort{ 9998 };
    uint32_t DebugFlags{ 0 };
    std::wstring LogDirectory;
    std::wstring LuaBuiltinResourceDirectory;
    std::string CustomProfile;
};

}
