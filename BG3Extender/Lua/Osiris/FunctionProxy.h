#pragma once

#include <Lua/LuaBinding.h>
#include <GameDefinitions/Osiris.h>
#include <Osiris/Shared/CustomFunctions.h>
#include <Extender/Shared/ExtensionHelpers.h>
#include <Osiris/Shared/OsirisHelpers.h>

BEGIN_NS(esv::lua)

using namespace bg3se::lua;


class OsiFunctionNameMetatable : public LightCppValueMetatable<OsiFunctionNameMetatable>,
	public Indexable, public Callable, public Stringifiable
{
public:
	static constexpr MetatableTag MetaTag = MetatableTag::OsiFunctionName;
	static constexpr bool HasLifetime = false;

	inline static void Make(lua_State* L, uint64_t handle)
	{
		lua_push_cppvalue(L, MetaTag, 0, handle);
	}

	inline static OsirisNameCache const* Get(lua_State* L)
	{
		CppValueMetadata self;
		lua_get_cppvalue(L, 1, MetaTag, self);
		return Get(L, self);
	}

	inline static OsirisNameCache const* Get(lua_State* L, CppValueMetadata& self)
	{
		auto cache = ServerState::FromLua(L)->Osiris().GetNameResolver().GetCache(self.Value);
		if (!cache) {
			luaL_error(L, "Osi cache entry %08x references nonexistent function", self.Value);
		}

		return cache;
	}

	static int Index(lua_State* L, CppValueMetadata& self);
	static int Call(lua_State* L, CppValueMetadata& self);
	static int ToString(lua_State* L, CppValueMetadata& self);
	static char const* GetTypeName(lua_State* L, CppValueMetadata& self);

private:
	static void GetInputArities(lua_State* L, CppValueMetadata& self);
	static void GetArities(lua_State* L, CppValueMetadata& self);

	static int LuaGet(lua_State* L);
	static int LuaDelete(lua_State* L);
	static int LuaExists(lua_State* L);
	static int LuaType(lua_State* L);
};


END_NS()
