#include "stdafx.h"
#include "Lua/LuaDebugMessages.h"
#include <Extender/ScriptExtender.h>
#include <lstate.h>
#include <sstream>
#include "resource.h"

#if !defined(OSI_NO_DEBUGGER)

// Hack to avoid issues from debug/release CRT difference between protobuf and extender
#if defined(NDEBUG)
#define DEBUGGER_MSG(name) BackendToDebugger name
#else
#define DEBUGGER_MSG(name) BackendToDebugger* _msg = new BackendToDebugger(); \
	BackendToDebugger& name = *_msg
#endif

namespace bg3se::lua::dbg
{
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
		DEBUG(" <-- BkResult(%d)", code);
	}

	bool LuaIsUserFunction(lua_State* L, CallInfo* ci);

	std::optional<STDString> FindLuaSourcePath(STDString const& name)
	{
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

			if (lua_getinfo(L, "fnuSl", &ar) == 0) {
				ERR("DebugMessageHandler::SendBreakpointTriggered(): Couldn't get function info for breakpoint?");
			}

			auto frame = bkpt->add_stack();
			if (ar.source) {
				frame->set_source(ar.source);
				auto path = FindLuaSourcePath(ar.source);
				if (path) {
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
		DEBUG(" <-- BkBreakpointTriggered");
	}

	void DebugMessageHandler::SendEvaluateResponse(DebuggerEvaluateRequest const& req)
	{
		req.Msg->set_reply_seq_no(req.Seq);
		Send(*req.Msg);

		if (req.Response->error_message().empty()) {
			DEBUG(" <-- BkEvaluateResponse(OK)");
		} else {
			DEBUG(" <-- BkEvaluateResponse(): \"%s\"", req.Response->error_message().c_str());
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
		DEBUG(" <-- BkContextUpdated(%s, %d)", server ? "Server" : "Client", loaded ? 1 : 0);
	}

	void DebugMessageHandler::SendModInfo()
	{
		// Create a list of paths from both client and server
		std::unordered_map<FixedString, ExtensionModConfig> const* configs{ nullptr };
		ModManager* modManager{ nullptr };

		std::unordered_set<STDString> paths;
		if (gExtender->GetClient().HasExtensionState()) {
			auto& state = gExtender->GetClient().GetExtensionState();
			for (auto const& path : state.GetLoadedFileFullPaths()) {
				paths.insert(path.second);
			}

			configs = &state.GetConfigs();
			modManager = state.GetModManager();
		}

		if (gExtender->GetServer().HasExtensionState()) {
			auto& state = gExtender->GetServer().GetExtensionState();
			for (auto const& path : state.GetLoadedFileFullPaths()) {
				paths.insert(path.second);
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
					modInfo->set_name(ToUTF8(mod->Info.Name).c_str());
					modInfo->set_author(ToUTF8(mod->Info.Author).c_str());
					modInfo->set_path(ToUTF8(mod->Info.Directory).c_str());
				}
				
			}
		}

		for (auto const& path : paths) {
			auto source = info->add_source();
			source->set_path(path.c_str());
		}

		Send(msg);
		DEBUG(" <-- BkModInfoResponse()");
	}

	void DebugMessageHandler::SendDebuggerReady()
	{
		DEBUGGER_MSG(msg);
		auto debugMsg = msg.mutable_debuggerready();
		Send(msg);
		DEBUG(" <-- BkDebuggerReady()");
	}

	void DebugMessageHandler::SendSourceResponse(uint32_t seq, STDString const& path, STDString const& body)
	{
		DEBUGGER_MSG(msg);
		msg.set_reply_seq_no(seq);
		auto resp = msg.mutable_sourceresponse();
		resp->set_name(path.c_str());
		resp->set_body(body.c_str());
		Send(msg);
		DEBUG(" <-- BkSourceResponse(%s)", path.c_str());
	}

	void DebugMessageHandler::SendGetVariablesResponse(DebuggerGetVariablesRequest const& req)
	{
		req.Msg->set_reply_seq_no(req.Seq);
		Send(*req.Msg);

		if (req.Response->error_message().empty()) {
			DEBUG(" <-- BkGetVariablesResponse(OK)");
		} else {
			DEBUG(" <-- BkGetVariablesResponse(): \"%s\"", req.Response->error_message().c_str());
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
		DEBUG(" <-- BkDebugOutput(): \"%s\"", message);
	}

	void DebugMessageHandler::SendConnectResponse(uint32_t seq)
	{
		DEBUGGER_MSG(msg);
		auto version = msg.mutable_connectresponse();
		version->set_protocol_version(ProtocolVersion);

		Send(msg);
		DEBUG(" <-- BkConnectResponse()");
	}
	
	void DebugMessageHandler::HandleConnectMessage(uint32_t seq, DbgConnectRequest const& req)
	{
		DEBUG(" --> DbgConnectRequest(Version %d)", req.protocol_version());
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
		DEBUG(" --> DbgUpdateSettings(%d, %d)", req.break_on_error(), req.break_on_generic_error());
		debugger_->UpdateSettings(req.break_on_error(), req.break_on_generic_error());
		SendResult(seq, ResultCode::Success);
	}

	void DebugMessageHandler::HandleSetBreakpoints(uint32_t seq, DbgSetBreakpoints const& req)
	{
		DEBUG(" --> DbgSetBreakpoints()");

		ResultCode rc = ResultCode::Success;

		debugger_->BeginUpdatingBreakpoints();
		for (auto const& bp : req.breakpoint()) {
			DEBUG("AddBreakpoint(%s:%d)", bp.path().c_str(), bp.line());
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
		DEBUG(" --> DbgContinue()");

		ResultCode rc = debugger_->ContinueExecution(req.context(), req.action());
		SendResult(seq, rc);
	}

	void DebugMessageHandler::HandleEvaluate(uint32_t seq, DbgEvaluate const& req)
	{
		DEBUG(" --> DbgEvaluate(): \"%s\"", req.expression().c_str());

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
		DEBUG(" --> DbgFetchMods()");
		SendModInfo();
	}

	void DebugMessageHandler::HandleRequestSource(uint32_t seq, DbgRequestSource const& req)
	{
		DEBUG(" --> DbgRequestSource(%s)", req.name().c_str());

		// Check for a loaded Lua file from mods
		auto path = FindLuaSourcePath(req.name().c_str());
		if (path) {
			FileReaderPin reader(*path);
			if (reader.IsLoaded()) {
				SendSourceResponse(seq, req.name().c_str(), reader.ToString());
				return;
			}
		}

		// Check for builtins
		int resourceId{ 0 };
		if (req.name() == "BuiltinLibrary.lua") {
			resourceId = IDR_LUA_BUILTIN_LIBRARY;
		} else if (req.name() == "BuiltinLibraryClient.lua") {
			resourceId = IDR_LUA_BUILTIN_LIBRARY_CLIENT;
		} else if (req.name() == "BuiltinLibraryServer.lua") {
			resourceId = IDR_LUA_BUILTIN_LIBRARY_SERVER;
		} else if (req.name() == "Game.Math.lua") {
			resourceId = IDR_LUA_GAME_MATH;
		} else if (req.name() == "Game.Tooltip.lua") {
			resourceId = IDR_LUA_GAME_TOOLTIP;
		}

		if (resourceId) {
			auto body = State::GetBuiltinLibrary(resourceId);
			SendSourceResponse(seq, req.name().c_str(), body);
			return;
		}

		LuaError("Script file could not be opened: " << *path);
		SendResult(seq, ResultCode::NoSuchFile);
	}

	void DebugMessageHandler::HandleGetVariables(uint32_t seq, DbgGetVariables const& req)
	{
		DEBUG(" --> DbgGetVariables(%d, %d, %d)", req.variableref(), req.frame(), req.local());

		if (!debugger_) {
			WARN("GetVariables: Not attached to story debugger!");
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
}

#endif
