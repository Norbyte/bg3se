#include <stdafx.h>
#include <Extender/Server/ScriptExtenderServer.h>
#include <Extender/ScriptExtender.h>

#define STATIC_HOOK(name) decltype(bg3se::esv::ScriptExtender::name) * decltype(bg3se::esv::ScriptExtender::name)::gHook;
STATIC_HOOK(gameStateWorkerStart_)
STATIC_HOOK(gameStateChangedEvent_)
STATIC_HOOK(gameStateMachineUpdate_)


BEGIN_NS(esv)

char const * GameStateNames[] =
{
	"Unknown",
	"Uninitialized",
	"Init",
	"Idle",
	"Exit",
	"LoadLevel",
	"LoadModule",
	"LoadSession",
	"UnloadLevel",
	"UnloadModule",
	"UnloadSession",
	"Sync",
	"Paused",
	"Running",
	"Save",
	"Disconnect",
	"BuildStory",
	"ReloadStory"
};

ScriptExtender::ScriptExtender(ExtenderConfig& config)
	: osiris_(config)
{
}

void ScriptExtender::Initialize()
{
	ResetExtensionState();

	auto& lib = GetStaticSymbols();

	if (!gExtender->GetLibraryManager().CriticalInitializationFailed()) {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		if (lib.esv__GameStateEventManager__ExecuteGameStateChangedEvent != nullptr) {
			gameStateChangedEvent_.Wrap(lib.esv__GameStateEventManager__ExecuteGameStateChangedEvent);
		}

		if (lib.esv__GameStateThreaded__GameStateWorker__DoWork != nullptr) {
			gameStateWorkerStart_.Wrap(lib.esv__GameStateThreaded__GameStateWorker__DoWork);
		}

		if (lib.esv__GameStateMachine__Update != nullptr) {
			gameStateMachineUpdate_.Wrap(lib.esv__GameStateMachine__Update);
		}

		DetourTransactionCommit();

		using namespace std::placeholders;
		gameStateChangedEvent_.SetPostHook(std::bind(&ScriptExtender::OnGameStateChanged, this, _1, _2, _3));
		gameStateWorkerStart_.AddPreHook(std::bind(&ScriptExtender::OnGameStateWorkerStart, this, _1));
		gameStateWorkerStart_.AddPostHook(std::bind(&ScriptExtender::OnGameStateWorkerExit, this, _1));
		gameStateMachineUpdate_.AddPostHook(std::bind(&ScriptExtender::OnUpdate, this, _1, _2));
	}
}

void ScriptExtender::Shutdown()
{
	ResetExtensionState();
	osiris_.Shutdown();
}

void ScriptExtender::PostStartup()
{
	// We need to initialize the function library here, as GlobalAllocator isn't available in Init().
	if (!gExtender->GetLibraryManager().CriticalInitializationFailed()) {
		osiris_.Initialize();
	}

	entityHelpers_.Setup();
}

void ScriptExtender::OnGameStateChanged(void * self, GameState fromState, GameState toState)
{
#if defined(DEBUG_SERVER_CLIENT)
		DEBUG("esv::ScriptExtender::OnGameStateChanged(): %s -> %s", 
			ServerGameStateNames[(unsigned)fromState], ServerGameStateNames[(unsigned)toState]);
#endif

	if (fromState != GameState::Unknown) {
		AddThread(GetCurrentThreadId());
	}

	switch (fromState) {
	case GameState::LoadModule:
		INFO("esv::ScriptExtender::OnGameStateChanged(): Loaded module");
		LoadExtensionState();
		osiris_.InitRuntimeLogging();
		break;

	case GameState::LoadSession:
		if (extensionState_) {
			extensionState_->OnGameSessionLoaded();
		}
		break;
	}

	switch (toState) {
	// Gift Bag selection in the main menu reloads the module without reconnecting
	// We only need to reset the extender enabled peer list on a disconnect.
	case GameState::Disconnect:
		//networkManager_.ServerReset();
		break;

	case GameState::UnloadSession:
		INFO("esv::ScriptExtender::OnGameStateChanged(): Unloading session");
		ResetExtensionState();
		break;

	case GameState::LoadModule:
		//networkManager_.ExtendNetworkingServer();
		break;

	case GameState::LoadSession:
		INFO("esv::ScriptExtender::OnGameStateChanged(): Loading game session");
		LoadExtensionState();
		//networkManager_.ExtendNetworkingServer();
		if (extensionState_) {
			extensionState_->OnGameSessionLoading();
		}
		break;
	}

	LuaServerPin lua(ExtensionState::Get());
	if (lua) {
		lua->OnGameStateChanged(fromState, toState);
	}
}

void ScriptExtender::OnGameStateWorkerStart(void * self)
{
	AddThread(GetCurrentThreadId());
}

void ScriptExtender::OnGameStateWorkerExit(void* self)
{
	RemoveThread(GetCurrentThreadId());
}

void ScriptExtender::OnUpdate(void* self, GameTime* time)
{
	RunPendingTasks();
}

bool ScriptExtender::IsInServerThread() const
{
	return IsInThread();
}

void ScriptExtender::ResetLuaState()
{
	if (extensionState_ && extensionState_->GetLua()) {
		bool serverThread = IsInServerThread();

		auto ext = extensionState_.get();
		ext->AddPostResetCallback([ext]() {
			ext->OnModuleResume();
			auto state = GetStaticSymbols().GetServerState();
			if (state && (state == GameState::Paused || state == GameState::Running)) {
				ext->OnGameSessionLoading();
				ext->OnGameSessionLoaded();
				ext->StoryLoaded();
				ext->OnResetCompleted();
			}
		});
		ext->LuaReset(true);
	}
}

void ScriptExtender::ResetExtensionState()
{
	extensionState_ = std::make_unique<ExtensionState>();
	extensionState_->Reset();
	extensionLoaded_ = false;
}

extern std::unordered_map<int32_t*, char*> maps;

void ScriptExtender::LoadExtensionState()
{
	if (extensionLoaded_) return;
	
	PostStartup();

	if (!extensionState_) {
		ResetExtensionState();
	}

	extensionState_->LoadConfigs();

	if (!gExtender->GetLibraryManager().CriticalInitializationFailed()) {
		//networkManager_.ExtendNetworkingServer();
		DEBUG("esv::ScriptExtender::LoadExtensionState(): Re-initializing module state.");
		osiris_.OnBaseModuleLoadedServer();
		extensionState_->LuaReset(true);
	}

	extensionLoaded_ = true;
}

END_NS()
