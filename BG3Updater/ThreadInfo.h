#pragma once

BEGIN_SE()

struct THREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
};

struct GameThreadInfo
{
    DWORD ThreadId;
    std::string Name;
};

class GameThreadRegistry
{
public:
    GameThreadRegistry();
    ~GameThreadRegistry();

    GameThreadInfo const* FindClientThread() const;

    bool SuspendClientThread() const;
    bool ResumeClientThread() const;

private:
    HANDLE mainThread_{ NULL };
    PVOID exceptionHandler_{ NULL };
    std::vector<GameThreadInfo> threads_;

    static LONG NTAPI ThreadNameCaptureFilter(_EXCEPTION_POINTERS *ExceptionInfo);
};

END_SE()
