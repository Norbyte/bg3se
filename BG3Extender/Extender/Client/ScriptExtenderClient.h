#pragma once

#include <Extender/Shared/ScriptExtenderBase.h>
#include <Extender/Shared/ExtenderConfig.h>
#include <Extender/Client/ExtensionStateClient.h>
#include <Extender/Client/ClientNetworking.h>
#include <Extender/Client/SDLManager.h>
#include <GameDefinitions/Symbols.h>
#include <GameDefinitions/EntitySystemHelpers.h>
#include <CoreLib/Wrappers.h>

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <ppl.h>

BEGIN_NS(ecl)

class ScriptExtender : public ThreadedExtenderState
{
public:
    ScriptExtender(ExtenderConfig& config);

    void Initialize();
    void PostStartup();
    void Shutdown();

    inline bool HasExtensionState() const
    {
        return (bool)extensionState_;
    }

    inline ExtensionState & GetExtensionState() const
    {
        assert(extensionState_);
        return *extensionState_;
    }

    inline ecs::ClientEntitySystemHelpers& GetEntityHelpers()
    {
        return entityHelpers_;
    }

    inline NetworkManager& GetNetworkManager()
    {
        return network_;
    }

    inline SDLManager& GetSDL()
    {
        return sdl_;
    }

    bool IsInClientThread() const;
    void ResetLuaState();
    void ResetExtensionState();
    void LoadExtensionState(ExtensionStateContext ctx);

    void UpdateServerProgress(STDString const& status);
    void UpdateClientProgress(STDString const& status);
    void OnGameStateChanged(GameState fromState, GameState toState);

    // HACK - we need to expose this so it can be added to the CrashReporter whitelist
    enum class GameStateWorkerStartTag {};
    enum class GameStateMachcineUpdateTag {};
    WrappableFunction<GameStateWorkerStartTag, void(void*)> gameStateWorkerStart_;
    WrappableFunction<GameStateMachcineUpdateTag, void(void*, GameTime*)> gameStateMachineUpdate_;

private:
    ExtenderConfig& config_;
    std::unique_ptr<ExtensionState> extensionState_;
    bool extensionLoaded_{ false };
    bool postStartupDone_{ false };
    ecs::ClientEntitySystemHelpers entityHelpers_;
    STDString serverStatus_;
    STDString clientStatus_;
    NetworkManager network_;
    SDLManager sdl_;

    void OnBaseModuleLoaded(void * self);
    void GameStateWorkerWrapper(void (*wrapped)(void*), void* self);
    void OnUpdate(void* self, GameTime* time);
    void OnIncLocalProgress(void* self, int progress, char const* state);
    void ShowLoadingProgress();
    void ShowVersionNumber();
};

END_NS()
