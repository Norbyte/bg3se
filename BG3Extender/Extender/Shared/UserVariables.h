#pragma once

#include <GameDefinitions/Base/Base.h>
#include <Extender/Shared/ExtenderNet.h>

BEGIN_SE()

enum class UserVariableType
{
	Null = 0,
	Int64 = 1,
	Double = 2,
	String = 3,
	Composite = 4
};

struct UserVariable
{
	UserVariable() : Type(UserVariableType::Null) {}
	UserVariable(int64_t v) : Type(UserVariableType::Int64), Int(v) {}
	UserVariable(double v) : Type(UserVariableType::Double), Dbl(v) {}
	UserVariable(FixedString const& v) : Type(UserVariableType::String), Str(v) {}
	UserVariable(STDString const& v) : Type(UserVariableType::Composite), CompositeStr(v) {}

	void SavegameVisit(ObjectVisitor* visitor);
	void ToNetMessage(net::UserVar& var) const;
	void FromNetMessage(net::UserVar const& var);
	size_t Budget() const;

	UserVariableType Type{ UserVariableType::Null };
	bool Dirty{ false };
	int64_t Int{ 0ll };
	double Dbl{ 0.0 };
	FixedString Str;
	STDString CompositeStr;
};

enum class UserVariableFlags
{
	IsOnServer = 1 << 0,
	IsOnClient = 1 << 1,
	SyncServerToClient = 1 << 2,
	SyncClientToServer = 1 << 3,
	SyncOnWrite = 1 << 4,
	DontCache = 1 << 5,
	WriteableOnServer = 1 << 6,
	WriteableOnClient = 1 << 7,
	SyncOnTick = 1 << 8,
	Persistent = 1 << 9
};

template<> struct IsBitmask<UserVariableFlags>
{
	static const bool value = true;
};

struct UserVariablePrototype
{
	UserVariableFlags Flags{ (UserVariableFlags)0 };

	inline bool Has(UserVariableFlags flag) const
	{
		return (Flags & flag) == flag;
	}

	inline bool NeedsSyncFor(bool server) const
	{
		return Has(server ? UserVariableFlags::SyncServerToClient : UserVariableFlags::SyncClientToServer);
	}

	inline bool IsAvailableFor(bool server) const
	{
		return Has(server ? UserVariableFlags::IsOnServer : UserVariableFlags::IsOnClient);
	}

	inline bool IsWriteableFor(bool server) const
	{
		return Has(server ? UserVariableFlags::WriteableOnServer : UserVariableFlags::WriteableOnClient);
	}

	bool NeedsRebroadcast(bool server) const;
};

class UserVariableInterface
{
public:
	virtual UserVariable* Get(Guid const& entity, FixedString const& key) = 0;
};

class UserVariableSyncWriter
{
public:
	inline UserVariableSyncWriter(UserVariableInterface* vars, bool isServer)
		: vars_(vars), isServer_(isServer)
	{}

	void Flush(bool force);
	void Sync(Guid const& entity, FixedString const& key, UserVariablePrototype const& proto, UserVariable const* value);
	void DeferredSync(Guid const& entity, FixedString const& key);

private:
	// Max (approximate) size of sync message we're allowed to send
	static constexpr size_t SyncMessageBudget = 300000;

	struct SyncRequest
	{
		Guid Entity;
		FixedString Variable;
	};

	UserVariableInterface* vars_;
	Array<SyncRequest> deferredSyncs_;
	Array<SyncRequest> nextTickSyncs_;
	net::ExtenderMessage* syncMsg_{ nullptr };
	size_t syncMsgBudget_{ 0 };
	bool isServer_;

	void AppendToSyncMessage(Guid const& entity, FixedString const& key, UserVariable const& value);
	void FlushSyncQueue(Array<SyncRequest>& queue);
	bool MakeSyncMessage();
	void SendSyncs();
};

class UserVariableManager : public UserVariableInterface
{
public:
	struct EntityVariables
	{
		MultiHashMap<FixedString, UserVariable> Vars;
	};

	inline UserVariableManager(bool isServer, ecs::EntitySystemHelpersBase& entityHelpers)
		: sync_(this, isServer),
		isServer_(isServer),
		entityHelpers_(entityHelpers)
	{}

	inline ecs::EntitySystemHelpersBase& GetEntityHelpers() const
	{
		return entityHelpers_;
	}

	Guid EntityToGuid(EntityHandle const& entity) const;
	EntityHandle GuidToEntity(Guid const& guid) const;
	UserVariable* Get(Guid const& entity, FixedString const& key) override;

