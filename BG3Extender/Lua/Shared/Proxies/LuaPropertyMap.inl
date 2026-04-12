#include <Lua/Shared/Proxies/LuaPropertyMap.h>

BEGIN_NS(lua)

StructRegistry gStructRegistry;

void StructRegistry::Initialize(int32_t size)
{
    se_assert(StructsById.empty());
    Validated.EnsureSize(size);
    StructsById.resize(size);
    Validated.Clear();
}

void StructRegistry::Register(GenericPropertyMap* pm, StructTypeId id)
{
    se_assert(pm->IsInitializing);
    se_assert((uint32_t)id < StructsById.size());
    se_assert(StructsById[(int32_t)id] == nullptr);

    pm->RegistryIndex = id;
    StructsById[(int32_t)id] = pm;
}

void GenericPropertyMap::Init()
{
    se_assert(!IsInitializing && !Initialized);
    IsInitializing = true;
}

void GenericPropertyMap::Finish()
{
    se_assert(!Initialized && IsInitializing);
    IsInitializing = false;
    Initialized = true;
}

void GenericPropertyMap::BuildHotPropertyMap()
{
    se_assert(InheritanceUpdated);
    
    Array<FixedStringUnhashed> keys;
    Array<lua::RawPropertyAccessorsHotData> hotProps;
    for (auto const& prop : Properties) {
        keys.push_back(prop.Key());
        hotProps.push_back(lua::RawPropertyAccessorsHotData(prop.Value()));
    }

    PropertiesHot.Build(keys, hotProps);

    // Lexicographically sort IterableProperties
    Array<FixedStringUnhashed> iterableKeys = IterableProperties.keys();
    std::sort(iterableKeys.begin(), iterableKeys.end(), [] (FixedStringUnhashed const& a, FixedStringUnhashed const& b) {
        return strcmp(a.GetString(), b.GetString()) < 0;
    });

    IterableProperties.clear();
    for (auto const& key : iterableKeys) {
        IterableProperties.set(key, Properties.find_index(key));
    }
}

bool GenericPropertyMap::HasProperty(FixedStringId const& prop) const
{
    auto& ah = static_cast<FixedStringUnhashed const&>(prop);
    return Properties.find(ah) != Properties.end();
}

PropertyOperationResult GenericPropertyMap::GetRawProperty(lua_State* L, LifetimeHandle lifetime, void const* object, FixedStringId const& prop) const
{
    auto& ah = static_cast<FixedStringUnhashed const&>(prop);
    auto const& entry = PropertiesHot.get(ah);

    auto result = entry.Get(L, lifetime, object);

    if (result == PropertyOperationResult::NoSuchProperty) {
        if (FallbackGetter) {
            result = FallbackGetter(L, lifetime, object, ah);
        }

        if (result == PropertyOperationResult::NoSuchProperty) {
            result = State::FromLua(L)->GetCustomProperties().GetProperty(L, *this, lifetime, object, prop);
        }
    }

    return result;
}

PropertyOperationResult GenericPropertyMap::GetRawProperty(lua_State* L, LifetimeHandle lifetime, void const* object, RawPropertyAccessors const& prop) const
{
    auto data = static_cast<uint8_t const*>(object) + prop.Offset;
    return prop.Get(L, lifetime, data, prop);
}

PropertyOperationResult GenericPropertyMap::SetRawProperty(lua_State* L, void* object, FixedStringId const& prop, int index) const
{
    auto& ah = static_cast<FixedStringUnhashed const&>(prop);
    auto const& entry = PropertiesHot.get(ah);

    auto result = entry.Set(L, index, object);

    if (result == PropertyOperationResult::NoSuchProperty) {
        if (FallbackSetter) {
            result = FallbackSetter(L, object, ah, index);
        }

        if (result == PropertyOperationResult::NoSuchProperty) {
            result = State::FromLua(L)->GetCustomProperties().SetProperty(L, *this, object, prop, index);
        }
    }

    return result;
}

PropertyOperationResult GenericPropertyMap::SetRawProperty(lua_State* L, void* object, RawPropertyAccessors const& prop, int index) const
{
    auto data = static_cast<uint8_t*>(object) + prop.Offset;
    return prop.Set(L, data, index, prop);
}

void GenericPropertyMap::AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
    typename RawPropertyAccessors::Setter* setter, typename RawPropertyAccessors::Serializer* serialize, 
    std::size_t offset, uint64_t flag, PropertyNotification notification, char const* newName, bool iterable, bool inherited)
{
    se_assert((!Initialized || !InheritanceUpdated) && IsInitializing);
    FixedStringUnhashed key{ prop };
    FixedString newNameKey{ newName ? newName : "" };
    
    auto existingProp = Properties.try_get(key);
    if (existingProp) {
        if (inherited) {
#if defined(_DEBUG)
            WARN("Property '%s' defined in type '%s' shadows same property defined in parent type", prop, Name.GetString());
#endif
            return;
        } else {
            ERR("Property '%s' defined in type '%s' shadows same property defined in parent type", prop, Name.GetString());
            se_assert(false);
        }
    }

    Properties.set(key, RawPropertyAccessors{ key, offset, flag, getter, setter, serialize, notification, this, newNameKey, iterable });

    if (iterable) {
        IterableProperties.set(key, Properties.size() - 1);
    }
}

