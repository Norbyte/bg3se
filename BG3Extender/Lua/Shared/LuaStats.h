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
	static int SetRawAttribute(lua_State* L);
	static int CopyFrom(lua_State* L);
};


END_NS()

namespace bg3se::lua
{
	void RegisterSharedMetatables(lua_State* L);
}