	MultiHashMap<FixedString, UserVariable>* GetAll(Guid const& entity);
	MultiHashMap<Guid, EntityVariables>& GetAll();
	EntityVariables* Set(Guid const& entity, FixedString const& key, UserVariablePrototype const& proto, UserVariable&& value);
	void MarkDirty(Guid const& entity, FixedString const& key, UserVariable& value);
	UserVariablePrototype const* GetPrototype(FixedString const& key) const;
	void RegisterPrototype(FixedString const& key, UserVariablePrototype const& proto);

	void BindCache(lua::CachedUserVariableManager* cache);
	void Update();
	void Flush(bool force);
	void SavegameVisit(ObjectVisitor* visitor);
	void NetworkSync(net::UserVar const& var);

private:
	MultiHashMap<Guid, EntityVariables> vars_;
	MultiHashMap<FixedString, UserVariablePrototype> prototypes_;
	UserVariableSyncWriter sync_;
	bool isServer_;
	lua::CachedUserVariableManager* cache_{ nullptr };
	ecs::EntitySystemHelpersBase& entityHelpers_;
};

class ModVariableMap
{
public:
	using VariableMap = MultiHashMap<FixedString, UserVariable>;

	inline ModVariableMap()
	{}

	inline ModVariableMap(Guid moduleUuid, bool isServer)
		: moduleUuid_(moduleUuid),
		isServer_(isServer)
	{}

	inline Guid const& ModuleUuid() const
	{
		return moduleUuid_;
	}

	UserVariable* Get(FixedString const& key);
	VariableMap& GetAll();
	UserVariable* Set(FixedString const& key, UserVariablePrototype const& proto, UserVariable&& value);
	void ClearVars();
	UserVariablePrototype const* GetPrototype(FixedString const& key) const;
	void RegisterPrototype(FixedString const& key, UserVariablePrototype const& proto);
	void SavegameVisit(ObjectVisitor* visitor);

private:
	Guid moduleUuid_;
	VariableMap vars_;
	MultiHashMap<FixedString, UserVariablePrototype> prototypes_;
	bool isServer_;
};

class ModVariableManager : public UserVariableInterface
{
public:
	inline ModVariableManager(bool isServer)
		: sync_(this, isServer),
		isServer_(isServer)
	{}

	std::optional<int32_t> GuidToModId(Guid const& uuid) const;
	UserVariable* Get(Guid const& modUuid, FixedString const& key) override;

	ModVariableMap::VariableMap* GetAll(Guid const& modUuid);
	MultiHashMap<Guid, ModVariableMap>& GetAll();
	ModVariableMap* GetMod(Guid const& modUuid);
	ModVariableMap* GetOrCreateMod(Guid const& modUuid);
	UserVariablePrototype const* GetPrototype(Guid const& modUuid, FixedString const& key) const;
	void RegisterPrototype(Guid const& modUuid, FixedString const& key, UserVariablePrototype const& proto);
	ModVariableMap* Set(Guid const& modUuid, FixedString const& key, UserVariablePrototype const& proto, UserVariable&& value);
	void Set(ModVariableMap& mod, FixedString const& key, UserVariablePrototype const& proto, UserVariable&& value);
	void MarkDirty(Guid const& modUuid, FixedString const& key, UserVariable& value);

	void OnModuleLoading();
	void BindCache(lua::CachedModVariableManager* cache);
	void Update();
	void Flush(bool force);
	void SavegameVisit(ObjectVisitor* visitor);
	void NetworkSync(net::UserVar const& var);

private:
	MultiHashMap<Guid, uint32_t> modIndices_;
	MultiHashMap<Guid, ModVariableMap> vars_;
	UserVariableSyncWriter sync_;
	bool isServer_;
	lua::CachedModVariableManager* cache_{ nullptr };
};

END_SE()

BEGIN_NS(lua)

enum class CachedUserVariableType
{
	Null = 0,
	Int64 = 1,
	Double = 2,
	String = 3,
	Reference = 4
};

struct CachedUserVariable
{
	CachedUserVariable() : Type(CachedUserVariableType::Null) {}
	CachedUserVariable(int64_t v) : Type(CachedUserVariableType::Int64), Int(v) {}
	CachedUserVariable(double v) : Type(CachedUserVariableType::Double), Dbl(v) {}
	CachedUserVariable(FixedString const& v) : Type(CachedUserVariableType::String), Str(v) {}
	CachedUserVariable(lua_State* L, Ref const& v);
	CachedUserVariable(lua_State* L, UserVariable const& v);
	CachedUserVariable(CachedUserVariable&& o);

