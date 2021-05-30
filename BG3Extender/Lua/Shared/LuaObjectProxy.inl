#include <Lua/Shared/LuaObjectProxy.h>

namespace bg3se::lua
{

	bool GenericPropertyMap::GetRawProperty(lua_State* L, LifetimeHolder const& lifetime, void* object, STDString const& prop) const
	{
		auto it = Properties.find(prop);
		if (it == Properties.end()) {
			return 0;
		}

		return it->second.Get(L, lifetime, object, it->second.Offset);
	}

	bool GenericPropertyMap::SetRawProperty(lua_State* L, LifetimeHolder const& lifetime, void* object, STDString const& prop, int index) const
	{
		auto it = Properties.find(prop);
		if (it == Properties.end()) {
			return 0;
		}

		return it->second.Set(L, lifetime, object, index, it->second.Offset);
	}

	void GenericPropertyMap::AddRawProperty(STDString const& prop, typename RawPropertyAccessors::Getter* getter, 
		typename RawPropertyAccessors::Setter* setter, std::size_t offset)
	{
		Properties.insert(std::make_pair(prop, RawPropertyAccessors{ getter, setter, offset }));
	}


	int ObjectProxy::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to read dead object of type '%s'", impl->GetTypeName());
			push(L, nullptr);
			return 1;
		}

		auto prop = luaL_checkstring(L, 2);
		if (!impl->GetProperty(L, prop)) {
			push(L, nullptr);
		}

		return 1;
	}

	int ObjectProxy::NewIndex(lua_State* L)
	{
		StackCheck _(L, 0);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to write dead object of type '%s'", impl->GetTypeName());
			return 0;
		}

		auto prop = luaL_checkstring(L, 2);
		impl->SetProperty(L, prop, 3);
		return 0;
	}

	int ObjectProxy::Next(lua_State* L)
	{
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to iterate dead object of type '%s'", impl->GetTypeName());
			return 0;
		}

		if (lua_type(L, 2) == LUA_TNIL) {
			return impl->Next(L, nullptr);
		} else {
			auto key = checked_get<char const*>(L, 2);
			return impl->Next(L, key);
		}
	}

	int ObjectProxy::ToString(lua_State* L)
	{
		StackCheck _(L, 1);
		char entityName[200];
		if (lifetime_.IsAlive()) {
			_snprintf_s(entityName, std::size(entityName) - 1, "%s (%p)", GetImpl()->GetTypeName(), GetImpl()->GetRaw());
		} else {
			_snprintf_s(entityName, std::size(entityName) - 1, "%s (%p, DEAD)", GetImpl()->GetTypeName(), GetImpl()->GetRaw());
		}

		push(L, entityName);
		return 1;
	}

	int ObjectProxy::GC(lua_State* L)
	{
		this->~ObjectProxy();
		return 0;
	}
}
