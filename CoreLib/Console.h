#pragma once

#include <CoreLib/Base/Base.h>
#include <functional>
#include <fstream>

BEGIN_SE()

class Console
{
public:
    using LogCallbackProc = void (DebugMessageType type, char const* message);

    virtual ~Console();
    virtual void Create();
    void Destroy();
    void OpenLogFile(std::wstring const& path);
    void CloseLogFile();

    virtual void Print(DebugMessageType type, char const* msg);
    void SetColor(DebugMessageType type);

    void Clear();
    void EnableOutput(bool enabled);
    void SetLogCallback(LogCallbackProc* callback);

    inline bool WasCreated() const
    {
        return created_;
    }

protected:
    bool created_{ false };
    bool silence_{ false };
    bool inputEnabled_{ false };
    bool enabled_{ false };
    bool logToFile_{ false };
    LogCallbackProc* logCallback_{ nullptr };
    std::ofstream logFile_;
};

END_SE()
