#include <Lua/Shared/Proxies/LuaCppObjectProxy.h>
#include <Extender/ScriptExtender.h>

BEGIN_NS(lua)

struct CppObjectProxyHelpers
{
	static int Next(lua_State* L, GenericPropertyMap const& pm, void* object, LifetimeHandle const& lifetime, FixedString const& key)
	{
		if (!key) {
			if (!pm.Properties.empty()) {
				StackCheck _(L, 2);
				auto it = pm.Properties.begin();
				push(L, it->first);
				if (pm.GetRawProperty(L, lifetime, object, it->first) != PropertyOperationResult::Success) {
					push(L, nullptr);
				}

				return 2;
			}
		} else {
			auto it = pm.Properties.find(key);
			if (it != pm.Properties.end()) {
				++it;
				if (it != pm.Properties.end()) {
					StackCheck _(L, 2);
					push(L, it->first);
					if (pm.GetRawProperty(L, lifetime, object, it->first) != PropertyOperationResult::Success) {
						push(L, nullptr);
					}

					return 2;
				}
			}
		}

		return 0;
	}
};

CppMetatableManager::CppMetatableManager()
{
	metatables_.resize((int)MetatableTag::Max + 1);
}

void CppMetatableManager::RegisterMetatable(MetatableTag tag, CMetatable* mt)
{
	assert(tag <= MetatableTag::Max);
	metatables_[(int)tag] = mt;
}

CMetatable* CppMetatableManager::GetMetatable(MetatableTag tag)
{
	assert(tag <= MetatableTag::Max);
	return metatables_[(int)tag];
}

CppMetatableManager& CppMetatableManager::FromLua(lua_State* L)
{
	return State::FromLua(L)->GetMetatableManager();
}


int LightObjectProxyByRefMetatable::Index(lua_State* L, CppObjectMetadata& self)
{
	auto pm = gExtender->GetPropertyMapManager().GetPropertyMap(self.PropertyMapTag);
	auto prop = get<FixedString>(L, 2);
	auto result = pm->GetRawProperty(L, self.Lifetime, self.Ptr, prop);
	switch (result) {
	case PropertyOperationResult::Success:
		break;

	case PropertyOperationResult::NoSuchProperty:
		luaL_error(L, "Property does not exist: %s::%s - property does not exist", GetTypeName(L, self), prop.GetString());
		push(L, nullptr);
		break;

	case PropertyOperationResult::Unknown:
	default:
		luaL_error(L, "Cannot get property %s::%s - unknown error", GetTypeName(L, self), prop.GetString());
		push(L, nullptr);
		break;
	}

	return 1;
}

int LightObjectProxyByRefMetatable::NewIndex(lua_State* L, CppObjectMetadata& self)
{
	auto pm = gExtender->GetPropertyMapManager().GetPropertyMap(self.PropertyMapTag);
	auto prop = get<FixedString>(L, 2);
	auto result = pm->SetRawProperty(L, self.Ptr, prop, 3);
	switch (result) {
	case PropertyOperationResult::Success:
		break;

	case PropertyOperationResult::NoSuchProperty:
		luaL_error(L, "Cannot set property %s::%s - property does not exist", GetTypeName(L, self), prop.GetString());
		break;

	case PropertyOperationResult::ReadOnly:
		luaL_error(L, "Cannot set property %s::%s - property is read-only", GetTypeName(L, self), prop.GetString());
		break;

	case PropertyOperationResult::UnsupportedType:
		luaL_error(L, "Cannot set property %s::%s - cannot write properties of this type", GetTypeName(L, self), prop.GetString());
		break;

	case PropertyOperationResult::Unknown:
	default:
		luaL_error(L, "Cannot set property %s::%s - unknown error", GetTypeName(L, self), prop.GetString());
		break;
	}

	return 0;
}

int LightObjectProxyByRefMetatable::ToString(lua_State* L, CppObjectMetadata& self)
{
	char entityName[200];
	if (self.Lifetime.IsAlive(L)) {
		_snprintf_s(entityName, std::size(entityName) - 1, "%s (%p)", GetTypeName(L, self), self.Ptr);
	} else {
		_snprintf_s(entityName, std::size(entityName) - 1, "%s (%p, DEAD REFERENCE)", GetTypeName(L, self), self.Ptr);
	}

	push(L, entityName);
	return 1;
}

bool LightObjectProxyByRefMetatable::IsEqual(lua_State* L, CppObjectMetadata& self, CppObjectMetadata& other)
{
	return self.Ptr == other.Ptr && self.PropertyMapTag == other.PropertyMapTag;
}

int LightObjectProxyByRefMetatable::Next(lua_State* L, CppObjectMetadata& self)
{
	auto pm = gExtender->GetPropertyMapManager().GetPropertyMap(self.PropertyMapTag);
	if (lua_type(L, 2) == LUA_TNIL) {
		return CppObjectProxyHelpers::Next(L, *pm, self.Ptr, self.Lifetime, FixedString{});
	} else {
		auto key = get<FixedString>(L, 2);
		return CppObjectProxyHelpers::Next(L, *pm, self.Ptr, self.Lifetime, key);
	}
}

char const* LightObjectProxyByRefMetatable::GetTypeName(lua_State* L, CppObjectMetadata& self)
{
	auto pm = gExtender->GetPropertyMapManager().GetPropertyMap(self.PropertyMapTag);
	return pm->Name.GetString();
}

END_NS()
