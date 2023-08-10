#include <stdafx.h>

#include <DbgHelp.h>
#include <psapi.h>
#include <Extender/ScriptExtender.h>
#include <Osiris/Shared/NodeHooks.h>
#include <resource.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

BEGIN_SE()

std::atomic<uint32_t> gDisableCrashReportingCount{ 0 };

std::unordered_set<void*> gRegisteredTrampolines;

struct ExcludedSymbol
{
	void * Ptr;
	std::size_t Size;
};

// Symbols that will be excluded from being reported.
// Wrapper frames are included in the stack trace for any server/client worker thread crash,
// so we have to filter them to make sure that we don't receive a report for every (unrelated) game crash.
static const ExcludedSymbol ExcludedSymbols[] = {
	{&bg3se::NodeVMTWrapper::s_WrappedIsValid, 0x100},
	{&bg3se::NodeVMTWrapper::s_WrappedPushDownTuple, 0x100},
	{&bg3se::NodeVMTWrapper::s_WrappedPushDownTupleDelete, 0x100},
	{&bg3se::NodeVMTWrapper::s_WrappedInsertTuple, 0x100},
	{&bg3se::NodeVMTWrapper::s_WrappedDeleteTuple, 0x100},
	{&bg3se::NodeVMTWrapper::s_WrappedCallQuery, 0x100},
	{&bg3se::CustomFunctionInjector::StaticCallWrapper, 0x120},
	{&bg3se::CustomFunctionInjector::StaticQueryWrapper, 0x120}
};

class CrashReporterSymbolData
{
public:
	struct SymbolData
	{
		// Address relative to binary
		uint32_t RVA;
		// Size of symbol
		uint32_t Size;
		// Address of name (null-terminated) in string table
		uint32_t NameOffset;
	};

	void Load(int resourceId)
	{
		auto symbols = GetExeResource(resourceId);
		if (symbols) {
			symbols_.resize(symbols->size());
			memcpy(symbols_.data(), symbols->data(), symbols->size());
		}
	}

	uint32_t NumSymbols() const
	{
		if (symbols_.empty()) return 0;
		return *(uint32_t*)symbols_.data();
	}

	SymbolData* FindSymbol(uint32_t rva) const
	{
		if (symbols_.empty()) return nullptr;
		
		SymbolData* sym = (SymbolData *)(symbols_.data() + 4);
		auto it = std::lower_bound(sym, sym + NumSymbols(), rva, [](SymbolData const& sym, uint32_t rva) {
			return sym.RVA < rva;
		});

		if (it == sym + NumSymbols()) {
			return nullptr;
		}

		if (rva >= it->RVA && rva < it->RVA + it->Size) {
			return it;
		}

		if (it > sym && rva >= (it-1)->RVA && rva < (it - 1)->RVA + (it - 1)->Size) {
			return (it - 1);
		}

		return nullptr;
	}

	char const* GetName(SymbolData* sym) const
	{
		return (char const*)(symbols_.data() + 4 + (sizeof(SymbolData) * NumSymbols()) + sym->NameOffset);
	}

private:
	std::vector<uint8_t> symbols_;
};

#pragma pack(push, 1)
// PE32+ header
// see: https://docs.microsoft.com/en-us/windows/win32/debug/pe-format
struct PeHeader
{
	uint32_t Magic;
	uint16_t Machine;
	uint16_t NumberOfSections;
	uint32_t TimeDateStamp;
	uint32_t PointerToSymbolTable;
	uint32_t NumberOfSymbols;
	uint16_t SizeOfOptionalHeader;
	uint16_t Characteristics;
	uint16_t OptionalMagic;
	uint8_t  MajorLinkerVersion;
	uint8_t  MinorLinkerVersion;
	uint32_t SizeOfCode;
	uint32_t SizeOfInitializedData;
	uint32_t SizeOfUninitializedData;
	uint32_t AddressOfEntryPoint;
	uint32_t BaseOfCode;
	uint64_t ImageBase;
};
#pragma pack(pop)

class BacktraceDumper
{
public:
	// Show at most 12 lines to avoid flooding the crash reporter dialog
	static constexpr unsigned MaxBacktraceLines = 12;
	static constexpr unsigned MaxLuaBacktraceLines = 12;
	void* Backtrace[32];
	WORD BacktraceSize;
	DWORD CrashThreadId;

