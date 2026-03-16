#include <stdafx.h>
#include "GameHelpers.h"
#include "resource.h"
#include "Manifest.h"

#include <Shlwapi.h>
#include <Shlobj.h>
#include <psapi.h>

#include <Config.inl>
#include <ThreadInfo.inl>

extern "C" {
    int default_CSPRNG(uint8_t* dest, unsigned int size)
    {
        bg3se::Fail("Signature verifier should not call CSPRNG");
    }
}

BEGIN_SE()

std::unique_ptr<GameHelpers> gGameHelpers;

void* OSAlloc(std::size_t size)
{
    return malloc(size);
}

void OSFree(void* ptr)
{
    return free(ptr);
}

GameHelpers::GameHelpers()
{
    gCoreLibPlatformInterface.GlobalConsole = new Console();
    gCoreLibPlatformInterface.Alloc = &OSAlloc;
    gCoreLibPlatformInterface.Free = &OSFree;
}

GameHelpers::~GameHelpers()
{
    delete gCoreLibPlatformInterface.GlobalConsole;
    gCoreLibPlatformInterface.GlobalConsole = nullptr;
}

#define SYM_OFF(name) mappings_.StaticSymbols.insert(std::make_pair(#name, SymbolMappings::StaticSymbol{ (int)offsetof(UpdaterSymbols, name) }))

DWORD WINAPI GameHelpers::ShowErrorThreadMain(LPVOID param)
{
    auto self = reinterpret_cast<GameHelpers*>(param);
    MessageBoxA(NULL, self->errorMsg_.c_str(), "Script Extender Updater Error", MB_OK | MB_ICONERROR);

    return 0;
}

void GameHelpers::ShowError(char const * msg)
{
    errorMsg_ = msg;
    CreateThread(NULL, 0, &ShowErrorThreadMain, this, 0, NULL);
}

HMODULE GetExeHandle()
{
    HMODULE hGameModule = GetModuleHandleW(L"bg3.exe");
    if (hGameModule == NULL) {
        hGameModule = GetModuleHandleW(L"bg3_dx11.exe");
    }

    return hGameModule;
}

bool IsInEditor()
{
    // FIXME - editor not released yet!
    return false;
    // return GetModuleHandleW(L"DivinityEngine2.exe") != NULL;
}

std::optional<VersionNumber> GetModuleVersion(HMODULE hModule)
{
    if (hModule == NULL) {
        return {};
    }

    auto hResource = FindResource(hModule, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
    if (hResource == NULL) return {};
    auto dwSize = SizeofResource(hModule, hResource);
    auto hData = LoadResource(hModule, hResource);
    if (hData == NULL) return {};
    auto pRes = LockResource(hData);
    if (pRes == NULL) return {};

    auto pResCopy = LocalAlloc(LMEM_FIXED, dwSize);
    CopyMemory(pResCopy, pRes, dwSize);

    UINT verLength;
    VS_FIXEDFILEINFO* fixedFileInfo;
    if (VerQueryValue(pResCopy, L"\\", (LPVOID*)&fixedFileInfo, &verLength) != TRUE) return {};

    VersionNumber version(
        HIWORD(fixedFileInfo->dwFileVersionMS),
        LOWORD(fixedFileInfo->dwFileVersionMS),
        HIWORD(fixedFileInfo->dwFileVersionLS),
        LOWORD(fixedFileInfo->dwFileVersionLS));

    LocalFree(pResCopy);
    FreeResource(hData);
    return version;
}

std::optional<VersionNumber> GetGameVersion()
{
    HMODULE hGameModule = GetExeHandle();
    if (hGameModule == NULL) {
        return {};
    }

    return GetModuleVersion(hGameModule);
}

std::optional<VersionNumber> GetModuleVersion(std::wstring_view path)
{
    HMODULE hModule = LoadLibraryExW(path.data(), NULL, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE);
    if (hModule == NULL) {
        return {};
    }

    auto version = GetModuleVersion(hModule);
    FreeLibrary(hModule);
    return version;
}

std::string trim(std::string const & s)
{
    size_t first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        first = 0;
    }

    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}

HANDLE UpdaterMutex{ NULL };

bool ShouldLoad()
{
    if (GetExeHandle() == NULL) {
        return false;
    }

    // Check to ensure that multiple instances of the updater were not chain-loaded in some fashion
    // (Rare, but users are surprisingly creative with DLL placement)
    if (UpdaterMutex == NULL) {
        char mutexName[MAX_PATH];
        sprintf_s(mutexName, "BG3SE_Upd_%d", GetCurrentProcessId());
        UpdaterMutex = CreateMutexA(NULL, TRUE, mutexName);
    }

    return UpdaterMutex != NULL;
}

END_SE()
