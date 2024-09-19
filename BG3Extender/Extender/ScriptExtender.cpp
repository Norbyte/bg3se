#include "stdafx.h"
#include <Extender/ScriptExtender.h>
#include <Extender/Shared/Console.h>
#include "Version.h"
#include "resource.h"
#include <iomanip>

#include <Extender/Shared/StatLoadOrderHelper.inl>
#include <Extender/Shared/UserVariables.inl>
#include <Extender/Shared/UseActions.inl>
#include <Extender/Shared/VirtualTextures.inl>

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
	client_(config_),
	imgui_(client_.GetSDL())
{
	gCoreLibPlatformInterface.GlobalConsole = new DebugConsole();
}

void ScriptExtender::WarnIfOffline()
{
	// Check if we were loaded through the updaters BG3ScriptExtender.dll postload mechanism.
	// If not, show a note that it's not an auto-updating build
	wchar_t path[1024];
	auto len = GetModuleFileNameW(gCoreLibPlatformInterface.ThisModule, path, std::size(path));
	if (len == 0) return;

	path[len] = 0;
	auto sep = wcsrchr(path, L'\\');
	if (sep == nullptr) return;
	sep++;

	auto end = wcschr(sep, L'.');
	if (end == nullptr) return;
	*end = 0;

	if (_wcsicmp(sep, L"BG3ScriptExtender") != 0) {
		WARN("NOTICE: Script Extender loaded without an updater present; new updates will not be fetched automatically");
	}
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
			// Hard exit below a certain version as the EoCClient error display UI won't work anymore
			Fail("Script Extender doesn't support game versions below v4.57, please upgrade!");
		}
	} else {
		ERR("Failed to retrieve game version info.");
	}

	WarnIfOffline();

	DEBUG("ScriptExtender::Initialize: Starting");
	auto initStart = std::chrono::high_resolution_clock::now();

	InitStaticSymbols();

	if (!Libraries.FindLibraries(gameVersion.Revision)) {
		ERR("ScriptExtender::Initialize: Could not load libraries; skipping scripting extension initialization.");
	}

#if defined(ENABLE_IMGUI)
	if (!Libraries.CriticalInitializationFailed()) {
		imgui_.EnableHooks();
	}
#endif

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

	auto app = GetStaticSymbols().AppInstance;
	if (app && *app) {
		DEBUG("We're already past App load, triggering CoreLibInit");
		OnCoreLibInit(nullptr);
	}

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

void ScriptExtender::OnStatsLoadGuarded(stats::RPGStats::LoadProc* wrapped, stats::RPGStats* mgr, Array<STDString>* paths)
{
	// Stats load is scheduled from the client on the shared worker pool
	client_.AddThread(GetCurrentThreadId());

	// Ensure that we have an empty extension state ready in case OnStatsLoad() is the first callback
	// due to late load
	if (!client_.HasExtensionState()) {
		client_.ResetExtensionState();
	}

	statLoadOrderHelper_.OnLoadStarted();
	client_.LoadExtensionState(ExtensionStateContext::Load);

	{
		DisableCrashReporting _;
		wrapped(mgr, paths);
	}

	client_.LoadExtensionState(ExtensionStateContext::Game);

	if (client_.HasExtensionState()) {
		client_.GetExtensionState().OnStatsLoaded();
	}

#if defined(ENABLE_IMGUI)
	imgui_.EnableUI(true);
#endif

	client_.RemoveThread(GetCurrentThreadId());
}

void ScriptExtender::OnStatsLoad(stats::RPGStats::LoadProc* wrapped, stats::RPGStats* mgr, Array<STDString>* paths)
{
	BEGIN_GUARDED()
	OnStatsLoadGuarded(wrapped, mgr, paths);
	END_GUARDED()
}

void ScriptExtender::OnECSUpdate(ecs::EntityWorld::UpdateProc* wrapped, ecs::EntityWorld* entityWorld, GameTime const& time)
{
	BEGIN_GUARDED()
	OnECSUpdateGuarded(wrapped, entityWorld, time);
	END_GUARDED()
}

