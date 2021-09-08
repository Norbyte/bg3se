#include "stdafx.h"

#include "DWriteWrapper.h"
#include "HttpFetcher.h"
#include "ErrorUtils.h"
#include "Crypto.h"
#include <ZipLib/ZipArchive.h>
#include <ZipLib/ZipFile.h>
#include "json/json.h"
#include "Manifest.h"

#include <Shlwapi.h>
#include <Shlobj.h>

#include <vector>
#include <thread>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>

#define UPDATER_MANIFEST_URL "https://dzh4fh4qphx4q.cloudfront.net/Channels/"
#define UPDATER_MANIFEST_NAME "Manifest2.json"
#define UPDATER_CHANNEL "Release"
#define APP_DLL L"BG3ScriptExtender.dll"

extern "C" {

int default_CSPRNG(uint8_t* dest, unsigned int size)
{
	Fail("Signature verifier should not call CSPRNG");
}

}

std::optional<VersionNumber> GetGameVersion()
{
	HMODULE hGameModule = GetModuleHandleW(L"bg3.exe");
	if (hGameModule == NULL) {
		hGameModule = GetModuleHandleW(L"bg3_dx11.exe");
	}

	if (hGameModule == NULL) {
		return {};
	}

	auto hResource = FindResource(hGameModule, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
	if (hResource == NULL) return {};
	auto dwSize = SizeofResource(hGameModule, hResource);
	auto hData = LoadResource(hGameModule, hResource);
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

void ConfigGetBool(Json::Value& node, char const* key, bool& value)
{
	auto configVar = node[key];
	if (!configVar.isNull() && configVar.isBool()) {
		value = configVar.asBool();
	}
}

void ConfigGetString(Json::Value& node, char const* key, std::wstring& value)
{
	auto configVar = node[key];
	if (!configVar.isNull() && configVar.isString()) {
		value = FromUTF8(configVar.asString());
	}
}

void ConfigGetString(Json::Value& node, char const* key, std::string& value)
{
	auto configVar = node[key];
	if (!configVar.isNull() && configVar.isString()) {
		value = configVar.asString();
	}
}

std::wstring GetDefaultCachePath()
{
	TCHAR appDataPath[MAX_PATH];
	if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appDataPath)))
	{
		return L"";
	}

	return std::wstring(appDataPath) + L"\\BG3ScriptExtender";
}

