/// <lua_module>Types</lua_module>
BEGIN_NS(lua::types)

std::optional<STDString> GetCppObjectTypeName(lua_State * L, int index)
{
    auto meta = lua_get_lightcppany(L, index);

    switch (meta.MetatableTag) {
    case MetatableTag::ObjectRef:
    case MetatableTag::Array:
    case MetatableTag::Map:
    case MetatableTag::Set:
    case MetatableTag::EnumValue:
    case MetatableTag::BitfieldValue:
    case MetatableTag::UserVariableHolder:
    case MetatableTag::ModVariableHolder:
    case MetatableTag::Entity:
    case MetatableTag::OsiFunctionName:
    case MetatableTag::SystemMap:
        return GetDebugName(meta);

#if defined(ENABLE_IMGUI)
    case MetatableTag::ImguiObject:
        return ImguiObjectProxyMetatable::GetTypeName(L, meta);
#endif

    default:
        return {};
    }
}

TypeInformation const* GetCppObjectType(lua_State * L, int index)
{
    auto meta = lua_get_lightcppany(L, index);

    switch (meta.MetatableTag) {
    case MetatableTag::ObjectRef:
    {
        auto propertyMap = gStructRegistry.Get(meta.PropertyMapTag);
        return propertyMap->TypeInfo;
    }

    case MetatableTag::Array:
    {
        auto impl = gExtender->GetPropertyMapManager().GetArrayProxy(meta.PropertyMapTag);
        return &impl->GetContainerType();
    }

    case MetatableTag::Map:
    {
        auto impl = gExtender->GetPropertyMapManager().GetMapProxy(meta.PropertyMapTag);
        return &impl->GetContainerType();
    }

    case MetatableTag::Set:
    {
        auto impl = gExtender->GetPropertyMapManager().GetSetProxy(meta.PropertyMapTag);
        return &impl->GetContainerType();
    }

#if defined(ENABLE_IMGUI)
    case MetatableTag::ImguiObject:
    {
        auto obj = ImguiObjectProxyMetatable::GetRenderable(meta);
        if (obj != nullptr) {
            return obj->GetRTTI().TypeInfo;
        } else {
            return nullptr;
        }
    }
#endif

    case MetatableTag::EnumValue:
    case MetatableTag::BitfieldValue:
        // TODO - implement reflection for enum/bitfield types!

    case MetatableTag::UserVariableHolder:
    case MetatableTag::ModVariableHolder:
    case MetatableTag::Entity:
    default:
        return nullptr;
    }
}

std::optional<STDString> GetObjectType(lua_State* L, AnyRef object)
{
    switch (lua_type(L, object.Index)) {
    case LUA_TLIGHTCPPOBJECT:
    case LUA_TCPPOBJECT:
        return GetCppObjectTypeName(L, 1);
    default:
        return {};
    }
}

TypeInformation* TypeOf(lua_State* L, AnyRef object)
{
    switch (lua_type(L, object.Index)) {
    case LUA_TLIGHTCPPOBJECT:
    case LUA_TCPPOBJECT:
        return const_cast<TypeInformation*>(GetCppObjectType(L, 1));
    default:
        return nullptr;
    }
}

bool IsA(lua_State* L, AnyRef object, FixedString typeName)
{
    auto expectedType = TypeInformationRepository::GetInstance().TryGetType(typeName);
    if (expectedType == nullptr) {
        luaL_error(L, "No such type: %s", typeName.GetString());
    }

    auto type = TypeOf(L, object);
    if (type == nullptr) return false;

    // Simple case: Types are equivalent
    if (type == expectedType) return true;

    // Check if both types are part of an inheritance tree
    if (type->PropertyMap != nullptr && expectedType->PropertyMap != nullptr) {
        // Yes, check if one of an ancestor of the other
        return type->PropertyMap->IsA(expectedType->PropertyMap->RegistryIndex);
    } else {
        // No, they cannot be the same type
        return false;
    }
}

TypeInformation* GetTypeInfo(FixedString const& typeName)
{
    auto const& type = TypeInformationRepository::GetInstance().GetType(typeName);
    if (type.Kind == LuaTypeId::Unknown) {
        return nullptr;
    } else {
        return const_cast<TypeInformation*>(&type);
    }
}

Array<FixedString> GetAllTypes()
{
    Array<FixedString> types;
    auto const& allTypes = TypeInformationRepository::GetInstance().GetAllTypes();
    for (auto const& type : allTypes) {
        types.push_back(type.Key);
    }

    return types;
}

void RegisterEnumeration(lua_State* L, EnumInfoStore const& ty)
{
    lua_newtable(L);

    for (auto const& elem : ty.Values) {
        EnumValueMetatable::Make(L, elem.Value, ty.RegistryIndex);
        lua_setfield(L, -2, elem.Key.GetString());

        push(L, elem.Value);
        EnumValueMetatable::Make(L, elem.Value, ty.RegistryIndex);
        lua_rawset(L, -3);
    }

    lua_setfield(L, -2, ty.LuaName.GetString());
}

