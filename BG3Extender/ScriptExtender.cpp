#include "stdafx.h"
#include "ScriptExtender.h"
#include "Version.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <psapi.h>
#include <regex>


#undef DEBUG_SERVER_CLIENT

void InitCrashReporting();
void ShutdownCrashReporting();

namespace bg3se
{

std::unique_ptr<ScriptExtender> gExtender;

#define STATIC_HOOK(name) decltype(ScriptExtender::name) * decltype(ScriptExtender::name)::gHook;
STATIC_HOOK(clientGameStateWorkerStart_)
STATIC_HOOK(serverGameStateWorkerStart_)
STATIC_HOOK(clientGameStateChangedEvent_)
STATIC_HOOK(serverGameStateChangedEvent_)

#if !defined(OSI_NO_DEBUGGER)
void LuaDebugThreadRunner(LuaDebugInterface& intf)
{
	intf.Run();
}
#endif

ScriptExtender::ScriptExtender()
	: osiris_(config_)/*,
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

	if (Libraries.FindLibraries(gameVersion.Revision)) {
		if (extensionsEnabled_) {
			ResetExtensionStateServer();
			ResetExtensionStateClient();
		} else {
			DEBUG("ScriptExtender::Initialize: Skipped library init -- scripting extensions not enabled.");
		}
	} else {
		ERR("ScriptExtender::Initialize: Could not load libraries; skipping scripting extension initialization.");
		extensionsEnabled_ = false;
	}

	// Wrap state change functions even if extension startup failed, otherwise
	// we won't be able to show any startup errors

	auto& lib = GetStaticSymbols();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (lib.ecl__GameStateEventManager__ExecuteGameStateChangedEvent != nullptr) {
		clientGameStateChangedEvent_.Wrap(lib.ecl__GameStateEventManager__ExecuteGameStateChangedEvent);
	}

	if (lib.esv__GameStateEventManager__ExecuteGameStateChangedEvent != nullptr) {
		serverGameStateChangedEvent_.Wrap(lib.esv__GameStateEventManager__ExecuteGameStateChangedEvent);
	}

	if (lib.ecl__GameStateThreaded__GameStateWorker__DoWork != nullptr) {
		clientGameStateWorkerStart_.Wrap(lib.ecl__GameStateThreaded__GameStateWorker__DoWork);
	}

	if (lib.esv__GameStateThreaded__GameStateWorker__DoWork != nullptr) {
		serverGameStateWorkerStart_.Wrap(lib.esv__GameStateThreaded__GameStateWorker__DoWork);
	}

	DetourTransactionCommit();

	using namespace std::placeholders;
	clientGameStateChangedEvent_.SetPostHook(std::bind(&ScriptExtender::OnClientGameStateChanged, this, _1, _2, _3));
	serverGameStateChangedEvent_.SetPostHook(std::bind(&ScriptExtender::OnServerGameStateChanged, this, _1, _2, _3));
	clientGameStateWorkerStart_.AddPreHook(std::bind(&ScriptExtender::OnClientGameStateWorkerStart, this, _1));
	serverGameStateWorkerStart_.AddPreHook(std::bind(&ScriptExtender::OnServerGameStateWorkerStart, this, _1));
	clientGameStateWorkerStart_.AddPostHook(std::bind(&ScriptExtender::OnClientGameStateWorkerExit, this, _1));
	serverGameStateWorkerStart_.AddPostHook(std::bind(&ScriptExtender::OnServerGameStateWorkerExit, this, _1));

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
	ResetExtensionStateServer();
	ResetExtensionStateClient();
	Hooks.UnhookAll();
	osiris_.Shutdown();
}

void ScriptExtender::LogLuaError(std::string_view msg)
{
	gConsole.Debug(DebugMessageType::Error, msg.data());
	osiris_.LogError(msg);
}

void ScriptExtender::LogOsirisError(std::string_view msg)
{
	gConsole.Debug(DebugMessageType::Error, msg.data());
	osiris_.LogError(msg);

#if !defined(OSI_NO_DEBUGGER)
	if (luaDebugger_) {
		luaDebugger_->OnGenericError(msg.data());
	}
#endif
}

void ScriptExtender::LogOsirisWarning(std::string_view msg)
{
	gConsole.Debug(DebugMessageType::Warning, msg.data());
	osiris_.LogWarning(msg);
}

void ScriptExtender::LogOsirisMsg(std::string_view msg)
{
	gConsole.Debug(DebugMessageType::Osiris, msg.data());
	osiris_.LogMessage(msg);
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
	return (ServerExtState && ServerExtState->HasFeatureFlag(flag))
		|| (ClientExtState && ClientExtState->HasFeatureFlag(flag));
}

ExtensionStateBase* ScriptExtender::GetCurrentExtensionState()
{
	auto tid = GetCurrentThreadId();
	if (ServerThreadIds.find(tid) != ServerThreadIds.end()) {
		return ServerExtState.get();
	} else if (ClientThreadIds.find(tid) != ClientThreadIds.end()) {
		return ClientExtState.get();
	} else {
		WARN("Called from unknown thread %d?", tid);
		return ClientExtState.get();
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
char const * ClientGameStateNames[] =
{
	"Unknown",
	"Init",
	"InitMenu",
	"InitNetwork",
	"InitConnection",
	"Idle",
	"LoadMenu",
	"Menu",
	"Exit",
	"SwapLevel",
	"LoadLevel",
	"LoadModule",
	"LoadSession",
	"LoadGMCampaign",
	"UnloadLevel",
	"UnloadModule",
	"UnloadSession",
	"Paused",
	"PrepareRunning",
	"Running",
	"Disconnect",
	"Join",
	"Save",
	"StartLoading",
	"StopLoading",
	"StartServer",
	"Movie",
	"Installation",
	"GameMasterPause",
	"ModReceiving",
	"Lobby",
	"BuildStory",
	"LoadLoca"
};


char const * ServerGameStateNames[] =
{
	"Unknown",
	"Uninitialized",
	"Init",
	"Idle",
	"Exit",
	"LoadLevel",
	"LoadModule",
	"LoadGMCampaign",
	"LoadSession",
	"UnloadLevel",
	"UnloadModule",
	"UnloadSession",
	"Sync",
	"Paused",
	"Running",
	"Save",
	"Disconnect",
	"GameMasterPause",
	"BuildStory",
	"ReloadStory",
	"Installation"
};

void ScriptExtender::PostStartup()
{
	if (postStartupDone_) return;

	std::lock_guard _(globalStateLock_);
	// We need to initialize the function library here, as GlobalAllocator isn't available in Init().
	if (!Libraries.CriticalInitializationFailed()) {
		osiris_.Initialize();
	}

	if (Libraries.PostStartupFindLibraries()) {
		Hooks.HookAll();
		clientEntityHelpers_.Setup();
		serverEntityHelpers_.Setup();
		//hitProxy_.PostStartup();
		hasher_.PostStartup();

		using namespace std::placeholders;
		Hooks.FileReader__ctor.SetWrapper(std::bind(&ScriptExtender::OnFileReaderCreate, this, _1, _2, _3, _4, _5));
	}

	GameVersionInfo gameVersion;
	if (Libraries.GetGameVersion(gameVersion) && !gameVersion.IsSupported()) {
		std::wstringstream ss;
		ss << L"Your game version (v" << gameVersion.Major << L"." << gameVersion.Minor << L"." << gameVersion.Revision << L"." << gameVersion.Build
			<< L") is not supported by the Script Extender";
		Libraries.ShowStartupError(ss.str().c_str(), true, false);
	} else if (Libraries.CriticalInitializationFailed()) {
		Libraries.ShowStartupError(L"A severe error has occurred during Script Extender initialization. Extension features will be unavailable.", true, false);
	} else if (Libraries.InitializationFailed()) {
		Libraries.ShowStartupError(L"An error has occurred during Script Extender initialization. Some extension features might be unavailable.", true, false);
	}

	postStartupDone_ = true;
}

void ScriptExtender::OnClientGameStateChanged(void * self, ecl::GameState fromState, ecl::GameState toState)
{
	if (config_.SendCrashReports) {
		// We need to initialize the crash reporter after the game engine has started,
		// otherwise the game will overwrite the top level exception filter
		InitCrashReporting();
	}

	// Check to make sure that startup is done even if the extender was loaded when the game was already in GameState::Init
	if (toState != ecl::GameState::Unknown
		&& toState != ecl::GameState::StartLoading
		&& toState != ecl::GameState::InitMenu
		&& !Libraries.InitializationFailed()) {
		PostStartup();
	}

	if (toState == ecl::GameState::Menu && Libraries.InitializationFailed()) {
		PostStartup();
	}

#if defined(DEBUG_SERVER_CLIENT)
	DEBUG("ScriptExtender::OnClientGameStateChanged(): %s -> %s", 
		ClientGameStateNames[(unsigned)fromState], ClientGameStateNames[(unsigned)toState]);
#endif

	if (!extensionsEnabled_) return;

	if (fromState != ecl::GameState::Unknown) {
		AddClientThread(GetCurrentThreadId());
	}

	switch (fromState) {
	case ecl::GameState::LoadModule:
		INFO("ScriptExtender::OnClientGameStateChanged(): Loaded module");
		LoadExtensionStateClient();
		break;

	case ecl::GameState::LoadSession:
		if (ClientExtState) {
			ClientExtState->OnGameSessionLoaded();
		}
		break;

	case ecl::GameState::InitConnection:
		//networkManager_.ExtendNetworkingClient();
		break;
	}

	switch (toState) {
	case ecl::GameState::LoadModule:
		osiris_.InitRuntimeLogging();
		break;

	case ecl::GameState::InitNetwork:
	case ecl::GameState::Disconnect:
		//networkManager_.ClientReset();
		break;

	case ecl::GameState::UnloadModule:
		hasher_.ClearCaches();
		break;

	case ecl::GameState::UnloadSession:
		INFO("ScriptExtender::OnClientGameStateChanged(): Unloading session");
		ResetExtensionStateClient();
		break;

	case ecl::GameState::LoadGMCampaign:
		INFO("ScriptExtender::OnClientGameStateChanged(): Loading GM campaign");
		LoadExtensionStateClient();
		//networkManager_.ExtendNetworkingClient();
		break;

	case ecl::GameState::LoadSession:
		INFO("ScriptExtender::OnClientGameStateChanged(): Loading game session");
		LoadExtensionStateClient();
		//networkManager_.ExtendNetworkingClient();
		if (ClientExtState) {
			ClientExtState->OnGameSessionLoading();
		}
		break;
	}

	ecl::LuaClientPin lua(ecl::ExtensionState::Get());
	if (lua) {
		lua->OnGameStateChanged(fromState, toState);
	}
}

void ScriptExtender::OnServerGameStateChanged(void * self, esv::GameState fromState, esv::GameState toState)
{
#if defined(DEBUG_SERVER_CLIENT)
		DEBUG("ScriptExtender::OnServerGameStateChanged(): %s -> %s", 
			ServerGameStateNames[(unsigned)fromState], ServerGameStateNames[(unsigned)toState]);
#endif

	if (fromState != esv::GameState::Unknown) {
		AddServerThread(GetCurrentThreadId());
	}

	if (!extensionsEnabled_) return;

	switch (fromState) {
	case esv::GameState::LoadModule:
		INFO("ScriptExtender::OnServerGameStateChanged(): Loaded module");
		LoadExtensionStateServer();
		break;

	case esv::GameState::LoadSession:
		if (ServerExtState) {
			ServerExtState->OnGameSessionLoaded();
			auto comp = GetServerEntityHelpers().GetComponent<esv::Character>("12345678-1234-1234-1234-123456781234");
		}
		break;
	}

	switch (toState) {
	// Gift Bag selection in the main menu reloads the module without reconnecting
	// We only need to reset the extender enabled peer list on a disconnect.
	case esv::GameState::Disconnect:
		//networkManager_.ServerReset();
		break;

	case esv::GameState::UnloadSession:
		INFO("ScriptExtender::OnServerGameStateChanged(): Unloading session");
		ResetExtensionStateServer();
		break;

	case esv::GameState::LoadModule:
		//networkManager_.ExtendNetworkingServer();
		break;

	case esv::GameState::LoadGMCampaign:
		INFO("ScriptExtender::OnServerGameStateChanged(): Loading GM campaign");
		LoadExtensionStateServer();
		//networkManager_.ExtendNetworkingServer();
		break;

	case esv::GameState::LoadSession:
		INFO("ScriptExtender::OnServerGameStateChanged(): Loading game session");
		LoadExtensionStateServer();
		//networkManager_.ExtendNetworkingServer();
		if (ServerExtState) {
			ServerExtState->OnGameSessionLoading();
		}
		break;
	}

	esv::LuaServerPin lua(esv::ExtensionState::Get());
	if (lua) {
		lua->OnGameStateChanged(fromState, toState);
	}
}

void ScriptExtender::AddClientThread(DWORD threadId)
{
	if (ClientThreadIds.find(threadId) == ClientThreadIds.end()) {
#if defined(DEBUG_SERVER_CLIENT)
		DEBUG("Registered client TID %d", threadId);
#endif
		ClientThreadIds.insert(threadId);
	}
}

void ScriptExtender::AddServerThread(DWORD threadId)
{
	if (ServerThreadIds.find(threadId) == ServerThreadIds.end()) {
#if defined(DEBUG_SERVER_CLIENT)
		DEBUG("Registered server TID %d", threadId);
#endif
		ServerThreadIds.insert(threadId);
	}
}

void ScriptExtender::RemoveClientThread(DWORD threadId)
{
	auto it = ClientThreadIds.find(threadId);
	if (it != ClientThreadIds.end()) {
#if defined(DEBUG_SERVER_CLIENT)
		DEBUG("Unregistered client TID %d", threadId);
#endif
		ClientThreadIds.erase(it);
	}
}

void ScriptExtender::RemoveServerThread(DWORD threadId)
{
	auto it = ServerThreadIds.find(threadId);
	if (it != ServerThreadIds.end()) {
#if defined(DEBUG_SERVER_CLIENT)
		DEBUG("Unregistered server TID %d", threadId);
#endif
		ServerThreadIds.erase(it);
	}
}

void ScriptExtender::OnClientGameStateWorkerStart(void * self)
{
	AddClientThread(GetCurrentThreadId());
}

void ScriptExtender::OnServerGameStateWorkerStart(void * self)
{
	AddServerThread(GetCurrentThreadId());
}

void ScriptExtender::OnClientGameStateWorkerExit(void* self)
{
	RemoveClientThread(GetCurrentThreadId());
}

void ScriptExtender::OnServerGameStateWorkerExit(void* self)
{
	RemoveServerThread(GetCurrentThreadId());
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

	STDWString loadMsg = L"Loading ";
	loadMsg += modManager->BaseModule.Info.Name;
	loadMsg += L" (Script Extender v";
	loadMsg += std::to_wstring(CurrentVersion);
#if defined(_DEBUG)
	loadMsg += L" Devel";
#endif
	loadMsg += L")";
	Libraries.ShowStartupMessage(loadMsg, false);
	
	if (extState == ServerExtState.get()) {
		LoadExtensionStateServer();
	} else {
		LoadExtensionStateClient();
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

bool ScriptExtender::IsInServerThread() const
{
	auto tid = GetCurrentThreadId();
	return ServerThreadIds.find(tid) != ServerThreadIds.end();
}

bool ScriptExtender::IsInClientThread() const
{
	auto tid = GetCurrentThreadId();
	return ClientThreadIds.find(tid) != ClientThreadIds.end();
}

void ScriptExtender::AttachConsoleThread(bool server)
{
	auto tid = GetCurrentThreadId();
	if (server) {
		ServerThreadIds.insert(tid);
		auto it = ClientThreadIds.find(tid);
		if (it != ClientThreadIds.end()) {
			ClientThreadIds.erase(it);
		}
	} else {
		ClientThreadIds.insert(tid);
		auto it = ServerThreadIds.find(tid);
		if (it != ServerThreadIds.end()) {
			ServerThreadIds.erase(it);
		}
	}
}

void ScriptExtender::ResetLuaState(bool resetServer, bool resetClient)
{
	bool serverThread = IsInServerThread();

	if (resetServer && ServerExtState && ServerExtState->GetLua()) {
		AttachConsoleThread(true);

		auto& ext = *ServerExtState;
		ext.AddPostResetCallback([&ext]() {
			ext.OnModuleResume();
			auto state = GetStaticSymbols().GetServerState();
			if (state && (state == esv::GameState::Paused || state == esv::GameState::Running || state == esv::GameState::GameMasterPause)) {
				ext.OnGameSessionLoading();
				ext.OnGameSessionLoaded();
				ext.StoryLoaded();
				ext.OnResetCompleted();
			}
		});
		ext.LuaReset(true);
	}

	if (resetClient) {
		auto server = GetEoCServer();
		if (server && server->GameServer && false /* networking not available yet! */) {
			// Reset clients via a network message if the server is running
			/*auto& networkMgr = gExtender->GetNetworkManager();
			auto msg = networkMgr.GetFreeServerMessage(ReservedUserId);
			if (msg != nullptr) {
				auto resetMsg = msg->GetMessage().mutable_s2c_reset_lua();
				resetMsg->set_bootstrap_scripts(true);
				networkMgr.ServerBroadcast(msg, ReservedUserId);
			} else {
				OsiErrorS("Could not get free message!");
			}*/

		} else if (ClientExtState && ClientExtState->GetLua()) {
			// Do a direct (local) reset if server is not available (main menu, etc.)
			auto& ext = *ClientExtState;
			AttachConsoleThread(false);

			ext.AddPostResetCallback([&ext]() {
				ext.OnModuleResume();
				auto state = GetStaticSymbols().GetClientState();
				if (state && (state == ecl::GameState::Paused || state == ecl::GameState::Running || state == ecl::GameState::GameMasterPause)) {
					ext.OnGameSessionLoading();
					ext.OnGameSessionLoaded();
					ext.OnResetCompleted();
				}
			});
			ext.LuaReset(true);
		}
	}

	AttachConsoleThread(serverThread);
}

