#include <Lua/Osiris/CallbackManager.h>

BEGIN_NS(esv::lua)

PendingCallbackManager::~PendingCallbackManager()
{
	for (auto entry : cache_) {
		GameDelete(entry);
	}
}

Array<uint32_t>* PendingCallbackManager::Enter(std::unordered_multimap<uint64_t, uint32_t>::iterator& begin,
	std::unordered_multimap<uint64_t, uint32_t>::iterator& end)
{
	if (depth_ >= cache_.size()) {
		cache_.push_back(GameAlloc<Array<uint32_t>>());
	}

	auto entry = cache_[depth_];
	entry->clear();
	depth_++;

	for (auto it = begin; it != end; it++) {
		entry->Add(it->second);
	}

	return entry;
}

void PendingCallbackManager::Exit(Array<uint32_t>* v)
{
	assert(depth_ > 0 && cache_[depth_ - 1] == v);
	depth_--;
}


OsirisCallbackManager::OsirisCallbackManager(ExtensionState& state)
	: state_(state)
{}

OsirisCallbackManager::~OsirisCallbackManager()
{
	if (osirisHooked_) {
		gExtender->GetServer().Osiris().BindCallbackManager(nullptr);
	}
}

OsirisCallbackManager::SubscriptionId OsirisCallbackManager::Subscribe(STDString const& name, uint32_t arity, OsirisHookSignature::HookType type, RegistryEntry handler)
{
	OsirisHookSignature sig{ name, arity, type };
	SubscriptionId id;

	auto sub = subscriptions_.Add(id);
	sub->Callback = std::move(handler);
	sub->Signature = sig;

	nameSubscriberRefs_.insert(std::make_pair(sig, id));

	if (storyLoaded_) {
		HookOsiris();
		RegisterNodeHandler(sig, id);
	}

	return id;
}

bool OsirisCallbackManager::Unsubscribe(SubscriptionId id)
{
	auto sub = subscriptions_.Find(id);
	if (!sub) return false;

	auto nameIt = nameSubscriberRefs_.find(sub->Signature);
	while (nameIt != nameSubscriberRefs_.end() && nameIt->first == sub->Signature) {
		if (nameIt->second == id) {
			nameSubscriberRefs_.erase(nameIt);
			break;
		}

		nameIt++;
	}

	if (sub->Node) {
		auto it = nodeSubscriberRefs_.find(*sub->Node);
		while (it != nodeSubscriberRefs_.end() && it->first == *sub->Node) {
			if (it->second == id) {
				nodeSubscriberRefs_.erase(it);
				break;
			}

			it++;
		}
	}

	sub->Callback.Reset();
	subscriptions_.Free(id);
	return true;
}

void OsirisCallbackManager::RunHandlers(uint64_t nodeRef, TuplePtrLL* tuple)
{
	if (merging_) {
		return;
	}

	auto it = nodeSubscriberRefs_.equal_range(nodeRef);
	if (it.first == nodeSubscriberRefs_.end()) {
		return;
	}

	LuaServerPin lua(state_);
	if (lua) {
		// Make a copy of the subscriber indices to as the subscriber map iterator can be invalidated
		// if the Lua handler function registers a new subscriber
		auto indices = pendingCallbacks_.Enter(it.first, it.second);
		for (auto index : *indices) {
			auto sub = subscriptions_.Find(index);
			if (sub) {
				RunHandler(lua.Get(), sub->Callback, tuple);
			}
		}
		pendingCallbacks_.Exit(indices);
	}
}

