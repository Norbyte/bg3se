#include "stdafx.h"
#include "Version.h"
#include <GameDefinitions/BaseTypes.h>
#include <OsirisProxy.h>
#include <rpc.h>
#include <rpcdce.h>
#include <thread>

namespace dse
{
	STDString ToUTF8(WStringView s)
	{
		int size = WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0, NULL, NULL);
		STDString converted;
		converted.resize(size);
		WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), converted.data(), (int)converted.size(), NULL, NULL);
		return converted;
	}

	STDWString FromUTF8(StringView s)
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0);
		STDWString converted;
		converted.resize(size);
		MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), converted.data(), (int)converted.size());
		return converted;
	}
}

std::string ToUTF8(std::wstring_view s)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0, NULL, NULL);
	std::string converted;
	converted.resize(size);
	WideCharToMultiByte(CP_UTF8, 0, s.data(), (int)s.size(), converted.data(), (int)converted.size(), NULL, NULL);
	return converted;
}

std::wstring FromUTF8(std::string_view s)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), NULL, 0);
	std::wstring converted;
	converted.resize(size);
	MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), converted.data(), (int)converted.size());
	return converted;
}

std::optional<UUID> ParseUuid(std::string_view s)
{
	UUID uuid;
	if (UuidFromStringA((RPC_CSTR)s.data(), &uuid) == RPC_S_OK) {
		// BG3 parses UUIDs into a weird byte swapped format
		auto v1 = uuid.Val[1];
		uuid.Val[1] = (((v1 >> 56) & 0xff) << 48) |
			(((v1 >> 48) & 0xff) << 56) |
			(((v1 >> 40) & 0xff) << 32) |
			(((v1 >> 32) & 0xff) << 40) |
			(((v1 >> 24) & 0xff) << 16) |
			(((v1 >> 16) & 0xff) << 24) |
			(((v1 >> 8) & 0xff) << 0) |
			(((v1 >> 0) & 0xff) << 8);

		return uuid;
	} else {
		return {};
	}
}

std::optional<UUID> ParseGuidString(std::string_view nameGuid)
{
	if (!dse::IsValidGuidString(nameGuid.data())) {
		OsiError("GUID (" << nameGuid << ") malformed!");
		return {};
	}

	auto guid = nameGuid.data() + nameGuid.size() - 36;
	return ParseUuid(std::string_view(guid, 36));
}

[[noreturn]]
void Fail(TCHAR const * reason)
{
#if defined(_DEBUG)
	DebugBreak();
#endif
	ERR(L"%s", reason);
	MessageBoxW(NULL, reason, L"BG3 Script Extender Error", MB_OK | MB_ICONERROR);
	TerminateProcess(GetCurrentProcess(), 1);
}

[[noreturn]]
void Fail(char const * reason)
{
	ERR("%s", reason);
#if defined(_DEBUG)
	DebugBreak();
#endif
	MessageBoxA(NULL, reason, "BG3 Script Extender Error", MB_OK | MB_ICONERROR);
	TerminateProcess(GetCurrentProcess(), 1);
}

void LogLuaError(std::string_view msg)
{
	dse::gOsirisProxy->LogLuaError(msg);
}

void LogOsirisError(std::string_view msg)
{
	dse::gOsirisProxy->LogOsirisError(msg);
}

void LogOsirisWarning(std::string_view msg)
{
	dse::gOsirisProxy->LogOsirisWarning(msg);
}

void LogOsirisMsg(std::string_view msg)
{
	dse::gOsirisProxy->LogOsirisMsg(msg);
}

DebugConsole gConsole;

void DebugConsole::SetColor(DebugMessageType type)
{
	WORD wAttributes = 0;
	switch (type) {
	case DebugMessageType::Error:
		wAttributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;

	case DebugMessageType::Warning:
		wAttributes = FOREGROUND_RED | FOREGROUND_GREEN;
		break;

	case DebugMessageType::Osiris:
		wAttributes = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;

	case DebugMessageType::Info:
		wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		break;

	case DebugMessageType::Debug:
	default:
		wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, wAttributes);
}

void DebugConsole::Debug(DebugMessageType type, char const* msg)
{
	if (consoleRunning_ && !silence_) {
		SetColor(type);
		OutputDebugStringA(msg);
		OutputDebugStringA("\r\n");
		std::cout << msg << std::endl;
		std::cout.flush();
		SetColor(DebugMessageType::Debug);
	}

	if (logToFile_) {
		logFile_.write(msg, strlen(msg));
		logFile_.write("\r\n", 2);
		logFile_.flush();
	}

#if !defined(OSI_NO_DEBUGGER)
	if (dse::gOsirisProxy) {
		auto debugger = dse::gOsirisProxy->GetLuaDebugger();
		if (debugger && debugger->IsDebuggerReady()) {
			debugger->OnLogMessage(type, msg);
		}
	}
#endif
}

