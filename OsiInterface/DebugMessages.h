#pragma once

#if !defined(OSI_NO_DEBUGGER)

#include <cstdint>
#include "osidebug.pb.h"
#include <GameDefinitions/Osiris.h>
#include "DebugInterface.h"

namespace dse::osidbg
{
	enum class GlobalBreakpointReason
	{
		StoryLoaded = 0,
		GameInit = 1,
		GameExit = 2
	};

	enum class BreakpointReason
	{
		NodeIsValid = 0,
		NodePushDownTuple = 1,
		NodeInsertTuple = 2,
		NodeDeleteTuple = 3,
		NodePushDownTupleDelete = 4,
		RuleActionCall = 5,
		GoalInitCall = 6,
		GoalExitCall = 7
	};

	enum class ResultCode
	{
		Success = 0,
		UnsupportedBreakpointType = 1,
		InvalidNodeId = 2,
		NotInPause = 3,
		NoDebuggee = 4,
		InvalidContinueAction = 5,
		InPause = 6,
		InvalidGoalId = 7,
		UnsupportedContinueFlags = 8,
		InvalidDatabaseId = 9,
		NotCallable = 10,
		InvalidParameters = 11,
		NoAdapter = 12,
		InvalidEvalType = 13,
		EvalEngineNotReady = 14,
		InvalidParamTupleArity = 15,
		InvalidParamType = 16,
		MissingRequiredParam = 17
	};

	enum class EvalType
	{
		IsValid = 0,
		Pushdown = 1,
		Insert = 2,
		Delete = 3
	};

	struct CallStackFrame
	{
		BreakpointReason frameType;
		Node * node;
		Goal * goal;
		uint32_t actionIndex;
		TupleLL * tupleLL;
		TuplePtrLL * tuplePtrLL;
	};

	struct QueryResultInfo
	{
		// Node ID of last query
		uint32_t queryNodeId;
		// Did the last query succeed?
		bool succeeded;
		// Results of last div query
		std::vector<OsiArgumentValue> results;
	};

	class Debugger;

	class DebugMessageHandler
	{
	public:
		static const uint32_t ProtocolVersion = 8;

		DebugMessageHandler(OsirisDebugInterface& intf);

		inline bool IsConnected() const
		{
			return intf_.IsConnected();
		}

		void SetDebugger(Debugger * debugger);
		void SendBreakpointTriggered(std::vector<CallStackFrame> const & callStack,
			QueryResultInfo const * results = nullptr);
		void SendGlobalBreakpointTriggered(GlobalBreakpointReason reason);
		void SendStoryLoaded();
		void SendDebugSessionEnded();
		void SendSyncStory(Goal * goal);
		void SendSyncStory(Database ** databases, uint32_t count);
		void SendSyncStory(Node ** nodes, uint32_t count);
		void SendSyncStoryFinished();
		void SendDebugOutput(char const * message);
		void SendBeginDatabaseContents(uint32_t databaseId);
		void SendDatabaseRow(uint32_t databaseId, TupleVec * row);
		void SendEndDatabaseContents(uint32_t databaseId);
		void SendEvaluateRow(uint32_t seq, VirtTupleLL & row);
		void SendEvaluateFinished(uint32_t seq, ResultCode rc, bool querySucceeded);

	private:
		OsirisDebugInterface& intf_;
		Debugger * debugger_{ nullptr };
		uint32_t inboundSeq_{ 1 };
		uint32_t outboundSeq_{ 1 };

		bool HandleMessage(DebuggerToBackend const * msg);
		void HandleConnect();
		void HandleDisconnect();

		void HandleIdentify(uint32_t seq, DbgIdentifyRequest const & req);
		void HandleSetGlobalBreakpoints(uint32_t seq, DbgSetGlobalBreakpoints const & req);
		void HandleSetBreakpoints(uint32_t seq, DbgSetBreakpoints const & req);
		void HandleContinue(uint32_t seq, DbgContinue const & req);
		void HandleGetDatabaseContents(uint32_t seq, DbgGetDatabaseContents const & req);
		void HandleSyncStory(uint32_t seq, DbgSyncStory const & req);
		void HandleEvaluate(uint32_t seq, DbgEvaluate const & req);

		void Send(BackendToDebugger & msg);
		void SendVersionInfo(uint32_t seq);
		void SendResult(uint32_t seq, ResultCode code);
	};
}

#endif
