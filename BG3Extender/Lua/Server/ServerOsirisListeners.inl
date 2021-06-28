#include <Lua/LuaBinding.h>
#include <Lua/Server/ServerOsirisBinding.h>

namespace bg3se::esv::lua
{
	OsirisCallbackManager::OsirisCallbackManager(ExtensionState& state)
		: state_(state)
	{}

	OsirisCallbackManager::~OsirisCallbackManager()
	{
		auto wrappers = gExtender->GetServer().Osiris().GetVMTWrappers();
		if (osirisHooked_ && wrappers) {
			wrappers->InsertPreHookLua = std::function<void(Node*, TuplePtrLL*, bool)>();
			wrappers->InsertPostHookLua = std::function<void(Node*, TuplePtrLL*, bool)>();
			wrappers->CallQueryPreHookLua = std::function<void(Node*, OsiArgumentDesc*)>();
			wrappers->CallQueryPostHookLua = std::function<void(Node*, OsiArgumentDesc*, bool)>();
		}
	}

	void OsirisCallbackManager::Subscribe(STDString const& name, uint32_t arity, OsirisHookSignature::HookType type, RegistryEntry handler)
	{
		OsirisHookSignature sig{ name, arity, type };
		auto it = nameSubscriberRefs_.insert(std::make_pair(sig, subscribers_.size()));
		subscribers_.push_back(std::move(handler));

		if (storyLoaded_) {
			HookOsiris();
			RegisterNodeHandler(it->first, it->second);
		}
	}

	void OsirisCallbackManager::RunHandlers(uint64_t nodeRef, TuplePtrLL* tuple) const
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
			std::for_each(it.first, it.second, [&lua, this, tuple](std::pair<uint64_t, std::size_t> handler) {
				RunHandler(lua.Get(), subscribers_[handler.second], tuple);
			});
		}
	}

	void OsirisCallbackManager::RunHandler(ServerState & lua, RegistryEntry const& func, TuplePtrLL* tuple) const
	{
		auto L = lua.GetState();
		StackCheck _(L, 0);
		LifetimePin p_(lua.GetStack());
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

	void OsirisCallbackManager::RunHandlers(uint64_t nodeRef, OsiArgumentDesc* args) const
	{
		auto it = nodeSubscriberRefs_.equal_range(nodeRef);
		if (it.first == nodeSubscriberRefs_.end()) {
			return;
		}

		LuaServerPin lua(state_);
		if (lua) {
			std::for_each(it.first, it.second, [&lua, this, args](std::pair<uint64_t, std::size_t> handler) {
				RunHandler(lua.Get(), subscribers_[handler.second], args);
			});
		}
	}

	void OsirisCallbackManager::RunHandler(ServerState & lua, RegistryEntry const& func, OsiArgumentDesc* args) const
	{
		auto L = lua.GetState();
		StackCheck _(L, 0);
		LifetimePin p_(lua.GetStack());
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

	void OsirisCallbackManager::RegisterNodeHandler(OsirisHookSignature const& sig, std::size_t handlerId)
	{
		auto func = LookupOsiFunction(sig.name, sig.arity);
		if (func == nullptr) {
			// OsiWarn("Couldn't register Osiris subscriber for " << sig.name << "/" << sig.arity << ": Symbol not found in story.");
			return;
		}

		if (func->Type == FunctionType::Call) {
			OsiWarn("Couldn't register Osiris subscriber for " << sig.name << "/" << sig.arity << ": Hooks on calls not supported yet!");
			return;
		}

		if (
			// Functions that aren't mapped to a node cannot be hooked
			func->Node.Get() == nullptr
			|| (
				func->Type != FunctionType::Event
				&& func->Type != FunctionType::Query
				&& func->Type != FunctionType::Call
				// User queries (QRY) are disguised as a database
				&& func->Type != FunctionType::Database
				&& func->Type != FunctionType::Proc
			)) {
			OsiWarn("Couldn't register Osiris subscriber for " << sig.name << "/" << sig.arity << ": Symbol must be an event, query, call, DB, PROC or QRY.");
			return;
		}

		uint64_t nodeRef = func->Node.Id;
		if (sig.type == OsirisHookSignature::AfterTrigger || sig.type == OsirisHookSignature::AfterDeleteTrigger) {
			nodeRef |= AfterTriggerNodeRef;
		}
		if (sig.type == OsirisHookSignature::BeforeDeleteTrigger || sig.type == OsirisHookSignature::AfterDeleteTrigger) {
			nodeRef |= DeleteTriggerNodeRef;
		}

		nodeSubscriberRefs_.insert(std::make_pair(nodeRef, handlerId));
	}

	void OsirisCallbackManager::HookOsiris()
	{
		if (osirisHooked_) return;

		gExtender->GetServer().Osiris().HookNodeVMTs();
		auto wrappers = gExtender->GetServer().Osiris().GetVMTWrappers();
		if (wrappers) {
			using namespace std::placeholders;
			wrappers->InsertPreHookLua = std::bind(&OsirisCallbackManager::InsertPreHook, this, _1, _2, _3);
			wrappers->InsertPostHookLua = std::bind(&OsirisCallbackManager::InsertPostHook, this, _1, _2, _3);
			wrappers->CallQueryPreHookLua = std::bind(&OsirisCallbackManager::CallQueryPreHook, this, _1, _2);
			wrappers->CallQueryPostHookLua = std::bind(&OsirisCallbackManager::CallQueryPostHook, this, _1, _2, _3);
		}

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



	int RegisterOsirisListener(lua_State* L)
	{
		StackCheck _(L, 0);
		auto name = checked_get<char const*>(L, 1);
		auto arity = checked_get<int>(L, 2);
		auto typeName = checked_get<char const*>(L, 3);
		luaL_checktype(L, 4, LUA_TFUNCTION);

		OsirisHookSignature::HookType type;
		if (strcmp(typeName, "before") == 0) {
			type = OsirisHookSignature::BeforeTrigger;
		} else if (strcmp(typeName, "after") == 0) {
			type = OsirisHookSignature::AfterTrigger;
		} else if (strcmp(typeName, "beforeDelete") == 0) {
			type = OsirisHookSignature::BeforeDeleteTrigger;
		} else if (strcmp(typeName, "afterDelete") == 0) {
			type = OsirisHookSignature::AfterDeleteTrigger;
		} else {
			return luaL_error(L, "Hook type must be 'before', 'beforeDelete', 'after' or 'afterDelete'");
		}

		LuaServerPin lua(esv::ExtensionState::Get());
		RegistryEntry handler(L, 4);
		lua->GetOsirisCallbacks().Subscribe(name, arity, type, std::move(handler));
		return 0;
	}
}
