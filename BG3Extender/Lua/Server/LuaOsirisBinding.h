#pragma once

#include <Lua/LuaBinding.h>
#include <GameDefinitions/Osiris.h>
#include <Osiris/Shared/CustomFunctions.h>
#include <Extender/Shared/ExtensionHelpers.h>
#include <Osiris/Shared/OsirisHelpers.h>

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

void LuaToOsi(lua_State * L, int i, TypedValue & tv, ValueType osiType, bool allowNil = false);
TypedValue * LuaToOsi(lua_State * L, int i, ValueType osiType, bool allowNil = false);
void LuaToOsi(lua_State * L, int i, OsiArgumentValue & arg, ValueType osiType, bool allowNil = false, bool reuseStrings = false);
void OsiToLua(lua_State * L, OsiArgumentValue const & arg);
void OsiToLua(lua_State * L, TypedValue const & tv);
Function const* LookupOsiFunction(STDString const& name, uint32_t arity);

class OsiFunction
{
public:
	inline bool IsBound() const
	{
		return function_ != nullptr;
	}

	inline bool IsDB() const
	{
		return IsBound() && function_->Type == FunctionType::Database;
	}

	bool Bind(Function const * func, class ServerState & state);
	void Unbind();

	int LuaCall(lua_State * L);
	int LuaGet(lua_State * L);
	int LuaDelete(lua_State * L);
	int LuaDeferredNotification(lua_State * L);

private:
	Function const * function_{ nullptr };
	AdapterRef adapter_;
	ServerState * state_;

	void OsiCall(lua_State * L);
	void OsiDeferredNotification(lua_State * L);
	void OsiInsert(lua_State * L, bool deleteTuple);
	int OsiQuery(lua_State * L);
	int OsiUserQuery(lua_State * L);

	bool MatchTuple(lua_State * L, int firstIndex, TupleVec const & tuple);
	void ConstructTuple(lua_State * L, TupleVec const & tuple);
};

class OsiFunctionNameProxy : public Userdata<OsiFunctionNameProxy>, public Callable
{
public:
	static char const * const MetatableName;
	// Maximum number of OUT params that a query can return.
	// (This setting determines how many function arities we'll check during name lookup)
	static constexpr uint32_t MaxQueryOutParams = 6;

	static void PopulateMetatable(lua_State * L);

	OsiFunctionNameProxy(STDString const & name, ServerState & state);

	void UnbindAll();
	int LuaCall(lua_State * L);

private:
	STDString name_;
	Vector<OsiFunction> functions_;
	ServerState & state_;
	uint32_t generationId_;

	static int LuaGet(lua_State * L);
	static int LuaDelete(lua_State * L);
	static int LuaDeferredNotification(lua_State * L);
	bool BeforeCall(lua_State * L);
	OsiFunction * TryGetFunction(uint32_t arity);
	OsiFunction * CreateFunctionMapping(uint32_t arity, Function const * func);
};


class CustomLuaCall : public CustomCallBase
{
public:
	inline CustomLuaCall(STDString const & name, std::vector<CustomFunctionParam> params,
		RegistryEntry handler)
		: CustomCallBase(name, std::move(params)), handler_(std::move(handler))
	{}

	virtual bool Call(OsiArgumentDesc const & params) override;

private:
	RegistryEntry handler_;
};


class CustomLuaQuery : public CustomQueryBase
{
public:
	inline CustomLuaQuery(STDString const & name, std::vector<CustomFunctionParam> params,
		RegistryEntry handler)
		: CustomQueryBase(name, std::move(params)), handler_(std::move(handler))
	{}

	virtual bool Query(OsiArgumentDesc & params) override;

private:
	RegistryEntry handler_;
};

inline void OsiReleaseArgument(OsiArgumentDesc & arg)
{
	arg.NextParam = nullptr;
}

inline void OsiReleaseArgument(TypedValue & arg) {}
inline void OsiReleaseArgument(ListNode<TypedValue *> & arg) {}
inline void OsiReleaseArgument(ListNode<TupleLL::Item> & arg) {}

template <class T>
class OsiArgumentPool
{
public:
	OsiArgumentPool()
	{
		argumentPool_.resize(1024);
		usedArguments_ = 0;
	}

	T * AllocateArguments(uint32_t num, uint32_t & tail)
	{
		if (usedArguments_ + num > argumentPool_.size()) {
			throw std::runtime_error("Ran out of argument descriptors");
		}

		tail = usedArguments_;
		auto ptr = argumentPool_.data() + usedArguments_;
		for (uint32_t i = 0; i < num; i++) {
			new (ptr + i) T();
		}

		usedArguments_ += num;
		return ptr;
	}

	void ReleaseArguments(uint32_t tail, uint32_t num)
	{
		if (tail + num != usedArguments_) {
			throw std::runtime_error("Attempted to release arguments out of order");
		}

		for (uint32_t i = 0; i < num; i++) {
			OsiReleaseArgument(argumentPool_[tail + i]);
		}

		usedArguments_ -= num;
	}

private:
	std::vector<T> argumentPool_;
	uint32_t usedArguments_;
};

template <class T>
class OsiArgumentListPin
{
public:
	inline OsiArgumentListPin(OsiArgumentPool<T> & pool, uint32_t numArgs)
		: pool_(pool), numArgs_(numArgs)
	{
		args_ = pool.AllocateArguments(numArgs_, tail_);
	}

	inline ~OsiArgumentListPin()
	{
		pool_.ReleaseArguments(tail_, numArgs_);
	}

	inline T * Args() const
	{
		return args_;
	}

private:
	OsiArgumentPool<T> & pool_;
	uint32_t numArgs_;
	uint32_t tail_;
	T * args_;
};


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

class OsirisBinding : Noncopyable<OsirisBinding>
{
public:
	OsirisBinding(ExtensionState& state);
	~OsirisBinding();

	inline uint32_t GenerationId() const
	{
		return generationId_;
	}

	inline IdentityAdapterMap & GetIdentityAdapterMap()
	{
		return identityAdapters_;
	}

	inline OsiArgumentPool<OsiArgumentDesc> & GetArgumentDescPool()
	{
		return argDescPool_;
	}

	inline OsiArgumentPool<TypedValue> & GetTypedValuePool()
	{
		return tvPool_;
	}

	inline OsiArgumentPool<ListNode<TypedValue *>> & GetTypedValueNodePool()
	{
		return tvNodePool_;
	}

	inline OsiArgumentPool<ListNode<TupleLL::Item>> & GetTupleNodePool()
	{
		return tupleNodePool_;
	}

	inline OsirisCallbackManager& GetOsirisCallbacks()
	{
		return osirisCallbacks_;
	}

	void StoryLoaded();
	void StorySetMerging(bool isMerging);

private:
	OsiArgumentPool<OsiArgumentDesc> argDescPool_;
	OsiArgumentPool<TypedValue> tvPool_;
	OsiArgumentPool<ListNode<TypedValue *>> tvNodePool_;
	OsiArgumentPool<ListNode<TupleLL::Item>> tupleNodePool_;
	IdentityAdapterMap identityAdapters_;
	// ID of current story instance.
	// Used to invalidate function/node pointers in Lua userdata objects
	uint32_t generationId_{ 0 };
	OsirisCallbackManager osirisCallbacks_;
};

END_NS()
