#include "ExtenderAPI.h"

BEGIN_SE()

bool UpdaterAPI::Init(HMODULE module)
{
    GetVersion = (GetVersionProc*)GetProcAddress(module, "SEUpdaterGetVersion");
    if (!GetVersion) {
        DEBUG("Updater API unavailable (SEUpdaterGetVersion export not found)");
        return false;
    }

    if (GetVersion() < UpdaterAPIVersion::VerInitialAPI) {
        DEBUG("Updater API unavailable (SEUpdaterGetVersion returned legacy version)");
        return false;
    }

    InitializeLibrary = (InitializeLibraryProc*)GetProcAddress(module, "SEUpdaterInitializeLibrary");
    SetLogCallback = (SetLogCallbackProc*)GetProcAddress(module, "SEUpdaterSetLogCallback");
    Initialize = (InitializeProc*)GetProcAddress(module, "SEUpdaterInitialize");
    ShowConsole = (ShowConsoleProc*)GetProcAddress(module, "SEUpdaterShowConsole");
    SetGameVersion = (SetGameVersionProc*)GetProcAddress(module, "SESetGameVersion");
    Update = (UpdateProc*)GetProcAddress(module, "SEUpdate");
    GetError = (GetErrorProc*)GetProcAddress(module, "SEUpdaterGetError");
    GetErrorFlags = (GetErrorFlagsProc*)GetProcAddress(module, "SEUpdaterGetErrorFlags");
    SetCapabilities = (SetCapabilitiesProc*)GetProcAddress(module, "SEUpdaterSetCapabilities");
    GetLog = (GetLogProc*)GetProcAddress(module, "SEUpdaterGetLog");
    GetResourceVersion = (GetResourceVersionProc*)GetProcAddress(module, "SEUpdaterGetResourceVersion");
    Shutdown = (ShutdownProc*)GetProcAddress(module, "SEUpdaterShutdown");

    return true;
}

END_SE()
