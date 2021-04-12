#pragma once

#include <ExtenderConfig.h>
#include <GameDefinitions/Osiris.h>
#include <GameDefinitions/GuidResources.h>
#include <ExtensionStateClient.h>
#include <ExtensionStateServer.h>
#if !defined(OSI_NO_DEBUGGER)
#include "Lua/LuaDebugger.h"
#include "Lua/LuaDebugMessages.h"
#endif
#include <GameHooks/OsirisWrappers.h>
#include <GameHooks/DataLibraries.h>
#include <GameHooks/EngineHooks.h>
//#include "NetProtocol.h"
#include <GameDefinitions/Symbols.h>
#include <GlobalFixedStrings.h>
#include <Osiris/OsirisExtender.h>
#include <EntitySystemHelpers.h>
//#include <Hit.h>

#include <thread>
#include <mutex>
#include <shared_mutex>

namespace bg3se {

class SavegameSerializer
{
public:
	void SavegameVisit(ObjectVisitor* visitor);

private:
	void Serialize(ObjectVisitor* visitor, uint32_t version);
	void SerializePersistentVariables(ObjectVisitor* visitor, uint32_t version);
	void RestorePersistentVariables(std::unordered_map<FixedString, STDString> const&);
	void SerializeStatObjects(ObjectVisitor* visitor, uint32_t version);
	void RestoreStatObject(FixedString const& statId, FixedString const& statType, ScratchBuffer const& blob);
	bool SerializeStatObject(FixedString const& statId, FixedString& statType, ScratchBuffer& blob);
};

class StatLoadOrderHelper
{
public:
	void OnLoadStarted();
	void OnLoadFinished();
	void OnStatFileOpened();
	void OnStatFileOpened(Path const& path);
	void UpdateModDirectoryMap();

	FixedString GetStatsEntryMod(FixedString statId) const;
	std::vector<CRPGStats_Object*> GetStatsLoadedBefore(FixedString modId) const;

private:
	struct StatsEntryModMapping
	{
		FixedString Mod;
		void* PreParseBuf;
	};

	std::shared_mutex modMapMutex_;
	std::unordered_map<STDString, FixedString> modDirectoryToModMap_;
	std::unordered_map<FixedString, StatsEntryModMapping> statsEntryToModMap_;
	FixedString statLastTxtMod_;
	bool loadingStats_{ false };
};

class ModuleHasher
{
public:
	void PostStartup();
	void ClearCaches();

	bool isHashing() const
	{
		return hashDepth_ > 0;
	}

private:
	std::vector<Module*> hashStack_;
	std::unordered_map<FixedString, STDString> hashCache_;
	std::recursive_mutex mutex_;
	static __declspec(thread) unsigned hashDepth_;

	bool FetchHashFromCache(Module& mod);
	void UpdateDependencyHashes(Module& mod);
	bool OnModuleHash(Module::HashProc* next, Module* self);
};

class ScriptExtender
{
public:
	ScriptExtender();

	void Initialize();
	void Shutdown();

	inline ExtenderConfig & GetConfig()
	{
		return config_;
	}

	void LogLuaError(std::string_view msg);
	void LogOsirisError(std::string_view msg);
	void LogOsirisWarning(std::string_view msg);
	void LogOsirisMsg(std::string_view msg);

	inline LibraryManager const & GetLibraryManager() const
	{
		return Libraries;
	}

	inline LibraryManager & GetLibraryManager()
	{
		return Libraries;
	}

#if !defined(OSI_NO_DEBUGGER)
	inline lua::dbg::Debugger* GetLuaDebugger()
	{
		return luaDebugger_.get();
	}

	inline lua::dbg::DebugMessageHandler* GetLuaDebugMessageHandler()
	{
		return luaDebugMsgHandler_.get();
	}
#endif

	ExtensionStateBase* GetCurrentExtensionState();

	inline bool HasServerExtensionState() const
	{
		return (bool)ServerExtState;
	}

	inline esv::ExtensionState & GetServerExtensionState()
	{
		return *ServerExtState;
	}

	inline bool HasClientExtensionState() const
	{
		return (bool)ClientExtState;
	}

	inline ecl::ExtensionState & GetClientExtensionState()
	{
		return *ClientExtState;
	}

	bool HasFeatureFlag(char const *) const;

	/*inline NetworkManager & GetNetworkManager()
	{
		return networkManager_;
	}*/

	inline StatLoadOrderHelper& GetStatLoadOrderHelper()
	{
		return statLoadOrderHelper_;
	}

