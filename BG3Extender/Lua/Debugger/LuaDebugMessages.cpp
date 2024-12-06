#include "stdafx.h"
#include <Lua/Debugger/LuaDebugMessages.h>
#include <Extender/ScriptExtender.h>
#include <lstate.h>
#include <sstream>
#include <shlwapi.h>
#include "resource.h"

#if !defined(OSI_NO_DEBUGGER)

// Hack to avoid issues from debug/release CRT difference between protobuf and extender
#if defined(NDEBUG)
#define DEBUGGER_MSG(name) BackendToDebugger name
#else
#define DEBUGGER_MSG(name) BackendToDebugger* _msg = new BackendToDebugger(); \
    BackendToDebugger& name = *_msg
#endif

BEGIN_NS(lua::dbg)

struct DebugPathInfo
{
    STDString Name;
    STDString Path;

    inline bool operator == (DebugPathInfo const& o) const
    {
        return Name == o.Name
            && Path == o.Path;
    }
};

END_NS()

namespace std
{
    template<> struct hash<bg3se::lua::dbg::DebugPathInfo>
    {
        typedef bg3se::lua::dbg::DebugPathInfo argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& fn) const noexcept
        {
            return std::hash<bg3se::STDString>{}(fn.Name) ^ std::hash<bg3se::STDString>{}(fn.Path);
        }
    };
}

BEGIN_NS(lua::dbg)

DebugMessageHandler::DebugMessageHandler(LuaDebugInterface& intf)
    : intf_(intf)
{
    intf_.SetConnectHandler(
        std::bind(&DebugMessageHandler::HandleConnect, this),
        std::bind(&DebugMessageHandler::HandleDisconnect, this));
    intf_.SetMessageHandler(std::bind(&DebugMessageHandler::HandleMessage, this, std::placeholders::_1));
}

void DebugMessageHandler::SetDebugger(Debugger* debugger)
{
    debugger_ = debugger;
}

void DebugMessageHandler::SendResult(uint32_t seq, ResultCode code)
{
    DEBUGGER_MSG(msg);
    msg.set_reply_seq_no(seq);
    auto results = msg.mutable_results();
    results->set_status_code((StatusCode)code);
    Send(msg);
    DBGMSG(" <-- BkResult(%d)", code);
}

bool LuaIsUserFunction(lua_State* L, CallInfo* ci);

std::optional<STDString> FindLuaSourcePath(STDString const& name)
{
    if (name.starts_with("builtin://") && !gExtender->GetConfig().LuaBuiltinResourceDirectory.empty()) {
        return ToUTF8(gExtender->GetConfig().LuaBuiltinResourceDirectory) + "/" + name.substr(10);
    }

    if (gExtender->GetServer().HasExtensionState()) {
        auto const& files = gExtender->GetServer().GetExtensionState().GetLoadedFileFullPaths();
        auto it = files.find(name);
        if (it != files.end()) {
            return it->second;
        }
    }

    if (gExtender->GetClient().HasExtensionState()) {
        auto const& files = gExtender->GetClient().GetExtensionState().GetLoadedFileFullPaths();
        auto it = files.find(name);
        if (it != files.end()) {
            return it->second;
        }
    }

    return {};
}

