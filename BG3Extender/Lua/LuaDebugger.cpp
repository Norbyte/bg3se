#include "stdafx.h"
#include <Lua/LuaDebugger.h>
#include <Lua/LuaSerializers.h>
#include <Extender/ScriptExtender.h>
#include <lstate.h>
#include <lapi.h>
#include <lobject.h>
#include <lgc.h>
#include <regex>

#if !defined(OSI_NO_DEBUGGER)

namespace bg3se::lua::dbg
{
	ContextDebugger::ContextDebugger(DebugMessageHandler& messageHandler, DbgContext ctx)
		: messageHandler_(messageHandler), context_(ctx)
	{}

	void LuaHook(lua_State* L, lua_Debug* ar)
	{
		gExtender->GetLuaDebugger()->OnLuaHook(L, ar);
	}

	bool LuaIsUserFunction(lua_State* L, CallInfo* ci)
	{
		return ci != &L->base_ci
			&& isLua(ci)
			&& ttisfunction(ci->func);
	}

	bool LuaIsUserFunction(lua_State* L)
	{
		return LuaIsUserFunction(L, L->ci);
	}

	int LuaCurrentLine(CallInfo* ci)
	{
		auto proto = clLvalue(ci->func)->p;
		auto pc = (int)(ci->u.l.savedpc - proto->code) - 1;
		return proto->lineinfo ? proto->lineinfo[pc] : -1;
	}

	int LuaGetStackDepth(lua_State* L)
	{
		int depth = 0;
		for (auto ci = L->ci; ci != &L->base_ci; ci = ci->previous) {
			depth++;
		}

		return depth;
	}

	void ObjectHandleToProtobuf(ObjectHandle const& handle, MsgValue* value)
	{
		value->set_type_id(MsgValueType::OBJECT_HANDLE);
		value->set_intval((int)handle.Handle);

		if (!handle) {
			value->set_stringval("(null handle)");
		}
		else {
			std::stringstream ss;

			// FIXME - lookup component by type?
			/*auto type = EnumInfo<ExtComponentType>::Find((ExtComponentType)handle.GetType());
			if (type) {
				ss << type;
			}
			else {*/
				ss << "UNKNOWN";
			//}

			ss << " handle 0x" << std::hex << handle.Handle;

			value->set_stringval(ss.str().c_str());
		}
	}

	void LuaToProtobuf(lua_State* L, int idx, MsgValue* value)
	{
		switch (lua_type(L, idx)) {
		case LUA_TNIL:
			value->set_type_id(MsgValueType::NIL);
			break;
		case LUA_TBOOLEAN:
			value->set_type_id(MsgValueType::BOOLEAN);
			value->set_boolval(lua_toboolean(L, idx));
			break;
		case LUA_TLIGHTUSERDATA:
			ObjectHandleToProtobuf(ObjectHandle((uint64_t)lua_touserdata(L, idx)), value);
			break;
		case LUA_TNUMBER:
			value->set_type_id(MsgValueType::FLOAT);
			value->set_floatval((float)lua_tonumber(L, idx));
			break;
		case LUA_TSTRING:
			value->set_type_id(MsgValueType::STRING);
			value->set_stringval(lua_tostring(L, idx));
			break;
		case LUA_TTABLE:
			value->set_type_id(MsgValueType::TABLE);
			break;
		case LUA_TFUNCTION:
			value->set_type_id(MsgValueType::FUNCTION);
			break;
		case LUA_TUSERDATA:
			value->set_type_id(MsgValueType::USERDATA);
			value->set_stringval(luaL_tolstring(L, idx, nullptr));
			lua_pop(L, 1);
			break;
		case LUA_TTHREAD:
			value->set_type_id(MsgValueType::THREAD);
			break;
		default:
			value->set_type_id(MsgValueType::UNKNOWN);
			break;
		}
	}

	void LuaToProtobuf(lua_State* L, int idx, MsgValue* value, DebuggerGetVariablesRequest const& req)
	{
		LuaToProtobuf(L, idx, value);

		if (value->type_id() == MsgValueType::TABLE) {
			auto ref = value->mutable_variables();
			ref->set_frame(req.Frame);
			ref->set_local(req.Local);
			ref->set_variableref(req.VariablesRef);
			for (auto const& key : req.Key) {
				auto newKey = ref->add_key();
				if (key.Int) {
					newKey->set_type(MsgTableKey::NUMERIC);
					newKey->set_index(*key.Int);
				} else {
					newKey->set_type(MsgTableKey::TEXT);
					newKey->set_key(key.String->c_str());
				}
			}
		}
	}

