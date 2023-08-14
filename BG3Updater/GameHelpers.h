#pragma once

#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Symbols.h>
#include <GameHooks/SymbolMapper.h>

#if defined(_DEBUG)
#define HAS_DEBUG_LOGGING
#endif

BEGIN_SE()

struct THREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
};

void DebugMsg(char const *);

template <typename... Args>
void Debug(char const * fmt, Args... args)
{
	char buf[1024];
	int length = sprintf_s(buf, 1024 - 1, fmt, args...);
	buf[length++] = 0;
	DebugMsg(buf);
}

#if defined(HAS_DEBUG_LOGGING)
#define DEBUG(msg, ...) Debug(__FUNCTION__ "(): " msg, __VA_ARGS__)
#define ERR(msg, ...) Debug(__FUNCTION__ "(): " msg, __VA_ARGS__)
#else
#define DEBUG(msg, ...)
#define ERR(msg, ...)
#endif

class GameHelpers
{
public:
	GameHelpers();

	void Initialize();

	void ShowError(char const * msg) const;
	std::optional<ecl::GameState> GetState() const;

	void SuspendClientThread() const;
	void ResumeClientThread() const;

private:
	typedef void(*EoCClient__HandleError)(void * self, std::string const& message, bool exitGame, std::string const& a4);

	struct ThreadInfo
	{
		DWORD ThreadId;
		std::string Name;
	};

	HMODULE eocApp_{ NULL };
	uint8_t const * moduleStart_{ nullptr };
	std::size_t moduleSize_{ 0 };
	std::vector<ThreadInfo> threads_;

	SymbolMappings mappings_;
	SymbolMapper symbolMapper_;
	SymbolMapper::ModuleInfo appModule_;

	bool CanShowError() const;

	bool ShowErrorDialog(char const * msg) const;
	void ClientHandleError(char const * msg, bool exitGame) const;

	ThreadInfo const * FindClientThread() const;
	static LONG NTAPI ThreadNameCaptureFilter(_EXCEPTION_POINTERS *ExceptionInfo);
};

std::optional<std::string> GetExeResource(int resourceId);

extern std::unique_ptr<GameHelpers> gGameHelpers;

END_SE()
