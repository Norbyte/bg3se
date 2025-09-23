BEGIN_SE()

using namespace rapidjson;

void ConfigGet(Value& node, char const* key, bool& value)
{
    auto configVar = node.FindMember(key);
    if (configVar != node.MemberEnd() && configVar->value.IsBool()) {
        value = configVar->value.GetBool();
    }
}

void ConfigGet(Value& node, char const* key, uint32_t& value)
{
    auto configVar = node.FindMember(key);
    if (configVar != node.MemberEnd() && configVar->value.IsUint()) {
        value = configVar->value.GetUint();
    }
}

void ConfigGet(Value& node, char const* key, std::string& value)
{
    auto configVar = node.FindMember(key);
    if (configVar != node.MemberEnd() && configVar->value.IsString()) {
        value = configVar->value.GetString();
    }
}

void ConfigGet(Value& node, char const* key, std::wstring& value)
{
    auto configVar = node.FindMember(key);
    if (configVar != node.MemberEnd() && configVar->value.IsString()) {
        value = FromStdUTF8(configVar->value.GetString());
    }
}

void ConfigGet(Value& node, char const* key, ProfilerThreshold& value)
{
    ConfigGet(node, (std::string(key) + "Warning").c_str(), value.Warning);
    ConfigGet(node, (std::string(key) + "Error").c_str(), value.Error);
}

bool ReadConfig(std::wstring const& configPath, std::string& configJson)
{
    std::ifstream f(configPath, std::ios::in | std::ios::binary);
    if (f.good()) {
        f.seekg(0, std::ios::end);
        configJson.resize((unsigned)f.tellg());
        f.seekg(0, std::ios::beg);
        f.read(configJson.data(), configJson.size());
        return true;
    }

    return false;
}

void LoadConfig(std::wstring const& configPath, ExtenderConfig& config)
{
    std::string configJson;
    if (!ReadConfig(configPath, configJson)) {
        return;
    }

    Document root;
    if (root.ParseInsitu(configJson.data()).HasParseError()) {
        std::stringstream err;
        err << "Failed to parse configuration file '" << ToStdUTF8(configPath) << "'";
        Fail(err.str().c_str());
    }

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
    ConfigGet(root, "ShowPerfWarnings", config.ShowPerfWarnings);
    ConfigGet(root, "EnableAchievements", config.EnableAchievements);
    ConfigGet(root, "DisableLauncher", config.DisableLauncher);
    ConfigGet(root, "DisableStoryPatching", config.DisableStoryPatching);
    ConfigGet(root, "DisableStoryCompilation", config.DisableStoryCompilation);
    ConfigGet(root, "InsanityCheck", config.InsanityCheck);
    ConfigGet(root, "Profiler", config.Profiler);

    ConfigGet(root, "DebuggerPort", config.DebuggerPort);
    ConfigGet(root, "LuaDebuggerPort", config.LuaDebuggerPort);
    ConfigGet(root, "DebugFlags", config.DebugFlags);

    ConfigGet(root, "LogDirectory", config.LogDirectory);
    ConfigGet(root, "LuaBuiltinResourceDirectory", config.LuaBuiltinResourceDirectory);
    ConfigGet(root, "CustomProfile", config.CustomProfile);

    config.EnableProfiler = config.CreateConsole;
    config.ProfilerWarnings = config.EnableProfiler && config.DeveloperMode;

    ConfigGet(root, "EnableProfiler", config.EnableProfiler);
    ConfigGet(root, "ProfilerWarnings", config.ProfilerWarnings);
    ConfigGet(root, "ProfilerLoadThreshold", config.ProfilerLoadThreshold);
    ConfigGet(root, "ProfilerLoadCallbackThreshold", config.ProfilerLoadCallbackThreshold);
    ConfigGet(root, "ProfilerCallbackThreshold", config.ProfilerCallbackThreshold);
    ConfigGet(root, "ProfilerClientCallbackThreshold", config.ProfilerClientCallbackThreshold);
}

END_SE()
