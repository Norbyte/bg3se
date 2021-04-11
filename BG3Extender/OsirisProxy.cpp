#include "stdafx.h"
#include "OsirisProxy.h"
#include "NodeHooks.h"
#include "Version.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <psapi.h>
#include <regex>


#undef DEBUG_SERVER_CLIENT

void InitCrashReporting();
void ShutdownCrashReporting();

namespace bg3se
{

std::unique_ptr<OsirisProxy> gOsirisProxy;


#if !defined(OSI_NO_DEBUGGER)
void OsirisDebugThreadRunner(OsirisDebugInterface & intf)
{
	intf.Run();
}

void LuaDebugThreadRunner(LuaDebugInterface& intf)
{
	intf.Run();
}
#endif

OsirisProxy::OsirisProxy()
	: CustomInjector(Wrappers, CustomFunctions),
	FunctionLibrary(*this)/*,
	hitProxy_(*this)*/
{
}

void OsirisProxy::Initialize()
{
	if (config_.SendCrashReports) {
		InitCrashReporting();
	}

	extensionsEnabled_ = config_.EnableExtensions;

	GameVersionInfo gameVersion;
	if (Libraries.GetGameVersion(gameVersion)) {
		if (gameVersion.IsSupported()) {
			INFO("Game version v%d.%d.%d.%d OK", gameVersion.Major, gameVersion.Minor, gameVersion.Revision, gameVersion.Build);
		} else {
			ERR("Game version v%d.%d.%d.%d is not supported, please upgrade!", gameVersion.Major, gameVersion.Minor, gameVersion.Revision, gameVersion.Build);
			// Hard exit below a certain version as th EoCClient error display UI won't work anymore
			Fail(L"Script Extender doesn't support game versions below v3.6.54, please upgrade!");
		}
	} else {
		ERR("Failed to retrieve game version info.");
	}

	DEBUG("OsirisProxy::Initialize: Starting");
	auto initStart = std::chrono::high_resolution_clock::now();
	Wrappers.Initialize();

	using namespace std::placeholders;
	Wrappers.RegisterDivFunctions.AddPreHook(std::bind(&OsirisProxy::OnRegisterDIVFunctions, this, _1, _2));
	Wrappers.InitGame.SetPreHook(std::bind(&OsirisProxy::OnInitGame, this, _1));
	Wrappers.DeleteAllData.SetPreHook(std::bind(&OsirisProxy::OnDeleteAllData, this, _1, _2));
	Wrappers.Error.SetPreHook(std::bind(&OsirisProxy::OnError, this, _1));
	Wrappers.Assert.SetPreHook(std::bind(&OsirisProxy::OnAssert, this, _1, _2, _3));
	Wrappers.Compile.SetWrapper(std::bind(&OsirisProxy::CompileWrapper, this, _1, _2, _3, _4));
	Wrappers.Load.AddPostHook(std::bind(&OsirisProxy::OnAfterOsirisLoad, this, _1, _2, _3));
	Wrappers.Merge.SetWrapper(std::bind(&OsirisProxy::MergeWrapper, this, _1, _2, _3));
#if !defined(OSI_NO_DEBUGGER)
	Wrappers.RuleActionCall.SetWrapper(std::bind(&OsirisProxy::RuleActionCall, this, _1, _2, _3, _4, _5, _6));
#endif

	if (Libraries.FindLibraries(gameVersion.Revision)) {
		if (extensionsEnabled_) {
			ResetExtensionStateServer();
			ResetExtensionStateClient();
		} else {
			DEBUG("OsirisProxy::Initialize: Skipped library init -- scripting extensions not enabled.");
		}
	} else {
		ERR("OsirisProxy::Initialize: Could not load libraries; skipping scripting extension initialization.");
		extensionsEnabled_ = false;
	}

	// Wrap state change functions even if extension startup failed, otherwise
	// we won't be able to show any startup errors
	Wrappers.InitializeExtensions();
	Wrappers.ClientGameStateChangedEvent.SetPostHook(std::bind(&OsirisProxy::OnClientGameStateChanged, this, _1, _2, _3));
	Wrappers.ServerGameStateChangedEvent.SetPostHook(std::bind(&OsirisProxy::OnServerGameStateChanged, this, _1, _2, _3));
	Wrappers.ClientGameStateWorkerStart.AddPreHook(std::bind(&OsirisProxy::OnClientGameStateWorkerStart, this, _1));
	Wrappers.ServerGameStateWorkerStart.AddPreHook(std::bind(&OsirisProxy::OnServerGameStateWorkerStart, this, _1));
	Wrappers.ClientGameStateWorkerStart.AddPostHook(std::bind(&OsirisProxy::OnClientGameStateWorkerExit, this, _1));
	Wrappers.ServerGameStateWorkerStart.AddPostHook(std::bind(&OsirisProxy::OnServerGameStateWorkerExit, this, _1));
	Wrappers.SkillPrototypeManagerInit.SetPreHook(std::bind(&OsirisProxy::OnSkillPrototypeManagerInit, this, _1));
	/*Wrappers.FileReader__ctor.SetWrapper(std::bind(&OsirisProxy::OnFileReaderCreate, this, _1, _2, _3, _4));
	Wrappers.esv__OsirisVariableHelper__SavegameVisit.SetPreHook(std::bind(&OsirisProxy::OnSavegameVisit, this, _1, _2));
	Wrappers.TranslatedStringRepository__UnloadOverrides.SetPreHook(std::bind(&OsirisProxy::OnModuleLoadStarted, this, _1));
	Wrappers.RPGStats__Load.AddPreHook(std::bind(&OsirisProxy::OnStatsLoadStarted, this, _1));
	Wrappers.RPGStats__Load.AddPostHook(std::bind(&OsirisProxy::OnStatsLoadFinished, this, _1));*/

#if !defined(OSI_NO_DEBUGGER)
	if (config_.EnableLuaDebugger && luaDebuggerThread_ == nullptr) {
		DEBUG("Starting Lua debugger server");
		try {
			luaDebugInterface_ = std::make_unique<LuaDebugInterface>(config_.LuaDebuggerPort);
			luaDebugMsgHandler_ = std::make_unique<lua::dbg::DebugMessageHandler>(std::ref(*luaDebugInterface_));
			luaDebuggerThread_ = new std::thread(std::bind(LuaDebugThreadRunner, std::ref(*luaDebugInterface_)));
			luaDebugger_ = std::make_unique<lua::dbg::Debugger>(std::ref(*luaDebugMsgHandler_));
			DEBUG("Lua debugger listening on 127.0.0.1:%d; DBG protocol version %d",
				config_.LuaDebuggerPort, lua::dbg::DebugMessageHandler::ProtocolVersion);
		} catch (std::exception& e) {
			ERR("Lua debugger startup failed: %s", e.what());
		}
	}
#endif

	auto initEnd = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(initEnd - initStart).count();
	DEBUG("Library startup took %d ms", ms);

	if (config_.SendCrashReports) {
		ShutdownCrashReporting();
	}
}

void OsirisProxy::Shutdown()
{
	DEBUG("OsirisProxy::Shutdown: Exiting");
	ResetExtensionStateServer();
	ResetExtensionStateClient();
	Wrappers.Shutdown();
}

void OsirisProxy::LogLuaError(std::string_view msg)
{
	std::string log = "[Osiris] {E} ";
	log += msg;
	gConsole.Debug(DebugMessageType::Error, log.c_str());
	if (StoryLoaded) {
		Wrappers.AssertOriginal(false, log.c_str(), false);
	}
}

void OsirisProxy::LogOsirisError(std::string_view msg)
{
	std::string log = "[Osiris] {E} ";
	log += msg;
	gConsole.Debug(DebugMessageType::Error, log.c_str());
	if (StoryLoaded) {
		Wrappers.AssertOriginal(false, log.c_str(), false);
	}

#if !defined(OSI_NO_DEBUGGER)
	if (luaDebugger_) {
		luaDebugger_->OnGenericError(msg.data());
	}
#endif
}

void OsirisProxy::LogOsirisWarning(std::string_view msg)
{
	std::string log = "[Osiris] {W} ";
	log += msg;
	gConsole.Debug(DebugMessageType::Warning, log.c_str());
	if (StoryLoaded) {
		Wrappers.AssertOriginal(false, log.c_str(), false);
	}
}

void OsirisProxy::LogOsirisMsg(std::string_view msg)
{
	std::string log = "[Osiris] ";
	log += msg;
	gConsole.Debug(DebugMessageType::Osiris, log.c_str());
	if (StoryLoaded) {
		Wrappers.AssertOriginal(false, log.c_str(), false);
	}
}

void OsirisProxy::RestartLogging(std::wstring const & Type)
{
	DebugFlag NewFlags = (DebugFlag)((config_.DebugFlags & 0xffff0000) | (*Wrappers.Globals.DebugFlags & 0x0000ffff));

	if (LogFilename.empty() || LogType != Type) {
		LogFilename = MakeLogFilePath(Type, L"log");
		LogType = Type;

		if (!LogFilename.empty()) {
			DEBUG(L"OsirisProxy::RestartLogging: Starting %s debug logging.\r\n"
				"\tPath=%s", Type.c_str(), LogFilename.c_str());
		}
	}

	Wrappers.CloseLogFile.CallOriginal(DynGlobals.OsirisObject);

	if (!LogFilename.empty()) {
		*Wrappers.Globals.DebugFlags = NewFlags;
		Wrappers.OpenLogFile.CallOriginal(DynGlobals.OsirisObject, LogFilename.c_str(), std::iostream::app);
	}
}

std::wstring OsirisProxy::MakeLogFilePath(std::wstring const & Type, std::wstring const & Extension)
{
	if (config_.LogDirectory.empty()) {
		config_.LogDirectory = FromUTF8(GetStaticSymbols().ToPath("/Extender Logs", PathRootType::GameStorage));
	}

	if (config_.LogDirectory.empty()) {
		return L"";
	}

	BOOL created = CreateDirectoryW(config_.LogDirectory.c_str(), NULL);
	if (created == FALSE) {
		DWORD lastError = GetLastError();
		if (lastError != ERROR_ALREADY_EXISTS) {
			std::wstringstream err;
			err << L"Could not create log directory '" << config_.LogDirectory << "': Error " << lastError;
			return L"";
		}
	}

	auto now = std::chrono::system_clock::now();
	auto tt = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
	gmtime_s(&tm, &tt);

	std::wstringstream ss;
	ss << config_.LogDirectory << L"\\"
		<< Type << L" "
		<< std::setfill(L'0')
		<< (tm.tm_year + 1900) << L"-"
		<< std::setw(2) << (tm.tm_mon + 1) << L"-"
		<< std::setw(2) << tm.tm_mday << L" "
		<< std::setw(2) << tm.tm_hour << L"-"
		<< std::setw(2) << tm.tm_min << L"-"
		<< std::setw(2) << tm.tm_sec << L"." << Extension;
	return ss.str();
}

void OsirisProxy::HookNodeVMTs()
{
	ResolveNodeVMTs(*Wrappers.Globals.Nodes);

	if (ResolvedNodeVMTs && !gNodeVMTWrappers) {
		gNodeVMTWrappers = std::make_unique<NodeVMTWrappers>(NodeVMTs);
	}
}

void OsirisProxy::OnRegisterDIVFunctions(void * Osiris, DivFunctions * Functions)
{
	// FIXME - register before OsirisWrappers!
	StoryLoaded = false;
	DynGlobals.OsirisObject = Osiris;
	uint8_t * interfaceLoadPtr = nullptr;
	// uint8_t * errorMessageFunc = ResolveRealFunctionAddress((uint8_t *)Functions->ErrorMessage);
	uint8_t * errorMessageFunc = ResolveRealFunctionAddress((uint8_t *)Wrappers.ErrorOriginal);

	// Look for TypedValue::VMT
	uint8_t const copyCtor1[] = {
		0x48, 0x89, 0x5C, 0x24, 0x08, // mov     [rsp+arg_0], rbx
		0x48, 0x89, 0x74, 0x24, 0x10, // mov     [rsp+arg_8], rsi
		0x57, // push    rdi
		0x48, 0x83, 0xEC, 0x20, // sub     rsp, 20h
		0x33, 0xF6, // xor     esi, esi
		0x48, 0x8D, 0x05 // lea     rax, TypedValue::VMT
	};

	auto start = reinterpret_cast<uint8_t *>(Wrappers.OsirisDllStart);
	auto end = start + Wrappers.OsirisDllSize - sizeof(copyCtor1);

	for (auto p = start; p < end; p++) {
		if (*p == 0x48
			&& memcmp(copyCtor1, p, sizeof(copyCtor1)) == 0) {
			Wrappers.Globals.TypedValueVMT = (void *)AsmResolveInstructionRef(p + 17);
			break;
		}
	}

	if (config_.EnableLogging) {
		RestartLogging(L"Osiris Runtime");
	}

#if 0
	DEBUG("OsirisProxy::OnRegisterDIVFunctions: Initializing story.");
	DEBUG("\tErrorMessageProc = %p", errorMessageFunc);
	DEBUG("\tOsirisManager = %p", Globals.Manager);
	DEBUG("\tOsirisInterface = %p", osirisInterface);
#endif

#if !defined(OSI_NO_DEBUGGER)
	// FIXME - move to DebuggerHooks
	if (config_.EnableDebugger) {
		if (debuggerThread_ == nullptr) {
			DEBUG("Starting debugger server");
			try {
				debugInterface_ = std::make_unique<OsirisDebugInterface>(config_.DebuggerPort);
				debugMsgHandler_ = std::make_unique<osidbg::DebugMessageHandler>(std::ref(*debugInterface_));
				debuggerThread_ = new std::thread(std::bind(OsirisDebugThreadRunner, std::ref(*debugInterface_)));
				DEBUG("Osiris debugger listening on 127.0.0.1:%d; DBG protocol version %d", 
					config_.DebuggerPort, osidbg::DebugMessageHandler::ProtocolVersion);
			} catch (std::exception & e) {
				ERR("Osiris debugger start failed: %s", e.what());
			}
		}
	}
#endif
}

void OsirisProxy::OnInitGame(void * Osiris)
{
	DEBUG("OsirisProxy::OnInitGame()");
#if !defined(OSI_NO_DEBUGGER)
	if (debugger_) {
		debugger_->GameInitHook();
	}
#endif
}

void OsirisProxy::OnDeleteAllData(void * Osiris, bool DeleteTypes)
{
#if !defined(OSI_NO_DEBUGGER)
	if (debugger_) {
		DEBUG("OsirisProxy::OnDeleteAllData()");
		debugger_->DeleteAllDataHook();
		debugger_.reset();
	}
#endif
}

void OsirisProxy::OnError(char const * Message)
{
	ERR("Osiris Error: %s", Message);
}

void OsirisProxy::OnAssert(bool Successful, char const * Message, bool Unknown2)
{
	if (!Successful) {
		WARN("Osiris Assert: %s", Message);
	}
}

bool OsirisProxy::CompileWrapper(std::function<bool(void *, wchar_t const *, wchar_t const *)> const & Next, void * Osiris, wchar_t const * Path, wchar_t const * Mode)
{
	DEBUG(L"OsirisProxy::CompileWrapper: Starting compilation of '%s'", Path);
	auto OriginalFlags = *Wrappers.Globals.DebugFlags;
	std::wstring storyPath;

	if (extensionsEnabled_) {
		CustomFunctions.PreProcessStory(Path);
	}

	if (config_.LogCompile || config_.LogFailedCompile) {
		if (!config_.LogCompile) {
			*Wrappers.Globals.DebugFlags = (DebugFlag)(OriginalFlags & ~DebugFlag::DF_CompileTrace);
		}

		RestartLogging(L"Compile");

		if (config_.LogCompile) {
			storyPath = MakeLogFilePath(L"Compile", L"div");
			CopyFileW(Path, storyPath.c_str(), TRUE);
		}
	}

	auto ret = Next(Osiris, Path, Mode);

	if (ret) {
		DEBUG("OsirisProxy::CompileWrapper: Success.");
	} else {
		ERR("OsirisProxy::CompileWrapper: Compilation FAILED.");
	}

	if (config_.LogCompile || config_.LogFailedCompile) {
		*Wrappers.Globals.DebugFlags = OriginalFlags;
		Wrappers.CloseLogFile.CallOriginal(DynGlobals.OsirisObject);

		if (ret) {
			if (config_.LogCompile) {
				DeleteFileW(storyPath.c_str());
			} else if (!LogFilename.empty()) {
				DeleteFileW(LogFilename.c_str());
			}
		}
	}

	return ret;
}

void OsirisProxy::OnAfterOsirisLoad(void * Osiris, void * Buf, int retval)
{
	std::lock_guard _(storyLoadLock_);

#if !defined(OSI_NO_DEBUGGER)
	if (debuggerThread_ != nullptr) {
		HookNodeVMTs();
	}
#endif

	StoryLoaded = true; 
	DEBUG("OsirisProxy::OnAfterOsirisLoad: %d nodes", (*Wrappers.Globals.Nodes)->Db.Size);

#if !defined(OSI_NO_DEBUGGER)
	if (debuggerThread_ != nullptr && gNodeVMTWrappers) {
		debugger_.reset();
		debugger_ = std::make_unique<osidbg::Debugger>(Wrappers.Globals, std::ref(*debugMsgHandler_));
		debugger_->StoryLoaded();
	}
#endif

	if (extensionsEnabled_) {
		esv::ExtensionState::Get().StoryLoaded();
	}
}
/*
void OsirisProxy::OnStatsLoadStarted(RPGStats* mgr)
{
	statLoadOrderHelper_.OnLoadStarted();
}

void OsirisProxy::OnStatsLoadFinished(RPGStats* mgr)
{
	statLoadOrderHelper_.OnLoadFinished();
	auto state = GetCurrentExtensionState();
	if (state) {
		state->OnStatsLoaded();
	}
}*/

void OsirisProxy::FlashTraceCallback(void * ctx, void * player, char const * message)
{
	DEBUG("Flash: %s", message);
}

void OsirisProxy::FlashWarningCallback(void * ctx, void * player, int code, char const * message)
{
	if (code == 201 || code == 408) {
		return;
	}

	if (code == 503) {
		STDString errmsg(message);
		if (errmsg.find("onEventResolution") != std::string::npos
			|| errmsg.find("onEventResize") != std::string::npos) {
			return;
		}
	}

	WARN("Flash: (%d) %s", code, message);
}

bool OsirisProxy::MergeWrapper(std::function<bool (void *, wchar_t *)> const & Next, void * Osiris, wchar_t * Src)
{
	DEBUG("OsirisProxy::MergeWrapper() - Started merge");

	auto cli = GetEoCClient();
	auto srv = GetEoCServer();

	if (ServerExtState) {
		ServerExtState->StorySetMerging(true);
	}

#if !defined(OSI_NO_DEBUGGER)
	if (debugger_ != nullptr) {
		debugger_->MergeStarted();
	}
#endif

	bool retval = Next(Osiris, Src);

#if !defined(OSI_NO_DEBUGGER)
	if (debugger_ != nullptr) {
		debugger_->MergeFinished();
	}
#endif

	if (ServerExtState) {
		ServerExtState->StorySetMerging(false);
	}

	DEBUG("OsirisProxy::MergeWrapper() - Finished merge");
	return retval;
}

void OsirisProxy::RuleActionCall(std::function<void (RuleActionNode *, void *, void *, void *, void *)> const & Next, RuleActionNode * Action, void * a1, void * a2, void * a3, void * a4)
{
#if !defined(OSI_NO_DEBUGGER)
	if (debugger_ != nullptr) {
		debugger_->RuleActionPreHook(Action);
	}
#endif

	Next(Action, a1, a2, a3, a4);

#if !defined(OSI_NO_DEBUGGER)
	if (debugger_ != nullptr) {
		debugger_->RuleActionPostHook(Action);
	}
#endif
}

void OsirisProxy::SaveNodeVMT(NodeType type, NodeVMT * vmt)
{
	assert(type >= NodeType::Database && type <= NodeType::Max);
	NodeVMTs[(unsigned)type] = vmt;
}

void OsirisProxy::ResolveNodeVMTs(NodeDb * Db)
{
	if (ResolvedNodeVMTs) return;

#if 0
	DEBUG("OsirisProxy::ResolveNodeVMTs");
#endif
	std::set<NodeVMT *> VMTs;

	for (unsigned i = 0; i < Db->Db.Size; i++) {
		auto node = Db->Db.Start[i];
		auto vmt = *(NodeVMT **)node;
		VMTs.insert(vmt);
	}

	if (VMTs.size() != (unsigned)NodeType::Max) {
		ERR("Could not locate all Osiris node VMT-s");
		return;
	}

	// RuleNode has a different SetLineNumber implementation
	void * srv{ nullptr };
	std::vector<NodeVMT *> srvA, srvB;
	for (auto vmt : VMTs) {
		if (srv == nullptr) {
			srv = vmt->SetLineNumber;
		}

		if (srv == vmt->SetLineNumber) {
			srvA.push_back(vmt);
		} else {
			srvB.push_back(vmt);
		}
	}

	NodeVMT * ruleNodeVMT;
	if (srvA.size() == 1) {
		ruleNodeVMT = *srvA.begin();
	} else if (srvB.size() == 1) {
		ruleNodeVMT = *srvB.begin();
	} else {
		ERR("Could not locate RuleNode::__vfptr");
		return;
	}

#if 0
	DEBUG("RuleNode::__vfptr is %p", ruleNodeVMT);
#endif
	SaveNodeVMT(NodeType::Rule, ruleNodeVMT);

	// RelOpNode is the only node that has the same GetAdapter implementation
	NodeVMT * relOpNodeVMT{ nullptr };
	for (auto vmt : VMTs) {
		if (vmt->GetAdapter == ruleNodeVMT->GetAdapter
			&& vmt != ruleNodeVMT) {
			if (relOpNodeVMT == nullptr) {
				relOpNodeVMT = vmt;
			} else {
				ERR("RelOpNode::__vfptr pattern matches multiple VMT-s");
				return;
			}
		}
	}

	if (relOpNodeVMT == nullptr) {
		ERR("Could not locate RelOpNode::__vfptr");
		return;
	}

#if 0
	DEBUG("RuleNode::__vfptr is %p", relOpNodeVMT);
#endif
	SaveNodeVMT(NodeType::RelOp, relOpNodeVMT);

	// Find And, NotAnd
	NodeVMT * and1VMT{ nullptr }, *and2VMT{ nullptr };
	for (auto vmt : VMTs) {
		if (vmt->SetNextNode == ruleNodeVMT->SetNextNode
			&& vmt->GetAdapter != ruleNodeVMT->GetAdapter) {
			if (and1VMT == nullptr) {
				and1VMT = vmt;
			} else if (and2VMT == nullptr) {
				and2VMT = vmt;
			} else {
				ERR("AndNode::__vfptr pattern matches multiple VMT-s");
				return;
			}
		}
	}

	if (and1VMT == nullptr || and2VMT == nullptr) {
		ERR("Could not locate AndNode::__vfptr");
		return;
	}

#if 0
	DEBUG("AndNode::__vfptr is %p and %p", and1VMT, and2VMT);
#endif
	// No reliable way to detect these; assume that AndNode VMT < NotAndNode VMT
	if (and1VMT < and2VMT) {
		SaveNodeVMT(NodeType::And, and1VMT);
		SaveNodeVMT(NodeType::NotAnd, and2VMT);
	} else {
		SaveNodeVMT(NodeType::NotAnd, and1VMT);
		SaveNodeVMT(NodeType::And, and2VMT);
	}

	// Find Query nodes
	void * snn{ nullptr };
	std::map<void *, std::vector<NodeVMT *>> snnMaps;
	std::vector<NodeVMT *> * queryVMTs{ nullptr };
	for (auto vmt : VMTs) {
		if (snnMaps.find(vmt->SetNextNode) == snnMaps.end()) {
			std::vector<NodeVMT *> nvmts{ vmt };
			snnMaps.insert(std::make_pair(vmt->SetNextNode, nvmts));
		} else {
			snnMaps[vmt->SetNextNode].push_back(vmt);
		}
	}

	for (auto & map : snnMaps) {
		if (map.second.size() == 3) {
			queryVMTs = &map.second;
			break;
		}
	}

	if (queryVMTs == nullptr) {
		ERR("Could not locate all Query node VMT-s");
		return;
	}

	for (auto vmt : *queryVMTs) {
		auto getName = (NodeVMT::GetQueryNameProc)vmt->GetQueryName;
		std::string name{ getName(nullptr) };
		if (name == "internal query") {
#if 0
			DEBUG("InternalQuery::__vfptr is %p", vmt);
#endif
			SaveNodeVMT(NodeType::InternalQuery, vmt);
		} else if (name == "DIV query") {
#if 0
			DEBUG("DivQuery::__vfptr is %p", vmt);
#endif
			SaveNodeVMT(NodeType::DivQuery, vmt);
		} else if (name == "Osi user query") {
#if 0
			DEBUG("UserQuery::__vfptr is %p", vmt);
#endif
			SaveNodeVMT(NodeType::UserQuery, vmt);
		} else {
			ERR("Unrecognized Query node VMT");
			return;
		}
	}


	// Proc node has different IsProc() code
	NodeVMT * procNodeVMT{ nullptr }, * databaseNodeVMT{ nullptr };
	for (auto vmt : VMTs) {
		if (vmt->IsProc != ruleNodeVMT->IsProc) {
			if (procNodeVMT == nullptr) {
				procNodeVMT = vmt;
			} else {
				ERR("ProcNode::__vfptr pattern matches multiple VMT-s");
				return;
			}
		}

		if (vmt->IsDataNode != ruleNodeVMT->IsDataNode
			&& vmt->IsProc == ruleNodeVMT->IsProc) {
			if (databaseNodeVMT == nullptr) {
				databaseNodeVMT = vmt;
			} else {
				ERR("DatabaseNode::__vfptr pattern matches multiple VMT-s");
				return;
			}
		}
	}

	if (procNodeVMT == nullptr) {
		ERR("Could not locate ProcNode::__vfptr");
		return;
	}

	if (databaseNodeVMT == nullptr) {
		ERR("Could not locate DatabaseNode::__vfptr");
		return;
	}

#if 0
	DEBUG("ProcNode::__vfptr is %p", procNodeVMT);
	DEBUG("DatabaseNode::__vfptr is %p", databaseNodeVMT);
#endif
	SaveNodeVMT(NodeType::Proc, procNodeVMT);
	SaveNodeVMT(NodeType::Database, databaseNodeVMT);
	ResolvedNodeVMTs = true;
}

bool OsirisProxy::HasFeatureFlag(char const * flag) const
{
	return (ServerExtState && ServerExtState->HasFeatureFlag(flag))
		|| (ClientExtState && ClientExtState->HasFeatureFlag(flag));
}

ExtensionStateBase* OsirisProxy::GetCurrentExtensionState()
{
	auto tid = GetCurrentThreadId();
	if (ServerThreadIds.find(tid) != ServerThreadIds.end()) {
		return ServerExtState.get();
	} else if (ClientThreadIds.find(tid) != ClientThreadIds.end()) {
		return ClientExtState.get();
	} else {
		WARN("Called from unknown thread %d?", tid);
		return ClientExtState.get();
	}
}

void OsirisProxy::OnBaseModuleLoaded(void * self)
{
}
/*
void OsirisProxy::OnModuleLoadStarted(TranslatedStringRepository* self)
{
	LoadExtensionStateClient();
	if (ClientExtState) {
		ClientExtState->OnModuleLoadStarted();
	}
}
*/
char const * ClientGameStateNames[] =
{
	"Unknown",
	"Init",
	"InitMenu",
	"InitNetwork",
	"InitConnection",
	"Idle",
	"LoadMenu",
	"Menu",
	"Exit",
	"SwapLevel",
	"LoadLevel",
	"LoadModule",
	"LoadSession",
	"LoadGMCampaign",
	"UnloadLevel",
	"UnloadModule",
	"UnloadSession",
	"Paused",
	"PrepareRunning",
	"Running",
	"Disconnect",
	"Join",
	"Save",
	"StartLoading",
	"StopLoading",
	"StartServer",
	"Movie",
	"Installation",
	"GameMasterPause",
	"ModReceiving",
	"Lobby",
	"BuildStory",
	"LoadLoca"
};


char const * ServerGameStateNames[] =
{
	"Unknown",
	"Uninitialized",
	"Init",
	"Idle",
	"Exit",
	"LoadLevel",
	"LoadModule",
	"LoadGMCampaign",
	"LoadSession",
	"UnloadLevel",
	"UnloadModule",
	"UnloadSession",
	"Sync",
	"Paused",
	"Running",
	"Save",
	"Disconnect",
	"GameMasterPause",
	"BuildStory",
	"ReloadStory",
	"Installation"
};

void OsirisProxy::OnClientGameStateChanged(void * self, ecl::GameState fromState, ecl::GameState toState)
{
	if (config_.SendCrashReports) {
		// We need to initialize the crash reporter after the game engine has started,
		// otherwise the game will overwrite the top level exception filter
		InitCrashReporting();
	}

	// Check to make sure that startup is done even if the extender was loaded when the game was already in GameState::Init
	if (toState != ecl::GameState::Unknown
		&& toState != ecl::GameState::StartLoading
		&& toState != ecl::GameState::InitMenu
		&& !Libraries.InitializationFailed()) {
		// We need to initialize the function library here, as GlobalAllocator isn't available in Init().
		std::lock_guard _(globalStateLock_);
		Libraries.PostStartupFindLibraries();

		if (!Libraries.InitializationFailed() && !functionLibraryInitialized_) {
			CustomInjector.Initialize();
			FunctionLibrary.Register();
			functionLibraryInitialized_ = true;
		}
	}

	if (toState == ecl::GameState::Menu && Libraries.InitializationFailed()) {
		PostInitLibraries();
	}

#if defined(DEBUG_SERVER_CLIENT)
	DEBUG("OsirisProxy::OnClientGameStateChanged(): %s -> %s", 
		ClientGameStateNames[(unsigned)fromState], ClientGameStateNames[(unsigned)toState]);
#endif

	if (!extensionsEnabled_) return;

	if (fromState != ecl::GameState::Unknown) {
		AddClientThread(GetCurrentThreadId());
	}

	switch (fromState) {
	case ecl::GameState::LoadModule:
		INFO("OsirisProxy::OnClientGameStateChanged(): Loaded module");
		LoadExtensionStateClient();
		break;

	case ecl::GameState::LoadSession:
		if (ClientExtState) {
			ClientExtState->OnGameSessionLoaded();
		}
		break;

	case ecl::GameState::InitConnection:
		//networkManager_.ExtendNetworkingClient();
		break;
	}

	switch (toState) {
	case ecl::GameState::LoadModule:
		InitRuntimeLogging();
		break;

	case ecl::GameState::InitNetwork:
	case ecl::GameState::Disconnect:
		//networkManager_.ClientReset();
		break;

	case ecl::GameState::UnloadModule:
		hasher_.ClearCaches();
		break;

	case ecl::GameState::UnloadSession:
		INFO("OsirisProxy::OnClientGameStateChanged(): Unloading session");
		ResetExtensionStateClient();
		break;

	case ecl::GameState::LoadGMCampaign:
		INFO("OsirisProxy::OnClientGameStateChanged(): Loading GM campaign");
		LoadExtensionStateClient();
		//networkManager_.ExtendNetworkingClient();
		break;

	case ecl::GameState::LoadSession:
		INFO("OsirisProxy::OnClientGameStateChanged(): Loading game session");
		LoadExtensionStateClient();
		//networkManager_.ExtendNetworkingClient();
		if (ClientExtState) {
			ClientExtState->OnGameSessionLoading();
		}
		break;
	}

	ecl::LuaClientPin lua(ecl::ExtensionState::Get());
	if (lua) {
		lua->OnGameStateChanged(fromState, toState);
	}
}

void OsirisProxy::OnServerGameStateChanged(void * self, esv::GameState fromState, esv::GameState toState)
{
#if defined(DEBUG_SERVER_CLIENT)
		DEBUG("OsirisProxy::OnServerGameStateChanged(): %s -> %s", 
			ServerGameStateNames[(unsigned)fromState], ServerGameStateNames[(unsigned)toState]);
#endif

	if (fromState != esv::GameState::Unknown) {
		AddServerThread(GetCurrentThreadId());
	}

	if (!extensionsEnabled_) return;

	switch (fromState) {
	case esv::GameState::LoadModule:
		INFO("OsirisProxy::OnServerGameStateChanged(): Loaded module");
		LoadExtensionStateServer();
		break;

	case esv::GameState::LoadSession:
		if (ServerExtState) {
			ServerExtState->OnGameSessionLoaded();
			auto comp = GetServerEntityHelpers().GetComponent<esv::Character>("12345678-1234-1234-1234-123456781234");
		}
		break;
	}

	switch (toState) {
	// Gift Bag selection in the main menu reloads the module without reconnecting
	// We only need to reset the extender enabled peer list on a disconnect.
	case esv::GameState::Disconnect:
		//networkManager_.ServerReset();
		break;

	case esv::GameState::UnloadSession:
		INFO("OsirisProxy::OnServerGameStateChanged(): Unloading session");
		ResetExtensionStateServer();
		break;

	case esv::GameState::LoadModule:
		//networkManager_.ExtendNetworkingServer();
		break;

	case esv::GameState::LoadGMCampaign:
		INFO("OsirisProxy::OnServerGameStateChanged(): Loading GM campaign");
		LoadExtensionStateServer();
		//networkManager_.ExtendNetworkingServer();
		break;

	case esv::GameState::LoadSession:
		INFO("OsirisProxy::OnServerGameStateChanged(): Loading game session");
		LoadExtensionStateServer();
		//networkManager_.ExtendNetworkingServer();
		if (ServerExtState) {
			ServerExtState->OnGameSessionLoading();
		}
		break;
	}

	esv::LuaServerPin lua(esv::ExtensionState::Get());
	if (lua) {
		lua->OnGameStateChanged(fromState, toState);
	}
}

void OsirisProxy::AddClientThread(DWORD threadId)
{
	if (ClientThreadIds.find(threadId) == ClientThreadIds.end()) {
#if defined(DEBUG_SERVER_CLIENT)
		DEBUG("Registered client TID %d", threadId);
#endif
		ClientThreadIds.insert(threadId);
	}
}

void OsirisProxy::AddServerThread(DWORD threadId)
{
	if (ServerThreadIds.find(threadId) == ServerThreadIds.end()) {
#if defined(DEBUG_SERVER_CLIENT)
		DEBUG("Registered server TID %d", threadId);
#endif
		ServerThreadIds.insert(threadId);
	}
}

void OsirisProxy::RemoveClientThread(DWORD threadId)
{
	auto it = ClientThreadIds.find(threadId);
	if (it != ClientThreadIds.end()) {
#if defined(DEBUG_SERVER_CLIENT)
		DEBUG("Unregistered client TID %d", threadId);
#endif
		ClientThreadIds.erase(it);
	}
}

void OsirisProxy::RemoveServerThread(DWORD threadId)
{
	auto it = ServerThreadIds.find(threadId);
	if (it != ServerThreadIds.end()) {
#if defined(DEBUG_SERVER_CLIENT)
		DEBUG("Unregistered server TID %d", threadId);
#endif
		ServerThreadIds.erase(it);
	}
}

void OsirisProxy::OnClientGameStateWorkerStart(void * self)
{
	AddClientThread(GetCurrentThreadId());
}

void OsirisProxy::OnServerGameStateWorkerStart(void * self)
{
	AddServerThread(GetCurrentThreadId());
}

void OsirisProxy::OnClientGameStateWorkerExit(void* self)
{
	RemoveClientThread(GetCurrentThreadId());
}

void OsirisProxy::OnServerGameStateWorkerExit(void* self)
{
	RemoveServerThread(GetCurrentThreadId());
}

void OsirisProxy::OnSkillPrototypeManagerInit(void * self)
{
	if (!extensionsEnabled_) return;

	std::lock_guard _(globalStateLock_);
	auto extState = GetCurrentExtensionState();
	if (extState == nullptr) {
		ERR("Extension not initialized in OnSkillPrototypeManagerInit?");
		return;
	}

	auto modManager = extState->GetModManager();
	if (modManager == nullptr) {
		ERR("Module info not available in OnSkillPrototypeManagerInit?");
		return;
	}

	STDWString loadMsg = L"Loading ";
	loadMsg += modManager->BaseModule.Info.Name;
	loadMsg += L" (Script Extender v";
	loadMsg += std::to_wstring(CurrentVersion);
#if defined(_DEBUG)
	loadMsg += L" Devel";
#endif
	loadMsg += L")";
	Libraries.ShowStartupMessage(loadMsg, false);
	
	if (extState == ServerExtState.get()) {
		LoadExtensionStateServer();
	} else {
		LoadExtensionStateClient();
	}

	extState->OnModuleLoading();
}

void OsirisProxy::ClearPathOverrides()
{
	std::unique_lock lock(pathOverrideMutex_);
	pathOverrides_.clear();
}

void OsirisProxy::AddPathOverride(STDString const & path, STDString const & overriddenPath)
{
	auto absolutePath = GetStaticSymbols().ToPath(path, PathRootType::Data);
	auto absoluteOverriddenPath = GetStaticSymbols().ToPath(overriddenPath, PathRootType::Data);

	std::unique_lock lock(pathOverrideMutex_);
	pathOverrides_.insert(std::make_pair(absolutePath, absoluteOverriddenPath));
}

bool OsirisProxy::IsInServerThread() const
{
	auto tid = GetCurrentThreadId();
	return ServerThreadIds.find(tid) != ServerThreadIds.end();
}

bool OsirisProxy::IsInClientThread() const
{
	auto tid = GetCurrentThreadId();
	return ClientThreadIds.find(tid) != ClientThreadIds.end();
}

void OsirisProxy::AttachConsoleThread(bool server)
{
	auto tid = GetCurrentThreadId();
	if (server) {
		ServerThreadIds.insert(tid);
		auto it = ClientThreadIds.find(tid);
		if (it != ClientThreadIds.end()) {
			ClientThreadIds.erase(it);
		}
	} else {
		ClientThreadIds.insert(tid);
		auto it = ServerThreadIds.find(tid);
		if (it != ServerThreadIds.end()) {
			ServerThreadIds.erase(it);
		}
	}
}

void OsirisProxy::ResetLuaState(bool resetServer, bool resetClient)
{
	if (resetServer && ServerExtState && ServerExtState->GetLua()) {
		auto& ext = *ServerExtState;
		ext.AddPostResetCallback([&ext]() {
			ext.OnModuleResume();
			auto state = GetStaticSymbols().GetServerState();
			if (state && (state == esv::GameState::Paused || state == esv::GameState::Running || state == esv::GameState::GameMasterPause)) {
				ext.OnGameSessionLoading();
				ext.OnGameSessionLoaded();
				ext.StoryLoaded();
				ext.OnResetCompleted();
			}
		});
		ext.LuaReset(true);
	}

	if (resetClient) {
		auto server = GetEoCServer();
		if (server && server->GameServer && false /* networking not available yet! */) {
			// Reset clients via a network message if the server is running
			/*auto& networkMgr = gOsirisProxy->GetNetworkManager();
			auto msg = networkMgr.GetFreeServerMessage(ReservedUserId);
			if (msg != nullptr) {
				auto resetMsg = msg->GetMessage().mutable_s2c_reset_lua();
				resetMsg->set_bootstrap_scripts(true);
				networkMgr.ServerBroadcast(msg, ReservedUserId);
			} else {
				OsiErrorS("Could not get free message!");
			}*/

		} else if (ClientExtState && ClientExtState->GetLua()) {
			bool serverThread = IsInServerThread();
			AttachConsoleThread(false);

			// Do a direct (local) reset if server is not available (main menu, etc.)
			auto& ext = *ClientExtState;
			ext.AddPostResetCallback([&ext]() {
				ext.OnModuleResume();
				auto state = GetStaticSymbols().GetClientState();
				if (state && (state == ecl::GameState::Paused || state == ecl::GameState::Running || state == ecl::GameState::GameMasterPause)) {
					ext.OnGameSessionLoading();
					ext.OnGameSessionLoaded();
					ext.OnResetCompleted();
				}
			});
			ext.LuaReset(true);

			AttachConsoleThread(serverThread);
		}
	}
}

FileReader * OsirisProxy::OnFileReaderCreate(FileReader::CtorProc* next, FileReader * self, Path const& path, unsigned int type, unsigned int unknown)
{
	if (!pathOverrides_.empty()) {
		std::shared_lock lock(pathOverrideMutex_);
		auto it = pathOverrides_.find(path.Name);
		if (it != pathOverrides_.end() && !hasher_.isHashing()) {
			DEBUG("FileReader path override: %s -> %s", path.Name.c_str(), it->second.c_str());
			Path overriddenPath;
			overriddenPath.Name = it->second;
#if !defined(OSI_EOCAPP)
			overriddenPath.Unknown = path->Unknown;
#endif
			lock.unlock();
			return next(self, overriddenPath, type, unknown);
		}
	}

	if (path.Name.size() > 4 
		&& memcmp(path.Name.data() + path.Name.size() - 4, ".txt", 4) == 0) {
		statLoadOrderHelper_.OnStatFileOpened(path);
	}

	DisableCrashReporting _;
	return next(self, path, type, unknown);
}

void OsirisProxy::OnSavegameVisit(void* osirisHelpers, ObjectVisitor* visitor)
{
	savegameSerializer_.SavegameVisit(visitor);
}

NewHit* OnDealDamage(DealDamageFunctor::ApplyDamageProc* next, NewHit* result, DealDamageFunctor* functor, EntityWorldHandle* casterHandle, EntityWorldHandle* targetHandle, glm::vec3* position, bool isFromItem, SpellIdWithPrototype* spellId, int storyActionId, ActionOriginator* originator, GuidResourceDefinitionManagerBase* classResourceMgr, Hit* hit, DamageSums* damageSums, HitWith hitWith)
{
	auto ret = next(result, functor, casterHandle, targetHandle, position, isFromItem, spellId, storyActionId, originator, classResourceMgr, hit, damageSums, hitWith);
	return ret;
}

void OnStatsFunctorExecuteType1(StatsFunctorSet::ExecuteType1Proc* next, Hit* hit, StatsFunctorSet* self, FunctorExecParamsType1* params)
{
	next(hit, self, params);
}

void OsirisProxy::PostInitLibraries()
{
	std::lock_guard _(globalStateLock_);
	if (LibrariesPostInitialized) return;

	if (extensionsEnabled_) {
		if (Libraries.PostStartupFindLibraries()) {
			clientEntityHelpers_.Setup();
			serverEntityHelpers_.Setup();
			Wrappers.InitializeDeferredExtensions();
			FunctionLibrary.PostStartup();
			//hitProxy_.PostStartup();
			hasher_.PostStartup();

			using namespace std::placeholders;
			Libraries.FileReader__ctor.SetWrapper(std::bind(&OsirisProxy::OnFileReaderCreate, this, _1, _2, _3, _4, _5));
			Libraries.DealDamageFunctor__ApplyDamage.SetWrapper(std::bind(&OnDealDamage, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14));
			Libraries.StatsFunctorSet__ExecuteType1.SetWrapper(std::bind(&OnStatsFunctorExecuteType1, _1, _2, _3, _4));
		}
	}

	GameVersionInfo gameVersion;
	if (Libraries.GetGameVersion(gameVersion) && !gameVersion.IsSupported()) {
		std::wstringstream ss;
		ss << L"Your game version (v" << gameVersion.Major << L"." << gameVersion.Minor << L"." << gameVersion.Revision << L"." << gameVersion.Build
			<< L") is not supported by the Script Extender; please upgrade to at least v3.6.54";
		Libraries.ShowStartupError(ss.str().c_str(), true, false);
	} else if (Libraries.CriticalInitializationFailed()) {
		Libraries.ShowStartupError(L"A severe error has occurred during Script Extender initialization. Extension features will be unavailable.", true, false);
	} else if (Libraries.InitializationFailed()) {
		Libraries.ShowStartupError(L"An error has occurred during Script Extender initialization. Some extension features might be unavailable.", true, false);
	}

	LibrariesPostInitialized = true;
}

void OsirisProxy::InitRuntimeLogging()
{
	if (!config_.LogRuntime) return;

	auto path = MakeLogFilePath(L"Extender Runtime", L"log");
	gConsole.OpenLogFile(path);
	DEBUG(L"Extender runtime log written to '%s'", path.c_str());
}

void OsirisProxy::ResetExtensionStateServer()
{
	std::lock_guard _(globalStateLock_);
	ServerExtState = std::make_unique<esv::ExtensionState>();
	ServerExtState->Reset();
	ServerExtensionLoaded = false;
}

extern std::unordered_map<int32_t*, char*> maps;

void OsirisProxy::LoadExtensionStateServer()
{
	std::lock_guard _(globalStateLock_);
	if (ServerExtensionLoaded) return;
	
	if (extensionsEnabled_) {
		if (Libraries.PostStartupFindLibraries()) {
			FunctionLibrary.PostStartup();
			//hitProxy_.PostStartup();
		}

		if (!ServerExtState) {
			ResetExtensionStateServer();
		}

		ServerExtState->LoadConfigs();
	}

	// PostInitLibraries() should be called after extension config is loaded,
	// otherwise it won't hook functions that may be needed later on
	PostInitLibraries();

	if (extensionsEnabled_ && !Libraries.CriticalInitializationFailed()) {
		//networkManager_.ExtendNetworkingServer();
		FunctionLibrary.OnBaseModuleLoadedServer();
	}

	ServerExtensionLoaded = true;
}

void OsirisProxy::ResetExtensionStateClient()
{
	std::lock_guard _(globalStateLock_);
	ClientExtState = std::make_unique<ecl::ExtensionState>();
	ClientExtState->Reset();
	ClearPathOverrides();
	ClientExtensionLoaded = false;
}

void OsirisProxy::LoadExtensionStateClient()
{
	std::lock_guard _(globalStateLock_);
	if (ClientExtensionLoaded) return;

	if (extensionsEnabled_) {
		if (Libraries.PostStartupFindLibraries()) {
			FunctionLibrary.PostStartup();
			//hitProxy_.PostStartup();
		}

		if (!ClientExtState) {
			ResetExtensionStateClient();
		}

		ClientExtState->LoadConfigs();
	}

	// PostInitLibraries() should be called after extension config is loaded,
	// otherwise it won't hook functions that may be needed later on
	PostInitLibraries();

	if (extensionsEnabled_ && !Libraries.CriticalInitializationFailed()) {
		//networkManager_.ExtendNetworkingClient();
		FunctionLibrary.OnBaseModuleLoadedClient();
	}

	ClientExtensionLoaded = true;
}


void SavegameSerializer::SavegameVisit(ObjectVisitor* visitor)
{
	if (visitor->EnterRegion(GFS.strScriptExtenderSave)) {
		uint32_t version = SavegameVersion;
		visitor->VisitUInt32(GFS.strExtenderVersion, version, 0);
		if (visitor->IsReading()) {
			if (version > SavegameVersion) {
				ERR("Savegame version too new! Extender version %d, savegame version %d; savegame data will not be loaded!", SavegameVersion, version);
				std::wstringstream ss;
				ss << "Could not load Script Extender save data - savegame is newer than the currently installed extender!<br>";
				ss << "Extender version v" << SavegameVersion << ", savegame version v" << version;
				gOsirisProxy->GetLibraryManager().ShowStartupError(ss.str().c_str(), true, false);
			}
			else {
				Serialize(visitor, version);
			}
		}
		else {
			Serialize(visitor, SavegameVersion);
		}

		visitor->ExitRegion(GFS.strScriptExtenderSave);
	}
}


void SavegameSerializer::Serialize(ObjectVisitor* visitor, uint32_t version)
{
	SerializePersistentVariables(visitor, version);
	SerializeStatObjects(visitor, version);
}


void SavegameSerializer::SerializePersistentVariables(ObjectVisitor* visitor, uint32_t version)
{
	STDString nullStr;
	if (visitor->EnterNode(GFS.strLuaVariables, GFS.strEmpty)) {
		auto const& configs = gOsirisProxy->GetServerExtensionState().GetConfigs();

		if (visitor->IsReading()) {
			std::unordered_map<FixedString, STDString> variables;
			uint32_t numMods{ 0 };
			visitor->VisitCount(GFS.strMod, &numMods);

			for (uint32_t i = 0; i < numMods; i++) {
				if (visitor->EnterNode(GFS.strMod, GFS.strModId)) {
					FixedString modId;
					visitor->VisitFixedString(GFS.strModId, modId, GFS.strEmpty);
					STDString modVars;
					visitor->VisitSTDString(GFS.strLuaVariables, modVars, nullStr);

					variables.insert(std::make_pair(modId, modVars));
					visitor->ExitNode(GFS.strMod);
				}
			}

			RestorePersistentVariables(variables);
		} else {
			auto& state = gOsirisProxy->GetServerExtensionState();
			auto mods = state.GetPersistentVarMods();

			for (auto const& modId : mods) {
				auto vars = state.GetModPersistentVars(modId);
				if (vars) {
					if (visitor->EnterNode(GFS.strMod, GFS.strModId)) {
						FixedString modIdTemp = modId;
						visitor->VisitFixedString(GFS.strModId, modIdTemp, GFS.strEmpty);
						visitor->VisitSTDString(GFS.strLuaVariables, *vars, nullStr);
						visitor->ExitNode(GFS.strMod);
					}
				}
			}
		}

		visitor->ExitNode(GFS.strLuaVariables);
	}
}

void SavegameSerializer::RestorePersistentVariables(std::unordered_map<FixedString, STDString> const& variables)
{
	auto& state = gOsirisProxy->GetServerExtensionState();
	for (auto const& var : variables) {
		state.RestoreModPersistentVars(var.first, var.second);
	}
}


void SavegameSerializer::SerializeStatObjects(ObjectVisitor* visitor, uint32_t version)
{
	STDString nullStr;
	if (visitor->EnterNode(GFS.strDynamicStats, GFS.strEmpty)) {
		if (visitor->IsReading()) {
			std::unordered_map<FixedString, STDString> variables;
			uint32_t numObjects{ 0 };
			visitor->VisitCount(GFS.strStatObject, &numObjects);

			for (uint32_t i = 0; i < numObjects; i++) {
				if (visitor->EnterNode(GFS.strStatObject, GFS.strStatId)) {
					FixedString statId, statType;
					ScratchBuffer blob;
					visitor->VisitFixedString(GFS.strStatId, statId, GFS.strEmpty);
					visitor->VisitFixedString(GFS.strStatType, statType, GFS.strEmpty);
					visitor->VisitBuffer(GFS.strBlob, blob);
					RestoreStatObject(statId, statType, blob);
					visitor->ExitNode(GFS.strStatObject);
				}
			}
		} else {
			auto const& statIds = gOsirisProxy->GetServerExtensionState().GetPersistentStats();

			for (auto statId : statIds) {
				FixedString statType;
				ScratchBuffer blob;
				if (SerializeStatObject(statId, statType, blob)) {
					if (visitor->EnterNode(GFS.strStatObject, GFS.strStatId)) {
						visitor->VisitFixedString(GFS.strStatId, statId, GFS.strEmpty);
						visitor->VisitFixedString(GFS.strStatType, statType, GFS.strEmpty);
						visitor->VisitBuffer(GFS.strBlob, blob);
						visitor->ExitNode(GFS.strStatObject);
					}
				}
			}
		}

		visitor->ExitNode(GFS.strDynamicStats);
	}
}

void SavegameSerializer::RestoreStatObject(FixedString const& statId, FixedString const& statType, ScratchBuffer const& blob)
{
	auto stats = GetStaticSymbols().GetStats();

	auto object = stats->Objects.Find(statId);
	if (object) {
		auto modifier = stats->ModifierLists.Find(object->ModifierListIndex);
		if (modifier->Name != statType) {
			OsiError("Stat entry '" << statId << "' is a '" << statType << "' in the save, but a '" 
				<< modifier->Name << "' in the game. It will not be loaded from the savegame!");
			return;
		}
	} else {
		auto newObject = stats->CreateObject(statId, statType);
		if (!newObject) {
			OsiError("Couldn't construct stats entry '" << statId << "' of type '" << statType 
				<< "'! It will not be loaded from the savegame!");
			return;
		}
		object = *newObject;
	}
	/*
#if defined(NDEBUG)
	MsgS2CSyncStat msg;
#else
	// Workaround for different debug/release CRT runtimes between protobuf and the extender in debug mode
	MsgS2CSyncStat& msg = *GameAlloc<MsgS2CSyncStat>();
#endif
	if (!msg.ParseFromArray(blob.Buffer, blob.Size)) {
		OsiError("Unable to parse protobuf payload for stat '" << statId << "'! It will not be loaded from the savegame!");
		return;
	}

	object->FromProtobuf(msg);
	stats->SyncWithPrototypeManager(object);*/
	object->BroadcastSyncMessage(true);
	gOsirisProxy->GetServerExtensionState().MarkDynamicStat(statId);
	gOsirisProxy->GetServerExtensionState().MarkPersistentStat(statId);
}

bool SavegameSerializer::SerializeStatObject(FixedString const& statId, FixedString& statType, ScratchBuffer& blob)
{
	auto stats = GetStaticSymbols().GetStats();

	auto object = stats->Objects.Find(statId);
	if (!object) {
		OsiError("Stat entry '" << statId << "' is marked as modified but cannot be found! It will not be written to the savegame!");
		return false;
	}

	auto modifier = stats->ModifierLists.Find(object->ModifierListIndex);
	statType = modifier->Name;

	/*MsgS2CSyncStat msg;
	object->ToProtobuf(&msg);
	uint32_t size = (uint32_t)msg.ByteSizeLong();
	blob.Size = size;
	blob.Buffer = GameAllocRaw(size);
	msg.SerializeToArray(blob.Buffer, size);*/
	return true;
}


void StatLoadOrderHelper::OnLoadStarted()
{
	loadingStats_ = true;
	statLastTxtMod_ = FixedString{};
	statsEntryToModMap_.clear();
	UpdateModDirectoryMap();
}

void StatLoadOrderHelper::OnLoadFinished()
{
	OnStatFileOpened();
	loadingStats_ = false;
}

void StatLoadOrderHelper::UpdateModDirectoryMap()
{
	std::unique_lock _(modMapMutex_);
	modDirectoryToModMap_.clear();

	/*auto modManager = GetModManagerClient();
	if (modManager) {
		for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
			auto directory = ToUTF8(mod.Info.Directory);
			modDirectoryToModMap_.insert(std::make_pair(directory, mod.Info.ModuleUUID));
		}
	}*/
}

void StatLoadOrderHelper::OnStatFileOpened()
{
	/*auto stats = GetStaticSymbols().GetStats();
	auto const& bufs = stats->PreParsedDataBufferMap;
	bufs.Iterate([this, stats](auto const& key, auto const& preParseBufIdx) {
		auto preParseBuf = stats->PreParsedDataBuffers[(uint32_t)preParseBufIdx];
		auto entry = statsEntryToModMap_.find(key);
		if (entry == statsEntryToModMap_.end()) {
			StatsEntryModMapping mapping;
			mapping.Mod = statLastTxtMod_;
			mapping.PreParseBuf = preParseBuf;
			statsEntryToModMap_.insert(std::make_pair(key, mapping));
		} else if (entry->second.PreParseBuf != preParseBuf) {
			entry->second.Mod = statLastTxtMod_;
			entry->second.PreParseBuf = preParseBuf;
		}
	});*/
}

static std::regex sStatPathRegex(".*/Public/(.*)/Stats/Generated/.*.txt$");

void StatLoadOrderHelper::OnStatFileOpened(Path const& path)
{
	if (!loadingStats_) return;

	std::cmatch match;
	if (std::regex_match(path.Name.c_str(), match, sStatPathRegex)) {
		std::unique_lock lock(modMapMutex_);

		auto modIt = modDirectoryToModMap_.find(match[1].str().c_str());
		if (modIt != modDirectoryToModMap_.end()) {
			statLastTxtMod_ = modIt->second;
			OnStatFileOpened();
		} else {
			WARN("Unable to resolve mod while loading stats .txt: %s", path.Name.c_str());
		}
	}
}

FixedString StatLoadOrderHelper::GetStatsEntryMod(FixedString statId) const
{
	auto entryIt = statsEntryToModMap_.find(statId);
	if (entryIt != statsEntryToModMap_.end()) {
		return entryIt->second.Mod;
	} else {
		return {};
	}
}

std::vector<CRPGStats_Object*> StatLoadOrderHelper::GetStatsLoadedBefore(FixedString modId) const
{
	std::unordered_set<FixedString> modsLoadedBefore;
	auto state = gOsirisProxy->GetCurrentExtensionState();
	if (!state) return {};

	bool modIdFound{ false };
	for (auto const& mod : state->GetModManager()->BaseModule.LoadOrderedModules) {
		modsLoadedBefore.insert(mod.Info.ModuleUUIDString);
		if (mod.Info.ModuleUUIDString == modId) {
			modIdFound = true;
			break;
		}
	}

	if (!modIdFound) {
		OsiError("Couldn't fetch stat entry list - mod " << modId << " is not loaded.");
		return {};
	}

	std::vector<CRPGStats_Object*> statsLoadedBefore;
	auto stats = GetStaticSymbols().GetStats();
	for (auto const& object : stats->Objects.Primitives) {
		auto statEntryMod = GetStatsEntryMod(object->Name);
		if (statEntryMod && modsLoadedBefore.find(statEntryMod) != modsLoadedBefore.end()) {
			statsLoadedBefore.push_back(object);
		}
	}

	return statsLoadedBefore;
}

__declspec(thread) unsigned ModuleHasher::hashDepth_{ 0 };

void ModuleHasher::PostStartup()
{
	using namespace std::placeholders;
	/*gOsirisProxy->GetLibraryManager().Module__Hash.SetWrapper(
		std::bind(&ModuleHasher::OnModuleHash, this, _1, _2)
	);*/
}

void ModuleHasher::ClearCaches()
{
	std::lock_guard _(mutex_);
	hashCache_.clear();
}

bool ModuleHasher::FetchHashFromCache(Module& mod)
{
	auto it = hashCache_.find(mod.Info.ModuleUUIDString);
	if (it != hashCache_.end()) {
		mod.Info.Hash = it->second;
		mod.HasValidHash = true;
		UpdateDependencyHashes(mod);
		return true;
	} else {
		return false;
	}
}

void ModuleHasher::UpdateDependencyHashes(Module& mod)
{
	/*for (auto& dependency : mod.DependentModules) {
		if (!FetchHashFromCache(dependency)) {
			GetStaticSymbols().Module__Hash(&dependency);
		}
	}

	for (auto& addon : mod.AddonModules) {
		if (!FetchHashFromCache(addon)) {
			GetStaticSymbols().Module__Hash(&addon);
		}
	}*/
}

bool ModuleHasher::OnModuleHash(Module::HashProc* next, Module* self)
{
	std::lock_guard _(mutex_);
	
	if (FetchHashFromCache(*self)) {
		return true;
	}

	if (!hashStack_.empty()) {
		auto mod = *hashStack_.rbegin();
		if (!mod->Info.Hash.empty()) {
			hashCache_.insert(std::make_pair(mod->Info.ModuleUUIDString, mod->Info.Hash));
		}
	}

	hashStack_.push_back(self);

	hashDepth_++;
	auto ok = next(self);
	hashDepth_--;
	if (!self->Info.Hash.empty()) {
		hashCache_.insert(std::make_pair(self->Info.ModuleUUIDString, self->Info.Hash));
	}

	hashStack_.pop_back();
	return ok;
}

}