void GenericPropertyMap::AddRawValidator(char const* prop, typename RawPropertyValidators::Validator* validate, std::size_t offset, 
    uint64_t flag)
{
    se_assert((!Initialized || !InheritanceUpdated) && IsInitializing);
    Validators.push_back(RawPropertyValidators{ FixedString(prop), validate, offset, flag });
}

void GenericPropertyMap::AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
    typename RawPropertyAccessors::Setter* setter, typename RawPropertyValidators::Validator* validate, 
    typename RawPropertyAccessors::Serializer* serialize, std::size_t offset, uint64_t flag, 
    PropertyNotification notification, char const* newName, bool iterable, bool inherited)
{
    if (getter != nullptr || setter != nullptr) {
        AddRawProperty(prop, getter, setter, serialize, offset, flag, notification, newName, iterable, inherited);
    }

    if (validate != nullptr) {
        AddRawValidator(prop, validate, offset, flag);
    }
}

bool GenericPropertyMap::ValidatePropertyMap(void const* object)
{
    switch (ecs::EntitySystemHelpersBase::CheckLevel) {
    case ecs::RuntimeCheckLevel::None: 
        return true;

    case ecs::RuntimeCheckLevel::Once:
        if (Validated == ValidationState::Unknown) {
            if (ValidateObject(object)) {
                Validated = ValidationState::Valid;
                se_assert((int32_t)RegistryIndex >= 0);
                gStructRegistry.Validated.Set((uint32_t)RegistryIndex);
            } else {
                ERR("Object class %s failed validation; proxying of this class is disabled", Name.GetString());
                Validated = ValidationState::Invalid;
            }
        }

        return Validated == ValidationState::Valid;

    case ecs::RuntimeCheckLevel::Always:
    case ecs::RuntimeCheckLevel::FullECS:
    default:
        return ValidateObject(object);
    }
}

bool GenericPropertyMap::ValidateObject(void const* object)
{
    for (auto const& property : Validators) {
        auto value = reinterpret_cast<void const*>((std::uintptr_t)object + property.Offset);
        if (!property.Validate(value, property.Flag)) {
            ERR("Validation of property '%s' failed on %s %p", property.Name.GetString(), Name.GetString(), object);
            return false;
        }
    }

    return true;
}

bool GenericPropertyMap::IsA(StructTypeId typeRegistryIndex) const
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

CustomPropertyManager::CustomPropertyManager()
{
    PropertyMaps.resize(gStructRegistry.StructsById.size());
}

bool CustomPropertyManager::PropertyCheck(GenericPropertyMap const& pm, FixedStringUnhashed const& name)
{
    if (!PropertyMaps[(int32_t)pm.RegistryIndex]) {
        PropertyMaps[(int32_t)pm.RegistryIndex] = std::make_unique<CustomPropertyMap>();
    }

    auto prop = pm.Properties.find(name);
    if (prop) {
        LuaError("Type " << pm.Name << " already has a property named '" << name << "'");
        return false;
    }

    auto& customProps = PropertyMaps[(int32_t)pm.RegistryIndex];
    auto customProp = customProps->Properties.try_get(name);
    if (customProp) {
        LuaError("Type " << pm.Name << " already has a custom property named '" << name << "'");
        return false;
    }

    return true;
}

bool CustomPropertyManager::RegisterProperty(lua_State* L, GenericPropertyMap const& pm, FixedStringUnhashed const& name, Ref value)
{
    if (!PropertyCheck(pm, name)) {
        return false;
    }

    auto& customProps = PropertyMaps[(uint32_t)pm.RegistryIndex];
    customProps->Properties.set(name, CustomProperty{
        .Value = RegistryEntry(L, value)
    });
    return true;
}

bool CustomPropertyManager::RegisterProperty(lua_State* L, GenericPropertyMap const& pm, FixedStringUnhashed const& name, Ref getter, Ref setter)
{
    if (!PropertyCheck(pm, name)) {
        return false;
    }

    auto& customProps = PropertyMaps[(uint32_t)pm.RegistryIndex];
    customProps->Properties.set(name, CustomProperty{
        .Getter = RegistryEntry(L, getter),
        .Setter = RegistryEntry(L, setter)
    });
    return true;
}

