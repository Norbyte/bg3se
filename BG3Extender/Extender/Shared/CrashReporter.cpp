#include <stdafx.h>

#include <DbgHelp.h>
#include <psapi.h>
#include <Extender/ScriptExtender.h>
#include <Extender/Version.h>
#include <Osiris/Shared/NodeHooks.h>
#include <resource.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

#undef DEBUG_CRASH_REPORTER
#define ALLOW_DEBUG_CRASH_REPORTING

#if defined(DEBUG_CRASH_REPORTER)
#define CRASHDBG(...) INFO(__VA_ARGS__)
#else
#define CRASHDBG(...) 
#endif

BEGIN_SE()

extern char const* BuildDate;

std::atomic<uint32_t> gDisableCrashReportingCount{ 0 };

std::unordered_set<void const *> gRegisteredTrampolines;

struct ExcludedSymbol
{
    void * Ptr;
    std::size_t Size;
};

// Symbols that will be excluded from being reported.
// Wrapper frames are included in the stack trace for any server/client worker thread crash,
// so we have to filter them to make sure that we don't receive a report for every (unrelated) game crash.
static const ExcludedSymbol ExcludedSymbols[] = {
    {&bg3se::NodeVMTWrapper::s_WrappedIsValid, 0x180},
    {&bg3se::NodeVMTWrapper::s_WrappedPushDownTuple, 0x180},
    {&bg3se::NodeVMTWrapper::s_WrappedPushDownTupleDelete, 0x180},
    {&bg3se::NodeVMTWrapper::s_WrappedInsertTuple, 0x180},
    {&bg3se::NodeVMTWrapper::s_WrappedDeleteTuple, 0x180},
    {&bg3se::NodeVMTWrapper::s_WrappedCallQuery, 0x180},
    {&bg3se::CustomFunctionInjector::StaticCallWrapper, 0x120},
    {&bg3se::CustomFunctionInjector::StaticQueryWrapper, 0x120},
    {&bg3se::OsirisWrappers::CallWrapper, 0x120},
    {&bg3se::OsirisWrappers::QueryWrapper, 0x120},
    {&bg3se::OsirisWrappers::ErrorWrapper, 0x120},
    {&bg3se::OsirisWrappers::AssertWrapper, 0x120},
};

