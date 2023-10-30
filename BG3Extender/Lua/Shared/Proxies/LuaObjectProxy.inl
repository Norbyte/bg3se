#include <Lua/Shared/Proxies/LuaObjectProxy.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se::lua
{
	char const* const ObjectProxy::MetatableName = "bg3se::Object";

	int ObjectProxy::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive(L)) {
			luaL_error(L, "Attempted to read dead object of type '%s'", impl->GetTypeName().GetString());
			push(L, nullptr);
			return 1;
		}

		auto prop = get<FixedString>(L, 2);
		if (!impl->GetProperty(L, prop)) {
			push(L, nullptr);
		}

		return 1;
	}

	int ObjectProxy::NewIndex(lua_State* L)
	{
		StackCheck _(L, 0);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive(L)) {
			luaL_error(L, "Attempted to write dead object of type '%s'", impl->GetTypeName().GetString());
			return 0;
		}

		auto prop = get<FixedString>(L, 2);
		impl->SetProperty(L, prop, 3);
		return 0;
	}

	int ObjectProxy::Next(lua_State* L)
	{
		auto impl = GetImpl();
		if (!lifetime_.IsAlive(L)) {
			luaL_error(L, "Attempted to iterate dead object of type '%s'", impl->GetTypeName().GetString());
			return 0;
		}

		if (lua_type(L, 2) == LUA_TNIL) {
			return impl->Next(L, FixedString{});
		} else {
			auto key = get<FixedString>(L, 2);
			return impl->Next(L, key);
		}
	}

	int ObjectProxy::ToString(lua_State* L)
	{
		StackCheck _(L, 1);
		char entityName[200];
		if (lifetime_.IsAlive(L)) {
			_snprintf_s(entityName, std::size(entityName) - 1, "%s (%p)", GetImpl()->GetTypeName().GetString(), GetImpl()->GetRaw(L));
		} else {
			_snprintf_s(entityName, std::size(entityName) - 1, "%s (%p, DEAD)", GetImpl()->GetTypeName().GetString(), GetImpl()->GetRaw(L));
		}

		push(L, entityName);
		return 1;
	}

	void* ObjectProxy::GetRaw(lua_State* L, int index, FixedString const& typeName)
	{
		auto proxy = Userdata<ObjectProxy>::CheckUserData(L, index);
		if (proxy->GetImpl()->IsA(typeName)) {
			auto obj = proxy->GetRaw(L);
			if (obj == nullptr) {
				luaL_error(L, "Argument %d: got object of type '%s' whose lifetime has expired", index, typeName.GetString());
				return nullptr;
			} else {
				return obj;
			}
		} else {
			luaL_error(L, "Argument %d: expected an object of type '%s', got '%s'", index, typeName.GetString(), proxy->GetImpl()->GetTypeName().GetString());
			return nullptr;
		}
	}

	void* ObjectProxy::TryGetRaw(lua_State* L, int index, FixedString const& typeName)
	{
		auto proxy = Userdata<ObjectProxy>::AsUserData(L, index);
		if (proxy && proxy->GetImpl()->IsA(typeName)) {
			return proxy->GetRaw(L);
		} else {
			return nullptr;
		}
	}
}
