#include "stdafx.h"
#include "Updater.h"
#include "HttpFetcher.h"
#include "resource.h"
#include "ExtenderAPI.h"
#include <Shlwapi.h>
#include <CommCtrl.h>
#include <detours.h>
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <UI.inl>

BEGIN_SE()

std::unordered_set<void const*> gRegisteredTrampolines;

WrappableFunction<ScriptExtenderUpdater::SDLInitTag, int(Uint32)>* WrappableFunction<ScriptExtenderUpdater::SDLInitTag, int(Uint32)>::gHook;

ManifestFetcher::ManifestFetcher(HttpFetcher& fetcher, UpdaterConfig const& config)
    : fetcher_(fetcher), config_(config)
{}
    
OperationResult ManifestFetcher::Fetch(Manifest& manifest)
{
    std::string manifestUrl = config_.ManifestURL + config_.UpdateChannel + "/" + config_.ManifestName;

    DEBUG("Fetching manifest from: %s", manifestUrl.c_str());
    std::vector<char> manifestBody;
    fetcher_.TransferCategory = ErrorCategory::ManifestFetch;
    fetcher_.Timeout = MANIFEST_FETCH_TIMEOUT;
    auto result = fetcher_.Fetch(manifestUrl, manifestBody);
    if (!result) {
        result.error().Message = std::string("Unable to fetch manifest - ") + result.error().Message;
        return result;
    }

    std::string_view manifestStr(manifestBody.data(), manifestBody.size());
    return Parse(manifestStr, manifest);
}
    
OperationResult ManifestFetcher::LoadEmbedded(Manifest& manifest)
{
    auto manifestBinary = GetExeResource(IDR_LOCAL_MANIFEST);
    if (!manifestBinary || manifestBinary->empty()) {
        DEBUG("No embedded manifest found; skipping embedded load");
        return ErrorReason{ "No embedded manifest found" };
    }

    DEBUG("Loading embedded manifest");
    return Parse(*manifestBinary, manifest);
}

OperationResult ManifestFetcher::Parse(std::string_view manifestStr, Manifest& manifest)
{
    ManifestSerializer parser;
    std::string parseError;
    auto result = parser.Parse(manifestStr, manifest);
    if (!result) {
        if (result.error().Category == ErrorCategory::UpdateRequired) {
            result.error().Message = "Unable to parse manifest - update required.";
        } else {
            result.error().Message = std::string("Unable to parse manifest: ") + result.error().Message;
        }
    }

    return result;
}


ResourceUpdater::ResourceUpdater(HttpFetcher& fetcher, UpdaterConfig const& config, ResourceCacheRepository& cache)
    : fetcher_(fetcher), config_(config), cache_(cache)
{}

