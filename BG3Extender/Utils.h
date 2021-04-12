#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <GameDefinitions/BaseTypes.h>

#define BEGIN_SE() namespace bg3se {
#define END_SE() }

#define BEGIN_NS(ns) namespace bg3se::ns {
#define END_NS() }

namespace std
{
	class thread;
}

enum class DebugMessageType
{
	Debug,
	Info,
	Osiris,
	Warning,
	Error
};

class DebugConsole
{
public:
	void Create();
	void OpenLogFile(std::wstring const& path);
	void CloseLogFile();

	void Debug(DebugMessageType type, char const* msg);
	void Debug(DebugMessageType type, wchar_t const* msg);

private:
	bool created_{ false };
	bool inputEnabled_{ false };
	bool silence_{ false };
	bool consoleRunning_{ false };
	bool logToFile_{ false };
	std::thread* consoleThread_{ nullptr };
	std::ofstream logFile_;

	void ConsoleThread();
	void SetColor(DebugMessageType type);
};

extern DebugConsole gConsole;

extern HMODULE gThisModule;

std::string ToUTF8(std::wstring_view s);
std::wstring FromUTF8(std::string_view s);
std::optional<UUID> ParseUuid(std::string_view s);
std::optional<UUID> ParseGuidString(std::string_view s);
std::string FormatUuid(UUID const& uuid);

template <typename... Args>
void Debug(DebugMessageType type, wchar_t const * fmt, Args... args)
{
	wchar_t buf[1024];
	_snwprintf_s(buf, std::size(buf), _TRUNCATE, fmt, args...);
	gConsole.Debug(type, buf);
}

template <typename... Args>
void Debug(DebugMessageType type, char const * fmt, Args... args)
{
	char buf[1024];
	_snprintf_s(buf, std::size(buf), _TRUNCATE, fmt, args...);
	gConsole.Debug(type, buf);
}

#define DEBUG(msg, ...) ::Debug(DebugMessageType::Debug, msg, __VA_ARGS__)
#define INFO(msg, ...) ::Debug(DebugMessageType::Info, msg, __VA_ARGS__)
#define WARN(msg, ...) ::Debug(DebugMessageType::Warning, msg, __VA_ARGS__)
#define ERR(msg, ...) ::Debug(DebugMessageType::Error, msg, __VA_ARGS__)

[[noreturn]]
void Fail(TCHAR const * reason);

[[noreturn]]
void Fail(char const * reason);


#if !defined(OSI_NO_DEBUG_LOG)
#define LuaError(msg) { \
	std::stringstream ss; \
	ss << __FUNCTION__ "(): " msg; \
	LogLuaError(ss.str()); \
}

#define OsiError(msg) { \
	std::stringstream ss; \
	ss << __FUNCTION__ "(): " msg; \
	LogOsirisError(ss.str()); \
}

#define OsiWarn(msg) { \
	std::stringstream ss; \
	ss << __FUNCTION__ "(): " msg; \
	LogOsirisWarning(ss.str()); \
}

#define OsiMsg(msg) { \
	std::stringstream ss; \
	ss << msg; \
	LogOsirisMsg(ss.str()); \
}

#define OsiErrorS(msg) LogOsirisError(__FUNCTION__ "(): " msg)
#define OsiWarnS(msg) LogOsirisWarning(__FUNCTION__ "(): " msg)
#define OsiMsgS(msg) LogOsirisMsg(__FUNCTION__ "(): " msg)
#else
#define LuaError(msg) (void)0
#define OsiError(msg) (void)0
#define OsiWarn(msg) (void)0
#define OsiMsg(msg) (void)0
#define OsiErrorS(msg) (void)0
#define OsiWarnS(msg) (void)0
#define OsiMsgS(msg) (void)0
#endif


void LogLuaError(std::string_view msg);
void LogOsirisError(std::string_view msg);
void LogOsirisWarning(std::string_view msg);
void LogOsirisMsg(std::string_view msg);


extern std::atomic<uint32_t> gDisableCrashReportingCount;

struct DisableCrashReporting
{
	inline DisableCrashReporting()
	{
		++gDisableCrashReportingCount;
	}

	inline ~DisableCrashReporting()
	{
		--gDisableCrashReportingCount;
	}

	DisableCrashReporting(DisableCrashReporting const&) = delete;
	DisableCrashReporting& operator = (DisableCrashReporting const&) = delete;
};
