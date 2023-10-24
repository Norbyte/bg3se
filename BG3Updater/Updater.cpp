#include "stdafx.h"
#include "Updater.h"
#include "HttpFetcher.h"
#include <Shlwapi.h>

BEGIN_SE()

ManifestFetcher::ManifestFetcher(UpdaterConfig const& config)
	: config_(config)
{}
	
bool ManifestFetcher::Fetch(Manifest& manifest, ErrorReason& reason)
{
	HttpFetcher fetcher;
	fetcher.DebugLogging = config_.Debug;
	fetcher.IPv4Only = config_.IPv4Only;
	std::string manifestUrl = config_.ManifestURL + config_.UpdateChannel + "/" + config_.ManifestName;

	DEBUG("Fetching manifest from: %s", manifestUrl.c_str());
	std::vector<uint8_t> manifestBinary;
	if (!fetcher.Fetch(manifestUrl, manifestBinary)) {
		reason.Category = ErrorCategory::ManifestFetch;
		reason.Message = "Unable to download manifest: ";
		reason.Message += fetcher.GetLastError();
		reason.CurlResult = fetcher.GetLastResultCode();
		return false;
	}

	std::string manifestStr((char*)manifestBinary.data(), (char*)manifestBinary.data() + manifestBinary.size());
	return Parse(manifestStr, manifest, reason);
}

bool ManifestFetcher::Parse(std::string const& manifestStr, Manifest& manifest, ErrorReason& reason)
{
	ManifestSerializer parser;
	std::string parseError;
	auto result = parser.Parse(manifestStr, manifest, parseError);
	if (result == ManifestParseResult::Failed) {
		reason.Category = ErrorCategory::General;
		reason.Message = "Unable to parse manifest: ";
		reason.Message += parseError;
		return false;
	}

	if (result == ManifestParseResult::UpdateRequired) {
		reason.Category = ErrorCategory::UpdateRequired;
		reason.Message = "Unable to parse manifest - update required.";
		return false;
	}

	return true;
}


ResourceUpdater::ResourceUpdater(UpdaterConfig const& config, ResourceCacheRepository& cache)
	: config_(config), cache_(cache)
{}