OperationResult ResourceUpdater::Update(Manifest const& manifest, std::string const& resourceName, VersionNumber const& gameVersion)
{
    DEBUG("Starting fetch for resource: %s", resourceName.c_str());
    auto resIt = manifest.Resources.find(resourceName);
    if (resIt == manifest.Resources.end()) {
        return ErrorReason{ ErrorCategory::LocalLoad, std::string("No manifest entry found for resource: ") + resourceName };
    }

    auto version = manifest.FindResourceVersionWithOverrides(resourceName, gameVersion, config_);
    if (!version) {
        if (!config_.TargetResourceDigest.empty()) {
            return ErrorReason{ ErrorCategory::NoMatchingVersion,
                std::string("The requested Script Extender version ID (") + config_.TargetResourceDigest + ") is not available."
                + "\r\nThis is likely caused by an invalid version setting in ScriptExtenderUpdaterConfig.json" };
        } else if (config_.TargetVersion) {
            return ErrorReason{ ErrorCategory::NoMatchingVersion,
                std::string("The requested Script Extender version (") + config_.TargetVersion->ToString() + ") is not available."
                + "\r\nThis is likely caused by an invalid version setting in ScriptExtenderUpdaterConfig.json" };
        } else {
            return ErrorReason{ ErrorCategory::NoMatchingVersion,
                std::string("No Script Extender version was found that supports your game version (v") + gameVersion.ToString() + ")" };
        }
    }

    if (cache_.LocalResourceExists(resourceName, *version)) {
        DEBUG("Resource already cached locally, skipping update: Version %s, Digest %s", version->Version.ToString().c_str(), version->Digest.c_str());
        return OperationSuccessful{};
    }

    DEBUG("Selected version for update: Version %s, Digest %s", version->Version.ToString().c_str(), version->Digest.c_str());
    DEBUG("Fetch fromn URL: %s", version->URL.c_str());
    return Update(resIt->second, *version);
}

    
OperationResult ResourceUpdater::Update(Manifest::Resource const& resource, Manifest::ResourceVersion const& version)
{
    if (version.Revoked) {
        return ErrorReason{ ErrorCategory::UpdateDownload,
            std::string("Attempted to download revoked resource: ") + version.Digest };
    }

    gUpdater->SetStatusText(std::wstring(L"Downloading update: ") + FromStdUTF8(version.Version.ToString()));
    DEBUG("Fetching update package: %s", version.URL.c_str());
    std::vector<char> response;
    fetcher_.TransferCategory = ErrorCategory::UpdateDownload;
    fetcher_.Timeout = CONTENT_FETCH_TIMEOUT;
    auto result = fetcher_.Fetch(version.URL, response);
    if (!result) {
        result.error().Message = std::string("Unable to download package - ") + result.error().Message;
        return result;
    }

    gUpdater->SetStatusText(std::wstring(L"Unpacking update: ") + FromStdUTF8(version.Version.ToString()));
    return cache_.UpdateLocalPackage(resource, version, std::string_view(response.data(), response.size()));
}

void UpdaterConsole::Print(DebugMessageType type, char const* msg)
{
    Console::Print(type, msg);

    if (gUpdater) {
        gUpdater->GetLog() += msg;
        gUpdater->GetLog() += "\r\n";
    }
}

void ScriptExtenderUpdater::SetGameVersion(int32_t major, int32_t minor, int32_t revision, int32_t build)
{
    gameVersion_ = VersionNumber(major, minor, revision, build);
}

void ScriptExtenderUpdater::LoadCaches()
{
    if (cache_) return;

    UpdatePaths();
    cache_ = std::make_unique<ResourceCacheRepository>(config_, config_.CachePath);
}

void ScriptExtenderUpdater::FetchUpdates()
{
    LoadCaches();

    if (!config_.DisableUpdates) {
        updateResult_ = TryToUpdate();
        // Ensure that we don't keep dangling HTTP connections open to the update server forever
        fetcher_.Cleanup();
    } else {
        updateResult_ = OperationSuccessful{};
    }

    UpdateFromEmbeddedCache();

    if (cancellingUpdate_) {
        // Update failure is due to cancellation, don't show an error message
        updateResult_ = OperationSuccessful{};
    }

    if (config_.DebugLoadSE) {
        DEBUG("Loading SE DLL from local bin");
        launchDllPath_ = GetDebugDllPath();
        launchVersion_ = Manifest::ResourceVersion{}; // Dummy info needed to satisfy the loader
    } else {
        auto resource = cache_->FindLoadableResource(UPDATER_RESOURCE_NAME, gameVersion_);
        if (resource) {
            launchDllPath_ = resource->GetAppDllPath();
            launchVersion_ = resource->GetVersion();
            launchNotice_ = resource->GetVersion().Notice;
        }
    }

    UpdateErrorText();
}

std::wstring ScriptExtenderUpdater::GetDebugDllPath()
{
    wchar_t path[MAX_PATH];
    auto len = GetModuleFileNameW(gCoreLibPlatformInterface.ThisModule, path, std::size(path));
    path[len] = 0;

    auto sep = wcsrchr(path, L'\\');
    if (sep != nullptr) {
        *sep = 0;
    }

    return std::wstring(path) + L'\\' + GAME_DLL;
}