	void StackFrameToEvalResults(lua_State* L, lua_Debug* ar, int frame, BkGetVariablesResponse* response)
	{
		if (!LuaIsUserFunction(L, ar->i_ci)) {
			return;
		}

		auto closure = clLvalue(ar->i_ci->func);
		auto proto = closure->p;
		auto pc = (int)(ar->i_ci->u.l.savedpc - proto->code) - 1;

		int localIdx = 1;
		for (int i = 0; i < proto->sizelocvars; i++) {
			if (pc >= proto->locvars[i].startpc && pc < proto->locvars[i].endpc) {
				auto name = lua_getlocal(L, ar, localIdx);
				if (name) {
					auto var = response->add_result();
					var->set_type(MsgChildValue::TEXT);
					var->set_index(proto->sizelocvars - i);
					var->set_name(name);

					auto val = var->mutable_value();
					LuaToProtobuf(L, -1, val);
					if (val->type_id() == MsgValueType::TABLE) {
						auto ref = val->mutable_variables();
						ref->set_frame(frame);
						ref->set_local(localIdx);
						ref->set_variableref(-1);
					}
					lua_pop(L, 1);
				}

				localIdx++;
			}
		}

		setobjs2s(L, L->top, ar->i_ci->func);
		api_incr_top(L);

		for (int i = 1; i <= proto->sizeupvalues; i++) {
			auto name = lua_getupvalue(L, -1, i);
			if (name) {
				auto var = response->add_result();
				var->set_type(MsgChildValue::TEXT);
				var->set_index(-i);
				var->set_name(name);

				auto val = var->mutable_value();
				LuaToProtobuf(L, -1, val);
				if (val->type_id() == MsgValueType::TABLE) {
					auto ref = val->mutable_variables();
					ref->set_frame(frame);
					ref->set_local(-i - 1);
					ref->set_variableref(-1);
				}
				lua_pop(L, 1);
			}
		}

		lua_pop(L, 1);
	}

	void LuaPushStackLocals(lua_State* L, lua_Debug* ar, std::vector<std::pair<int, STDString>>& locals)
	{
		if (!LuaIsUserFunction(L, ar->i_ci)) {
			return;
		}

		auto closure = clLvalue(ar->i_ci->func);
		auto proto = closure->p;
		auto pc = (int)(ar->i_ci->u.l.savedpc - proto->code) - 1;

		int variableIdx = 1;
		int localIdx = 1;
		for (int i = 0; i < proto->sizelocvars; i++) {
			if (pc >= proto->locvars[i].startpc && pc < proto->locvars[i].endpc) {
				auto name = lua_getlocal(L, ar, localIdx);
				if (name) {
					locals.push_back(std::make_pair(variableIdx, name));
					variableIdx++;
				}

				localIdx++;
			}
		}

		auto funcIdx = lua_gettop(L) + 1;
		setobjs2s(L, L->top, ar->i_ci->func);
		api_incr_top(L);

		for (int i = 1; i <= proto->sizeupvalues; i++) {
			auto name = lua_getupvalue(L, -1, i);
			if (name) {
				locals.push_back(std::make_pair(variableIdx, name));
				variableIdx++;
			}
		}

		lua_remove(L, funcIdx);
	}

	void LuaTableToEvalResults(lua_State* L, int index, DebuggerGetVariablesRequest const& req)
	{
		for (auto idx : iterate(L, index)) {
			auto pair = req.Response->add_result();
			switch (lua_type(L, -2)) {
			case LUA_TNUMBER:
				pair->set_type(MsgChildValue::NUMERIC);
				pair->set_index(lua_tointeger(L, -2));
				break;

			case LUA_TSTRING:
				pair->set_type(MsgChildValue::TEXT);
				pair->set_name(lua_tostring(L, -2));
				break;
			}

			auto val = pair->mutable_value();
			LuaToProtobuf(L, -1, val, req);
			if (val->type_id() == MsgValueType::TABLE) {
				auto key = val->mutable_variables()->add_key();
				switch (lua_type(L, -2)) {
				case LUA_TNUMBER:
					key->set_type(MsgTableKey::NUMERIC);
					key->set_index(lua_tointeger(L, -2));
					break;

				case LUA_TSTRING:
					key->set_type(MsgTableKey::TEXT);
					key->set_key(lua_tostring(L, -2));
					break;
				}
			}
		}
	}