void DebugMessageHandler::SendBreakpointTriggered(DbgContext context, BkBreakpointTriggered_Reason reason, 
    lua_State* L, char const* message)
{
    StackCheck _(L);

    lua_Debug ar;
    memset(&ar, 0, sizeof(ar));

    DEBUGGER_MSG(msg);
    auto bkpt = msg.mutable_breakpointtriggered();
    bkpt->set_reason(reason);
    bkpt->set_context(context);
    if (message != nullptr) {
        bkpt->set_message(message);
    }

    for (int i = 0;; i++) {
        if (lua_getstack(L, i, &ar) == 0) {
            break;
        }

        if (lua_getinfo(L, "nuSl", &ar) == 0) {
            ERR("DebugMessageHandler::SendBreakpointTriggered(): Couldn't get function info for breakpoint?");
        }

        auto frame = bkpt->add_stack();
        if (ar.source) {
            frame->set_source(ar.source);
            auto path = FindLuaSourcePath(ar.source);
            // Only send a path to the debugger if the file is not loaded from a .pak,
            // otherwise VS won't send a DAP source request
            if (path && PathFileExistsW(FromUTF8(*path).c_str())) {
                frame->set_path(path->c_str());
            }
        }

        if (ar.name) {
            frame->set_function(ar.name);
        }

        frame->set_line(ar.currentline);

        if (LuaIsUserFunction(L, ar.i_ci)) {
            auto closure = clLvalue(ar.i_ci->func);
            auto proto = closure->p;
            if (proto->sizelineinfo) {
                int firstLine = 9999999, lastLine = -1;
                for (int li = 0; li < proto->sizelineinfo; li++) {
                    firstLine = std::min(firstLine, proto->lineinfo[li]);
                    lastLine = std::max(lastLine, proto->lineinfo[li]);
                }

                frame->set_scope_first_line(firstLine);
                frame->set_scope_last_line(lastLine);
            }
        }
    }

    Send(msg);
    DBGMSG(" <-- BkBreakpointTriggered");
}

void DebugMessageHandler::SendEvaluateResponse(DebuggerEvaluateRequest const& req)
{
    req.Msg->set_reply_seq_no(req.Seq);
    Send(*req.Msg);

    if (req.Response->error_message().empty()) {
        DBGMSG(" <-- BkEvaluateResponse(OK)");
    } else {
        DBGMSG(" <-- BkEvaluateResponse(): \"%s\"", req.Response->error_message().c_str());
    }
}

void DebugMessageHandler::SendLuaStateUpdate(bool server, bool loaded)
{
    if (!IsConnected()) return;

    DEBUGGER_MSG(msg);
    auto ctx = msg.mutable_contextupdated();
    ctx->set_context(server ? DbgContext::SERVER : DbgContext::CLIENT);
    ctx->set_status(loaded ? BkContextUpdated_Status_LOADED : BkContextUpdated_Status_UNLOADED);

    Send(msg);
    DBGMSG(" <-- BkContextUpdated(%s, %d)", server ? "Server" : "Client", loaded ? 1 : 0);
}

void DebugMessageHandler::SendModInfo()
{
    // Create a list of paths from both client and server
    std::unordered_map<FixedString, ExtensionModConfig> const* configs{ nullptr };
    ModManager* modManager{ nullptr };

    std::unordered_set<DebugPathInfo> paths;
    if (gExtender->GetClient().HasExtensionState()) {
        auto& state = gExtender->GetClient().GetExtensionState();
        for (auto const& path : state.GetLoadedFileFullPaths()) {
            paths.insert(DebugPathInfo{ path.first, path.second });
        }

        configs = &state.GetConfigs();
        modManager = state.GetModManager();
    }

    if (gExtender->GetServer().HasExtensionState()) {
        auto& state = gExtender->GetServer().GetExtensionState();
        for (auto const& path : state.GetLoadedFileFullPaths()) {
            paths.insert(DebugPathInfo{ path.first, path.second });
        }

        if (configs == nullptr) {
            configs = &state.GetConfigs();
            modManager = state.GetModManager();
        }
    }

    DEBUGGER_MSG(msg);
    auto info = msg.mutable_modinforesponse();

    if (configs != nullptr && modManager != nullptr) {
        for (auto const& config : *configs) {
            auto mod = modManager->FindModByNameGuid(config.first.GetString());
            if (mod != nullptr) {
                auto modInfo = info->add_module();
                modInfo->set_uuid(config.first.GetString());
                modInfo->set_name(mod->Info.Name.c_str());
                modInfo->set_author(mod->Info.Author.c_str());
                modInfo->set_path(mod->Info.Directory.c_str());
            }
                
        }
    }

    for (auto const& path : paths) {
        auto source = info->add_source();
        source->set_name(path.Name.c_str());
        // Only send a path to the debugger if the file is not loaded from a .pak,
        // otherwise VS won't send a DAP source request
        if (!path.Path.empty() && PathFileExistsW(FromUTF8(path.Path).c_str())) {
            source->set_path(path.Path.c_str());
        }
    }

    Send(msg);
    DBGMSG(" <-- BkModInfoResponse()");
}

