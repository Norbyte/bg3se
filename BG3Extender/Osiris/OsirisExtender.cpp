#include "stdafx.h"
#include <Osiris/OsirisExtender.h>
#include <Extender/Shared/ExtensionHelpers.h>
#include <Extender/ScriptExtender.h>
#include <iomanip>

namespace bg3se
{

#if !defined(OSI_NO_DEBUGGER)
void OsirisDebugThreadRunner(OsirisDebugInterface & intf)
{
	intf.Run();
}
#endif

OsirisExtender::OsirisExtender(ExtenderConfig & config)
	: config_(config), 
	injector_(wrappers_, customFunctions_),
	functionLibrary_(*this)
{
	functionLibrary_.Startup();
}


OsirisExtender::~OsirisExtender()
{
	Shutdown();
}

void OsirisExtender::Initialize()
{
	if (initialized_) return;

	wrappers_.Initialize();

	wrappers_.RegisterDivFunctions.SetPreHook(&OsirisExtender::OnRegisterDIVFunctions, this);
	wrappers_.InitGame.SetPreHook(&OsirisExtender::OnInitGame, this);
	wrappers_.DeleteAllData.SetPreHook(&OsirisExtender::OnDeleteAllData, this);
	wrappers_.Error.SetPreHook(&OsirisExtender::OnError, this);
	wrappers_.Assert.SetPreHook(&OsirisExtender::OnAssert, this);
	wrappers_.Compile.SetWrapper(&OsirisExtender::CompileWrapper, this);
	wrappers_.Load.SetPostHook(&OsirisExtender::OnAfterOsirisLoad, this);
	wrappers_.Merge.SetWrapper(&OsirisExtender::MergeWrapper, this);
	wrappers_.Event.SetWrapper(&OsirisExtender::OnEvent, this);
#if !defined(OSI_NO_DEBUGGER)
	wrappers_.RuleActionCall.SetWrapper(&OsirisExtender::RuleActionCall, this);
#endif

	injector_.Initialize();
	functionLibrary_.Register();
	initialized_ = true;
}

void OsirisExtender::Shutdown()
{
	if (!initialized_) return;

	if (debugger_) {
		debugger_.reset();
	}

	if (debuggerThread_) {
		debugInterface_->Shutdown();
		debuggerThread_->join();
		debugMsgHandler_.reset();
		debugInterface_.reset();
		debuggerThread_.reset();
	}

	wrappers_.Shutdown();
	initialized_ = false;
}

void OsirisExtender::OnBaseModuleLoadedServer()
{
	customFunctions_.ClearDynamicEntries();
}

void OsirisExtender::HookNodeVMTs()
{
	if (wrappers_.ResolveNodeVMTs()) {
		nodeVmtWrappers_.reset();
		nodeVmtWrappers_ = std::make_unique<NodeVMTWrappers>(wrappers_.VMTs);
	}
}

void OsirisExtender::LogError(std::string_view msg)
{
	if (storyLoaded_) {
		std::string log = "[Osiris] {E} ";
		log += msg;
		wrappers_.AssertOriginal(false, log.c_str(), false);
	}
}

void OsirisExtender::LogWarning(std::string_view msg)
{
	if (storyLoaded_) {
		std::string log = "[Osiris] {W} ";
		log += msg;
		wrappers_.AssertOriginal(false, log.c_str(), false);
	}
}

void OsirisExtender::LogMessage(std::string_view msg)
{
	if (storyLoaded_) {
		std::string log = "[Osiris] ";
		log += msg;
		wrappers_.AssertOriginal(false, log.c_str(), false);
	}
}

void OsirisExtender::RestartLogging(std::wstring const & Type)
{
	DebugFlag NewFlags = (DebugFlag)((*wrappers_.Globals.DebugFlags & 0x0000ffff) | (config_.DebugFlags & 0xffff0000));

	if (logFilename_.empty() || logType_ != Type) {
		logFilename_ = gExtender->MakeLogFilePath(Type, L"log");
		logType_ = Type;

		if (!logFilename_.empty()) {
			DEBUG("ScriptExtender::RestartLogging: Starting %s debug logging.\r\n"
				"\tPath=%s", ToStdUTF8(Type).c_str(), ToStdUTF8(logFilename_).c_str());
		}
	}

	wrappers_.CloseLogFile.CallOriginal(dynamicGlobals_.OsirisObject);

	if (!logFilename_.empty()) {
		*wrappers_.Globals.DebugFlags = NewFlags;
		wrappers_.OpenLogFile.CallOriginal(dynamicGlobals_.OsirisObject, logFilename_.c_str(), std::iostream::app);
	}
}

void OsirisExtender::OnRegisterDIVFunctions(void * Osiris, DivFunctions * Functions)
{
	wrappers_.RegisterDIVFunctionsPreHook(Osiris, Functions);

	storyLoaded_ = false;
	dynamicGlobals_.OsirisObject = Osiris;
	uint8_t * interfaceLoadPtr = nullptr;
	auto errorMessageFunc = ResolveRealFunctionAddress((uint8_t *)wrappers_.ErrorOriginal);

	// Look for TypedValue::VMT
	uint8_t const copyCtor1[] = {
		0x48, 0x83, 0xec, 0x20, // sub     rsp, 20h
		0x48, 0x8b, 0xd9, // mov     rbx, rcx
		0x48, 0x8d, 0x05 // lea     rax, TypedValue__VMT
	};

	uint8_t const copyCtor2[] = {
		0x48, 0x89, 0x01, // mov     [rcx], rax
		0x0f, 0xb7, 0x41, 0x10 // movzx   eax, word ptr [rcx+10h]
	};

	auto start = reinterpret_cast<uint8_t *>(wrappers_.OsirisDllStart);
	auto end = start + wrappers_.OsirisDllSize - sizeof(copyCtor1);

	for (auto p = start; p < end; p++) {
		if (*p == 0x48
			&& memcmp(copyCtor1, p, sizeof(copyCtor1)) == 0
			&& memcmp(copyCtor2, p + 14, sizeof(copyCtor2)) == 0) {
			wrappers_.Globals.TypedValueVMT = (void *)AsmResolveInstructionRef(p + 7);
			break;
		}
	}

	if (wrappers_.Globals.TypedValueVMT == nullptr) {
		ERR("Could not find TypedValue vtable");
	}

	if (config_.EnableLogging) {
		RestartLogging(L"Osiris Runtime");
	}

#if 0
	DEBUG("ScriptExtender::OnRegisterDIVFunctions: Initializing story.");
	DEBUG("\tErrorMessageProc = %p", errorMessageFunc);
	DEBUG("\tOsirisManager = %p", Globals.Manager);
	DEBUG("\tOsirisInterface = %p", osirisInterface);
#endif

#if !defined(OSI_NO_DEBUGGER)
	// FIXME - move to DebuggerHooks
	if (config_.EnableDebugger) {
		if (!debuggerThread_) {
			DEBUG("Starting debugger server");
			try {
				debugInterface_ = std::make_unique<OsirisDebugInterface>(config_.DebuggerPort);
				debugMsgHandler_ = std::make_unique<osidbg::DebugMessageHandler>(std::ref(*debugInterface_));
				debuggerThread_ = std::make_unique<std::thread>(std::bind(OsirisDebugThreadRunner, std::ref(*debugInterface_)));
				DEBUG("Osiris debugger listening on 127.0.0.1:%d; DBG protocol version %d", 
					config_.DebuggerPort, osidbg::DebugMessageHandler::ProtocolVersion);
			} catch (std::exception & e) {
				ERR("Osiris debugger start failed: %s", e.what());
			}
		}
	}
#endif
}

void OsirisExtender::OnInitGame(void * Osiris)
{
	DEBUG("OsirisExtender::OnInitGame()");
#if !defined(OSI_NO_DEBUGGER)
	if (debugger_) {
		debugger_->GameInitHook();
	}
#endif
	DEBUG("OnRegisterDIVFunctions END");
}

void OsirisExtender::OnDeleteAllData(void * Osiris, bool DeleteTypes)
{
#if !defined(OSI_NO_DEBUGGER)
	if (debugger_) {
		DEBUG("OsirisExtender::OnDeleteAllData()");
		debugger_->DeleteAllDataHook();
		debugger_.reset();
	}
#endif
}

void OsirisExtender::OnError(char const * Message)
{
	ERR("Osiris Error: %s", Message);
}

void OsirisExtender::OnAssert(bool Successful, char const * Message, bool Unknown2)
{
	if (!Successful) {
		WARN("Osiris Assert: %s", Message);
	}
}

bool OsirisExtender::CompileWrapper(bool (* next)(void *, wchar_t const *, wchar_t const *), void * Osiris, wchar_t const * Path, wchar_t const * Mode)
{
	if (!config_.ExtendStory) {
		return next(Osiris, Path, Mode);
	}

	DEBUG("OsirisExtender::CompileWrapper: Starting compilation of '%s'", ToStdUTF8(Path).c_str());
	auto OriginalFlags = *wrappers_.Globals.DebugFlags;
	std::wstring storyPath;

	customFunctions_.PreProcessStory(Path);

	if (config_.LogCompile || config_.LogFailedCompile) {
		if (!config_.LogCompile) {
			*wrappers_.Globals.DebugFlags = (DebugFlag)(OriginalFlags & ~DebugFlag::DF_CompileTrace);
		}

		RestartLogging(L"Compile");

		if (config_.LogCompile) {
			storyPath = gExtender->MakeLogFilePath(L"Compile", L"div");
			CopyFileW(Path, storyPath.c_str(), TRUE);
		}
	}

	auto ret = next(Osiris, Path, Mode);

	if (ret) {
		DEBUG("ScriptExtender::CompileWrapper: Success.");
	} else {
		ERR("ScriptExtender::CompileWrapper: Compilation FAILED.");
	}

	if (config_.LogCompile || config_.LogFailedCompile) {
		*wrappers_.Globals.DebugFlags = OriginalFlags;
		wrappers_.CloseLogFile.CallOriginal(dynamicGlobals_.OsirisObject);

		if (ret) {
			if (config_.LogCompile) {
				DeleteFileW(storyPath.c_str());
			} else if (!logFilename_.empty()) {
				DeleteFileW(logFilename_.c_str());
			}
		}
	}

	return ret;
}

void OsirisExtender::OnAfterOsirisLoad(void * Osiris, void * Buf, int retval)
{
	std::lock_guard _(storyLoadLock_);

#if !defined(OSI_NO_DEBUGGER)
	if (debuggerThread_) {
		HookNodeVMTs();
	}
#endif

	storyLoaded_ = true; 
	DEBUG("ScriptExtender::OnAfterOsirisLoad: %d nodes", (*wrappers_.Globals.Nodes)->Db.Size);

#if !defined(OSI_NO_DEBUGGER)
	if (debuggerThread_ && nodeVmtWrappers_) {
		debugger_.reset();
		debugger_ = std::make_unique<osidbg::Debugger>(wrappers_.Globals, std::ref(*debugMsgHandler_));
		debugger_->StoryLoaded();
	}
#endif

	if (gExtender->GetServer().HasExtensionState()) {
		gExtender->GetServer().GetExtensionState().StoryLoaded();
	}
}

bool OsirisExtender::MergeWrapper(bool (*next)(void*, wchar_t*), void * Osiris, wchar_t * Src)
{
	DEBUG("ScriptExtender::MergeWrapper() - Started merge");

	auto cli = GetEoCClient();
	auto srv = GetEoCServer();

	if (gExtender->GetServer().HasExtensionState()) {
		gExtender->GetServer().GetExtensionState().StorySetMerging(true);
	}

#if !defined(OSI_NO_DEBUGGER)
	if (debugger_ != nullptr) {
		debugger_->MergeStarted();
	}
#endif

	bool retval = next(Osiris, Src);

#if !defined(OSI_NO_DEBUGGER)
	if (debugger_ != nullptr) {
		debugger_->MergeFinished();
	}
#endif

	if (gExtender->GetServer().HasExtensionState()) {
		gExtender->GetServer().GetExtensionState().StorySetMerging(false);
		gExtender->GetServer().GetExtensionState().StoryLoaded();
	}

	DEBUG("ScriptExtender::MergeWrapper() - Finished merge");
	return retval;
}

ReturnCode OsirisExtender::OnEvent(ReturnCode(*next)(void*, uint32_t, OsiArgumentDesc*), void* self, uint32_t functionId, OsiArgumentDesc* args)
{
	auto fun = (*wrappers_.Globals.Functions)->FindById(functionId);
	if (fun && osirisCallbacksAttachment_ != nullptr) {
		osirisCallbacksAttachment_->EventPreHook(*fun, args);
	}

	auto ret = next(self, functionId, args);

	if (fun && osirisCallbacksAttachment_ != nullptr) {
		osirisCallbacksAttachment_->EventPostHook(*fun, args);
	}

	return ret;
}

void OsirisExtender::RuleActionCall(void (*next)(RuleActionNode*, void*, void*, void*, void*), RuleActionNode * Action, void * a1, void * a2, void * a3, void * a4)
{
#if !defined(OSI_NO_DEBUGGER)
	if (debugger_ != nullptr) {
		debugger_->RuleActionPreHook(Action);
	}
#endif

	next(Action, a1, a2, a3, a4);

#if !defined(OSI_NO_DEBUGGER)
	if (debugger_ != nullptr) {
		debugger_->RuleActionPostHook(Action);
	}
#endif
}

void OsirisExtender::InitRuntimeLogging()
{
	if (!config_.LogRuntime) return;

	auto path = gExtender->MakeLogFilePath(L"Extender Runtime", L"log");
	gCoreLibPlatformInterface.GlobalConsole->OpenLogFile(path);
	DEBUG("Extender runtime log written to '%s'", ToStdUTF8(path).c_str());
}

void OsirisExtender::BindCallbackManager(esv::lua::OsirisCallbackManager* mgr)
{
	osirisCallbacksAttachment_ = mgr;
	if (nodeVmtWrappers_) {
		nodeVmtWrappers_->OsirisCallbacksAttachment = mgr;
	}
}

}
