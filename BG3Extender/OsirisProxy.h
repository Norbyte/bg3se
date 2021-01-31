#pragma once

#include <GameDefinitions/Osiris.h>
#include <GameDefinitions/ResourceDefinitions.h>
#include <ExtensionStateClient.h>
#include <ExtensionStateServer.h>
#if !defined(OSI_NO_DEBUGGER)
#include "DebugInterface.h"
#include "DebugMessages.h"
#include "Debugger.h"
#include "Lua/LuaDebugger.h"
#include "Lua/LuaDebugMessages.h"
#endif
#include "OsirisWrappers.h"
#include "CustomFunctions.h"
#include "DataLibraries.h"
#include "Functions/FunctionLibrary.h"
//#include "NetProtocol.h"
#include <GameDefinitions/Symbols.h>
#include <GlobalFixedStrings.h>
//#include <Hit.h>

#include <thread>
#include <mutex>
#include <shared_mutex>

namespace dse {


struct ToolConfig
{
#if defined(OSI_EXTENSION_BUILD)
	bool CreateConsole{ false };
	bool EnableDebugger{ false };
	bool EnableLuaDebugger{ false };
#else
	bool CreateConsole{ true };
	bool EnableDebugger{ true };
	bool EnableLuaDebugger{ true };
#endif

	bool EnableExtensions{ true };
#if defined(OSI_EOCAPP)
	bool LogFailedCompile{ true };
#else
	bool LogFailedCompile{ false };
#endif
	bool EnableLogging{ false };
	bool LogCompile{ false };
	bool LogRuntime{ false };
	bool SendCrashReports{ true };
	bool EnableAchievements{ true };

#if defined(OSI_EXTENSION_BUILD)
	bool DisableModValidation{ true };
#if defined(_DEBUG)
	bool DeveloperMode{ true };
#else
	bool DeveloperMode{ false };
#endif // defined(_DEBUG)
#else
	bool DisableModValidation{ false };
	bool DeveloperMode{ true };
#endif // defined(OSI_EXTENSION_BUILD)

	bool ShowPerfWarnings{ false };
	bool DumpNetworkStrings{ false };
#if defined(OSI_EXTENSION_BUILD)
	bool SyncNetworkStrings{ true };
#else
	bool SyncNetworkStrings{ false };
#endif
	uint32_t DebuggerPort{ 9999 };
	uint32_t LuaDebuggerPort{ 9998 };
	uint32_t DebugFlags{ 0 };
	std::wstring LogDirectory;
};


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

class EntitySystemHelpersBase
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
	T* GetEntityComponent(ObjectHandle entityHandle, bool logError = true)
	{
		auto component = GetRawEntityComponent(entityHandle, T::ComponentType, logError);
		if (component) {
			return reinterpret_cast<T*>(component);
		} else {
			return nullptr;
		}
	}

	virtual EntityWorldBase* GetEntityWorld() = 0;

