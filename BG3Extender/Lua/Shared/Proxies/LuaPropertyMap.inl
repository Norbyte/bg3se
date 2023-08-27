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
