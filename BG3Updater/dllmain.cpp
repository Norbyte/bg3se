#include "stdafx.h"

#include "DWriteWrapper.h"
#include "HttpFetcher.h"
#include "ErrorUtils.h"
#include "Crypto.h"
#include <ZipLib/ZipArchive.h>
#include <ZipLib/ZipFile.h>

#include <Shlwapi.h>
#include <Shlobj.h>
#include <comdef.h>

#include <vector>
#include <thread>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

HRESULT UnzipToFolder(PCWSTR pszZipFile, PCWSTR pszDestFolder, std::string & reason);

#define UPDATER_HOST L"d1ov7wr93ghrd7.cloudfront.net"
#define UPDATER_PATH_PREFIX L"/"
#define UPDATER_PATH_POSTFIX L"/Latest.zip"

// Switch to toggle between ZipLib and Shell Zip API
#define USE_ZIPLIB

extern "C" {

int default_CSPRNG(uint8_t* dest, unsigned int size)
{
	Fail("Signature verifier should not call CSPRNG");
}

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
#if defined(USE_ZIPLIB)
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
#else
		std::string unzipReason;
		HRESULT hr = UnzipToFolder(zipPath.c_str(), extensionPath_.c_str(), unzipReason);
		if (hr == S_OK) {
			return true;
		}

		if (hr == S_FALSE) {
			reason = "Unable to extract Script Extender update package.\r\n";
			reason += unzipReason;
		} else {
			_com_error err(hr);
			LPCTSTR errMsg = err.ErrorMessage();

			std::stringstream ss;
			ss << "Unable to extract Script Extender update package.\r\n"
				<< "HRESULT 0x"
				<< std::hex << std::setw(8) << std::setfill('0') << hr;
			if (errMsg != nullptr) {
				ss << ": " << ToUTF8(errMsg);
			}

			reason = ss.str();
		}

		return false;
#endif
	}

	bool TryToUpdate(std::string & reason)
	{
		HttpFetcher fetcher(UPDATER_HOST);

		std::wstring packageUri = UPDATER_PATH_PREFIX + updateChannel_ + UPDATER_PATH_POSTFIX;

		std::string etag;
		DEBUG("Fetching ETag");
		if (!fetcher.FetchETag(packageUri.c_str(), etag)) {
			reason = "Failed to check for for Script Extender updates. Make sure you're connected to the internet and try again\r\n";
			reason += fetcher.GetLastError();
			return false;
		}

		DEBUG("Server returned ETag %s", etag.c_str());
		std::string currentETag = ReadETag();
		if (currentETag == etag) {
			DEBUG("Package is already up to date, nothing to do.");
			return true;
		}

		DEBUG("Fetching update package: %s", ToUTF8(packageUri).c_str());
		std::vector<uint8_t> response;
		if (!fetcher.Fetch(packageUri.c_str(), response)) {
			reason = "Failed to download Script Extender updates. Make sure you're connected to the internet and try again\r\n";
			reason += fetcher.GetLastError();
			return false;
		}

		auto zipPath = extensionPath_ + L"\\Update.zip";
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
			SaveETag(etag);
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

private:
	std::wstring appDataPath_;
	std::wstring extensionPath_;
	std::wstring dllPath_;
	std::wstring updateChannel_;
	bool isGame_{ false };
	bool completed_{ false };

	bool ExtenderDLLExists()
	{
		return PathFileExists(dllPath_.c_str());
	}

	std::string ReadETag()
	{
		if (!ExtenderDLLExists()) {
			// Force update if extension DLL is missing
			return "";
		}

		auto etagPath = extensionPath_ + L"\\.etag";
		std::ifstream f(etagPath, std::ios::binary | std::ios::in);
		if (!f.good()) {
			return "";
		}

		f.seekg(0, std::ios::end);
		auto size = f.tellg();
		f.seekg(0, std::ios::beg);

		std::string s;
		s.resize(size);
		f.read(s.data(), size);
		DEBUG("ETag loaded: %s", s.c_str());
		return s;
	}

	void SaveETag(std::string const & etag)
	{
		auto etagPath = extensionPath_ + L"\\.etag";
		std::ofstream f(etagPath, std::ios::binary | std::ios::out);
		if (!f.good()) {
			return;
		}

		f.write(etag.c_str(), etag.size());
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
		isGame_ = (GetModuleHandleW(L"bg3.exe") != NULL || GetModuleHandleW(L"bg3_dx11.exe") != NULL);

		TCHAR appDataPath[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appDataPath)))
		{
			appDataPath_ = appDataPath;
		}

		extensionPath_ = appDataPath_ + L"\\OsirisExtender";
		if (!PathFileExists(extensionPath_.c_str())) {
			CreateDirectory(extensionPath_.c_str(), NULL);
		}

		if (isGame_) {
			dllPath_ = extensionPath_ + L"\\OsiExtenderEoCApp.dll";
		} else {
			dllPath_ = extensionPath_ + L"\\OsiExtenderEoCPlugin.dll";
		}

		DEBUG("Determined DLL path: %s", ToUTF8(dllPath_).c_str());

		updateChannel_ = L"Release";
		std::ifstream channelFile("OsiUpdateChannel.txt", std::ios::in | std::ios::binary);
		if (channelFile.good()) {
			std::string channel;
			channelFile.seekg(0, std::ios::end);
			channel.resize(channelFile.tellg());
			channelFile.seekg(0, std::ios::beg);
			channelFile.read(channel.data(), channel.size());

			channel = trim(channel);
			if (!channel.empty()) {
				updateChannel_ = FromUTF8(channel);
			}
		}

		DEBUG("Update channel: %s", ToUTF8(updateChannel_).c_str());
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
	DEBUG("Init loader");
	gErrorUtils = std::make_unique<ErrorUtils>();
	gLoader = std::make_unique<OsiLoader>();
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
		if (ShouldLoad()) {
#if defined(_DEBUG)
			AllocConsole();
			SetConsoleTitleW(L"Script Extender Updater Debug Console");
			DisableThreadLibraryCalls(hModule);
			FILE * reopenedStream;
			freopen_s(&reopenedStream, "CONOUT$", "w", stdout);
#endif

			DEBUG("Creating DWrite wrapper");
			gDWriteWrapper = std::make_unique<DWriteWrapper>();
			DEBUG("Start updater thread");
			StartUpdaterThread();
		}
		break;

	case DLL_PROCESS_DETACH:
		if (gDWriteWrapper) {
			DEBUG("Shutting down wrapper");
			gDWriteWrapper.reset();
		}
		break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

