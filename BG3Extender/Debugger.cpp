#include "stdafx.h"
#include "Debugger.h"
#include "NodeHooks.h"
#include "OsirisProxy.h"
#include <sstream>

#if !defined(OSI_NO_DEBUGGER)
#undef DUMP_TRACEPOINTS

namespace dse::osidbg
{
	RuleActionMap::RuleActionMap(OsirisStaticGlobals const & globals)
		: globals_(globals)
	{}

	void RuleActionMap::AddRuleActionMappings(Node * node, Goal * goal, bool isInit, RuleActionList * actions)
	{
		auto head = actions->Actions.Head;
		auto current = head->Next;
		uint32_t actionIndex = 0;
		while (current != head) {
			ruleActionMappings_.insert(std::pair<RuleActionNode *, RuleActionMapping>(
				current->Item,
				{ current->Item, node, goal, isInit, actionIndex }
			));
			current = current->Next;
			actionIndex++;
		}
	}

	void RuleActionMap::UpdateRuleActionMappings()
	{
		ruleActionMappings_.clear();

		auto const & nodeDb = (*globals_.Nodes)->Db;
		for (unsigned i = 0; i < nodeDb.Size; i++) {
			auto node = nodeDb.Start[i];
			NodeType type = gNodeVMTWrappers->GetType(node);
			if (type == NodeType::Rule) {
				auto rule = static_cast<RuleNode *>(node);
				AddRuleActionMappings(rule, nullptr, false, rule->Calls);
			}
		}

		auto const & goalDb = (*globals_.Goals);
		for (unsigned i = 0; i < goalDb->Count; i++) {
			auto goal = goalDb->Goals.Find(i + 1);
			AddRuleActionMappings(nullptr, *goal, true, (*goal)->InitCalls);
			AddRuleActionMappings(nullptr, *goal, false, (*goal)->ExitCalls);
		}
	}

	RuleActionMapping const * RuleActionMap::FindActionMapping(RuleActionNode * action)
	{
		auto mapping = ruleActionMappings_.find(action);
		if (mapping == ruleActionMappings_.end()) {
			WARN("Debugger::FindActionMapping(%016x): Could not find action mapping for rule action", action);
			return nullptr;
		}

		return &mapping->second;
	}



	BreakpointManager::BreakpointManager(OsirisStaticGlobals const & globals)
		: breakpoints_(new std::unordered_map<uint64_t, Breakpoint>()),
		globals_(globals)
	{}

	ResultCode BreakpointManager::SetGlobalBreakpoints(GlobalBreakpointType breakpoints)
	{
		if (breakpoints & ~GlobalBreakpointTypeAll) {
			WARN("Debugger::SetGlobalBreakpoints(): Unsupported breakpoint type set: %08x", breakpoints);
			return ResultCode::UnsupportedBreakpointType;
		}

		DEBUG("Debugger::SetGlobalBreakpoints(): Set to %08x", breakpoints);
		globalBreakpoints_ = breakpoints;
		return ResultCode::Success;
	}

	void BreakpointManager::BeginUpdatingNodeBreakpoints()
	{
		DEBUG("Debugger::BeginUpdatingNodeBreakpoints()");
		pendingBreakpoints_.reset(new std::unordered_map<uint64_t, Breakpoint>());
	}

	ResultCode BreakpointManager::AddBreakpoint(uint32_t nodeId, uint32_t goalId, bool isInit, int32_t actionIndex, BreakpointType type)
	{
		if (type & ~BreakpointTypeAll) {
			WARN("Debugger::AddBreakpoint(): Unsupported breakpoint type set: %08x", type);
			return ResultCode::UnsupportedBreakpointType;
		}

		if (nodeId > (*globals_.Nodes)->Db.Size) {
			WARN("Debugger::AddBreakpoint(): Tried to set on nonexistent node ID %d", nodeId);
			return ResultCode::InvalidNodeId;
		}

		if (goalId > (*globals_.Goals)->Count) {
			WARN("Debugger::AddBreakpoint(): Tried to set on nonexistent goal ID %d", nodeId);
			return ResultCode::InvalidGoalId;
		}

		uint64_t breakpointId;
		if (actionIndex == -1) {
			if (nodeId == 0) {
				WARN("Debugger::AddBreakpoint(): Node ID must be nonzero for node actions", nodeId);
				return ResultCode::InvalidNodeId;
			}

			breakpointId = MakeNodeBreakpointId(nodeId);
		}
		else {
			if (nodeId != 0) {
				breakpointId = MakeRuleActionBreakpointId(nodeId, actionIndex);
			}
			else if (goalId != 0) {
				if (isInit) {
					breakpointId = MakeGoalInitBreakpointId(goalId, actionIndex);
				}
				else {
					breakpointId = MakeGoalExitBreakpointId(goalId, actionIndex);
				}
			}
			else {
				WARN("Debugger::AddBreakpoint(): No node/goal specified");
				return ResultCode::InvalidNodeId;
			}
		}

		DEBUG("Debugger::AddBreakpoint(): Set on key %016x to %08x", breakpointId, type);
		Breakpoint bp;
		bp.nodeId = nodeId;
		bp.goalId = goalId;
		bp.isInit = isInit;
		bp.actionIndex = actionIndex;
		bp.type = type;
		(*pendingBreakpoints_)[breakpointId] = bp;

		return ResultCode::Success;
	}

