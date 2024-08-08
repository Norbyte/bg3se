#pragma once

#include <GameDefinitions/Base/Base.h>
#include <functional>

BEGIN_SE()

extern char const* BuildDate;

class DebugConsole : public Console
{
public:
	void Create() override;
	void HandleCommand(std::string const& cmd);
	void Print(DebugMessageType type, char const* msg) override;

private:
	bool consoleRunning_{ false };
	bool serverContext_{ true };
	bool multiLineMode_{ false };
	std::thread* consoleThread_{ nullptr };
	std::string multiLineCommand_;
	uint32_t width_{ 0 };
	uint32_t height_{ 0 };
	uint32_t resized_{ false };

	void ConsoleThread();
	void InputLoop();
	void UpdateConsoleSize();
	void SubmitTaskAndWait(bool server, std::function<void()> fun);
	void PrintHelp();
	void ResetLua();
	void ResetLuaClient();
	void ResetLuaServer();
	void ExecLuaCommand(std::string const& cmd);
	void ClearFromReset();
};

END_SE()
