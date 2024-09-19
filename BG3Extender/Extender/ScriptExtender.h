#pragma once

#include <Extender/Shared/ExtenderConfig.h>
#include <GameDefinitions/Osiris.h>
#include <GameDefinitions/GuidResources.h>
#include <Extender/Client/ScriptExtenderClient.h>
#if defined(ENABLE_IMGUI)
#include <Extender/Client/IMGUI/IMGUI.h>
#endif
#include <Extender/Server/ScriptExtenderServer.h>
#include <Extender/Shared/StatLoadOrderHelper.h>
#include <Extender/Shared/VirtualTextures.h>
#include <Extender/Shared/UseActions.h>
#include <Extender/Shared/Hooks.h>
#if !defined(OSI_NO_DEBUGGER)
#include <Lua/Debugger/LuaDebugger.h>
#include <Lua/Debugger/LuaDebugMessages.h>
#endif
#include <Lua/Shared/LuaBundle.h>
#include <Lua/Shared/Proxies/LuaCppClass.h>
#include <GameHooks/OsirisWrappers.h>
#include <GameHooks/DataLibraries.h>
#include <GameHooks/EngineHooks.h>

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

	inline bool WasInitialized() const
	{
		return postStartupDone_;
	}

	inline ExtenderConfig & GetConfig()
	{
		return config_;
	}

	void LogLuaError(std::string_view msg);
	void LogOsirisError(std::string_view msg);
	void LogOsirisWarning(std::string_view msg);
	void LogOsirisMsg(std::string_view msg);

	inline esv::ScriptExtender& GetServer()
	{
		return server_;
	}

	inline ecl::ScriptExtender& GetClient()
	{
		return client_;
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
	ecs::EntitySystemHelpersBase* GetECS(ecs::EntityWorld* world);

	bool HasFeatureFlag(char const *) const;

	inline stats::StatLoadOrderHelper& GetStatLoadOrderHelper()
	{
		return statLoadOrderHelper_;
	}

	inline EngineHooks& GetEngineHooks()
	{
		return engineHooks_;
	}

	inline Hooks& GetHooks()
	{
		return hooks_;
	}

	inline lua::LuaBundle& GetLuaBuiltinBundle()
	{
		return luaBuiltinBundle_;
	}

	inline lua::CppPropertyMapManager& GetPropertyMapManager()
	{
		return propertyMapManager_;
	}

	inline VirtualTextureHelpers& GetVirtualTextureHelpers()
	{
		return virtualTextures_;
	}

	inline UseActionHelpers& GetUseActionHelpers()
	{
		return useActions_;
	}

#if defined(ENABLE_IMGUI)
	inline extui::IMGUIManager& IMGUI()
	{
		return imgui_;
	}
#endif

	void ClearPathOverrides();
	void AddPathOverride(STDString const & path, STDString const & overriddenPath);
	std::optional<STDString> GetPathOverride(STDString const& path);

	std::wstring MakeLogFilePath(std::wstring const& Type, std::wstring const& Extension);
	void InitRuntimeLogging();

	void OnSDLEvent(SDL_Event* event);

	enum class CoreLibInitTag {};
	WrappableFunction<CoreLibInitTag, CoreLibInitProc> CoreLibInit;
	enum class AppUpdatePathsTag {};
	WrappableFunction<AppUpdatePathsTag, AppUpdatePathsProc> AppUpdatePaths;
	enum class AppLoadGraphicSettingsTag {};
	WrappableFunction<AppLoadGraphicSettingsTag, App::LoadGraphicSettingsProc> AppLoadGraphicSettings;

private:
	esv::ScriptExtender server_;
	ecl::ScriptExtender client_;
	LibraryManager Libraries;
	EngineHooks engineHooks_;
	Hooks hooks_;
	bool LibrariesPostInitialized{ false };
	std::recursive_mutex globalStateLock_;
	std::shared_mutex pathOverrideMutex_;
	std::unordered_map<STDString, STDString> pathOverrides_;
	stats::StatLoadOrderHelper statLoadOrderHelper_;
	lua::LuaBundle luaBuiltinBundle_;
	lua::CppPropertyMapManager propertyMapManager_;
	VirtualTextureHelpers virtualTextures_;
	UseActionHelpers useActions_;
#if defined(ENABLE_IMGUI)
	extui::IMGUIManager imgui_;
#endif

	ExtenderConfig config_;
	bool postStartupDone_{ false };
	bool updateHooksAdded_{ false };

#if !defined(OSI_NO_DEBUGGER)
	std::thread* luaDebuggerThread_{ nullptr };
	std::unique_ptr<LuaDebugInterface> luaDebugInterface_;
	std::unique_ptr<lua::dbg::DebugMessageHandler> luaDebugMsgHandler_;
	std::unique_ptr<lua::dbg::Debugger> luaDebugger_;
#endif

	void WarnIfOffline();
	void OnCoreLibInit(void * self);
	void OnAppUpdatePaths(void * self);
	void OnAppLoadGraphicSettings(App* self);
	void HookStateMachineUpdates();
	void OnBaseModuleLoaded(void * self);
	void OnModuleLoadStarted(TranslatedStringRepository* self);
	void OnStatsLoad(stats::RPGStats::LoadProc* wrapped, stats::RPGStats* mgr, Array<STDString>* paths);
	void OnStatsLoadGuarded(stats::RPGStats::LoadProc* wrapped, stats::RPGStats* mgr, Array<STDString>* paths);
	void OnECSUpdate(ecs::EntityWorld::UpdateProc* wrapped, ecs::EntityWorld* entityWorld, GameTime const& time);
	void OnECSUpdateGuarded(ecs::EntityWorld::UpdateProc* wrapped, ecs::EntityWorld* entityWorld, GameTime const& time);
	void OnECSFlushECBs(ecs::EntityWorld* entityWorld);
	FileReader * OnFileReaderCreate(FileReader::CtorProc* next, FileReader * self, Path const& path, unsigned int type, unsigned int unknown);
};

extern std::unique_ptr<ScriptExtender> gExtender;

}
