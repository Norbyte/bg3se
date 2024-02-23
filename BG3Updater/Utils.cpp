#include <stdafx.h>
#include "GameHelpers.h"
#include "resource.h"
#include "Manifest.h"

#include <Shlwapi.h>
#include <Shlobj.h>
#include <psapi.h>

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
	if (exceptionHandler_ != NULL) {
		RemoveVectoredExceptionHandler(exceptionHandler_);
	}

	delete gCoreLibPlatformInterface.GlobalConsole;
	gCoreLibPlatformInterface.GlobalConsole = nullptr;
}

#define SYM_OFF(name) mappings_.StaticSymbols.insert(std::make_pair(#name, SymbolMappings::StaticSymbol{ (int)offsetof(UpdaterSymbols, name) }))

void GameHelpers::BindToGame()
{
	if (bindingsDone_) return;
	exceptionHandler_ = AddVectoredExceptionHandler(1, &ThreadNameCaptureFilter);
	bindingsDone_ = true;
}

void GameHelpers::SetMainThread(HANDLE h)
{
	hMainThread_ = h;
}

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

void GameHelpers::SuspendClientThread() const
{
	auto thread = FindClientThread();
	if (thread != nullptr) {
		auto hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, thread->ThreadId);
		if (hThread && hThread != INVALID_HANDLE_VALUE) {
			SuspendThread(hThread);
			CloseHandle(hThread);
		}
	} else {
		DEBUG("Could not suspend client thread (thread not found!)");
		SuspendThread(hMainThread_);
	}
}

void GameHelpers::ResumeClientThread() const
{
	auto thread = FindClientThread();
	if (thread != nullptr) {
		auto hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, thread->ThreadId);
		if (hThread && hThread != INVALID_HANDLE_VALUE) {
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
	} else {
		DEBUG("Could not resume client thread (thread not found!)");
		ResumeThread(hMainThread_);
	}
}

GameHelpers::ThreadInfo const * GameHelpers::FindClientThread() const
{
	for (auto const & it : threads_) {
		if (it.Name == "ClientInit" || it.Name == "ClientLoadModule") {
			return &it;
		}
	}

	return nullptr;
}

LONG NTAPI GameHelpers::ThreadNameCaptureFilter(_EXCEPTION_POINTERS *ExceptionInfo)
{
	if (ExceptionInfo->ExceptionRecord->ExceptionCode == 0x406D1388) {
		auto info = reinterpret_cast<THREADNAME_INFO *>(&ExceptionInfo->ExceptionRecord->ExceptionInformation);
		if (info->dwType == 0x1000 && info->dwFlags == 0) {
			ThreadInfo thread;
			thread.ThreadId = info->dwThreadID;
			thread.Name = info->szName;
			gGameHelpers->threads_.push_back(thread);
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
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
		value = FromStdUTF8(configVar.asString());
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
	config.IPv4Only = false;
	config.DisableUpdates = false;

	std::ifstream f(configPath, std::ios::in);
	if (!f.good()) {
		return;
	}

	Json::CharReaderBuilder factory;
	Json::Value root;
	std::string errs;
	if (!Json::parseFromStream(factory, f, &root, &errs)) {
		std::wstring werrs = FromStdUTF8(errs);

		std::wstringstream err;
		err << L"Failed to load configuration file '" << configPath << "':\r\n" << werrs;
		Fail(ToStdUTF8(err.str()).c_str());
	}

	ConfigGetString(root, "ManifestURL", config.ManifestURL);
	ConfigGetString(root, "ManifestName", config.ManifestName);
	ConfigGetString(root, "UpdateChannel", config.UpdateChannel);
	ConfigGetString(root, "TargetVersion", config.TargetVersion);
	ConfigGetString(root, "TargetResourceDigest", config.TargetResourceDigest);
	ConfigGetString(root, "CachePath", config.CachePath);
#if defined(HAS_DEBUG_LOGGING)
	ConfigGetBool(root, "Debug", config.Debug);
	ConfigGetBool(root, "ValidateSignature", config.ValidateSignature);
#endif
	ConfigGetBool(root, "IPv4Only", config.IPv4Only);
	ConfigGetBool(root, "DisableUpdates", config.DisableUpdates);
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

	if (UpdaterMutex == NULL) {
		char mutexName[MAX_PATH];
		sprintf_s(mutexName, "BG3SE_Upd_%d", GetCurrentProcessId());
		UpdaterMutex = CreateMutexA(NULL, TRUE, mutexName);
	}

	return UpdaterMutex != NULL;
}

END_SE()