	bool ContextDebugger::IsBreakpoint(lua_State* L, lua_Debug* ar, BkBreakpointTriggered::Reason& reason)
	{
		// Fast-path to avoid expensive lookups if we can't break anyway
		if (!requestPause_ && !breakpoints_) {
			return false;
		}

		int line;
		if (LuaIsUserFunction(L)) {
			line = LuaCurrentLine(L->ci);
			if (line == -1) {
				return false;
			}
		}

		if (requestPause_) {
			auto stackSize = LuaGetStackDepth(L);
			if (stackSize <= pauseMaxStackDepth_) {
				if (pauseMaxStackDepth_ == 0x7fffffff) {
					reason = BkBreakpointTriggered::PAUSE;
				} else {
					reason = BkBreakpointTriggered::STEP;
				}

				return true;
			}
		}

		if (breakpoints_) {
			auto lineIt = breakpoints_->lines.find(line);
			if (lineIt == breakpoints_->lines.end()) {
				return false;
			}

			if (lua_getinfo(L, "fnSl", ar) == 0) {
				return false;
			}


			ExtensionStateBase* state{ nullptr };
			if (context_ == DbgContext::SERVER) {
				state = &gExtender->GetServerExtensionState();
			} else {
				state = &gExtender->GetClientExtensionState();
			}

			auto pathIt = state->GetLoadedFileFullPaths().find(ar->source);
			if (pathIt != state->GetLoadedFileFullPaths().end()) {
				auto fileIt = breakpoints_->breakpoints.find(pathIt->second);
				if (fileIt != breakpoints_->breakpoints.end()) {
					auto lineIt = fileIt->second.find(line);
					if (lineIt != fileIt->second.end()) {
						reason = BkBreakpointTriggered::BREAKPOINT;
						return true;
					}
				}
			}
		}

		return false;
	}

	void ContextDebugger::TriggerBreakpoint(lua_State* L, BkBreakpointTriggered_Reason reason, char const* msg)
	{
		DEBUG("Debugger::TriggerBreakpoint()");

		{
			std::unique_lock<std::mutex> lk(breakpointMutex_);
			isPaused_ = true;
			currentStackDepth_ = LuaGetStackDepth(L);
		}

		if (requestPause_) {
			requestPause_ = false;
			pauseMaxStackDepth_ = -1;
		}

		messageHandler_.SendBreakpointTriggered(context_, reason, L, msg);

		{
			std::unique_lock<std::mutex> lk(breakpointMutex_);
			breakpointCv_.wait(lk, [this]() { this->ExecuteQueuedActions(); return !this->isPaused_; });
		}

		DEBUG("Continuing from breakpoint.");
	}

	void ContextDebugger::OnLuaHook(lua_State* L, lua_Debug* ar)
	{
		ExecuteQueuedActions();
		
		BkBreakpointTriggered::Reason reason = BkBreakpointTriggered::BREAKPOINT;
		if (IsBreakpoint(L, ar, reason)) {
			TriggerBreakpoint(L, reason, nullptr);
		}
	}

	void ContextDebugger::OnLuaError(lua_State* L, char const* msg)
	{
		if (breakOnError_) {
			TriggerBreakpoint(L, BkBreakpointTriggered::EXCEPTION, msg);
		}
	}

	void ContextDebugger::OnGenericError(char const* msg)
	{
		if (breakOnGenericError_) {
			LuaVirtualPin lua(GetExtensionState());
			if (lua) {
				TriggerBreakpoint(lua->GetState(), BkBreakpointTriggered::EXCEPTION, msg);
			}
		}
	}

	void ContextDebugger::DebugBreak(lua_State* L)
	{
		TriggerBreakpoint(L, BkBreakpointTriggered::EXCEPTION, "Ext.DebugBreak() called");
	}

	void ContextDebugger::OnContextCreated(lua_State* L)
	{
		if (enabled_) {
			lua_sethook(L, LuaHook, LUA_MASKLINE, 0);
		}
	}

