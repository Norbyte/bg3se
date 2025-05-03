#include "stdafx.h"
#include <Extender/Shared/DWriteWrapper.h>
#include <Extender/Shared/ExtenderConfig.h>
#include <Extender/ScriptExtender.h>
#include "json/json.h"
#include <ShellAPI.h>
#include <KnownFolders.h>
#include <ShlObj.h>
#include <sstream>
#include <fstream>

using namespace bg3se;

void ConfigGetBool(Json::Value & node, char const * key, bool & value)
{
    auto configVar = node[key];
    if (!configVar.isNull() && configVar.isBool()) {
        value = configVar.asBool();
    }
}

void ConfigGetInt(Json::Value& node, char const* key, uint32_t& value)
{
    auto configVar = node[key];
    if (!configVar.isNull() && configVar.isUInt()) {
        value = configVar.asUInt();
    }
}

void ConfigGet(Json::Value& node, char const* key, std::string& value)
{
    auto configVar = node[key];
    if (!configVar.isNull() && configVar.isString()) {
        value = configVar.asString();
    }
}

void ConfigGet(Json::Value& node, char const* key, std::wstring& value)
{
    auto configVar = node[key];
    if (!configVar.isNull() && configVar.isString()) {
        value = FromStdUTF8(StringView(configVar.asString()));
    }
}

void LoadConfig(std::wstring const & configPath, ExtenderConfig & config)
{
    std::ifstream f(configPath, std::ios::in);
    if (!f.good()) {
        return;
    }

    Json::CharReaderBuilder factory;
    Json::Value root;
    std::string errs;
    if (!Json::parseFromStream(factory, f, &root, &errs)) {
        std::stringstream err;
        err << "Failed to load configuration file '" << ToStdUTF8(configPath) << "':\r\n" << errs;
        Fail(err.str().c_str());
    }

    ConfigGetBool(root, "CreateConsole", config.CreateConsole);
    ConfigGetBool(root, "DefaultToClientConsole", config.DefaultToClientConsole);
    ConfigGetBool(root, "EnableLogging", config.EnableLogging);
    ConfigGetBool(root, "LogCompile", config.LogCompile);
    ConfigGetBool(root, "LogFailedCompile", config.LogFailedCompile);
    ConfigGetBool(root, "LogRuntime", config.LogRuntime);
    ConfigGetBool(root, "ExtendStory", config.ExtendStory);
    ConfigGetBool(root, "SendCrashReports", config.SendCrashReports);
    ConfigGetBool(root, "ForceCrashReporting", config.ForceCrashReporting);
    ConfigGetBool(root, "EnableDebugger", config.EnableDebugger);
    ConfigGetBool(root, "EnableLuaDebugger", config.EnableLuaDebugger);
    ConfigGetBool(root, "DeveloperMode", config.DeveloperMode);
    ConfigGetBool(root, "ClearOnReset", config.ClearOnReset);
    ConfigGetBool(root, "ShowPerfWarnings", config.ShowPerfWarnings);
    ConfigGetBool(root, "EnableAchievements", config.EnableAchievements);
    ConfigGetBool(root, "DisableLauncher", config.DisableLauncher);
    ConfigGetBool(root, "DisableStoryPatching", config.DisableStoryPatching);
    ConfigGetBool(root, "DisableStoryCompilation", config.DisableStoryCompilation);
    ConfigGetBool(root, "InsanityCheck", config.InsanityCheck);

    ConfigGetInt(root, "DebuggerPort", config.DebuggerPort);
    ConfigGetInt(root, "LuaDebuggerPort", config.LuaDebuggerPort);
    ConfigGetInt(root, "DebugFlags", config.DebugFlags);

    ConfigGet(root, "LogDirectory", config.LogDirectory);
    ConfigGet(root, "LuaBuiltinResourceDirectory", config.LuaBuiltinResourceDirectory);
    ConfigGet(root, "CustomProfile", config.CustomProfile);
}

void SetupScriptExtender(HMODULE hModule)
{
    gExtender = std::make_unique<ScriptExtender>();
    auto & config = gExtender->GetConfig();
    LoadConfig(L"ScriptExtenderSettings.json", config);

    DisableThreadLibraryCalls(hModule);
    if (config.CreateConsole) {
        gCoreLibPlatformInterface.GlobalConsole->Create();
    }

    if (config.DebugFlags == 0) {
        // Disabled: DF_FunctionList, DF_NodeList ,DF_LogSuccessfulFacts, DF_LogFailedFacts, DB_LogFactFailures, DF_DumpDatabases, DF_DebugFacts, DF_LogRuleFailures
        config.DebugFlags = DF_DebugTrace | DF_SuppressInitLog;
    }

    gExtender->Initialize();

#if 0
    DEBUG(" ***** ScriptExtender setup completed ***** ");
#endif
}

HANDLE ExtenderMutex{ NULL };

bool ShouldInitializeExtender()
{
    if (GetModuleHandleW(L"bg3.exe") == NULL && GetModuleHandleW(L"bg3_dx11.exe") == NULL) {
        return false;
    }

    if (ExtenderMutex == NULL) {
        char mutexName[MAX_PATH];
        sprintf_s(mutexName, "BG3SE_Ext_%d", GetCurrentProcessId());
        ExtenderMutex = CreateMutexA(NULL, TRUE, mutexName);
    }

    return ExtenderMutex != NULL;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        bg3se::gCoreLibPlatformInterface.ThisModule = hModule;
        gDWriteWrapper = std::make_unique<DWriteWrapper>();
        if (ShouldInitializeExtender()) {
            SetupScriptExtender(hModule);
        }
        break;

    case DLL_PROCESS_DETACH:
        if (gExtender) {
            gExtender->Shutdown();
            gExtender.reset();
        }
        gDWriteWrapper.reset();
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

