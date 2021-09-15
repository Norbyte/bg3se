#include <Lua/Shared/LuaEvent.h>

namespace bg3se::lua
{
	char const* const EventObject::MetatableName = "bg3se::Event";

	int EventObject::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to read dead event of type '%s'", impl->GetTypeName());
			push(L, nullptr);
			return 1;
		}

		auto prop = checked_get<FixedString>(L, 2);

		if (prop == GFS.strName) {
			push(L, eventName_);
		} else if (prop == GFS.strReadOnly) {
			push(L, !writeable_);
		} else if (prop == GFS.strCanPreventAction) {
			push(L, !canPreventAction_);
		} else if (!impl->GetProperty(L, lifetime_.Get(), prop)) {
			push(L, nullptr);
		}

		return 1;
	}

	int EventObject::NewIndex(lua_State* L)
	{
		StackCheck _(L, 0);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to write dead event of type '%s'", impl->GetTypeName());
			return 0;
		}

		auto prop = checked_get<FixedString>(L, 2);
		if (writeable_) {
			impl->SetProperty(L, lifetime_.Get(), prop, 3);
		} else {
			luaL_error(L, "Event '%s' is not writeable in this context (while attempting to write property %s.%s)",
				eventName_, impl->GetTypeName(), prop.GetString());
			return false;
		}

		return 0;
	}

	int EventObject::Next(lua_State* L)
	{
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to iterate dead event of type '%s'", impl->GetTypeName());
			return 0;
		}

		if (lua_type(L, 2) == LUA_TNIL) {
			return impl->Next(L, lifetime_.Get(), FixedString{});
		} else {
			auto key = checked_get<FixedString>(L, 2);
			return impl->Next(L, lifetime_.Get(), key);
		}
	}

	int EventObject::ToString(lua_State* L)
	{
		StackCheck _(L, 1);
		char entityName[200];
		if (lifetime_.IsAlive()) {
			_snprintf_s(entityName, std::size(entityName) - 1, "Event %s (%s)", eventName_, GetImpl()->GetTypeName());
		} else {
			_snprintf_s(entityName, std::size(entityName) - 1, "Event %s (%s, DEAD)", eventName_, GetImpl()->GetTypeName());
		}

		push(L, entityName);
		return 1;
	}

	int EventObject::GC(lua_State* L)
	{
		this->~EventObject();
		return 0;
	}
}