	void ContextDebugger::EnableDebugging(bool enabled)
	{
		enabled_ = enabled;

		if (!enabled) {
			breakpoints_.reset();
			newBreakpoints_.reset();
			requestPause_ = false;
			pauseMaxStackDepth_ = -1;
			breakOnError_ = false;
			if (isPaused_) {
				ContinueExecution(DbgContinue_Action_CONTINUE);
			}
		}

		LuaVirtualPin lua(GetExtensionState());
		if (lua) {
			if (enabled) {
				lua_sethook(lua->GetState(), LuaHook, LUA_MASKLINE, 0);
			} else {
				lua_sethook(lua->GetState(), nullptr, 0, 0);
			}
		}
	}

	void ContextDebugger::BeginUpdatingBreakpoints()
	{
		newBreakpoints_ = std::make_unique<BreakpointSet>();
	}

	void ContextDebugger::AddBreakpoint(STDString const& path, int line)
	{
		STDString bpPath = path;
		std::replace(bpPath.begin(), bpPath.end(), '\\', '/');
		if (!bpPath.empty()) {
			bpPath[0] = std::tolower(bpPath[0]);
		}

		auto fileIt = newBreakpoints_->breakpoints.find(bpPath);
		if (fileIt == newBreakpoints_->breakpoints.end()) {
			std::unordered_set<int> lines{ line };
			newBreakpoints_->breakpoints.insert(std::make_pair(bpPath, lines));
		} else {
			fileIt->second.insert(line);
		}

		newBreakpoints_->lines.insert(line);
	}

	void ContextDebugger::FinishUpdatingBreakpoints()
	{
		if (newBreakpoints_->breakpoints.empty()) {
			newBreakpoints_.reset();
		}

		auto bps = newBreakpoints_.release();

		pendingActions_.push([=]() {
			breakpoints_.reset(bps);
		});
		breakpointCv_.notify_one();
	}

	void ContextDebugger::UpdateSettings(bool breakOnError, bool breakOnGenericError)
	{
		breakOnError_ = breakOnError;
		breakOnGenericError_ = breakOnGenericError;
	}

	ResultCode ContextDebugger::ContinueExecution(DbgContinue_Action action)
	{
		std::unique_lock<std::mutex> lk(breakpointMutex_);

		if (action == DbgContinue_Action_PAUSE) {
			if (isPaused_) {
				WARN("ContextDebugger::ContinueExecution(): Already paused");
				return ResultCode::InPause;
			}

			DEBUG("ContextDebugger::ContinueExecution(): Force pause");
			// Forcibly break on the next call
			requestPause_ = true;
			pauseMaxStackDepth_ = 0x7fffffff;
			// This is not a "continue" message, it just sets the breakpoint flags,
			// so we don't go through the continue code here
			return ResultCode::Success;
		}

		if (!isPaused_) {
			WARN("ContextDebugger::ContinueExecution(): Not paused");
			return ResultCode::NotInPause;
		}

		switch (action) {
		case DbgContinue_Action_CONTINUE:
			// No forced break
			requestPause_ = false;
			pauseMaxStackDepth_ = -1;
			break;

		case DbgContinue_Action_STEP_OVER:
			// Step over the current frame; max depth is the current call stack depth
			requestPause_ = true;
			pauseMaxStackDepth_ = currentStackDepth_;
			break;

		case DbgContinue_Action_STEP_INTO:
			// Step into the current frame; max depth is unlimited
			requestPause_ = true;
			pauseMaxStackDepth_ = 0x7ffffffe;
			break;

		case DbgContinue_Action_STEP_OUT:
			// Step out of the current frame; max depth is current - 1
			requestPause_ = true;
			pauseMaxStackDepth_ = currentStackDepth_ - 1;
			break;

		default:
			WARN("ContextDebugger::ContinueExecution(): Continue action %d not known", action);
			return ResultCode::InvalidContinueAction;
		}

		DEBUG("ContextDebugger::ContinueExecution(): Continuing; action %d", action);
		isPaused_ = false;
		breakpointCv_.notify_one();

		return ResultCode::Success;
	}

	void ContextDebugger::Evaluate(DebuggerEvaluateRequest const& req)
	{
		pendingActions_.push([=]() {
			auto rc = this->EvaluateInContext(req);
			req.CompletionCallback(req, rc);
		});
		breakpointCv_.notify_one();
	}

