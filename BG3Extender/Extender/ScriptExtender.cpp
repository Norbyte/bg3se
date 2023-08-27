#include "stdafx.h"
#include <Extender/ScriptExtender.h>
#include "Version.h"
#include "resource.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <psapi.h>
#include <regex>

#include <Extender/Shared/StatLoadOrderHelper.inl>
#include <Extender/Shared/SavegameSerializer.inl>

#undef DEBUG_SERVER_CLIENT

namespace bg3se
{

void InitCrashReporting();
void ShutdownCrashReporting();

decltype(ScriptExtender::CoreLibInit)* decltype(ScriptExtender::CoreLibInit)::gHook;
decltype(ScriptExtender::AppUpdatePaths)* decltype(ScriptExtender::AppUpdatePaths)::gHook;

std::unique_ptr<ScriptExtender> gExtender;

#if !defined(OSI_NO_DEBUGGER)
void LuaDebugThreadRunner(LuaDebugInterface& intf)
{
	intf.Run();
}
#endif

ScriptExtender::ScriptExtender()
	: server_(config_),
	client_(config_)
{
}

void ScriptExtender::Initialize()
{
	if (config_.SendCrashReports) {
		InitCrashReporting();
	}

	extensionsEnabled_ = config_.EnableExtensions;

	GameVersionInfo gameVersion;
	if (Libraries.GetGameVersion(gameVersion)) {
		if (gameVersion.IsSupported()) {
			INFO("Game version v%d.%d.%d.%d OK", gameVersion.Major, gameVersion.Minor, gameVersion.Revision, gameVersion.Build);
		} else {
			ERR("Game version v%d.%d.%d.%d is not supported, please upgrade!", gameVersion.Major, gameVersion.Minor, gameVersion.Revision, gameVersion.Build);
			// Hard exit below a certain version as th EoCClient error display UI won't work anymore
			Fail(L"Script Extender doesn't support game versions below v3.6.54, please upgrade!");
		}
	} else {
		ERR("Failed to retrieve game version info.");
	}

	DEBUG("ScriptExtender::Initialize: Starting");
	auto initStart = std::chrono::high_resolution_clock::now();

	InitStaticSymbols();

	if (!Libraries.FindLibraries(gameVersion.Revision)) {
		ERR("ScriptExtender::Initialize: Could not load libraries; skipping scripting extension initialization.");
		extensionsEnabled_ = false;
	}

	server_.Initialize();
	client_.Initialize();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (GetStaticSymbols().CoreLibSDM__Init != nullptr) {
		CoreLibInit.Wrap(GetStaticSymbols().CoreLibSDM__Init);
		CoreLibInit.SetPostHook(&ScriptExtender::OnCoreLibInit, this);
	}

	if (GetStaticSymbols().App__UpdatePaths != nullptr) {
		AppUpdatePaths.Wrap(GetStaticSymbols().App__UpdatePaths);
		AppUpdatePaths.SetPostHook(&ScriptExtender::OnAppUpdatePaths, this);
	}

	DetourTransactionCommit();

#if !defined(OSI_NO_DEBUGGER)
	if (config_.EnableLuaDebugger && luaDebuggerThread_ == nullptr) {
		DEBUG("Starting Lua debugger server");
		try {
			luaDebugInterface_ = std::make_unique<LuaDebugInterface>(config_.LuaDebuggerPort);
			luaDebugMsgHandler_ = std::make_unique<lua::dbg::DebugMessageHandler>(std::ref(*luaDebugInterface_));
			luaDebuggerThread_ = new std::thread(std::bind(LuaDebugThreadRunner, std::ref(*luaDebugInterface_)));
			luaDebugger_ = std::make_unique<lua::dbg::Debugger>(std::ref(*luaDebugMsgHandler_));
			DEBUG("Lua debugger listening on 127.0.0.1:%d; DBG protocol version %d",
				config_.LuaDebuggerPort, lua::dbg::DebugMessageHandler::ProtocolVersion);
		} catch (std::exception& e) {
			ERR("Lua debugger startup failed: %s", e.what());
		}
	}
#endif

	auto initEnd = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(initEnd - initStart).count();
	DEBUG("Library startup took %d ms", ms);

	if (config_.SendCrashReports) {
		ShutdownCrashReporting();
	}
}

void ScriptExtender::Shutdown()
{
	DEBUG("ScriptExtender::Shutdown: Exiting");
	server_.Shutdown();
	client_.Shutdown();
	Hooks.UnhookAll();
}

void ScriptExtender::LogLuaError(std::string_view msg)
{
	gConsole.Debug(DebugMessageType::Error, msg.data());
	server_.Osiris().LogError(msg);
}

void ScriptExtender::LogOsirisError(std::string_view msg)
{
	gConsole.Debug(DebugMessageType::Error, msg.data());
	server_.Osiris().LogError(msg);

#if !defined(OSI_NO_DEBUGGER)
	if (luaDebugger_) {
		luaDebugger_->OnGenericError(msg.data());
	}
#endif
}

void ScriptExtender::LogOsirisWarning(std::string_view msg)
{
	gConsole.Debug(DebugMessageType::Warning, msg.data());
	server_.Osiris().LogWarning(msg);
}

void ScriptExtender::LogOsirisMsg(std::string_view msg)
{
	gConsole.Debug(DebugMessageType::Osiris, msg.data());
	server_.Osiris().LogMessage(msg);
}

std::wstring ScriptExtender::MakeLogFilePath(std::wstring const & Type, std::wstring const & Extension)
{
	if (config_.LogDirectory.empty()) {
		config_.LogDirectory = FromUTF8(GetStaticSymbols().ToPath("/Extender Logs", PathRootType::GameStorage));
	}

	if (config_.LogDirectory.empty()) {
		return L"";
	}

	BOOL created = CreateDirectoryW(config_.LogDirectory.c_str(), NULL);
	if (created == FALSE) {
		DWORD lastError = GetLastError();
		if (lastError != ERROR_ALREADY_EXISTS) {
			std::wstringstream err;
			err << L"Could not create log directory '" << config_.LogDirectory << "': Error " << lastError;
			return L"";
		}
	}

	auto now = std::chrono::system_clock::now();
	auto tt = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
	gmtime_s(&tm, &tt);

	std::wstringstream ss;
	ss << config_.LogDirectory << L"\\"
		<< Type << L" "
		<< std::setfill(L'0')
		<< (tm.tm_year + 1900) << L"-"
		<< std::setw(2) << (tm.tm_mon + 1) << L"-"
		<< std::setw(2) << tm.tm_mday << L" "
		<< std::setw(2) << tm.tm_hour << L"-"
		<< std::setw(2) << tm.tm_min << L"-"
		<< std::setw(2) << tm.tm_sec << L"." << Extension;
	return ss.str();
}

void ScriptExtender::OnStatsLoad(stats::RPGStats::LoadProc* wrapped, stats::RPGStats* mgr, ObjectSet<STDString>* paths)
{
	statLoadOrderHelper_.OnLoadStarted();

	if (client_.IsInClientThread()) {
		client_.LoadExtensionState(ExtensionStateContext::Load);
	} else if (server_.IsInServerThread()) {
		server_.LoadExtensionState(ExtensionStateContext::Load);
	}

	wrapped(mgr, paths);

	statLoadOrderHelper_.OnLoadFinished();
	if (client_.IsInClientThread()) {
		client_.LoadExtensionState(ExtensionStateContext::Game);
	} else if (server_.IsInServerThread()) {
		server_.LoadExtensionState(ExtensionStateContext::Game);
	}

	auto state = GetCurrentExtensionState();
	if (state) {
		state->OnStatsLoaded();
	}
}

bool ScriptExtender::HasFeatureFlag(char const * flag) const
{
	return (server_.HasExtensionState() && server_.GetExtensionState().HasFeatureFlag(flag))
		|| (client_.HasExtensionState() && client_.GetExtensionState().HasFeatureFlag(flag));
}

ExtensionStateBase* ScriptExtender::GetCurrentExtensionState()
{
	if (server_.IsInServerThread()) {
		if (server_.HasExtensionState()) {
			return &server_.GetExtensionState();
		} else {
			return nullptr;
		}
	} else if (client_.IsInClientThread()) {
		if (client_.HasExtensionState()) {
			return &client_.GetExtensionState();
		} else {
			return nullptr;
		}
	} else {
		WARN("Called from unknown thread %d?", GetCurrentThreadId());
		if (client_.HasExtensionState()) {
			return &client_.GetExtensionState();
		} else {
			return nullptr;
		}
	}
}

void ScriptExtender::OnCoreLibInit(void * self)
{
	PostStartup();
}

void ScriptExtender::OnAppUpdatePaths(void * self)
{
	if (!config_.CustomProfile.empty()) {
		auto& gameLocalPath = GetStaticSymbols().ls__PathRoots[4];
		if (gameLocalPath->ends_with("Baldur's Gate 3")) {
			*gameLocalPath = gameLocalPath->substr(0, gameLocalPath->size() - 15) + config_.CustomProfile.c_str();
		}
	}
}

void ScriptExtender::OnBaseModuleLoaded(void * self)
{
}


void ScriptExtender::OnSkillPrototypeManagerInit(void * self)
{
	if (!extensionsEnabled_) return;

	std::lock_guard _(globalStateLock_);
	auto extState = GetCurrentExtensionState();
	if (extState == nullptr) {
		ERR("Extension not initialized in OnSkillPrototypeManagerInit?");
		return;
	}

	auto modManager = extState->GetModManager();
	if (modManager == nullptr) {
		ERR("Module info not available in OnSkillPrototypeManagerInit?");
		return;
	}

	STDString loadMsg = "Loading ";
	loadMsg += modManager->BaseModule.Info.Name;
	loadMsg += " (Script Extender v";
	loadMsg += std::to_string(CurrentVersion);
#if defined(_DEBUG)
	loadMsg += " Devel";
#endif
	loadMsg += ")";
	Libraries.ShowStartupMessage(loadMsg, false);
	
	if (server_.IsInServerThread()) {
		server_.LoadExtensionState(ExtensionStateContext::Game);
	} else {
		client_.LoadExtensionState(ExtensionStateContext::Game);
	}

	extState->OnModuleLoading();
}


void ScriptExtender::ClearPathOverrides()
{
	std::unique_lock lock(pathOverrideMutex_);
	pathOverrides_.clear();
}


void ScriptExtender::AddPathOverride(STDString const & path, STDString const & overriddenPath)
{
	auto absolutePath = GetStaticSymbols().ToPath(path, PathRootType::Data);
	auto absoluteOverriddenPath = GetStaticSymbols().ToPath(overriddenPath, PathRootType::Data);

	std::unique_lock lock(pathOverrideMutex_);
	pathOverrides_.insert(std::make_pair(absolutePath, absoluteOverriddenPath));
}

std::optional<STDString> ScriptExtender::GetPathOverride(STDString const & path)
{
	auto absolutePath = GetStaticSymbols().ToPath(path, PathRootType::Data);

	std::unique_lock lock(pathOverrideMutex_);
	auto it = pathOverrides_.find(absolutePath);
	if (it != pathOverrides_.end()) {
		return it->second;
	} else {
		return {};
	}
}

FileReader * ScriptExtender::OnFileReaderCreate(FileReader::CtorProc* next, FileReader * self, Path const& path, unsigned int type, unsigned int unknown)
{
	if (!pathOverrides_.empty()) {
		std::shared_lock lock(pathOverrideMutex_);
		auto it = pathOverrides_.find(path.Name);
		if (it != pathOverrides_.end() && !client_.Hasher().isHashing()) {
			DEBUG("FileReader path override: %s -> %s", path.Name.c_str(), it->second.c_str());
			Path overriddenPath;
			overriddenPath.Name = it->second;
#if !defined(OSI_EOCAPP)
			overriddenPath.Unknown = path->Unknown;
#endif
			lock.unlock();
			return next(self, overriddenPath, type, unknown);
		}
	}

	if (path.Name.size() > 4 
		&& memcmp(path.Name.data() + path.Name.size() - 4, ".txt", 4) == 0) {
		statLoadOrderHelper_.OnStatFileOpened(path);
	}

	DisableCrashReporting _;
	return next(self, path, type, unknown);
}

void ScriptExtender::OnSavegameVisit(void* osirisHelpers, ObjectVisitor* visitor)
{
	savegameSerializer_.SavegameVisit(visitor);
}

void ScriptExtender::PostStartup()
{
	if (postStartupDone_) return;

	std::lock_guard _(globalStateLock_);
	// We need to initialize the function library here, as GlobalAllocator isn't available in Init().
	if (Libraries.PostStartupFindLibraries()) {
		lua::RegisterLibraries();
		TypeInformationRepository::GetInstance().Initialize();

		// FIXME - savegame hooks not yet added
		// gExtender->GetServer().Osiris().GetWrappers().InitializeDeferredExtensions();
		Hooks.HookAll();
		server_.PostStartup();
		client_.PostStartup();

		luaBuiltinBundle_.SetResourcePath(config_.LuaBuiltinResourceDirectory);
		if (!luaBuiltinBundle_.LoadBuiltinResource(IDR_LUA_BUILTIN_BUNDLE)) {
			ERR("Failed to load Lua builtin resource bundle!");
		}

		Hooks.FileReader__ctor.SetWrapper(&ScriptExtender::OnFileReaderCreate, this);
		Hooks.RPGStats__Load.SetWrapper(&ScriptExtender::OnStatsLoad, this);
	}

	GameVersionInfo gameVersion;
	if (Libraries.GetGameVersion(gameVersion) && !gameVersion.IsSupported()) {
		std::stringstream ss;
		ss << "Your game version (v" << gameVersion.Major << "." << gameVersion.Minor << "." << gameVersion.Revision << "." << gameVersion.Build
			<< ") is not supported by the Script Extender";
		Libraries.ShowStartupError(ss.str().c_str(), true, false);
	} else if (Libraries.CriticalInitializationFailed()) {
		Libraries.ShowStartupError("A severe error has occurred during Script Extender initialization. Extension features will be unavailable.", true, false);
	} else if (Libraries.InitializationFailed()) {
		Libraries.ShowStartupError("An error has occurred during Script Extender initialization. Some extension features might be unavailable.", true, false);
	}

	postStartupDone_ = true;
}

void ScriptExtender::InitRuntimeLogging()
{
	if (!config_.LogRuntime) return;

	auto path = MakeLogFilePath(L"Extender Runtime", L"log");
	gConsole.OpenLogFile(path);
	DEBUG(L"Extender runtime log written to '%s'", path.c_str());
}

}
