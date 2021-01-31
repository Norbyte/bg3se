#include "stdafx.h"
#include "DebugMessages.h"
#include "NodeHooks.h"
#include "OsirisProxy.h"
#include <sstream>

#if !defined(OSI_NO_DEBUGGER)

namespace dse::osidbg
{
	DebugMessageHandler::DebugMessageHandler(OsirisDebugInterface& intf)
		: intf_(intf)
	{
		intf_.SetConnectHandler(
			std::bind(&DebugMessageHandler::HandleConnect, this),
			std::bind(&DebugMessageHandler::HandleDisconnect, this));
		intf_.SetMessageHandler(std::bind(&DebugMessageHandler::HandleMessage, this, std::placeholders::_1));
	}

	void MakeMsgColumn(MsgTypedValue & msgTv, TypedValue const & tv)
	{
		msgTv.set_type_id(tv.TypeId);
		auto const & val = tv.Value.Val;

		switch ((ValueType)tv.TypeId) {
		case ValueType::None: break;
		case ValueType::Undefined: *msgTv.mutable_stringval() = "(Undefined)"; break;
		case ValueType::Integer: msgTv.set_intval(val.Int32); break;
		case ValueType::Integer64: msgTv.set_intval(val.Int64); break;
		case ValueType::Real: msgTv.set_floatval(val.Float); break;
		default: *msgTv.mutable_stringval() = val.String; break;
		}
	}

	void MakeMsgColumn(MsgTypedValue & msgTv, OsiArgumentValue const & arg)
	{
		msgTv.set_type_id((uint32_t)arg.TypeId);

		switch (arg.TypeId) {
		case ValueType::None: break;
		case ValueType::Undefined: *msgTv.mutable_stringval() = "(Undefined)"; break;
		case ValueType::Integer: msgTv.set_intval(arg.Int32); break;
		case ValueType::Integer64: msgTv.set_intval(arg.Int64); break;
		case ValueType::Real: msgTv.set_floatval(arg.Float); break;
		default: *msgTv.mutable_stringval() = arg.String; break;
		}
	}

	void MakeMsgTuple(MsgTuple & msgTuple, TupleLL const & tuple)
	{
		auto head = tuple.Items.Head;
		auto col = head->Next;
		while (col != head) {
			auto column = msgTuple.add_column();
			MakeMsgColumn(*column, col->Item.Value);
			col = col->Next;
		}
	}

	void MakeMsgTuple(MsgTuple & msgTuple, TuplePtrLL const & tuple)
	{
		auto const & items = tuple.Items;
		auto head = items.Head;
		auto col = head->Next;
		while (col != head) {
			auto column = msgTuple.add_column();
			MakeMsgColumn(*column, *col->Item);
			col = col->Next;
		}
	}

	void MakeMsgTuple(MsgTuple & msgTuple, TupleVec const & tuple)
	{
		for (unsigned i = 0; i < tuple.Size; i++) {
			auto column = msgTuple.add_column();
			MakeMsgColumn(*column, tuple.Values[i]);
		}
	}

	void MakeMsgTuple(MsgTuple & msgTuple, std::vector<OsiArgumentValue> const & tuple)
	{
		for (auto const & val : tuple) {
			auto column = msgTuple.add_column();
			MakeMsgColumn(*column, val);
		}
	}

	void DebugDumpTV(std::wstringstream & ss, TypedValue const & tv)
	{
		switch ((ValueType)tv.TypeId) {
		case ValueType::Integer:
			ss << tv.Value.Val.Int32;
			break;

		case ValueType::Integer64:
			ss << tv.Value.Val.Int64;
			break;

		case ValueType::Real:
			ss << tv.Value.Val.Float;
			break;

		case ValueType::None:
		case ValueType::Undefined:
			ss << "(None)";
			break;

		default:
			if (tv.Value.Val.String) {
				ss << '"' << tv.Value.Val.String << '"';
			}
			else {
				ss << "(Null)";
			}
			break;
		}
	}

	void DebugDumpTuple(std::wstringstream & ss, TupleLL const & tuple)
	{
		auto head = tuple.Items.Head;
		auto cur = head->Next;
		while (cur != head) {
			DebugDumpTV(ss, cur->Item.Value);
			ss << ", ";
			cur = cur->Next;
		}
	}

