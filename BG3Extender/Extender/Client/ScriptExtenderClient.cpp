#include <stdafx.h>
#include <Extender/Client/ScriptExtenderClient.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Version.h>

#define STATIC_HOOK(name) decltype(bg3se::ecl::ScriptExtender::name) * decltype(bg3se::ecl::ScriptExtender::name)::gHook;
STATIC_HOOK(gameStateWorkerStart_)
STATIC_HOOK(gameStateMachineUpdate_)

#include <Extender/Shared/ThreadedExtenderState.inl>
#include <Extender/Shared/ModuleHasher.inl>

BEGIN_SE()

void InitCrashReporting();

extern char const* BuildDate;

END_SE()

BEGIN_NS(ecl)

char const * GameStateNames[] =
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
	"ModReceiving",
	"Lobby",
	"BuildStory",
	"GeneratePsoCache",
	"LoadPsoCache",
	"AnalyticsSessionEnd"
};

ScriptExtender::ScriptExtender(ExtenderConfig& config)
	: config_(config)
{
}

void ScriptExtender::Initialize()
{
	// Wrap state change functions even if extension startup failed, otherwise
	// we won't be able to show any startup errors

	auto& lib = GetStaticSymbols();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	if (lib.ecl__GameStateThreaded__GameStateWorker__DoWork != nullptr) {
		gameStateWorkerStart_.Wrap(lib.ecl__GameStateThreaded__GameStateWorker__DoWork);
	}

	if (lib.ecl__GameStateMachine__Update != nullptr) {
		gameStateMachineUpdate_.Wrap(lib.ecl__GameStateMachine__Update);
	}

	DetourTransactionCommit();

	gameStateWorkerStart_.SetWrapper(&ScriptExtender::GameStateWorkerWrapper, this);
	gameStateMachineUpdate_.SetPostHook(&ScriptExtender::OnUpdate, this);
}

void ScriptExtender::Shutdown()
{
	DEBUG("ecl::ScriptExtender::Shutdown: Exiting");
	ResetExtensionState();
}

void ScriptExtender::PostStartup()
{
	if (postStartupDone_) return;

	entityHelpers_.Setup();
	gExtender->GetPropertyMapManager().RegisterComponents(entityHelpers_);
	postStartupDone_ = true;
}

bool IsLoadingState(GameState state)
{
	return state == GameState::Init
		|| state == GameState::InitMenu
		|| state == GameState::InitNetwork
		|| state == GameState::InitConnection
		|| state == GameState::LoadMenu
		|| state == GameState::SwapLevel
		|| state == GameState::LoadLevel
		|| state == GameState::LoadModule
		|| state == GameState::LoadSession
		|| state == GameState::UnloadLevel
		|| state == GameState::UnloadModule
		|| state == GameState::UnloadSession
		|| state == GameState::PrepareRunning
		|| state == GameState::Installation
		|| state == GameState::ModReceiving;
}