	void BreakpointManager::FinishUpdatingNodeBreakpoints()
	{
		auto pendingBps = std::move(this->pendingBreakpoints_);
		if (pendingBps.get() != nullptr) {
			DEBUG("BreakpointManager::FinishUpdatingNodeBreakpoints(): Syncing breakpoints in server thread");
			this->breakpoints_.swap(pendingBps);
		}
	}

	void BreakpointManager::ClearAllBreakpoints()
	{
		globalBreakpoints_ = 0;
		breakpoints_->clear();
		ClearForcedBreakpoints();
	}

	void BreakpointManager::SetDebuggingDisabled(bool disabled)
	{
		debuggingDisabled_ = disabled;
	}

	void BreakpointManager::SetForcedBreakpoints(bool enabled, uint32_t bpMask, uint32_t flags, uint32_t maxDepth)
	{
		forceBreakpoint_ = enabled;
		forceBreakpointMask_ = bpMask;
		forceBreakpointFlags_ = flags;
		maxBreakDepth_ = maxDepth;
	}

	void BreakpointManager::ClearForcedBreakpoints()
	{
		forceBreakpoint_ = false;
		maxBreakDepth_ = 0;
		forceBreakpointMask_ = 0;
	}

	uint64_t BreakpointManager::MakeNodeBreakpointId(uint32_t nodeId)
	{
		return ((uint64_t)BreakpointItemType::BP_Node << 56) | nodeId;
	}

	uint64_t BreakpointManager::MakeRuleActionBreakpointId(uint32_t nodeId, uint32_t actionIndex)
	{
		return ((uint64_t)BreakpointItemType::BP_RuleAction << 56) | ((uint64_t)actionIndex << 32) | nodeId;
	}

	uint64_t BreakpointManager::MakeGoalInitBreakpointId(uint32_t goalId, uint32_t actionIndex)
	{
		return ((uint64_t)BreakpointItemType::BP_GoalInit << 56) | ((uint64_t)actionIndex << 32) | goalId;
	}

	uint64_t BreakpointManager::MakeGoalExitBreakpointId(uint32_t goalId, uint32_t actionIndex)
	{
		return ((uint64_t)BreakpointItemType::BP_GoalExit << 56) | ((uint64_t)actionIndex << 32) | goalId;
	}

	bool BreakpointManager::ForcedBreakpointConditionsSatisfied(std::vector<CallStackFrame> const & stack, 
		Node * bpNode, BreakpointType bpType)
	{
		// Check if the current frame type is one we can break on
		if (!(forceBreakpointMask_ & bpType)) {
			return false;
		}

		// Check if we're on the correct stack depth
		if (stack.size() > maxBreakDepth_) {
			return false;
		}

		// Skip rule pushdown frames (avoids unnecessary additional single-stepping frame)
		if (forceBreakpointFlags_ & ContinueSkipRulePushdown) {
			if (bpType == BreakpointType::BreakOnPushDown
				&& bpNode != nullptr
				&& gNodeVMTWrappers->GetType(bpNode) == NodeType::Rule) {
				return false;
			}
		}

		// Skip database propagation nodes
		if (forceBreakpointFlags_ & ContinueSkipDbPropagation) {
			// Look for a likely database propagation signature in the call stack
			// (an Insert/Delete frame followed by a Pushdown frame)
			for (auto i = 0; i < stack.size() - 1; i++) {
				auto & first = stack[i];
				auto & second = stack[i + 1];

				if ((first.frameType == BreakpointReason::NodeInsertTuple
					|| first.frameType == BreakpointReason::NodeDeleteTuple)
					&& (second.frameType == BreakpointReason::NodePushDownTuple
						|| second.frameType == BreakpointReason::NodePushDownTupleDelete)) {
					// Check whether the first node is a parent of the second node
					auto secondType = gNodeVMTWrappers->GetType(second.node);
					uint32_t parentNodeId;
					if (secondType == NodeType::Rule || secondType == NodeType::RelOp)
					{
						auto rel = static_cast<RelNode *>(second.node);
						parentNodeId = rel->Parent.Id;
					}
					else if (secondType == NodeType::And || secondType == NodeType::NotAnd) {
						auto join = static_cast<JoinNode *>(second.node);
						parentNodeId = join->Left.Id;
					}
					else {
						ERR("Debugger::ForcedBreakpointConditionsSatisfied(): Illegal call order: %d --> %d",
							first.node->Id, second.node->Id);
						parentNodeId = first.node->Id;
					}

					if (parentNodeId != first.node->Id) {
						return false;
					}
				}
			}
		}

		return true;
	}