	void DebugDumpTuple(std::wstringstream & ss, TuplePtrLL const & tuple)
	{
		auto const & items = tuple.Items;
		auto head = items.Head;
		auto cur = head->Next;
		while (cur != head) {
			DebugDumpTV(ss, *cur->Item);
			ss << ", ";
			cur = cur->Next;
		}
	}

	void MakeMsgCallStack(BkBreakpointTriggered & msg, std::vector<CallStackFrame> const & callStack)
	{
		for (auto const & frame : callStack) {
			auto msgFrame = msg.add_call_stack();
			msgFrame->set_type((MsgFrame_FrameType)frame.frameType);
			msgFrame->set_action_index((MsgFrame_FrameType)frame.actionIndex);

			if (frame.node != nullptr) {
				msgFrame->set_node_id(frame.node->Id);
			}

			if (frame.goal != nullptr) {
				msgFrame->set_goal_id(frame.goal->Id);
			}

			auto tuple = msgFrame->mutable_tuple();
			if (frame.tupleLL != nullptr) {
				MakeMsgTuple(*tuple, *frame.tupleLL);
			} else if (frame.tuplePtrLL != nullptr) {
				MakeMsgTuple(*tuple, *frame.tuplePtrLL);
			}
		}
	}

	void DebugMessageHandler::SendBreakpointTriggered(std::vector<CallStackFrame> const & callStack,
		QueryResultInfo const * results)
	{
		auto const & lastFrame = *callStack.rbegin();
		BackendToDebugger msg;
		auto trigger = msg.mutable_breakpointtriggered();
		MakeMsgCallStack(*trigger, callStack);

		if (results != nullptr) {
			if (results->succeeded) {
				trigger->set_query_succeeded(BkBreakpointTriggered::SUCCEEDED);
			} else {
				trigger->set_query_succeeded(BkBreakpointTriggered::FAILED);
			}
			
			auto tuple = trigger->mutable_query_results();
			MakeMsgTuple(*tuple, results->results);

			trigger->set_query_node_id(results->queryNodeId);
		} else {
			trigger->set_query_succeeded(BkBreakpointTriggered::NOT_A_QUERY);
		}

		Send(msg);

		std::wstringstream tup;
		if (lastFrame.tupleLL != nullptr) {
			DebugDumpTuple(tup, *lastFrame.tupleLL);
		} else if (lastFrame.tuplePtrLL != nullptr) {
			DebugDumpTuple(tup, *lastFrame.tuplePtrLL);
		}
		DEBUG(L" <-- BkBreakpointTriggered(type %d, node %d/goal %d, tuple (%s))",
			lastFrame.frameType, 
			lastFrame.node ? lastFrame.node->Id : 0,
			lastFrame.goal ? lastFrame.goal->Id : 0,
			tup.str().c_str());
	}

	void DebugMessageHandler::SendGlobalBreakpointTriggered(GlobalBreakpointReason reason)
	{
		BackendToDebugger msg;
		auto trigger = msg.mutable_globalbreakpointtriggered();
		trigger->set_reason((BkGlobalBreakpointTriggered_Reason)reason);
		Send(msg);

		DEBUG(" <-- BkGlobalBreakpointTriggered(%d)", reason);
	}

	void DebugMessageHandler::SendStoryLoaded()
	{
		BackendToDebugger msg;
		auto storyLoaded = msg.mutable_storyloaded();
		Send(msg);
		DEBUG(" <-- BkStoryLoaded()");
	}

	void DebugMessageHandler::SendDebugSessionEnded()
	{
		BackendToDebugger msg;
		auto sessionEnded = msg.mutable_debugsessionended();
		Send(msg);
		DEBUG(" <-- BkDebugSessionEnded()");
	}

	void AddActionInfo(RuleActionList * actions, std::function<MsgActionInfo * ()> addAction)
	{
		auto head = actions->Actions.Head;
		auto current = head->Next;
		uint32_t actionIndex = 0;
		while (current != head) {
			auto actionInfo = addAction();
			if (current->Item->FunctionName != nullptr) {
				actionInfo->set_function(current->Item->FunctionName);
				if (current->Item->Arguments != nullptr) {
					actionInfo->set_arity((uint32_t)current->Item->Arguments->Args.Size);
				} else {
					actionInfo->set_arity(0);
				}
			} else {
				actionInfo->set_goal_id(current->Item->GoalIdOrDebugHook);
			}
			current = current->Next;
			actionIndex++;
		}
	}

