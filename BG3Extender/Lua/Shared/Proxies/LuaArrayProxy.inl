#include <Lua/Shared/Proxies/LuaArrayProxy.h>

namespace bg3se::lua
{
	char const* const ArrayProxy::MetatableName = "bg3se::Array";

	int ArrayProxy::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive(L)) {
			luaL_error(L, "Attempted to read dead Array<%s>", impl->GetTypeName());
			push(L, nullptr);
			return 1;
		}

		auto index = get<int>(L, 2);
		// TODO - integer range check?
		if (!impl->GetElement(L, index)) {
			push(L, nullptr);
		}

		return 1;
	}

	int ArrayProxy::NewIndex(lua_State* L)
	{
		StackCheck _(L, 0);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive(L)) {
			luaL_error(L, "Attempted to write dead Array<%s>", impl->GetTypeName());
			return 0;
		}

		auto index = get<int>(L, 2);
		impl->SetElement(L, index, 3);
		return 0;
	}

	int ArrayProxy::Length(lua_State* L)
	{
		StackCheck _(L, 1);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive(L)) {
			luaL_error(L, "Attempted to get length of dead Array<%s>", impl->GetTypeName());
			push(L, nullptr);
			return 1;
		}

		push(L, impl->Length());
		return 1;
	}

	int ArrayProxy::Next(lua_State* L)
	{
		auto impl = GetImpl();
		if (!lifetime_.IsAlive(L)) {
			luaL_error(L, "Attempted to iterate dead Array<%s>", impl->GetTypeName());
			return 0;
		}

		if (lua_type(L, 2) == LUA_TNIL) {
			return impl->Next(L, 0);
		} else {
			auto key = get<int>(L, 2);
			return impl->Next(L, key);
		}
	}

	int ArrayProxy::ToString(lua_State* L)
	{
		StackCheck _(L, 1);
		char entityName[200];
		if (lifetime_.IsAlive(L)) {
			_snprintf_s(entityName, std::size(entityName) - 1, "Array<%s> (%p)", GetImpl()->GetTypeName(), GetImpl()->GetRaw());
		} else {
			_snprintf_s(entityName, std::size(entityName) - 1, "Array<%s> (%p, DEAD)", GetImpl()->GetTypeName(), GetImpl()->GetRaw());
		}

		push(L, entityName);
		return 1;
	}
}