std::wstring GetExtenderDllDir()
{
    std::wstring dllPath;
    dllPath.resize(1024);
    DWORD length = GetModuleFileName(gCoreLibPlatformInterface.ThisModule, dllPath.data(), (DWORD)dllPath.size());
    // Module not found
    if (length == 0) return {};
    dllPath.resize(length);

    auto sep = dllPath.find_last_of('\\');
    dllPath = dllPath.substr(0, sep);
    dllPath += L"\\";
    return dllPath;
}

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

    void Load(std::wstring symbolFile)
    {
        auto path = GetExtenderDllDir() + symbolFile;
        std::vector<uint8_t> symbols;

        if (LoadFile(path, symbols)) {
            symbols_ = symbols;
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

class CrashDataWriter
{
public:
    static constexpr uint32_t Version = 1;
    static constexpr uint32_t Signature = 'CRSH';
    static constexpr uint32_t TagHeader = 'VERS';
    static constexpr uint32_t TagExtenderVersion = 'SVER';
    static constexpr uint32_t TagGameVersion = 'GVER';
    static constexpr uint32_t TagBuildDate = 'SDAT';
    static constexpr uint32_t TagCertification = 'CERT';
    static constexpr uint32_t TagReleaseChannel = 'CHAN';
    static constexpr uint32_t TagBuildType = 'BLDT';
    static constexpr uint32_t TagStackTrace = 'TRAC';
    static constexpr uint32_t TagLuaStackTrace = 'LUAT';
    static constexpr uint32_t TagLoadedModules = 'MODS';
    static constexpr uint32_t TagErrorLog = 'ERRL';
    static constexpr uint32_t TagServerState = 'SSTA';
    static constexpr uint32_t TagClientState = 'CSTA';
    static constexpr uint32_t TagSessionLength = 'SLEN';
    static constexpr uint32_t TagMinidump = 'MDMP';

    CrashDataWriter(std::wstring const& path)
        : f_(path.c_str(), std::ios::out | std::ios::binary)
    {}

    bool CanWrite()
    {
        return f_.good();
    }

    void AddSection(uint32_t fourCC, std::string_view buf)
    {
        auto len = (uint32_t)buf.size();
        f_.write((char*)&fourCC, sizeof(fourCC));
        f_.write((char*)&len, sizeof(len));
        f_.write(buf.data(), buf.size());
    }

    void AddHeader()
    {
        f_.write((char*)&Signature, sizeof(Signature));

        uint32_t ver = Version;
        AddSection(TagHeader, std::string_view((char*)&ver, sizeof(ver)));
    }

private:
    std::ofstream f_;
};

class BacktraceDumper
{
public:
    // Show at most 12 lines to avoid flooding the crash reporter dialog
#if !defined(DEBUG_CRASH_REPORTER)
    static constexpr unsigned MaxBacktraceLines = 12;
    static constexpr unsigned MaxLuaDisplayBacktraceLines = 12;
#else
    static constexpr unsigned MaxBacktraceLines = 32;
    static constexpr unsigned MaxLuaDisplayBacktraceLines = 32;
#endif
    static constexpr unsigned MaxLuaFullBacktraceLines = 200;

    void* Backtrace[32]{ nullptr };
    WORD BacktraceSize{ 0 };
    DWORD CrashThreadId{ 0 };

    void Initialize()
    {
        eocAppSymbols_.Load(L"BG3.symtab");
        symbols_.Load(L"BG3ScriptExtender.symtab");

        MODULEINFO moduleInfo;
        auto hEoCApp = gExtender->GetLibraryManager().GetAppHandle();
        if (hEoCApp != NULL) {
            if (GetModuleInformation(GetCurrentProcess(), hEoCApp, &moduleInfo, sizeof(moduleInfo))) {
                eocAppStart_ = (uint8_t*)moduleInfo.lpBaseOfDll;
                eocAppEnd_ = eocAppStart_ + moduleInfo.SizeOfImage;
            }
        }

        if (GetModuleInformation(GetCurrentProcess(), gCoreLibPlatformInterface.ThisModule, &moduleInfo, sizeof(moduleInfo))) {
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

        CRASHDBG("Backtrace:\r\n%s\r\n", backtrace.str().c_str());
        return backtrace.str();
    }

    std::pair<std::string, std::string> DumpLuaBacktrace(ExtensionStateBase& state)
    {
        auto lua = state.GetLua();
        if (!lua || !lua->GetState()) {
            return { std::string(), std::string() };
        }

        auto L = lua->GetState();

        lua_Debug ar;
        std::stringstream bt;
        std::string fullTrace;
        std::string displayTrace;

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

            if (lines++ == MaxLuaDisplayBacktraceLines) {
                displayTrace = bt.str();
                displayTrace += "(...)\r\n";
            }

            if (lines++ == MaxLuaDisplayBacktraceLines) {
                bt << "(...)" << std::endl;
                fullTrace = bt.str();
                break;
            }
        }

        if (displayTrace.empty()) {
            displayTrace = bt.str();
        }

        if (fullTrace.empty()) {
            fullTrace = bt.str();
        }

        CRASHDBG("Lua Backtrace:\r\n%s\r\n", fullTrace.c_str());
        return { fullTrace, displayTrace };
    }

    std::pair<std::string, std::string> DumpLuaBacktrace()
    {
        if (gExtender->GetServer().HasExtensionState() && gExtender->GetServer().GetExtensionState().GetOwningThread() == CrashThreadId) {
            return DumpLuaBacktrace(gExtender->GetServer().GetExtensionState());
        }

        if (gExtender->GetClient().HasExtensionState() && gExtender->GetClient().GetExtensionState().GetOwningThread() == CrashThreadId) {
            return DumpLuaBacktrace(gExtender->GetClient().GetExtensionState());
        }

        return { std::string(), std::string() };
    }

    std::string DumpModuleList()
    {
        auto modManager = GetStaticSymbols().GetModManagerClient();
        if (!modManager) {
            return {};
        }

        std::string mods;
        for (auto const& mod : modManager->LoadOrderedModules) {
            mods += mod.Info.ModuleUUIDString.GetStringView();
            mods += "\t";
            mods += mod.Info.Name;
            mods += "\t";
            mods += std::to_string(mod.Info.ModVersion.Major()) + "."
                + std::to_string(mod.Info.ModVersion.Minor()) + "."
                + std::to_string(mod.Info.ModVersion.Revision()) + "."
                + std::to_string(mod.Info.ModVersion.Build());
            mods += "\r\n";
        }

        return mods;
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
    static std::string Backtrace;
    static std::string LuaFullBacktrace;
    static std::string LuaDisplayBacktrace;
    static std::string ModuleList;
    static std::string ErrorLog;

    static void OnLogMessage(DebugMessageType type, char const* msg)
    {
        if (!Initialized) return;
        if (type != DebugMessageType::Warning && type != DebugMessageType::Error) return;
        if (ErrorLog.size() > 0x10000) return;

        ErrorLog += msg;
        ErrorLog += "\r\n";
    }

    static void Initialize()
    {
        if (Initialized) return;

        gRegisteredTrampolines.insert(ResolveRealFunctionAddress(&CrashReporter::IsExtensionRelatedCrash));
        gRegisteredTrampolines.insert(ResolveRealFunctionAddress(&CrashReporter::OnUnhandledException));
        PrevExceptionFilter = SetUnhandledExceptionFilter(&OnUnhandledException);
        PrevTerminateHandler = std::set_terminate(&OnTerminate);

        if (gCoreLibPlatformInterface.GlobalConsole) {
            gCoreLibPlatformInterface.GlobalConsole->SetLogCallback(&OnLogMessage);
        }

        ErrorLog.reserve(0x10000);
        Dumper.Initialize();
        Initialized = true;
        CRASHDBG("Enabled crash reporting");
    }

    static void Shutdown()
    {
        if (!Initialized) return;

        if (gCoreLibPlatformInterface.GlobalConsole) {
            gCoreLibPlatformInterface.GlobalConsole->SetLogCallback(nullptr);
        }

        SetUnhandledExceptionFilter(PrevExceptionFilter);
        std::set_terminate(PrevTerminateHandler);
        PrevExceptionFilter = nullptr;
        PrevTerminateHandler = nullptr;
        Initialized = false;
        CRASHDBG("Disabled crash reporting");
    }

    static std::wstring GetMiniDumpPath()
    {
        std::wstring tempPath;
        DWORD tempPathLen = GetTempPathW(0, NULL);
        tempPath.resize(tempPathLen);
        GetTempPathW(tempPathLen, tempPath.data());
        tempPath.resize(tempPathLen - 1);

        tempPath += L"\\OsiExtMiniDump.dmp";
        CRASHDBG("Writing minidump to: %s", ToStdUTF8(tempPath).c_str());
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
    static __declspec(noinline) bool IsExtensionRelatedCrash(_EXCEPTION_POINTERS* exceptionInfo)
    {
        if (exceptionInfo != nullptr 
            && exceptionInfo->ExceptionRecord->ExceptionCode == 0xC00000FD
            && !gExtender->GetConfig().ForceCrashReporting) {
            // Osiris is riddled with various stack overflow issues, skip reporting those
            return false;
        }

        CRASHDBG("IsExtensionRelatedCrash() - checking frames");
        if (gDisableCrashReportingCount > 0 && !gExtender->GetConfig().ForceCrashReporting) {
            CRASHDBG("gDisableCrashReportingCount > 0 --> false");
            return false;
        }

        void * moduleStart, * moduleEnd;
        MODULEINFO moduleInfo;
        if (!GetModuleInformation(GetCurrentProcess(), gCoreLibPlatformInterface.ThisModule, &moduleInfo, sizeof(moduleInfo))) {
            CRASHDBG("No module info found for extender --> false");
            return false;
        }

        moduleStart = moduleInfo.lpBaseOfDll;
        moduleEnd = (uint8_t *)moduleStart + moduleInfo.SizeOfImage;

        Dumper.SnapshotThread();

        // We need to collect backtrace even if ForceCrashReporting is set
        if (gExtender->GetConfig().ForceCrashReporting) {
            CRASHDBG("ForceCrashReporting --> true");
            return true;
        }

        for (auto i = 0; i < Dumper.BacktraceSize; i++) {
            if (Dumper.Backtrace[i] >= moduleStart && Dumper.Backtrace[i] < moduleEnd) {
                bool excluded = false;

                // Only exclude crashes where the IP itself is not in a trampoline
                if (i > 0) {
                    // Exclude trampolines from crash reporting to make sure that we don't send reports for
                    // BG3 code thats below a trampoline
                    for (auto const & sym : ExcludedSymbols) {
                        if (Dumper.Backtrace[i] >= sym.Ptr && Dumper.Backtrace[i] < (uint8_t *)sym.Ptr + sym.Size) {
                            excluded = true;
                            CRASHDBG("Exclude %p: Whitelisted region (%p .. %p)", Dumper.Backtrace[i], sym.Ptr, (uint8_t*)sym.Ptr + sym.Size);
                            break;
                        }
                    }

                    for (auto const& trampoline : gRegisteredTrampolines) {
                        if (Dumper.Backtrace[i] >= trampoline && Dumper.Backtrace[i] < (uint8_t*)trampoline + 0x120) {
                            excluded = true;
                            CRASHDBG("Exclude %p: Whitelisted trampoline (%p .. %p)", Dumper.Backtrace[i], trampoline, (uint8_t*)trampoline + 0x120);
                            break;
                        }
                    }
                }

                if (!excluded) {
                    CRASHDBG("Frame %p in extender --> true", Dumper.Backtrace[i]);
                    return true;
                }
            } else {
                CRASHDBG("Exclude %p: Not in extender module", Dumper.Backtrace[i]);
            }
        }

        CRASHDBG("No eligible frames found --> false");
        return false;
    }

    static void LaunchCrashReporter(std::wstring & miniDumpPath)
    {
        std::wstring crashReporterPath;
        crashReporterPath.resize(1024);
        DWORD length = GetModuleFileName(gCoreLibPlatformInterface.ThisModule, crashReporterPath.data(), (DWORD)crashReporterPath.size());
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

        auto cmdline = std::wstring(L"CrashReporter.exe ") + miniDumpPath;

        CRASHDBG("Saved MiniDump to: %s", ToStdUTF8(miniDumpPath).c_str());
        CRASHDBG("Launch crash reporter: %s", ToStdUTF8(crashReporterPath).c_str());
        if (!CreateProcessW(crashReporterPath.c_str(), cmdline.data(), NULL, NULL, FALSE, 0,
            NULL, NULL, &si, &pi)) {
            return;
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    static void TryDumpLuaBacktraceInternal()
    {
        auto traces = Dumper.DumpLuaBacktrace();
        LuaDisplayBacktrace = traces.first;
        LuaFullBacktrace = traces.first;
    }

    static bool TryDumpLuaBacktrace()
    {
        __try {
            TryDumpLuaBacktraceInternal();
            return true;
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
    }

    static void TryDumpModuleListInternal()
    {
        ModuleList = Dumper.DumpModuleList();
    }

    static bool TryDumpModuleList()
    {
        __try {
            TryDumpModuleListInternal();
            return true;
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            ModuleList.clear();
            return false;
        }
    }

    static void CreateBacktraceFile(std::wstring const& dumpPath)
    {
        auto bt = Backtrace;
        if (!LuaDisplayBacktrace.empty()) {
            bt += "\r\n\r\nLua Backtrace:\r\n";
            bt += LuaDisplayBacktrace;
        }

        std::wstring btPath = dumpPath;
        btPath += L".bt";

        CRASHDBG("BT path: %s", ToStdUTF8(btPath).c_str());
        CRASHDBG("BT contents:\r\n%s\r\n", Backtrace.c_str());

        std::ofstream f(btPath.c_str(), std::ios::out | std::ios::binary);
        if (f.good()) {
            f.write(bt.data(), bt.size());
            CRASHDBG("BT write OK.");
        }
    }

    static void CreateCrashReportFile(std::wstring const& dumpPath)
    {
        CrashDataWriter writer(dumpPath + L".crpt");
        writer.AddHeader();
        writer.AddSection(CrashDataWriter::TagExtenderVersion, RES_DLL_VERSION_STRING);

        auto const& ver = gExtender->GetGameVersion();
        std::string gameVer = std::to_string(ver.Major) + "."
            + std::to_string(ver.Minor) + "."
            + std::to_string(ver.Revision) + "."
            + std::to_string(ver.Build);
        writer.AddSection(CrashDataWriter::TagGameVersion, gameVer);
        writer.AddSection(CrashDataWriter::TagBuildDate, BuildDate);
        writer.AddSection(CrashDataWriter::TagReleaseChannel, SE_BUILD_CHANNEL);

#if !defined(NDEBUG)
        writer.AddSection(CrashDataWriter::TagBuildType, "Debug");
#elif defined(SE_IS_DEVELOPER_BUILD)
        writer.AddSection(CrashDataWriter::TagBuildType, "Devel");
#else
        writer.AddSection(CrashDataWriter::TagBuildType, "Release");
#endif

#if defined(SE_CERTIFICATION_ID)
        writer.AddSection(CrashDataWriter::TagCertification, SE_CERTIFICATION_ID);
#endif
        
        Backtrace = Dumper.DumpBacktrace();
        writer.AddSection(CrashDataWriter::TagStackTrace, Backtrace);
        
        if (TryDumpLuaBacktrace()) {
            writer.AddSection(CrashDataWriter::TagLuaStackTrace, LuaFullBacktrace);
        }

        if (TryDumpModuleList()) {
            writer.AddSection(CrashDataWriter::TagLoadedModules, ModuleList);
        }

        if (!ErrorLog.empty()) {
            writer.AddSection(CrashDataWriter::TagErrorLog, ErrorLog);
        }

        auto clientState = GetStaticSymbols().GetClientState();
        if (clientState) {
            auto state = *clientState;
            writer.AddSection(CrashDataWriter::TagClientState, std::string_view((char *)&state, sizeof(state)));
        }

        auto serverState = GetStaticSymbols().GetServerState();
        if (serverState) {
            auto state = *serverState;
            writer.AddSection(CrashDataWriter::TagServerState, std::string_view((char *)&state, sizeof(state)));
        }

        auto startTime = gExtender->GetStartTime();
        if (startTime) {
            auto runtime = GetTickCount64() - startTime;
            writer.AddSection(CrashDataWriter::TagSessionLength, std::string_view((char*)&runtime, sizeof(runtime)));
        }

        std::ifstream df(dumpPath.c_str(), std::ios::in | std::ios::binary);
        if (df.good()) {
            df.seekg(0, std::ios::end);
            auto size = df.tellg();
            df.seekg(0, std::ios::beg);

            std::vector<char> dump;
            dump.resize(size);
            df.read(dump.data(), size);

            writer.AddSection(CrashDataWriter::TagMinidump, std::string_view(dump.data(), dump.size()));
        }
    }

    static DWORD WINAPI CrashReporterThread(LPVOID userData)
    {
        auto params = (CrashReporterThreadParams *)userData;
        auto dumpPath = GetMiniDumpPath();
        if (CreateMiniDump(params, dumpPath)) {
            CreateCrashReportFile(dumpPath);
            CreateBacktraceFile(dumpPath);
            LaunchCrashReporter(dumpPath);
        }

#if defined(DEBUG_CRASH_REPORTER)
        MessageBoxW(NULL, L"CrashReporterThread done", L"", MB_OK | MB_ICONERROR | MB_APPLMODAL);
#endif
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
#if defined(DEBUG_CRASH_REPORTER)
        MessageBoxW(NULL, L"OnUnhandledException: Launched crash reporter thread", L"", MB_OK | MB_APPLMODAL);
#endif
        SuspendThread(GetCurrentThread());
    }

    static LONG OnUnhandledException(_EXCEPTION_POINTERS * exceptionInfo)
    {
        if (IsExtensionRelatedCrash(exceptionInfo)) {
            LaunchCrashReporterThread(exceptionInfo);
            return EXCEPTION_EXECUTE_HANDLER;
        } else if (PrevExceptionFilter != nullptr) {
#if defined(DEBUG_CRASH_REPORTER)
            MessageBoxW(NULL, L"OnUnhandledException: Nothing to report - PrevExceptionFilter", L"", MB_OK | MB_APPLMODAL);
#endif
            return PrevExceptionFilter(exceptionInfo);
        } else {
#if defined(DEBUG_CRASH_REPORTER)
            MessageBoxW(NULL, L"OnUnhandledException: Nothing to report", L"", MB_OK | MB_APPLMODAL);
#endif
            return EXCEPTION_CONTINUE_SEARCH;
        }
    }

    static void OnTerminate()
    {
        if (IsExtensionRelatedCrash(nullptr)) {
            LaunchCrashReporterThread(nullptr);
        } else {
#if defined(DEBUG_CRASH_REPORTER)
            MessageBoxW(NULL, L"OnTerminate: Nothing to report", L"", MB_OK | MB_APPLMODAL);
#endif
            if (PrevTerminateHandler) {
                PrevTerminateHandler();
            }
        }
    }
};

bool CrashReporter::Initialized{ false };
LPTOP_LEVEL_EXCEPTION_FILTER CrashReporter::PrevExceptionFilter{ nullptr };
std::terminate_handler CrashReporter::PrevTerminateHandler{ nullptr };
BacktraceDumper CrashReporter::Dumper;
std::string CrashReporter::Backtrace;
std::string CrashReporter::LuaFullBacktrace;
std::string CrashReporter::LuaDisplayBacktrace;
std::string CrashReporter::ModuleList;
std::string CrashReporter::ErrorLog;

void InitCrashReporting()
{
    CrashReporter::Initialize();
}

void ShutdownCrashReporting()
{
    CrashReporter::Shutdown();
}

LONG HandleGuardedException(DWORD code, EXCEPTION_POINTERS* info)
{
    if (CrashReporter::Initialized || code == 0xE06D7363L /* STATUS_CPP_EH_EXCEPTION */) {
#if defined(DEBUG_CRASH_REPORTER) || defined(ALLOW_DEBUG_CRASH_REPORTING) || !defined(_DEBUG)
        return CrashReporter::OnUnhandledException(info);
#else
        DebugBreak();
        return EXCEPTION_CONTINUE_SEARCH;
#endif
    } else {
        return EXCEPTION_CONTINUE_SEARCH;
    }
}

void DumpCurrentStack()
{
    CrashReporter::Dumper.SnapshotThread();
    DEBUG("%s\r\n", CrashReporter::Dumper.DumpBacktrace().c_str());
}

END_SE()
