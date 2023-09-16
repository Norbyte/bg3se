#include <Lua/Shared/Proxies/LuaPropertyMap.h>

BEGIN_NS(lua)

void GenericPropertyMap::Init(int registryIndex)
{
	assert(!IsInitializing && !Initialized);
	IsInitializing = true;
	RegistryIndex = registryIndex;
}

void GenericPropertyMap::Finish()
{
	assert(!Initialized && IsInitializing);
	IsInitializing = false;
	Initialized = true;
}

bool GenericPropertyMap::HasProperty(FixedString const& prop) const
{
	return Properties.find(prop) != Properties.end();
}

PropertyOperationResult GenericPropertyMap::GetRawProperty(lua_State* L, LifetimeHandle const& lifetime, void* object, FixedString const& prop) const
{
	auto it = Properties.find(prop);
	if (it == Properties.end()) {
		if (FallbackGetter) {
			return FallbackGetter(L, lifetime, object, prop);
		} else {
			return PropertyOperationResult::NoSuchProperty;
		}
	}

	return it->second.Get(L, lifetime, object, it->second.Offset, it->second.Flag);
}

PropertyOperationResult GenericPropertyMap::SetRawProperty(lua_State* L, void* object, FixedString const& prop, int index) const
{
	auto it = Properties.find(prop);
	if (it == Properties.end()) {
		if (FallbackSetter) {
			return FallbackSetter(L, object, prop, index);
		} else {
			return PropertyOperationResult::NoSuchProperty;
		}
	}

	return it->second.Set(L, object, index, it->second.Offset, it->second.Flag);
}

void GenericPropertyMap::AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
	typename RawPropertyAccessors::Setter* setter, typename RawPropertyAccessors::Serializer* serialize, 
	typename RawPropertyAccessors::Unserializer* unserialize, std::size_t offset, uint64_t flag)
{
	assert(!Initialized && IsInitializing);
	auto key = FixedString(prop);
	assert(Properties.find(key) == Properties.end());
	Properties.insert(std::make_pair(key, RawPropertyAccessors{ key, offset, flag, getter, setter, serialize, unserialize }));

}

void GenericPropertyMap::AddRawValidator(char const* prop, typename RawPropertyValidators::Validator* validate, std::size_t offset, uint64_t flag)
{
	assert(!Initialized && IsInitializing);
	Validators.push_back(RawPropertyValidators{ FixedString(prop), validate, offset, flag });
}

void GenericPropertyMap::AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
	typename RawPropertyAccessors::Setter* setter, typename RawPropertyValidators::Validator* validate, 
	typename RawPropertyAccessors::Serializer* serialize, typename RawPropertyAccessors::Unserializer* unserialize, 
	std::size_t offset, uint64_t flag)
{
	if (getter != nullptr || setter != nullptr) {
		AddRawProperty(prop, getter, setter, serialize, unserialize, offset, flag);
	}

	if (validate != nullptr) {
		AddRawValidator(prop, validate, offset, flag);
	}
}

bool GenericPropertyMap::ValidatePropertyMap(void* object)
{
	if (Validated == ValidationState::Unknown) {
		if (ValidateObject(object)) {
			Validated = ValidationState::Valid;
		} else {
			ERR("Object class %s failed validation; proxying of this class is disabled", Name.GetString());
			Validated = ValidationState::Invalid;
		}
	}

	return Validated == ValidationState::Valid;
}

bool GenericPropertyMap::ValidateObject(void* object)
{
	for (auto const& property : Validators) {
		if (!property.Validate(object, property.Offset, property.Flag)) {
			ERR("Validation of property '%s' failed on %s %p", property.Name.GetString(), Name.GetString(), object);
			return false;
		}
	}

	return true;
}

bool GenericPropertyMap::IsA(int typeRegistryIndex) const
{
	if (RegistryIndex == typeRegistryIndex) {
		return true;
	}

	for (auto const& parent : ParentRegistryIndices) {
		if (parent == typeRegistryIndex) {
			return true;
		}
	}

	return false;
}

void SerializeRawObject(lua_State* L, void* obj, GenericPropertyMap const& pm)
{
	StackCheck _(L, 1);
	lua_createtable(L, 0, (int)pm.Properties.size());
	for (auto const& prop : pm.Properties) {
		prop.second.Serialize(L, obj, prop.second.Offset, prop.second.Flag);
		lua_setfield(L, -2, prop.first.GetString());
	}
}

void UnserializeRawObject(lua_State* L, int index, void* obj, GenericPropertyMap const& pm)
{
	StackCheck _(L);
	for (auto const& prop : pm.Properties) {
		lua_getfield(L, index, prop.first.GetString());
		if (lua_type(L, -1) != LUA_TNIL) {
			prop.second.Unserialize(L, obj, lua_absindex(L, -1), prop.second.Offset, prop.second.Flag);
		}
		lua_pop(L, 1);
	}

}

END_NS()