	inline ServerEntitySystemHelpers& GetServerEntityHelpers()
	{
		return serverEntityHelpers_;
	}

	inline ClientEntitySystemHelpers& GetClientEntityHelpers()
	{
		return clientEntityHelpers_;
	}

	inline OsirisExtender& GetOsiris()
	{
		return osiris_;
	}

	inline EngineHooks& GetEngineHooks()
	{
		return Hooks;
	}

	void ClearPathOverrides();
	void AddPathOverride(STDString const & path, STDString const & overriddenPath);

	bool IsInServerThread() const;
	bool IsInClientThread() const;
	void AttachConsoleThread(bool server);

	void ResetLuaState(bool resetServer, bool resetClient);

	// HACK - we need to expose this so it can be added to the CrashReporter whitelist
	enum class ClientGameStateWorkerStartTag {};
	enum class ServerGameStateWorkerStartTag {};
	HookableFunction<ClientGameStateWorkerStartTag, void(void*)> clientGameStateWorkerStart_;
	HookableFunction<ServerGameStateWorkerStartTag, void(void*)> serverGameStateWorkerStart_;

private:
	OsirisExtender osiris_;
	std::unique_ptr<esv::ExtensionState> ServerExtState;
	std::unique_ptr<ecl::ExtensionState> ClientExtState;
	LibraryManager Libraries;
	EngineHooks Hooks;
	bool LibrariesPostInitialized{ false };
	bool ServerExtensionLoaded{ false };
	bool ClientExtensionLoaded{ false };
	std::unordered_set<DWORD> ClientThreadIds;
	std::unordered_set<DWORD> ServerThreadIds;
	std::recursive_mutex globalStateLock_;
	//NetworkManager networkManager_;
	std::shared_mutex pathOverrideMutex_;
	std::unordered_map<STDString, STDString> pathOverrides_;
	SavegameSerializer savegameSerializer_;
	StatLoadOrderHelper statLoadOrderHelper_;
	//esv::HitProxy hitProxy_;
	ModuleHasher hasher_;
	ServerEntitySystemHelpers serverEntityHelpers_;
	ClientEntitySystemHelpers clientEntityHelpers_;

	ExtenderConfig config_;
	bool extensionsEnabled_{ false };
	bool postStartupDone_{ false };

	enum class ClientGameStateChangedEventTag {};
	enum class ServerGameStateChangedEventTag {};
	PostHookableFunction<ClientGameStateChangedEventTag, void(void*, ecl::GameState, ecl::GameState)> clientGameStateChangedEvent_;
	PostHookableFunction<ServerGameStateChangedEventTag, void(void*, esv::GameState, esv::GameState)> serverGameStateChangedEvent_;

#if !defined(OSI_NO_DEBUGGER)
	std::thread* luaDebuggerThread_{ nullptr };
	std::unique_ptr<LuaDebugInterface> luaDebugInterface_;
	std::unique_ptr<lua::dbg::DebugMessageHandler> luaDebugMsgHandler_;
	std::unique_ptr<lua::dbg::Debugger> luaDebugger_;
#endif

	void PostStartup();
	void OnBaseModuleLoaded(void * self);
	/*void OnModuleLoadStarted(TranslatedStringRepository * self);
	void OnStatsLoadStarted(RPGStats* mgr);
	void OnStatsLoadFinished(RPGStats* mgr);*/
	void OnClientGameStateChanged(void * self, ecl::GameState fromState, ecl::GameState toState);
	void OnServerGameStateChanged(void * self, esv::GameState fromState, esv::GameState toState);
	void OnClientGameStateWorkerStart(void * self);
	void OnServerGameStateWorkerStart(void * self);
	void OnClientGameStateWorkerExit(void* self);
	void OnServerGameStateWorkerExit(void* self);
	void OnSkillPrototypeManagerInit(void * self);
	FileReader * OnFileReaderCreate(FileReader::CtorProc* next, FileReader * self, Path const& path, unsigned int type, unsigned int unknown);
	void OnSavegameVisit(void* osirisHelpers, ObjectVisitor* visitor);
	void ResetExtensionStateServer();
	void LoadExtensionStateServer();
	void ResetExtensionStateClient();
	void LoadExtensionStateClient();

	void AddClientThread(DWORD threadId);
	void AddServerThread(DWORD threadId);
	void RemoveClientThread(DWORD threadId);
	void RemoveServerThread(DWORD threadId);
};

extern std::unique_ptr<ScriptExtender> gExtender;

}
