#include <Lua/Shared/LuaObjectProxy.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se::lua
{

	bool GenericPropertyMap::GetRawProperty(lua_State* L, LifetimeHolder const& lifetime, void* object, FixedString const& prop) const
	{
		auto it = Properties.find(prop);
		if (it == Properties.end()) {
			return 0;
		}

		return it->second.Get(L, lifetime, object, it->second.Offset);
	}

	bool GenericPropertyMap::SetRawProperty(lua_State* L, LifetimeHolder const& lifetime, void* object, FixedString const& prop, int index) const
	{
		auto it = Properties.find(prop);
		if (it == Properties.end()) {
			return 0;
		}

		return it->second.Set(L, lifetime, object, index, it->second.Offset);
	}

	void GenericPropertyMap::AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
		typename RawPropertyAccessors::Setter* setter, std::size_t offset)
	{
		auto key = FixedString(prop);
		Properties.insert(std::make_pair(key, RawPropertyAccessors{ key, getter, setter, offset }));
	}


	char const* const ObjectProxy::MetatableName = "bg3se::Object";

	int ObjectProxy::Index(lua_State* L)
	{
		StackCheck _(L, 1);
		auto impl = GetImpl();
		if (!lifetime_.IsAlive()) {
			luaL_error(L, "Attempted to read dead object of type '%s'", impl->GetTypeName());
			push(L, nullptr);
			return 1;
		}

		auto prop = checked_get<FixedString>(L, 2);
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

		auto prop = checked_get<FixedString>(L, 2);
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
			return impl->Next(L, FixedString{});
		} else {
			auto key = checked_get<FixedString>(L, 2);
			return impl->Next(L, key);
		}
	}

	int ObjectProxy::ToString(lua_State* L)
	{
		StackCheck _(L, 1);
		char entityName[200];
		if (lifetime_.IsAlive()) {
			_snprintf_s(entityName, std::size(entityName) - 1, "%s (%p)", GetImpl()->GetTypeName().GetString(), GetImpl()->GetRaw());
		} else {
			_snprintf_s(entityName, std::size(entityName) - 1, "%s (%p, DEAD)", GetImpl()->GetTypeName().GetString(), GetImpl()->GetRaw());
		}

		push(L, entityName);
		return 1;
	}

	int ObjectProxy::GC(lua_State* L)
	{
		this->~ObjectProxy();
		return 0;
	}


	int ComponentGetReplicationFlags(lua_State* L, BaseComponent* self)
	{
		uint32_t qword{ 0 };
		if (lua_gettop(L) > 1) {
			qword = checked_get<uint32_t>(L, 2);
		}

		auto helpers = gExtender->GetCurrentExtensionState()->GetLua()->GetEntitySystemHelpers();
		auto flags = helpers->GetReplicationFlags(*self);
		if (flags) {
			if (qword < flags->NumQwords()) {
				push(L, flags->GetBuf()[qword]);
			} else {
				push(L, 0ull);
			}
		} else {
			push(L, 0ull);
		}

		return 1;
	}

	void ComponentSetReplicationFlag(BaseComponent* component, uint32_t qword, uint64_t flags)
	{
		auto helpers = gExtender->GetCurrentExtensionState()->GetLua()->GetEntitySystemHelpers();
		auto curReplicationFlags = helpers->GetOrCreateReplicationFlags(*component);
		if (curReplicationFlags) {
			curReplicationFlags->EnsureSize((qword + 1) * 64);
			bool changed = (curReplicationFlags->GetBuf()[qword] & flags) != flags;
			curReplicationFlags->GetBuf()[qword] |= flags;
			if (changed) {
				helpers->NotifyReplicationFlagsDirtied();
			}
		} else {
			OsiError("Couldn't add entity notification?");
		}
	}

	int ComponentSetReplicationFlag(lua_State* L, BaseComponent* self)
	{
		auto flags = checked_get<uint64_t>(L, 2);

		uint32_t qword{ 0 };
		if (lua_gettop(L) > 2) {
			qword = checked_get<uint32_t>(L, 3);
		}

		ComponentSetReplicationFlag(self, qword, flags);
		return 0;
	}

	int ComponentReplicate(lua_State* L, BaseComponent* self)
	{
		ComponentSetReplicationFlag(self, 0, 0xffffffffffffffffull);
		return 0;
	}
}