void ScriptExtenderUpdater::UpdateErrorText()
{
    showError_ = false;
    criticalError_ = false;

    if (!updateResult_) {
        showError_ = true;
        errorMessage_ = updateResult_.error().Message;

        auto errorCategory = updateResult_.error().Category;
        DEBUG("Update failed; reason category %d, message: %s", errorCategory, updateResult_.error().Message.c_str());

        switch (errorCategory) {
        case ErrorCategory::UpdateDownload:
            if (updateResult_.error().IsInternetIssue()) {
                errorMessage_ = std::string("Failed to download Script Extender update package. Make sure you're connected to the internet and try again.\r\n") + errorMessage_;
            }
            else {
                errorMessage_ = std::string("Failed to download Script Extender update package.\r\n") + errorMessage_;
            }
            break;

        case ErrorCategory::LocalUpdate:
            errorMessage_ = std::string("Failed to apply Script Extender update:\r\n") + errorMessage_;
            break;

        case ErrorCategory::NoMatchingVersion:
            // Use error message as-is
            break;

        case ErrorCategory::UpdateRequired:
            errorMessage_ = "Failed to check for Script Extender updates:\r\nThe Script Extender launcher is too old and must be re-downloaded.";
            break;

        case ErrorCategory::General:
        case ErrorCategory::ManifestFetch:
        default:
            if (updateResult_.error().IsInternetIssue()) {
                errorMessage_ = std::string("Script Extender update failed; make sure you're connected to the internet and try again.\r\n") + errorMessage_;
            }
            else {
                errorMessage_ = std::string("Script Extender update failed.\r\n") + errorMessage_;
            }
            break;
        }

        // If we found a local fallback version to load, show an update warning; otherwise show an error
        if (launchDllPath_) {
            criticalError_ = false;

            if (launchVersion_
                && (errorCategory == ErrorCategory::UpdateDownload
                || errorCategory == ErrorCategory::ManifestFetch)) {
                errorMessage_ += std::string("\r\n\r\nThe latest downloaded version (v")
                    + std::to_string(launchVersion_->Version.Major)
                    + ") will be loaded.";
            }
        } else {
            criticalError_ = true;
        }
    }

    // Successful update, but local resource could not be found -> critical
    if (!showError_ && !launchDllPath_) {
        showError_ = true;
        criticalError_ = true;
        if (updateManifest_ && !updateManifest_->NoMatchingVersionNotice.empty()) {
            errorMessage_ = updateManifest_->NoMatchingVersionNotice;
        } else {
            errorMessage_ = std::string("No extender version found for game version v") + gameVersion_.ToString();
        }
    }

    if (!showError_ && !launchNotice_.empty()) {
        DEBUG("Notice in manifest resource data: %s", launchNotice_.c_str());
        showError_ = true;
        criticalError_ = false;
        errorMessage_ = launchNotice_;
    }

    if (!showError_ && updateManifest_ && !updateManifest_->Notice.empty()) {
        DEBUG("Notice in manifest: %s", updateManifest_->Notice.c_str());
        showError_ = true;
        criticalError_ = false;
        errorMessage_ = updateManifest_->Notice;
    }

    if (showError_) {
        DEBUG("Update result [%s]: %s", (criticalError_ ? "critical" : "warning"), errorMessage_.c_str());
    } else {
        DEBUG("Update result - no error text");
    }
}

bool ScriptExtenderUpdater::LoadExtender()
{
    if (!launchDllPath_) {
        DEBUG("Unable to launch extender - no loadable DLL path found");
        return false;
    }

    DEBUG("Loading extender DLL: %s", ToStdUTF8(*launchDllPath_).c_str());
    HMODULE handle = LoadLibraryExW(launchDllPath_->c_str(), NULL, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_APPLICATION_DIR | LOAD_LIBRARY_SEARCH_SYSTEM32);

    if (handle == NULL) {
        if (!criticalError_) {
            auto errc = GetLastError();
            DEBUG("Extender DLL load failed; error code %d", errc);
            showError_ = true;
            criticalError_ = false;
            errorMessage_ = "Failed to load Script Extender library.\r\n"
                "LoadLibrary() returned error code ";
            errorMessage_ += std::to_string(errc);
        }

        return false;
    } else {
        return true;
    }
}

