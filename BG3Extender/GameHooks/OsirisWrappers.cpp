#include "stdafx.h"
#include <Extender/ScriptExtender.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <psapi.h>

namespace bg3se
{

#define STATIC_HOOK(name) decltype(OsirisWrappers::name) * decltype(OsirisWrappers::name)::gHook;

STATIC_HOOK(RegisterDivFunctions)
STATIC_HOOK(InitGame)
STATIC_HOOK(DeleteAllData)
STATIC_HOOK(GetFunctionMappings)
STATIC_HOOK(OpenLogFile)
STATIC_HOOK(CloseLogFile)
STATIC_HOOK(Compile)
STATIC_HOOK(Load)
STATIC_HOOK(Merge)
STATIC_HOOK(Event)
STATIC_HOOK(RuleActionCall)
STATIC_HOOK(Call)
STATIC_HOOK(Query)
STATIC_HOOK(Error)
STATIC_HOOK(Assert)
STATIC_HOOK(CreateFileW)
STATIC_HOOK(CloseHandle)


OsirisWrappers::OsirisWrappers()
{}

void OsirisWrappers::RegisterDIVFunctionsPreHook(void * Osiris, DivFunctions * Functions)
{
	CallOriginal = Functions->Call;
	QueryOriginal = Functions->Query;
	ErrorOriginal = Functions->ErrorMessage;
	AssertOriginal = Functions->Assert;

	Functions->Call = &CallWrapper;
	Functions->Query = &QueryWrapper;
	Functions->ErrorMessage = &ErrorWrapper;
	Functions->Assert = &AssertWrapper;
}

void OsirisWrappers::Initialize()
{
	Kernel32Module = LoadLibrary(L"kernel32.dll");
	if (Kernel32Module == NULL) {
		Fail("Could not load kernel32.dll");
	}

	OsirisModule = LoadLibrary(L"osiris.dll");
	if (OsirisModule == NULL) {
		Fail("Could not load osiris.dll");
	}

	MODULEINFO moduleInfo;
	if (!GetModuleInformation(GetCurrentProcess(), OsirisModule, &moduleInfo, sizeof(moduleInfo))) {
		Fail("Could not get module info of osiris.dll");
	}

	OsirisDllStart = moduleInfo.lpBaseOfDll;
	OsirisDllSize = moduleInfo.SizeOfImage;

	OriginalRuleActionCallProc = (RuleActionCallProc)FindRuleActionCallProc();
	if (!OriginalRuleActionCallProc) {
		ERR("Could not locate RuleAction::Call in osiris.dll");
	}

	FARPROC OsirisCtorProc = GetProcAddress(OsirisModule, "??0COsiris@@QEAA@XZ");
	if (OsirisCtorProc == NULL) {
		Fail("Could not locate COsiris::COsiris() in osiris.dll");
	}

	FindOsirisGlobals(OsirisCtorProc);

	FARPROC SetOptionProc = GetProcAddress(OsirisModule, "?SetOption@COsiris@@QEAAXI@Z");
	if (SetOptionProc == NULL) {
		Fail("Could not locate COsiris::SetOption in osiris.dll");
	}

	FindDebugFlags(SetOptionProc);

#if 0
	DEBUG("OsirisWrappers::Initialize: Detouring functions");
#endif

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	RegisterDivFunctions.Wrap(OsirisModule, "?RegisterDIVFunctions@COsiris@@QEAAXPEAUTOsirisInitFunction@@@Z");
	InitGame.Wrap(OsirisModule, "?InitGame@COsiris@@QEAA_NXZ");
	DeleteAllData.Wrap(OsirisModule, "?DeleteAllData@COsiris@@IEAAX_N@Z");
	GetFunctionMappings.Wrap(OsirisModule, "?GetFunctionMappings@COsiris@@QEAAXPEAPEAUMappingInfo@@PEAI@Z");
	OpenLogFile.Wrap(OsirisModule, "?OpenLogFile@COsiris@@QEAA_NPEB_WH@Z");
	CloseLogFile.Wrap(OsirisModule, "?CloseLogFile@COsiris@@QEAAXXZ");
	Load.Wrap(OsirisModule, "?Load@COsiris@@QEAA_NAEAVCOsiSmartBuf@@@Z");
	Compile.Wrap(OsirisModule, "?Compile@COsiris@@QEAA_NPEB_W0@Z");
	Merge.Wrap(OsirisModule, "?Merge@COsiris@@QEAA_NPEB_W@Z");
	Event.Wrap(OsirisModule, "?Event@COsiris@@QEAA?AW4ReturnCode@osi@@IPEAVCOsiArgumentDesc@@@Z");
	if (OriginalRuleActionCallProc != nullptr) {
		RuleActionCall.Wrap(OriginalRuleActionCallProc);
	}

	Call.Wrap(&CallWrapper);
	Query.Wrap(&QueryWrapper);
	Error.Wrap(&ErrorWrapper);
	// FIXME - PERF Assert.Wrap(&AssertWrapper);

	CreateFileW.Wrap(Kernel32Module, "CreateFileW");
	CloseHandle.Wrap(Kernel32Module, "CloseHandle");

	DetourTransactionCommit();
}

void OsirisWrappers::Shutdown()
{
#if 0
	DEBUG("OsirisWrappers::Shutdown: Unregistering hooks");
#endif
	// Temporarily disabled until we figure out why detour detach fails
	/*DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	RegisterDivFunctions.Unwrap();
	InitGame.Unwrap();
	DeleteAllData.Unwrap();
	GetFunctionMappings.Unwrap();
	OpenLogFile.Unwrap();
	CloseLogFile.Unwrap();
	Load.Unwrap();
	Compile.Unwrap();
	Merge.Unwrap();
	Event.Unwrap();
	RuleActionCall.Unwrap();

	Call.Unwrap();
	Query.Unwrap();
	Error.Unwrap();
	Assert.Unwrap();

	CreateFileW.Unwrap();
	CloseHandle.Unwrap();

	DetourTransactionCommit();*/

	FreeModule(Kernel32Module);
	FreeModule(OsirisModule);
}

bool OsirisWrappers::CallWrapper(uint32_t FunctionId, OsiArgumentDesc * Params)
{
	auto& osi = gExtender->GetServer().Osiris();

	auto wrappers = osi.GetVMTWrappers();
	if (wrappers && wrappers->OsirisCallbacksAttachment) {
		wrappers->OsirisCallbacksAttachment->CallPreHook(FunctionId, Params);
	}

	bool succeeded = gExtender->GetServer().Osiris().GetWrappers().CallOriginal(FunctionId, Params);

	if (wrappers && wrappers->OsirisCallbacksAttachment) {
		wrappers->OsirisCallbacksAttachment->CallPostHook(FunctionId, Params, succeeded);
	}

	return succeeded;
}

bool OsirisWrappers::QueryWrapper(uint32_t FunctionId, OsiArgumentDesc * Params)
{
	return gExtender->GetServer().Osiris().GetWrappers().QueryOriginal(FunctionId, Params);
}

void OsirisWrappers::ErrorWrapper(char const * Message)
{
	gExtender->GetServer().Osiris().GetWrappers().ErrorOriginal(Message);
}

void OsirisWrappers::AssertWrapper(bool Successful, char const * Message, bool Unknown2)
{
	gExtender->GetServer().Osiris().GetWrappers().AssertOriginal(Successful, Message, Unknown2);
}

void * OsirisWrappers::FindRuleActionCallProc()
{
#if 0
	DEBUG("OsirisWrappers::FindRuleActionCallProc");
#endif
	uint8_t * Addr = static_cast<uint8_t *>(OsirisDllStart);

	// Function prologue of RuleAction::Call
	static const uint8_t instructions[42] = {
		0x48, 0x89, 0x5c, 0x24, 0x10, // mov     [rsp-28h+arg_8], rbx
		0x48, 0x89, 0x74, 0x24, 0x18, // mov     [rsp-28h+arg_10], rsi
		0x48, 0x89, 0x7c, 0x24, 0x20, // mov     [rsp-28h+arg_18], rdi
		0x55, // push    rbp
		0x41, 0x54, // push    r12
		0x41, 0x55, // push    r13
		0x41, 0x56, // push    r14
		0x41, 0x57, // push    r15
		0x48, 0x8b, 0xec, // mov     rbp, rsp
		0x48, 0x81, 0xec, 0x80, 0x00, 0x00, 0x00, // sub     rsp, 80h
		0x48, 0x8b, 0xf1, // mov     rsi, rcx
		0x8b, 0x49, 0x14, // mov     ecx, [rcx+14h]
		0x85, 0xc9, // test    ecx, ecx
	};

	// Look for prologue in the entire osiris DLL
	for (uint8_t * ptr = Addr; ptr < Addr + OsirisDllSize - sizeof(instructions); ptr++)
	{
		if (*reinterpret_cast<uint64_t *>(ptr) == *reinterpret_cast<uint64_t const *>(&instructions[0])
			&& memcmp(instructions, ptr, sizeof(instructions)) == 0)
		{
			return ptr;
		}
	}

	return nullptr;
}

void const* ResolveRealFunctionAddress(void const * ptr)
{
	auto p = (uint8_t const*)ptr;

	// Unconditional jump
	if (p[0] == 0xE9) {
		int32_t relOffset = *reinterpret_cast<int32_t const *>(p + 1);
		return p + relOffset + 5;
	}

	// Resolve function pointer through relocations
	auto end = p + 64;
	for (; p < end; p++)
	{
		// Look for the instruction "cmp qword ptr [rip+xxxxxx], 0"
		if (p[0] == 0x48 && p[1] == 0x83 && p[2] == 0x3d && p[6] == 0x00 &&
			// Look for the instruction "jmp xxxx"
			p[13] == 0xe9)
		{
			int32_t relOffset = *reinterpret_cast<int32_t const *>(p + 14);
			return p + relOffset + 18;
		}
	}

	// Could not find any relocations
	return ptr;
}

void OsirisWrappers::FindOsirisGlobals(FARPROC CtorProc)
{
#if 0
	DEBUG("ScriptExtender::FindOsirisGlobals:");
#endif
	auto addr = (uint8_t const *)ResolveRealFunctionAddress((void const *)CtorProc);

	// Try to find pointers of Osiris globals
	const unsigned NumGlobals = 9;
	uint8_t * globals[NumGlobals];
	unsigned foundGlobals = 0;
	for (auto ptr = addr; ptr < addr + 0x500; ptr++)
	{
		// Look for the instruction "mov <reg>, r14"
		if ((ptr[0] == 0x49 || ptr[0] == 0x48) && ptr[1] == 0x8B &&
			// Look for the instruction "mov cs:[rip + xxx], <64-bit register>"
			ptr[3] == 0x48 && ptr[4] == 0x89 && (ptr[5] & 0xC7) == 0x05)
		{
			int32_t relOffset = *reinterpret_cast<int32_t const *>(ptr + 6);
			uint64_t osiPtr = (uint64_t)ptr + relOffset + 10;
			globals[foundGlobals++] = (uint8_t *)osiPtr;
			if (foundGlobals == NumGlobals) break;
		}
	}

	if (foundGlobals < NumGlobals)
	{
		Fail("Could not locate global Osiris variables");
	}

	Globals.Variables = (VariableDb **)globals[0];
	Globals.Types = (OsiTypeDb **)globals[1];
	Globals.Enums = (EnumDb**)globals[2];
	Globals.Functions = (FunctionDb **)globals[3];
	Globals.Objects = (ObjectDb **)globals[4];
	Globals.Goals = (GoalDb **)globals[5];
	Globals.Adapters = (AdapterDb **)globals[6];
	Globals.Databases = (DatabaseDb **)globals[7];
	Globals.Nodes = (NodeDb **)globals[8];

#if 0
	DEBUG("\tVariables = %p", Globals.Variables);
	DEBUG("\tTypes = %p", Globals.Types);
	DEBUG("\tFunctions = %p", Globals.Functions);
	DEBUG("\tObjects = %p", Globals.Objects);
	DEBUG("\tGoals = %p", Globals.Goals);
	DEBUG("\tAdapters = %p", Globals.Adapters);
	DEBUG("\tDatabases = %p", Globals.Databases);
	DEBUG("\tNodes = %p", Globals.Nodes);
#endif
}

void OsirisWrappers::FindDebugFlags(FARPROC SetOptionProc)
{
	auto addr = (uint8_t const*)ResolveRealFunctionAddress((void const *)SetOptionProc);

	// Try to find pointer of global var DebugFlags
	for (auto ptr = addr; ptr < addr + 0x80; ptr++)
	{
		// Look for the instruction "mov ecx, cs:xxx"
		if (ptr[0] == 0x8B && ptr[1] == 0x0D &&
			// Look for the instruction "shr e*x, 14h"
			ptr[8] == 0xC1 && ptr[10] == 0x14)
		{
			int32_t relOffset = *reinterpret_cast<int32_t const *>(ptr + 2);
			uint64_t dbgPtr = (uint64_t)ptr + relOffset + 6;
			Globals.DebugFlags = (DebugFlag *)dbgPtr;
			break;
		}
	}

	if (Globals.DebugFlags == nullptr) {
		Fail("Could not locate global variable DebugFlags");
	}

#if 0
	DEBUG("ScriptExtender::FindDebugFlags: DebugFlags = %p", Globals.DebugFlags);
#endif
}

void OsirisWrappers::SaveNodeVMT(NodeType type, NodeVMT * vmt)
{
	assert(type >= NodeType::Database && type <= NodeType::Max);
	VMTs[(unsigned)type] = vmt;
}

bool OsirisWrappers::ResolveNodeVMTs()
{
	if (!resolvedVMTs_ || !*resolvedVMTs_) {
		resolvedVMTs_ = ResolveNodeVMTsInternal();
	}

	return *resolvedVMTs_;
}


bool OsirisWrappers::ResolveNodeVMTsInternal()
{
	auto Db = *Globals.Nodes;

#if 0
	DEBUG("ScriptExtender::ResolveNodeVMTs");
#endif
	std::set<NodeVMT *> VMTs;

	for (unsigned i = 0; i < Db->Db.Size; i++) {
		auto node = Db->Db.Elements[i];
		auto vmt = *(NodeVMT **)node;
		VMTs.insert(vmt);
	}

	if (VMTs.size() != (unsigned)NodeType::Max) {
		OsiErrorS("Could not locate all Osiris node VMT-s");
		return false;
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
		OsiErrorS("Could not locate RuleNode::__vfptr");
		return false;
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
				OsiErrorS("RelOpNode::__vfptr pattern matches multiple VMT-s");
				return false;
			}
		}
	}

	if (relOpNodeVMT == nullptr) {
		OsiErrorS("Could not locate RelOpNode::__vfptr");
		return false;
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
				OsiErrorS("AndNode::__vfptr pattern matches multiple VMT-s");
				return false;
			}
		}
	}

	if (and1VMT == nullptr || and2VMT == nullptr) {
		OsiErrorS("Could not locate AndNode::__vfptr");
		return false;
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
		OsiErrorS("Could not locate all Query node VMT-s");
		return false;
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
			OsiErrorS("Unrecognized Query node VMT");
			return false;
		}
	}


	// Proc node has different IsProc() code
	NodeVMT * procNodeVMT{ nullptr }, * databaseNodeVMT{ nullptr };
	for (auto vmt : VMTs) {
		if (vmt->IsProc != ruleNodeVMT->IsProc) {
			if (procNodeVMT == nullptr) {
				procNodeVMT = vmt;
			} else {
				OsiErrorS("ProcNode::__vfptr pattern matches multiple VMT-s");
				return false;
			}
		}

		if (vmt->IsDataNode != ruleNodeVMT->IsDataNode
			&& vmt->IsProc == ruleNodeVMT->IsProc) {
			if (databaseNodeVMT == nullptr) {
				databaseNodeVMT = vmt;
			} else {
				OsiErrorS("DatabaseNode::__vfptr pattern matches multiple VMT-s");
				return false;
			}
		}
	}

	if (procNodeVMT == nullptr) {
		OsiErrorS("Could not locate ProcNode::__vfptr");
		return false;
	}

	if (databaseNodeVMT == nullptr) {
		OsiErrorS("Could not locate DatabaseNode::__vfptr");
		return false;
	}

#if 0
	DEBUG("ProcNode::__vfptr is %p", procNodeVMT);
	DEBUG("DatabaseNode::__vfptr is %p", databaseNodeVMT);
#endif
	SaveNodeVMT(NodeType::Proc, procNodeVMT);
	SaveNodeVMT(NodeType::Database, databaseNodeVMT);
	return true;
}

}
