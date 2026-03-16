#pragma once

#include <ThreadInfo.h>

#define EXCLUDE_EXTERNALS
#include <BG3Extender/GameDefinitions/Enumerations.h>

#if defined(_DEBUG)
#define HAS_DEBUG_LOGGING
#endif

BEGIN_SE()

class GameHelpers
{
public:
    GameHelpers();
    ~GameHelpers();

    void ShowError(char const * msg);
    std::optional<ecl::GameState> GetState() const;

private:
    GameThreadRegistry threads_;
    std::string errorMsg_;

    static DWORD WINAPI ShowErrorThreadMain(LPVOID param);
};

HMODULE GetExeHandle();
bool IsInEditor();
std::string trim(std::string const& s);
bool ShouldLoad();
std::optional<std::string> GetExeResource(int resourceId);

extern std::unique_ptr<GameHelpers> gGameHelpers;

END_SE()
