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

#define UPDATER_HOST L"d7ueajfc8twst.cloudfront.net"
#define UPDATER_PATH_PREFIX L"/Channels/"
#define UPDATER_MANIFEST_PATH L"/Manifest.json"
#define UPDATER_CHANNEL L"Release"

extern "C" {

int default_CSPRNG(uint8_t* dest, unsigned int size)
{
	Fail("Signature verifier should not call CSPRNG");
}

}

std::optional<std::wstring> GetGameVersion()
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

	wchar_t version[100];
	swprintf_s(version, L"v%d.%d.%d.%d",
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

std::wstring GetDefaultExtensionPath()
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
	config.UpdateHost = UPDATER_HOST;
	config.UpdatePath = UPDATER_PATH_PREFIX;
	config.ManifestPath = UPDATER_MANIFEST_PATH;
	config.UpdateChannel = UPDATER_CHANNEL;
	config.ExtensionPath = GetDefaultExtensionPath();
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

	ConfigGetString(root, "UpdateHost", config.UpdateHost);
	ConfigGetString(root, "UpdatePath", config.UpdatePath);
	ConfigGetString(root, "ManifestPath", config.ManifestPath);
	ConfigGetString(root, "UpdateChannel", config.UpdateChannel);
#if defined(_DEBUG)
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

#pragma pack(push, 1)
struct PackageSignature
{
	static constexpr uint32_t MAGIC_V1 = 'BGS1';

	uint32_t Magic;
	uint8_t EccSignature[64];
	uint8_t Unused[192];
};
#pragma pack(pop)

class OsiLoader
{
public:
	void Launch()
	{
		UpdatePaths();
		std::string reason;
		bool updated = TryToUpdate(reason);

		if (!updated && !ExtenderDLLExists()) {
			DEBUG("Update failed; reason: %s", reason.c_str());
			completed_ = true;
			auto msg = FromUTF8(reason);
			gErrorUtils->ShowError(msg.c_str());
			return;
		}

		// Make sure that we can load dependencies from the extension directory
		// (protobuf, etc.)
		DEBUG("SetDllDirectoryW(%s)", ToUTF8(extensionPath_).c_str());
		SetDllDirectoryW(extensionPath_.c_str());

		DEBUG("Loading extension: %s", ToUTF8(dllPath_).c_str());
		HMODULE handle = LoadLibraryW(dllPath_.c_str());
		// Wait a bit for extender startup to complete
		Sleep(300);
		completed_ = true;

		if (handle == NULL) {
			auto errc = GetLastError();
			DEBUG("Extension load failed; error code %d", errc);
			std::wstring errmsg = L"Script Extender DLL load failed.\r\n"
				"Error code: ";
			errmsg += std::to_wstring(errc);
			gErrorUtils->ShowError(errmsg.c_str());
		}
	}

	bool AreDllsWriteable()
	{
		auto eocAppPath = extensionPath_ + L"\\OsiExtenderEoCApp.dll";
		if (PathFileExistsW(eocAppPath.c_str())) {
			std::ofstream f;
			f.open(eocAppPath.c_str(), std::ios::out | std::ios::app, _SH_DENYRW);
			if (!f.good()) {
				DEBUG("OsiExtenderEoCApp.dll not writeable, skipping update.");
				return false;
			}
		}

		auto eocPluginPath = extensionPath_ + L"\\OsiExtenderEoCPlugin.dll";
		if (PathFileExistsW(eocPluginPath.c_str())) {
			std::ofstream f;
			f.open(eocPluginPath.c_str(), std::ios::out | std::ios::app, _SH_DENYRW);
			if (!f.good()) {
				DEBUG("OsiExtenderEoCPlugin.dll not writeable, skipping update.");
				return false;
			}
		}

		DEBUG("DLL write check OK");
		return true;
	}

	bool UnzipPackage(std::wstring const& zipPath, std::wstring const& extensionPath, std::string& reason)
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

			auto outPath = extensionPath + L"\\" + FromUTF8(entry->GetFullName());
			auto tempPath = extensionPath + L"\\extract.tmp";
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
				DEBUG("Failed to write file %s", entry->GetFullName().c_str());
				reason = "Script Extender update failed:\r\n";
				reason += std::string("Failed to update file ") + entry->GetFullName();
				break;
			}
		}

		return true;
	}

	bool TryToUpdate(std::string & reason)
	{
		HttpFetcher fetcher(config_.UpdateHost);

		std::wstring manifestUrl = config_.UpdatePath + config_.UpdateChannel + config_.ManifestPath;

		DEBUG("Fetching manifest");
		std::vector<uint8_t> manifestBinary;
		if (!fetcher.Fetch(manifestUrl.c_str(), manifestBinary)) {
			reason = "Failed to check for Script Extender updates. Make sure you're connected to the internet and try again.\r\n";
			reason += fetcher.GetLastError();
			return false;
		}

		std::string manifestStr((char *)manifestBinary.data(), (char *)manifestBinary.data() + manifestBinary.size());
		DEBUG("Server returned manifest: %s", manifestStr.c_str());
		ManifestParser parser;
		Manifest manifest;
		std::string parseError;
		if (!parser.Parse(manifestStr, manifest, parseError)) {
			reason = "Failed to check for Script Extender updates:\r\nUnable to parse manifest: ";
			reason += parseError;
			return false;
		}

		auto version = manifest.Versions.find(ToUTF8(gameVersion_));
		if (version == manifest.Versions.end()) {
			reason = "Failed to check for Script Extender updates:\r\nScript extender not available for game version ";
			reason += ToUTF8(gameVersion_);
			return false;
		}

		DEBUG("Selected version for update: %s / %s", version->second.Path.c_str(), version->second.Digest.c_str());

		auto digest = ReadCurrentDigest();
		DEBUG("Current digest is: %s", digest.c_str());
		if (digest == version->second.Digest) {
			DEBUG("Extender already up to date.");
			return true;
		}

		auto packagePath = ToUTF8(config_.UpdatePath + config_.UpdateChannel) + "/" + version->second.Path;
		DEBUG("Fetching update package: %s", packagePath.c_str());
		std::vector<uint8_t> response;
		if (!fetcher.Fetch(FromUTF8(packagePath).c_str(), response)) {
			reason = "Failed to download Script Extender update package.\r\n";
			reason += "Path: ";
			reason += packagePath;
			reason += "\r\n";
			reason += fetcher.GetLastError();
			return false;
		}

		auto zipPath = extensionPath_ + L"\\Package.zip";
		DEBUG("Saving update to: %s", ToUTF8(zipPath).c_str());
		SaveFile(zipPath, response);

		// Check if any of the files are currently in use by the game.
		// The shell Zip API won't tell us if it failed to overwrite one of the files, so we need to 
		// check beforehand that the files are writeable.
		if (!AreDllsWriteable()) {
			return true;
		}

		if (!CryptoUtils::VerifySignedFile(zipPath, reason)) {
			DEBUG("Unable to verify package signature: %s", reason.c_str());
			return false;
		}
		
		std::string unzipReason;
		DEBUG("Unpacking update to %s", ToUTF8(extensionPath_).c_str());
		if (UnzipPackage(zipPath, extensionPath_, reason)) {
			DEBUG("Saving updated ETag");
			SaveDigest(version->second.Digest);
			return true;
		} else {
			DEBUG("Unzipping failed: %s", reason.c_str());
			return false;
		}
	}

	inline bool IsCompleted() const
	{
		return completed_;
	}

	void InitConsole()
	{
#if defined(_DEBUG)
		if (!config_.Debug) return;

		AllocConsole();
		SetConsoleTitleW(L"Script Extender Updater Debug Console");
		FILE* reopenedStream;
		freopen_s(&reopenedStream, "CONOUT$", "w", stdout);
#endif
	}

	void LoadConfig()
	{
		LoadConfigFile(L"ScriptExtenderUpdaterConfig.json", config_);
	}