	bool BreakpointManager::ShouldTriggerBreakpoint(std::vector<CallStackFrame> const & stack, Node * bpNode, 
		uint64_t bpNodeId, BreakpointType bpType, GlobalBreakpointType globalBpType)
	{
		if (debuggingDisabled_) {
			return false;
		}

		// Check if there is a breakpoint on this node ID
		auto it = breakpoints_->find(bpNodeId);
		if (it != breakpoints_->end()
			&& (it->second.type & bpType)) {
			return true;
		}

		// Check if there is a global breakpoint for this frame type
		if (globalBreakpoints_ & globalBpType) {
			return true;
		}

		// Check if we're single stepping
		if (forceBreakpoint_
			&& ForcedBreakpointConditionsSatisfied(stack, bpNode, bpType)) {
			return true;
		}

		return false;
	}

	bool BreakpointManager::ShouldTriggerGlobalBreakpoint(GlobalBreakpointType globalBpType)
	{
		return (globalBreakpoints_ & globalBpType) != 0;
	}



	Debugger::Debugger(OsirisStaticGlobals & globals, DebugMessageHandler & messageHandler)
		: globals_(globals), messageHandler_(messageHandler),
		actionMappings_(globals),
		debugAdapters_(globals),
		breakpoints_(globals)
	{
		if (messageHandler_.IsConnected()) {
			breakpoints_.SetGlobalBreakpoints(
				GlobalBreakpointType::GlobalBreakOnStoryLoaded);
		}

		messageHandler_.SetDebugger(this);

		using namespace std::placeholders;
		gNodeVMTWrappers->IsValidPreHook = std::bind(&Debugger::IsValidPreHook, this, _1, _2, _3);
		gNodeVMTWrappers->IsValidPostHook = std::bind(&Debugger::IsValidPostHook, this, _1, _2, _3, _4);
		gNodeVMTWrappers->PushDownPreHook = std::bind(&Debugger::PushDownPreHook, this, _1, _2, _3, _4, _5);
		gNodeVMTWrappers->PushDownPostHook = std::bind(&Debugger::PushDownPostHook, this, _1, _2, _3, _4, _5);
		gNodeVMTWrappers->InsertPreHook = std::bind(&Debugger::InsertPreHook, this, _1, _2, _3);
		gNodeVMTWrappers->InsertPostHook = std::bind(&Debugger::InsertPostHook, this, _1, _2, _3);
		gNodeVMTWrappers->CallQueryPreHook = std::bind(&Debugger::CallQueryPreHook, this, _1, _2);
		gNodeVMTWrappers->CallQueryPostHook = std::bind(&Debugger::CallQueryPostHook, this, _1, _2, _3);
		DEBUG("Debugger::Debugger(): Attached to story");
	}

	Debugger::~Debugger()
	{
		DEBUG("Debugger::~Debugger(): Shutting down debugger");
		messageHandler_.SendDebugSessionEnded();
		messageHandler_.SetDebugger(nullptr);

		if (gNodeVMTWrappers) {
			gNodeVMTWrappers->IsValidPreHook = std::function<void(Node *, VirtTupleLL *, AdapterRef *)>();
			gNodeVMTWrappers->IsValidPostHook = std::function<void(Node *, VirtTupleLL *, AdapterRef *, bool)>();
			gNodeVMTWrappers->PushDownPreHook = std::function<void(Node *, VirtTupleLL *, AdapterRef *, EntryPoint, bool)>();
			gNodeVMTWrappers->PushDownPostHook = std::function<void(Node *, VirtTupleLL *, AdapterRef *, EntryPoint, bool)>();
			gNodeVMTWrappers->InsertPreHook = std::function<void(Node *, TuplePtrLL *, bool)>();
			gNodeVMTWrappers->InsertPostHook = std::function<void(Node *, TuplePtrLL *, bool)>();
			gNodeVMTWrappers->CallQueryPreHook = std::function<void(Node *, OsiArgumentDesc *)>();
			gNodeVMTWrappers->CallQueryPostHook = std::function<void(Node *, OsiArgumentDesc *, bool)>();
		}
	}

	void Debugger::StoryLoaded()
	{
		ServerThreadReentry();
		isInitialized_ = false;
		actionMappings_.UpdateRuleActionMappings();
		debugAdapters_.UpdateAdapters();
		if (!debugAdapters_.HasAllAdapters()) {
			WARN("Debugger::StoryLoaded(): Not all debug adapters are available - some debug calls will not work!");
		}

		messageHandler_.SendStoryLoaded();
		if (breakpoints_.ShouldTriggerGlobalBreakpoint(GlobalBreakpointType::GlobalBreakOnStoryLoaded)) {
			GlobalBreakpointInServerThread(GlobalBreakpointReason::StoryLoaded);
		}
	}

	void Debugger::MergeStarted()
	{
		ServerThreadReentry();
		// Disable debugging during merge, as the nodes will be changing dynamically
		// which breaks most debugger assumptions
		debuggingDisabled_ = true;
		breakpoints_.SetDebuggingDisabled(true);
	}

