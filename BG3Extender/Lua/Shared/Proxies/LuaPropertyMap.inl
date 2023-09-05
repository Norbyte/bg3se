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

PropertyOperationResult GenericPropertyMap::SetRawProperty(lua_State* L, LifetimeHandle const& lifetime, void* object, FixedString const& prop, int index) const
{
	auto it = Properties.find(prop);
	if (it == Properties.end()) {
		if (FallbackSetter) {
			return FallbackSetter(L, lifetime, object, prop, index);
		} else {
			return PropertyOperationResult::NoSuchProperty;
		}
	}

	return it->second.Set(L, lifetime, object, index, it->second.Offset, it->second.Flag);
}

void GenericPropertyMap::AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
	typename RawPropertyAccessors::Setter* setter, std::size_t offset, uint64_t flag)
{
	assert(!Initialized && IsInitializing);
	auto key = FixedString(prop);
	assert(Properties.find(key) == Properties.end());
	Properties.insert(std::make_pair(key, RawPropertyAccessors{ key, getter, setter, offset, flag }));

}

void GenericPropertyMap::AddRawValidator(char const* prop, typename RawPropertyValidators::Validator* validate, std::size_t offset, uint64_t flag)
{
	assert(!Initialized && IsInitializing);
	Validators.push_back(RawPropertyValidators{ FixedString(prop), validate, offset, flag });
}

void GenericPropertyMap::AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
	typename RawPropertyAccessors::Setter* setter, typename RawPropertyValidators::Validator* validate, std::size_t offset, uint64_t flag)
{
	if (getter != nullptr || setter != nullptr) {
		AddRawProperty(prop, getter, setter, offset, flag);
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

END_NS()
