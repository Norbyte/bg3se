#include <stdafx.h>
#include "GameHelpers.h"
#include "resource.h"

#include <atlbase.h>
#include <psapi.h>
#include <iostream>
#include <fstream>

BEGIN_SE()

std::unique_ptr<GameHelpers> gGameHelpers;

void* BG3Alloc(std::size_t size)
{
	return gGameHelpers->Symbols().ls__GlobalAllocator__Alloc(size, 2, 0, 8);
}

void BG3Free(void* ptr)
{
	gGameHelpers->Symbols().ls__GlobalAllocator__Free(ptr);
}

GameHelpers::GameHelpers()
	: symbolMapper_(mappings_)
{
	gCoreLibPlatformInterface.StaticSymbols = &symbols_;
	gCoreLibPlatformInterface.GlobalConsole = new Console();
}

#define SYM_OFF(name) mappings_.StaticSymbols.insert(std::make_pair(#name, SymbolMappings::StaticSymbol{ (int)offsetof(UpdaterSymbols, name) }))

void GameHelpers::Initialize()
{
	SymbolMappingLoader loader(mappings_);
	loader.AddKnownModule("Main");

	SYM_OFF(ls__GlobalAllocator__Alloc);
	SYM_OFF(ls__GlobalAllocator__Free);

	SYM_OFF(ls__FixedString__CreateFromString);
	SYM_OFF(ls__FixedString__GetString);
	SYM_OFF(ls__FixedString__IncRef);
	SYM_OFF(ls__FixedString__DecRef);
	SYM_OFF(ls__gGlobalStringTable);

	SYM_OFF(ecl__EoCClient);
	SYM_OFF(ecl__EoCClient__HandleError);
	SYM_OFF(ls__gTranslatedStringRepository);

	if (loader.LoadBuiltinMappings(IDR_BINARY_MAPPINGS)) {
		if (GetModuleHandleW(L"bg3.exe") != NULL) {
			symbolMapper_.AddModule("Main", L"bg3.exe");
		} else {
			symbolMapper_.AddModule("Main", L"bg3_dx11.exe");
		}

		symbolMapper_.MapAllSymbols(false);
		AddVectoredExceptionHandler(1, &ThreadNameCaptureFilter);

		gCoreLibPlatformInterface.Alloc = &BG3Alloc;
		gCoreLibPlatformInterface.Free = &BG3Free;
		gCoreLibPlatformInterface.ls__FixedString__CreateFromString = symbols_.ls__FixedString__CreateFromString;
		gCoreLibPlatformInterface.ls__FixedString__GetString = symbols_.ls__FixedString__GetString;
		gCoreLibPlatformInterface.ls__FixedString__IncRef = symbols_.ls__FixedString__IncRef;
		gCoreLibPlatformInterface.ls__FixedString__DecRef = symbols_.ls__FixedString__DecRef;
		gCoreLibPlatformInterface.ls__gGlobalStringTable = symbols_.ls__gGlobalStringTable;
	}
}

void GameHelpers::ShowError(char const * msg) const
{
	if (!ShowErrorDialog(msg)) {
		MessageBoxA(NULL, msg, "Script Extender Updater Error", MB_OK | MB_ICONERROR);
	}
}

bool GameHelpers::ShowErrorDialog(char const * msg) const
{
	auto client = symbols_.GetEoCClient();
	if (client == nullptr
		|| symbols_.ecl__EoCClient__HandleError == nullptr
		|| symbols_.ls__GlobalAllocator__Alloc == nullptr
		|| symbols_.ls__GlobalAllocator__Free == nullptr
		|| symbols_.ls__FixedString__CreateFromString == nullptr
		|| symbols_.ls__gTranslatedStringRepository == nullptr) {
		return false;
	}

	unsigned retries{ 0 };
	while (!CanShowError() && retries < 3000) {
		Sleep(100);
		retries++;
	}

	if (retries >= 3000) {
		return false;
	}

	return ClientHandleError(msg, false);
}

bool GameHelpers::ClientHandleError(char const * msg, bool exitGame) const
{
	std::string filtered(msg);
	for (auto pos = filtered.find("\r\n"); pos != std::wstring::npos; pos = filtered.find("\r\n")) {
		filtered.replace(filtered.begin() + pos, filtered.begin() + pos + 2, "<br>");
	}

	// Abuse a deprecated string key for displaying custom text
	TranslatedString ts;
	ts.Handle.Handle = FixedString("h11018635g3003g46c6g8013g4630abe55cad");
	ts.Handle.Version = 1;
	ts.ArgumentString.Handle = FixedString("ls::TranslatedStringRepository::s_HandleUnknown");

	// Create a new entry in the string repository text pool
	auto& texts = (*symbols_.ls__gTranslatedStringRepository)->TranslatedStrings[0];
	auto tskRef = texts->Texts.Find(ts.Handle);
	if (tskRef) {
		// Avoid using the game allocator as much as possible, as the mapping may be broken
		auto str = new std::string(msg);

		// Update reference to new string
		auto originalRef = **tskRef;
		**tskRef = StringView(*str);

		symbols_.ecl__EoCClient__HandleError(*symbols_.ecl__EoCClient, ts, exitGame, ts);
		return true;
	} else {
		return false;
	}
}

bool GameHelpers::CanShowError() const
{
	auto state = GetState();
	return state
		&& (*state == ecl::GameState::Running
		|| *state == ecl::GameState::Paused
		|| *state == ecl::GameState::Menu
		|| *state == ecl::GameState::Lobby);
}

std::optional<ecl::GameState> GameHelpers::GetState() const
{
	return symbols_.GetClientState();
}

void GameHelpers::SuspendClientThread() const
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

END_SE()
