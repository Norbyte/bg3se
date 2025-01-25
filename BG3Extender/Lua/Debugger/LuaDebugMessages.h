#pragma once

#if !defined(OSI_NO_DEBUGGER)

#include <cstdint>
#include <Lua/Debugger/LuaDebug.pb.h>
#include <GameDefinitions/Module.h>
#include <Osiris/Debugger/DebugInterface.h>

#if defined(DBG_PROTOCOL_DEBUG)
#define DBGMSG(...) DEBUG(__VA_ARGS__)
#else
#define DBGMSG(...)
#endif

struct lua_State;
struct lua_Debug;

namespace bg3se::lua::dbg
{
    struct DebuggerEvaluateRequest;
    struct DebuggerGetVariablesRequest;

    enum class ResultCode
    {
        Success = 0,
        NotInPause = 1,
        NoDebuggee = 2,
        InvalidContinueAction = 3,
        InPause = 4,
        EvalEngineNotReady = 5,
        EvalFailed = 6,
        NoSuchFile = 7
    };

    class Debugger;

    class DebugMessageHandler
    {
    public:
        static const uint32_t ProtocolVersion = 4;

        DebugMessageHandler(LuaDebugInterface& intf);

        inline bool IsConnected() const
        {
            return intf_.IsConnected();
        }

        inline bool IsDebuggerReady() const
        {
            return intf_.IsConnected() && debuggerReady_;
        }

        void SetDebugger(Debugger* debugger);

        void SendConnectResponse(uint32_t seq);
        void SendBreakpointTriggered(DbgContext context, BkBreakpointTriggered_Reason reason, 
            lua_State* L, char const* msg);
        void SendEvaluateResponse(DebuggerEvaluateRequest const& req);
        void SendLuaStateUpdate(bool server, bool loaded);
        void SendDebugOutput(DebugMessageType type, char const* message);
        void SendModInfo();
        void SendResult(uint32_t seq, ResultCode code);
        void SendDebuggerReady();
        void SendSourceResponse(uint32_t seq, STDString const& path, STDString const& body);
        void SendGetVariablesResponse(DebuggerGetVariablesRequest const& req);

    private:
        LuaDebugInterface& intf_;
        Debugger* debugger_{ nullptr };
        uint32_t inboundSeq_{ 1 };
        uint32_t outboundSeq_{ 1 };
        bool debuggerReady_{ false };

        bool HandleMessage(dbg::DebuggerToBackend const* msg);
        void HandleConnect();
        void HandleDisconnect();

        void HandleConnectMessage(uint32_t seq, DbgConnectRequest const& req);
        void HandleUpdateSettings(uint32_t seq, DbgUpdateSettings const& req);
        void HandleSetBreakpoints(uint32_t seq, DbgSetBreakpoints const& req);
        void HandleContinue(uint32_t seq, DbgContinue const& req);
        void HandleEvaluate(uint32_t seq, DbgEvaluate const& req);
        void HandleFetchMods(uint32_t seq, DbgFetchMods const& req);
        void HandleRequestSource(uint32_t seq, DbgRequestSource const& req);
        void HandleGetVariables(uint32_t seq, DbgGetVariables const& req);
        void HandleReset(uint32_t seq, DbgReset const& req);

        void Send(dbg::BackendToDebugger& msg);
    };
}

#endif
#pragma once
