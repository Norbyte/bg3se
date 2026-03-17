#include "stdafx.h"
#include "Updater.h"
#include "ExtenderAPI.h"

BEGIN_SE()

void SEUpdaterInitializeLibrary()
{
    if (!gGameHelpers) {
        gGameHelpers = std::make_unique<GameHelpers>();
    }
}

UpdaterAPIVersion SEUpdaterGetVersion()
{
    return UpdaterAPIVersion::VerInitialAPI;
}

void SEUpdaterSetLogCallback(Console::LogCallbackProc* callback)
{
    if (gCoreLibPlatformInterface.GlobalConsole) {
        auto console = static_cast<Console*>(gCoreLibPlatformInterface.GlobalConsole);
        console->SetLogCallback(callback);
    }
}

void SEUpdaterInitialize(char const* binPath)
{
    if (gUpdater) return;

    if (!gGameHelpers) {
        gGameHelpers = std::make_unique<GameHelpers>();
    }

    gUpdater = std::make_unique<ScriptExtenderUpdater>();
    gUpdater->Initialize(binPath);
}

void SEUpdaterShowConsole()
{
    if (!gUpdater) return;

    gUpdater->GetConfig().Debug = true;
    gUpdater->InitConsole();
}

void SESetGameVersion(int32_t major, int32_t minor, int32_t revision, int32_t build)
{
    if (!gUpdater) return;

    gUpdater->SetGameVersion(major, minor, revision, build);
}

bool SEUpdate()
{
    if (!gUpdater) return false;

    gUpdater->FetchUpdates();
    return gUpdater->ShowError();
}

void SEUpdaterGetError(char* buf, uint32_t* length)
{
    if (!gUpdater) {
        *length = 0;
        return;
    }

    *length = (uint32_t)gUpdater->GetErrorMessage().size();
    if (buf != nullptr) {
        std::copy(gUpdater->GetErrorMessage().begin(), gUpdater->GetErrorMessage().end(), buf);
    }
}

uint32_t SEUpdaterGetErrorFlags()
{
    if (!gUpdater) return 0;

    return (gUpdater->ShowError() ? ErrShowError : 0)
        | (gUpdater->IsCriticalError() ? ErrIsCritical : 0);
}

void SEUpdaterSetCapabilities(uint32_t capabilities)
{
    if (gUpdater) {
        gUpdater->SetAPICapabilities(capabilities);
    }
}

void SEUpdaterGetLog(char* buf, uint32_t* length)
{
    if (!gUpdater) {
        *length = 0;
        return;
    }

    *length = (uint32_t)gUpdater->GetLog().size();
    if (buf != nullptr) {
        std::copy(gUpdater->GetLog().begin(), gUpdater->GetLog().end(), buf);
    }
}

bool SEUpdaterGetResourceVersion(int32_t* major, int32_t* minor, int32_t* revision, int32_t* build)
{
    if (!gUpdater) return false;

    gUpdater->LoadCaches();
    auto ver = gUpdater->GetCache()->FindResourceVersion(UPDATER_RESOURCE_NAME, gUpdater->GetCachedGameVersion());
    if (ver) {
        *major = ver->Version.Major;
        *minor = ver->Version.Minor;
        *revision = ver->Version.Revision;
        *build = ver->Version.Build;
        return true;
    } else {
        return false;
    }
}

void SEUpdaterShutdown()
{
    if (gUpdater) {
        gUpdater.reset();
    }
    
    if (gGameHelpers) {
        gGameHelpers.reset();
    }
}

void SEUpdaterTest()
{
    SEUpdaterInitialize(nullptr);
    SEUpdaterShowConsole();
    SESetGameVersion(4, 37, 67, 641);
    SEUpdate();
    SEUpdaterShutdown();
}

END_SE()

extern "C"
{

void SEUpdaterInitializeLibrary()
{
    bg3se::SEUpdaterInitializeLibrary();
}

bg3se::UpdaterAPIVersion SEUpdaterGetVersion()
{
    return bg3se::SEUpdaterGetVersion();
}

void SEUpdaterSetLogCallback(bg3se::Console::LogCallbackProc* callback)
{
    bg3se::SEUpdaterSetLogCallback(callback);
}

void SEUpdaterInitialize(char const* configPath)
{
    bg3se::SEUpdaterInitialize(configPath);
}

void SEUpdaterShowConsole()
{
    bg3se::SEUpdaterShowConsole();
}

void SESetGameVersion(int32_t major, int32_t minor, int32_t revision, int32_t build)
{
    bg3se::SESetGameVersion(major, minor, revision, build);
}

bool SEUpdate()
{
    return bg3se::SEUpdate();
}

void SEUpdaterGetError(char* buf, uint32_t* length)
{
    bg3se::SEUpdaterGetError(buf, length);
}

uint32_t SEUpdaterGetErrorFlags()
{
    return bg3se::SEUpdaterGetErrorFlags();
}

void SEUpdaterSetCapabilities(uint32_t capabilities)
{
    return bg3se::SEUpdaterSetCapabilities(capabilities);
}

void SEUpdaterGetLog(char* buf, uint32_t* length)
{
    bg3se::SEUpdaterGetLog(buf, length);
}

bool SEUpdaterGetResourceVersion(int32_t* major, int32_t* minor, int32_t* revision, int32_t* build)
{
    return bg3se::SEUpdaterGetResourceVersion(major, minor, revision, build);
}

void SEUpdaterTest()
{
    bg3se::SEUpdaterTest();
}

void SEUpdaterShutdown()
{
    bg3se::SEUpdaterShutdown();
}

}