void ScriptExtenderUpdater::Run()
{
    FetchUpdates();
    if (ui_) {
        ui_->Hide();
    }
    LoadExtender();
    completed_ = true;

    if (showError_
        // Allow passing non-critical errors to SE for ingame display
        && (criticalError_ || !(apiCapabilities_ & CapErrorReporting)))
    {
        gGameHelpers->ShowError(errorMessage_.c_str(), !criticalError_);
    }
}
    
OperationResult ScriptExtenderUpdater::TryToUpdate()
{
    cancellingUpdate_ = false;

    ManifestFetcher manifestFetcher(fetcher_, config_);
    Manifest manifest;
    SetStatusText(L"Fetching manifest");
    auto result = manifestFetcher.Fetch(manifest);
    if (!result) {
        return result;
    }

    if (cancellingUpdate_) {
        return ErrorReason{ ErrorCategory::Canceled, "" };
    }

    cache_->UpdateFromManifest(manifest);
    cache_->SaveManifestIfNecessary();

    updateManifest_ = manifest;
    ResourceUpdater updater(fetcher_, config_, *cache_);
    return updater.Update(manifest, UPDATER_RESOURCE_NAME, gameVersion_);
}
    
void ScriptExtenderUpdater::UpdateFromEmbeddedCache()
{
    Manifest manifest;
    ManifestFetcher manifestFetcher(fetcher_, config_);
    auto result = manifestFetcher.LoadEmbedded(manifest);
    if (!result) {
        DEBUG("Failed to load embedded manifest: %s", result.error().Message.c_str());
        return;
    }

    // Pick a version to load from the local cache
    auto localRes = cache_->GetManifest().FindResourceVersionWithOverrides(UPDATER_RESOURCE_NAME, gameVersion_, config_);

    // Pick a version to load from the embedded manifest
    auto embeddedRes = manifest.FindResourceVersionWithOverrides(UPDATER_RESOURCE_NAME, gameVersion_, config_);

    if (!embeddedRes) {
        DEBUG("Not extracting embedded resource - doesn't match current game version");
        return;
    }

    // Don't unpack if the embedded resource is already available
    if (localRes && localRes->Digest == embeddedRes->Digest) {
        DEBUG("Not extracting embedded resource - already available in local cache");
        return;
    }

    // Don't unpack if the embedded resource is not newer than the one in the local cache
    if (localRes && *embeddedRes < *localRes) {
        DEBUG("Not extracting embedded resource - less recent than version in local cache");
        return;
    }

    auto packageBinary = GetExeResource(IDR_LOCAL_SE_PACKAGE);
    if (!packageBinary || packageBinary->empty()) {
        DEBUG("Not extracting embedded resource - embedded package blob is missing or empty");
        return;
    }

    auto const& resource = manifest.Resources[UPDATER_RESOURCE_NAME];
    cache_->UpdateLocalPackage(resource, *embeddedRes, *packageBinary);
}

bool ScriptExtenderUpdater::IsCompleted() const
{
    return completed_;
}

void ScriptExtenderUpdater::InitConsole()
{
    if (!config_.Debug) return;

    gCoreLibPlatformInterface.GlobalConsole->Create();
    gCoreLibPlatformInterface.GlobalConsole->EnableOutput(true);
    SetConsoleTitleW(L"BG3 Script Extender Debug Console");
}

void ScriptExtenderUpdater::InitUI()
{
    ui_ = std::make_unique<UpdaterUI>();
    ui_->Show();
}

void ScriptExtenderUpdater::SetStatusText(std::wstring const& status)
{
    if (ui_) {
        ui_->SetStatusText(status);
    }
}

void ScriptExtenderUpdater::RequestCancelUpdate()
{
    cancellingUpdate_ = true;
    fetcher_.Cancel();
}

