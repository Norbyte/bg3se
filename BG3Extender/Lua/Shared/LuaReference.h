#pragma once

#include <GameDefinitions/Base/Base.h>
#include <lua.h>

BEGIN_NS(lua)

// Generation ID forward declarations
uint32_t get_generation_id(lua_State* L);

class RegistryEntry
{
public:
	RegistryEntry();
	RegistryEntry(lua_State * L, int index);
	RegistryEntry(lua_State * L, Ref const& local);
	~RegistryEntry();

	RegistryEntry(RegistryEntry const &) = delete;
	RegistryEntry(RegistryEntry &&);

	RegistryEntry & operator = (RegistryEntry const &) = delete;
	RegistryEntry & operator = (RegistryEntry &&);

	explicit inline operator bool() const
	{
		return ref_ != -1;
	}

	inline int GetRef() const
	{
		return ref_;
	}

	void Push() const;

	inline void ResetWithoutUnbind()
	{
		L_ = nullptr;
		ref_ = -1;
	}

	void Bind(lua_State* L, Ref const& ref);

private:
	lua_State * L_;
	int ref_;
};

class PersistentRegistryEntry
{
public:
	PersistentRegistryEntry();
	PersistentRegistryEntry(lua_State * L, int index);
	PersistentRegistryEntry(lua_State * L, Ref const& local);
	~PersistentRegistryEntry();

	PersistentRegistryEntry(PersistentRegistryEntry const &) = delete;
	PersistentRegistryEntry(PersistentRegistryEntry&&);

	PersistentRegistryEntry& operator = (PersistentRegistryEntry const &) = delete;
	PersistentRegistryEntry& operator = (PersistentRegistryEntry&&);

	bool IsValid(lua_State* L) const;
	bool TryPush(lua_State* L) const;
	void Bind(lua_State* L, Ref const& ref);

	inline int GetRef() const
	{
		return ref_;
	}

	inline void ResetWithoutUnbind()
	{
		L_ = nullptr;
		ref_ = -1;
	}

private:
	lua_State * L_;
	uint32_t generationId_;
	int ref_;

	void Release();
};

enum class RefType
{
	None = 0,
	Local = 1,
	Registry = 2
};

class Ref
{
public:
	inline Ref()
		: type_(RefType::None), index_(0)
	{}

	inline Ref(lua_State* L, int index)
		: type_(RefType::Local), index_(index)
	{}

	inline Ref(lua_State* L, RefType type, int index)
		: type_(type), index_(index)
	{}

	inline Ref(RegistryEntry const& ref)
		: type_(ref ? RefType::Registry : RefType::None), index_(ref ? ref.GetRef() : 0)
	{}

	inline Ref(Ref const& ref)
		: type_(ref.type_), index_(ref.index_)
	{}

	explicit inline operator bool() const
	{
		return type_ != RefType::None;
	}

	void Push(lua_State* L) const
	{
		switch (type_) {
		case RefType::None:
			lua_pushnil(L);
			break;

		case RefType::Local:
			lua_pushvalue(L, index_);
			break;

		case RefType::Registry:
			lua_rawgeti(L, LUA_REGISTRYINDEX, index_);
			break;
		}
	}

	inline int Index() const
	{
		return index_;
	}

	inline RefType Type() const
	{
		return type_;
	}

private:
	RefType type_;
	int index_;
};

class PersistentRef
{
public:
	inline PersistentRef()
		: generationId_(0), type_(RefType::None), index_(0)
	{}

	inline PersistentRef(lua_State* L, int index)
		: generationId_(get_generation_id(L)), type_(RefType::Local), index_(index)
	{}

	inline PersistentRef(lua_State* L, RegistryEntry const& ref)
		: generationId_(get_generation_id(L)), type_(ref ? RefType::Registry : RefType::None), index_(ref ? ref.GetRef() : 0)
	{}

	inline PersistentRef(lua_State * L, PersistentRegistryEntry const& ref)
		: generationId_(get_generation_id(L)), type_(ref.IsValid(L) ? RefType::Registry : RefType::None), index_(ref.IsValid(L) ? ref.GetRef() : 0)
	{}

	inline PersistentRef(PersistentRef const& ref)
		: generationId_(ref.generationId_), type_(ref.type_), index_(ref.index_)
	{}

	explicit inline operator bool() const
	{
		return type_ != RefType::None;
	}

	inline bool IsValid(lua_State* L) const
	{
		return generationId_ == get_generation_id(L);
	}

	inline Ref MakeRef(lua_State* L) const
	{
		assert(IsValid(L));
		return Ref(L, type_, index_);
	}

	void Push(lua_State* L) const
	{
		if (!IsValid(L)) {
			lua_pushnil(L);
			return;
		}

		switch (type_) {
		case RefType::None:
			lua_pushnil(L);
			break;

		case RefType::Local:
			lua_pushvalue(L, index_);
			break;

		case RefType::Registry:
			lua_rawgeti(L, LUA_REGISTRYINDEX, index_);
			break;
		}
	}

private:
	RefType type_;
	int index_;
	uint32_t generationId_;
};

END_NS()
