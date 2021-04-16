#pragma once

#include <string>

class CrashReporter
{
public:
	CrashReporter(std::wstring const & miniDumpPath);

	void Report();

private:
	std::wstring miniDumpPath_;
	HWND progressBarWindowHWnd_;
	bool uploadSucceeded_;
	std::wstring resultText_;


	static HRESULT CALLBACK UploadProgressCallbackProc(HWND hwnd,
		UINT uNotification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData);
	bool ShowUploadProgressDialog();

	static HRESULT CALLBACK UploadConfirmationCallbackProc(HWND hwnd,
		UINT uNotification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData);
	bool ShowUploadConfirmationDialog();

	static DWORD WINAPI MinidumpUploaderThread(LPVOID lpThreadParameter);
};