	void Debugger::MergeFinished()
	{
		ServerThreadReentry();
		debuggingDisabled_ = false;
		breakpoints_.SetDebuggingDisabled(false);

		isInitialized_ = true;
		actionMappings_.UpdateRuleActionMappings();
		if (breakpoints_.ShouldTriggerGlobalBreakpoint(GlobalBreakpointType::GlobalBreakOnGameInit)) {
			GlobalBreakpointInServerThread(GlobalBreakpointReason::GameInit);
		}
	}

	void Debugger::GameInitHook()
	{
		ServerThreadReentry();
		isInitialized_ = true;
		if (breakpoints_.ShouldTriggerGlobalBreakpoint(GlobalBreakpointType::GlobalBreakOnGameInit)) {
			GlobalBreakpointInServerThread(GlobalBreakpointReason::GameInit);
		}
	}

	void Debugger::DeleteAllDataHook()
	{
		ServerThreadReentry();
		isInitialized_ = false;
		if (breakpoints_.ShouldTriggerGlobalBreakpoint(GlobalBreakpointType::GlobalBreakOnGameExit)) {
			GlobalBreakpointInServerThread(GlobalBreakpointReason::GameExit);
		}
	}

	ResultCode Debugger::GetDatabaseContents(uint32_t databaseId)
	{
		auto & dbs = (*globals_.Databases)->Db;
		if (databaseId == 0 || databaseId > dbs.Size)
		{
			WARN("Debugger::GetDatabaseContents(): Invalid database ID %d", databaseId);
			return ResultCode::InvalidDatabaseId;
		}

		if (!isPaused_) {
			// Technically we can read rows anytime, but its not thread-safe and there 
			// is a slight chance of crashing.
			WARN("Debugger::GetDatabaseContents(): Cannot read rows while story is running!");
			return ResultCode::NotInPause;
		}

		auto & db = dbs.Start[databaseId - 1];
		auto const & facts = db->Facts;
		auto head = facts.Head;
		auto current = head->Next;

		messageHandler_.SendBeginDatabaseContents(databaseId);
		while (current != head) {
			messageHandler_.SendDatabaseRow(databaseId, &current->Item);
			current = current->Next;
		}

		messageHandler_.SendEndDatabaseContents(databaseId);

		return ResultCode::Success;
	}

	ResultCode Debugger::ContinueExecution(DbgContinue_Action action, uint32_t breakpointMask, uint32_t flags)
	{
		if (breakpointMask & ~BreakpointTypeAll) {
			WARN("Debugger::ContinueExecution(): Unsupported breakpoint type set: %08x", breakpointMask);
			return ResultCode::UnsupportedBreakpointType;
		}

		if (flags & ~ContinueFlagAll) {
			WARN("Debugger::ContinueExecution(): Unsupported flag set: %08x", flags);
			return ResultCode::UnsupportedContinueFlags;
		}

		std::unique_lock<std::mutex> lk(breakpointMutex_);

		if (action == DbgContinue_Action_PAUSE) {
			if (isPaused_) {
				WARN("Debugger::ContinueExecution(): Already paused");
				return ResultCode::InPause;
			}

			DEBUG("Debugger::ContinueExecution(): Force pause on next node");
			// Forcibly break on the next call
			breakpoints_.SetForcedBreakpoints(true, breakpointMask, flags, 0x7fffffff);
			// This is not a "continue" message, it just sets the breakpoint flags,
			// so we don't go through the continue code here
			return ResultCode::Success;
		}

		if (!isPaused_) {
			WARN("Debugger::ContinueExecution(): Not paused");
			return ResultCode::NotInPause;
		}

		switch (action) {
		case DbgContinue_Action_CONTINUE:
			// No forced break on the next node
			breakpoints_.SetForcedBreakpoints(false, breakpointMask, flags, 0);
			break;

		case DbgContinue_Action_STEP_OVER:
			// Step over the current frame; max depth is the current call stack depth
			breakpoints_.SetForcedBreakpoints(true, breakpointMask, flags, (uint32_t)callStack_.size());
			break;

		case DbgContinue_Action_STEP_INTO:
			// Step into the current frame; max depth is unlimited
			breakpoints_.SetForcedBreakpoints(true, breakpointMask, flags, 0x7fffffff);
			break;

		case DbgContinue_Action_STEP_OUT:
			// Step out of the current frame; max depth is current - 1
			breakpoints_.SetForcedBreakpoints(true, breakpointMask, flags, (uint32_t)callStack_.size() - 1);
			break;

		default:
			WARN("Debugger::ContinueExecution(): Continue action %d not known", action);
			return ResultCode::InvalidContinueAction;
		}

		DEBUG("Debugger::ContinueExecution(): Continuing; action %d, mask %08x, flags %08x", action, breakpointMask, flags);
		isPaused_ = false;
		breakpointCv_.notify_one();

		return ResultCode::Success;
	}