void OsirisCallbackManager::RunHandler(ServerState& lua, RegistryEntry const& func, TuplePtrLL* tuple)
{
	auto L = lua.GetState();
	StackCheck _(L, 0);
	LifetimeStackPin p_(lua.GetStack());
	int32_t stackArgs = 1;
	if (tuple != nullptr) {
		auto node = tuple->Items.Head->Next;
		while (node != tuple->Items.Head) {
			node = node->Next;
			stackArgs++;
		}
	}

	lua_checkstack(L, stackArgs);
	auto stackSize = lua_gettop(L);

	try {
		func.Push();

		int32_t numArgs = 0;
		if (tuple != nullptr) {
			auto node = tuple->Items.Head->Next;
			while (node != tuple->Items.Head) {
				OsiToLua(L, *node->Item);
				node = node->Next;
				numArgs++;
			}
		}

		if (CallWithTraceback(L, numArgs, 0) != 0) {
			LuaError("Osiris event handler failed: " << lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
	catch (Exception& e) {
		auto stackRemaining = lua_gettop(L) - stackSize;
		if (stackRemaining > 0) {
			LuaError("Osiris event callback failed: " << lua_tostring(L, -1));
			lua_pop(L, stackRemaining);
		} else {
			LuaError("Internal error during call Osiris event callback: " << e.what());
		}
	}
}

void OsirisCallbackManager::RunHandlers(uint64_t nodeRef, OsiArgumentDesc* args)
{
	auto it = nodeSubscriberRefs_.equal_range(nodeRef);
	if (it.first == nodeSubscriberRefs_.end()) {
		return;
	}

	LuaServerPin lua(state_);
	if (lua) {
		// Make a copy of the subscriber indices to as the subscriber map iterator can be invalidated
		// if the Lua handler function registers a new subscriber
		auto indices = pendingCallbacks_.Enter(it.first, it.second);
		for (auto index : *indices) {
			auto sub = subscriptions_.Find(index);
			if (sub) {
				RunHandler(lua.Get(), sub->Callback, args);
			}
		}
		pendingCallbacks_.Exit(indices);
	}
}

void OsirisCallbackManager::RunHandler(ServerState& lua, RegistryEntry const& func, OsiArgumentDesc* args)
{
	auto L = lua.GetState();
	StackCheck _(L, 0);
	LifetimeStackPin _p(lua.GetStack());
	int32_t stackArgs = 1;
	auto node = args;
	while (node) {
		node = node->NextParam;
		stackArgs++;
	}

	lua_checkstack(L, stackArgs);
	auto stackSize = lua_gettop(L);

	try {
		func.Push();

		int32_t numArgs = 0;
		auto node = args;
		while (node) {
			OsiToLua(L, node->Value);
			node = node->NextParam;
			numArgs++;
		}

		if (CallWithTraceback(L, numArgs, 0) != 0) {
			LuaError("Osiris event handler failed: " << lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
	catch (Exception& e) {
		auto stackRemaining = lua_gettop(L) - stackSize;
		if (stackRemaining > 0) {
			LuaError("Osiris event callback failed: " << lua_tostring(L, -1));
			lua_pop(L, stackRemaining);
		}
		else {
			LuaError("Internal error during call Osiris event callback: " << e.what());
		}
	}
}

void OsirisCallbackManager::StoryLoaded()
{
	HookOsiris();
	storyLoaded_ = true;
	nodeSubscriberRefs_.clear();
	for (auto const& it : nameSubscriberRefs_) {
		RegisterNodeHandler(it.first, it.second);
	}
}

void OsirisCallbackManager::StorySetMerging(bool isMerging)
{
	merging_ = isMerging;
}

void OsirisCallbackManager::RegisterNodeHandler(OsirisHookSignature const& sig, SubscriptionId handlerId)
{
	auto func = LookupOsiFunction(sig.name, sig.arity);
	if (func != nullptr && func->Type == FunctionType::UserQuery) {
		// We need to find the backing node for the user query
		func = LookupOsiFunction(sig.name + "__DEF__", sig.arity);
	}

	if (func == nullptr) {
		OsiError("Couldn't register Osiris subscriber for " << sig.name << "/" << sig.arity << ": Symbol not found in story.");
		return;
	}

	if (
		// Functions that aren't mapped to a node cannot be hooked, with the exception of events/calls
		(func->Type != FunctionType::Event 
		&& func->Type != FunctionType::Call
		&& func->Node.Get() == nullptr)
		|| (
			func->Type != FunctionType::Event
			&& func->Type != FunctionType::Query
			&& func->Type != FunctionType::Call
			// User queries (QRY) are disguised as a database
			&& func->Type != FunctionType::UserQuery
			&& func->Type != FunctionType::Database
			&& func->Type != FunctionType::Proc
		)) {
		OsiError("Couldn't register Osiris subscriber for " << sig.name << "/" << sig.arity << ": Symbol must be an event, query, call, DB, PROC or QRY.");
		return;
	}

	uint64_t nodeRef;

	if (func->Type == FunctionType::Event || func->Type == FunctionType::Call) {
		nodeRef = func->OsiFunctionId;
		if (sig.type == OsirisHookSignature::BeforeTrigger) {
			nodeRef |= BeforeFunctionRef;
		} else if (sig.type == OsirisHookSignature::AfterTrigger) {
			nodeRef |= AfterFunctionRef;
		} else {
			OsiError("Couldn't register Osiris subscriber for " << sig.name << "/" << sig.arity << ": Delete triggers not supported on events.");
			return;
		}
	} else {
		nodeRef = func->Node.Id;
		if (sig.type == OsirisHookSignature::AfterTrigger || sig.type == OsirisHookSignature::AfterDeleteTrigger) {
			nodeRef |= AfterTriggerNodeRef;
		}
		if (sig.type == OsirisHookSignature::BeforeDeleteTrigger || sig.type == OsirisHookSignature::AfterDeleteTrigger) {
			nodeRef |= DeleteTriggerNodeRef;
		}
	}

	nodeSubscriberRefs_.insert(std::make_pair(nodeRef, handlerId));
	auto sub = subscriptions_.Find(handlerId);
	if (sub) {
		sub->Node = nodeRef;
	}
}

void OsirisCallbackManager::HookOsiris()
{
	if (osirisHooked_) return;

	gExtender->GetServer().Osiris().HookNodeVMTs();
	gExtender->GetServer().Osiris().BindCallbackManager(this);

	osirisHooked_ = true;
}

void OsirisCallbackManager::InsertPreHook(Node* node, TuplePtrLL* tuple, bool deleted)
{
	uint64_t nodeRef = node->Id;
	if (deleted) {
		nodeRef |= DeleteTriggerNodeRef;
	}

	RunHandlers(nodeRef, tuple);
}

void OsirisCallbackManager::InsertPostHook(Node* node, TuplePtrLL* tuple, bool deleted)
{
	uint64_t nodeRef = node->Id | AfterTriggerNodeRef;
	if (deleted) {
		nodeRef |= DeleteTriggerNodeRef;
	}

	RunHandlers(nodeRef, tuple);
}

void OsirisCallbackManager::CallQueryPreHook(Node* node, OsiArgumentDesc* args)
{
	RunHandlers(node->Id, args);
}

void OsirisCallbackManager::CallQueryPostHook(Node* node, OsiArgumentDesc* args, bool succeeded)
{
	uint64_t nodeRef = node->Id | AfterTriggerNodeRef;
	RunHandlers(nodeRef, args);
}

void OsirisCallbackManager::CallPreHook(uint32_t functionId, OsiArgumentDesc* args)
{
	uint64_t nodeRef = functionId | BeforeFunctionRef;
	RunHandlers(nodeRef, args);
}

void OsirisCallbackManager::CallPostHook(uint32_t functionId, OsiArgumentDesc* args, bool succeeded)
{
	uint64_t nodeRef = functionId | AfterFunctionRef;
	RunHandlers(nodeRef, args);
}

void OsirisCallbackManager::EventPreHook(OsiFunctionDef* node, OsiArgumentDesc* args)
{
	uint64_t nodeRef = node->OsiFunctionId | BeforeFunctionRef;
	RunHandlers(nodeRef, args);
}

void OsirisCallbackManager::EventPostHook(OsiFunctionDef* node, OsiArgumentDesc* args)
{
	uint64_t nodeRef = node->OsiFunctionId | AfterFunctionRef;
	RunHandlers(nodeRef, args);
}

END_NS()
