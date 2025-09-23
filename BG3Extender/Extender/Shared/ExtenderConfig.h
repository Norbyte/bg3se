#pragma once

#include <cstdint>
#include <string>

BEGIN_SE()

struct ProfilerThreshold
{
    inline constexpr ProfilerThreshold(uint32_t warn, uint32_t error)
        : Warning(warn), Error(error)
    {}

    uint32_t Warning{ 0 };
    uint32_t Error{ 0 };
};

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
    bool Profiler{ false };

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

    // Lua profiler configuration
    bool EnableProfiler{ true };
    bool ProfilerWarnings{ false };
    ProfilerThreshold ProfilerLoadThreshold{ 50000, 50000 };
    ProfilerThreshold ProfilerLoadCallbackThreshold{ 50000, 50000 };
    ProfilerThreshold ProfilerCallbackThreshold{ 1500, 5000 };
    ProfilerThreshold ProfilerClientCallbackThreshold{ 1000, 2000 };
};

inline bool ProfilerShouldReport(uint64_t took, ExtenderConfig const& config, ProfilerThreshold const& threshold)
{
    return config.EnableProfiler
        && (took >= threshold.Error
            || config.ProfilerWarnings && took >= threshold.Warning);
}

inline bool ProfilerShouldWarn(uint64_t took, ExtenderConfig const& config, ProfilerThreshold const& threshold)
{
    return config.ProfilerWarnings && took >= threshold.Warning;
}

#define PERF_REPORT(type, took, ...) ProfilerShouldWarn(took, gExtender->GetConfig(), gExtender->GetConfig().Profiler##type##Threshold) \
    ? WARN(##__VA_ARGS__) \
    : ERR(##__VA_ARGS__)

#define PERF_SHOULD_REPORT(type, took) ProfilerShouldReport((took), gExtender->GetConfig(), gExtender->GetConfig().Profiler##type##Threshold)

END_SE()
