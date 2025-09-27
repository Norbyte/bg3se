#include <stdafx.h>
#include <Extender/Version.h>
#include <Extender/Shared/Console.h>
#include <Extender/ScriptExtender.h>

BEGIN_SE()

char const* BuildDate = __DATE__ " " __TIME__;

void DebugConsole::SubmitTaskAndWait(bool server, std::function<void()> task)
{
    if (server) {
        auto state = GetStaticSymbols().GetServerState();
        if (!state) {
            ERR("Cannot queue server commands when the server state machine is not initialized");
        } else if (*state == esv::GameState::Paused || *state == esv::GameState::Running) {
            gExtender->GetServer().SubmitTaskAndWait(task);
        } else {
            ERR("Cannot queue server commands in game state %s", EnumInfo<esv::GameState>::Find(*state).GetString());
        }
    } else {
        auto state = GetStaticSymbols().GetClientState();
        if (!state) {
            ERR("Cannot queue client commands when the client state machine is not initialized");
        } else if (*state == ecl::GameState::Menu 
            || *state == ecl::GameState::Lobby 
            || *state == ecl::GameState::Paused 
            || *state == ecl::GameState::Running) {
            gExtender->GetClient().SubmitTaskAndWait(task);
        } else {
            ERR("Cannot queue client commands in game state %s", EnumInfo<ecl::GameState>::Find(*state).GetString());
        }
    }
}

void DebugConsole::PrintHelp()
{
    DEBUG("Anything typed in will be executed as Lua code except the following special commands:");
    DEBUG("  server - Switch to server context");
    DEBUG("  client - Switch to client context");
    DEBUG("  reset client - Reset client Lua state");
    DEBUG("  reset server - Reset server Lua state");
    DEBUG("  reset - Reset client and server Lua states");
    DEBUG("  silence <on|off> - Enable/disable silent mode (log output when in input mode)");
    DEBUG("  clear - Clear the console");
    DEBUG("  exit - Leave console mode");
    DEBUG("  !<cmd> <arg1> ... <argN> - Trigger Lua \"ConsoleCommand\" event with arguments cmd, arg1, ..., argN");
}

void DebugConsole::ClearFromReset()
{
    // Clear console if the setting is enabled
    if (gExtender->GetConfig().ClearOnReset)
    {
        gCoreLibPlatformInterface.GlobalConsole->Clear();
    }
}

void DebugConsole::ResetLua()
{
    ClearFromReset();

    DEBUG("Resetting Lua states.");
    SubmitTaskAndWait(true, []() {
        gExtender->GetServer().ResetLuaState();
    });

    SubmitTaskAndWait(false, []() {
        if (!gExtender->GetServer().RequestResetClientLuaState()) {
            gExtender->GetClient().ResetLuaState();
        }
    });
}

void DebugConsole::ResetLuaClient()
{
    ClearFromReset();

    DEBUG("Resetting client Lua state.");
    SubmitTaskAndWait(false, []() {
        if (!gExtender->GetServer().RequestResetClientLuaState()) {
            gExtender->GetClient().ResetLuaState();
        }
    });
}

void DebugConsole::ResetLuaServer()
{
    ClearFromReset();

    DEBUG("Resetting server Lua state.");
    SubmitTaskAndWait(true, []() {
        gExtender->GetServer().ResetLuaState();
    });
}

void DebugConsole::ExecLuaCommand(std::string const& cmd)
{
    auto task = [cmd]() {
        auto state = gExtender->GetCurrentExtensionState();

        if (!state) {
            ERR("Extensions not initialized!");
            return;
        }

        LuaVirtualPin pin(*state);
        if (!pin) {
            ERR("Lua state not initialized!");
            return;
        }

        if (cmd[0] == '!') {
            lua::DoConsoleCommandEvent params;
            params.Command = cmd.substr(1);
            pin->ThrowEvent("DoConsoleCommand", params, false);
        } else {
            auto L = pin->GetState();
            lua::StaticLifetimeStackPin _(L, lua::LifetimeHandle{});
            lua::ProfilerStackGuard _p(&*pin);
            if (luaL_loadstring(L, cmd.c_str()) || lua::CallWithTraceback(L, 0, 0)) { // stack: errmsg
                ERR("%s", lua_tostring(L, -1));
                lua_pop(L, 1);
            }
        }
    };

    SubmitTaskAndWait(serverContext_, task);
}

