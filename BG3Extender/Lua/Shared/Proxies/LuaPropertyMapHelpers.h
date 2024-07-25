#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <Lua/Shared/LuaTypeValidators.h>
#include <GameDefinitions/RootTemplates.h>

BEGIN_NS(lua)

void ProcessPropertyNotifications(RawPropertyAccessors const& prop, bool isWriting);
void DisablePropertyWarnings();
void EnablePropertyWarnings();

template <class T>
PropertyOperationResult GenericGetOffsetProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, RawPropertyAccessors const& prop)
{
	if (prop.PendingNotifications != PropertyNotification::None) [[unlikely]] {
		ProcessPropertyNotifications(prop, false);
	}

	auto* value = (T*)((std::uintptr_t)obj + prop.Offset);
	push(L, value, lifetime);
	return PropertyOperationResult::Success;
}

template <class T>
PropertyOperationResult GenericSerializeOffsetProperty(lua_State* L, void const* obj, RawPropertyAccessors const& prop)
{
	auto* value = (T*)((std::uintptr_t)obj + prop.Offset);
	if constexpr (!std::is_pointer_v<T>) {
		// FIXME FIXME FIXME FIXME FIXME
		// FIXME FIXME FIXME FIXME FIXME
		// FIXME FIXME FIXME FIXME FIXME
		// The top-level serializer needs to default-construct a temporary (or do placement new)
		// to ensure that the new object starts out from a known good state.
		Serialize(L, value);
		return PropertyOperationResult::Success;
	} else {
		return PropertyOperationResult::UnsupportedType;
	}
}

template <class T>
PropertyOperationResult GenericUnserializeOffsetProperty(lua_State* L, void* obj, int index, RawPropertyAccessors const& prop)
{
	auto* value = (T*)((std::uintptr_t)obj + prop.Offset);
	if constexpr (!std::is_pointer_v<T>) {
		return Unserialize(L, index, value);
	} else {
		return PropertyOperationResult::UnsupportedType;
	}
}

template <class T>
PropertyOperationResult GenericSetOffsetProperty(lua_State* L, void* obj, int index, RawPropertyAccessors const& prop)
{
	if (prop.PendingNotifications != PropertyNotification::None) [[unlikely]] {
		ProcessPropertyNotifications(prop, true);
	}

	if constexpr (IsByVal<T>) {
		auto* value = (T*)((std::uintptr_t)obj + prop.Offset);
		*value = get<T>(L, index);
		return PropertyOperationResult::Success;
	} else {
		return GenericUnserializeOffsetProperty<T>(L, obj, index, prop);
	}
}

template <class UnderlyingType>
PropertyOperationResult GenericGetOffsetBitmaskFlag(lua_State* L, LifetimeHandle const& lifetime, void* obj, RawPropertyAccessors const& prop)
{
	auto value = *(UnderlyingType*)((std::uintptr_t)obj + prop.Offset);
	push(L, (value & (UnderlyingType)prop.Flag) == (UnderlyingType)prop.Flag);
	return PropertyOperationResult::Success;
}

template <class UnderlyingType>
PropertyOperationResult GenericSetOffsetBitmaskFlag(lua_State* L, void* obj, int index, RawPropertyAccessors const& prop)
{
	auto* value = (UnderlyingType*)((std::uintptr_t)obj + prop.Offset);
	auto set = get<bool>(L, index);
	if (set) {
		*value |= (UnderlyingType)prop.Flag;
	} else {
		*value &= (UnderlyingType)~prop.Flag;
	}

	return PropertyOperationResult::Success;
}

template <class T>
bool GenericValidateOffsetProperty(void const* obj, std::size_t offset, uint64_t flag)
{
	auto value = reinterpret_cast<T const*>((std::uintptr_t)obj + offset);
	return ValidateAny(value);
}

void InheritProperties(GenericPropertyMap const& base, GenericPropertyMap& child);

END_NS()