void ScriptExtender::OnGameStateChanged(GameState fromState, GameState toState)
{
	if (gExtender->GetConfig().SendCrashReports) {
		// We need to initialize the crash reporter after the game engine has started,
		// otherwise the game will overwrite the top level exception filter
		InitCrashReporting();
	}

	// Check to make sure that startup is done even if the extender was loaded when the game was already in GameState::Init
	if (toState != GameState::Unknown
		&& toState != GameState::StartLoading
		&& toState != GameState::InitMenu
		&& !gExtender->GetLibraryManager().CriticalInitializationFailed()) {
		// We need to initialize the function library here, as GlobalAllocator isn't available in Init().
		gExtender->PostStartup();
	}

	if (toState == GameState::Menu
		&& gExtender->GetLibraryManager().InitializationFailed()) {
		gExtender->PostStartup();
	}

	// FIXME - EnableModuleHashing flag not currently mapped
	/*if (toState == GameState::LoadModule && config_.DisableModValidation) {
		if (GetStaticSymbols().GetGlobalSwitches()) {
			GetStaticSymbols().GetGlobalSwitches()->EnableModuleHashing = false;
			INFO("Disabled module hashing");
		} else {
			WARN("Could not disable mod hashing - GlobalSwitches not mapped");
		}
	}*/

#if defined(DEBUG_SERVER_CLIENT)
	DEBUG("ecl::ScriptExtender::OnGameStateChanged(): %s -> %s", 
		GameStateNames[(unsigned)fromState], ClientGameStateNames[(unsigned)toState]);
#endif

	if (fromState != GameState::Unknown) {
		AddThread(GetCurrentThreadId());
	}

	if (gExtender->WasInitialized() && !gExtender->GetLibraryManager().CriticalInitializationFailed()) {
		if (IsLoadingState(toState)) {
			UpdateClientProgress(EnumInfo<GameState>::Find(toState).GetString());
		} else {
			UpdateClientProgress("");
		}
	}

	switch (fromState) {
	case GameState::LoadModule:
		INFO("ecl::ScriptExtender::OnGameStateChanged(): Loaded module");
		ShowVersionNumber();
		LoadExtensionState(ExtensionStateContext::Game);
		break;

	// Initialize client state when exiting from a game and returning to menu
	case GameState::LoadMenu:
		LoadExtensionState(ExtensionStateContext::Game);
		break;

	case GameState::LoadSession:
		if (extensionState_) {
			extensionState_->OnGameSessionLoaded();
		}
		break;

	case GameState::InitNetwork:
		network_.ExtendNetworking();
		break;
	}

	switch (toState) {
	case GameState::Init:
		ResetExtensionState();
		break;

	case GameState::InitNetwork:
	case GameState::Disconnect:
		network_.Reset();
		break;

	case GameState::UnloadModule:
		hasher_.ClearCaches();
		break;

	case GameState::UnloadSession:
		INFO("ecl::ScriptExtender::OnGameStateChanged(): Unloading session");
		ResetExtensionState();
		break;

	case GameState::Menu:
		#if defined(SE_IS_DEVELOPER_BUILD) && defined(NDEBUG)
		if (!gExtender->GetConfig().DeveloperMode) {
			gExtender->GetLibraryManager().ShowStartupError("This is an experimental version of the Script Extender meant for development use; things may frequently break here. It is recommended to switch back to the Release version unless you know what you are doing!", false, false);
		}
		#endif
		break;

	case GameState::LoadModule:
		gExtender->InitRuntimeLogging();
		break;

	case GameState::LoadSession:
		INFO("ecl::ScriptExtender::OnClientGameStateChanged(): Loading game session");
		LoadExtensionState(ExtensionStateContext::Game);
		network_.ExtendNetworking();
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

	if (extensionState_) {
		LuaClientPin lua(*extensionState_);
		if (lua) {
			lua->OnGameStateChanged(fromState, toState);
		}
	}
}

void ScriptExtender::GameStateWorkerWrapper(void (*wrapped)(void*), void* self)
{
	AddThread(GetCurrentThreadId());
	wrapped(self);
	RemoveThread(GetCurrentThreadId());
}

void ScriptExtender::OnUpdate(void* self, GameTime* time)
{
	// In case we're loaded too late to see LoadModule transition
	AddThread(GetCurrentThreadId());

	RunPendingTasks();
	if (extensionState_) {
		extensionState_->OnUpdate(*time);
		if (gExtender->GetLuaDebugger()) {
			gExtender->GetLuaDebugger()->ClientTick();
		}
	}
}

void ScriptExtender::OnIncLocalProgress(void* self, int progress, char const* state)
{
	if (strcmp(state, "EffectManager") != 0) {
		UpdateClientProgress(state);
	}
	else {
		UpdateClientProgress("");
	}
}

void ScriptExtender::UpdateServerProgress(STDString const& status)
{
	serverStatus_ = status;
	ShowLoadingProgress();
}

void ScriptExtender::UpdateClientProgress(STDString const& status)
{
	clientStatus_ = status;
	ShowLoadingProgress();
}

void ScriptExtender::ShowLoadingProgress()
{
	// FIXME - not supported for now
}

void ScriptExtender::ShowVersionNumber()
{
	RuntimeStringHandle rsh(FixedString("h5b6e4138g2cf0g4d67gb825gee416cf8c54f"));
	auto versionText = GetStaticSymbols().GetTranslatedStringRepository()->GetTranslatedString(rsh);
	if (versionText) {
		auto expandedVersion = STDString(*versionText) +
			"\r\nScript Extender v" + STDString(std::to_string(CurrentVersion)) + " loaded, built on " + BuildDate + ".";
		GetStaticSymbols().GetTranslatedStringRepository()->UpdateTranslatedString(rsh, expandedVersion);
	}
}

bool ScriptExtender::IsInClientThread() const
{
	return IsInThread();
}

void ScriptExtender::ResetLuaState()
{
	if (extensionState_ && extensionState_->GetLua()) {
		auto ext = extensionState_.get();

		ext->AddPostResetCallback([&ext]() {
			ext->OnModuleResume();
			auto state = GetStaticSymbols().GetClientState();
			if (state && (state == GameState::Paused || state == GameState::Running)) {
				ext->OnGameSessionLoading();
				ext->OnGameSessionLoaded();
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
	gExtender->ClearPathOverrides();
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
		OsiMsg("Initializing client with target context " << ContextToString(ctx));
		gExtender->GetLibraryManager().ApplyCodePatches();
		//networkManager_.ExtendNetworkingClient();
		extensionState_->LuaReset(ctx, true);
	}

	extensionLoaded_ = true;
}

END_NS()
