#pragma once

#if !defined(OSI_NO_DEBUGGER)

#include <cstdint>
#include <concurrent_queue.h>
#include "LuaDebug.pb.h"
#include <GameDefinitions/Osiris.h>
#include <Lua/LuaDebugMessages.h>
#include "OsirisHelpers.h"

struct lua_Debug;

namespace dse
{
	class ExtensionStateBase;
}

namespace dse::esv::lua
{
	class ServerState;
}

namespace dse::ecl::lua
{
	class ClientState;
}

namespace dse::lua::dbg
{
	struct DebuggerEvaluateRequest
	{
		uint32_t Seq;
		DbgContext Context;
		BackendToDebugger* Msg;
		BkEvaluateResponse* Response;
		int32_t Frame;
		STDString Expression;
		std::function<void(DebuggerEvaluateRequest const&, ResultCode)> CompletionCallback;
	};


	struct DebuggerGetVariablesRequest
	{
		struct KeyType
		{
			std::optional<STDString> String;
			std::optional<int64_t> Int;
		};

		uint32_t Seq;
		DbgContext Context;
		BackendToDebugger* Msg;
		int VariablesRef;
		int Frame;
		int Local;
		std::vector<KeyType> Key;
		BkGetVariablesResponse* Response;
		std::function<void(DebuggerGetVariablesRequest const&, ResultCode)> CompletionCallback;
	};


	class ContextDebugger
	{
	public:
		ContextDebugger(DebugMessageHandler& messageHandler, DbgContext ctx);

		void OnContextCreated(lua_State* L);
		void OnLuaHook(lua_State* L, lua_Debug* ar);
		void OnLuaError(lua_State* L, char const* msg);
		void OnGenericError(char const* msg);
		void EnableDebugging(bool enabled);
		void DebugBreak(lua_State* L);

		inline bool IsPaused() const
		{
			return isPaused_;
		}

		void BeginUpdatingBreakpoints();
		void AddBreakpoint(STDString const& path, int line);
		void FinishUpdatingBreakpoints();

		void UpdateSettings(bool breakOnError, bool breakOnGenericError);
		ResultCode ContinueExecution(DbgContinue_Action action);
		void Evaluate(DebuggerEvaluateRequest const& req);
		void GetVariables(DebuggerGetVariablesRequest const& req);

	private:
		struct BreakpointSet
		{
			// Currently active breakpoints
			std::unordered_map<STDString, std::unordered_set<int>> breakpoints;
			// Line-only map to avoid string-based file lookup if a specific line has no breakpoints globally
			std::unordered_set<int> lines;
		};

		DebugMessageHandler& messageHandler_;
		DbgContext context_;
		bool enabled_{ false };

		// Actions that we'll perform in the server/client thread instead of the messaging runtime thread.
		// This is needed to make sure that certain operations (eg. expression evaluation) execute in a thread-safe way.
		Concurrency::concurrent_queue<std::function<void()>> pendingActions_;

		std::mutex breakpointMutex_;
		std::condition_variable breakpointCv_;
		// Is the Lua context currently held in pause?
		// The server/client thread will recheck this value when the condition variable is notified,
		// and resume execution if the pause flag is no longer set.
		bool isPaused_{ false };
		// Requests the Lua thread to halt execution
		bool requestPause_{ false };
		int pauseMaxStackDepth_{ -1 };
		int currentStackDepth_{ 0 };
		// Trigger breakpoint on Lua errors
		bool breakOnError_{ false };
		// Trigger breakpoint on non-Lua error message if we're running Lua code
		bool breakOnGenericError_{ false };

		// Breakpoint set currently in use by the debugger
		std::unique_ptr<BreakpointSet> breakpoints_;
		// Breakpoint set being updated through DAP
		std::unique_ptr<BreakpointSet> newBreakpoints_;

		ExtensionStateBase& GetExtensionState();
		void ExecuteQueuedActions();
		bool IsBreakpoint(lua_State* L, lua_Debug* ar, BkBreakpointTriggered::Reason& reason);
		void TriggerBreakpoint(lua_State* L, BkBreakpointTriggered_Reason reason, char const* msg);

		ResultCode EvaluateInContext(DebuggerEvaluateRequest const& req);
		bool PushVariableContext(lua_State* L, DebuggerGetVariablesRequest const& req);
		ResultCode GetVariablesInLocal(lua_State* L, DebuggerGetVariablesRequest const& req);
		ResultCode GetVariablesInStackFrame(lua_State* L, DebuggerGetVariablesRequest const& req);

		ResultCode GetVariablesInContext(DebuggerGetVariablesRequest const& req);
	};

	class Debugger
	{
	public:
		Debugger(DebugMessageHandler& messageHandler);
		~Debugger();

		bool IsDebuggerReady() const;
		void OnLogMessage(DebugMessageType type, STDString const& message);
		void OnLuaHook(lua_State* L, lua_Debug* ar);
		void OnLuaError(lua_State* L, char const* msg);
		void OnGenericError(char const* msg);
		void DebugBreak(lua_State* L);

		void ServerStateCreated(esv::lua::ServerState* state);
		void ClientStateCreated(ecl::lua::ClientState* state);
		void ServerStateDeleted();
		void ClientStateDeleted();
		void EnableDebugging(bool enabled);

		void BeginUpdatingBreakpoints();
		ResultCode AddBreakpoint(STDString const& path, int line);
		void FinishUpdatingBreakpoints();

		void UpdateSettings(bool breakOnError, bool breakOnGenericError);
		ResultCode ContinueExecution(DbgContext ctx, DbgContinue_Action action);
		void Evaluate(DebuggerEvaluateRequest const& req);
		void GetVariables(DebuggerGetVariablesRequest const& req);

	private:
		DebugMessageHandler& messageHandler_;
		ContextDebugger server_;
		ContextDebugger client_;
	};
}

#endif
