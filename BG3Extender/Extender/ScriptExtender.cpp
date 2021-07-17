#include "stdafx.h"
#include <Extender/ScriptExtender.h>
#include "Version.h"
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

void InitCrashReporting();
void ShutdownCrashReporting();

namespace bg3se
{

std::unique_ptr<ScriptExtender> gExtender;

#if !defined(OSI_NO_DEBUGGER)
void LuaDebugThreadRunner(LuaDebugInterface& intf)
{
	intf.Run();
}
#endif

ScriptExtender::ScriptExtender()
	: server_(config_)/*,
	hitProxy_(*this)*/
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

	if (!Libraries.FindLibraries(gameVersion.Revision)) {
		ERR("ScriptExtender::Initialize: Could not load libraries; skipping scripting extension initialization.");
		extensionsEnabled_ = false;
	}

	server_.Initialize();
	client_.Initialize();

	/*SkillPrototypeManagerInit.SetPreHook(std::bind(&ScriptExtender::OnSkillPrototypeManagerInit, this, _1));
	Wrappers.FileReader__ctor.SetWrapper(std::bind(&ScriptExtender::OnFileReaderCreate, this, _1, _2, _3, _4));
	Wrappers.esv__OsirisVariableHelper__SavegameVisit.SetPreHook(std::bind(&ScriptExtender::OnSavegameVisit, this, _1, _2));
	Wrappers.TranslatedStringRepository__UnloadOverrides.SetPreHook(std::bind(&ScriptExtender::OnModuleLoadStarted, this, _1));
	Wrappers.RPGStats__Load.AddPreHook(std::bind(&ScriptExtender::OnStatsLoadStarted, this, _1));
	Wrappers.RPGStats__Load.AddPostHook(std::bind(&ScriptExtender::OnStatsLoadFinished, this, _1));*/

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

/*
void ScriptExtender::OnStatsLoadStarted(RPGStats* mgr)
{
	statLoadOrderHelper_.OnLoadStarted();
}

void ScriptExtender::OnStatsLoadFinished(RPGStats* mgr)
{
	statLoadOrderHelper_.OnLoadFinished();
	auto state = GetCurrentExtensionState();
	if (state) {
		state->OnStatsLoaded();
	}
}*/

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

void ScriptExtender::OnBaseModuleLoaded(void * self)
{
}
/*
void ScriptExtender::OnModuleLoadStarted(TranslatedStringRepository* self)
{
	LoadExtensionStateClient();
	if (ClientExtState) {
		ClientExtState->OnModuleLoadStarted();
	}
}
*/

void ScriptExtender::PostStartup()
{
	if (postStartupDone_) return;

	std::lock_guard _(globalStateLock_);
	// We need to initialize the function library here, as GlobalAllocator isn't available in Init().
	if (Libraries.PostStartupFindLibraries()) {
		Hooks.HookAll();
		server_.PostStartup();
		client_.PostStartup();
		//hitProxy_.PostStartup();

		using namespace std::placeholders;
		Hooks.FileReader__ctor.SetWrapper(std::bind(&ScriptExtender::OnFileReaderCreate, this, _1, _2, _3, _4, _5));
	}

	GameVersionInfo gameVersion;
	if (Libraries.GetGameVersion(gameVersion) && !gameVersion.IsSupported()) {
		std::stringstream ss;
		ss << "Your game version (v" << gameVersion.Major << L"." << gameVersion.Minor << L"." << gameVersion.Revision << L"." << gameVersion.Build
			<< ") is not supported by the Script Extender";
		Libraries.ShowStartupError(ss.str().c_str(), true, false);
	} else if (Libraries.CriticalInitializationFailed()) {
		Libraries.ShowStartupError("A severe error has occurred during Script Extender initialization. Extension features will be unavailable.", true, false);
	} else if (Libraries.InitializationFailed()) {
		Libraries.ShowStartupError("An error has occurred during Script Extender initialization. Some extension features might be unavailable.", true, false);
	}

	postStartupDone_ = true;
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
		server_.LoadExtensionState();
	} else {
		client_.LoadExtensionState();
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

}