	CachedUserVariable& operator = (CachedUserVariable&& o);

	CachedUserVariableType Type{ CachedUserVariableType::Null };
	bool Dirty{ false };
	int64_t Int{ 0ll };
	double Dbl{ 0.0 };
	FixedString Str;
	RegistryEntry Reference;

	void Push(lua_State* L) const;
	bool LikelyChanged(CachedUserVariable const& o) const;
	UserVariable ToUserVariable(lua_State* L) const;
	STDString StringifyReference(lua_State* L) const;
	void ParseReference(lua_State* L, StringView json);
};

class CachedUserVariableManager
{
public:
	CachedUserVariableManager(UserVariableManager& global, bool isServer);
	~CachedUserVariableManager();

	inline UserVariableManager& GetGlobal() const
	{
		return global_;
	}

	inline bool IsServer() const
	{
		return isServer_;
	}

	void Push(lua_State* L, EntityHandle entity, FixedString const& key);
	void Push(lua_State* L, EntityHandle entity, FixedString const& key, UserVariablePrototype const& proto);
	void Set(lua_State* L, EntityHandle entity, FixedString const& key, CachedUserVariable && var);
	void Set(lua_State* L, EntityHandle entity, FixedString const& key, UserVariablePrototype const& proto, CachedUserVariable && var);
	void Invalidate();
	void Invalidate(EntityHandle entity, FixedString const& key);
	void Flush();

private:
	struct FlushRequest
	{
		EntityHandle Entity;
		FixedString Variable;
		UserVariablePrototype const* Proto;
	};
	
	struct EntityVariables
	{
		Guid CachedGuid;
		MultiHashMap<FixedString, CachedUserVariable> Vars;
	};

	UserVariableManager& global_;
	bool isServer_;
	MultiHashMap<EntityHandle, EntityVariables> vars_;
	Array<FlushRequest> flushQueue_;

	CachedUserVariable* GetFromCache(EntityHandle entity, FixedString const& key, Guid& entityGuid);
	CachedUserVariable* GetFromCache(EntityHandle entity, FixedString const& key);
	CachedUserVariable* PutCache(lua_State* L, EntityHandle entity, FixedString const& key, Guid const& entityGuid, UserVariablePrototype const& proto, UserVariable const& value);
	CachedUserVariable* PutCache(EntityHandle entity, FixedString const& key, Guid const& entityGuid, UserVariablePrototype const& proto, CachedUserVariable && value, bool isWrite);
	void MoveToGlobal(lua_State* L, EntityHandle entity, FixedString const& key, UserVariablePrototype const& proto, CachedUserVariable& var);
};

class CachedModVariableManager
{
public:
	CachedModVariableManager(ModVariableManager& global, bool isServer);
	~CachedModVariableManager();

	inline ModVariableManager& GetGlobal() const
	{
		return global_;
	}

	inline bool IsServer() const
	{
		return isServer_;
	}

	Guid ModIndexToGuid(uint32_t modIndex);
	void Push(lua_State* L, uint32_t modIndex, FixedString const& key);
	void Push(lua_State* L, uint32_t modIndex, FixedString const& key, UserVariablePrototype const& proto);
	void Set(lua_State* L, uint32_t modIndex, FixedString const& key, CachedUserVariable && var);
	void Set(lua_State* L, uint32_t modIndex, FixedString const& key, UserVariablePrototype const& proto, CachedUserVariable && var);
	void Invalidate();
	void Invalidate(uint32_t modIndex, FixedString const& key);
	void Flush();

private:
	struct FlushRequest
	{
		uint32_t ModIndex;
		FixedString Variable;
		UserVariablePrototype const* Proto;
	};
	
	struct ModVariables
	{
		Guid CachedGuid;
		MultiHashMap<FixedString, CachedUserVariable> Vars;
	};

	ModVariableManager& global_;
	bool isServer_;
	MultiHashMap<uint32_t, ModVariables> vars_;
	Array<FlushRequest> flushQueue_;

	CachedUserVariable* GetFromCache(uint32_t modIndex, FixedString const& key, Guid& modUuid);
	CachedUserVariable* GetFromCache(uint32_t modIndex, FixedString const& key);
	CachedUserVariable* PutCache(lua_State* L, uint32_t modIndex, FixedString const& key, Guid const& modUuid, UserVariablePrototype const& proto, UserVariable const& value);
	CachedUserVariable* PutCache(uint32_t modIndex, FixedString const& key, Guid const& modUuid, UserVariablePrototype const& proto, CachedUserVariable && value, bool isWrite);
};

END_NS()
