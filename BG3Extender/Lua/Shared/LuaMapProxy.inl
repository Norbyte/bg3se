#include <Lua/Shared/LuaMapProxy.h>

namespace bg3se::lua
{
	char const* const MapProxy::MetatableName = "bg3se::Map";

	int MapProxy::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to read dead Map<%s, %s>", impl->GetKeyTypeName(), impl->GetValueTypeName());
			push(L, nullptr);
			return 1;
		}

		if (!impl->GetValue(L, 2)) {
			push(L, nullptr);
		}

		return 1;
	}

	int MapProxy::NewIndex(lua_State* L)
	{
		StackCheck _(L, 0);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to write dead Map<%s, %s>", impl->GetKeyTypeName(), impl->GetValueTypeName());
			return 0;
		}

		impl->SetValue(L, 2, 3);
		return 0;
	}

	int MapProxy::Next(lua_State* L)
	{
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to iterate dead Map<%s, %s>", impl->GetKeyTypeName(), impl->GetValueTypeName());
			return 0;
		}

		return impl->Next(L, 2);
	}

	int MapProxy::ToString(lua_State* L)
	{
		StackCheck _(L, 1);
		char entityName[200];
		auto impl = GetImpl();
		if (lifetime_.IsAlive()) {
			_snprintf_s(entityName, std::size(entityName) - 1, "Map<%s, %s> (%p)", 
				impl->GetKeyTypeName(), impl->GetValueTypeName(), GetImpl()->GetRaw());
		} else {
			_snprintf_s(entityName, std::size(entityName) - 1, "Map<%s, %s> (%p, DEAD)", 
				impl->GetKeyTypeName(), impl->GetValueTypeName(), GetImpl()->GetRaw());
		}

		push(L, entityName);
		return 1;
	}

	int MapProxy::GC(lua_State* L)
	{
		this->~MapProxy();
		return 0;
	}
}