	void Debugger::SyncStory()
	{
		auto const & goalDb = (*globals_.Goals);
		for (unsigned i = 0; i < goalDb->Count; i++) {
			auto goal = goalDb->Goals.Find(i + 1);
			messageHandler_.SendSyncStory(*goal);
		}

		auto const & databaseDb = (*globals_.Databases)->Db;
		for (unsigned i = 0; i < databaseDb.Size; i += 100) {
			uint32_t numDatabases = std::min<uint32_t>(databaseDb.Size - i, 100);
			messageHandler_.SendSyncStory(&databaseDb.Start[i], numDatabases);
		}

		auto const & nodeDb = (*globals_.Nodes)->Db;
		for (unsigned i = 0; i < nodeDb.Size; i += 100) {
			uint32_t numNodes = std::min<uint32_t>(nodeDb.Size - i, 100);
			messageHandler_.SendSyncStory(&nodeDb.Start[i], numNodes);
		}
	}

	void Debugger::Evaluate(uint32_t seq, EvalType type, uint32_t nodeId, MsgTuple const & params,
		std::function<void(ResultCode, bool)> completionCallback)
	{
		pendingActions_.push([=]() {
			bool querySucceeded = false;
			auto rc = this->EvaluateInServerThread(seq, type, nodeId, params, querySucceeded);
			completionCallback(rc, querySucceeded);
		});
		breakpointCv_.notify_one();
	}

	void MsgToValue(MsgTypedValue const & msg, TypedValue & tv, void * tvVmt)
	{
		tv.VMT = tvVmt;
		tv.TypeId = msg.type_id();
		switch ((ValueType)msg.type_id()) {
		case ValueType::None:
			break;

		case ValueType::Integer:
			tv.Value.Val.Int32 = (int32_t)msg.intval();
			break;

		case ValueType::Integer64:
			tv.Value.Val.Int64 = msg.intval();
			break;

		case ValueType::Real:
			tv.Value.Val.Float = msg.floatval();
			break;

		case ValueType::String:
		case ValueType::GuidString:
		default:
			tv.Value.Val.String = _strdup(msg.stringval().c_str());
			break;
		}
	}

	void MsgToTuple(MsgTuple const & msg, VirtTupleLL & tuple, void * tvVmt)
	{
		tuple.Data.Items.Size = msg.column_size();
		auto head = new ListNode<TupleLL::Item>();
		tuple.Data.Items.Head = head;
		head->Head = head;
		head->Next = head;

		auto prev = head;
		for (int i = 0; i < msg.column_size(); i++) {
			auto item = new ListNode<TupleLL::Item>();
			item->Head = head;
			item->Next = head;
			prev->Next = item;

			auto & param = msg.column()[i];
			item->Item.Index = i;
			MsgToValue(param, item->Item.Value, tvVmt);

			prev = item;
		}
	}

	void MsgToTuple(MsgTuple const & msg, TuplePtrLL & tuple, void * tvVmt)
	{
		auto & items = tuple.Items;
		items.Init();

		auto prev = items.Head;
		for (int i = 0; i < msg.column_size(); i++) {
			auto & param = msg.column()[i];
			prev = items.Insert(prev);
			prev->Item = new TypedValue();
			MsgToValue(param, *prev->Item, tvVmt);
		}
	}

	void TupClearOutParams(VirtTupleLL & tuple, FunctionSignature const & signature)
	{
		auto head = tuple.Data.Items.Head;
		auto cur = head->Next;
		for (unsigned i = 0; i < tuple.Data.Items.Size; i++) {
			if (signature.OutParamList.isOutParam(i)) {
				cur->Item.Value.TypeId = (uint32_t)ValueType::None;
			}

			cur = cur->Next;
		}
	}

	bool AreTypesCompatible(uint32_t type1, uint32_t type2)
	{
		if (type1 > (uint32_t)ValueType::GuidString)
		{
			type1 = (uint32_t)ValueType::GuidString;
		}

		if (type2 > (uint32_t)ValueType::GuidString)
		{
			type2 = (uint32_t)ValueType::GuidString;
		}

		return type1 == type2;
	}

