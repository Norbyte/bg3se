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

class EntitySystemHelpersBase : public Noncopyable<EntitySystemHelpersBase>
{
public:
	EntitySystemHelpersBase();

	inline std::optional<EntityWorldBase::ComponentTypeIndex> GetComponentIndex(ExtComponentType type) const
	{
		auto idx = componentIndices_[(unsigned)type];
		if (idx != UndefinedIndex) {
			return idx;
		} else {
			return {};
		}
	}

	std::optional<EntityWorldBase::HandleTypeIndex> GetHandleIndex(ExtComponentType type) const
	{
		auto idx = handleIndices_[(unsigned)type];
		if (idx != -1) {
			return idx;
		}
		else {
			return {};
		}
	}

	template <class T>
	T* RawComponentPtrToComponent(void* ptr)
	{
		if constexpr (std::is_same_v<T, esv::Character> || std::is_same_v<T, esv::Item>) {
			return reinterpret_cast<T*>((uintptr_t)ptr - 8);
		}

		return reinterpret_cast<T*>(ptr);
	}

	template <class T>
	T* GetComponent(ObjectHandle componentHandle, bool logError = true)
	{
		auto component = GetRawComponent(componentHandle, T::ComponentType, logError);
		if (component) {
			return RawComponentPtrToComponent<T>(component);
		} else {
			return nullptr;
		}
	}

	template <class T>
	T* GetComponent(char const* nameGuid, bool logError = true)
	{
		auto component = GetRawComponent(nameGuid, T::ComponentType, logError);
		if (component) {
			return RawComponentPtrToComponent<T>(component);
		} else {
			return nullptr;
		}
	}

	template <class T>
	T* GetComponent(FixedString const& guid, bool logError = true)
	{
		auto component = GetRawComponent(guid, T::ComponentType, logError);
		if (component) {
			return RawComponentPtrToComponent<T>(component);
		} else {
			return nullptr;
		}
	}

	template <class T>
	T* GetComponent(NetId netId, bool logError = true)
	{
		auto component = GetRawComponent(netId, T::ComponentType, logError);
		if (component) {
			return RawComponentPtrToComponent<T>(component);
		} else {
			return nullptr;
		}
	}

	template <class T>
	T* GetEntityComponent(EntityHandle entityHandle, bool logError = true)
	{
		auto component = GetRawEntityComponent(entityHandle, T::ComponentType, logError);
		if (component) {
			return reinterpret_cast<T*>(component);
		} else {
			return nullptr;
		}
	}

	template <class T, class Fun>
	void IterateComponents(Fun fun)
	{
		auto world = GetEntityWorld();
		if (!world) {
			return;
		}

		auto componentIndex = GetComponentIndex(T::ComponentType);
		if (!componentIndex) {
			return;
		}

		auto pool = world->Components.Types[(int)*componentIndex].Pool;
		auto size = pool->GetSize();
		for (auto i = 0; i < size; i++) {
			auto component = pool->GetComponentByIndex(i);
			if (component) {
				fun(RawComponentPtrToComponent<T>(component));
			}
		}
	}

	virtual EntityWorldBase* GetEntityWorld() = 0;

	template <class T>
	std::optional<GuidResourceDefinitionManager<T>*> GetResourceManager()
	{
		auto mgr = GetRawResourceManager(T::ResourceManagerType);
		if (mgr) {
			return static_cast<GuidResourceDefinitionManager<T>*>(mgr);
		} else {
			return {};
		}
	}

protected:
	static constexpr int32_t UndefinedIndex{ -1 };

	void MapComponentIndices(char const* componentName, ExtComponentType type);
	void MapResourceManagerIndex(char const* componentName, ExtResourceManagerType type);
	void UpdateComponentMappings();

private:
	struct ComponentIndexMappings
	{
		std::array<int32_t, 4> Indices;
		std::size_t NumIndices{ 0 };

		void Add(int32_t index);
	};
	
	struct IndexMappings
	{
		EntityWorldBase::HandleTypeIndex HandleIndex;
		EntityWorldBase::ComponentTypeIndex ComponentIndex;
	};

	std::unordered_map<STDString, IndexMappings> componentIndexMappings_;
	std::unordered_map<STDString, int32_t> systemIndexMappings_;
	std::array<EntityWorldBase::ComponentTypeIndex, (int)ExtComponentType::Max> componentIndices_;
	std::array<EntityWorldBase::HandleTypeIndex, (int)ExtComponentType::Max> handleIndices_;
	std::array<int32_t, (int)ExtResourceManagerType::Max> resourceManagerIndices_;
	bool initialized_{ false };

	void* GetRawComponent(NetId netId, ExtComponentType type, bool logError);
	void* GetRawComponent(ObjectHandle componentHandle, ExtComponentType type, bool logError);
	void* GetRawComponent(char const* nameGuid, ExtComponentType type, bool logError);
	void* GetRawComponent(FixedString const& guid, ExtComponentType type, bool logError);
	void* GetRawEntityComponent(EntityHandle entityHandle, ExtComponentType type, bool logError);
	GuidResourceDefinitionManagerBase* GetRawResourceManager(ExtResourceManagerType type);
};

class ServerEntitySystemHelpers : public EntitySystemHelpersBase
{
public:
	void Setup();

	EntityWorldBase* GetEntityWorld() override;
};

class ClientEntitySystemHelpers : public EntitySystemHelpersBase
{
public:
	void Setup();

	EntityWorldBase* GetEntityWorld() override;
};

class OsirisProxy
{
public:
	OsirisProxy();

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

extern std::unique_ptr<OsirisProxy> gOsirisProxy;

}