void DebugMessageHandler::SendDebuggerReady()
{
    DEBUGGER_MSG(msg);
    auto debugMsg = msg.mutable_debuggerready();
    Send(msg);
    DBGMSG(" <-- BkDebuggerReady()");
}

void DebugMessageHandler::SendSourceResponse(uint32_t seq, STDString const& path, STDString const& body)
{
    DEBUGGER_MSG(msg);
    msg.set_reply_seq_no(seq);
    auto resp = msg.mutable_sourceresponse();
    resp->set_name(path.c_str());
    resp->set_body(body.c_str());
    Send(msg);
    DBGMSG(" <-- BkSourceResponse(%s)", path.c_str());
}

void DebugMessageHandler::SendGetVariablesResponse(DebuggerGetVariablesRequest const& req)
{
    req.Msg->set_reply_seq_no(req.Seq);
    Send(*req.Msg);

    if (req.Response->error_message().empty()) {
        DBGMSG(" <-- BkGetVariablesResponse(OK)");
    } else {
        DBGMSG(" <-- BkGetVariablesResponse(): \"%s\"", req.Response->error_message().c_str());
    }
}

void DebugMessageHandler::SendDebugOutput(DebugMessageType type, char const* message)
{
    DEBUGGER_MSG(msg);
    auto debugMsg = msg.mutable_debugoutput();
    debugMsg->set_message(message);

    switch (type) {
    case DebugMessageType::Debug:
        debugMsg->set_severity(BkDebugOutput::LEVEL_DEBUG);
        break;
    case DebugMessageType::Info:
    case DebugMessageType::Osiris:
        debugMsg->set_severity(BkDebugOutput::LEVEL_INFO);
        break;
    case DebugMessageType::Warning:
        debugMsg->set_severity(BkDebugOutput::LEVEL_WARNING);
        break;
    case DebugMessageType::Error:
        debugMsg->set_severity(BkDebugOutput::LEVEL_ERROR);
        break;
    default:
        debugMsg->set_severity(BkDebugOutput::LEVEL_INFO);
        break;
    }

    Send(msg);
    DBGMSG(" <-- BkDebugOutput(): \"%s\"", message);
}

void DebugMessageHandler::SendConnectResponse(uint32_t seq)
{
    DEBUGGER_MSG(msg);
    auto version = msg.mutable_connectresponse();
    version->set_protocol_version(ProtocolVersion);

    Send(msg);
    DBGMSG(" <-- BkConnectResponse()");
}
    
void DebugMessageHandler::HandleConnectMessage(uint32_t seq, DbgConnectRequest const& req)
{
    DBGMSG(" --> DbgConnectRequest(Version %d)", req.protocol_version());
    SendConnectResponse(seq);

    if (req.protocol_version() != ProtocolVersion) {
        WARN("DebugMessageHandler::HandleConnectMessage(): Client sent unsupported protocol version; got %d, we only support %d",
            req.protocol_version(), ProtocolVersion);
        intf_.Disconnect();
        return;
    }

    SendModInfo();

    if (gExtender->GetServer().HasExtensionState() && gExtender->GetServer().GetExtensionState().GetLua()) {
        SendLuaStateUpdate(true, true);
    } else {
        SendLuaStateUpdate(true, false);
    }

    if (gExtender->GetClient().HasExtensionState() && gExtender->GetClient().GetExtensionState().GetLua()) {
        SendLuaStateUpdate(false, true);
    } else {
        SendLuaStateUpdate(false, false);
    }

    debuggerReady_ = true;
    debugger_->EnableDebugging(true);
    SendDebuggerReady();
}

void DebugMessageHandler::HandleUpdateSettings(uint32_t seq, DbgUpdateSettings const& req)
{
    DBGMSG(" --> DbgUpdateSettings(%d, %d)", req.break_on_error(), req.break_on_generic_error());
    debugger_->UpdateSettings(req.break_on_error(), req.break_on_generic_error());
    SendResult(seq, ResultCode::Success);
}