void DebugConsole::Debug(DebugMessageType type, wchar_t const* msg)
{
	if (consoleRunning_ && !silence_) {
		SetColor(type);
		OutputDebugStringW(msg);
		OutputDebugStringW(L"\r\n");
		std::wcout << msg << std::endl;
		std::wcout.flush();
		SetColor(DebugMessageType::Debug);
	}

	if (logToFile_) {
		auto utf = ToUTF8(msg);
		logFile_.write(utf.c_str(), utf.size());
		logFile_.write("\r\n", 2);
		logFile_.flush();
	}

#if !defined(OSI_NO_DEBUGGER)
	if (dse::gOsirisProxy) {
		auto debugger = dse::gOsirisProxy->GetLuaDebugger();
		if (debugger && debugger->IsDebuggerReady()) {
			debugger->OnLogMessage(type, dse::ToUTF8(msg));
		}
	}
#endif
}

void DebugConsole::ConsoleThread()
{
	std::string line;
	bool silence = true;
	while (consoleRunning_) {
		wchar_t tempBuf;
		DWORD tempRead;
		if (!ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE), &tempBuf, 1, &tempRead, NULL)
			|| !tempRead
			|| tempBuf != '\r') {
			continue;
		}

		DEBUG("Entering server Lua console.");

		bool serverContext_ = true;
		dse::gOsirisProxy->AttachConsoleThread(serverContext_);

		while (consoleRunning_) {
			inputEnabled_ = true;
			silence_ = silence;
			std::cout << ">> ";
			std::cout.flush();
			std::getline(std::cin, line);
			inputEnabled_ = false;
			silence_ = false;
			if (line == "exit") break;

			if (line.empty()) {
			} else if (line == "server") {
				DEBUG("Switching to server context.");
				serverContext_ = true;
				dse::gOsirisProxy->AttachConsoleThread(true);
			} else if (line == "client") {
				DEBUG("Switching to client context.");
				serverContext_ = false;
				dse::gOsirisProxy->AttachConsoleThread(false);
			} else if (line == "reset") {
				DEBUG("Resetting Lua states.");
				dse::gOsirisProxy->ResetLuaState(true, true);
			} else if (line == "silence on") {
				DEBUG("Silent mode ON");
				silence = true;
			} else if (line == "silence off") {
				DEBUG("Silent mode OFF");
				silence = false;
			} else if (line == "help") {
				DEBUG("Anything typed in will be executed as Lua code except the following special commands:");
				DEBUG("  server - Switch to server context");
				DEBUG("  client - Switch to client context");
				DEBUG("  reset - Reset client and server Lua states");
				DEBUG("  silence <on|off> - Enable/disable silent mode (log output when in input mode)");
				DEBUG("  exit - Leave console mode");
				DEBUG("  !<cmd> <arg1> ... <argN> - Trigger Lua \"ConsoleCommand\" event with arguments cmd, arg1, ..., argN");
			} else {
				dse::ExtensionStateBase* state{ nullptr };
				if (serverContext_) {
					if (dse::gOsirisProxy->HasServerExtensionState()) {
						state = &dse::esv::ExtensionState::Get();
					}
				} else {
					if (dse::gOsirisProxy->HasClientExtensionState()) {
						state = &dse::ecl::ExtensionState::Get();
					}
				}

				if (state) {
					dse::LuaVirtualPin pin(*state);
					if (pin) {
						if (line[0] == '!') {
							pin->CallExt("DoConsoleCommand", 0, dse::lua::ReturnType<>{}, line.substr(1));
						} else {
							auto L = pin->GetState();
							if (luaL_loadstring(L, line.c_str()) || dse::lua::CallWithTraceback(L, 0, 0)) { // stack: errmsg
								ERR("%s", lua_tostring(L, -1));
								lua_pop(L, 1);
							}
						}
					} else {
						ERR("Lua state not initialized!");
					}
				} else {
					ERR("Extensions not initialized!");
				}
			}
		}

		DEBUG("Exiting console mode.");
	}
}

void DebugConsole::Create()
{
	AllocConsole();
	SetConsoleTitleW(L"BG3 Script Extender Debug Console");

	if (IsValidCodePage(CP_UTF8)) {
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);
	}

	FILE * outputStream;
	freopen_s(&outputStream, "CONOUT$", "w", stdout);
	FILE* inputStream;
	freopen_s(&inputStream, "CONIN$", "r", stdin);
	consoleRunning_ = true;

	DEBUG("******************************************************************************");
	DEBUG("*                                                                            *");
	DEBUG("*                     BG3 Script Extender Debug Console                      *");
	DEBUG("*                                                                            *");
	DEBUG("******************************************************************************");
	DEBUG("");
	DEBUG("BG3Ext v%d built on " __DATE__ " " __TIME__, dse::CurrentVersion);

	consoleThread_ = new std::thread(&DebugConsole::ConsoleThread, this);
	created_ = true;

	/*FIXME FIXME FIXME
	 - Automated function hooker -- HOOK(StaticSymbolsFuncName, HookClass) --> expand to hook variable declaration + initialization + wrapping + unwrapping code
	 - Automated symbol definitions -- SYMBOL(name, ...?)*/
}

void DebugConsole::OpenLogFile(std::wstring const& path)
{
	if (logToFile_) {
		CloseLogFile();
	}

	logFile_.rdbuf()->pubsetbuf(0, 0);
	logFile_.open(path.c_str(), std::ios::binary | std::ios::out | std::ios::app);
	if (!logFile_.good()) {
		ERR(L"Failed to open log file '%s'", path.c_str());
	} else {
		logToFile_ = true;
	}
}

void DebugConsole::CloseLogFile()
{
	if (!logToFile_) return;

	logFile_.close();
	logToFile_ = false;
}
