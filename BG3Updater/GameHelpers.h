#pragma once

#include <CoreLib/SymbolMapper.h>
#include <BG3Extender/GameDefinitions/Enumerations.h>
#include "UpdaterSymbols.h"

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

class GameHelpers
{
public:
	GameHelpers();

	void Initialize();

	void ShowError(char const * msg) const;
	std::optional<ecl::GameState> GetState() const;

	void SuspendClientThread() const;
	void ResumeClientThread() const;

	UpdaterSymbols const& Symbols() const
	{
		return symbols_;
	}

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

	UpdaterSymbols symbols_;

	SymbolMappings mappings_;
	SymbolMapper symbolMapper_;
	SymbolMapper::ModuleInfo appModule_;

	bool CanShowError() const;

	bool ShowErrorDialog(char const * msg) const;
	bool ClientHandleError(char const * msg, bool exitGame) const;

	ThreadInfo const * FindClientThread() const;
	static LONG NTAPI ThreadNameCaptureFilter(_EXCEPTION_POINTERS *ExceptionInfo);
};

std::optional<std::string> GetExeResource(int resourceId);

extern std::unique_ptr<GameHelpers> gGameHelpers;

END_SE()