void ScriptExtender::OnECSUpdateGuarded(ecs::EntityWorld::UpdateProc* wrapped, ecs::EntityWorld* entityWorld, GameTime const& time)
{
	auto ecs = GetECS(entityWorld);
	if (ecs != nullptr) {
		ecs->Update();
		
		{
			DisableCrashReporting _;
			wrapped(entityWorld, time);
		}

		ecs->PostUpdate();

		if (entityWorld->Replication) {
			if (GetServer().HasExtensionState()) {
				esv::LuaServerPin lua(GetServer().GetExtensionState());
				if (lua) {
					lua->GetComponentEventHooks().FireDeferredEvents();
				}
			}
		} else {
			if (GetClient().HasExtensionState()) {
				ecl::LuaClientPin lua(GetClient().GetExtensionState());
				if (lua) {
					lua->GetComponentEventHooks().FireDeferredEvents();
				}
			}
		}

		if (entityWorld->Replication && GetServer().HasExtensionState()) {
			esv::LuaServerPin lua(GetServer().GetExtensionState());
			if (lua) {
				lua->GetReplicationEventHooks()->OnEntityReplication(*entityWorld);
			}
		}
	} else {
		wrapped(entityWorld, time);
	}
}

void ScriptExtender::OnECSFlushECBs(ecs::EntityWorld* entityWorld)
{
	BEGIN_GUARDED()
	auto ecs = GetECS(entityWorld);
	if (ecs != nullptr) {
		ecs->OnFlushECBs();
	}
	END_GUARDED()
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

ecs::EntitySystemHelpersBase* ScriptExtender::GetECS(ecs::EntityWorld* world)
{
	if (world == GetStaticSymbols().GetClientEntityWorld()) {
		return &client_.GetEntityHelpers();
	} else if (world == GetStaticSymbols().GetServerEntityWorld()) {
		return &server_.GetEntityHelpers();
	} else  {
		return nullptr;
	}
}

void ScriptExtender::OnCoreLibInit(void * self)
{
	PostStartup();
}

void ScriptExtender::OnAppUpdatePaths(void * self)
{
	if (!config_.CustomProfile.empty() && GetStaticSymbols().ls__PathRoots) {
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
		BEGIN_GUARDED()
		gExtender->GetClient().OnGameStateChanged(states.From, states.To);
		END_GUARDED()

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
		BEGIN_GUARDED()
		gExtender->GetServer().OnGameStateChanged(states.From, states.To);
		END_GUARDED()

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
	HookStateMachineUpdates();
}

void ScriptExtender::HookStateMachineUpdates()
{
	if (updateHooksAdded_ || Libraries.CriticalInitializationFailed()) return;

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

	updateHooksAdded_ = true;
}

void ScriptExtender::OnBaseModuleLoaded(void * self)
{
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
		if (it != pathOverrides_.end()) {
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
		//engineHooks_.Kernel_FindFirstFileW.SetWrapper(&ScriptExtender::OnFindFirstFileW, this);
		//engineHooks_.Kernel_FindNextFileW.SetWrapper(&ScriptExtender::OnFindNextFileW, this);
		//engineHooks_.Kernel_FindClose.SetWrapper(&ScriptExtender::OnFindClose, this);
		
		engineHooks_.RPGStats__Load.SetWrapper(&ScriptExtender::OnStatsLoad, this);
		engineHooks_.ecs__EntityWorld__Update.SetWrapper(&ScriptExtender::OnECSUpdate, this);
		engineHooks_.ecs__EntityWorld__FlushECBs.SetPreHook(&ScriptExtender::OnECSFlushECBs, this);
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

	HookStateMachineUpdates();

	postStartupDone_ = true;
}

void ScriptExtender::InitRuntimeLogging()
{
	if (!config_.LogRuntime) return;

	auto path = MakeLogFilePath(L"Extender Runtime", L"log");
	gCoreLibPlatformInterface.GlobalConsole->OpenLogFile(path);
	DEBUG("Extender runtime log written to '%s'", ToStdUTF8(path).c_str());
}

void ScriptExtender::OnSDLEvent(SDL_Event* event)
{
	if (event->type == SDL_KEYDOWN 
		&& event->key.keysym.scancode == SDL_SCANCODE_F12 
		&& (unsigned)((SDLKeyModifier)event->key.keysym.mod & (SDLKeyModifier::LCtrl | SDLKeyModifier::RCtrl)) != 0) {
		
		auto console = gCoreLibPlatformInterface.GlobalConsole;
		if (!console->WasCreated()) {
			console->Create();
		}

	}
}

}