	void Initialize()
	{
#if defined(USE_GAME_SYMBOL_TABLE)
		eocAppSymbols_.Load(IDR_SYMBOL_TABLE_GAME);
#endif

#if defined(USE_DBG_SYMBOL_TABLE)
		symbols_.Load(IDR_SYMBOL_TABLE_EXTENDER);
#endif

		MODULEINFO moduleInfo;
		auto hEoCApp = GetModuleHandleW(L"EoCApp.exe");
		if (hEoCApp != NULL) {
			if (GetModuleInformation(GetCurrentProcess(), hEoCApp, &moduleInfo, sizeof(moduleInfo))) {
				eocAppStart_ = (uint8_t*)moduleInfo.lpBaseOfDll;
				eocAppEnd_ = eocAppStart_ + moduleInfo.SizeOfImage;
			}
		}

		if (GetModuleInformation(GetCurrentProcess(), gThisModule, &moduleInfo, sizeof(moduleInfo))) {
			extenderStart_ = (uint8_t*)moduleInfo.lpBaseOfDll;
			extenderEnd_ = extenderStart_ + moduleInfo.SizeOfImage;
		}

		moduleHandles_.reserve(0x100);
		modules_.reserve(0x100);
	}

	void __forceinline SnapshotThread()
	{
		CrashThreadId = GetCurrentThreadId();
		BacktraceSize = CaptureStackBackTrace(2, (DWORD)std::size(Backtrace), Backtrace, NULL);
	}

	bool TryGetFunctionName(std::stringstream& ss, uint8_t* bt, bool isFirst)
	{
		char moduleName[0x100];

		if (bt >= extenderStart_ && bt < extenderEnd_) {
			auto symbolOffset = (uint32_t)((uint8_t*)bt - extenderStart_);
			auto sym = symbols_.FindSymbol(symbolOffset);
			if (sym) {
				ss << symbols_.GetName(sym) << "+0x" << std::hex << std::setw(0) << (uint64_t)(symbolOffset - sym->RVA) << std::endl;
				return true;
			}
		}

		if (bt >= eocAppStart_ && bt < eocAppEnd_) {
			auto symbolOffset = (uint32_t)((uint8_t*)bt - eocAppStart_);
			auto sym = eocAppSymbols_.FindSymbol(symbolOffset);
			if (sym) {
				ss << eocAppSymbols_.GetName(sym) << "+0x" << std::hex << std::setw(0) << (uint64_t)(symbolOffset - sym->RVA) << std::endl;
				return true;
			}
		}

		bool found{ false };
		for (auto i = 0; i < modules_.size(); i++) {
			auto const& mod = modules_[i];
			if (bt >= mod.lpBaseOfDll && bt <= (uint8_t*)mod.lpBaseOfDll + mod.SizeOfImage) {
				DWORD length = GetModuleBaseNameA(GetCurrentProcess(), moduleHandles_[i], moduleName, sizeof(moduleName) - 1);
				if (length) {
					moduleName[length] = 0;

					// Remove ntdll/kernelbase error handler frames from the top of the stack
					if (isFirst && (_stricmp(moduleName, "ntdll.dll") == 0 || _stricmp(moduleName, "kernelbase.dll") == 0)) {
						return false;
					}

					auto base = (mod.lpBaseOfDll == eocAppStart_) ? 0x140000000 : 0x180000000;
					auto relativeAddr = (uint8_t*)bt - (uint8_t*)mod.lpBaseOfDll + base;
					ss << moduleName << "!0x" << std::hex << (uint64_t)relativeAddr;
					found = true;
					break;
				}
			}
		}

		if (!found) {
			ss << "Unknown!0x" << std::hex << std::setfill('0') << std::setw(16) << (uint64_t)bt;
		}

		ss << std::endl;
		return true;
	}

