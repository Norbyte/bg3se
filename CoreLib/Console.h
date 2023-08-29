#pragma once

#include <CoreLib/Base/Base.h>
#include <functional>
#include <fstream>

BEGIN_SE()

class Console
{
public:
	virtual void Create();
	void OpenLogFile(std::wstring const& path);
	void CloseLogFile();

	void Print(DebugMessageType type, char const* msg);
	void Print(DebugMessageType type, wchar_t const* msg);
	void SetColor(DebugMessageType type);

	void Clear();
	void EnableOutput(bool enabled);

protected:
	bool created_{ false };
	bool silence_{ false };
	bool inputEnabled_{ false };
	bool enabled_{ false };
	bool logToFile_{ false };
	std::ofstream logFile_;
};

END_SE()