	ResultCode Debugger::EvaluateInServerThread(uint32_t seq, EvalType type, uint32_t nodeId, MsgTuple const & params,
		bool & querySucceeded)
	{
		DEBUG("Debugger::EvaluateInServerThread(): Type %d, node %d", type, nodeId);

		if (nodeId == 0 || nodeId > (*globals_.Nodes)->Db.Size) {
			WARN("Debugger::EvaluateInServerThread(): Tried to call nonexistent node %d", nodeId);
			return ResultCode::InvalidNodeId;
		}

		auto node = (*globals_.Nodes)->Db.Start[nodeId - 1];
		if (node->Function == nullptr) {
			WARN("Debugger::EvaluateInServerThread(): Node has no function!");
			return ResultCode::NotCallable;
		}

		auto const & sig = node->Function->Signature;
		if (params.column_size() != sig->Params->Params.Size) {
			WARN("Debugger::EvaluateInServerThread(): Got %d params, but node %d has %d!",
				params.column_size(), nodeId, sig->Params->Params.Size);
			return ResultCode::InvalidParamTupleArity;
		}

		auto typeNode = sig->Params->Params.Head->Next;
		for (unsigned i = 0; i < sig->Params->Params.Size; i++) {
			auto typeId = params.column()[i].type_id();
			if (typeId != (uint32_t)ValueType::None
				&& !AreTypesCompatible(typeNode->Item.Type, typeId)) {
				WARN("Debugger::EvaluateInServerThread(): Parameter %d type mismatch; expected %d, got %d!",
					i, typeNode->Item.Type, typeId);
				return ResultCode::InvalidParamType;
			}
			typeNode = typeNode->Next;

			if (typeId == (uint32_t)ValueType::None
				&& type != EvalType::Pushdown
				&& !sig->OutParamList.isOutParam(i)) {
				WARN("Debugger::EvaluateInServerThread(): Got a null value for IN parameter %d!", i);
				return ResultCode::MissingRequiredParam;
			}
		}

		auto adapter = debugAdapters_.FindAdapter(params.column_size());
		if (adapter == nullptr) {
			WARN("Debugger::EvaluateInServerThread(): No debug adapter available for %d columns!", params.column_size());
			return ResultCode::NoAdapter;
		}

		if (globals_.TypedValueVMT == nullptr) {
			WARN("Debugger::EvaluateInServerThread(): TypedValue VMT not available");
			return ResultCode::EvalEngineNotReady;
		}

		AdapterRef adapterRef;
		adapterRef.Id = adapter->Id;
		adapterRef.Manager = adapter->Db;

		breakpoints_.SetDebuggingDisabled(true);

		switch (type)
		{
		case EvalType::IsValid:
		{
			VirtTupleLL tuple;
			MsgToTuple(params, tuple, globals_.TypedValueVMT);
			// Evaluate whether the query succeeds
			querySucceeded = node->IsValid(&tuple, &adapterRef);
			// Fetch output values
			TupClearOutParams(tuple, *sig);
			node->IsValid(&tuple, &adapterRef);
			// TODO - We need to do this twice, for some reason
			TupClearOutParams(tuple, *sig);
			node->IsValid(&tuple, &adapterRef);
			messageHandler_.SendEvaluateRow(seq, tuple);
			break;
		}

		case EvalType::Insert:
		{
			TuplePtrLL tuple;
			MsgToTuple(params, tuple, globals_.TypedValueVMT);
			node->InsertTuple(&tuple);
			break;
		}

		case EvalType::Delete:
		{
			TuplePtrLL tuple;
			MsgToTuple(params, tuple, globals_.TypedValueVMT);
			node->DeleteTuple(&tuple);
			break;
		}

		default:
			WARN("Debugger::EvaluateInServerThread(): Unknown eval type %d", type);
			breakpoints_.SetDebuggingDisabled(false);
			return ResultCode::InvalidEvalType;
		}

		breakpoints_.SetDebuggingDisabled(false);

		return ResultCode::Success;
	}

	void Debugger::ServerThreadReentry()
	{
		// Called when the debugger is entered from any of the server thread hooks
		std::function<void ()> func;
		while (pendingActions_.try_pop(func)) {
			func();
		}
	}

	void Debugger::FinishedSingleStep()
	{
		// Called when we're finished single stepping and want to cancel single-step triggers
		breakpoints_.ClearForcedBreakpoints();
	}

	void Debugger::FinishUpdatingNodeBreakpoints()
	{
		DEBUG("Debugger::FinishUpdatingNodeBreakpoints()");

		pendingActions_.push([this]() {
			breakpoints_.FinishUpdatingNodeBreakpoints();
		});
		breakpointCv_.notify_one();
	}

	void Debugger::ConditionalBreakpointInServerThread(Node * bpNode, uint64_t bpNodeId, BreakpointType bpType, GlobalBreakpointType globalBpType)
	{
		if (breakpoints_.ShouldTriggerBreakpoint(callStack_, bpNode, bpNodeId, bpType, globalBpType)) {
			FinishedSingleStep();
			BreakpointInServerThread();
		}
	}

	void Debugger::BreakpointInServerThread()
	{
		if (callStack_.empty()) {
			Fail("Tried to trigger breakpoint with empty callstack");
		}

		auto const & lastFrame = *callStack_.rbegin();
		DEBUG("Debugger::BreakpointInServerThread(): type %d", lastFrame.frameType);
		{
			std::unique_lock<std::mutex> lk(breakpointMutex_);
			isPaused_ = true;
		}

		QueryResultInfo * queryResults = nullptr;
		if (hasLastQueryInfo_
			&& callStack_.size() == lastQueryDepth_ - 1)
		{
			queryResults = &lastQueryResults_;
		}

		messageHandler_.SendBreakpointTriggered(callStack_, queryResults);

		{
			std::unique_lock<std::mutex> lk(breakpointMutex_);
			breakpointCv_.wait(lk, [this]() { this->ServerThreadReentry(); return !this->isPaused_; });
		}

		DEBUG("Continuing from breakpoint.");
	}