private:
	std::wstring extensionPath_;
	std::wstring dllPath_;
	std::wstring gameVersion_;
	UpdaterConfig config_;
	bool completed_{ false };

	bool ExtenderDLLExists()
	{
		return PathFileExists(dllPath_.c_str());
	}

	std::string ReadCurrentDigest()
	{
		if (!ExtenderDLLExists()) {
			// Force update if extension DLL is missing
			return "";
		}

		auto digestPath = extensionPath_ + L"\\.digest";
		std::ifstream f(digestPath, std::ios::binary | std::ios::in);
		if (!f.good()) {
			return "";
		}

		f.seekg(0, std::ios::end);
		auto size = f.tellg();
		f.seekg(0, std::ios::beg);

		std::string s;
		s.resize(size);
		f.read(s.data(), size);
		DEBUG("Digest loaded: %s", s.c_str());
		return s;
	}

	void SaveDigest(std::string const & digest)
	{
		auto etagPath = extensionPath_ + L"\\.digest";
		std::ofstream f(etagPath, std::ios::binary | std::ios::out);
		if (!f.good()) {
			return;
		}

		f.write(digest.c_str(), digest.size());
	}

	void SaveFile(std::wstring const & path, std::vector<uint8_t> const & body)
	{
		std::ofstream f(path, std::ios::binary | std::ios::out);
		if (!f.good()) {
			return;
		}

		f.write(reinterpret_cast<char const *>(body.data()), body.size());
	}

	void UpdatePaths()
	{
		auto rootDir = config_.ExtensionPath;
		if (!PathFileExistsW(rootDir.c_str())) {
			CreateDirectoryW(rootDir.c_str(), NULL);
		}

		auto version = GetGameVersion();
		if (version) {
			gameVersion_ = *version;
		} else {
			gameVersion_ = L"Unknown Version";
		}

		extensionPath_ = rootDir + L"\\" + gameVersion_;
		if (!PathFileExistsW(extensionPath_.c_str())) {
			CreateDirectoryW(extensionPath_.c_str(), NULL);
		}

		dllPath_ = extensionPath_ + L"\\BG3ScriptExtender.dll";

		DEBUG("DLL path: %s", ToUTF8(dllPath_).c_str());
		DEBUG("Update channel: %s", ToUTF8(config_.UpdateChannel).c_str());
	}
};

std::unique_ptr<OsiLoader> gLoader;

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
			bool completed = gLoader->IsCompleted();
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
	gLoader = std::make_unique<OsiLoader>();
	gLoader->LoadConfig();
	gLoader->InitConsole();
	CreateThread(NULL, 0, &ClientWorkerSuspenderThread, NULL, 0, NULL);
	DEBUG("Launch loader");
	gLoader->Launch();
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
		if (ShouldLoad()) {
			gDWriteWrapper = std::make_unique<DWriteWrapper>();
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