void DebugConsole::HandleCommand(std::string const& cmd)
{
    if (cmd.empty()) {
    } else if (cmd == "server") {
        DEBUG("Switching to server context.");
        serverContext_ = true;
    } else if (cmd == "client") {
        DEBUG("Switching to client context.");
        serverContext_ = false;
#if defined(_DEBUG)
    } else if (cmd == "debugbreak") {
        gCoreLibPlatformInterface.EnableDebugBreak = !gCoreLibPlatformInterface.EnableDebugBreak;
        if (gCoreLibPlatformInterface.EnableDebugBreak) {
            DEBUG("Debug breaks ON");
        } else {
            DEBUG("Debug breaks OFF");
        }
#endif
    } else if (cmd == "reset") {
        ResetLua();
    } else if (cmd == "reset server") {
        ResetLuaServer();
    } else if (cmd == "reset client") {
        ResetLuaClient();
    } else if (cmd == "silence on") {
        DEBUG("Silent mode ON");
        silence_ = true;
    } else if (cmd == "silence off") {
        DEBUG("Silent mode OFF");
        silence_ = false;
    } else if (cmd == "clear") {
        Clear();
    } else if (cmd == "help") {
        PrintHelp();
    } else {
        ExecLuaCommand(cmd);
    }
}

void DebugConsole::Print(DebugMessageType type, char const* msg)
{
    Console::Print(type, msg);

    if (!silence_ && gExtender) {
        auto debugger = gExtender->GetLuaDebugger();
        if (debugger && debugger->IsDebuggerReady()) {
            debugger->OnLogMessage(type, msg);
        }
    }
}

void DebugConsole::ConsoleThread()
{
    while (consoleRunning_) {
        wchar_t tempBuf;
        DWORD tempRead{ 0 };
        if (!ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE), &tempBuf, 1, &tempRead, NULL)
            || !tempRead
            || tempBuf != '\r') {
            continue;
        }

        DEBUG("Entering server Lua console.");

        InputLoop();

        DEBUG("Exiting console mode.");
    }
}

void DebugConsole::InputLoop()
{
    std::string line;
    while (consoleRunning_) {
        UpdateConsoleSize();

        inputEnabled_ = true;
        if (serverContext_) {
            std::cout << "S";
        } else {
            std::cout << "C";
        }

        if (multiLineMode_) {
            std::cout << " -->> ";
        } else {
            std::cout << " >> ";
        }

        std::cout.flush();
        std::getline(std::cin, line);

        if (std::cin.fail() || std::cin.eof()) {
            std::cin.clear();
            multiLineMode_ = false;
            multiLineCommand_.clear();
            std::cout << std::endl;
        }

        inputEnabled_ = false;

        if (!multiLineMode_) {
            if (line == "exit") {
                break;
            }

            if (line == "--[[") {
                multiLineMode_ = true;
                multiLineCommand_.clear();
                continue;
            }

            HandleCommand(line);
        } else {
            if (line == "]]--") {
                multiLineMode_ = false;
                HandleCommand(multiLineCommand_);
            } else {
                multiLineCommand_ += line;
                multiLineCommand_ += '\n';
            }
        }
    }
}

void DebugConsole::UpdateConsoleSize()
{
    CONSOLE_SCREEN_BUFFER_INFOEX buf;
    buf.cbSize = sizeof(buf);
    GetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &buf);

    auto width = buf.srWindow.Right - buf.srWindow.Left + 1;
    auto height = buf.srWindow.Bottom - buf.srWindow.Top + 1;

    if (width_ != width || height_ != height) {
        width_ = width;
        height_ = height;
        resized_ = true;
    }
}

void DebugConsole::Create()
{
    Console::Create();
    EnableOutput(true);

    consoleRunning_ = true;
    serverContext_ = !gExtender->GetConfig().DefaultToClientConsole;

    SetConsoleTitleW(L"BG3 Script Extender Debug Console");
    DEBUG("******************************************************************************");
    DEBUG("*                                                                            *");
    DEBUG("*                     BG3 Script Extender Debug Console                      *");
    DEBUG("*                                                                            *");
    DEBUG("******************************************************************************");
    DEBUG("");
    DEBUG("BG3Ext v%d built on %s", CurrentVersion, BuildDate);

    consoleThread_ = new std::thread(&DebugConsole::ConsoleThread, this);
}


END_SE()