	void ContextDebugger::GetVariables(DebuggerGetVariablesRequest const& req)
	{
		pendingActions_.push([=]() {
			auto rc = this->GetVariablesInContext(req);
			req.CompletionCallback(req, rc);
		});
		breakpointCv_.notify_one();
	}

	ExtensionStateBase& ContextDebugger::GetExtensionState()
	{
		if (context_ == DbgContext::CLIENT) {
			return gExtender->GetClientExtensionState();
		} else {
			return gExtender->GetServerExtensionState();
		}
	}

	void ContextDebugger::ExecuteQueuedActions()
	{
		std::function<void()> func;
		while (pendingActions_.try_pop(func)) {
			func();
		}
	}

	const std::regex validLocalNameRe("^[a-zA-Z_][a-zA-Z0-9_]*$");

	ResultCode ContextDebugger::EvaluateInContext(DebuggerEvaluateRequest const& req)
	{
		LuaVirtualPin lua(GetExtensionState());
		if (!lua) {
			return ResultCode::EvalEngineNotReady;
		}

		auto L = lua->GetState();
		StackCheck _(L);

		STDString syntaxCheck = "local x = " + req.Expression;
		if (luaL_loadstring(L, syntaxCheck.c_str())) {
			req.Response->set_error_message(lua_tostring(L, -1));
			lua_pop(L, 1);
			return ResultCode::EvalFailed;
		}

		lua_pop(L, 1);
		auto top = lua_gettop(L);

		STDString evalateLocals;
		std::vector<std::pair<int, STDString>> locals;
		if (req.Frame != -1) {
			lua_Debug ar;
			memset(&ar, 0, sizeof(ar));

			if (lua_getstack(L, req.Frame, &ar) == 0) {
				req.Response->set_error_message("Nonexistent stack frame");
				return ResultCode::EvalFailed;
			}

			LuaPushStackLocals(L, &ar, locals);

			evalateLocals = "local _EVAL_LOCALS_ = { ... }\r\n"
				"local Ext = Ext\r\n";
			
			for (auto const& local : locals) {
				if (std::regex_match(local.second, validLocalNameRe)) {
					evalateLocals += "local " + local.second + " = _EVAL_LOCALS_[" + std::to_string(local.first).c_str() + "]\r\n";
				} else {
					OsiWarn("Variable '" << local.second << "' not passed to evaluation context");
				}
			}
		}

		STDString evaluator = evalateLocals;
		evaluator += "local result = " + req.Expression + "\r\n"
			"return Ext.DebugEvaluate(result)";

		LifetimePin _p(lua->GetStack());
		if (luaL_loadstring(L, evaluator.c_str())) {
			req.Response->set_error_message(lua_tostring(L, -1));
			lua_pop(L, top - lua_gettop(L));
			return ResultCode::EvalFailed;
		}

		if (!locals.empty()) {
			lua_insert(L, top + 1);
		}

		if (bg3se::lua::CallWithTraceback(L, (int)locals.size(), LUA_MULTRET)) {
			req.Response->set_error_message(lua_tostring(L, -1));
			lua_pop(L, 1);
			return ResultCode::EvalFailed;
		} else {
			auto numReturnValues = lua_gettop(L) - top;
			auto result = req.Response->mutable_result();
			if (numReturnValues == 0) {
				result->set_type_id(MsgValueType::NONE);
			} else {
				LuaToProtobuf(L, -numReturnValues, result);

				if (numReturnValues >= 2) {
					auto index = (int32_t)lua_tointeger(L, -numReturnValues + 1);
					auto ref = result->mutable_variables();
					ref->set_frame(-1);
					ref->set_local(-1);
					ref->set_variableref(index);
				}
			}

			lua_pop(L, numReturnValues);
			return ResultCode::Success;
		}
	}

