#include "stdafx.h"
#include <Extender/ScriptExtender.h>
#include <Extender/Shared/Console.h>
#include "Version.h"
#include "resource.h"
#include <iomanip>

#include <Extender/Shared/StatLoadOrderHelper.inl>
#include <Extender/Shared/UserVariables.inl>

#undef DEBUG_SERVER_CLIENT

namespace bg3se
{

void InitCrashReporting();
void ShutdownCrashReporting();

decltype(ScriptExtender::CoreLibInit)* decltype(ScriptExtender::CoreLibInit)::gHook;
decltype(ScriptExtender::AppUpdatePaths)* decltype(ScriptExtender::AppUpdatePaths)::gHook;
decltype(ScriptExtender::AppLoadGraphicSettings)* decltype(ScriptExtender::AppLoadGraphicSettings)::gHook;

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
	gCoreLibPlatformInterface.GlobalConsole = new DebugConsole();
}

void ScriptExtender::Initialize()
{
	if (config_.SendCrashReports) {
		InitCrashReporting();
	}

	GameVersionInfo gameVersion;
	if (Libraries.GetGameVersion(gameVersion)) {
		if (gameVersion.IsSupported()) {
			INFO("Game version v%d.%d.%d.%d OK", gameVersion.Major, gameVersion.Minor, gameVersion.Revision, gameVersion.Build);
		} else {
			ERR("Game version v%d.%d.%d.%d is not supported, please upgrade!", gameVersion.Major, gameVersion.Minor, gameVersion.Revision, gameVersion.Build);
			// Hard exit below a certain version as th EoCClient error display UI won't work anymore
			Fail("Script Extender doesn't support game versions below v4.37, please upgrade!");
		}
	} else {
		ERR("Failed to retrieve game version info.");
	}

	DEBUG("ScriptExtender::Initialize: Starting");
	auto initStart = std::chrono::high_resolution_clock::now();

	InitStaticSymbols();

	if (!Libraries.FindLibraries(gameVersion.Revision)) {
		ERR("ScriptExtender::Initialize: Could not load libraries; skipping scripting extension initialization.");
	}

	server_.Initialize();
	client_.Initialize();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (GetStaticSymbols().App__Ctor != nullptr) {
		CoreLibInit.Wrap(GetStaticSymbols().App__Ctor);
		CoreLibInit.SetPreHook(&ScriptExtender::OnCoreLibInit, this);
	}

	if (GetStaticSymbols().App__UpdatePaths != nullptr) {
		AppUpdatePaths.Wrap(GetStaticSymbols().App__UpdatePaths);
		AppUpdatePaths.SetPostHook(&ScriptExtender::OnAppUpdatePaths, this);
	}

	if (GetStaticSymbols().App__LoadGraphicSettings != nullptr) {
		AppLoadGraphicSettings.Wrap(GetStaticSymbols().App__LoadGraphicSettings);
		AppLoadGraphicSettings.SetPostHook(&ScriptExtender::OnAppLoadGraphicSettings, this);
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
	engineHooks_.UnhookAll();
}

void ScriptExtender::LogLuaError(std::string_view msg)
{
	gCoreLibPlatformInterface.GlobalConsole->Print(DebugMessageType::Error, msg.data());
	server_.Osiris().LogError(msg);
}

void ScriptExtender::LogOsirisError(std::string_view msg)
{
	gCoreLibPlatformInterface.GlobalConsole->Print(DebugMessageType::Error, msg.data());
	server_.Osiris().LogError(msg);

#if !defined(OSI_NO_DEBUGGER)
	if (luaDebugger_) {
		luaDebugger_->OnGenericError(msg.data());
	}
#endif
}

void ScriptExtender::LogOsirisWarning(std::string_view msg)
{
	gCoreLibPlatformInterface.GlobalConsole->Print(DebugMessageType::Warning, msg.data());
	server_.Osiris().LogWarning(msg);
}

void ScriptExtender::LogOsirisMsg(std::string_view msg)
{
	gCoreLibPlatformInterface.GlobalConsole->Print(DebugMessageType::Osiris, msg.data());
	server_.Osiris().LogMessage(msg);
}

std::wstring ScriptExtender::MakeLogFilePath(std::wstring const & Type, std::wstring const & Extension)
{
	if (config_.LogDirectory.empty()) {
		config_.LogDirectory = FromUTF8(GetStaticSymbols().ToPath("/Script Extender Logs", PathRootType::UserProfile));
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

void ScriptExtender::OnStatsLoad(stats::RPGStats::LoadProc* wrapped, stats::RPGStats* mgr, Array<STDString>* paths)
{
	// Stats load is scheduled from the client on the shared worker pool
	client_.AddThread(GetCurrentThreadId());

	statLoadOrderHelper_.OnLoadStarted();
	client_.LoadExtensionState(ExtensionStateContext::Load);

	wrapped(mgr, paths);

	statLoadOrderHelper_.OnLoadFinished();
	client_.LoadExtensionState(ExtensionStateContext::Game);

	if (client_.HasExtensionState()) {
		client_.GetExtensionState().OnStatsLoaded();
	}

	client_.RemoveThread(GetCurrentThreadId());
}

void ScriptExtender::OnECSUpdate(ecs::EntityWorld* entityWorld, GameTime const& time)
{
	if (entityWorld->Replication && GetServer().HasExtensionState()) {
		esv::LuaServerPin lua(GetServer().GetExtensionState());
		if (lua) {
			lua->GetEntityEventHooks()->OnEntityReplication(*entityWorld);
		}
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

class ClientEventManagerHook
{
public:
	struct GameStates
	{
		ecl::GameState From;
		ecl::GameState To;
	};

	virtual ~ClientEventManagerHook()
	{}

	virtual bool OnGameStateChanged(GameStates& states)
	{
		gExtender->GetClient().OnGameStateChanged(states.From, states.To);
		return true;
	}

	virtual bool Unknown()
	{
		return false;
	}

	uint64_t dummy{ 0 };
};

class ServerEventManagerHook
{
public:
	struct GameStates
	{
		esv::GameState From;
		esv::GameState To;
	};

	virtual ~ServerEventManagerHook()
	{}

	virtual bool OnGameStateChanged(GameStates& states)
	{
		gExtender->GetServer().OnGameStateChanged(states.From, states.To);
		return true;
	}

	virtual bool Unknown()
	{
		return false;
	}

	uint64_t dummy{ 0 };
};

void ScriptExtender::OnAppLoadGraphicSettings(App * self)
{
	auto clientEvtMgr = GetStaticSymbols().ecl__gGameStateEventManager;
	if (clientEvtMgr && *clientEvtMgr) {
		auto client = GameAlloc<ClientEventManagerHook>();
		(*clientEvtMgr)->Callbacks.push_back(&client->dummy);
	}

	auto serverEvtMgr = GetStaticSymbols().esv__gGameStateEventManager;
	if (serverEvtMgr && *serverEvtMgr) {
		auto server = GameAlloc<ServerEventManagerHook>();
		(*serverEvtMgr)->Callbacks.push_back(&server->dummy);
	}
}

void ScriptExtender::OnBaseModuleLoaded(void * self)
{
}


void ScriptExtender::OnSkillPrototypeManagerInit(void * self)
{
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

void ScriptExtender::PostStartup()
{
	if (postStartupDone_) return;

	std::lock_guard _(globalStateLock_);
	// We need to initialize the function library here, as GlobalAllocator isn't available in Init().
	if (Libraries.PostStartupFindLibraries()) {
		lua::RegisterLibraries();
		lua::InitObjectProxyPropertyMaps();
		TypeInformationRepository::GetInstance().Initialize();

		engineHooks_.HookAll();
		hooks_.Startup();
		server_.PostStartup();
		client_.PostStartup();

		luaBuiltinBundle_.SetResourcePath(config_.LuaBuiltinResourceDirectory);
		if (!luaBuiltinBundle_.LoadBuiltinResource(IDR_LUA_BUILTIN_BUNDLE)) {
			ERR("Failed to load Lua builtin resource bundle!");
		}

		engineHooks_.FileReader__ctor.SetWrapper(&ScriptExtender::OnFileReaderCreate, this);
		engineHooks_.RPGStats__Load.SetWrapper(&ScriptExtender::OnStatsLoad, this);
		engineHooks_.ecs__EntityWorld__Update.SetPostHook(&ScriptExtender::OnECSUpdate, this);
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
	gCoreLibPlatformInterface.GlobalConsole->OpenLogFile(path);
	DEBUG("Extender runtime log written to '%s'", ToStdUTF8(path).c_str());
}

}