	template <class T>
	std::optional<ResourceDefinitionManager<T>*> GetResourceManager()
	{
		auto mgr = GetRawResourceManager(T::ResourceManagerType);
		if (mgr) {
			return static_cast<ResourceDefinitionManager<T>*>(mgr);
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
	void* GetRawEntityComponent(ObjectHandle entityHandle, ExtComponentType type, bool logError);
	ResourceDefinitionManagerBase* GetRawResourceManager(ExtResourceManagerType type);
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

	inline ToolConfig & GetConfig()
	{
		return config_;
	}

	void LogLuaError(std::string_view msg);
	void LogOsirisError(std::string_view msg);
	void LogOsirisWarning(std::string_view msg);
	void LogOsirisMsg(std::string_view msg);

	inline OsirisStaticGlobals const & GetGlobals() const
	{
		return Wrappers.Globals;
	}

	inline OsirisDynamicGlobals const & GetDynamicGlobals() const
	{
		return DynGlobals;
	}

	inline CustomFunctionManager & GetCustomFunctionManager()
	{
		return CustomFunctions;
	}

	inline CustomFunctionInjector const & GetCustomFunctionInjector() const
	{
		return CustomInjector;
	}

	inline CustomFunctionInjector& GetCustomFunctionInjector()
	{
		return CustomInjector;
	}

	inline esv::CustomFunctionLibrary & GetFunctionLibrary()
	{
		return FunctionLibrary;
	}

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

	inline void * GetOsirisDllStart() const
	{
		return Wrappers.OsirisDllStart;
	}

	inline uint32_t GetOsirisDllSize() const
	{
		return Wrappers.OsirisDllSize;
	}

	inline OsirisWrappers & GetWrappers()
	{
		return Wrappers;
	}

	inline OsirisWrappers const & GetWrappers() const
	{
		return Wrappers;
	}

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

	inline bool IsStoryLoaded() const
	{
		return StoryLoaded;
	}

	void ClearPathOverrides();
	void AddPathOverride(STDString const & path, STDString const & overriddenPath);

	bool IsInServerThread() const;
	bool IsInClientThread() const;
	void AttachConsoleThread(bool server);
	void HookNodeVMTs();

	std::wstring MakeLogFilePath(std::wstring const& Type, std::wstring const& Extension);
	void ResetLuaState(bool resetServer, bool resetClient);

private:
	OsirisWrappers Wrappers;
	OsirisDynamicGlobals DynGlobals;
	CustomFunctionManager CustomFunctions;
	CustomFunctionInjector CustomInjector;
	esv::CustomFunctionLibrary FunctionLibrary;
	std::unique_ptr<esv::ExtensionState> ServerExtState;
	std::unique_ptr<ecl::ExtensionState> ClientExtState;
	LibraryManager Libraries;
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

	NodeVMT * NodeVMTs[(unsigned)NodeType::Max + 1];
	bool ResolvedNodeVMTs{ false };

	void OnRegisterDIVFunctions(void *, DivFunctions *);
	void OnInitGame(void *);
	void OnDeleteAllData(void *, bool);

	void OnError(char const * Message);
	void OnAssert(bool Successful, char const * Message, bool Unknown2);
	bool CompileWrapper(std::function<bool (void *, wchar_t const *, wchar_t const *)> const & Next, void * Osiris, wchar_t const * Path, wchar_t const * Mode);
	void OnAfterOsirisLoad(void * Osiris, void * Buf, int retval);
	bool MergeWrapper(std::function<bool(void *, wchar_t *)> const & Next, void * Osiris, wchar_t * Src);
	void RuleActionCall(std::function<void(RuleActionNode *, void *, void *, void *, void *)> const & Next, RuleActionNode * Action, void * a1, void * a2, void * a3, void * a4);

	ToolConfig config_;
	bool extensionsEnabled_{ false };
	bool functionLibraryInitialized_{ false };

	std::wstring LogFilename;
	std::wstring LogType;

	bool StoryLoaded{ false };
	std::recursive_mutex storyLoadLock_;

#if !defined(OSI_NO_DEBUGGER)
	std::thread * debuggerThread_{ nullptr };
	std::unique_ptr<OsirisDebugInterface> debugInterface_;
	std::unique_ptr<osidbg::DebugMessageHandler> debugMsgHandler_;
	std::unique_ptr<osidbg::Debugger> debugger_;

	std::thread* luaDebuggerThread_{ nullptr };
	std::unique_ptr<LuaDebugInterface> luaDebugInterface_;
	std::unique_ptr<lua::dbg::DebugMessageHandler> luaDebugMsgHandler_;
	std::unique_ptr<lua::dbg::Debugger> luaDebugger_;
#endif

	void ResolveNodeVMTs(NodeDb * Db);
	void SaveNodeVMT(NodeType type, NodeVMT * vmt);
	void RestartLogging(std::wstring const & Type);

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
	FileReader * OnFileReaderCreate(FileReader::CtorProc* next, FileReader * self, Path const& path, unsigned int type);
	void OnSavegameVisit(void* osirisHelpers, ObjectVisitor* visitor);
	void PostInitLibraries();
	void InitRuntimeLogging();
	void ResetExtensionStateServer();
	void LoadExtensionStateServer();
	void ResetExtensionStateClient();
	void LoadExtensionStateClient();

	void AddClientThread(DWORD threadId);
	void AddServerThread(DWORD threadId);
	void RemoveClientThread(DWORD threadId);
	void RemoveServerThread(DWORD threadId);
	
	static void FlashTraceCallback(void * ctx, void * player, char const * message);
	static void FlashWarningCallback(void * ctx, void * player, int code, char const * message);
};

extern std::unique_ptr<OsirisProxy> gOsirisProxy;

}
