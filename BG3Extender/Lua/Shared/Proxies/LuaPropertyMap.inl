#include <Lua/Shared/Proxies/LuaPropertyMap.h>

BEGIN_NS(lua)

StructRegistry gStructRegistry;

void StructRegistry::Register(GenericPropertyMap* pm, StructTypeId id)
{
    se_assert(pm->IsInitializing);
    pm->RegistryIndex = id;

    if (Validated.Size < (uint32_t)id + 1) {
        Validated.EnsureSize(id + 1);
    }

    if (StructsById.size() < (uint32_t)id + 1) {
        StructsById.resize(id + 1);
    }

    se_assert(StructsById[id] == nullptr);
    StructsById[id] = pm;
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

    if (entry.HasNotifications()) [[unlikely]] {
        const_cast<RawPropertyAccessorsHotData&>(entry).MarkNotificationsProcessed();
        ProcessPropertyNotifications(*entry.Cold, false);
    }

    auto getter = entry.Getter();
    auto offset = entry.Offset();
    auto data = reinterpret_cast<uint8_t const*>(object) + offset;
    auto result = getter(L, lifetime, data, entry);

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
    auto data = reinterpret_cast<uint8_t const*>(object) + prop.Offset;
    return prop.Get(L, lifetime, data, prop);
}

PropertyOperationResult GenericPropertyMap::SetRawProperty(lua_State* L, void* object, FixedStringId const& prop, int index) const
{
    auto& ah = static_cast<FixedStringUnhashed const&>(prop);
    auto const& entry = PropertiesHot.get(ah);

    if (entry.HasNotifications()) [[unlikely]] {
        const_cast<RawPropertyAccessorsHotData&>(entry).MarkNotificationsProcessed();
        ProcessPropertyNotifications(*entry.Cold, true);
    }

    auto setter = entry.Setter();
    auto offset = entry.Offset();
    auto data = reinterpret_cast<uint8_t*>(object) + offset;
    auto result = setter(L, data, index, entry);

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
    auto data = reinterpret_cast<uint8_t*>(object) + prop.Offset;
    return prop.Set(L, data, index, prop);
}

void GenericPropertyMap::AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
    typename RawPropertyAccessors::Setter* setter, typename RawPropertyAccessors::Serializer* serialize, 
    std::size_t offset, uint64_t flag, PropertyNotification notification, char const* newName, bool iterable)
{
    se_assert((!Initialized || !InheritanceUpdated) && IsInitializing);
    FixedStringUnhashed key{ prop };
    FixedString newNameKey{ newName ? newName : "" };
    se_assert(Properties.find(key) == Properties.end());
    Properties.set(key, RawPropertyAccessors{ key, offset, flag, getter, setter, serialize, notification, this, newNameKey, iterable });

    if (iterable) {
        IterableProperties.set(key, Properties.size() - 1);
    }
}

void GenericPropertyMap::AddRawValidator(char const* prop, typename RawPropertyValidators::Validator* validate, std::size_t offset, uint64_t flag)
{
    se_assert((!Initialized || !InheritanceUpdated) && IsInitializing);
    Validators.push_back(RawPropertyValidators{ FixedString(prop), validate, offset, flag });
}

void GenericPropertyMap::AddRawProperty(char const* prop, typename RawPropertyAccessors::Getter* getter,
    typename RawPropertyAccessors::Setter* setter, typename RawPropertyValidators::Validator* validate, 
    typename RawPropertyAccessors::Serializer* serialize, std::size_t offset, uint64_t flag, 
    PropertyNotification notification, char const* newName, bool iterable)
{
    if (getter != nullptr || setter != nullptr) {
        AddRawProperty(prop, getter, setter, serialize, offset, flag, notification, newName, iterable);
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
                se_assert(RegistryIndex >= 0);
                gStructRegistry.Validated.Set(RegistryIndex);
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

CustomPropertyManager::CustomPropertyManager()
{
    PropertyMaps.resize(gStructRegistry.StructsById.size());
}

bool CustomPropertyManager::PropertyCheck(GenericPropertyMap const& pm, FixedStringUnhashed const& name)
{
    if (!PropertyMaps[pm.RegistryIndex]) {
        PropertyMaps[pm.RegistryIndex] = std::make_unique<CustomPropertyMap>();
    }

    auto prop = pm.Properties.find(name);
    if (prop) {
        LuaError("Type " << pm.Name << " already has a property named '" << name << "'");
        return false;
    }

    auto& customProps = PropertyMaps[pm.RegistryIndex];
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

    auto& customProps = PropertyMaps[pm.RegistryIndex];
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

    auto& customProps = PropertyMaps[pm.RegistryIndex];
    customProps->Properties.set(name, CustomProperty{
        .Getter = RegistryEntry(L, getter),
        .Setter = RegistryEntry(L, setter)
    });
    return true;
}

PropertyOperationResult CustomPropertyManager::GetProperty(lua_State* L, GenericPropertyMap const& pm, LifetimeHandle lifetime, void const* object, FixedStringId const& prop) const
{
    auto& customProps = PropertyMaps[pm.RegistryIndex];
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
    auto& customProps = PropertyMaps[pm.RegistryIndex];
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
            auto result = prop.Serialize(L, obj, prop);
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
