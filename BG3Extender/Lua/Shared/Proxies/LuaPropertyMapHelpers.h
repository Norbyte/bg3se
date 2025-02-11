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
PropertyOperationResult GenericGetOffsetProperty(lua_State* L, LifetimeHandle lifetime, void const* obj, RawPropertyAccessorsHotData const& prop)
{
    auto value = reinterpret_cast<T const*>(obj);
    push(L, *value, lifetime);
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
PropertyOperationResult GenericSetOffsetProperty(lua_State* L, void* obj, int index, RawPropertyAccessorsHotData const& prop)
{
    if constexpr (IsByVal<T>) {
        auto value = reinterpret_cast<T*>(obj);
        *value = get<T>(L, index);
        return PropertyOperationResult::Success;
    } else {
        // TEMP HACK - undo offset math done by caller until Unserialize uses offset properties
        auto origObj = reinterpret_cast<uint8_t*>(obj) - prop.Offset();
        return GenericUnserializeOffsetProperty<T>(L, obj, index, *prop.Cold);
    }
}

template <class UnderlyingType>
PropertyOperationResult GenericGetOffsetBitmaskFlag(lua_State* L, LifetimeHandle lifetime, void const* obj, RawPropertyAccessorsHotData const& prop)
{
    auto value = *reinterpret_cast<UnderlyingType const*>(obj);
    auto flag = (UnderlyingType)prop.Flag();
    push(L, (value & flag) == flag);
    return PropertyOperationResult::Success;
}

template <class UnderlyingType>
PropertyOperationResult GenericSetOffsetBitmaskFlag(lua_State* L, void* obj, int index, RawPropertyAccessorsHotData const& prop)
{
    auto value = reinterpret_cast<UnderlyingType*>(obj);
    auto flag = (UnderlyingType)prop.Flag();
    auto set = get<bool>(L, index);
    if (set) {
        *value |= flag;
    } else {
        *value &= ~flag;
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