void DebugMessageHandler::HandleSetBreakpoints(uint32_t seq, DbgSetBreakpoints const& req)
{
    DBGMSG(" --> DbgSetBreakpoints()");

    ResultCode rc = ResultCode::Success;

    debugger_->BeginUpdatingBreakpoints();
    for (auto const& bp : req.breakpoint()) {
        DBGMSG("AddBreakpoint(%s:%d)", bp.path().c_str(), bp.line());
        rc = debugger_->AddBreakpoint(bp.path().c_str(), bp.line());
        if (rc != ResultCode::Success) {
            break;
        }
    }

    debugger_->FinishUpdatingBreakpoints();

    SendResult(seq, rc);
}

void DebugMessageHandler::HandleContinue(uint32_t seq, DbgContinue const& req)
{
    DBGMSG(" --> DbgContinue()");

    ResultCode rc = debugger_->ContinueExecution(req.context(), req.action());
    SendResult(seq, rc);
}

void DebugMessageHandler::HandleEvaluate(uint32_t seq, DbgEvaluate const& req)
{
    DBGMSG(" --> DbgEvaluate(): \"%s\"", req.expression().c_str());

    if (!debugger_) {
        WARN("Evaluate: Not attached to story debugger!");
        SendResult(seq, ResultCode::EvalEngineNotReady);
        return;
    }
        
    auto process = [this](DebuggerEvaluateRequest const& req, ResultCode rc) {
        if (rc == ResultCode::Success || rc == ResultCode::EvalFailed) {
            SendEvaluateResponse(req);
        } else {
            SendResult(req.Seq, rc);
        }
    };

    DebuggerEvaluateRequest evalReq;
    evalReq.Seq = seq;
    evalReq.Context = req.context();
    evalReq.Frame = req.frame();
    evalReq.Expression = req.expression();
    evalReq.Msg = new BackendToDebugger();
    evalReq.Response = evalReq.Msg->mutable_evaluateresponse();
    evalReq.CompletionCallback = process;
    debugger_->Evaluate(evalReq);
}

void DebugMessageHandler::HandleFetchMods(uint32_t seq, DbgFetchMods const& req)
{
    DBGMSG(" --> DbgFetchMods()");
    SendModInfo();
}

void DebugMessageHandler::HandleRequestSource(uint32_t seq, DbgRequestSource const& req)
{
    DBGMSG(" --> DbgRequestSource(%s)", req.name().c_str());

    // Check for a loaded Lua file from mods
    auto path = FindLuaSourcePath(req.name().c_str());
    if (path) {
        auto reader = GetStaticSymbols().MakeFileReader(*path, PathRootType::Root);
        if (!reader.IsLoaded()) {
            LuaError("Script file could not be opened: " << *path);
        } else {
            SendSourceResponse(seq, req.name().c_str(), reader.ToString());
            return;
        }
    }

    // Check for a builtin resource
    auto builtin = gExtender->GetLuaBuiltinBundle().GetResource(req.name().c_str());
    if (!builtin && req.name().find_first_of("/") == std::string::npos && req.name().find_first_of("\\") == std::string::npos) {
        // Fallback check for builtin libs; some VS code versions strip the path
        // if a protocol is specified in the resource name
        builtin = gExtender->GetLuaBuiltinBundle().GetResource(STDString("Libs/") + req.name().c_str());
    }

    if (builtin) {
        SendSourceResponse(seq, req.name().c_str(), builtin->c_str());
        return;
    }

    SendResult(seq, ResultCode::NoSuchFile);
}

