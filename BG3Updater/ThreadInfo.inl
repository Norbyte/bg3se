#include <ThreadInfo.h>

BEGIN_SE()

GameThreadRegistry* gThreadRegistry{ nullptr };

GameThreadRegistry::GameThreadRegistry()
{
    auto hProcess = GetCurrentProcess();
    DuplicateHandle(hProcess, GetCurrentThread(), hProcess, &mainThread_, 0, FALSE, DUPLICATE_SAME_ACCESS);

    gThreadRegistry = this;
    exceptionHandler_ = AddVectoredExceptionHandler(1, &ThreadNameCaptureFilter);
}

GameThreadRegistry::~GameThreadRegistry()
{
    if (exceptionHandler_ != NULL) {
        RemoveVectoredExceptionHandler(exceptionHandler_);
    }
    gThreadRegistry = nullptr;
}

bool GameThreadRegistry::SuspendClientThread() const
{
    auto thread = FindClientThread();
    if (thread != nullptr) {
        auto hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, thread->ThreadId);
        if (hThread && hThread != INVALID_HANDLE_VALUE) {
            SuspendThread(hThread);
            CloseHandle(hThread);
            return true;
        }
    } else {
        DEBUG("Could not suspend client thread (thread not found!)");
        SuspendThread(mainThread_);
    }

    return false;
}

bool GameThreadRegistry::ResumeClientThread() const
{
    auto thread = FindClientThread();
    if (thread != nullptr) {
        auto hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, thread->ThreadId);
        if (hThread && hThread != INVALID_HANDLE_VALUE) {
            ResumeThread(hThread);
            CloseHandle(hThread);
            return true;
        }
    } else {
        DEBUG("Could not resume client thread (thread not found!)");
        ResumeThread(mainThread_);
    }

    return false;
}

GameThreadInfo const * GameThreadRegistry::FindClientThread() const
{
    for (auto const & it : threads_) {
        if (it.Name == "ClientInit" || it.Name == "ClientLoadModule") {
            return &it;
        }
    }

    return nullptr;
}

LONG NTAPI GameThreadRegistry::ThreadNameCaptureFilter(_EXCEPTION_POINTERS *ExceptionInfo)
{
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == 0x406D1388) {
        auto info = reinterpret_cast<THREADNAME_INFO *>(&ExceptionInfo->ExceptionRecord->ExceptionInformation);
        if (info->dwType == 0x1000 && info->dwFlags == 0) {
            GameThreadInfo thread{
                .ThreadId = info->dwThreadID,
                .Name = info->szName
            };
            gThreadRegistry->threads_.push_back(thread);
        }
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

END_SE()