	std::string DumpBacktrace()
	{
		DWORD cbNeeded{ 0 };
		EnumProcessModules(GetCurrentProcess(), NULL, 0, &cbNeeded);

		moduleHandles_.resize(cbNeeded / sizeof(HMODULE));
		EnumProcessModules(GetCurrentProcess(), moduleHandles_.data(), (DWORD)moduleHandles_.size() * sizeof(HMODULE), &cbNeeded);

		modules_.resize(moduleHandles_.size());
		for (std::size_t i = 0; i < modules_.size(); i++) {
			GetModuleInformation(GetCurrentProcess(), moduleHandles_[i], modules_.data() + i, sizeof(MODULEINFO));
		}

		std::stringstream backtrace;

		unsigned lines{ 0 };
		for (auto i = 0; i < BacktraceSize; i++) {
			auto bt = Backtrace[i];
			if (TryGetFunctionName(backtrace, (uint8_t*)bt, backtrace.str().empty())) {
				if (lines++ >= MaxBacktraceLines) {
					backtrace << "(...)" << std::endl;
					break;
				}
			}
		}

		return backtrace.str();
	}

	std::string DumpLuaBacktrace(ExtensionStateBase& state)
	{
		auto lua = state.GetLua();
		if (!lua || !lua->GetState()) {
			return std::string();
		}

		auto L = lua->GetState();

		lua_Debug ar;
		std::stringstream bt;

		unsigned lines{ 0 };
		for (int i = 0;; i++) {
			memset(&ar, 0, sizeof(ar));
			if (lua_getstack(L, i, &ar) == 0) {
				break;
			}

			std::string frame;
			lua_getinfo(L, "fnuSl", &ar);

			if (ar.source) {
				if (ar.source[0] == '=') {
					bt << "[C++ code]";
				} else {
					bt << ar.source;
				}
			} else {
				bt << "(unknown)";
			}

			if (ar.currentline > 0) {
				bt << ":" << std::to_string(ar.currentline);
			}

			if (ar.name) {
				bt << " " << ar.name;
			}

			bt << std::endl;

			if (lines++ >= MaxLuaBacktraceLines) {
				bt << "(...)" << std::endl;
				break;
			}
		}

		return bt.str();
	}

	std::string DumpLuaBacktrace()
	{
		if (gExtender->GetServer().IsInThread(CrashThreadId) && gExtender->GetServer().HasExtensionState()) {
			return DumpLuaBacktrace(gExtender->GetServer().GetExtensionState());
		}

		if (gExtender->GetClient().IsInThread(CrashThreadId) && gExtender->GetClient().HasExtensionState()) {
			return DumpLuaBacktrace(gExtender->GetClient().GetExtensionState());
		}

		return std::string();
	}

private:
	CrashReporterSymbolData eocAppSymbols_;
	CrashReporterSymbolData symbols_;
	std::vector<HMODULE> moduleHandles_;
	std::vector<MODULEINFO> modules_;
	uint8_t* eocAppStart_{ nullptr };
	uint8_t* eocAppEnd_{ nullptr };
	uint8_t* extenderStart_{ nullptr };
	uint8_t* extenderEnd_{ nullptr };
};

class CrashReporter
{
public:
	struct CrashReporterThreadParams
	{
		_EXCEPTION_POINTERS * ExceptionInfo;
		DWORD ThreadId;
	};

	static bool Initialized;
	static LPTOP_LEVEL_EXCEPTION_FILTER PrevExceptionFilter;
	static std::terminate_handler PrevTerminateHandler;
	static BacktraceDumper Dumper;
	static std::string LuaBacktrace;

	static void Initialize()
	{
		if (Initialized) return;

		PrevExceptionFilter = SetUnhandledExceptionFilter(&OnUnhandledException);
		PrevTerminateHandler = std::set_terminate(&OnTerminate);
		Dumper.Initialize();
		Initialized = true;
	}

	static void Shutdown()
	{
		if (!Initialized) return;

		SetUnhandledExceptionFilter(PrevExceptionFilter);
		std::set_terminate(PrevTerminateHandler);
		PrevExceptionFilter = nullptr;
		PrevTerminateHandler = nullptr;
		Initialized = false;
	}

	static std::wstring GetMiniDumpPath()
	{
		std::wstring tempPath;
		DWORD tempPathLen = GetTempPathW(0, NULL);
		tempPath.resize(tempPathLen);
		GetTempPathW(tempPathLen, tempPath.data());
		tempPath.resize(tempPathLen - 1);

		tempPath += L"\\OsiExtMiniDump.dmp";
		return tempPath;
	}
	
