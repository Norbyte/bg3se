#include "stdafx.h"
#include "CrashReporter.h"
#include "HttpUploader.h"

#include <DbgHelp.h>
#include <psapi.h>
#include <CommCtrl.h>
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

std::string ToUTF8(std::wstring const & s)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), (int)s.size(), NULL, 0, NULL, NULL);
	std::string converted;
	converted.resize(size);
	WideCharToMultiByte(CP_UTF8, 0, s.c_str(), (int)s.size(), converted.data(), (int)converted.size(), NULL, NULL);
	return converted;
}


std::wstring FromUTF8(std::string const & s)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), NULL, 0);
	std::wstring converted;
	converted.resize(size);
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), converted.data(), (int)converted.size());
	return converted;
}

CrashReporter::CrashReporter(std::wstring const & miniDumpPath)
	: miniDumpPath_(miniDumpPath)
{}

DWORD WINAPI CrashReporter::MinidumpUploaderThread(LPVOID lpThreadParameter)
{
	auto self = reinterpret_cast<CrashReporter *>(lpThreadParameter);

	std::ifstream f(self->miniDumpPath_, std::ios::in | std::ios::binary);
	if (!f.good()) return 0;
	std::vector<uint8_t> crashDump;
	f.seekg(0, std::ios::end);
	auto dumpSize = f.tellg();
	crashDump.resize(dumpSize);
	f.seekg(0, std::ios::beg);
	f.read((char *)crashDump.data(), dumpSize);

	std::vector<uint8_t> response;
	std::string errorReason;
	std::string dumpId;
	HttpUploader uploader(L"osicrashreports.norbyte.dev");

	bool succeeded = uploader.Upload(L"/submit.php", crashDump, response);
	if (succeeded) {
		if (response.size() < 4 || memcmp(response.data(), "OK:", 3) != 0) {
			errorReason = "Server returned illegible response";
		} else {
			dumpId.resize(response.size() - 3);
			memcpy(dumpId.data(), response.data() + 3, response.size() - 3);
		}
	} else {
		errorReason = uploader.GetLastError();
	}

	if (succeeded) {
		self->resultText_ = L"Crash dump successfully uploaded.\r\nYour crash dump reference number is ";
		self->resultText_  += FromUTF8(dumpId) + L".";
	} else {
		self->resultText_ = L"Unable to upload crash dump. Reason:\r\n";
		self->resultText_ += FromUTF8(errorReason);

	}

	self->uploadSucceeded_ = succeeded;
	SendMessage(self->progressBarWindowHWnd_, TDM_CLICK_BUTTON, IDABORT, 0);

	return 0;
}

bool CrashReporter::ShowUploadConfirmationDialog()
{
	TASKDIALOG_BUTTON buttons[2];
	memset(&buttons, 0, sizeof(buttons));
	buttons[0].nButtonID = IDYES;
	buttons[0].pszButtonText = L"Send Crash Info";
	buttons[1].nButtonID = IDNO;
	buttons[1].pszButtonText = L"Don't Send";

	TASKDIALOGCONFIG config;
	memset(&config, 0, sizeof(config));
	config.cbSize = sizeof(TASKDIALOGCONFIG);
	config.hwndParent = NULL;
	config.hInstance = NULL;

	config.pszWindowTitle = L"Script Extender Crash";
	config.pszMainInstruction = L"The game has unexpectedly crashed.";
	config.pszContent = L"Would you like to send the crash information to the Script Extender team?\r\n"
		"Additional details about what went wrong can help to create a solution.\r\n"
		"No information besides the crash dump will be uploaded.";
	config.pszMainIcon = TD_WARNING_ICON;

	config.cButtons = 2;
	config.pButtons = buttons;
	config.nDefaultButton = IDYES;

	config.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION;
	config.cxWidth = 230;

	int button;
	auto hr = TaskDialogIndirect(&config, &button, NULL, NULL);
	return SUCCEEDED(hr) && button == IDYES;
}

HRESULT CALLBACK CrashReporter::UploadProgressCallbackProc(HWND hwnd,
	UINT uNotification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData)
{
	auto self = reinterpret_cast<CrashReporter *>(dwRefData);

	switch (uNotification)
	{
	case TDN_CREATED:
		SendMessage(hwnd, TDM_SET_PROGRESS_BAR_MARQUEE, TRUE, 0);
		self->progressBarWindowHWnd_ = hwnd;
		CreateThread(NULL, 0, &MinidumpUploaderThread, self, 0, NULL);
		break;
	}

	return S_OK;
}

bool CrashReporter::ShowUploadProgressDialog()
{
	TASKDIALOG_BUTTON button;
	memset(&button, 0, sizeof(button));
	button.nButtonID = IDABORT;
	button.pszButtonText = L"Abort";

	TASKDIALOGCONFIG config;
	memset(&config, 0, sizeof(config));
	config.cbSize = sizeof(TASKDIALOGCONFIG);
	config.hwndParent = NULL;
	config.hInstance = NULL;

	config.cButtons = 1;
	config.pButtons = &button;
	config.nDefaultButton = IDABORT;

	config.pfCallback = UploadProgressCallbackProc;
	config.lpCallbackData = (LONG_PTR)this;

	config.pszWindowTitle = L"Script Extender Crash Reporter";
	config.pszMainInstruction = L"Uploading crash dump ...";

	config.dwFlags = TDF_SHOW_MARQUEE_PROGRESS_BAR;
	auto hr = TaskDialogIndirect(&config, NULL, NULL, NULL);
	return SUCCEEDED(hr);
}

void CrashReporter::Report()
{
	DWORD attributes = GetFileAttributes(miniDumpPath_.c_str());
	if (attributes == INVALID_FILE_ATTRIBUTES) return;

	if (ShowUploadConfirmationDialog()) {
		ShowUploadProgressDialog();

		if (!resultText_.empty()) {
			if (uploadSucceeded_) {
				MessageBoxW(NULL, resultText_.c_str(), L"Script Extender Crash",
					MB_OK | MB_ICONINFORMATION | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
			} else {
				MessageBoxW(NULL, resultText_.c_str(), L"Script Extender Crash",
					MB_OK | MB_ICONWARNING | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
			}
		}
	}
}