void ScriptExtenderUpdater::Initialize(std::string_view exeDirOverride)
{
    SetStatusText(L"Initializing");
    UpdateExeDir(exeDirOverride);
    LoadConfig();
    LoadGameVersion();
    HookSDL();
}

void ScriptExtenderUpdater::UpdateExeDir(std::string_view exeDirOverride)
{
    if (!exeDirOverride.empty()) {
        exeDir_ = FromStdUTF8(exeDirOverride);
    } else {
        HMODULE hGameModule = GetExeHandle();
        if (hGameModule != NULL) {
            exeDir_.resize(MAX_PATH);
            DWORD modulePathSize = GetModuleFileNameW(hGameModule, exeDir_.data(), (DWORD)exeDir_.size());
            exeDir_.resize(modulePathSize);
            auto sep = exeDir_.find_last_of(L'\\');
            if (sep != std::string::npos) {
                exeDir_ = exeDir_.substr(0, sep);
            }
        }
    }
}

void ScriptExtenderUpdater::LoadConfig()
{
    std::wstring configPath;
    if (!exeDir_.empty()) {
        configPath = exeDir_ + L"\\" UPDATER_CONFIG_FILE;
    } else {
        configPath = UPDATER_CONFIG_FILE;
    }
        
    LoadConfigFile(configPath, config_);

    if (IsInEditor()) {
        config_.UpdateChannel += UPDATER_CHANNEL_EDITOR;
    } else {
        config_.UpdateChannel += UPDATER_CHANNEL_GAME;
    }

#if defined(HAS_DEBUG_LOGGING)
    // Enable debug logging if shift is down when the game is launched
    if (GetAsyncKeyState(VK_SHIFT) < 0) {
        config_.Debug = true;
    }
#endif

    fetcher_.DebugLogging = config_.Debug;
    fetcher_.IPv4Only = config_.IPv4Only;
}

void ScriptExtenderUpdater::LoadGameVersion()
{
    auto version = GetGameVersion();
    if (!version && !exeDir_.empty()) {
        auto exePath = exeDir_;
        exePath += L"\\bg3.exe";
        version = GetModuleVersion(exePath);
    }

    if (version) {
        gameVersion_ = *version;
    }
}

void ScriptExtenderUpdater::HookSDL()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    SDLInitHook.Wrap(ResolveFunctionTrampoline(&SDL_Init));
    DetourTransactionCommit();
    SDLInitHook.SetPostHook(&ScriptExtenderUpdater::OnSDLInit, this);
}

void ScriptExtenderUpdater::UnhookSDL()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    SDLInitHook.Unwrap();
    DetourTransactionCommit();
}

void ScriptExtenderUpdater::OnSDLInit(Uint32, int)
{
    while (needsClientSuspend_) {
        Sleep(10);
    }

    UnhookSDL();
}

void ScriptExtenderUpdater::UpdatePaths()
{
    auto cacheDir = config_.CachePath;
    if (!PathFileExistsW(cacheDir.c_str())) {
        CreateDirectoryW(cacheDir.c_str(), NULL);
    }

    DEBUG("Cache path: %s", ToStdUTF8(cacheDir).c_str());
    DEBUG("Update channel: %s", config_.UpdateChannel.c_str());
}

std::unique_ptr<ScriptExtenderUpdater> gUpdater;

DWORD WINAPI UpdaterThread(LPVOID param)
{
    gUpdater->InitConsole();
    gUpdater->InitUI();
    DEBUG("Launch loader");
    gUpdater->Run();
    gUpdater->RequestClientSuspend(false);
    DEBUG("Extender launcher thread exiting");
    return 0;
}

void StartUpdaterThread()
{
    gGameHelpers = std::make_unique<GameHelpers>();

    gUpdater = std::make_unique<ScriptExtenderUpdater>();
    gUpdater->Initialize({});
    gUpdater->RequestClientSuspend(true);
    CreateThread(NULL, 0, &UpdaterThread, NULL, 0, NULL);
}


END_SE()
 