#include <GameDefinitions/Stats/Stats.h>
#include <Lua/LuaSerializers.h>

namespace bg3se::lua
{
	/*char const* const ObjectProxy<CombatComponentTemplate>::MetatableName = "eoc::CombatComponentTemplate";

	CombatComponentTemplate* ObjectProxy<CombatComponentTemplate>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		luaL_error(L, "CharacterTemplate not bound!");
		return nullptr;
	}

	int ObjectProxy<CombatComponentTemplate>::Index(lua_State* L)
	{
		return GenericGetter(L, gCombatComponentTemplatePropertyMap);
	}

	int ObjectProxy<CombatComponentTemplate>::NewIndex(lua_State* L)
	{
		return GenericSetter(L, gCombatComponentTemplatePropertyMap);
	}



	char const* const ObjectProxy<CharacterTemplate>::MetatableName = "eoc::CharacterTemplate";

	CharacterTemplate* ObjectProxy<CharacterTemplate>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		luaL_error(L, "CharacterTemplate not bound!");
		return nullptr;
	}

	int ObjectProxy<CharacterTemplate>::Index(lua_State* L)
	{
		auto obj = Get(L);
		if (!obj) return 0;

		auto prop = luaL_checkstring(L, 2);

		if (strcmp(prop, "CombatTemplate") == 0) {
			ObjectProxy<CombatComponentTemplate>::New(L, &obj->CombatComponent);
			return 1;
		}

		auto fetched = LuaPropertyMapGet(L, gCharacterTemplatePropertyMap, obj, prop, true);
		return fetched ? 1 : 0;
	}

	int ObjectProxy<CharacterTemplate>::NewIndex(lua_State* L)
	{
		return GenericSetter(L, gCharacterTemplatePropertyMap);
	}
	*/


	char const* const ObjectProxy<ItemTemplate>::MetatableName = "eoc::ItemTemplate";

	ItemTemplate* ObjectProxy<ItemTemplate>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		luaL_error(L, "ItemTemplate not bound!");
		return nullptr;
	}

	int ObjectProxy<ItemTemplate>::Index(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*auto obj = Get(L);
		if (!obj) return 0;

		auto prop = luaL_checkstring(L, 2);

		if (strcmp(prop, "CombatTemplate") == 0) {
			ObjectProxy<CombatComponentTemplate>::New(L, &obj->CombatComponent);
			return 1;
		}

		auto fetched = LuaPropertyMapGet(L, gItemTemplatePropertyMap, obj, prop, true);
		return fetched ? 1 : 0;*/
	}

	int ObjectProxy<ItemTemplate>::NewIndex(lua_State* L)
	{
		return luaL_error(L, "Not implemented yet!");

		/*return GenericSetter(L, gItemTemplatePropertyMap);*/
	}



	/*char const* const ObjectProxy<ProjectileTemplate>::MetatableName = "eoc::ProjectileTemplate";

	ProjectileTemplate* ObjectProxy<ProjectileTemplate>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		luaL_error(L, "ProjectileTemplate not bound!");
		return nullptr;
	}

	int ObjectProxy<ProjectileTemplate>::Index(lua_State* L)
	{
		return GenericGetter(L, gProjectileTemplatePropertyMap);
	}

	int ObjectProxy<ProjectileTemplate>::NewIndex(lua_State* L)
	{
		return GenericSetter(L, gProjectileTemplatePropertyMap);
	}


	char const* const ObjectProxy<SurfaceTemplate>::MetatableName = "eoc::SurfaceTemplate";

	SurfaceTemplate* ObjectProxy<SurfaceTemplate>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		luaL_error(L, "SurfaceTemplate not bound!");
		return nullptr;
	}

	int ObjectProxy<SurfaceTemplate>::Index(lua_State* L)
	{
		auto obj = Get(L);
		auto prop = checked_get<char const*>(L, 2);

		if (strcmp(prop, "Statuses") == 0) {
			return LuaWrite(L, obj->Statuses.Value);
		}

		return GenericGetter(L, gSurfaceTemplatePropertyMap);
	}

	int ObjectProxy<SurfaceTemplate>::NewIndex(lua_State* L)
	{
		auto obj = Get(L);
		auto prop = checked_get<char const*>(L, 2);

		if (strcmp(prop, "Statuses") == 0) {
			lua_pushvalue(L, 3);
			LuaRead(L, obj->Statuses.Value);
			lua_pop(L, 1);
			return 0;
		}

		return GenericSetter(L, gSurfaceTemplatePropertyMap);
	}*/
}
