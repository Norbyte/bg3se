#include "stdafx.h"
#include "ErrorUtils.h"

#include <Shellapi.h>
#include <Shlwapi.h>
#include <Shlobj.h>
#include <Shldisp.h>
#include <atlbase.h>
#include <psapi.h>
#include <iostream>

void DebugMsg(char const * msg)
{
	OutputDebugStringA(msg);
	OutputDebugStringA("\r\n");
	std::cout << msg << std::endl;
	std::cout.flush();
}

uint8_t const * AsmCallToAbsoluteAddress(uint8_t const * call)
{
	if (call[0] != 0xE8) {
		return nullptr;
	}

	int32_t rel = *(int32_t const *)(call + 1);
	return call + rel + 5;
}

uint8_t const * AsmLeaToAbsoluteAddress(uint8_t const * lea)
{
	if ((lea[0] != 0x48 && lea[0] != 0x4C) || (lea[1] != 0x8D && lea[1] != 0x8B)) {
		return nullptr;
	}

	int32_t rel = *(int32_t const *)(lea + 3);
	return lea + rel + 7;
}

std::unique_ptr<ErrorUtils> gErrorUtils;

ErrorUtils::ErrorUtils()
{
	if (FindModule()) {
		FindErrorFuncs();
		AddVectoredExceptionHandler(1, &ThreadNameCaptureFilter);
	}
}

void ErrorUtils::ShowError(wchar_t const * msg) const
{
	if (!ShowErrorDialog(msg)) {
		MessageBoxW(NULL, msg, L"Osiris Loader Error", MB_OK | MB_ICONERROR);
	}
}

bool ErrorUtils::ShowErrorDialog(wchar_t const * msg) const
{
	if (EoCClient == nullptr
		|| EoCClientHandleError == nullptr) {
		return false;
	}

	unsigned retries{ 0 };
	while (!CanShowError() && retries < 600) {
		Sleep(100);
		retries++;
	}

	if (retries >= 300) {
		return false;
	}

	ClientHandleError(msg, false);
	return true;
}

void ErrorUtils::ClientHandleError(wchar_t const * msg, bool exitGame) const
{
	if (EoCClientHandleError == nullptr) return;

	std::wstring filtered(msg);
	for (auto pos = filtered.find(L"\r\n"); pos != std::wstring::npos; pos = filtered.find(L"\r\n")) {
		filtered.replace(filtered.begin() + pos, filtered.begin() + pos + 2, L"<br>");
	}

	STDWString str;
	str.Size = filtered.size();
	str.Capacity = 0xfff; // Used to bypass 7-character inline buffer check
	str.BufPtr = const_cast<wchar_t *>(filtered.c_str());
	EoCClientHandleError(*EoCClient, &str, exitGame, &str);
}

bool ErrorUtils::CanShowError() const
{
	auto state = GetState();
	return state
		&& (*state == GameState::Running
		|| *state == GameState::Paused
		|| *state == GameState::GameMasterPause
		|| *state == GameState::Menu
		|| *state == GameState::Lobby);
}

std::optional<GameState> ErrorUtils::GetState() const
{
	if (EoCClient == nullptr
		|| *EoCClient == nullptr
		|| (*EoCClient)->State == nullptr
		|| *(*EoCClient)->State == nullptr
		|| EoCClientHandleError == nullptr) {
		return {};
	}

	return (*(*EoCClient)->State)->State;
}

bool ErrorUtils::FindModule()
{
	eocApp_ = GetModuleHandleW(L"bg3.exe");
	if (eocApp_ == NULL) {
		eocApp_ = GetModuleHandleW(L"bg3_dx11.exe");
	}
	if (eocApp_ == NULL) {
		return false;
	}

	MODULEINFO moduleInfo;
	if (!GetModuleInformation(GetCurrentProcess(), eocApp_, &moduleInfo, sizeof(moduleInfo))) {
		return false;
	}

	moduleStart_ = (uint8_t const *)moduleInfo.lpBaseOfDll;
	moduleSize_ = moduleInfo.SizeOfImage;
	return true;
}

void ErrorUtils::FindErrorFuncs()
{
	uint8_t const fragment1[] = { 
		0x4C, 0x8D, 0x8D, 0x08, 0x04, 0x00, 0x00, // lea     r9, [rbp+8F8h+var_4F0]
		0x41, 0xB0, 0x01, // mov     r8b, 1
		0x48, 0x8D, 0x95, 0xE8, 0x03, 0x00, 0x00, // lea     rdx, [rbp+8F8h+var_510]
		0xE8 // call    ecl__EocClient__HandleError
	};

	uint8_t const fragment2[] = {
		0x48, 0x8D, 0x8D, 0xE8, 0x03, 0x00, 0x00 // lea     rdx, [rbp+8F8h+var_510]
	};

	auto moduleEnd = moduleStart_ + moduleSize_;
	for (auto p = moduleStart_; p < moduleEnd; p++) {
		if (*p == 0x48 && p[1] == 0x8B && p[2] == 0x0D
			&& memcmp(p + 7, fragment1, sizeof(fragment1)) == 0
			&& memcmp(p + 29, fragment2, sizeof(fragment2)) == 0) {
			EoCClient = (eclEoCClient **)AsmLeaToAbsoluteAddress(p);
			EoCClientHandleError = (EoCClient__HandleError)AsmCallToAbsoluteAddress(p + 24);
		}
	}
}