	bool ContextDebugger::PushVariableContext(lua_State* L, DebuggerGetVariablesRequest const& req)
	{
		if (req.Frame == -1 && req.Local == -1 && req.VariablesRef == -1) {
			lua_pushglobaltable(L);
			return true;
		} else if (req.Frame == -1 && req.Local == -1 && req.VariablesRef != -1) {
			lua_getglobal(L, "Ext"); // stack: Ext
			lua_getfield(L, -1, "_EVAL_ROOTS_"); // stack: Ext, roots
			lua_remove(L, -2); // stack: roots
			push(L, req.VariablesRef); // stack: roots, key
			lua_gettable(L, -2); // stack: roots, value
			lua_remove(L, -2); // stack: value
			if (lua_type(L, -1) != LUA_TNIL) {
				return true;
			} else {
				lua_pop(L, 1);
				req.Response->set_error_message("Variable reference invalid");
				return false;
			}
		} else if (req.Frame != -1 && req.Local != -1 && req.VariablesRef == -1) {
			lua_Debug ar;
			if (lua_getstack(L, req.Frame, &ar) == 0) {
				req.Response->set_error_message("Nonexistent stack frame");
				return false;
			}

			if (!LuaIsUserFunction(L, ar.i_ci)) {
				// Locals in C++ frames are unavailable
				req.Response->set_error_message("Cannot evaluate expressions in C++ frames");
				return false;
			}

			if (req.Local >= 0) {
				if (!lua_getlocal(L, &ar, req.Local)) {
					req.Response->set_error_message("Nonexistent local variable reference");
					return false;
				}
			} else {
				auto funcIdx = lua_gettop(L) + 1;
				setobjs2s(L, L->top, ar.i_ci->func);
				api_incr_top(L);

				if (!lua_getupvalue(L, -1, -req.Local - 1)) {
					req.Response->set_error_message("Nonexistent upvalue reference");
					return false;
				}

				lua_remove(L, funcIdx);
			}

			return true;
		} else {
			req.Response->set_error_message("Weird variable reference");
			return false;
		}
	}

	ResultCode ContextDebugger::GetVariablesInContext(DebuggerGetVariablesRequest const& req)
	{
		LuaVirtualPin lua(GetExtensionState());
		if (!lua) {
			return ResultCode::EvalEngineNotReady;
		}

		auto L = lua->GetState();
		StackCheck _(L);

		if (req.Frame != -1 && req.Local == -1) {
			if (!req.Key.empty()) {
				req.Response->set_error_message("Cannot get variables using keys in a stack frame request");
				return ResultCode::EvalFailed;
			}

			return GetVariablesInStackFrame(L, req);
		} else  {
			return GetVariablesInLocal(L, req);
		}
	}

	ResultCode ContextDebugger::GetVariablesInLocal(lua_State* L, DebuggerGetVariablesRequest const& req)
	{
		auto top = lua_gettop(L);
		try {
			if (!PushVariableContext(L, req)) {
				return ResultCode::EvalFailed;
			}

			for (auto const& key : req.Key) {
				if (key.Int) {
					push(L, *key.Int); // stack: table, key
				} else {
					push(L, *key.String); // stack: table, key
				}

				lua_gettable(L, -2); // stack: table, value
				lua_remove(L, -2); // stack: value
			}

			if (lua_type(L, -1) == LUA_TTABLE) {
				LuaTableToEvalResults(L, -1, req);
				lua_pop(L, 1);
				return ResultCode::Success;
			} else {
				req.Response->set_error_message("Not a table");
				lua_pop(L, 1);
				return ResultCode::EvalFailed;
			}

		} catch (Exception&) {
			auto stackRemaining = lua_gettop(L) - top;
			if (stackRemaining > 0) {
				req.Response->set_error_message(lua_tostring(L, -1));
				lua_pop(L, stackRemaining);
			} else {
				req.Response->set_error_message("Internal Lua error during evaluation");
			}

			return ResultCode::EvalFailed;
		}

		return ResultCode::Success;
	}

	ResultCode ContextDebugger::GetVariablesInStackFrame(lua_State* L, DebuggerGetVariablesRequest const& req)
	{
		lua_Debug ar;
		memset(&ar, 0, sizeof(ar));
		
		if (lua_getstack(L, req.Frame, &ar) == 0) {
			req.Response->set_error_message("Nonexistent stack frame");
			return ResultCode::EvalFailed;
		}

		if (!LuaIsUserFunction(L, ar.i_ci)) {
			// Locals in C++ frames are unavailable
			return ResultCode::Success;
		}

		StackFrameToEvalResults(L, &ar, req.Frame, req.Response);
		return ResultCode::Success;
	}

	Debugger::Debugger(DebugMessageHandler& messageHandler)
		: messageHandler_(messageHandler), 
		server_(messageHandler, DbgContext::SERVER),
		client_(messageHandler, DbgContext::CLIENT)
	{
		messageHandler_.SetDebugger(this);
		DEBUG("Debugger::Debugger(): Starting Lua debugger");
	}

