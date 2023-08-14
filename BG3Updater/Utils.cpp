#include <stdafx.h>
#include "GameHelpers.h"
#include <GameDefinitions/Symbols.h>

#include <atlbase.h>
#include <psapi.h>
#include <iostream>
#include <fstream>

std::wstring FromStdUTF8(std::string_view s)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0);
	std::wstring converted;
	converted.resize(size);
	MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), converted.data(), (int)converted.size());
	return converted;
}

BEGIN_SE()

void DebugMsg(char const * msg)
{
	OutputDebugStringA(msg);
	OutputDebugStringA("\r\n");
	std::cout << msg << std::endl;
	std::cout.flush();
}

std::optional<std::string> GetExeResource(int resourceId)
{
	auto hResource = FindResource(gThisModule, MAKEINTRESOURCE(resourceId), L"SCRIPT_EXTENDER");

	if (hResource) {
		auto hGlobal = LoadResource(gThisModule, hResource);
		if (hGlobal) {
			auto resourceData = LockResource(hGlobal);
			if (resourceData) {
				DWORD resourceSize = SizeofResource(gThisModule, hResource);
				std::string contents;
				contents.resize(resourceSize);
				memcpy(contents.data(), resourceData, resourceSize);
				return contents;
			}
		}
	}

	ERR("Could not get bootstrap resource %d!", resourceId);
	return {};
}

StaticSymbols* gStaticSymbols{ nullptr };

void InitStaticSymbols()
{
	gStaticSymbols = new StaticSymbols();
}

std::unique_ptr<GameHelpers> gGameHelpers;

GameHelpers::GameHelpers()
	: symbolMapper_(mappings_)
{
	InitStaticSymbols();
}

#define SYM_OFF(name) mappings_.StaticSymbols.insert(std::make_pair(#name, SymbolMappings::StaticSymbol{ (int)offsetof(StaticSymbols, name) }))

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

	if (loader.LoadBuiltinMappings()) {
		if (GetModuleHandleW(L"bg3.exe") != NULL) {
			symbolMapper_.AddModule("Main", L"bg3.exe");
		} else {
			symbolMapper_.AddModule("Main", L"bg3_dx11.exe");
		}

		symbolMapper_.MapAllSymbols(false);
		AddVectoredExceptionHandler(1, &ThreadNameCaptureFilter);
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
	auto client = GetStaticSymbols().GetEoCClient();
	if (client == nullptr
		|| GetStaticSymbols().ecl__EoCClient__HandleError == nullptr
		|| GetStaticSymbols().ls__GlobalAllocator__Alloc == nullptr
		|| GetStaticSymbols().ls__GlobalAllocator__Free == nullptr
		|| GetStaticSymbols().ls__FixedString__CreateFromString == nullptr
		|| GetStaticSymbols().ls__gTranslatedStringRepository == nullptr) {
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

	ClientHandleError(msg, false);
	return true;
}

void GameHelpers::ClientHandleError(char const * msg, bool exitGame) const
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
	auto& texts = (*GetStaticSymbols().ls__gTranslatedStringRepository)->TranslatedStrings[0];
	auto tskRef = texts->Texts.Find(ts.Handle);
	auto str = GameAlloc<STDString>(msg);
	texts->Strings.Add(str);

	// Update reference to new string
	**tskRef = StringView(*str);

	GetStaticSymbols().ecl__EoCClient__HandleError(*GetStaticSymbols().ecl__EoCClient, ts, exitGame, ts);
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
	return GetStaticSymbols().GetClientState();
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

bool TryCreateDirectory(std::wstring const& path)
{
	if (!PathFileExistsW(path.c_str())) {
		return CreateDirectoryW(path.c_str(), NULL) == TRUE;
	} else {
		return true;
	}
}

bool SaveFile(std::wstring const& path, std::vector<uint8_t> const& body)
{
	std::ofstream f(path, std::ios::binary | std::ios::out);
	if (!f.good()) {
		return false;
	}

	f.write(reinterpret_cast<char const*>(body.data()), body.size());
	return f.good();
}

bool SaveFile(std::wstring const& path, std::string const& body)
{
	std::ofstream f(path, std::ios::binary | std::ios::out);
	if (!f.good()) {
		return false;
	}

	f.write(body.data(), body.size());
	return f.good();
}

bool LoadFile(std::wstring const& path, std::vector<uint8_t>& body)
{
	std::ifstream f(path, std::ios::in | std::ios::binary);
	if (f.good()) {
		f.seekg(0, std::ios::end);
		auto size = f.tellg();
		f.seekg(0, std::ios::beg);

		body.resize(size);
		f.read(reinterpret_cast<char*>(body.data()), size);
		return f.good();
	}

	return false;
}

bool LoadFile(std::wstring const& path, std::string& body)
{
	std::ifstream f(path, std::ios::in | std::ios::binary);
	if (f.good()) {
		f.seekg(0, std::ios::end);
		auto size = f.tellg();
		f.seekg(0, std::ios::beg);

		body.resize(size);
		f.read(body.data(), size);
		return f.good();
	}

	return false;
}

END_SE()