	void DebugMessageHandler::SendSyncStory(Goal * goal)
	{
		BackendToDebugger msg;
		auto sync = msg.mutable_syncstorydata();
		auto goalInfo = sync->add_goal();
		goalInfo->set_id(goal->Id);
		goalInfo->set_name(goal->Name);
		AddActionInfo(goal->InitCalls, [goalInfo]() -> MsgActionInfo * { return goalInfo->add_initactions(); });
		AddActionInfo(goal->ExitCalls, [goalInfo]() -> MsgActionInfo * { return goalInfo->add_exitactions(); });
		Send(msg);
		DEBUG(" <-- BkSyncStoryData(Goal #%d)", goal->Id);
	}

	void DebugMessageHandler::SendSyncStory(Database ** databases, uint32_t count)
	{
		BackendToDebugger msg;
		auto sync = msg.mutable_syncstorydata();
		for (uint32_t i = 0; i < count; i++) {
			auto * db = databases[i];
			auto dbInfo = sync->add_database();
			dbInfo->set_id(db->DatabaseId);
			auto numParams = db->NumParams;
			auto const & paramTypes = db->ParamTypes;
			for (auto arg = 0; arg < numParams; arg++) {
				dbInfo->add_argumenttype(paramTypes[arg]);
			}
		}

		Send(msg);
		DEBUG(" <-- BkSyncStoryData(%d databases)", count);
	}

	void DebugMessageHandler::SendSyncStory(Node ** nodes, uint32_t count)
	{
		BackendToDebugger msg;
		auto sync = msg.mutable_syncstorydata();
		for (uint32_t i = 0; i < count; i++) {
			auto * node = nodes[i];
			auto nodeInfo = sync->add_node();
			nodeInfo->set_id(node->Id);
			auto type = gNodeVMTWrappers->GetType(node);
			nodeInfo->set_type((uint32_t)type);
			if (node->Function != nullptr) {
				nodeInfo->set_name(node->Function->Signature->Name);
			}
			
			if (type == NodeType::Rule) {
				auto ruleInfo = sync->add_rule();
				ruleInfo->set_node_id(node->Id);
				RuleNode * rule = static_cast<RuleNode *>(node);
				AddActionInfo(rule->Calls, [ruleInfo]() -> MsgActionInfo * { return ruleInfo->add_actions(); });
			}
		}

		Send(msg);
		DEBUG(" <-- BkSyncStoryData(%d nodes)", count);
	}

	void DebugMessageHandler::SendSyncStoryFinished()
	{
		BackendToDebugger msg;
		auto syncFinished = msg.mutable_syncstoryfinished();
		Send(msg);
		DEBUG(" <-- BkSyncStoryFinished()");
	}

	void DebugMessageHandler::SendDebugOutput(char const * message)
	{
		BackendToDebugger msg;
		auto debugMsg = msg.mutable_debugoutput();
		debugMsg->set_message(message);
		Send(msg);

		if (IsConnected()) {
			DEBUG(" <-- BkDebugOutput(): \"%s\"", message);
		}
	}

	void DebugMessageHandler::SetDebugger(Debugger * debugger)
	{
		debugger_ = debugger;
	}

	void DebugMessageHandler::HandleIdentify(uint32_t seq, DbgIdentifyRequest const & req)
	{
		DEBUG(" --> DbgIdentifyRequest(Version %d)", req.protocol_version());
		SendVersionInfo(seq);

		if (req.protocol_version() != ProtocolVersion) {
			WARN("DebugMessageHandler::HandleIdentify(): Client sent unsupported protocol version; got %d, we only support %d", 
				req.protocol_version(), ProtocolVersion);
			intf_.Disconnect();
		}
	}

