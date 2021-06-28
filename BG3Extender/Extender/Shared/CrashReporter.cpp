#include "stdafx.h"

#include <DbgHelp.h>
#include <psapi.h>
#include <Extender/ScriptExtender.h>
#include <Osiris/Shared/NodeHooks.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

std::atomic<uint32_t> gDisableCrashReportingCount{ 0 };

struct ExcludedSymbol
{
	void * Ptr;
	std::size_t Size;
};

// Symbols that will be excluded from being reported.
// Wrapper frames are included in the stack trace for any server/client worker thread crash,
// so we have to filter them to make sure that we don't receive a report for every (unrelated) game crash.
static const ExcludedSymbol ExcludedSymbols[] = {
	/*{&decltype(bg3se::LibraryManager::esv__ActionMachine__SetState)::CallToTrampoline, 0x120},
	{&decltype(bg3se::LibraryManager::esv__ActionMachine__ResetState)::CallToTrampoline, 0x120},
	{&decltype(bg3se::LibraryManager::esv__CombineManager__ExecuteCombination)::CallToTrampoline, 0x120},
	{&decltype(bg3se::LibraryManager::esv__ProjectileHelpers__ShootProjectile)::CallToTrampoline, 0x120},
	{&decltype(bg3se::LibraryManager::esv__Projectile__Explode)::CallToTrampoline, 0x120},
	{&decltype(bg3se::LibraryManager::UIObjectManager__CreateUIObject)::CallToTrampoline, 0x120},*/
	{&decltype(bg3se::EngineHooks::FileReader__ctor)::CallToTrampoline, 0x120},
	{&decltype(bg3se::ecl::ScriptExtender::gameStateWorkerStart_)::CallToTrampoline, 0x120},
	{&decltype(bg3se::esv::ScriptExtender::gameStateWorkerStart_)::CallToTrampoline, 0x120},
	{&decltype(bg3se::ecl::ScriptExtender::gameStateMachineUpdate_)::CallToTrampoline, 0x120},
	{&decltype(bg3se::esv::ScriptExtender::gameStateMachineUpdate_)::CallToTrampoline, 0x120},
	{&decltype(bg3se::OsirisWrappers::Event)::CallToTrampoline, 0x120},
	{&decltype(bg3se::OsirisWrappers::Call)::CallToTrampoline, 0x120},
	{&decltype(bg3se::OsirisWrappers::Query)::CallToTrampoline, 0x120},
	{&decltype(bg3se::OsirisWrappers::RuleActionCall)::CallToTrampoline, 0x120},
	//{&bg3se::esv::CustomFunctionLibrary::OnShootProjectile, 0x120},
	{&bg3se::NodeVMTWrapper::s_WrappedIsValid, 0x100},
	{&bg3se::NodeVMTWrapper::s_WrappedPushDownTuple, 0x100},
	{&bg3se::NodeVMTWrapper::s_WrappedPushDownTupleDelete, 0x100},
	{&bg3se::NodeVMTWrapper::s_WrappedInsertTuple, 0x100},
	{&bg3se::NodeVMTWrapper::s_WrappedDeleteTuple, 0x100},
	{&bg3se::NodeVMTWrapper::s_WrappedCallQuery, 0x100},
	//{&bg3se::ecl::lua::UIObjectFunctionCallCapture, 0x120},
	{&bg3se::CustomFunctionInjector::StaticCallWrapper, 0x120},
	{&bg3se::CustomFunctionInjector::StaticQueryWrapper, 0x120}
};

class CrashReporter
{
public:
	struct CrashReporterThreadParams
	{
		_EXCEPTION_POINTERS * ExceptionInfo;
		DWORD ThreadId;
	};

	static void * Backtrace[32];
	static bool Initialized;
	static LPTOP_LEVEL_EXCEPTION_FILTER PrevExceptionFilter;
	static std::terminate_handler PrevTerminateHandler;

	static void Initialize()
	{
		if (Initialized || IsDebuggerPresent()) return;

		PrevExceptionFilter = SetUnhandledExceptionFilter(&OnUnhandledException);
		PrevTerminateHandler = std::set_terminate(&OnTerminate);
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

		auto traceSize = CaptureStackBackTrace(2, (DWORD)std::size(Backtrace), Backtrace, NULL);
		for (auto i = 0; i < traceSize; i++) {
			if (Backtrace[i] >= moduleStart && Backtrace[i] < moduleEnd) {
				bool excluded = false;
				for (auto const & sym : ExcludedSymbols) {
					if (Backtrace[i] >= sym.Ptr && Backtrace[i] < (uint8_t *)sym.Ptr + sym.Size) {
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

	static DWORD WINAPI CrashReporterThread(LPVOID userData)
	{
		auto params = (CrashReporterThreadParams *)userData;
		auto dumpPath = GetMiniDumpPath();
		if (CreateMiniDump(params, dumpPath)) {
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

void * CrashReporter::Backtrace[32];
bool CrashReporter::Initialized{ false };
LPTOP_LEVEL_EXCEPTION_FILTER CrashReporter::PrevExceptionFilter{ nullptr };
std::terminate_handler CrashReporter::PrevTerminateHandler{ nullptr };

void InitCrashReporting()
{
	CrashReporter::Initialize();
}

void ShutdownCrashReporting()
{
	CrashReporter::Shutdown();
}
