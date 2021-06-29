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
#include <GameHooks/Wrappers.h>

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

	inline bool HasExtensionState() const
	{
		return (bool)extensionState_;
	}

	inline ExtensionState & GetExtensionState() const
	{
		return *extensionState_;
	}

	inline ServerEntitySystemHelpers& GetEntityHelpers()
	{
		return entityHelpers_;
	}

	inline OsirisExtender& Osiris()
	{
		return osiris_;
	}

	bool IsInServerThread() const;
	void ResetLuaState();
	void ResetExtensionState();
	void LoadExtensionState();

	// HACK - we need to expose this so it can be added to the CrashReporter whitelist
	enum class GameStateWorkerStartTag {};
	enum class GameStateMachcineUpdateTag {};
	HookableFunction<GameStateWorkerStartTag, void(void*)> gameStateWorkerStart_;
	HookableFunction<GameStateMachcineUpdateTag, void(void*, GameTime*)> gameStateMachineUpdate_;

private:
	OsirisExtender osiris_;
	std::unique_ptr<ExtensionState> extensionState_;
	bool extensionLoaded_{ false };
	ServerEntitySystemHelpers entityHelpers_;

	enum class GameStateChangedEventTag {};
	PostHookableFunction<GameStateChangedEventTag, void(void*, GameState, GameState)> gameStateChangedEvent_;

	void OnBaseModuleLoaded(void * self);
	/*void OnModuleLoadStarted(TranslatedStringRepository * self);
	void OnStatsLoadStarted(RPGStats* mgr);
	void OnStatsLoadFinished(RPGStats* mgr);*/
	void OnGameStateChanged(void * self, GameState fromState, GameState toState);
	void OnGameStateWorkerStart(void * self);
	void OnGameStateWorkerExit(void* self);
	void OnUpdate(void* self, GameTime* time);
};

END_NS()
