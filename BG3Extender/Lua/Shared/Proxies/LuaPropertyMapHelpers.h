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
PropertyOperationResult GenericGetValue(lua_State* L, LifetimeHandle lifetime, void const* obj, RawPropertyAccessorsHotData const& prop)
{
    auto value = reinterpret_cast<T const*>(obj);
    push(L, *value, lifetime);
    return PropertyOperationResult::Success;
}

template <class T>
PropertyOperationResult GenericSerializeValue(lua_State* L, void const* value, RawPropertyAccessors const& prop)
{
    static_assert(!std::is_pointer_v<T>);
    Serialize(L, reinterpret_cast<T const*>(value));
    return PropertyOperationResult::Success;
}

template <class T>
PropertyOperationResult GenericUnserializeValue(lua_State* L, void* valuePtr, int index, RawPropertyAccessors const& prop)
{
    if constexpr (!std::is_pointer_v<T>) {
        auto value = reinterpret_cast<T*>(valuePtr);
        return Unserialize(L, index, value);
    } else {
        return PropertyOperationResult::UnsupportedType;
    }
}

template <class T>
PropertyOperationResult GenericSetValue(lua_State* L, void* valuePtr, int index, RawPropertyAccessorsHotData const& prop)
{
    auto value = reinterpret_cast<T*>(valuePtr);
    if constexpr (IsByVal<T>) {
        *value = get<T>(L, index);
        return PropertyOperationResult::Success;
    } else {
        return GenericUnserializeValue<T>(L, value, index, *prop.GetCold());
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
bool GenericValidateOffsetProperty(void const* value, uint64_t flag)
{
    return ValidateAny(reinterpret_cast<T const*>(value));
}

void InheritProperties(GenericPropertyMap const& base, GenericPropertyMap& child);

END_NS()
