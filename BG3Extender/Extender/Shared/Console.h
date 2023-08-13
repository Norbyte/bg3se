#pragma once

#include <GameDefinitions/Base/Base.h>
#include <functional>

BEGIN_SE()

class DebugConsole
{
public:
	void Create();
	void OpenLogFile(std::wstring const& path);
	void CloseLogFile();

	void Debug(DebugMessageType type, char const* msg);
	void Debug(DebugMessageType type, wchar_t const* msg);

	void HandleCommand(std::string const& cmd);

	void Clear();

private:
	bool created_{ false };
	bool inputEnabled_{ false };
	bool silence_{ true };
	bool consoleRunning_{ false };
	bool logToFile_{ false };
	bool serverContext_{ true };
	bool multiLineMode_{ false };
	std::thread* consoleThread_{ nullptr };
	std::ofstream logFile_;
	std::string multiLineCommand_;

	void ConsoleThread();
	void SetColor(DebugMessageType type);
	void SubmitTaskAndWait(bool server, std::function<void()> fun);
	void PrintHelp();
	void ResetLua();
	void ResetLuaClient();
	void ResetLuaServer();
	void ExecLuaCommand(std::string const& cmd);
	void ClearFromReset();
};

extern DebugConsole gConsole;

END_SE()
