#include <Shlwapi.h>
#include <CommCtrl.h>
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")

BEGIN_SE()

void UpdaterUI::Show()
{
    requestShow_ = true;
    CreateThread(NULL, 0, &UIThreadMain, this, 0, NULL);
}

void UpdaterUI::Hide()
{
    requestShow_ = false;
    if (progressWindow_) {
        SendMessage(progressWindow_, TDM_CLICK_BUTTON, static_cast<WPARAM>(TDCBF_CANCEL_BUTTON), 0);
    }
}

void UpdaterUI::DoShow()
{
    Sleep(1000);

    if (!requestShow_) {
        return;
    }

    TASKDIALOG_BUTTON button;
    memset(&button, 0, sizeof(button));
    button.nButtonID = IDCANCEL;
    button.pszButtonText = L"Cancel";

    TASKDIALOGCONFIG config;
    memset(&config, 0, sizeof(config));
    config.cbSize = sizeof(TASKDIALOGCONFIG);
    config.hwndParent = NULL;
    config.hInstance = NULL;

    config.cButtons = 1;
    config.pButtons = &button;
    config.nDefaultButton = IDCANCEL;

    config.pfCallback = &UICallback;
    config.lpCallbackData = (LONG_PTR)this;

    // Ensure string doesn't go poof during construction
    auto status = status_;
    config.pszWindowTitle = L"Script Extender Updater";
    config.pszMainInstruction = L"Checking for Script Extender updates";
    config.pszContent = status.c_str();

    config.dwFlags = TDF_SHOW_MARQUEE_PROGRESS_BAR;
    TaskDialogIndirect(&config, NULL, NULL, NULL);
    progressWindow_ = NULL;
}

void UpdaterUI::SetStatusText(std::wstring const& status)
{
    status_ = status;
    if (progressWindow_ != NULL) {
        SendMessage(progressWindow_, TDM_UPDATE_ELEMENT_TEXT, TDE_CONTENT, (LONG_PTR)status_.c_str());
    }
}

DWORD WINAPI UpdaterUI::UIThreadMain(LPVOID param)
{
    auto self = reinterpret_cast<UpdaterUI*>(param);
    self->DoShow();
    return 0;
}

HRESULT UpdaterUI::UICallback(HWND hwnd, UINT uNotification, WPARAM wParam, LPARAM lParam, LONG_PTR lpRefData)
{
    auto self = reinterpret_cast<UpdaterUI*>(lpRefData);

    switch (uNotification)
    {
    case TDN_CREATED:
        PostMessage(hwnd, TDM_SET_PROGRESS_BAR_MARQUEE, TRUE, 0);
        self->progressWindow_ = hwnd;

        if (!self->requestShow_) {
            PostMessage(hwnd, TDM_CLICK_BUTTON, static_cast<WPARAM>(TDCBF_CANCEL_BUTTON), 0);
        }
        break;

    case TDN_BUTTON_CLICKED:
        if (self->requestShow_ && wParam == IDCANCEL) {
            gUpdater->RequestCancelUpdate();
        }
        break;
    }

    return S_OK;
}

END_SE()