	void Debugger::GlobalBreakpointInServerThread(GlobalBreakpointReason reason)
	{
		if (debuggingDisabled_) return;

		DEBUG("Debugger::GlobalBreakpointInServerThread(): Reason %d", reason);
		{
			std::unique_lock<std::mutex> lk(breakpointMutex_);
			isPaused_ = true;
		}

		messageHandler_.SendGlobalBreakpointTriggered(reason);

		{
			std::unique_lock<std::mutex> lk(breakpointMutex_);
			breakpointCv_.wait(lk, [this]() { return !this->isPaused_; });
		}

		DEBUG("Continuing from breakpoint.");
		ServerThreadReentry();
	}

	void Debugger::PushFrame(CallStackFrame const & frame)
	{
		callStack_.push_back(frame);
	}

	void Debugger::PopFrame(CallStackFrame const & frame)
	{
		if (callStack_.empty()) {
			Fail("Tried to remove frame from empty callstack");
		}

		auto const & lastFrame = *callStack_.rbegin();
		if (lastFrame.frameType != frame.frameType
			|| lastFrame.node != frame.node
			|| lastFrame.goal != frame.goal
			|| lastFrame.tupleLL != frame.tupleLL
			|| lastFrame.tuplePtrLL != frame.tuplePtrLL
			|| lastFrame.actionIndex != frame.actionIndex) {
			Fail("Call stack frame mismatch");
		}

		callStack_.pop_back();
	}

	void Debugger::IsValidPreHook(Node * node, VirtTupleLL * tuple, AdapterRef * adapter)
	{
		ServerThreadReentry();
		PushFrame({ BreakpointReason::NodeIsValid, node, nullptr, 0, &tuple->Data, nullptr });

#if defined(DUMP_TRACEPOINTS)
		DEBUG("IsValid(Node %d)", node->Id);
#endif
		ConditionalBreakpointInServerThread(
			node,
			BreakpointManager::MakeNodeBreakpointId(node->Id),
			BreakOnValid,
			GlobalBreakOnValid);
	}

	void Debugger::IsValidPostHook(Node * node, VirtTupleLL * tuple, AdapterRef * adapter, bool succeeded)
	{
		hasLastQueryInfo_ = true;
		lastQueryDepth_ = (uint32_t)callStack_.size();
		lastQueryResults_.queryNodeId = node->Id;
		lastQueryResults_.succeeded = succeeded;
		if (gNodeVMTWrappers->GetType(node) != NodeType::DivQuery
			&& !lastQueryResults_.results.empty()) {
			lastQueryResults_.results.clear();
		}

		ServerThreadReentry();
		PopFrame({ BreakpointReason::NodeIsValid, node, nullptr, 0, &tuple->Data, nullptr });
	}

	void Debugger::PushDownPreHook(Node * node, VirtTupleLL * tuple, AdapterRef * adapter, EntryPoint entry, bool deleted)
	{
		ServerThreadReentry();
		auto reason = deleted ? BreakpointReason::NodePushDownTupleDelete : BreakpointReason::NodePushDownTuple;
		PushFrame({ reason, node, nullptr, 0, &tuple->Data, nullptr });

#if defined(DUMP_TRACEPOINTS)
		DEBUG("PushDown(Node %d)", node->Id);
#endif
		ConditionalBreakpointInServerThread(
			node,
			BreakpointManager::MakeNodeBreakpointId(node->Id),
			BreakOnPushDown,
			GlobalBreakOnPushDown);

		hasLastQueryInfo_ = false;
	}

	void Debugger::PushDownPostHook(Node * node, VirtTupleLL * tuple, AdapterRef * adapter, EntryPoint entry, bool deleted)
	{
		// Trigger a failed query breakpoint if the last query didn't succeed
		if (hasLastQueryInfo_
			&& callStack_.size() == lastQueryDepth_ - 1
			&& !lastQueryResults_.succeeded) {
			ConditionalBreakpointInServerThread(
				node,
				BreakpointManager::MakeNodeBreakpointId(node->Id),
				BreakOnFailedQuery,
				GlobalBreakOnFailedQuery);
		}

		ServerThreadReentry();
		auto reason = deleted ? BreakpointReason::NodePushDownTupleDelete : BreakpointReason::NodePushDownTuple;
		PopFrame({ reason, node, nullptr, 0, &tuple->Data, nullptr });
	}

	void Debugger::InsertPreHook(Node * node, TuplePtrLL * tuple, bool deleted)
	{
		ServerThreadReentry();
		auto reason = deleted ? BreakpointReason::NodeDeleteTuple : BreakpointReason::NodeInsertTuple;
		PushFrame({ reason, node, nullptr, 0, nullptr, tuple });

#if defined(DUMP_TRACEPOINTS)
		DEBUG("%s(Node %d)", deleted ? L"Delete" : L"Insert", node->Id);
#endif
		ConditionalBreakpointInServerThread(
			node,
			BreakpointManager::MakeNodeBreakpointId(node->Id),
			deleted ? BreakOnDelete : BreakOnInsert,
			deleted ? GlobalBreakOnDelete : GlobalBreakOnInsert);
	}