void LoadConfigFile(std::wstring const& configPath, UpdaterConfig& config)
{
	config.ManifestURL = UPDATER_MANIFEST_URL;
	config.ManifestName = UPDATER_MANIFEST_NAME;
	config.UpdateChannel = UPDATER_CHANNEL;
	config.CachePath = GetDefaultCachePath();
#if defined(_DEBUG)
	config.Debug = true;
#else
	config.Debug = false;
#endif
	config.ValidateSignature = true;

	std::ifstream f(configPath, std::ios::in);
	if (!f.good()) {
		return;
	}

	Json::CharReaderBuilder factory;
	Json::Value root;
	std::string errs;
	if (!Json::parseFromStream(factory, f, &root, &errs)) {
		std::wstring werrs = FromUTF8(errs);

		std::wstringstream err;
		err << L"Failed to load configuration file '" << configPath << "':\r\n" << werrs;
		Fail(ToUTF8(err.str()).c_str());
	}

	ConfigGetString(root, "ManifestURL", config.ManifestURL);
	ConfigGetString(root, "ManifestName", config.ManifestName);
	ConfigGetString(root, "UpdateChannel", config.UpdateChannel);
	ConfigGetString(root, "CachePath", config.CachePath);
#if defined(HAS_DEBUG_LOGGING)
	ConfigGetBool(root, "Debug", config.Debug);
	ConfigGetBool(root, "ValidateSignature", config.ValidateSignature);
#endif
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


class CachedResource
{
public:
	CachedResource(std::wstring const& cachePath, Manifest::Resource const& resource, Manifest::ResourceVersion const& version)
		: cachePath_(cachePath), resource_(resource), version_(version)
	{}

	std::wstring GetResourceLocalPath() const
	{
		return cachePath_ + L"\\" + FromUTF8(resource_.Name);
	}

	std::wstring TryCreateLocalResourceCacheDirectory()
	{
		auto path = GetResourceLocalPath();
		TryCreateDirectory(path);
		return path;
	}

	std::wstring GetLocalPath() const
	{
		return GetResourceLocalPath() + L"\\" + FromUTF8(version_.Version.ToString()) + L"_" + FromUTF8(version_.Digest);
	}

	std::wstring GetLocalPackagePath() const
	{
		return GetLocalPath() + L".package";
	}

	std::wstring TryCreateLocalCacheDirectory()
	{
		TryCreateLocalResourceCacheDirectory();
		auto path = GetLocalPath();
		TryCreateDirectory(path);
		return path;
	}

	bool UpdateLocalPackage(std::vector<uint8_t> const& contents, std::string& reason)
	{
		TryCreateLocalResourceCacheDirectory();
		auto packagePath = GetLocalPackagePath();
		DEBUG("Saving update package to: %s", ToUTF8(packagePath).c_str());

		// Check if any of the files are currently in use by the game.
		// The shell Zip API won't tell us if it failed to overwrite one of the files, so we need to 
		// check beforehand that the files are writeable.
		if (!AreDllsWriteable()) {
			return false;
		}

		auto tempPath = packagePath + L".tmp";
		if (!SaveFile(tempPath, contents)) {
			DEBUG("Unable to write package temp file: %s", ToUTF8(tempPath).c_str());
			reason = "Script Extender update failed:\r\n";
			reason += std::string("Failed to write file ") + ToUTF8(tempPath);
			return false;
		}

		if (!CryptoUtils::VerifySignedFile(tempPath, reason)) {
			DEBUG("Unable to verify package signature: %s", reason.c_str());
			return false;
		}

		if (!MoveFileExW(tempPath.c_str(), packagePath.c_str(), MOVEFILE_REPLACE_EXISTING)) {
			DEBUG("Failed to move package file %s", packagePath.c_str());
			reason = "Script Extender update failed:\r\n";
			reason += std::string("Failed to move file ") + ToUTF8(packagePath);
			return false;
		}


		std::string unzipReason;
		auto cachePath = TryCreateLocalCacheDirectory();
		DEBUG("Unpacking update to %s", ToUTF8(cachePath).c_str());
		if (UnzipPackage(packagePath, cachePath, reason)) {
			return true;
		} else {
			DEBUG("Unzipping failed: %s", reason.c_str());
			return false;
		}
	}

	bool RemoveLocalPackage()
	{
		TryCreateLocalResourceCacheDirectory();
		auto packagePath = GetLocalPackagePath();
		DEBUG("Removing local package: %s", ToUTF8(packagePath).c_str());
		auto localPath = GetLocalPath();
		bool ok = DeleteLocalCacheFromZip(packagePath, localPath);
		DeleteFileW(packagePath.c_str());
		return ok;
	}

	bool ExtenderDLLExists()
	{
		auto dllPath = GetLocalPath() + L"\\" + APP_DLL;
		return PathFileExists(dllPath.c_str());
	}

private:
	std::wstring cachePath_;
	Manifest::Resource const& resource_;
	Manifest::ResourceVersion const& version_;

	bool AreDllsWriteable()
	{
		auto dllPath = GetLocalPath() + L"\\" + APP_DLL;
		if (PathFileExistsW(dllPath.c_str())) {
			std::ofstream f;
			f.open(dllPath.c_str(), std::ios::out | std::ios::app, _SH_DENYRW);
			if (!f.good()) {
				DEBUG("Extender DLL not writeable, skipping update.");
				return false;
			}
		}

		DEBUG("DLL write check OK");
		return true;
	}

	bool UnzipPackage(std::wstring const& zipPath, std::wstring const& resourcePath, std::string& reason)
	{
		auto archive = ZipFile::Open(zipPath);
		if (!archive) {
			reason = "Script Extender update failed:\r\nUnable to open update package, file possibly corrupted?";
			return false;
		}

		auto entries = archive->GetEntriesCount();
		for (auto i = 0; i < entries; i++) {
			auto entry = archive->GetEntry(i);

			DEBUG("Extracting: %s", entry->GetFullName().c_str());

			auto outPath = resourcePath + L"\\" + FromUTF8(entry->GetFullName());
			auto tempPath = resourcePath + L"\\extract.tmp";
			std::ofstream f(tempPath.c_str(), std::ios::out | std::ios::binary);
			if (!f.good()) {
				DEBUG("Failed to open %s for extraction", entry->GetFullName().c_str());
				reason = "Script Extender update failed:\r\n";
				reason += std::string("Failed to open file ") + entry->GetFullName() + " for extraction";
				break;
			}

			auto stream = entry->GetDecompressionStream();
			if (!stream) {
				DEBUG("Failed to decompress %s", entry->GetFullName().c_str());
				reason = "Script Extender update failed:\r\n";
				reason += std::string("Failed to decompress file ") + entry->GetFullName();
				break;
			}

			auto len = entry->GetSize();

			char buf[4096];
			while (len) {
				auto chunkSize = std::min(len, std::size(buf));
				stream->read(buf, chunkSize);
				f.write(buf, chunkSize);
				len -= chunkSize;
			}

			entry->CloseDecompressionStream();
			f.close();

			if (!MoveFileExW(tempPath.c_str(), outPath.c_str(), MOVEFILE_REPLACE_EXISTING)) {
				DEBUG("Failed to move file %s", entry->GetFullName().c_str());
				reason = "Script Extender update failed:\r\n";
				reason += std::string("Failed to update file ") + entry->GetFullName();
				break;
			}
		}

		return true;
	}

	bool DeleteLocalCacheFromZip(std::wstring const& zipPath, std::wstring const& resourcePath)
	{
		auto archive = ZipFile::Open(zipPath);
		if (!archive) {
			return false;
		}

		auto entries = archive->GetEntriesCount();
		for (auto i = 0; i < entries; i++) {
			auto entry = archive->GetEntry(i);

			DEBUG("Deleting local file: %s", entry->GetFullName().c_str());
			auto extractedPath = resourcePath + L"\\" + FromUTF8(entry->GetFullName());
			DeleteFileW(extractedPath.c_str());
		}

		return true;
	}
};


class ResourceCacheRepository
{
public:
	ResourceCacheRepository(std::wstring const& path)
		: path_(path)
	{
		DEBUG("ResourceCache path: %s", ToUTF8(path).c_str());
		LoadManifest(GetCachedManifestPath());
	}

	std::wstring GetCachedManifestPath() const
	{
		return path_ + L"\\Manifest2.json";
	}

	bool LoadManifest(std::wstring const& path)
	{
		std::string manifestText;
		DEBUG("Loading cache manifest: %s", ToUTF8(path).c_str());
		if (LoadFile(path, manifestText)) {
			ManifestSerializer parser;
			std::string parseError;
			auto result = parser.Parse(manifestText, manifest_, parseError);
			if (result == ManifestParseResult::Successful) {
				DEBUG("Cache manifest load OK.");
				return true;
			} else {
				DEBUG("Cache manifest parse failed: %d", result);
				manifest_.Resources.clear();
				return false;
			}
		}

		DEBUG("Cache manifest load failed.");
		return false;
	}

	bool SaveManifest(std::wstring const& path)
	{
		manifest_.ManifestVersion = Manifest::CurrentVersion;

		ManifestSerializer parser;
		std::string manifestText = parser.Stringify(manifest_);

		DEBUG("Saving cache manifest: %s", ToUTF8(path).c_str());
		return SaveFile(path, manifestText);
	}

	bool ResourceExists(std::string const& name, Manifest::ResourceVersion const& version) const
	{
		auto resource = manifest_.Resources.find(name);
		if (resource == manifest_.Resources.end()) {
			return false;
		}

		auto found = resource->second.ResourceVersions.find(version.Digest);
		if (found == resource->second.ResourceVersions.end()) {
			return false;
		}

		return HasLocalCopy(resource->second, found->second);
	}

	bool UpdateLocalPackage(Manifest::Resource const& resource, Manifest::ResourceVersion const& version, std::vector<uint8_t> const& contents, std::string& reason)
	{
		DEBUG("Updating local copy of resource %s, digest %s", resource.Name.c_str(), version.Digest.c_str());
		CachedResource res(path_, resource, version);
		if (res.UpdateLocalPackage(contents, reason)) {
			AddResourceToManifest(resource, version);
			if (!SaveManifest(GetCachedManifestPath())) {
				reason = "Script Extender update failed:\r\n";
				reason += std::string("Failed to write manifest file ") + ToUTF8(GetCachedManifestPath());
				return false;
			} else {
				return true;
			}
		} else {
			return false;
		}
	}

	bool RemoveLocalPackage(Manifest::Resource const& resource, Manifest::ResourceVersion const& version)
	{
		auto resIt = manifest_.Resources.find(resource.Name);
		if (resIt == manifest_.Resources.end()) {
			return false;
		}

		auto verIt = resIt->second.ResourceVersions.find(version.Digest);
		if (verIt == resIt->second.ResourceVersions.end()) {
			return false;
		}

		DEBUG("Removing local copy of resource %s, digest %s", resource.Name.c_str(), version.Digest.c_str());

		CachedResource res(path_, resource, version);
		res.RemoveLocalPackage();
		resIt->second.ResourceVersions.erase(verIt);
		return true;
	}

	std::optional<std::wstring> FindResourcePath(std::string const& name, VersionNumber const& gameVersion)
	{
		auto resource = manifest_.Resources.find(name);
		if (resource == manifest_.Resources.end()) {
			return {};
		}

		auto ver = resource->second.FindResourceVersion(gameVersion);
		if (!ver) {
			return {};
		}

		CachedResource res(path_, resource->second, *ver);
		if (res.ExtenderDLLExists()) {
			return res.GetLocalPath();
		} else {
			return {};
		}
	}

private:
	std::wstring path_;
	Manifest manifest_;

	bool HasLocalCopy(Manifest::Resource const& resource, Manifest::ResourceVersion const& version) const
	{
		CachedResource res(path_, resource, version);
		return res.ExtenderDLLExists();
	}

	void AddResourceToManifest(Manifest::Resource const& resource, Manifest::ResourceVersion const& version)
	{
		auto resIt = manifest_.Resources.find(resource.Name);
		if (resIt == manifest_.Resources.end()) {
			Manifest::Resource res;
			res.Name = resource.Name;
			manifest_.Resources.insert(std::make_pair(res.Name, res));
			resIt = manifest_.Resources.find(resource.Name);
		}

		AddVersionToResource(resIt->second, version);
	}

	void AddVersionToResource(Manifest::Resource& resource, Manifest::ResourceVersion const& version)
	{
		Manifest::ResourceVersion ver{ version };
		ver.URL = "";

		auto it = resource.ResourceVersions.find(version.Digest);
		if (it == resource.ResourceVersions.end()) {
			resource.ResourceVersions.insert(std::make_pair(ver.Digest, ver));
		} else {
			it->second = ver;
		}
	}
};

class ManifestFetcher
{
public:
	ManifestFetcher(UpdaterConfig const& config)
		: config_(config)
	{}
	
	bool Fetch(Manifest& manifest, std::string& reason)
	{
		HttpFetcher fetcher;
		std::string manifestUrl = config_.ManifestURL + config_.UpdateChannel + "/" + config_.ManifestName;

		DEBUG("Fetching manifest from: %s", manifestUrl.c_str());
		std::vector<uint8_t> manifestBinary;
		if (!fetcher.Fetch(manifestUrl, manifestBinary)) {
			reason = "Failed to check for Script Extender updates. Make sure you're connected to the internet and try again.\r\n";
			reason += fetcher.GetLastError();
			return false;
		}

		std::string manifestStr((char*)manifestBinary.data(), (char*)manifestBinary.data() + manifestBinary.size());
		if (Parse(manifestStr, manifest, reason)) {
			return true;
		}

		return false;
	}

	bool Parse(std::string const& manifestStr, Manifest& manifest, std::string& reason)
	{
		ManifestSerializer parser;
		std::string parseError;
		auto result = parser.Parse(manifestStr, manifest, parseError);
		if (result == ManifestParseResult::Failed) {
			reason = "Failed to check for Script Extender updates:\r\nUnable to parse manifest: ";
			reason += parseError;
			return false;
		}

		if (result == ManifestParseResult::UpdateRequired) {
			reason = "Failed to check for Script Extender updates:\r\nUnable to parse manifest: ";
			return false;
		}

		return true;
	}

private:
	UpdaterConfig const& config_;
};



class ResourceUpdater
{
public:
	ResourceUpdater(UpdaterConfig const& config, ResourceCacheRepository& cache)
		: config_(config), cache_(cache)
	{}

	bool Update(Manifest const& manifest, std::string const& resourceName, VersionNumber const& gameVersion, std::string& reason)
	{
		DEBUG("Starting fetch for resource: %s", resourceName.c_str());
		auto resIt = manifest.Resources.find(resourceName);
		if (resIt == manifest.Resources.end()) {
			reason = "Failed to check for Script Extender updates:\r\nNo manifest entry found for resource: ";
			reason += resourceName;
			return false;
		}

		auto version = resIt->second.FindResourceVersion(gameVersion);
		if (!version) {
			reason = "Failed to check for Script Extender updates:\r\nScript extender not available for game version v";
			reason += gameVersion.ToString();
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

	
	bool Update(Manifest::Resource const& resource, Manifest::ResourceVersion const& version, std::string & reason)
	{
		if (version.Revoked) {
			reason = "Failed to download Script Extender update package.\r\n";
			reason += "Attempted to download unavailable resource version.";
			return false;
		}

		HttpFetcher fetcher;
		DEBUG("Fetching update package: %s", version.URL.c_str());
		std::vector<uint8_t> response;
		if (!fetcher.Fetch(version.URL, response)) {
			reason = "Failed to download Script Extender update package.\r\n";
			reason += fetcher.GetLastError();
			reason += "\r\n";
			reason += "URL: ";
			reason += version.URL;
			return false;
		}

		return cache_.UpdateLocalPackage(resource, version, response, reason);
	}

private:
	UpdaterConfig const& config_;
	ResourceCacheRepository& cache_;
};

class ScriptExtenderUpdater
{
public:
	void Launch()
	{
		UpdatePaths();
		cache_ = std::make_unique<ResourceCacheRepository>(config_.CachePath);

		std::string updateReason;
		bool updated = TryToUpdate(updateReason);

		auto resourcePath = cache_->FindResourcePath("ScriptExtender", gameVersion_);

		if (updated && !resourcePath) {
			updated = false;
			updateReason = "Failed to initialize Script Extender:\r\nNo cached package available for game version v";
			updateReason += gameVersion_.ToString();
		}

		if (!updated) {
			DEBUG("Update failed; reason: %s", updateReason.c_str());
			completed_ = true;
			gErrorUtils->ShowError(updateReason.c_str());
		}

		if (resourcePath) {
			// Make sure that we can load dependencies from the extension directory
			// (protobuf, etc.)
			DEBUG("SetDllDirectoryW(%s)", ToUTF8(*resourcePath).c_str());
			SetDllDirectoryW(resourcePath->c_str());

			auto dllPath = *resourcePath + L"\\" + APP_DLL;
			DEBUG("Loading extender DLL: %s", ToUTF8(dllPath).c_str());
			HMODULE handle = LoadLibraryW(dllPath.c_str());
			// Wait a bit for extender startup to complete
			Sleep(300);
			completed_ = true;

			if (handle == NULL) {
				auto errc = GetLastError();
				DEBUG("Extender DLL load failed; error code %d", errc);
				std::string errmsg = "Failed to load Script Extender library.\r\n"
					"LoadLibrary() returned error code ";
				errmsg += std::to_string(errc);
				gErrorUtils->ShowError(errmsg.c_str());
			}
		} else {
			completed_ = true;
		}
	}
	
	bool TryToUpdate(std::string & reason)
	{
		Manifest manifest;
		ManifestFetcher manifestFetcher(config_);
		if (!manifestFetcher.Fetch(manifest, reason)) {
			return false;
		}

		for (auto const& res : manifest.Resources) {
			for (auto const& ver : res.second.ResourceVersions) {
				if (ver.second.Revoked) {
					cache_->RemoveLocalPackage(res.second, ver.second);
				}
			}
		}

		ResourceUpdater updater(config_, *cache_);
		return updater.Update(manifest, "ScriptExtender", gameVersion_, reason);
	}

	inline bool IsCompleted() const
	{
		return completed_;
	}

	void InitConsole()
	{
#if defined(HAS_DEBUG_LOGGING)
		if (!config_.Debug) return;

		AllocConsole();
		SetConsoleTitleW(L"BG3 Script Extender Debug Console");

		if (IsValidCodePage(CP_UTF8)) {
			SetConsoleCP(CP_UTF8);
			SetConsoleOutputCP(CP_UTF8);
		}

		auto hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleMode(hStdout, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

		FILE* reopenedStream;
		freopen_s(&reopenedStream, "CONOUT$", "w", stdout);
#endif
	}

	void LoadConfig()
	{
		LoadConfigFile(L"ScriptExtenderUpdaterConfig.json", config_);
	}

private:
	VersionNumber gameVersion_;
	UpdaterConfig config_;
	std::unique_ptr<ResourceCacheRepository> cache_;
	bool completed_{ false };

	void UpdatePaths()
	{
		auto cacheDir = config_.CachePath;
		if (!PathFileExistsW(cacheDir.c_str())) {
			CreateDirectoryW(cacheDir.c_str(), NULL);
		}

		auto version = GetGameVersion();
		if (version) {
			gameVersion_ = *version;
		}

		DEBUG("Cache path: %s", ToUTF8(cacheDir).c_str());
		DEBUG("Update channel: %s", config_.UpdateChannel.c_str());
	}
};

std::unique_ptr<ScriptExtenderUpdater> gUpdater;

bool ShouldLoad()
{
	return GetModuleHandleW(L"bg3.exe") != NULL
		|| GetModuleHandleW(L"bg3_dx11.exe") != NULL;
}

// This thread is responsible for polling and suspending/resuming
// the client init thread if the update is still pending during client init.
// The goal is to prevent the client from loading modules before the extender is loaded.
DWORD WINAPI ClientWorkerSuspenderThread(LPVOID param)
{
	bool suspended{ false };
	for (;;) {
		auto state = gErrorUtils->GetState();
		if (state) {
			bool completed = gUpdater->IsCompleted();
			if (!suspended && !completed && (*state == GameState::LoadModule || *state == GameState::Init)) {
				DEBUG("Suspending client thread (pending update)");
				gErrorUtils->SuspendClientThread();
				suspended = true;
			}

			if (completed) {
				if (suspended) {
					DEBUG("Resuming client thread");
					gErrorUtils->ResumeClientThread();
				}
				break;
			}

			if (*state == GameState::Menu) {
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
	gErrorUtils = std::make_unique<ErrorUtils>();
	gUpdater = std::make_unique<ScriptExtenderUpdater>();
	gUpdater->LoadConfig();
	gUpdater->InitConsole();
	CreateThread(NULL, 0, &ClientWorkerSuspenderThread, NULL, 0, NULL);
	DEBUG("Launch loader");
	gUpdater->Launch();
	DEBUG("Extender launcher thread exiting");
	return 0;
}

void StartUpdaterThread()
{
	CreateThread(NULL, 0, &UpdaterThread, NULL, 0, NULL);
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		gDWriteWrapper = std::make_unique<DWriteWrapper>();
		if (ShouldLoad()) {
			StartUpdaterThread();
		}
		break;

	case DLL_PROCESS_DETACH:
		if (gDWriteWrapper) {
			gDWriteWrapper.reset();
		}
		break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

