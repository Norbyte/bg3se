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
			luaL_error(L, "Attempted to read dead object of type '%s'", impl->GetTypeName());
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
			luaL_error(L, "Attempted to write dead object of type '%s'", impl->GetTypeName());
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
			luaL_error(L, "Attempted to iterate dead object of type '%s'", impl->GetTypeName());
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

	int ObjectProxy::GC(lua_State* L)
	{
		this->~ObjectProxy();
		return 0;
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
			luaL_error(L, "Argument %d: expected an object of type '%s', got '%s'", index, typeName.GetString(), proxy->GetImpl()->GetTypeName());
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


	int ComponentGetReplicationFlags(lua_State* L, BaseComponent* self)
	{
		uint32_t qword{ 0 };
		if (lua_gettop(L) > 1) {
			qword = get<uint32_t>(L, 2);
		}

		ERR("FIXME");
		/*auto helpers = gExtender->GetCurrentExtensionState()->GetLua()->GetEntitySystemHelpers();
		auto flags = helpers->GetReplicationFlags(*self);
		if (flags) {
			if (qword < flags->NumQwords()) {
				push(L, flags->GetBuf()[qword]);
			} else {
				push(L, 0ull);
			}
		} else {
			push(L, 0ull);
		}*/

		push(L, 0ull);
		return 1;
	}

	void ComponentSetReplicationFlag(BaseComponent* component, uint32_t qword, uint64_t flags)
	{
		ERR("FIXME");
		/*auto helpers = gExtender->GetCurrentExtensionState()->GetLua()->GetEntitySystemHelpers();
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
		}*/
	}

	int ComponentSetReplicationFlag(lua_State* L, BaseComponent* self)
	{
		auto flags = get<uint64_t>(L, 2);

		uint32_t qword{ 0 };
		if (lua_gettop(L) > 2) {
			qword = get<uint32_t>(L, 3);
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