void DebugMessageHandler::HandleGetVariables(uint32_t seq, DbgGetVariables const& req)
{
    DBGMSG(" --> DbgGetVariables(%d, %d, %d)", req.variableref(), req.frame(), req.local());

    if (!debugger_) {
        WARN("GetVariables: Not attached to Lua debugger!");
        SendResult(seq, ResultCode::EvalEngineNotReady);
        return;
    }

    auto process = [this](DebuggerGetVariablesRequest const& req, ResultCode rc) {
        if (rc == ResultCode::Success || rc == ResultCode::EvalFailed) {
            SendGetVariablesResponse(req);
        }
        else {
            SendResult(req.Seq, rc);
        }
    };

    DebuggerGetVariablesRequest varsReq;
    varsReq.Seq = seq;
    varsReq.Context = req.context();
    varsReq.VariablesRef = req.variableref();
    varsReq.Frame = req.frame();
    varsReq.Local = req.local();

    for (auto const& key : req.key()) {
        DebuggerGetVariablesRequest::KeyType ele;
        if (key.type() == MsgTableKey::NUMERIC) {
            ele.Int = key.index();
        } else {
            ele.String = key.key();
        }

        varsReq.Key.push_back(ele);
    }

    varsReq.Msg = new BackendToDebugger();
    varsReq.Response = varsReq.Msg->mutable_getvariablesresponse();
    varsReq.CompletionCallback = process;
    debugger_->GetVariables(varsReq);
}

void DebugMessageHandler::HandleReset(uint32_t seq, DbgReset const& req)
{
    DBGMSG(" --> DbgReset(%d)", req.context());

    if (req.context() == DbgContext::SERVER) {
        if (gExtender->GetServer().HasExtensionState() && gExtender->GetServer().GetExtensionState().GetLua()) {
            gExtender->GetServer().EnqueueTask([]() {
                gExtender->GetServer().ResetLuaState();
            });
        } else {
            WARN("DebugMessageHandler::HandleReset(): Cannot reset - server is not running");
        }
    } else {
        if (gExtender->GetClient().HasExtensionState() && gExtender->GetClient().GetExtensionState().GetLua()) {
            gExtender->GetClient().EnqueueTask([]() {
                gExtender->GetClient().ResetLuaState();
            });
        } else {
            WARN("DebugMessageHandler::HandleReset(): Cannot reset - client is not running");
        }
    }
}

bool DebugMessageHandler::HandleMessage(DebuggerToBackend const* msg)
{
    uint32_t seq = msg->seq_no();
    if (seq != inboundSeq_) {
        ERR("Incorrect sequence number in message; expected %d, got %d", inboundSeq_, seq);
        return false;
    }

    inboundSeq_++;

    switch (msg->msg_case()) {
    case DebuggerToBackend::kConnect:
        HandleConnectMessage(seq, msg->connect());
        break;

    case DebuggerToBackend::kUpdateSettings:
        HandleUpdateSettings(seq, msg->updatesettings());
        break;

    case DebuggerToBackend::kSetBreakpoints:
        HandleSetBreakpoints(seq, msg->setbreakpoints());
        break;

    case DebuggerToBackend::kContinue:
        HandleContinue(seq, msg->continue_());
        break;

    case DebuggerToBackend::kEvaluate:
        HandleEvaluate(seq, msg->evaluate());
        break;

    case DebuggerToBackend::kFetchMods:
        HandleFetchMods(seq, msg->fetchmods());
        break;

    case DebuggerToBackend::kRequestSource:
        HandleRequestSource(seq, msg->requestsource());
        break;

    case DebuggerToBackend::kGetVariables:
        HandleGetVariables(seq, msg->getvariables());
        break;

    case DebuggerToBackend::kReset:
        HandleReset(seq, msg->reset());
        break;

    default:
        ERR("DebugMessageHandler::HandleMessage(): Unknown message type received: %d", msg->msg_case());
        return false;
    }

    return true;
}

void DebugMessageHandler::HandleConnect()
{
    INFO("Connected to Lua debugger frontend");

    outboundSeq_ = 1;
    inboundSeq_ = 1;
}

void DebugMessageHandler::HandleDisconnect()
{
    INFO("Disconnected from Lua debugger frontend");
    debuggerReady_ = false;
    if (debugger_) {
        debugger_->EnableDebugging(false);
    }
}

void DebugMessageHandler::Send(BackendToDebugger& msg)
{
    if (intf_.IsConnected()) {
        msg.set_seq_no(outboundSeq_++);
        intf_.Send(msg);
    }
}

END_NS()

#endif
