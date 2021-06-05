#include <Lua/Shared/LuaSetProxy.h>

namespace bg3se::lua
{
	char const* const SetProxy::MetatableName = "bg3se::Set";

	int SetProxy::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to read dead Set<%s>", impl->GetTypeName());
			push(L, nullptr);
			return 1;
		}

		push(L, impl->HasElement(L, 2));
		return 1;
	}

	int SetProxy::NewIndex(lua_State* L)
	{
		StackCheck _(L, 0);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to write dead Set<%s>", impl->GetTypeName());
			return 0;
		}

		auto val = checked_get<bool>(L, 3);
		if (val) {
			impl->AddElement(L, 3);
		} else {
			impl->RemoveElement(L, 3);
		}

		return 0;
	}

	int SetProxy::Next(lua_State* L)
	{
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to iterate dead Set<%s>", impl->GetTypeName());
			return 0;
		}

		if (lua_type(L, 2) == LUA_TNIL) {
			return impl->Next(L, -1);
		} else {
			auto key = checked_get<int>(L, 2);
			return impl->Next(L, key);
		}
	}

	int SetProxy::ToString(lua_State* L)
	{
		StackCheck _(L, 1);
		char entityName[200];
		if (lifetime_.IsAlive()) {
			_snprintf_s(entityName, std::size(entityName) - 1, "Set<%s> (%p)", GetImpl()->GetTypeName(), GetImpl()->GetRaw());
		} else {
			_snprintf_s(entityName, std::size(entityName) - 1, "Set<%s> (%p, DEAD)", GetImpl()->GetTypeName(), GetImpl()->GetRaw());
		}

		push(L, entityName);
		return 1;
	}

	int SetProxy::GC(lua_State* L)
	{
		this->~SetProxy();
		return 0;
	}
}
