#pragma once

#include <Extender/Shared/ScriptExtenderBase.h>
#include <Extender/Shared/ExtenderConfig.h>
#include <GameDefinitions/Osiris.h>
#include <GameDefinitions/GuidResources.h>
#include <Extender/Server/ExtensionStateServer.h>
#include <GameHooks/OsirisWrappers.h>
#include <GameDefinitions/Symbols.h>
#include <Osiris/OsirisExtender.h>
#include <GameDefinitions/EntitySystemHelpers.h>
#include <CoreLib/Wrappers.h>
#include <Extender/Shared/SavegameSerializer.h>
#include <Extender/Server/ServerNetworking.h>

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <ppl.h>

BEGIN_NS(esv)

class ScriptExtender : public ThreadedExtenderState
{
public:
    ScriptExtender(ExtenderConfig& config);

    void Initialize();
    void PostStartup();
    void Shutdown();
    void OnGameStateChanged(GameState fromState, GameState toState);

    inline bool HasExtensionState() const
    {
        return (bool)extensionState_;
    }

    inline ExtensionState & GetExtensionState() const
    {
        se_assert(extensionState_);
        return *extensionState_;
    }

    inline ecs::ServerEntitySystemHelpers& GetEntityHelpers()
    {
        return entityHelpers_;
    }

    inline OsirisExtender& Osiris()
    {
        return osiris_;
    }

    inline NetworkManager& GetNetworkManager()
    {
        return network_;
    }

    bool IsInServerThread() const;
    void ResetLuaState();
    bool RequestResetClientLuaState();
    void ResetExtensionState();
    void LoadExtensionState(ExtensionStateContext ctx);
    void OnSavegameVisit(OsirisVariableHelper* helper, SavegameVisitor* visitor);

    // HACK - we need to expose this so it can be added to the CrashReporter whitelist
    enum class GameStateWorkerStartTag {};
    enum class GameStateMachineUpdateTag {};
    WrappableFunction<GameStateWorkerStartTag, void(void*)> gameStateWorkerStart_;
    WrappableFunction<GameStateMachineUpdateTag, void(void*, GameTime*)> gameStateMachineUpdate_;

private:
    OsirisExtender osiris_;
    std::unique_ptr<ExtensionState> extensionState_;
    bool extensionLoaded_{ false };
    bool postStartupDone_{ false };
    ecs::ServerEntitySystemHelpers entityHelpers_;
    SavegameSerializer savegameSerializer_;
    NetworkManager network_;

    void OnBaseModuleLoaded(void * self);
    void GameStateWorkerWrapper(void (* wrapped)(void*), void * self);
    void PreUpdate(void* self, GameTime* time);
    void PostUpdate(void* self, GameTime* time);
};

END_NS()