PropertyOperationResult CustomPropertyManager::GetProperty(lua_State* L, GenericPropertyMap const& pm, LifetimeHandle lifetime, void const* object, FixedStringId const& prop) const
{
    auto& customProps = PropertyMaps[(int32_t)pm.RegistryIndex];
    if (!customProps) {
        return PropertyOperationResult::NoSuchProperty;
    }

    auto customProp = customProps->Properties.try_get(prop);
    if (!customProp) {
        return PropertyOperationResult::NoSuchProperty;
    }

    if (customProp->Value) {
        push(L, customProp->Value);
    } else {
        push(L, customProp->Getter);
        LightObjectProxyMetatable::Make(L, pm, object, lifetime);

        if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
            luaL_error(L, "%s", lua_tostring(L, -1));
        }
    }

    return PropertyOperationResult::Success;
}

PropertyOperationResult CustomPropertyManager::SetProperty(lua_State* L, GenericPropertyMap const& pm, void* object, FixedStringId const& prop, int index) const
{
    auto& customProps = PropertyMaps[(int32_t)pm.RegistryIndex];
    if (!customProps) {
        return PropertyOperationResult::NoSuchProperty;
    }

    auto customProp = customProps->Properties.try_get(prop);
    if (!customProp) {
        return PropertyOperationResult::NoSuchProperty;
    }

    if (customProp->Setter) {
        push(L, customProp->Setter);
        LightObjectProxyMetatable::Make(L, pm, object, State::FromLua(L)->GetCurrentLifetime());
        lua_pushvalue(L, index);

        if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
            luaL_error(L, "%s", lua_tostring(L, -1));
        }
        return PropertyOperationResult::Success;
    } else {
        return PropertyOperationResult::ReadOnly;
    }
}


void SerializeRawObject(lua_State* L, void const* obj, GenericPropertyMap const& pm)
{
    StackCheck _(L, 1);
    lua_createtable(L, 0, (int)pm.Properties.size());
    for (auto it : pm.IterableProperties) {
        auto const& prop = pm.Properties.values()[it.Value()];
        if (prop.Serialize != nullptr) {
            auto value = reinterpret_cast<void const*>((std::uintptr_t)obj + prop.Offset);
            auto result = prop.Serialize(L, value, prop);
            if (result == PropertyOperationResult::Success) {
                lua_setfield(L, -2, it.Key().GetString());
            }
        }
    }
}

void UnserializeRawObjectFromTable(lua_State* L, int index, void* obj, GenericPropertyMap const& pm)
{
    StackCheck _(L);
    for (auto it : pm.IterableProperties) {
        auto const& prop = pm.Properties.values()[it.Value()];
        lua_getfield(L, index, it.Key().GetString());
        if (lua_type(L, -1) != LUA_TNIL) {
            pm.SetRawProperty(L, obj, prop, lua_absindex(L, -1));
        }
        lua_pop(L, 1);
    }
}

void UnserializeRawObjectFromUserdata(lua_State* L, int index, void* obj, GenericPropertyMap const& pm)
{
    StackCheck _(L);
    auto other = ObjectProxy::GetRaw(L, index, pm);

    if (pm.Assign != nullptr) {
        pm.Assign(obj, other);
    } else {
        luaL_error(L, "Type %s does not support assignment", pm.Name.GetString());
    }
}

std::atomic<uint32_t> gDisablePropertyWarningsCount{ 0 };

void DisablePropertyWarnings()
{
    gDisablePropertyWarningsCount++;
}

void EnablePropertyWarnings()
{
    gDisablePropertyWarningsCount--;
}

void ProcessPropertyNotifications(RawPropertyAccessors const& prop, bool isWriting)
{
    if (gDisablePropertyWarningsCount > 0) return;

    if ((unsigned)prop.PendingNotifications & (unsigned)PropertyNotification::Deprecated) {
        WARN("DEPRECATED: %s property %s.%s that is deprecated and will be removed in future versions", 
            (isWriting ? "Writing" : "Reading"), prop.PropertyMap->Name.GetString(), prop.Name.GetString());
    }
    
    if ((unsigned)prop.PendingNotifications & (unsigned)PropertyNotification::Renamed) {
        WARN("DEPRECATED: %s property %s.%s that has been renamed to %s", 
            (isWriting ? "Writing" : "Reading"), prop.PropertyMap->Name.GetString(), prop.Name.GetString(), prop.NewName.GetString());
    }
    
    if ((unsigned)prop.PendingNotifications & (unsigned)PropertyNotification::TemporaryName) {
        WARN("DEPRECATED: %s property %s.%s that is a placeholder and will be renamed in future versions", 
            (isWriting ? "Writing" : "Reading"), prop.PropertyMap->Name.GetString(), prop.Name.GetString());
    }

    prop.PendingNotifications = PropertyNotification::None;
}

END_NS()
