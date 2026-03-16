#include <CoreLib/ConfigUtils.inl>

BEGIN_SE()

using namespace rapidjson;

void ConfigGet(Value& node, char const* key, ProfilerThreshold& value)
{
    ConfigGet(node, (std::string(key) + "Error").c_str(), value.Error);
}

void ApplyConfig(ExtenderConfig& config, Document& root)
{
    ConfigGet(root, "CreateConsole", config.CreateConsole);
    ConfigGet(root, "DefaultToClientConsole", config.DefaultToClientConsole);
    ConfigGet(root, "EnableLogging", config.EnableLogging);
    ConfigGet(root, "LogCompile", config.LogCompile);
    ConfigGet(root, "LogFailedCompile", config.LogFailedCompile);
    ConfigGet(root, "LogRuntime", config.LogRuntime);
    ConfigGet(root, "ExtendStory", config.ExtendStory);
    ConfigGet(root, "SendCrashReports", config.SendCrashReports);
    ConfigGet(root, "ForceCrashReporting", config.ForceCrashReporting);
    ConfigGet(root, "EnableDebugger", config.EnableDebugger);
    ConfigGet(root, "EnableLuaDebugger", config.EnableLuaDebugger);
    ConfigGet(root, "DeveloperMode", config.DeveloperMode);
    ConfigGet(root, "ClearOnReset", config.ClearOnReset);
    ConfigGet(root, "EnableAchievements", config.EnableAchievements);
    ConfigGet(root, "DisableLauncher", config.DisableLauncher);
    ConfigGet(root, "DisableStoryPatching", config.DisableStoryPatching);
    ConfigGet(root, "DisableStoryCompilation", config.DisableStoryCompilation);
    ConfigGet(root, "InsanityCheck", config.InsanityCheck);
    ConfigGet(root, "LocalMessagePassing", config.LocalMessagePassing);
    ConfigGet(root, "Optick", config.Optick);

    ConfigGet(root, "DebuggerPort", config.DebuggerPort);
    ConfigGet(root, "LuaDebuggerPort", config.LuaDebuggerPort);
    ConfigGet(root, "DebugFlags", config.DebugFlags);

    ConfigGet(root, "LogDirectory", config.LogDirectory);
    ConfigGet(root, "LuaBuiltinResourceDirectory", config.LuaBuiltinResourceDirectory);
    ConfigGet(root, "CustomProfile", config.CustomProfile);

    config.EnablePerfMessages = config.CreateConsole;

    ConfigGet(root, "EnablePerfMessages", config.EnablePerfMessages);
    ConfigGet(root, "ProfilerLoadThreshold", config.ProfilerLoadThreshold);
    ConfigGet(root, "ProfilerLoadCallbackThreshold", config.ProfilerLoadCallbackThreshold);
    ConfigGet(root, "ProfilerCallbackThreshold", config.ProfilerCallbackThreshold);
    ConfigGet(root, "ProfilerClientCallbackThreshold", config.ProfilerClientCallbackThreshold);
}

void LoadConfig(std::wstring const& configPath, ExtenderConfig& config)
{
    ApplyConfigFile(configPath, config, &ApplyConfig);
}

END_SE()
