#pragma once

#include <CoreLib/Console.h>

BEGIN_SE()

enum class UpdaterAPIVersion : uint32_t
{
    VerInitialAPI = 7
};

// Indicates what features the loaded SE supports
enum UpdaterAPICapability : uint32_t
{
    // Supports displaying error messages ingame (disables the updater error display logic)
    CapErrorReporting = 1 << 0
};

enum UpdaterAPIErrorFlags : uint32_t
{
    ErrShowError = 1 << 0,
    ErrIsCritical = 1 << 1,
};

struct UpdaterAPI
{
    using InitializeLibraryProc = void ();
    using GetVersionProc = UpdaterAPIVersion ();
    using SetLogCallbackProc = void (Console::LogCallbackProc* callback);
    using InitializeProc = void (char const* configPath);
    using ShowConsoleProc = void ();
    using SetGameVersionProc = void (int32_t major, int32_t minor, int32_t revision, int32_t build);
    using UpdateProc = bool ();
    using GetErrorProc = void(char* buf, uint32_t* length);
    using GetErrorFlagsProc = uint32_t ();
    using SetCapabilitiesProc = void (uint32_t capabilities);
    using GetLogProc = void (char* buf, uint32_t* length);
    using GetResourceVersionProc = bool (int32_t* major, int32_t* minor, int32_t* revision, int32_t* build);
    using ShutdownProc = void ();

    InitializeLibraryProc* InitializeLibrary{ nullptr };
    GetVersionProc* GetVersion{ nullptr };
    SetLogCallbackProc* SetLogCallback{ nullptr };
    InitializeProc* Initialize{ nullptr };
    ShowConsoleProc* ShowConsole{ nullptr };
    SetGameVersionProc* SetGameVersion{ nullptr };
    UpdateProc* Update{ nullptr };
    GetErrorProc* GetError{ nullptr };
    GetErrorFlagsProc* GetErrorFlags{ nullptr };
    SetCapabilitiesProc* SetCapabilities{ nullptr };
    GetLogProc* GetLog{ nullptr };
    GetResourceVersionProc* GetResourceVersion{ nullptr };
    ShutdownProc* Shutdown{ nullptr };

    bool Init(HMODULE module);
};

END_SE()
