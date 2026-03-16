BEGIN_SE()

class UpdaterUI
{
public:
    void Show();
    void Hide();
    void SetStatusText(std::wstring const& status);

private:
    bool requestShow_{ false };
    HWND progressWindow_{ NULL };
    std::wstring status_;

    void DoShow();

    static HRESULT UICallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LONG_PTR lpRefData);
    static DWORD WINAPI UIThreadMain(LPVOID param);
};

END_SE()