	static BOOL WINAPI MinidumpFilter(PVOID CallbackParam, PMINIDUMP_CALLBACK_INPUT CallbackInput,
		PMINIDUMP_CALLBACK_OUTPUT CallbackOutput)
	{
		switch (CallbackInput->CallbackType) {
		case IncludeModuleCallback:
		case ThreadCallback:
		case ThreadExCallback:
			return TRUE;

		case IncludeThreadCallback:
			// We don't want to include information about the minidump writing 
			// thread, as that's not of interest to the caller
			if (CallbackInput->IncludeThread.ThreadId == GetCurrentThreadId()) {
				return FALSE;
			} else {
				return TRUE;
			}

		case ModuleCallback:
			if (!(CallbackOutput->ModuleWriteFlags & ModuleReferencedByMemory)) {
				CallbackOutput->ModuleWriteFlags &= ~ModuleWriteModule;
			}

			return TRUE;

		case MemoryCallback:
		case CancelCallback:
		default:
			return FALSE;
		}
	}

	static bool CreateMiniDump(CrashReporterThreadParams * params, std::wstring const & dumpPath)
	{
		HANDLE dumpFile = CreateFileW(dumpPath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (dumpFile == INVALID_HANDLE_VALUE) return false;

		_MINIDUMP_EXCEPTION_INFORMATION dumpExcInfo;
		dumpExcInfo.ThreadId = params->ThreadId;
		dumpExcInfo.ExceptionPointers = params->ExceptionInfo;
		dumpExcInfo.ClientPointers = FALSE;

		PMINIDUMP_EXCEPTION_INFORMATION pExcInfo{ nullptr };
		if (params->ExceptionInfo != nullptr) {
			pExcInfo = &dumpExcInfo;
		}

		// Set up the callback to be called by the minidump writer.  This allows us to
		// filter out information that we may not care about.
		MINIDUMP_CALLBACK_INFORMATION callback = { 0 };
		callback.CallbackParam = nullptr;
		callback.CallbackRoutine = MinidumpFilter;

		auto flags = (MINIDUMP_TYPE)(MiniDumpWithIndirectlyReferencedMemory
			| MiniDumpScanMemory);
		bool written = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
			dumpFile, flags, pExcInfo, NULL, NULL);
		CloseHandle(dumpFile);
		return written;
	}

	// Inspects the current stack to check whether the extension DLL
	// is present in any of the frames. If it is, the crash may be extension related.
	// If no extension frames are present, we skip error reporting.
	// noinline needed to ensure that the error handler stack is always 2 levels deep
	static __declspec(noinline) bool IsExtensionRelatedCrash()
	{
		if (gExtender->GetConfig().ForceCrashReporting) {
			return true;
		}

		if (gDisableCrashReportingCount > 0) {
			return false;
		}

		void * moduleStart, * moduleEnd;
		MODULEINFO moduleInfo;
		if (!GetModuleInformation(GetCurrentProcess(), gThisModule, &moduleInfo, sizeof(moduleInfo))) {
			return false;
		}

		moduleStart = moduleInfo.lpBaseOfDll;
		moduleEnd = (uint8_t *)moduleStart + moduleInfo.SizeOfImage;

		Dumper.SnapshotThread();
		for (auto i = 0; i < Dumper.BacktraceSize; i++) {
			if (Dumper.Backtrace[i] >= moduleStart && Dumper.Backtrace[i] < moduleEnd) {
				bool excluded = false;
				for (auto const & sym : ExcludedSymbols) {
					if (Dumper.Backtrace[i] >= sym.Ptr && Dumper.Backtrace[i] < (uint8_t *)sym.Ptr + sym.Size) {
						excluded = true;
						break;
					}
				}

				for (auto const& trampoline : gRegisteredTrampolines) {
					if (Dumper.Backtrace[i] >= trampoline && Dumper.Backtrace[i] < (uint8_t*)trampoline + 0x120) {
						excluded = true;
						break;
					}
				}

				if (!excluded) {
					return true;
				}
			}
		}

		return false;
	}

