#include <stdafx.h>
#include <Extender/Server/ScriptExtenderServer.h>
#include <Extender/ScriptExtender.h>

#include <Extender/Shared/SavegameSerializer.inl>

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
    : ThreadedExtenderState(ContextType::Server), 
    osiris_(config)
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
        gameStateMachineUpdate_.SetPrePostHook(&ScriptExtender::PreUpdate, &ScriptExtender::PostUpdate, this);

        gExtender->GetEngineHooks().esv__OsirisVariableHelper__SavegameVisit.SetPreHook(&ScriptExtender::OnSavegameVisit, this);
    }
}

void ScriptExtender::Shutdown()
{
    ContextGuardAnyThread _(ContextType::Server);
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
    gExtender->GetPropertyMapManager().RegisterComponents(entityHelpers_);
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

void ScriptExtender::OnGameStateChanged(GameState fromState, GameState toState)
{
#if defined(DEBUG_SERVER_CLIENT)
        DEBUG("esv::ScriptExtender::OnGameStateChanged(): %s -> %s", 
            ServerGameStateNames[(unsigned)fromState], ServerGameStateNames[(unsigned)toState]);
#endif

    if (fromState != GameState::Unknown) {
        BindToThreadPersistent();
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
    case GameState::Init:
        network_.ExtendNetworking();
        break;
        
    case GameState::Disconnect:
        network_.Reset();
        break;

    case GameState::UnloadSession:
        INFO("esv::ScriptExtender::OnGameStateChanged(): Unloading session");
        ResetExtensionState();
        break;

    case GameState::LoadModule:
        network_.ExtendNetworking();
        break;

    case GameState::LoadSession:
        INFO("esv::ScriptExtender::OnGameStateChanged(): Loading game session");
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

    if (gExtender->WasInitialized() && !gExtender->GetLibraryManager().CriticalInitializationFailed()) {
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
    ContextGuard _(ContextType::Server);
    wrapped(self);
}

void ScriptExtender::PreUpdate(void* self, GameTime* time)
{
    network_.Update();
    RunPendingTasks();
    if (extensionState_) {
        extensionState_->OnUpdate(*time);
        if (gExtender->GetLuaDebugger()) {
            gExtender->GetLuaDebugger()->ServerTick();
        }
    }
}

void ScriptExtender::PostUpdate(void* self, GameTime* time)
{
    network_.Update();
    RunPendingTasks();
}

void ScriptExtender::ResetLuaState()
{
    if (extensionState_ && extensionState_->GetLua()) {
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
    auto server = GetStaticSymbols().GetEoCServer();
    if (!server || !server->GameServer) return false;

    // Reset clients via a network message if the server is running
    auto msg = network_.GetFreeMessage(ReservedUserId);
    if (msg != nullptr) {
        auto resetMsg = msg->GetMessage().mutable_s2c_reset_lua();
        resetMsg->set_bootstrap_scripts(true);
        network_.Broadcast(msg, ReservedUserId);
        return true;
    } else {
        return false;
    }
}

void ScriptExtender::ResetExtensionState()
{
    network_.OnResetExtensionState();
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
        network_.ExtendNetworking();
        osiris_.GetCustomFunctionManager().ClearDynamicEntries();
        extensionState_->LuaReset(ctx, true);
    }

    extensionLoaded_ = true;
}

void ScriptExtender::OnSavegameVisit(OsirisVariableHelper* helpers, SavegameVisitor* visitor)
{
    if (extensionState_ && visitor->LSFVisitor != nullptr) {
        savegameSerializer_.SavegameVisit(visitor->LSFVisitor);
    }
}

END_NS()