FileReader * ScriptExtender::OnFileReaderCreate(FileReader::CtorProc* next, FileReader * self, Path const& path, unsigned int type, unsigned int unknown)
{
	if (!pathOverrides_.empty()) {
		std::shared_lock lock(pathOverrideMutex_);
		auto it = pathOverrides_.find(path.Name);
		if (it != pathOverrides_.end() && !hasher_.isHashing()) {
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

void ScriptExtender::ResetExtensionStateServer()
{
	std::lock_guard _(globalStateLock_);
	ServerExtState = std::make_unique<esv::ExtensionState>();
	ServerExtState->Reset();
	ServerExtensionLoaded = false;
}

extern std::unordered_map<int32_t*, char*> maps;

void ScriptExtender::LoadExtensionStateServer()
{
	std::lock_guard _(globalStateLock_);
	if (ServerExtensionLoaded) return;
	
	if (extensionsEnabled_) {
		PostStartup();

		if (!ServerExtState) {
			ResetExtensionStateServer();
		}

		ServerExtState->LoadConfigs();
	}

	if (extensionsEnabled_ && !Libraries.CriticalInitializationFailed()) {
		//networkManager_.ExtendNetworkingServer();
		DEBUG("ScriptExtender::LoadExtensionStateServer(): Re-initializing module state.");
		osiris_.OnBaseModuleLoadedServer();
		ServerExtState->LuaReset(true);
	}

	ServerExtensionLoaded = true;
}

void ScriptExtender::ResetExtensionStateClient()
{
	std::lock_guard _(globalStateLock_);
	ClientExtState = std::make_unique<ecl::ExtensionState>();
	ClientExtState->Reset();
	ClearPathOverrides();
	ClientExtensionLoaded = false;
}

void ScriptExtender::LoadExtensionStateClient()
{
	std::lock_guard _(globalStateLock_);
	if (ClientExtensionLoaded) return;

	if (extensionsEnabled_) {
		PostStartup();

		if (!ClientExtState) {
			ResetExtensionStateClient();
		}

		ClientExtState->LoadConfigs();
	}

	if (extensionsEnabled_ && !Libraries.CriticalInitializationFailed()) {
		//networkManager_.ExtendNetworkingClient();
		DEBUG("ScriptExtender::LoadExtensionStateClient(): Re-initializing module state.");
		ClientExtState->LuaReset(true);
	}

	ClientExtensionLoaded = true;
}


void SavegameSerializer::SavegameVisit(ObjectVisitor* visitor)
{
	if (visitor->EnterRegion(GFS.strScriptExtenderSave)) {
		uint32_t version = SavegameVersion;
		visitor->VisitUInt32(GFS.strExtenderVersion, version, 0);
		if (visitor->IsReading()) {
			if (version > SavegameVersion) {
				ERR("Savegame version too new! Extender version %d, savegame version %d; savegame data will not be loaded!", SavegameVersion, version);
				std::wstringstream ss;
				ss << "Could not load Script Extender save data - savegame is newer than the currently installed extender!<br>";
				ss << "Extender version v" << SavegameVersion << ", savegame version v" << version;
				gExtender->GetLibraryManager().ShowStartupError(ss.str().c_str(), true, false);
			}
			else {
				Serialize(visitor, version);
			}
		}
		else {
			Serialize(visitor, SavegameVersion);
		}

		visitor->ExitRegion(GFS.strScriptExtenderSave);
	}
}


void SavegameSerializer::Serialize(ObjectVisitor* visitor, uint32_t version)
{
	SerializePersistentVariables(visitor, version);
	SerializeStatObjects(visitor, version);
}


void SavegameSerializer::SerializePersistentVariables(ObjectVisitor* visitor, uint32_t version)
{
	STDString nullStr;
	if (visitor->EnterNode(GFS.strLuaVariables, GFS.strEmpty)) {
		auto const& configs = gExtender->GetServerExtensionState().GetConfigs();

		if (visitor->IsReading()) {
			std::unordered_map<FixedString, STDString> variables;
			uint32_t numMods{ 0 };
			visitor->VisitCount(GFS.strMod, &numMods);

			for (uint32_t i = 0; i < numMods; i++) {
				if (visitor->EnterNode(GFS.strMod, GFS.strModId)) {
					FixedString modId;
					visitor->VisitFixedString(GFS.strModId, modId, GFS.strEmpty);
					STDString modVars;
					visitor->VisitSTDString(GFS.strLuaVariables, modVars, nullStr);

					variables.insert(std::make_pair(modId, modVars));
					visitor->ExitNode(GFS.strMod);
				}
			}

			RestorePersistentVariables(variables);
		} else {
			auto& state = gExtender->GetServerExtensionState();
			auto mods = state.GetPersistentVarMods();

			for (auto const& modId : mods) {
				auto vars = state.GetModPersistentVars(modId);
				if (vars) {
					if (visitor->EnterNode(GFS.strMod, GFS.strModId)) {
						FixedString modIdTemp = modId;
						visitor->VisitFixedString(GFS.strModId, modIdTemp, GFS.strEmpty);
						visitor->VisitSTDString(GFS.strLuaVariables, *vars, nullStr);
						visitor->ExitNode(GFS.strMod);
					}
				}
			}
		}

		visitor->ExitNode(GFS.strLuaVariables);
	}
}

void SavegameSerializer::RestorePersistentVariables(std::unordered_map<FixedString, STDString> const& variables)
{
	auto& state = gExtender->GetServerExtensionState();
	for (auto const& var : variables) {
		state.RestoreModPersistentVars(var.first, var.second);
	}
}


void SavegameSerializer::SerializeStatObjects(ObjectVisitor* visitor, uint32_t version)
{
	STDString nullStr;
	if (visitor->EnterNode(GFS.strDynamicStats, GFS.strEmpty)) {
		if (visitor->IsReading()) {
			std::unordered_map<FixedString, STDString> variables;
			uint32_t numObjects{ 0 };
			visitor->VisitCount(GFS.strStatObject, &numObjects);

			for (uint32_t i = 0; i < numObjects; i++) {
				if (visitor->EnterNode(GFS.strStatObject, GFS.strStatId)) {
					FixedString statId, statType;
					ScratchBuffer blob;
					visitor->VisitFixedString(GFS.strStatId, statId, GFS.strEmpty);
					visitor->VisitFixedString(GFS.strStatType, statType, GFS.strEmpty);
					visitor->VisitBuffer(GFS.strBlob, blob);
					RestoreStatObject(statId, statType, blob);
					visitor->ExitNode(GFS.strStatObject);
				}
			}
		} else {
			auto const& statIds = gExtender->GetServerExtensionState().GetPersistentStats();

			for (auto statId : statIds) {
				FixedString statType;
				ScratchBuffer blob;
				if (SerializeStatObject(statId, statType, blob)) {
					if (visitor->EnterNode(GFS.strStatObject, GFS.strStatId)) {
						visitor->VisitFixedString(GFS.strStatId, statId, GFS.strEmpty);
						visitor->VisitFixedString(GFS.strStatType, statType, GFS.strEmpty);
						visitor->VisitBuffer(GFS.strBlob, blob);
						visitor->ExitNode(GFS.strStatObject);
					}
				}
			}
		}

		visitor->ExitNode(GFS.strDynamicStats);
	}
}

void SavegameSerializer::RestoreStatObject(FixedString const& statId, FixedString const& statType, ScratchBuffer const& blob)
{
	auto stats = GetStaticSymbols().GetStats();

	auto object = stats->Objects.Find(statId);
	if (object) {
		auto modifier = stats->ModifierLists.Find(object->ModifierListIndex);
		if (modifier->Name != statType) {
			OsiError("Stat entry '" << statId << "' is a '" << statType << "' in the save, but a '" 
				<< modifier->Name << "' in the game. It will not be loaded from the savegame!");
			return;
		}
	} else {
		auto newObject = stats->CreateObject(statId, statType);
		if (!newObject) {
			OsiError("Couldn't construct stats entry '" << statId << "' of type '" << statType 
				<< "'! It will not be loaded from the savegame!");
			return;
		}
		object = *newObject;
	}
	/*
#if defined(NDEBUG)
	MsgS2CSyncStat msg;
#else
	// Workaround for different debug/release CRT runtimes between protobuf and the extender in debug mode
	MsgS2CSyncStat& msg = *GameAlloc<MsgS2CSyncStat>();
#endif
	if (!msg.ParseFromArray(blob.Buffer, blob.Size)) {
		OsiError("Unable to parse protobuf payload for stat '" << statId << "'! It will not be loaded from the savegame!");
		return;
	}

	object->FromProtobuf(msg);
	stats->SyncWithPrototypeManager(object);*/
	object->BroadcastSyncMessage(true);
	gExtender->GetServerExtensionState().MarkDynamicStat(statId);
	gExtender->GetServerExtensionState().MarkPersistentStat(statId);
}

bool SavegameSerializer::SerializeStatObject(FixedString const& statId, FixedString& statType, ScratchBuffer& blob)
{
	auto stats = GetStaticSymbols().GetStats();

	auto object = stats->Objects.Find(statId);
	if (!object) {
		OsiError("Stat entry '" << statId << "' is marked as modified but cannot be found! It will not be written to the savegame!");
		return false;
	}

	auto modifier = stats->ModifierLists.Find(object->ModifierListIndex);
	statType = modifier->Name;

	/*MsgS2CSyncStat msg;
	object->ToProtobuf(&msg);
	uint32_t size = (uint32_t)msg.ByteSizeLong();
	blob.Size = size;
	blob.Buffer = GameAllocRaw(size);
	msg.SerializeToArray(blob.Buffer, size);*/
	return true;
}


void StatLoadOrderHelper::OnLoadStarted()
{
	loadingStats_ = true;
	statLastTxtMod_ = FixedString{};
	statsEntryToModMap_.clear();
	UpdateModDirectoryMap();
}

void StatLoadOrderHelper::OnLoadFinished()
{
	OnStatFileOpened();
	loadingStats_ = false;
}

void StatLoadOrderHelper::UpdateModDirectoryMap()
{
	std::unique_lock _(modMapMutex_);
	modDirectoryToModMap_.clear();

	/*auto modManager = GetModManagerClient();
	if (modManager) {
		for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
			auto directory = ToUTF8(mod.Info.Directory);
			modDirectoryToModMap_.insert(std::make_pair(directory, mod.Info.ModuleUUID));
		}
	}*/
}

void StatLoadOrderHelper::OnStatFileOpened()
{
	/*auto stats = GetStaticSymbols().GetStats();
	auto const& bufs = stats->PreParsedDataBufferMap;
	bufs.Iterate([this, stats](auto const& key, auto const& preParseBufIdx) {
		auto preParseBuf = stats->PreParsedDataBuffers[(uint32_t)preParseBufIdx];
		auto entry = statsEntryToModMap_.find(key);
		if (entry == statsEntryToModMap_.end()) {
			StatsEntryModMapping mapping;
			mapping.Mod = statLastTxtMod_;
			mapping.PreParseBuf = preParseBuf;
			statsEntryToModMap_.insert(std::make_pair(key, mapping));
		} else if (entry->second.PreParseBuf != preParseBuf) {
			entry->second.Mod = statLastTxtMod_;
			entry->second.PreParseBuf = preParseBuf;
		}
	});*/
}

static std::regex sStatPathRegex(".*/Public/(.*)/Stats/Generated/.*.txt$");

void StatLoadOrderHelper::OnStatFileOpened(Path const& path)
{
	if (!loadingStats_) return;

	std::cmatch match;
	if (std::regex_match(path.Name.c_str(), match, sStatPathRegex)) {
		std::unique_lock lock(modMapMutex_);

		auto modIt = modDirectoryToModMap_.find(match[1].str().c_str());
		if (modIt != modDirectoryToModMap_.end()) {
			statLastTxtMod_ = modIt->second;
			OnStatFileOpened();
		} else {
			WARN("Unable to resolve mod while loading stats .txt: %s", path.Name.c_str());
		}
	}
}

FixedString StatLoadOrderHelper::GetStatsEntryMod(FixedString statId) const
{
	auto entryIt = statsEntryToModMap_.find(statId);
	if (entryIt != statsEntryToModMap_.end()) {
		return entryIt->second.Mod;
	} else {
		return {};
	}
}

std::vector<CRPGStats_Object*> StatLoadOrderHelper::GetStatsLoadedBefore(FixedString modId) const
{
	std::unordered_set<FixedString> modsLoadedBefore;
	auto state = gExtender->GetCurrentExtensionState();
	if (!state) return {};

	bool modIdFound{ false };
	for (auto const& mod : state->GetModManager()->BaseModule.LoadOrderedModules) {
		modsLoadedBefore.insert(mod.Info.ModuleUUIDString);
		if (mod.Info.ModuleUUIDString == modId) {
			modIdFound = true;
			break;
		}
	}

	if (!modIdFound) {
		OsiError("Couldn't fetch stat entry list - mod " << modId << " is not loaded.");
		return {};
	}

	std::vector<CRPGStats_Object*> statsLoadedBefore;
	auto stats = GetStaticSymbols().GetStats();
	for (auto const& object : stats->Objects.Primitives) {
		auto statEntryMod = GetStatsEntryMod(object->Name);
		if (statEntryMod && modsLoadedBefore.find(statEntryMod) != modsLoadedBefore.end()) {
			statsLoadedBefore.push_back(object);
		}
	}

	return statsLoadedBefore;
}

__declspec(thread) unsigned ModuleHasher::hashDepth_{ 0 };

void ModuleHasher::PostStartup()
{
	using namespace std::placeholders;
	/*gExtender->GetLibraryManager().Module__Hash.SetWrapper(
		std::bind(&ModuleHasher::OnModuleHash, this, _1, _2)
	);*/
}

void ModuleHasher::ClearCaches()
{
	std::lock_guard _(mutex_);
	hashCache_.clear();
}

bool ModuleHasher::FetchHashFromCache(Module& mod)
{
	auto it = hashCache_.find(mod.Info.ModuleUUIDString);
	if (it != hashCache_.end()) {
		mod.Info.Hash = it->second;
		mod.HasValidHash = true;
		UpdateDependencyHashes(mod);
		return true;
	} else {
		return false;
	}
}

void ModuleHasher::UpdateDependencyHashes(Module& mod)
{
	/*for (auto& dependency : mod.DependentModules) {
		if (!FetchHashFromCache(dependency)) {
			GetStaticSymbols().Module__Hash(&dependency);
		}
	}

	for (auto& addon : mod.AddonModules) {
		if (!FetchHashFromCache(addon)) {
			GetStaticSymbols().Module__Hash(&addon);
		}
	}*/
}

bool ModuleHasher::OnModuleHash(Module::HashProc* next, Module* self)
{
	std::lock_guard _(mutex_);
	
	if (FetchHashFromCache(*self)) {
		return true;
	}

	if (!hashStack_.empty()) {
		auto mod = *hashStack_.rbegin();
		if (!mod->Info.Hash.empty()) {
			hashCache_.insert(std::make_pair(mod->Info.ModuleUUIDString, mod->Info.Hash));
		}
	}

	hashStack_.push_back(self);

	hashDepth_++;
	auto ok = next(self);
	hashDepth_--;
	if (!self->Info.Hash.empty()) {
		hashCache_.insert(std::make_pair(self->Info.ModuleUUIDString, self->Info.Hash));
	}

	hashStack_.pop_back();
	return ok;
}

}
