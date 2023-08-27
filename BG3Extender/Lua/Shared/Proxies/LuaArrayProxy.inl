#include <Lua/Shared/Proxies/LuaArrayProxy.h>

namespace bg3se
{
#define BY_VAL_ARRAY_HELPERS(ty) char const* const TypeInfo<ty>::TypeName = #ty;

	BY_VAL_ARRAY_HELPERS(uint8_t);
	BY_VAL_ARRAY_HELPERS(int16_t);
	BY_VAL_ARRAY_HELPERS(uint16_t);
	BY_VAL_ARRAY_HELPERS(int32_t);
	BY_VAL_ARRAY_HELPERS(uint32_t);
	BY_VAL_ARRAY_HELPERS(int64_t);
	BY_VAL_ARRAY_HELPERS(uint64_t);
	BY_VAL_ARRAY_HELPERS(float);
	BY_VAL_ARRAY_HELPERS(double);
	BY_VAL_ARRAY_HELPERS(bool);
	BY_VAL_ARRAY_HELPERS(ComponentHandle);
	BY_VAL_ARRAY_HELPERS(EntityHandle);
	BY_VAL_ARRAY_HELPERS(ecs::EntityRef);
	BY_VAL_ARRAY_HELPERS(FixedString);
	BY_VAL_ARRAY_HELPERS(STDString);
	BY_VAL_ARRAY_HELPERS(STDWString);
	BY_VAL_ARRAY_HELPERS(Path);
	BY_VAL_ARRAY_HELPERS(Guid);
	BY_VAL_ARRAY_HELPERS(NetId);
	BY_VAL_ARRAY_HELPERS(UserId);
	BY_VAL_ARRAY_HELPERS(glm::ivec2);
	BY_VAL_ARRAY_HELPERS(glm::vec2);
	BY_VAL_ARRAY_HELPERS(glm::vec3);
	BY_VAL_ARRAY_HELPERS(glm::vec4);
	BY_VAL_ARRAY_HELPERS(glm::mat3);
}

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

	int ArrayProxy::GC(lua_State* L)
	{
		this->~ArrayProxy();
		return 0;
	}
}
