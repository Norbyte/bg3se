#include "stdafx.h"
#include "ErrorUtils.h"

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
	if (call[0] != 0xE8 && call[0] != 0xE9) {
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

void ErrorUtils::ShowError(char const * msg) const
{
	if (!ShowErrorDialog(msg)) {
		MessageBoxA(NULL, msg, "Script Extender Updater Error", MB_OK | MB_ICONERROR);
	}
}

bool ErrorUtils::ShowErrorDialog(char const * msg) const
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

	if (retries >= 600) {
		return false;
	}

	ClientHandleError(msg, false);
	return true;
}

void ErrorUtils::ClientHandleError(char const * msg, bool exitGame) const
{
	if (EoCClientHandleError == nullptr) return;

	std::string filtered(msg);
	for (auto pos = filtered.find("\r\n"); pos != std::wstring::npos; pos = filtered.find("\r\n")) {
		filtered.replace(filtered.begin() + pos, filtered.begin() + pos + 2, "<br>");
	}

	EoCClientHandleError(*EoCClient, filtered, exitGame, filtered);
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
		0x90, // nop 
		0x45, 0x33, 0xC0, // xor     r8d, r8d
		0x33, 0xD2, // xor     edx, edx
		0x48, 0x8D, 0x4C, 0x24, 0x28, // lea     rcx, [rsp+78h+var_50]
		0xE8 // call    ls__TranslatedString__GetTranslatedString
	};

	uint8_t const fragment2[] = {
		0x4C, 0x8D, 0x4C, 0x24, 0x38, // lea     r9, [rsp+78h+a1]
		0x45, 0x33, 0xC0, // xor     r8d, r8d
		0x48, 0x8B, 0xD0, // mov     rdx, rax
		0x48, 0x8B, 0xCB, // mov     rcx, rbx
		0xE8 // call    ecl__EocClient__HandleError
	};

	auto moduleEnd = moduleStart_ + moduleSize_;
	for (auto p = moduleStart_; p < moduleEnd; p++) {
		if (*p == 0x90 && p[1] == 0x45 && p[2] == 0x33
			&& memcmp(p, fragment1, sizeof(fragment1)) == 0
			&& memcmp(p + 0x10, fragment2, sizeof(fragment2)) == 0) {
			EoCClient = (eclEoCClient **)AsmLeaToAbsoluteAddress(p - 0x18);
			EoCClientHandleError = (EoCClient__HandleError)AsmCallToAbsoluteAddress(p + 0x1E);
			break;
		}
	}
}

void ErrorUtils::SuspendClientThread() const
{
	auto thread = FindClientThread();
	if (thread != nullptr) {
		auto hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, thread->ThreadId);
		if (hThread && hThread != INVALID_HANDLE_VALUE) {
			SuspendThread(hThread);
			CloseHandle(hThread);
			// The error handler only displays a status message during the loading screen
			ClientHandleError("Checking for Script Extender updates", false);
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
		if (hThread && hThread != INVALID_HANDLE_VALUE) {
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
	if (IsDebuggerPresent()) {
		DebugBreak();
	}
#endif
	MessageBoxA(NULL, reason, "Script Extender Updater Error", MB_OK | MB_ICONERROR);
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