bool ResourceUpdater::Update(Manifest const& manifest, std::string const& resourceName, VersionNumber const& gameVersion, ErrorReason& reason)
{
	DEBUG("Starting fetch for resource: %s", resourceName.c_str());
	auto resIt = manifest.Resources.find(resourceName);
	if (resIt == manifest.Resources.end()) {
		reason.Message = "No manifest entry found for resource: ";
		reason.Message += resourceName;
		return false;
	}

	auto version = resIt->second.FindResourceVersionWithOverrides(gameVersion, config_);
	if (!version) {
		if (!config_.TargetResourceDigest.empty()) {
			reason.Message = "Script extender digest not found in manifest: ";
			reason.Message += config_.TargetResourceDigest;
		} else if (!config_.TargetVersion.empty()) {
			reason.Message = "Script extender version not found in manifest: ";
			reason.Message += config_.TargetVersion;
		} else {
			reason.Message = "Script extender not available for game version v";
			reason.Message += gameVersion.ToString();
		}
		return false;
	}

	if (cache_.ResourceExists(resIt->first, *version)) {
		DEBUG("Resource already cached locally, skipping update: Version %s, Digest %s", version->Version.ToString().c_str(), version->Digest.c_str());
		return true;
	}

	DEBUG("Selected version for update: Version %s, Digest %s", version->Version.ToString().c_str(), version->Digest.c_str());
	DEBUG("Fetch fromn URL: %s", version->URL.c_str());
	return Update(resIt->second, *version, reason);
}

	
bool ResourceUpdater::Update(Manifest::Resource const& resource, Manifest::ResourceVersion const& version, ErrorReason& reason)
{
	if (version.Revoked) {
		reason.Category = ErrorCategory::UpdateDownload;
		reason.Message = "Attempted to download unavailable resource version.";
		return false;
	}

	HttpFetcher fetcher;
	fetcher.DebugLogging = config_.Debug;
	fetcher.IPv4Only = config_.IPv4Only;

	DEBUG("Fetching update package: %s", version.URL.c_str());
	std::vector<uint8_t> response;
	if (!fetcher.Fetch(version.URL, response)) {
		reason.Category = ErrorCategory::UpdateDownload;
		reason.Message = "Unable to download package: ";
		reason.Message += fetcher.GetLastError();
		reason.CurlResult = fetcher.GetLastResultCode();
		return false;
	}

	if (cache_.UpdateLocalPackage(resource, version, response, reason.Message)) {
		return true;
	} else {
		reason.Category = ErrorCategory::LocalUpdate;
		return false;
	}
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

bool ScriptExtenderUpdater::FetchUpdates()
{
	LoadCaches();

	ErrorReason updateReason;
	if (!config_.DisableUpdates) {
		updated_ = TryToUpdate(updateReason);
	} else {
		updated_ = true;
	}

	launchDllPath_ = cache_->FindResourcePath("ScriptExtender", gameVersion_);

	bool showError = !updated_;
	if (updated_ && !launchDllPath_) {
		showError = true;
		if (updateManifest_ && !updateManifest_->NoMatchingVersionNotice.empty()) {
			updateReason.Message = updateManifest_->NoMatchingVersionNotice;
		} else {
			updateReason.Message = "No extender version found for game version v";
			updateReason.Message += gameVersion_.ToString();
		}
	}

	if (showError) {
		DEBUG("Update failed; reason category %d, message: %s", updateReason.Category, updateReason.Message.c_str());

		switch (updateReason.Category) {
		case ErrorCategory::UpdateDownload:
			if (updateReason.IsInternetIssue()) {
				errorMessage_ = std::string("Failed to download Script Extender update package. Make sure you're connected to the internet and try again.\r\n") + updateReason.Message;
			} else {
				errorMessage_ = std::string("Failed to download Script Extender update package.\r\n") + updateReason.Message;
			}
			break;

		case ErrorCategory::LocalUpdate:
			errorMessage_ = std::string("Failed to apply Script Extender update:\r\n") + updateReason.Message;
			break;

		case ErrorCategory::UpdateRequired:
			errorMessage_ = "Failed to check for Script Extender updates:\r\nThe Script Extender launcher is too old and must be re-downloaded.";
			break;

		case ErrorCategory::General:
		case ErrorCategory::ManifestFetch:
		default:
			if (updateReason.IsInternetIssue()) {
				errorMessage_ = std::string("Script Extender update failed; make sure you're connected to the internet and try again.\r\n") + updateReason.Message;
			} else {
				errorMessage_ = std::string("Script Extender update failed.\r\n") + updateReason.Message;
			}
			break;
		}
	}

	if (launchDllPath_) {
		auto ver = cache_->FindResourceVersion("ScriptExtender", gameVersion_);
		if (ver && errorMessage_.empty() && !ver->Notice.empty()) {
			DEBUG("Notice in manifest resource data: %s", ver->Notice.c_str());
			errorMessage_ = ver->Notice;
		}
	}

	if (errorMessage_.empty() && updateManifest_ && !updateManifest_->Notice.empty()) {
		DEBUG("Notice in manifest: %s", updateManifest_->Notice.c_str());
		errorMessage_ = updateManifest_->Notice;
	}

	return !showError;
}

bool ScriptExtenderUpdater::LoadExtender()
{
	auto dllPath = cache_->FindResourceDllPath("ScriptExtender", gameVersion_);
	if (!dllPath) {
		errorMessage_ = "No extender version found for game version v";
		errorMessage_ += gameVersion_.ToString();
		return false;
	}

	DEBUG("Loading extender DLL: %s", ToStdUTF8(*dllPath).c_str());
	HMODULE handle = LoadLibraryExW(dllPath->c_str(), NULL, LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_APPLICATION_DIR);

	if (handle == NULL) {
		if (updated_) {
			auto errc = GetLastError();
			DEBUG("Extender DLL load failed; error code %d", errc);
			errorMessage_ = "Failed to load Script Extender library.\r\n"
				"LoadLibrary() returned error code ";
			errorMessage_ += std::to_string(errc);
		}

		return false;
	} else {
		// Wait a bit for extender startup to complete
		Sleep(300);
		return true;
	}
}

void ScriptExtenderUpdater::Run()
{
	FetchUpdates();
	LoadExtender();
	completed_ = true;

	if (!errorMessage_.empty()) {
		gGameHelpers->ShowError(errorMessage_.c_str());
	}
}
	
bool ScriptExtenderUpdater::TryToUpdate(ErrorReason& reason)
{
	ManifestFetcher manifestFetcher(config_);
	Manifest manifest;
	if (!manifestFetcher.Fetch(manifest, reason)) {
		return false;
	}

	cache_->UpdateFromManifest(manifest);

	updateManifest_ = manifest;
	ResourceUpdater updater(config_, *cache_);
	return updater.Update(manifest, "ScriptExtender", gameVersion_, reason);
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

void ScriptExtenderUpdater::Initialize(char const* exeDirOverride)
{
	UpdateExeDir(exeDirOverride);
	LoadConfig();
	LoadGameVersion();
}

void ScriptExtenderUpdater::UpdateExeDir(char const* exeDirOverride)
{
	if (exeDirOverride && *exeDirOverride) {
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
		configPath = exeDir_ + L"\\ScriptExtenderUpdaterConfig.json";
	} else {
		configPath = L"ScriptExtenderUpdaterConfig.json";
	}
		
	LoadConfigFile(configPath, config_);

	if (IsInEditor()) {
		config_.UpdateChannel += UPDATER_CHANNEL_EDITOR;
	} else {
		config_.UpdateChannel += UPDATER_CHANNEL_GAME;
	}
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


// This thread is responsible for polling and suspending/resuming
// the client init thread if the update is still pending during client init.
// The goal is to prevent the client from loading modules before the extender is loaded.
DWORD WINAPI ClientWorkerSuspenderThread(LPVOID param)
{
	bool suspended{ false };
	for (;;) {
		auto state = gGameHelpers->GetState();
		if (state) {
			bool completed = gUpdater->IsCompleted();
			if (!suspended && !completed && (*state == ecl::GameState::LoadModule || *state == ecl::GameState::Init)) {
				DEBUG("Suspending client thread (pending update)");
				gGameHelpers->SuspendClientThread();
				suspended = true;
			}

			if (completed) {
				if (suspended) {
					DEBUG("Resuming client thread");
					gGameHelpers->ResumeClientThread();
				}
				break;
			}

			if (*state == ecl::GameState::Menu) {
				// No update takes place once we reach the menu, exit thread
				break;
			}
		}

		Sleep(1);
	}

	DEBUG("Client suspend worker exiting");
	return 0;
}

DWORD WINAPI UpdaterThread(LPVOID param)
{
	gGameHelpers = std::make_unique<GameHelpers>();
	gUpdater = std::make_unique<ScriptExtenderUpdater>();
	gUpdater->Initialize(nullptr);
	gUpdater->InitConsole();
	gGameHelpers->Initialize();
	CreateThread(NULL, 0, &ClientWorkerSuspenderThread, NULL, 0, NULL);
	DEBUG("Launch loader");
	gUpdater->Run();
	DEBUG("Extender launcher thread exiting");
	return 0;
}

void StartUpdaterThread()
{
	CreateThread(NULL, 0, &UpdaterThread, NULL, 0, NULL);
}


END_SE()
