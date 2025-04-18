#pragma once

#include <string>

class CrashReporter
{
public:
    CrashReporter(std::wstring const & miniDumpPath);

    void Report();

private:
    std::wstring miniDumpPath_;
    std::wstring reportPath_;
    std::wstring backtracePath_;
    HWND progressBarWindowHWnd_;
    bool uploadSucceeded_{ false };
    std::wstring resultText_;
    bool quiet_{ false };
    volatile bool uploadFinished_{ false };

    std::string GetBacktrace();

    static HRESULT CALLBACK UploadProgressCallbackProc(HWND hwnd,
        UINT uNotification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData);
    bool ShowUploadProgressDialog();

    static HRESULT CALLBACK UploadConfirmationCallbackProc(HWND hwnd,
        UINT uNotification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData);
    bool ShowUploadConfirmationDialog();

    static DWORD WINAPI MinidumpUploaderThread(LPVOID lpThreadParameter);
};