	void Debugger::InsertPostHook(Node * node, TuplePtrLL * tuple, bool deleted)
	{
		ServerThreadReentry();
		auto reason = deleted ? BreakpointReason::NodeDeleteTuple : BreakpointReason::NodeInsertTuple;
		PopFrame({ reason, node, nullptr, 0, nullptr, tuple });
	}

	void Debugger::CallQueryPreHook(Node * node, OsiArgumentDesc * args)
	{
		ServerThreadReentry();

#if defined(DUMP_TRACEPOINTS)
		DEBUG("CallQuery(Node %d)", node->Id);
#endif
		// No breakpoint allowed on CallQuery
	}

	void Debugger::CallQueryPostHook(Node * node, OsiArgumentDesc * args, bool succeeded)
	{
		ServerThreadReentry();
		// No breakpoint allowed on CallQuery

		lastQueryResults_.results.clear();
		while (args) {
			lastQueryResults_.results.push_back(args->Value);
			args = args->NextParam;
		}
	}

	void Debugger::RuleActionPreHook(RuleActionNode * action)
	{
		// Avoid action mapping errors during merge
		if (debuggingDisabled_) {
			return;
		}

		ServerThreadReentry();
		auto const * mapping = actionMappings_.FindActionMapping(action);
		if (mapping == nullptr) {
			return;
		}

		BreakpointReason reason;
		BreakpointType bpType;
		GlobalBreakpointType globalBpType;
		uint64_t breakpointId;
		if (mapping->rule != nullptr) {
			reason = BreakpointReason::RuleActionCall;
			bpType = BreakpointType::BreakOnRuleAction;
			globalBpType = GlobalBreakpointType::GlobalBreakOnRuleAction;
			breakpointId = BreakpointManager::MakeRuleActionBreakpointId(mapping->rule->Id, mapping->actionIndex);
#if defined(DUMP_TRACEPOINTS)
			DEBUG("RuleAction(Rule %d, Action %d)", mapping->rule->Id, mapping->actionIndex);
#endif
		} else if (mapping->isInit) {
			reason = BreakpointReason::GoalInitCall;
			bpType = BreakpointType::BreakOnInitCall;
			globalBpType = GlobalBreakpointType::GlobalBreakOnInitCall;
			breakpointId = BreakpointManager::MakeGoalInitBreakpointId(mapping->goal->Id, mapping->actionIndex);
#if defined(DUMP_TRACEPOINTS)
			DEBUG("GoalInit(Goal %d, Action %d)", mapping->goal->Id, mapping->actionIndex);
#endif
		} else {
			reason = BreakpointReason::GoalExitCall;
			bpType = BreakpointType::BreakOnExitCall;
			globalBpType = GlobalBreakpointType::GlobalBreakOnExitCall;
			breakpointId = BreakpointManager::MakeGoalExitBreakpointId(mapping->goal->Id, mapping->actionIndex);
#if defined(DUMP_TRACEPOINTS)
			DEBUG("GoalExit(Goal %d, Action %d)", mapping->goal->Id, mapping->actionIndex);
#endif
		}

		PushFrame({ reason, mapping->rule, mapping->goal, mapping->actionIndex, nullptr, nullptr });

		ConditionalBreakpointInServerThread(nullptr, breakpointId, bpType, globalBpType);

		// Capture debug output from the DebugBreak() Osiris call and forward it to the dbg frontend
		
		if (action->FunctionName != nullptr
			&& action->Arguments != nullptr
			&& action->Arguments->Args.Size == 1
			&& strcmp(action->FunctionName, "DebugBreak") == 0) {
			TypedValue * message = action->Arguments->Args.Head->Next->Item;
			if (message->TypeId == (uint32_t)ValueType::String
				&& message->Value.Val.String != nullptr) {
				messageHandler_.SendDebugOutput(message->Value.Val.String);
			}
			else {
				WARN("Invalid message parameter type for DebugBreak(): %d", message->TypeId);
			}
		}
	}

	void Debugger::RuleActionPostHook(RuleActionNode * action)
	{
		// Avoid action mapping errors during merge
		if (debuggingDisabled_) {
			return;
		}

		ServerThreadReentry();
		auto const * mapping = actionMappings_.FindActionMapping(action);
		if (mapping == nullptr) {
			return;
		}

		BreakpointReason reason;
		if (mapping->rule != nullptr) {
			reason = BreakpointReason::RuleActionCall;
		}
		else if (mapping->isInit) {
			reason = BreakpointReason::GoalInitCall;
		}
		else {
			reason = BreakpointReason::GoalExitCall;
		}

		PopFrame({ reason, mapping->rule, mapping->goal, mapping->actionIndex, nullptr, nullptr });
	}
}

#endif
