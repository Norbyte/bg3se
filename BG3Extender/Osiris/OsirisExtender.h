#pragma once

#include <Extender/Shared/ExtenderConfig.h>
#include <GameDefinitions/Osiris.h>
#if !defined(OSI_NO_DEBUGGER)
#include <Osiris/Debugger/DebugInterface.h>
#include <Osiris/Debugger/DebugMessages.h>
#include <Osiris/Debugger/Debugger.h>
#endif
#include <GameHooks/OsirisWrappers.h>
#include <Osiris/Shared/CustomFunctions.h>
#include <Osiris/Shared/NodeHooks.h>
#include <Osiris/Functions/FunctionLibrary.h>
#include <GameHooks/DataLibraries.h>
#include <GameDefinitions/Symbols.h>

#include <thread>

namespace bg3se
{

class OsirisExtender
{
public:
	OsirisExtender(ExtenderConfig & config);
	~OsirisExtender();

	void Initialize();
	void Shutdown();

	void OnBaseModuleLoadedServer();
	void HookNodeVMTs();
	void InitRuntimeLogging();

	void BindCallbackManager(esv::lua::OsirisCallbackManager* mgr);

	void LogError(std::string_view msg);
	void LogWarning(std::string_view msg);
	void LogMessage(std::string_view msg);

	Function const* LookupFunction(const STDString& name, uint32_t arity);
	
	inline ValueType GetBaseType(ValueType type)
	{
		return (*GetGlobals().Types)->ResolveAlias((uint16_t)type);
	}

	inline OsirisStaticGlobals const & GetGlobals() const
	{
		return wrappers_.Globals;
	}

	inline OsirisDynamicGlobals const & GetDynamicGlobals() const
	{
		return dynamicGlobals_;
	}

	inline CustomFunctionManager & GetCustomFunctionManager()
	{
		return customFunctions_;
	}

	inline CustomFunctionInjector const & GetCustomFunctionInjector() const
	{
		return injector_;
	}

	inline CustomFunctionInjector & GetCustomFunctionInjector()
	{
		return injector_;
	}

	inline esv::CustomFunctionLibrary & GetFunctionLibrary()
	{
		return functionLibrary_;
	}

	inline NodeVMTWrappers * GetVMTWrappers()
	{
		return nodeVmtWrappers_.get();
	}

	inline OsirisWrappers & GetWrappers()
	{
		return wrappers_;
	}

	inline OsirisWrappers const & GetWrappers() const
	{
		return wrappers_;
	}

	inline bool IsStoryLoaded() const
	{
		return storyLoaded_;
	}

private:
	ExtenderConfig& config_;
	std::unique_ptr<NodeVMTWrappers> nodeVmtWrappers_;
	OsirisWrappers wrappers_;
	OsirisDynamicGlobals dynamicGlobals_;
	CustomFunctionManager customFunctions_;
	CustomFunctionInjector injector_;
	esv::CustomFunctionLibrary functionLibrary_;
	esv::lua::OsirisCallbackManager* osirisCallbacksAttachment_{ nullptr };
	bool initialized_{ false };

	void OnRegisterDIVFunctions(void *, DivFunctions *);
	void OnInitGame(void *);
	void OnDeleteAllData(void *, bool);

	void OnError(char const * Message);
	void OnAssert(bool Successful, char const * Message, bool Unknown2);
	bool CompileWrapper(bool (*next)(void*, wchar_t const*, wchar_t const*), void * Osiris, wchar_t const * Path, wchar_t const * Mode);
	void OnAfterOsirisLoad(void * Osiris, void * Buf, int retval);
	bool MergeWrapper(bool (*next)(void *, wchar_t *), void * Osiris, wchar_t * Src);
	ReturnCode OnEvent(ReturnCode(*next)(void*, uint32_t, OsiArgumentDesc*), void*, uint32_t, OsiArgumentDesc*);
	void RuleActionCall(void (*next)(RuleActionNode *, void *, void *, void *, void *), RuleActionNode * Action, void * a1, void * a2, void * a3, void * a4);

	std::wstring logFilename_;
	std::wstring logType_;

	bool storyLoaded_{ false };
	std::recursive_mutex storyLoadLock_;

#if !defined(OSI_NO_DEBUGGER)
	std::unique_ptr<std::thread> debuggerThread_;
	std::unique_ptr<OsirisDebugInterface> debugInterface_;
	std::unique_ptr<osidbg::DebugMessageHandler> debugMsgHandler_;
	std::unique_ptr<osidbg::Debugger> debugger_;
#endif

	void RestartLogging(std::wstring const & Type);
};

}
