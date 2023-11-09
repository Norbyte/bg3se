#include "stdafx.h"
#include <CoreLib/Console.h>
#include <iostream>

BEGIN_SE()


Console::~Console()
{
	Destroy();
}

void Console::SetColor(DebugMessageType type)
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

void Console::Print(DebugMessageType type, char const* msg)
{
	if (enabled_ && (!inputEnabled_ || !silence_)) {
		SetColor(type);
		OutputDebugStringA(msg);
		OutputDebugStringA("\r\n");
		std::cout << msg << std::endl;
		std::cout.flush();
		SetColor(DebugMessageType::Debug);
	}

	if (logCallback_) {
		logCallback_(msg);
	}

	if (logToFile_) {
		logFile_.write(msg, strlen(msg));
		logFile_.write("\r\n", 2);
		logFile_.flush();
	}
}

void Console::Clear()
{
	// Clear screen, move cursor to top-left and clear scrollback
	std::cout << "\x1b[2J" "\x1b[H" "\x1b[3J";
}

void Console::EnableOutput(bool enabled)
{
	enabled_ = enabled;
}

void Console::SetLogCallback(LogCallbackProc* callback)
{
	logCallback_ = callback;
}

void Console::Create()
{
	AllocConsole();

	if (IsValidCodePage(CP_UTF8)) {
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);
	}

	auto hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleMode(hStdout, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	// Disable Ctrl+C handling
	SetConsoleCtrlHandler(NULL, TRUE);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	GetCurrentConsoleFontEx(hStdout, FALSE, &cfi);

	// Change to a more readable font if user has one of the default eyesore fonts
	if (wcscmp(cfi.FaceName, L"Terminal") == 0 || wcscmp(cfi.FaceName, L"Courier New") || (cfi.FontFamily & TMPF_VECTOR) == 0) {
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;
		cfi.dwFontSize.Y = 14;
		cfi.FontFamily = FF_MODERN | TMPF_VECTOR | TMPF_TRUETYPE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Lucida Console");
		SetCurrentConsoleFontEx(hStdout, FALSE, &cfi);
	}

	FILE * outputStream;
	freopen_s(&outputStream, "CONOUT$", "w", stdout);
	FILE* inputStream;
	freopen_s(&inputStream, "CONIN$", "r", stdin);
	created_ = true;
}

void Console::Destroy()
{
	if (!created_) return;

	FreeConsole();
	created_ = false;
}

void Console::OpenLogFile(std::wstring const& path)
{
	if (logToFile_) {
		CloseLogFile();
	}

	logFile_.rdbuf()->pubsetbuf(0, 0);
	logFile_.open(path.c_str(), std::ios::binary | std::ios::out | std::ios::app);
	if (!logFile_.good()) {
		ERR("Failed to open log file '%s'", ToStdUTF8(path).c_str());
	} else {
		logToFile_ = true;
	}
}

void Console::CloseLogFile()
{
	if (!logToFile_) return;

	logFile_.close();
	logToFile_ = false;
}


END_SE()
