#pragma once

#include <Lua/LuaBinding.h>
#include <GameDefinitions/Osiris.h>

BEGIN_NS(esv)

class ExtensionState;

struct OsirisHookSignature
{
	enum HookType
	{
		BeforeTrigger,
		AfterTrigger,
		BeforeDeleteTrigger,
		AfterDeleteTrigger
	};

	STDString name;
	uint32_t arity;
	HookType type;

	inline bool operator == (OsirisHookSignature const& o) const
	{
		return name == o.name
			&& arity == o.arity
			&& type == o.type;
	}
};

END_NS()

namespace std
{
	template<> struct hash<bg3se::esv::OsirisHookSignature>
	{
		typedef bg3se::esv::OsirisHookSignature argument_type;
		typedef std::size_t result_type;

		result_type operator()(argument_type const& sig) const noexcept
		{
			return std::hash<bg3se::STDString>{}(sig.name) ^ std::hash<uint32_t>{}(sig.arity | ((uint32_t)sig.type << 6));
		}
	};
}

BEGIN_NS(esv::lua)

using namespace bg3se::lua;

class ServerState;

// Stores an immutable copy of the subscribed handler ID-s during callback evaluation
// to ensure that changes to the subscriber list don't affect the current list being evaluated
class PendingCallbackManager
{
public:
	~PendingCallbackManager();
	Array<uint32_t>* Enter(std::unordered_multimap<uint64_t, uint32_t>::iterator& begin,
		std::unordered_multimap<uint64_t, uint32_t>::iterator& end);
	void Exit(Array<uint32_t>* v);

private:
	Array<Array<uint32_t>*> cache_;
	uint32_t depth_{ 0 };
};

class OsirisCallbackManager : Noncopyable<OsirisCallbackManager>
{
public:
	using SubscriptionId = uint32_t;

	OsirisCallbackManager(ExtensionState& state);
	~OsirisCallbackManager();

	SubscriptionId Subscribe(STDString const& name, uint32_t arity, OsirisHookSignature::HookType type, RegistryEntry handler);
	bool Unsubscribe(SubscriptionId id);

	void StoryLoaded();
	void StorySetMerging(bool isMerging);

	void InsertPreHook(Node* node, TuplePtrLL* tuple, bool deleted);
	void InsertPostHook(Node* node, TuplePtrLL* tuple, bool deleted);
	void CallQueryPreHook(Node* node, OsiArgumentDesc* args);
	void CallQueryPostHook(Node* node, OsiArgumentDesc* args, bool succeeded);
	void CallPreHook(uint32_t functionId, OsiArgumentDesc* args);
	void CallPostHook(uint32_t functionId, OsiArgumentDesc* args, bool succeeded);
	void EventPreHook(Function* node, OsiArgumentDesc* args);
	void EventPostHook(Function* node, OsiArgumentDesc* args);

private:
	static constexpr uint64_t AfterTriggerNodeRef = 0x8000000000000000ull;
	static constexpr uint64_t DeleteTriggerNodeRef = 0x4000000000000000ull;
	static constexpr uint64_t AfterFunctionRef = 0x2000000000000000ull;
	static constexpr uint64_t BeforeFunctionRef = 0x1000000000000000ull;

	struct Subscription
	{
		RegistryEntry Callback;
		OsirisHookSignature Signature;
		std::optional<uint64_t> Node;
	};

	ExtensionState& state_;
	SaltedPool<Subscription> subscriptions_;
	std::unordered_multimap<OsirisHookSignature, SubscriptionId> nameSubscriberRefs_;
	std::unordered_multimap<uint64_t, SubscriptionId> nodeSubscriberRefs_;
	PendingCallbackManager pendingCallbacks_;
	bool storyLoaded_{ false };
	bool osirisHooked_{ false };
	// Are we currently merging Osiris files (story)?
	// If so, then we won't trigger events for calls/inserts that might occur during the merge,
	// as those are not real inserts but byproducts of the merge process.
	bool merging_{ false };

	void RegisterNodeHandler(OsirisHookSignature const& sig, SubscriptionId handlerId);
	void HookOsiris();

	void RunHandlers(uint64_t nodeRef, TuplePtrLL* tuple);
	void RunHandler(ServerState& lua, RegistryEntry const& func, TuplePtrLL* tuple);
	void RunHandlers(uint64_t nodeRef, OsiArgumentDesc* tuple);
	void RunHandler(ServerState& lua, RegistryEntry const& func, OsiArgumentDesc* tuple);
};

END_NS()
