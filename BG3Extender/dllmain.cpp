#include "stdafx.h"
#include "DWriteWrapper.h"
#include "OsirisProxy.h"
#include "json/json.h"
#include <ShellAPI.h>
#include <KnownFolders.h>
#include <ShlObj.h>
#include <sstream>
#include <fstream>

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

void LoadConfig(std::wstring const & configPath, bg3se::ToolConfig & config)
{
	std::ifstream f(configPath, std::ios::in);
	if (!f.good()) {
		return;
	}

	Json::CharReaderBuilder factory;
	Json::Value root;
	std::string errs;
	if (!Json::parseFromStream(factory, f, &root, &errs)) {
		std::wstring werrs = FromUTF8(errs);

		std::wstringstream err;
		err << L"Failed to load configuration file '" << configPath << "':\r\n" << werrs;
		Fail(err.str().c_str());
	}

	ConfigGetBool(root, "CreateConsole", config.CreateConsole);
	ConfigGetBool(root, "EnableLogging", config.EnableLogging);
	ConfigGetBool(root, "LogCompile", config.LogCompile);
	ConfigGetBool(root, "LogFailedCompile", config.LogFailedCompile);
	ConfigGetBool(root, "LogRuntime", config.LogRuntime);
	ConfigGetBool(root, "EnableExtensions", config.EnableExtensions);
	ConfigGetBool(root, "SendCrashReports", config.SendCrashReports);
	ConfigGetBool(root, "DumpNetworkStrings", config.DumpNetworkStrings);
	ConfigGetBool(root, "SyncNetworkStrings", config.SyncNetworkStrings);
	ConfigGetBool(root, "EnableDebugger", config.EnableDebugger);
	ConfigGetBool(root, "EnableLuaDebugger", config.EnableLuaDebugger);
	ConfigGetBool(root, "DisableModValidation", config.DisableModValidation);
	ConfigGetBool(root, "DeveloperMode", config.DeveloperMode);
	ConfigGetBool(root, "ShowPerfWarnings", config.ShowPerfWarnings);
	ConfigGetBool(root, "EnableAchievements", config.EnableAchievements);

	ConfigGetInt(root, "DebuggerPort", config.DebuggerPort);
	ConfigGetInt(root, "LuaDebuggerPort", config.LuaDebuggerPort);
	ConfigGetInt(root, "DebugFlags", config.DebugFlags);

	auto logDir = root["LogDirectory"];
	if (!logDir.isNull()) {
		if (logDir.isString()) {
			config.LogDirectory = FromUTF8(logDir.asString());
		}
		else {
			Fail("Config option 'LogDirectory' should be a string.");
		}
	}
}

void SetupOsirisProxy(HMODULE hModule)
{
	bg3se::gOsirisProxy = std::make_unique<bg3se::OsirisProxy>();
	auto & config = bg3se::gOsirisProxy->GetConfig();
	LoadConfig(L"OsirisExtenderSettings.json", config);

	DisableThreadLibraryCalls(hModule);
	if (config.CreateConsole) {
		gConsole.Create();
	}

	if (config.DebugFlags == 0) {
		// Disabled: DF_FunctionList, DF_NodeList ,DF_LogSuccessfulFacts, DF_LogFailedFacts, DB_LogFactFailures, DF_DumpDatabases, DF_DebugFacts, DF_LogRuleFailures
		config.DebugFlags = bg3se::DF_DebugTrace | bg3se::DF_SuppressInitLog;
	}

	bg3se::gOsirisProxy->Initialize();

#if 0
	DEBUG(" ***** OsirisProxy setup completed ***** ");
#endif
}

bool ShouldRunProxy()
{
	return GetModuleHandleW(L"bg3.exe") != NULL
		|| GetModuleHandleW(L"bg3_dx11.exe") != NULL;
}

HMODULE gThisModule{ NULL };

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		gThisModule = hModule;
		if (ShouldRunProxy()) {
			gDWriteWrapper = std::make_unique<DWriteWrapper>();
			SetupOsirisProxy(hModule);
		}
		break;

	case DLL_PROCESS_DETACH:
		if (gDWriteWrapper) {
			bg3se::gOsirisProxy->Shutdown();
			bg3se::gOsirisProxy.reset();
			gDWriteWrapper.reset();
		}
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

