#pragma once

#include <Lua/LuaHelpers.h>
#include <lua/LuaBinding.h>
#include <GameDefinitions/Stats/Functors.h>

#include <mutex>
#include <unordered_set>
#include <optional>

BEGIN_NS(lua::stats)

using namespace bg3se::stats;

class StatsProxy : public Userdata<StatsProxy>, public Indexable, public NewIndexable, public Pushable
{
public:
	static char const * const MetatableName;

	StatsProxy(Object * obj, std::optional<int> level, LifetimeHandle const& lifetime)
		: obj_(obj), level_(level), lifetime_(lifetime)
	{}

	int Index(lua_State * L);
	int NewIndex(lua_State * L);

	inline Object* Get() const
	{
		return obj_;
	}

private:
	Object * obj_;
	std::optional<int> level_;
	LifetimeHandle lifetime_;

	static int Sync(lua_State* L);
	static int SetPersistence(lua_State* L);
};

class SpellPrototypeProxy : public Userdata<SpellPrototypeProxy>, public Indexable, public Pushable
{
public:
	static char const * const MetatableName;

	SpellPrototypeProxy(SpellPrototype* obj, std::optional<int> level, LifetimeHandle& lifetime);

	int Index(lua_State * L);

private:
	SpellPrototype * obj_;
	Object * stats_;
	std::optional<int> level_;
	LifetimeHandle lifetime_;
};


class StatsExtraDataProxy : public Userdata<StatsExtraDataProxy>, public Indexable, 
	public NewIndexable, public Pushable
{
public:
	static char const * const MetatableName;

	int Index(lua_State * L);
	int NewIndex(lua_State * L);
};


class FunctorsProxy : public ArrayProxyImplBase
{
public:
	FunctorsProxy(LifetimeHandle const& lifetime, Functors* obj);
	~FunctorsProxy() override;

	inline Functors* Get() const
	{
		return object_;
	}

	void* GetRaw() override;
	char const* GetTypeName() const override;
	bool GetElement(lua_State* L, unsigned arrayIndex) override;
	bool SetElement(lua_State* L, unsigned arrayIndex, int luaIndex) override;
	unsigned Length() override;
	int Next(lua_State* L, int key) override;

private:
	Functors* object_;
	LifetimeHandle lifetime_;
};


/*
class DamageList : public Userdata<DamageList>, public Pushable<PushPolicy::None>
{
public:
	static char const * const MetatableName;

	static void PopulateMetatable(lua_State * L);

	inline DamagePairList & Get()
	{
		return damages_;
	}

private:
	DamagePairList damages_;

	static int GetByType(lua_State * L);
	static int Add(lua_State * L);
	static int Clear(lua_State * L);
	static int Multiply(lua_State * L);
	static int Merge(lua_State * L);
	static int ConvertDamageType(lua_State * L);
	static int AggregateSameTypeDamages(lua_State * L);
	static int ToTable(lua_State * L);
};
*/

/*int NewDamageList(lua_State* L);
int GetSurfaceTemplate(lua_State* L);*/

void RegisterStatsLib(lua_State* L);

END_NS()

namespace bg3se::lua::utils
{
	void RegisterUtilsLib(lua_State* L);
	void RegisterStaticDataLib(lua_State* L);
	void RegisterLocalizationLib(lua_State* L);
	void RegisterJsonLib(lua_State* L);
	void RegisterIOLib(lua_State* L);
	void RegisterMathLib(lua_State* L);
}

namespace bg3se::lua
{
	inline void push(lua_State* L, stats::Functors* v)
	{
		ArrayProxy::MakeImplByRef<stats::FunctorsProxy>(L, GetCurrentLifetime(), v);
	}

	void RegisterEntityProxy(lua_State* L);
	void RegisterSharedLibraries(lua_State* L);
}
