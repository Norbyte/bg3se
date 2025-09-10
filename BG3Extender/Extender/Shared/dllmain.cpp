#include "stdafx.h"
#include <Extender/Shared/DWriteWrapper.h>
#include <Extender/Shared/ExtenderConfig.h>
#include <Extender/ScriptExtender.h>
#include <CoreLib/JsonLibs.h>
#include <ShellAPI.h>
#include <KnownFolders.h>
#include <ShlObj.h>
#include <sstream>
#include <fstream>
#include <Extender/Shared/ExtenderConfig.inl>

using namespace bg3se;

void SetupScriptExtender(HMODULE hModule)
{
    gExtender = std::make_unique<ScriptExtender>();
    auto & config = gExtender->GetConfig();
    LoadConfig(L"ScriptExtenderSettings.json", config);

    DisableThreadLibraryCalls(hModule);
    if (config.CreateConsole) {
        gCoreLibPlatformInterface.GlobalConsole->Create();
    }

    if (config.DebugFlags == 0) {
        // Disabled: DF_FunctionList, DF_NodeList ,DF_LogSuccessfulFacts, DF_LogFailedFacts, DB_LogFactFailures, DF_DumpDatabases, DF_DebugFacts, DF_LogRuleFailures
        config.DebugFlags = DF_DebugTrace | DF_SuppressInitLog;
    }

    gExtender->Initialize();

#if 0
    DEBUG(" ***** ScriptExtender setup completed ***** ");
#endif
}

HANDLE ExtenderMutex{ NULL };

bool ShouldInitializeExtender()
{
    if (GetModuleHandleW(L"bg3.exe") == NULL && GetModuleHandleW(L"bg3_dx11.exe") == NULL) {
        return false;
    }

    if (ExtenderMutex == NULL) {
        char mutexName[MAX_PATH];
        sprintf_s(mutexName, "BG3SE_Ext_%d", GetCurrentProcessId());
        ExtenderMutex = CreateMutexA(NULL, TRUE, mutexName);
    }

    return ExtenderMutex != NULL;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        bg3se::gCoreLibPlatformInterface.ThisModule = hModule;
        gDWriteWrapper = std::make_unique<DWriteWrapper>();
        if (ShouldInitializeExtender()) {
            SetupScriptExtender(hModule);
        }
        break;

    case DLL_PROCESS_DETACH:
        if (gExtender) {
            gExtender->Shutdown();
            gExtender.reset();
        }
        gDWriteWrapper.reset();
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