void RegisterEnumeration(lua_State* L, BitfieldInfoStore const& ty)
{
    lua_newtable(L);

    for (auto const& elem : ty.Values) {
        BitfieldValueMetatable::Make(L, elem.Value, ty.RegistryIndex);
        lua_setfield(L, -2, elem.Key.GetString());

        push(L, elem.Value);
        BitfieldValueMetatable::Make(L, elem.Value, ty.RegistryIndex);
        lua_rawset(L, -3);
    }

    lua_setfield(L, -2, ty.LuaName.GetString());
}

void RegisterEnumerations(lua_State* L)
{
    StackCheck _(L);
    lua_getglobal(L, "Ext");
    lua_newtable(L);

    for (auto const& ty : EnumRegistry::Get().EnumsById) {
        RegisterEnumeration(L, *ty);
    }
    
    for (auto const& ty : BitfieldRegistry::Get().BitfieldsById) {
        RegisterEnumeration(L, *ty);
    }

    lua_setfield(L, -2, "Enums");
    lua_pop(L, 1);
}

bool Validate(lua_State* L)
{
    auto meta = lua_get_lightcppobject(L, 1, MetatableTag::ObjectRef);
    auto& pm = LightObjectProxyMetatable::GetPropertyMap(meta);
    return pm.ValidateObject(meta.Ptr);
}

UserReturn Serialize(lua_State* L)
{
    auto meta = lua_get_lightcppany(L, 1);

    switch (meta.MetatableTag) {
        case MetatableTag::ObjectRef:
        {
            auto& pm = LightObjectProxyMetatable::GetPropertyMap(meta);
            pm.Serialize(L, meta.Ptr);
            return 1;
        }
            
        case MetatableTag::Array:
        {
            auto impl = ArrayProxyMetatable::GetImpl(meta);
            impl->Serialize(L, meta);
            return 1;
        }

        case MetatableTag::Map:
        {
            auto impl = MapProxyMetatable::GetImpl(meta);
            impl->Serialize(L, meta);
            return 1;
        }

        case MetatableTag::Set:
        {
            auto impl = SetProxyMetatable::GetImpl(meta);
            impl->Serialize(L, meta);
            return 1;
        }

        default:
            return luaL_error(L, "Don't know how to serialize userdata of type %s", GetDebugName(meta.MetatableTag));
    }
}

void Unserialize(lua_State* L)
{
    auto meta = lua_get_lightcppany(L, 1);
    luaL_checktype(L, 2, LUA_TTABLE);

    switch (meta.MetatableTag) {
        case MetatableTag::ObjectRef:
        {
            auto& pm = LightObjectProxyMetatable::GetPropertyMap(meta);
            pm.Unserialize(L, 2, meta.Ptr);
            break;
        }

        case MetatableTag::Array:
        {
            auto impl = ArrayProxyMetatable::GetImpl(meta);
            impl->Unserialize(L, meta, 2);
            break;
        }

        case MetatableTag::Map:
        {
            auto impl = MapProxyMetatable::GetImpl(meta);
            impl->Unserialize(L, meta, 2);
            break;
        }

        case MetatableTag::Set:
        {
            auto impl = SetProxyMetatable::GetImpl(meta);
            impl->Unserialize(L, meta, 2);
            break;
        }

        default:
        {
            luaL_error(L, "Don't know how to unserialize userdata of metatype %d", (unsigned)meta.MetatableTag);
            break;
        }
    }
}

UserReturn Construct(lua_State* L, FixedString const& typeName)
{
    auto const& type = TypeInformationRepository::GetInstance().GetType(typeName);
    if (type.Kind == LuaTypeId::Unknown) {
        return luaL_error(L, "Unknown type name '%s'", typeName.GetString());
    }
    
    if (type.Kind != LuaTypeId::Object) {
        return luaL_error(L, "Unable to construct non-object type '%s'", typeName.GetString());
    }
    
    if (type.PropertyMap == nullptr || type.PropertyMap->Construct == nullptr) {
        return luaL_error(L, "Type '%s' is not constructible", typeName.GetString());
    }

    // TODO
    return 0;
}

std::optional<STDString> GetValueType(lua_State* L, AnyRef object)
{
    return GetDebugName(L, object.Index);
}

UserReturn GetHashSetValueAt(lua_State* L, AnyRef object, uint32_t index)
{
    auto meta = lua_get_lightcppobject(L, object.Index, MetatableTag::Set);
    auto impl = SetProxyMetatable::GetImpl(meta);
    if (!impl->GetElementAt(L, meta, index)) {
        push(L, nullptr);
    }

    return 1;
}

void RegisterTypesLib()
{
    DECLARE_MODULE(Types, Both)
    BEGIN_MODULE()
    MODULE_FUNCTION(GetValueType)
    MODULE_FUNCTION(GetObjectType)
    MODULE_FUNCTION(GetTypeInfo)
    MODULE_FUNCTION(TypeOf)
    MODULE_FUNCTION(IsA)
    MODULE_FUNCTION(GetAllTypes)
    MODULE_FUNCTION(Validate)
    MODULE_FUNCTION(Serialize)
    MODULE_FUNCTION(Unserialize)
    MODULE_FUNCTION(Construct)
    MODULE_FUNCTION(GetHashSetValueAt)
    END_MODULE()
}

END_NS()
