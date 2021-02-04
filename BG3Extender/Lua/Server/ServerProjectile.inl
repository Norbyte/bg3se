//#include <GameDefinitions/Projectile.h>
#include <Lua/LuaBinding.h>

namespace bg3se::lua
{
	/*char const* const ObjectProxy<esv::ShootProjectileHelper>::MetatableName = "esv::ShootProjectileRequest";

	esv::ShootProjectileHelper* ObjectProxy<esv::ShootProjectileHelper>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		luaL_error(L, "ShootProjectileRequest object has expired!");
		return nullptr;
	}

	int ObjectProxy<esv::ShootProjectileHelper>::Index(lua_State* L)
	{
		return GenericGetter(L, gShootProjectileHelperPropertyMap);
	}

	int ObjectProxy<esv::ShootProjectileHelper>::NewIndex(lua_State* L)
	{
		return GenericSetter(L, gShootProjectileHelperPropertyMap);
	}


	char const* const ObjectProxy<esv::Projectile>::MetatableName = "esv::Projectile";

	esv::Projectile* ObjectProxy<esv::Projectile>::Get(lua_State* L)
	{
		if (obj_) return obj_;
		auto projectile = esv::GetEntityWorld()->GetProjectile(handle_);
		if (projectile == nullptr) luaL_error(L, "Projectile handle invalid");
		return projectile;
	}

	int ObjectProxy<esv::Projectile>::Index(lua_State* L)
	{
		auto projectile = Get(L);
		if (!projectile) return 0;

		StackCheck _(L, 1);
		auto prop = luaL_checkstring(L, 2);

		if (strcmp(prop, GFS.strHandle.Str) == 0) {
			push(L, projectile->Base.Component.Handle);
			return 1;
		}

		if (strcmp(prop, GFS.strRootTemplate.Str) == 0) {
			ObjectProxy<ProjectileTemplate>::New(L, projectile->ProjectileTemplate);
			return 1;
		}

		bool fetched = LuaPropertyMapGet(L, gProjectilePropertyMap, projectile, prop, true);
		if (!fetched) push(L, nullptr);
		return 1;
	}

	int ObjectProxy<esv::Projectile>::NewIndex(lua_State* L)
	{
		return GenericSetter(L, gProjectilePropertyMap);
	}*/
}
