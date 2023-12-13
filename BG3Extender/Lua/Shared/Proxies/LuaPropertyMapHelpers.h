#pragma once

#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <Lua/Shared/LuaTypeValidators.h>
#include <GameDefinitions/RootTemplates.h>

BEGIN_NS(lua)

template <class T>
PropertyOperationResult GenericGetOffsetProperty(lua_State* L, LifetimeHandle const& lifetime, void* obj, std::size_t offset, uint64_t)
{
	auto* value = (T*)((std::uintptr_t)obj + offset);
	push(L, value, lifetime);
	return PropertyOperationResult::Success;
}

template <class T>
PropertyOperationResult GenericSerializeOffsetProperty(lua_State* L, void* obj, std::size_t offset, uint64_t flag)
{
	auto* value = (T*)((std::uintptr_t)obj + offset);
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
PropertyOperationResult GenericUnserializeOffsetProperty(lua_State* L, void* obj, int index, std::size_t offset, uint64_t flag)
{
	auto* value = (T*)((std::uintptr_t)obj + offset);
	if constexpr (!std::is_pointer_v<T>) {
		return Unserialize(L, index, value);
	} else {
		return PropertyOperationResult::UnsupportedType;
	}
}

template <class T>
PropertyOperationResult GenericSetOffsetProperty(lua_State* L, void* obj, int index, std::size_t offset, uint64_t flag)
{
	if constexpr (IsByVal<T>) {
		auto* value = (T*)((std::uintptr_t)obj + offset);
		*value = get<T>(L, index);
		return PropertyOperationResult::Success;
	} else {
		return GenericUnserializeOffsetProperty<T>(L, obj, index, offset, flag);
	}
}

template <class UnderlyingType>
PropertyOperationResult GenericGetOffsetBitmaskFlag(lua_State* L, LifetimeHandle const& lifetime, void* obj, std::size_t offset, uint64_t flag)
{
	auto value = *(UnderlyingType*)((std::uintptr_t)obj + offset);
	push(L, (value & (UnderlyingType)flag) == (UnderlyingType)flag);
	return PropertyOperationResult::Success;
}

template <class UnderlyingType>
PropertyOperationResult GenericSetOffsetBitmaskFlag(lua_State* L, void* obj, int index, std::size_t offset, uint64_t flag)
{
	auto* value = (UnderlyingType*)((std::uintptr_t)obj + offset);
	auto set = get<bool>(L, index);
	if (set) {
		*value |= (UnderlyingType)flag;
	} else {
		*value &= (UnderlyingType)~flag;
	}

	return PropertyOperationResult::Success;
}

template <class T>
bool GenericValidateOffsetProperty(void* obj, std::size_t offset, uint64_t flag)
{
	auto value = reinterpret_cast<T*>((std::uintptr_t)obj + offset);
	return ValidateAny(value);
}

void InheritProperties(GenericPropertyMap const& base, GenericPropertyMap& child);

template <class T, class T2>
inline void MarkAsInherited(LuaPropertyMap<T> const& base, LuaPropertyMap<T2>& child)
{
	static_assert(std::is_base_of_v<T, T2>, "Can only copy properties from base class");
	static_assert(static_cast<T*>(reinterpret_cast<T2*>(nullptr)) == reinterpret_cast<T*>(nullptr), "Base and child class should start at same base ptr");

	assert(!child.InheritanceUpdated);
	assert(child.IsInitializing);
	assert(child.Parent == nullptr);

	child.Parent = &base;
}

END_NS()
