#pragma once

#include <Extender/Shared/ExtenderConfig.h>
#include <GameDefinitions/Osiris.h>
#include <GameDefinitions/GuidResources.h>
#include <Extender/Client/ScriptExtenderClient.h>
#include <Extender/Server/ScriptExtenderServer.h>
#include <Extender/Shared/StatLoadOrderHelper.h>
#include <Extender/Shared/SavegameSerializer.h>
#if !defined(OSI_NO_DEBUGGER)
#include <Lua/LuaDebugger.h>
#include <Lua/LuaDebugMessages.h>
#endif
#include <GameHooks/OsirisWrappers.h>
#include <GameHooks/DataLibraries.h>
#include <GameHooks/EngineHooks.h>
//#include "NetProtocol.h"
//#include <Hit.h>

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <ppl.h>

namespace bg3se {


class ScriptExtender
{
public:
	ScriptExtender();

	void Initialize();
	void PostStartup();
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

	inline esv::ScriptExtender & GetServer()
	{
		return server_;
	}

	inline ecl::ScriptExtender & GetClient()
	{
		return client_;
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

	inline EngineHooks& GetEngineHooks()
	{
		return Hooks;
	}

	void ClearPathOverrides();
	void AddPathOverride(STDString const & path, STDString const & overriddenPath);

private:
	esv::ScriptExtender server_;
	ecl::ScriptExtender client_;
	LibraryManager Libraries;
	EngineHooks Hooks;
	bool LibrariesPostInitialized{ false };
	std::recursive_mutex globalStateLock_;
	//NetworkManager networkManager_;
	std::shared_mutex pathOverrideMutex_;
	std::unordered_map<STDString, STDString> pathOverrides_;
	SavegameSerializer savegameSerializer_;
	StatLoadOrderHelper statLoadOrderHelper_;
	//esv::HitProxy hitProxy_;

	ExtenderConfig config_;
	bool extensionsEnabled_{ false };
	bool postStartupDone_{ false };

#if !defined(OSI_NO_DEBUGGER)
	std::thread* luaDebuggerThread_{ nullptr };
	std::unique_ptr<LuaDebugInterface> luaDebugInterface_;
	std::unique_ptr<lua::dbg::DebugMessageHandler> luaDebugMsgHandler_;
	std::unique_ptr<lua::dbg::Debugger> luaDebugger_;
#endif

	void OnBaseModuleLoaded(void * self);
	/*void OnModuleLoadStarted(TranslatedStringRepository * self);
	void OnStatsLoadStarted(RPGStats* mgr);
	void OnStatsLoadFinished(RPGStats* mgr);*/
	void OnSkillPrototypeManagerInit(void * self);
	FileReader * OnFileReaderCreate(FileReader::CtorProc* next, FileReader * self, Path const& path, unsigned int type, unsigned int unknown);
	void OnSavegameVisit(void* osirisHelpers, ObjectVisitor* visitor);
};

extern std::unique_ptr<ScriptExtender> gExtender;

}