void ErrorUtils::SuspendClientThread() const
{
	auto thread = FindClientThread();
	if (thread != nullptr) {
		auto hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, thread->ThreadId);
		if (hThread != INVALID_HANDLE_VALUE) {
			SuspendThread(hThread);
			CloseHandle(hThread);
			// The error handler only displays a status message during the loading screen
			ClientHandleError(L"Checking for Script Extender updates", false);
		}
	} else {
		DEBUG("Could not suspend client thread (thread not found!)");
	}
}

void ErrorUtils::ResumeClientThread() const
{
	auto thread = FindClientThread();
	if (thread != nullptr) {
		auto hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, thread->ThreadId);
		if (hThread != INVALID_HANDLE_VALUE) {
			ResumeThread(hThread);
			CloseHandle(hThread);
		}
	} else {
		DEBUG("Could not resume client thread (thread not found!)");
	}
}

ErrorUtils::ThreadInfo const * ErrorUtils::FindClientThread() const
{
	for (auto const & it : threads_) {
		if (it.Name == "ClientInit" || it.Name == "ClientLoadModule") {
			return &it;
		}
	}

	return nullptr;
}

LONG NTAPI ErrorUtils::ThreadNameCaptureFilter(_EXCEPTION_POINTERS *ExceptionInfo)
{
	if (ExceptionInfo->ExceptionRecord->ExceptionCode == 0x406D1388) {
		auto info = reinterpret_cast<THREADNAME_INFO *>(&ExceptionInfo->ExceptionRecord->ExceptionInformation);
		if (info->dwType == 0x1000 && info->dwFlags == 0) {
			ThreadInfo thread;
			thread.ThreadId = info->dwThreadID;
			thread.Name = info->szName;
			gErrorUtils->threads_.push_back(thread);
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

[[noreturn]]
void Fail(char const * reason)
{
#if defined(_DEBUG)
	DebugBreak();
#endif
	MessageBoxA(NULL, reason, "Osiris Loader Error", MB_OK | MB_ICONERROR);
	TerminateProcess(GetCurrentProcess(), 1);
}


std::string ToUTF8(std::wstring const & s)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), (int)s.size(), NULL, 0, NULL, NULL);
	std::string converted;
	converted.resize(size);
	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), (int)s.size(), converted.data(), (int)converted.size(), NULL, NULL);
	return converted;
}


std::wstring FromUTF8(std::string const & s)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), NULL, 0);
	std::wstring converted;
	converted.resize(size);
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), converted.data(), (int)converted.size());
	return converted;
}

std::string GetLastErrorString(DWORD lastError)
{
	if (lastError == 0) return "";

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM 
		| FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
		LoadLibrary(_T("wininet.dll")), 
		lastError, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPSTR)&messageBuffer, 
		0, NULL);

	std::string message(messageBuffer, size);
	LocalFree(messageBuffer);

	return message;
}

//
// Helper class for exception-safe scope-based 
// CoInitialize/CoUninitialize calls.
//
class CComScopedInit
{
public:

	CComScopedInit()
	{
		HRESULT hr = ::CoInitialize(NULL);
		if (FAILED(hr))
			AtlThrow(hr);
	}

	~CComScopedInit()
	{
		::CoUninitialize();
	}


	// Ban copy
private:
	CComScopedInit(const CComScopedInit&);
	CComScopedInit& operator=(const CComScopedInit&);
};


//
// Unzip a zip file to the specified folder.
//
HRESULT UnzipToFolder(PCWSTR pszZipFile, PCWSTR pszDestFolder, std::string & reason)
{
	CComScopedInit comInit;

	HRESULT hr;

	CComPtr<IShellDispatch> spISD;
	hr = spISD.CoCreateInstance(CLSID_Shell);
	if (FAILED(hr))
		return hr;

	CComVariant vtZipFile(pszZipFile);
	CComPtr<Folder> spZipFile;
	hr = spISD->NameSpace(vtZipFile, &spZipFile);
	if (FAILED(hr))
		return hr;

	if (hr == S_FALSE) {
		reason = "IShellDispatch::NameSpace() call failed on zip file\r\n";
		reason += ToUTF8(pszZipFile);
		return hr;
	}

	CComVariant vtDestFolder(pszDestFolder);
	CComPtr<Folder> spDestination;
	hr = spISD->NameSpace(vtDestFolder, &spDestination);
	if (FAILED(hr))
		return hr;

	if (hr == S_FALSE) {
		reason = "IShellDispatch::NameSpace() call failed on destination folder\r\n";
		reason += ToUTF8(pszDestFolder);
		return hr;
	}

	CComPtr<FolderItems> spFilesInside;
	hr = spZipFile->Items(&spFilesInside);
	if (FAILED(hr))
		return hr;

	CComPtr<IDispatch> spDispItem;
	hr = spFilesInside.QueryInterface(&spDispItem);
	if (FAILED(hr))
		return hr;

	CComVariant vtItem(spDispItem);
	CComVariant vtOptions(FOF_NO_UI);
	hr = spDestination->CopyHere(vtItem, vtOptions);
	if (FAILED(hr))
		return hr;

	return S_OK;
}