	void DebugMessageHandler::HandleSetGlobalBreakpoints(uint32_t seq, DbgSetGlobalBreakpoints const & req)
	{
		DEBUG(" --> DbgSetGlobalBreakpoints(%d)", req.breakpoint_mask());

		ResultCode rc;
		if (!debugger_) {
			WARN("SetGlobalBreakpoints: Not attached to story debugger!");
			rc = ResultCode::NoDebuggee;
		}
		else
		{
			rc = debugger_->Breakpoints().SetGlobalBreakpoints((GlobalBreakpointType)req.breakpoint_mask());
		}

		SendResult(seq, rc);
	}

	void DebugMessageHandler::HandleSetBreakpoints(uint32_t seq, DbgSetBreakpoints const & req)
	{
		DEBUG(" --> DbgSetBreakpoints()");

		ResultCode rc;
		if (!debugger_) {
			WARN("SetBreakpoint: Not attached to story debugger!");
			rc = ResultCode::NoDebuggee;
		}
		else
		{
			rc = ResultCode::Success;

			debugger_->Breakpoints().BeginUpdatingNodeBreakpoints();
			for (auto const & bp : req.breakpoint()) {
				DEBUG("AddBreakpoint(node %d, goal %d, action %d, flags %d)",
					bp.node_id(), bp.goal_id(), bp.action_index(), bp.breakpoint_mask());
				rc = debugger_->Breakpoints().AddBreakpoint(
					bp.node_id(),
					bp.goal_id(), 
					bp.is_init_action(),
					bp.action_index(),
					(BreakpointType)bp.breakpoint_mask()
				);

				if (rc != ResultCode::Success) {
					break;
				}
			}

			debugger_->FinishUpdatingNodeBreakpoints();
		}

		SendResult(seq, rc);
	}

	void DebugMessageHandler::HandleGetDatabaseContents(uint32_t seq, DbgGetDatabaseContents const & req)
	{
		DEBUG(" --> DbgGetDatabaseContents(%d)", req.database_id());

		ResultCode rc;
		if (!debugger_) {
			WARN("GetDatabaseContents: Not attached to story debugger!");
			rc = ResultCode::NoDebuggee;
		}
		else
		{
			rc = debugger_->GetDatabaseContents(req.database_id());
		}

		SendResult(seq, rc);
	}

	void DebugMessageHandler::HandleContinue(uint32_t seq, DbgContinue const & req)
	{
		DEBUG(" --> DbgContinue()");

		ResultCode rc;
		if (!debugger_) {
			WARN("Continue: Not attached to story debugger!");
			rc = ResultCode::NoDebuggee;
		}
		else
		{
			rc = debugger_->ContinueExecution(req.action(), req.breakpoint_mask(), req.flags());
		}

		SendResult(seq, rc);
	}

	void DebugMessageHandler::HandleSyncStory(uint32_t seq, DbgSyncStory const & req)
	{
		DEBUG(" --> DbgSyncStory()");

		if (debugger_) {
			debugger_->SyncStory();
		} else {
			WARN("SyncStory: Not attached to story debugger!");
		}

		SendSyncStoryFinished();
	}

	void DebugMessageHandler::HandleEvaluate(uint32_t seq, DbgEvaluate const & req)
	{
		DEBUG(" --> DbgEvaluate(%d, %d)", req.type(), req.node_id());

		if (debugger_) {
			bool querySucceeded = false;
			auto process = [this, seq] (ResultCode rc, bool querySucceeded) {
				SendEvaluateFinished(seq, rc, querySucceeded);
			};

			debugger_->Evaluate(seq, (EvalType)req.type(), req.node_id(), req.params(), process);
		} else {
			WARN("Evaluate: Not attached to story debugger!");
		}
	}

	bool DebugMessageHandler::HandleMessage(DebuggerToBackend const * msg)
	{
		uint32_t seq = msg->seq_no();
		if (seq != inboundSeq_) {
			ERR("Incorrect sequence number in message; expected %d, got %d", inboundSeq_, seq);
			return false;
		}

		inboundSeq_++;

		switch (msg->msg_case()) {
		case DebuggerToBackend::kIdentify:
			HandleIdentify(seq, msg->identify());
			break;

		case DebuggerToBackend::kSetGlobalBreakpoints:
			HandleSetGlobalBreakpoints(seq, msg->setglobalbreakpoints());
			break;

		case DebuggerToBackend::kSetBreakpoints:
			HandleSetBreakpoints(seq, msg->setbreakpoints());
			break;

		case DebuggerToBackend::kContinue:
			HandleContinue(seq, msg->continue_());
			break;

		case DebuggerToBackend::kGetDatabaseContents:
			HandleGetDatabaseContents(seq, msg->getdatabasecontents());
			break;

		case DebuggerToBackend::kSyncStory:
			HandleSyncStory(seq, msg->syncstory());
			break;

		case DebuggerToBackend::kEvaluate:
			HandleEvaluate(seq, msg->evaluate());
			break;

		default:
			ERR("Unknown message type received: %d", msg->msg_case());
			return false;
		}

		return true;
	}

