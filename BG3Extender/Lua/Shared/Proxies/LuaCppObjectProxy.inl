#include <Lua/Shared/Proxies/LuaCppObjectProxy.h>
#include <Extender/ScriptExtender.h>

BEGIN_NS(lua)

int CppObjectProxyHelpers::Next(lua_State* L, GenericPropertyMap const& pm, void* object, LifetimeHandle const& lifetime, FixedString const& key)
{
	if (!key) {
		if (!pm.IterableProperties.empty()) {
			StackCheck _(L, 2);
			auto it = pm.IterableProperties.begin();
			push(L, it.Key());
			auto const& prop = pm.Properties.values()[it.Value()];
			if (pm.GetRawProperty(L, lifetime, object, prop) != PropertyOperationResult::Success) {
				push(L, nullptr);
			}

			return 2;
		}
	} else {
		auto it = pm.IterableProperties.find(key);
		if (it != pm.IterableProperties.end()) {
			++it;
			if (it != pm.IterableProperties.end()) {
				StackCheck _(L, 2);
				push(L, it.Key());
				auto const& prop = pm.Properties.values()[it.Value()];
				if (pm.GetRawProperty(L, lifetime, object, prop) != PropertyOperationResult::Success) {
					push(L, nullptr);
				}

				return 2;
			}
		}
	}

	if (pm.FallbackNext) {
		return pm.FallbackNext(L, lifetime, object, key);
	}

	return 0;
}

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

void* ObjectProxy::GetRaw(lua_State* L, int index, GenericPropertyMap const& pm)
{
	CppObjectMetadata meta;
	lua_get_cppobject(L, index, meta);

	if (meta.MetatableTag == MetatableTag::ImguiObject) {
		// Temporary jank to support imgui objects
		auto obj = ImguiObjectProxyMetatable::GetGeneric(L, index);
		auto& objPm = obj->GetRTTI();
		if (!objPm.IsA(pm.RegistryIndex)) {
			luaL_error(L, "Argument %d: Expected object of type '%s', got '%s'", index,
				pm.Name.GetString(), objPm.Name.GetString());
		}

		return obj;
	} else {

		auto& objPm = *gStructRegistry.Get(meta.PropertyMapTag);
		if (!objPm.IsA(pm.RegistryIndex)) {
			luaL_error(L, "Argument %d: Expected object of type '%s', got '%s'", index,
				pm.Name.GetString(), objPm.Name.GetString());
		}

		if (!meta.Lifetime.IsAlive(L)) {
			luaL_error(L, "Attempted to fetch '%s' whose lifetime has expired", pm.Name.GetString());
		}

		return meta.Ptr;
	}
}

GenericPropertyMap& LightObjectProxyMetatable::GetPropertyMap(CppObjectMetadata const& meta)
{
	assert(meta.MetatableTag == MetaTag);
	return *gStructRegistry.Get(meta.PropertyMapTag);
}


void* LightObjectProxyMetatable::TryGetGeneric(lua_State* L, int index, int propertyMapIndex)
{
	CppObjectMetadata meta;
	if (lua_try_get_cppobject(L, index, MetaTag, meta)) {
		auto& pm = *gStructRegistry.Get(meta.PropertyMapTag);
		if (pm.IsA(meta.PropertyMapTag) && meta.Lifetime.IsAlive(L)) {
			return meta.Ptr;
		}
	}

	return nullptr;
}

void* LightObjectProxyMetatable::GetGeneric(lua_State* L, int index, int propertyMapIndex)
{
	CppObjectMetadata meta;
	if (lua_try_get_cppobject(L, index, meta)) {
		auto& pm = *gStructRegistry.Get(meta.PropertyMapTag);
		if (pm.IsA(propertyMapIndex)) {
			if (!meta.Lifetime.IsAlive(L)) {
				luaL_error(L, "Attempted to fetch '%s' whose lifetime has expired", GetTypeName(L, meta));
				return 0;
			}

			return meta.Ptr;
		} else {
			luaL_error(L, "Argument %d: Expected object of type '%s', got '%s'", index,
				gStructRegistry.Get(propertyMapIndex)->Name.GetString(),
				pm.Name.GetString());
			return nullptr;
		}
	} else {
		luaL_error(L, "Argument %d: Expected object of type '%s', got '%s'", index,
			gStructRegistry.Get(propertyMapIndex)->Name.GetString(),
			lua_typename(L, lua_type(L, index)));
		return nullptr;
	}
}

int LightObjectProxyMetatable::Index(lua_State* L, CppObjectMetadata& self)
{
	auto pm = gStructRegistry.Get(self.PropertyMapTag);
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

int LightObjectProxyMetatable::NewIndex(lua_State* L, CppObjectMetadata& self)
{
	auto pm = gStructRegistry.Get(self.PropertyMapTag);
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

int LightObjectProxyMetatable::ToString(lua_State* L, CppObjectMetadata& self)
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

bool LightObjectProxyMetatable::IsEqual(lua_State* L, CppObjectMetadata& self, CppObjectMetadata& other)
{
	return self.Ptr == other.Ptr && self.PropertyMapTag == other.PropertyMapTag;
}

int LightObjectProxyMetatable::Next(lua_State* L, CppObjectMetadata& self)
{
	auto pm = gStructRegistry.Get(self.PropertyMapTag);
	if (lua_type(L, 2) == LUA_TNIL) {
		return CppObjectProxyHelpers::Next(L, *pm, self.Ptr, self.Lifetime, FixedString{});
	} else {
		auto key = get<FixedString>(L, 2);
		return CppObjectProxyHelpers::Next(L, *pm, self.Ptr, self.Lifetime, key);
	}
}

char const* LightObjectProxyMetatable::GetTypeName(lua_State* L, CppObjectMetadata& self)
{
	auto pm = gStructRegistry.Get(self.PropertyMapTag);
	return pm->Name.GetString();
}

END_NS()
