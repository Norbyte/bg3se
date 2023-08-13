#include <stdafx.h>
#include <Extender/Server/ScriptExtenderServer.h>
#include <Extender/ScriptExtender.h>

#define STATIC_HOOK(name) decltype(bg3se::esv::ScriptExtender::name) * decltype(bg3se::esv::ScriptExtender::name)::gHook;
STATIC_HOOK(gameStateWorkerStart_)
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
	auto& lib = GetStaticSymbols();

	if (!gExtender->GetLibraryManager().CriticalInitializationFailed()) {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		if (lib.esv__GameStateThreaded__GameStateWorker__DoWork != nullptr) {
			gameStateWorkerStart_.Wrap(lib.esv__GameStateThreaded__GameStateWorker__DoWork);
		}

		if (lib.esv__GameStateMachine__Update != nullptr) {
			gameStateMachineUpdate_.Wrap(lib.esv__GameStateMachine__Update);
		}

		DetourTransactionCommit();

		gameStateWorkerStart_.SetWrapper(&ScriptExtender::GameStateWorkerWrapper, this);
		gameStateMachineUpdate_.SetPostHook(&ScriptExtender::OnUpdate, this);
	}
}

void ScriptExtender::Shutdown()
{
	ResetExtensionState();
	osiris_.Shutdown();
}

void ScriptExtender::PostStartup()
{
	if (postStartupDone_) return;

	// We need to initialize the function library here, as GlobalAllocator isn't available in Init().
	if (!gExtender->GetLibraryManager().CriticalInitializationFailed()) {
		osiris_.Initialize();
	}

	entityHelpers_.Setup();

	postStartupDone_ = true;
}

bool IsLoadingState(GameState state)
{
	return state == GameState::Init
		|| state == GameState::LoadLevel
		|| state == GameState::LoadModule
		|| state == GameState::LoadSession
		|| state == GameState::UnloadLevel
		|| state == GameState::UnloadModule
		|| state == GameState::UnloadSession
		|| state == GameState::Sync
		|| state == GameState::BuildStory
		|| state == GameState::ReloadStory;
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
		LoadExtensionState(ExtensionStateContext::Game);
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
		LoadExtensionState(ExtensionStateContext::Game);
		//networkManager_.ExtendNetworkingServer();
		if (extensionState_) {
			extensionState_->OnGameSessionLoading();
		}
		break;

	case GameState::LoadLevel:
		if (extensionState_ && extensionState_->GetLua()) {
			extensionState_->GetLua()->OnLevelLoading();
		}
		break;
	}

	if (gExtender->WasInitialized()) {
		if (IsLoadingState(toState)) {
			gExtender->GetClient().UpdateServerProgress(EnumInfo<GameState>::Find(toState).GetString());
		}
		else {
			gExtender->GetClient().UpdateServerProgress("");
		}
	}

	if (extensionState_) {
		LuaServerPin lua(*extensionState_);
		if (lua) {
			lua->OnGameStateChanged(fromState, toState);
		}
	}
}

void ScriptExtender::GameStateWorkerWrapper(void (* wrapped)(void *), void* self)
{
	AddThread(GetCurrentThreadId());
	wrapped(self);
	RemoveThread(GetCurrentThreadId());
}

void ScriptExtender::OnUpdate(void* self, GameTime* time)
{
	RunPendingTasks();
	if (extensionState_) {
		extensionState_->OnUpdate(*time);
	}
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

bool ScriptExtender::RequestResetClientLuaState()
{
	// FIXME - networking not supported yet
	return false;
}

void ScriptExtender::ResetExtensionState()
{
	extensionState_ = std::make_unique<ExtensionState>();
	extensionState_->Reset();
	extensionLoaded_ = false;
}

void ScriptExtender::LoadExtensionState(ExtensionStateContext ctx)
{
	if (extensionLoaded_ && (!extensionState_ || ctx == extensionState_->Context())) {
		return;
	}

	PostStartup();

	if (!extensionState_) {
		ResetExtensionState();
	}

	extensionState_->LoadConfigs();

	if (!gExtender->GetLibraryManager().CriticalInitializationFailed()) {
		OsiMsg("Initializing server with target context " << ContextToString(ctx));
		gExtender->GetLibraryManager().ApplyCodePatches();
		//networkManager_.ExtendNetworkingServer();
		osiris_.GetCustomFunctionManager().ClearDynamicEntries();
		extensionState_->LuaReset(ctx, true);
	}

	extensionLoaded_ = true;
}

END_NS()
