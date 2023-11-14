#pragma once

#include <Lua/LuaHelpers.h>
#include <lua/LuaBinding.h>
#include <GameDefinitions/Stats/Functors.h>

#include <mutex>
#include <unordered_set>
#include <optional>

BEGIN_NS(lua::stats)

using namespace bg3se::stats;

class StatsProxy : public Userdata<StatsProxy>, public Indexable, public NewIndexable
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

class SpellPrototypeProxy : public Userdata<SpellPrototypeProxy>, public Indexable
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
	public NewIndexable
{
public:
	static char const * const MetatableName;

	int Index(lua_State * L);
	int NewIndex(lua_State * L);
};


/*
class DamageList : public Userdata<DamageList><PushPolicy::None>
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

END_NS()

namespace bg3se::lua
{
	void RegisterSharedMetatables(lua_State* L);
}
