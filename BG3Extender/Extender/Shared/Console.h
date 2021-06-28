#pragma once

BEGIN_SE()

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
	void SubmitTaskAndWait(bool server, std::function<void()> fun);
};

extern DebugConsole gConsole;

END_SE()