	Debugger::~Debugger()
	{
		DEBUG("Debugger::~Debugger(): Shutting down Lua debugger");
		messageHandler_.SetDebugger(nullptr);
	}

	bool Debugger::IsDebuggerReady() const
	{
		return messageHandler_.IsDebuggerReady();
	}

	__declspec(thread) bool SendingLogMessage = false;

	void Debugger::OnLogMessage(DebugMessageType type, STDString const& message)
	{
		// Don't send debug messages by default to avoid log spam
		if (type != DebugMessageType::Debug && IsDebuggerReady() && !SendingLogMessage) {
			SendingLogMessage = true;
			messageHandler_.SendDebugOutput(type, message.c_str());
			SendingLogMessage = false;
		}
	}

	void Debugger::OnLuaHook(lua_State* L, lua_Debug* ar)
	{
		if (!IsDebuggerReady()) return;

		if (gExtender->IsInServerThread()) {
			server_.OnLuaHook(L, ar);
		} else {
			client_.OnLuaHook(L, ar);
		}
	}

	void Debugger::OnLuaError(lua_State* L, char const* msg)
	{
		if (!IsDebuggerReady()) return;

		if (gExtender->IsInServerThread()) {
			server_.OnLuaError(L, msg);
		} else {
			client_.OnLuaError(L, msg);
		}
	}

	void Debugger::OnGenericError(char const* msg)
	{
		if (!IsDebuggerReady()) return;

		if (gExtender->IsInServerThread()) {
			server_.OnGenericError(msg);
		} else {
			client_.OnGenericError(msg);
		}
	}

	void Debugger::DebugBreak(lua_State* L)
	{
		if (!IsDebuggerReady()) return;

		if (gExtender->IsInServerThread()) {
			server_.DebugBreak(L);
		} else {
			client_.DebugBreak(L);
		}
	}

	void Debugger::ServerStateCreated(esv::lua::ServerState* state)
	{
		server_.OnContextCreated(state->GetState());
		messageHandler_.SendLuaStateUpdate(true, true);
	}

	void Debugger::ClientStateCreated(ecl::lua::ClientState* state)
	{
		client_.OnContextCreated(state->GetState());
		messageHandler_.SendLuaStateUpdate(false, true);
	}

	void Debugger::ServerStateDeleted()
	{
		messageHandler_.SendLuaStateUpdate(true, false);
	}

	void Debugger::ClientStateDeleted()
	{
		messageHandler_.SendLuaStateUpdate(false, false);
	}

	void Debugger::EnableDebugging(bool enabled)
	{
		server_.EnableDebugging(enabled);
		client_.EnableDebugging(enabled);
	}

	void Debugger::BeginUpdatingBreakpoints()
	{
		server_.BeginUpdatingBreakpoints();
		client_.BeginUpdatingBreakpoints();
	}

	ResultCode Debugger::AddBreakpoint(STDString const& path, int line)
	{
		server_.AddBreakpoint(path, line);
		client_.AddBreakpoint(path, line);
		return ResultCode::Success;
	}

	void Debugger::FinishUpdatingBreakpoints()
	{
		server_.FinishUpdatingBreakpoints();
		client_.FinishUpdatingBreakpoints();
	}

	void Debugger::UpdateSettings(bool breakOnError, bool breakOnGenericError)
	{
		server_.UpdateSettings(breakOnError, breakOnGenericError);
		client_.UpdateSettings(breakOnError, breakOnGenericError);
	}

	ResultCode Debugger::ContinueExecution(DbgContext ctx, DbgContinue_Action action)
	{
		if (ctx == DbgContext::SERVER) {
			return server_.ContinueExecution(action);
		} else {
			return client_.ContinueExecution(action);
		}
	}

	void Debugger::Evaluate(DebuggerEvaluateRequest const& req)
	{
		if (req.Context == DbgContext::SERVER) {
			return server_.Evaluate(req);
		} else {
			return client_.Evaluate(req);
		}
	}

	void Debugger::GetVariables(DebuggerGetVariablesRequest const& req)
	{
		if (req.Context == DbgContext::SERVER) {
			return server_.GetVariables(req);
		}
		else {
			return client_.GetVariables(req);
		}
	}
}

#endif
