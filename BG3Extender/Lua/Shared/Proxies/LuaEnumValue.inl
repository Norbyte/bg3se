#include <Lua/Shared/Proxies/LuaEnumValue.h>
#include <Extender/ScriptExtender.h>

BEGIN_NS(lua)

EnumInfoStore* EnumValueMetatable::GetEnumInfo(CppObjectMetadata const& self)
{
    return EnumRegistry::Get().EnumsById[self.PropertyMapTag];
}

FixedString EnumValueMetatable::GetLabel(CppObjectMetadata const& self)
{
    return GetEnumInfo(self)->Find(static_cast<EnumUnderlyingType>(self.Value));
}

EnumUnderlyingType EnumValueMetatable::GetValue(CppObjectMetadata const& self)
{
    return static_cast<EnumUnderlyingType>(self.Value);
}

int EnumValueMetatable::Index(lua_State* L, CppObjectMetadata& self)
{
    auto index = get<FixedString>(L, 2);
    if (index == GFS.strLabel) {
        push(L, GetLabel(self));
    } else if (index == GFS.strValue) {
        push(L, GetValue(self));
    } else if (index == GFS.strEnumName) {
        push(L, GetEnumInfo(self)->EnumName);
    } else {
        luaL_error(L, "Enum values have no property named '%s'", index.GetString());
        push(L, nullptr);
    }

    return 1;
}

int EnumValueMetatable::ToString(lua_State* L, CppObjectMetadata& self)
{
    StackCheck _(L, 1);
    push(L, GetLabel(self));
    return 1;
}

bool EnumValueMetatable::IsEqual(lua_State* L, CppObjectMetadata& self, int otherIndex)
{
    auto ei = GetEnumInfo(self);
    auto other = try_get_enum_value(L, otherIndex, self.PropertyMapTag);
    return other && *other == self.Value;
}

bool EnumValueMetatable::IsLessThan(lua_State* L, CppObjectMetadata& self, int otherIndex)
{
    auto ei = GetEnumInfo(self);
    auto other = try_get_enum_value(L, otherIndex, self.PropertyMapTag);
    return other && self.Value < *other;
}

bool EnumValueMetatable::IsLessThan(lua_State* L, int selfIndex, CppObjectMetadata& other)
{
    auto ei = GetEnumInfo(other);
    auto self = try_get_enum_value(L, selfIndex, other.PropertyMapTag);
    return self && *self < other.Value;
}

char const* EnumValueMetatable::GetTypeName(lua_State* L, CppObjectMetadata& self)
{
    auto ei = GetEnumInfo(self);
    return ei->LuaName.GetString();
}

EnumUnderlyingType get_enum_value(lua_State* L, int index, EnumTypeId typeId)
{
    auto const& store = *EnumRegistry::Get().EnumsById[typeId];

    switch (lua_type(L, index)) {
    case LUA_TSTRING:
    {
        auto val = do_get(L, index, Overload<FixedString>{});
        auto valueIndex = store.Find(val);
        if (valueIndex) {
            return *valueIndex;
        } else {
            luaL_error(L, "Param %d: not a valid '%s' enum label: %s", index, store.EnumName.GetString(), val.GetString());
        }
        break;
    }

    case LUA_TNUMBER:
    {
        auto val = (EnumUnderlyingType)lua_tointeger(L, index);
        if (store.Find(val)) {
            return val;
        } else {
            luaL_error(L, "Param %d: not a valid '%s' enum index: %d", index, store.EnumName.GetString(), val);
        }
        break;
    }

    case LUA_TLIGHTCPPOBJECT:
    {
        auto meta = lua_get_cppvalue(L, lua_absindex(L, index));
        if (meta.MetatableTag == EnumValueMetatable::MetaTag && meta.PropertyMapTag == (unsigned)store.RegistryIndex) {
            return static_cast<EnumUnderlyingType>(meta.Value);
        } else {
            luaL_error(L, "Param %d: expected a '%s' enum value, got %s", index, store.EnumName.GetString(), GetDebugName(meta));
        }
        break;
    }

    default:
        luaL_error(L, "Param %d: expected integer, string or enum label of type '%s' value, got %s", index,
            store.EnumName.GetString(), GetDebugName(L, index));
        break;
    }

    return 0;
}

std::optional<EnumUnderlyingType> try_get_enum_value(lua_State* L, int index, EnumTypeId typeId)
{
    auto const& store = *EnumRegistry::Get().EnumsById[typeId];

    switch (lua_type(L, index)) {
    case LUA_TSTRING:
    {
        auto val = do_get(L, index, Overload<FixedString>{});
        auto valueIndex = store.Find(val);
        if (valueIndex) {
            return *valueIndex;
        }
        break;
    }

    case LUA_TNUMBER:
    {
        auto val = (EnumUnderlyingType)lua_tointeger(L, index);
        if (store.Find(val)) {
            return val;
        }
        break;
    }

    case LUA_TLIGHTCPPOBJECT:
    {
        auto meta = lua_get_cppvalue(L, lua_absindex(L, index));
        if (meta.MetatableTag == EnumValueMetatable::MetaTag && meta.PropertyMapTag == (unsigned)store.RegistryIndex) {
            return static_cast<EnumUnderlyingType>(meta.Value);
        }
        break;
    }
    }

    return {};
}

void push_enum_value(lua_State* L, EnumUnderlyingType value, EnumTypeId id)
{
    EnumValueMetatable::Make(L, value, id);
}

END_NS()