	void DebugMessageHandler::HandleConnect()
	{
		INFO("Connected to debugger frontend");

		outboundSeq_ = 1;
		inboundSeq_ = 1;
	}

	void DebugMessageHandler::HandleDisconnect()
	{
		INFO("Disconnected from debugger frontend");
		if (debugger_) {
			debugger_->Breakpoints().ClearAllBreakpoints();
			if (debugger_->IsPaused()) {
				debugger_->ContinueExecution(DbgContinue_Action_CONTINUE, 0, 0);
			}
		}
	}

	void DebugMessageHandler::Send(BackendToDebugger & msg)
	{
		if (intf_.IsConnected()) {
			msg.set_seq_no(outboundSeq_++);
			intf_.Send(msg);
		}
	}

	void DebugMessageHandler::SendResult(uint32_t seq, ResultCode code)
	{
		BackendToDebugger msg;
		msg.set_reply_seq_no(seq);
		auto results = msg.mutable_results();
		results->set_status_code((StatusCode)code);
		Send(msg);
		DEBUG(" <-- BkResult(%d)", code);
	}

	void DebugMessageHandler::SendVersionInfo(uint32_t seq)
	{
		BackendToDebugger msg;
		auto version = msg.mutable_versioninfo();
		version->set_protocol_version(ProtocolVersion);
		if (debugger_ != nullptr)
		{
			version->set_story_loaded(true);
			version->set_story_initialized(debugger_->IsInitialized());
		}
		else
		{
			version->set_story_loaded(false);
			version->set_story_initialized(false);
		}

		Send(msg);
		DEBUG(" <-- BkVersionInfoResponse()");
	}

	void DebugMessageHandler::SendBeginDatabaseContents(uint32_t databaseId)
	{
		BackendToDebugger msg;
		auto beginMsg = msg.mutable_begindatabasecontents();
		beginMsg->set_database_id(databaseId);
		Send(msg);
		DEBUG(" <-- BkBeginDatabaseContents()");
	}

	void DebugMessageHandler::SendDatabaseRow(uint32_t databaseId, TupleVec * row)
	{
		BackendToDebugger msg;
		auto rowMsg = msg.mutable_databaserow();
		rowMsg->set_database_id(databaseId);

		auto msgRow = rowMsg->add_row();
		MakeMsgTuple(*msgRow, *row);

		Send(msg);
		DEBUG(" <-- BkDatabaseRow()");
	}

	void DebugMessageHandler::SendEndDatabaseContents(uint32_t databaseId)
	{
		BackendToDebugger msg;
		auto endMsg = msg.mutable_enddatabasecontents();
		endMsg->set_database_id(databaseId);
		Send(msg);
		DEBUG(" <-- BkEndDatabaseContents()");
	}

	void DebugMessageHandler::SendEvaluateRow(uint32_t seq, VirtTupleLL & row)
	{
		BackendToDebugger msg;
		msg.set_reply_seq_no(seq);
		auto rowMsg = msg.mutable_evaluaterow();
		auto msgRow = rowMsg->add_row();
		MakeMsgTuple(*msgRow, row.Data);

		Send(msg);
		DEBUG(" <-- BkEvaluateRow()");
	}

	void DebugMessageHandler::SendEvaluateFinished(uint32_t seq, ResultCode rc, bool querySucceeded)
	{
		BackendToDebugger msg;
		msg.set_reply_seq_no(seq);
		auto endMsg = msg.mutable_evaluatefinished();
		endMsg->set_result_code((StatusCode)rc);
		endMsg->set_query_succeeded(querySucceeded);
		Send(msg);
		DEBUG(" <-- BkEvaluateFinished()");
	}
}

#endif