	static void LaunchCrashReporter(std::wstring & miniDumpPath)
	{
		std::wstring crashReporterPath;
		crashReporterPath.resize(1024);
		DWORD length = GetModuleFileName(gThisModule, crashReporterPath.data(), (DWORD)crashReporterPath.size());
		// Module not found
		if (length == 0) return;
		crashReporterPath.resize(length);

		auto sep = crashReporterPath.find_last_of('\\');
		crashReporterPath = crashReporterPath.substr(0, sep);
		crashReporterPath += L"\\CrashReporter.exe";

		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);

		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));

		if (!CreateProcessW(crashReporterPath.c_str(), miniDumpPath.data(), NULL, NULL, FALSE, 0,
			NULL, NULL, &si, &pi)) {
			return;
		}

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	static void TryDumpLuaBacktraceInternal()
	{
		LuaBacktrace = Dumper.DumpLuaBacktrace();

		if (!LuaBacktrace.empty()) {
			LuaBacktrace = "\r\n\r\nLua backtrace:\r\n" + LuaBacktrace;
		}
	}

	static void TryDumpLuaBacktrace()
	{
		__try {
			TryDumpLuaBacktraceInternal();
		} __except (EXCEPTION_EXECUTE_HANDLER) {
			LuaBacktrace = "\r\n(error retrieving Lua stack trace)";
		}
	}

	static void CreateBacktraceFile(std::wstring const& dumpPath)
	{
		auto bt = Dumper.DumpBacktrace();
		TryDumpLuaBacktrace();
		bt += LuaBacktrace;

		std::wstring btPath = dumpPath;
		btPath += L".bt";
		std::ofstream f(btPath.c_str(), std::ios::out | std::ios::binary);
		if (f.good()) {
			f.write(bt.data(), bt.size());
		}
	}

	static DWORD WINAPI CrashReporterThread(LPVOID userData)
	{
		auto params = (CrashReporterThreadParams *)userData;
		auto dumpPath = GetMiniDumpPath();
		if (CreateMiniDump(params, dumpPath)) {
			CreateBacktraceFile(dumpPath);
			LaunchCrashReporter(dumpPath);
		}

		ExitProcess(1);
		return 0;
	}

	static void LaunchCrashReporterThread(_EXCEPTION_POINTERS * exceptionInfo)
	{
		CrashReporterThreadParams params;
		params.ExceptionInfo = exceptionInfo;
		params.ThreadId = GetCurrentThreadId();

		// Run crash reporter in a separate thread to ensure that
		// we succeed if the stack is corrupted
		CreateThread(NULL, 0, &CrashReporterThread, &params, 0, NULL);
		SuspendThread(GetCurrentThread());
	}

	static LONG OnUnhandledException(_EXCEPTION_POINTERS * exceptionInfo)
	{
		if (IsExtensionRelatedCrash()) {
			LaunchCrashReporterThread(exceptionInfo);
			return EXCEPTION_EXECUTE_HANDLER;
		} else if (PrevExceptionFilter != nullptr) {
			return PrevExceptionFilter(exceptionInfo);
		} else {
			return EXCEPTION_CONTINUE_SEARCH;
		}
	}

	static void OnTerminate()
	{
		if (IsExtensionRelatedCrash()) {
			LaunchCrashReporterThread(nullptr);
		} else if (PrevTerminateHandler) {
			PrevTerminateHandler();
		}
	}
};

bool CrashReporter::Initialized{ false };
LPTOP_LEVEL_EXCEPTION_FILTER CrashReporter::PrevExceptionFilter{ nullptr };
std::terminate_handler CrashReporter::PrevTerminateHandler{ nullptr };
BacktraceDumper CrashReporter::Dumper;
std::string CrashReporter::LuaBacktrace;

void InitCrashReporting()
{
	CrashReporter::Initialize();
}

void ShutdownCrashReporting()
{
	CrashReporter::Shutdown();
}

void DumpCurrentStack()
{
	CrashReporter::Dumper.SnapshotThread();
	DEBUG("%s\r\n", CrashReporter::Dumper.DumpBacktrace().c_str());
}

END_SE()
