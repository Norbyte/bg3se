#include <Lua/Osiris/ValueHelpers.h>

BEGIN_SE()

OsirisStaticGlobals& GetOsiGlobals()
{
    return gExtender->GetServer().Osiris().GetWrappers().Globals;
}

OsiStringHandle OsiStringMake(char const* str, bool isGuidString)
{
    OsiStringHandle handle;
    auto& globals = GetOsiGlobals();
    globals.AddString(*globals.StringTable, handle, str, isGuidString);
    return handle;
}

void OsiStringIncRef(OsiStringHandle str)
{
    auto& globals = GetOsiGlobals();
    globals.AddStringRef(*globals.StringTable, str);
}

void OsiStringDecRef(OsiStringHandle str)
{
    auto& globals = GetOsiGlobals();
    globals.RemoveString(*globals.StringTable, str);
}

char const* OsiStringGet(OsiStringHandle str)
{
    auto& globals = GetOsiGlobals();
    return globals.GetString(*globals.StringTable, str);
}

bool OsiIsStringAlias(ValueType typeId)
{
    if (typeId < ValueType::String) return false;

    if (typeId == ValueType::String || typeId == ValueType::GuidString) return true;

    auto& globals = GetOsiGlobals();
    auto alias = (*globals.Types)->ResolveAlias((uint16_t)typeId);
    return alias == ValueType::String || alias == ValueType::GuidString;
}

bool OsiIsGuidStringAlias(ValueType typeId)
{
    if (typeId < ValueType::String) return false;

    if (typeId == ValueType::GuidString) return true;

    auto& globals = GetOsiGlobals();
    auto alias = (*globals.Types)->ResolveAlias((uint16_t)typeId);
    return alias == ValueType::GuidString;
}

TypedValue::~TypedValue()
{
    ReleaseValue();
}

TypedValue::TypedValue(TypedValue& o)
    : Value(o.Value), TypeId(o.TypeId), Index(o.Index), Flags(o.Flags)
{
    if (OsiIsStringAlias(TypeId)) {
        OsiStringIncRef(Value.StringHandle);
    }
}

TypedValue::TypedValue(TypedValue&& o) noexcept
    : Value(o.Value), TypeId(o.TypeId), Index(o.Index), Flags(o.Flags)
{
    o.Flags &= ~ValueFlags::IsValid;
    o.Value.Int64 = 0;
    o.TypeId = ValueType::None;
}

void TypedValue::ReleaseValue()
{
    if (OsiIsStringAlias(TypeId)) {
        OsiStringDecRef(Value.StringHandle);
    }
}

TypedValue& TypedValue::operator = (TypedValue const& o)
{
    ReleaseValue();

    Value = o.Value;
    TypeId = o.TypeId;
    Index = o.Index;
    Flags = o.Flags;

    if (OsiIsStringAlias(TypeId)) {
        OsiStringIncRef(Value.StringHandle);
    }

    return *this;
}

TypedValue& TypedValue::operator = (TypedValue&& o) noexcept
{
    ReleaseValue();

    Value = o.Value;
    TypeId = o.TypeId;
    Index = o.Index;
    Flags = o.Flags;

    o.Flags &= ~ValueFlags::IsValid;
    o.Value.Int64 = 0;
    o.TypeId = ValueType::None;

    return *this;
}

char const* TypedValue::GetString() const
{
    se_assert(OsiIsStringAlias(TypeId));
    return OsiStringGet(Value.StringHandle);
}

void TypedValue::ClearValue()
{
    ReleaseValue();

    Flags &= ~ValueFlags::IsValid;
    Value.Int64 = 0;
    TypeId = ValueType::None;
}

void TypedValue::SetValue(ValueType typeId, int32_t v)
{
    ReleaseValue();

    Value.Int32 = v;
    TypeId = typeId;
    Flags |= ValueFlags::IsValid;
}

void TypedValue::SetValue(ValueType typeId, float v)
{
    ReleaseValue();

    Value.Float = v;
    TypeId = typeId;
    Flags |= ValueFlags::IsValid;
}

void TypedValue::SetValue(ValueType typeId, int64_t v)
{
    ReleaseValue();

    Value.Int64 = v;
    TypeId = typeId;
    Flags |= ValueFlags::IsValid;
}

void TypedValue::SetValue(ValueType typeId, OsiStringHandle s)
{
    ReleaseValue();

    Value.StringHandle = s;
    TypeId = typeId;
    Flags |= ValueFlags::IsValid;
}

void TypedValue::SetValue(ValueType typeId, char const* s)
{
    ReleaseValue();

    Value.StringHandle = OsiStringMake(s, OsiIsGuidStringAlias(typeId));
    TypeId = typeId;
    Flags |= ValueFlags::IsValid;
}

Database* DatabaseRef::Get() const
{
    auto manager = *gExtender->GetServer().Osiris().GetGlobals().Databases;
    if (Id == 0 || manager == nullptr) {
        return nullptr;
    }

    return manager->Db.Elements[Id - 1];
}

Adapter* AdapterRef::Get() const
{
    auto manager = *gExtender->GetServer().Osiris().GetGlobals().Adapters;
    if (Id == 0 || manager == nullptr) {
        return nullptr;
    }

    return manager->Db.Elements[Id - 1];
}

Node* NodeRef::Get() const
{
    auto manager = *gExtender->GetServer().Osiris().GetGlobals().Nodes;
    if (Id == 0 || manager == nullptr) {
        return nullptr;
    }

    return manager->Db.Elements[Id - 1];
}

SmallTuple::SmallTuple()
{
    for (uint32_t i = 0; i < std::size(inline_); i++) {
        new (inline_ + i) TypedValue();
    }
}

SmallTuple::~SmallTuple()
{
    for (uint32_t i = 0; i < size_; i++) {
        Buf()[i].~TypedValue();
    }

    if (capacity_ > InlineSize) {
        GameFree(values_);
    }
}

void SmallTuple::Reserve(uint32_t newCapacity)
{
    if (capacity_ < newCapacity) {
        auto buf = GameAllocArray<TypedValue>(newCapacity);
        for (uint32_t i = 0; i < size_; i++) {
            buf[i] = Buf()[i];
            Buf()[i].~TypedValue();
        }

        values_ = buf;
        capacity_ = newCapacity;
    }
}

TypedValue& SmallTuple::Add()
{
    if (size_ == capacity_) Reserve(capacity_ * 2);
    auto& val = Buf()[size_++];
    val = TypedValue{};
    val.Index = size_ - 1;
    return val;
}

END_SE()

BEGIN_NS(esv::lua)

using namespace bg3se::lua;

ValueType GetBaseType(ValueType type)
{
    return (*gExtender->GetServer().Osiris().GetGlobals().Types)->ResolveAlias((uint16_t)type);
}

int64_t LuaToInt(lua_State* L, int i, int type)
{
    if (type == LUA_TNUMBER) {
#if LUA_VERSION_NUM > 501
        if (lua_isinteger(L, i)) {
            return (int64_t)lua_tointeger(L, i);
        } else {
            return (int64_t)lua_tonumber(L, i);
        }
#else
        return (int64_t)lua_tonumber(L, i);
#endif
    } else if (type == LUA_TLIGHTCPPOBJECT) {
        auto handle = get<EntityHandle>(L, i);
        return (int64_t)handle.Handle;
    } else {
        luaL_error(L, "Number expected for argument %d, got %s", i, GetDebugName(L, i));
    }

    return 0;
}

char * LuaToString(lua_State* L, int i, int type, char* reuseString)
{
    if (type == LUA_TSTRING) {
        if (reuseString != nullptr) {
            size_t len;
            auto s = lua_tolstring(L, i, &len);
            strncpy_s(reuseString, 0x100, s, len);
            return reuseString;
        } else {
            // TODO - not sure if we're the owners of the string or the TypedValue is
            return _strdup(lua_tostring(L, i));
        }
    } else {
        luaL_error(L, "String expected for argument %d, got %s", i, GetDebugName(L, i));
    }

    return nullptr;
}

void LuaToOsi(lua_State * L, int i, TypedValue & tv, ValueType osiType, bool allowNil)
{
    auto type = lua_type(L, i);
    if (allowNil && type == LUA_TNIL) {
        tv.ClearValue();
        return;
    }

    switch (GetBaseType(osiType)) {
    case ValueType::Integer:
        tv.SetValue(osiType, (int32_t)LuaToInt(L, i, type));
        break;

    case ValueType::Integer64:
        tv.SetValue(osiType, (int64_t)LuaToInt(L, i, type));
        break;

    case ValueType::Real:
        if (type != LUA_TNUMBER) {
            luaL_error(L, "Number expected for argument %d, got %s", i, GetDebugName(L, i));
        }

#if LUA_VERSION_NUM > 501
        if (lua_isinteger(L, i)) {
            tv.SetValue(osiType, (float)lua_tointeger(L, i));
        } else {
            tv.SetValue(osiType, (float)lua_tonumber(L, i));
        }
#else
        tv.SetValue(osiType, (float)lua_tonumber(L, i));
#endif
        break;

    case ValueType::String:
    case ValueType::GuidString:
        tv.SetValue(osiType, LuaToString(L, i, type, nullptr));
        break;

    default:
        luaL_error(L, "Unhandled Osi argument type %d", osiType);
        break;
    }
}

TypedValue * LuaToOsi(lua_State * L, int i, ValueType osiType, bool allowNil)
{
    auto tv = new TypedValue();
    LuaToOsi(L, i, *tv, osiType, allowNil);
    return tv;
}

void LuaToOsi(lua_State * L, int i, OsiArgumentValue & arg, ValueType osiType, bool allowNil, bool reuseStrings)
{
    arg.TypeId = osiType;
    auto type = lua_type(L, i);
    if (allowNil && type == LUA_TNIL) {
        arg.TypeId = ValueType::None;
        return;
    }

    switch (GetBaseType(osiType)) {
    case ValueType::Integer:
        arg.Int32 = (int32_t)LuaToInt(L, i, type);
        break;

    case ValueType::Integer64:
        arg.Int64 = (int64_t)LuaToInt(L, i, type);
        break;

    case ValueType::Real:
        if (type != LUA_TNUMBER) {
            luaL_error(L, "Number expected for argument %d, got %s", i, lua_typename(L, type));
        }

#if LUA_VERSION_NUM > 501
        if (lua_isinteger(L, i)) {
            arg.Float = (float)lua_tointeger(L, i);
        } else {
            arg.Float = (float)lua_tonumber(L, i);
        }
#else
        arg.Float = (float)lua_tonumber(L, i);
#endif
        break;

    case ValueType::String:
    case ValueType::GuidString:
        if (reuseStrings) {
            arg.String = LuaToString(L, i, type, const_cast<char*>(arg.String));
        } else {
            arg.String = LuaToString(L, i, type, nullptr);
        }
        break;

    default:
        luaL_error(L, "Unhandled Osi argument type %d", osiType);
        break;
    }
}

void OsiToLua(lua_State * L, OsiArgumentValue const & arg)
{
    switch (GetBaseType(arg.TypeId)) {
    case ValueType::None:
        lua_pushnil(L);
        break;

    case ValueType::Integer:
        push(L, arg.Int32);
        break;

    case ValueType::Integer64:
        push(L, arg.Int64);
        break;

    case ValueType::Real:
        push(L, arg.Float);
        break;

    case ValueType::String:
    case ValueType::GuidString:
        push(L, arg.String);
        break;

    default:
        luaL_error(L, "Unhandled Osi argument type %d", arg.TypeId);
        break;
    }
}

void OsiToLua(lua_State * L, TypedValue const & tv)
{
    switch (GetBaseType((ValueType)tv.TypeId)) {
    case ValueType::None:
        lua_pushnil(L);
        break;

    case ValueType::Integer:
        push(L, tv.Value.Int32);
        break;

    case ValueType::Integer64:
        push(L, tv.Value.Int64);
        break;

    case ValueType::Real:
        push(L, tv.Value.Float);
        break;

    case ValueType::String:
    case ValueType::GuidString:
        push(L, tv.GetString());
        break;

    default:
        luaL_error(L, "Unhandled Osi TypedValue type %d", tv.TypeId);
        break;
    }
}

uint32_t FunctionNameHash(char const * str)
{
    uint32_t hash{ 0 };
    while (*str) {
        hash = (*str++ | 0x20) + 129 * (hash % 4294967);
    }

    return hash;
}

OsiFunctionDef const* LookupOsiFunction(STDString const& name, uint32_t arity)
{
    auto functions = gExtender->GetServer().Osiris().GetGlobals().Functions;
    if (!functions) {
        return nullptr;
    }

    OsiString sig(name);
    sig += "/";
    sig += std::to_string(arity);

    auto hash = FunctionNameHash(name.c_str()) + arity;
    auto func = (*functions)->Find(hash, sig);
    if (func == nullptr
        || ((*func)->Node.Id == 0
            && (*func)->Type != FunctionType::Call
            && (*func)->Type != FunctionType::Query
            && (*func)->Type != FunctionType::Event)) {
        return nullptr;
    }

    return *func;
};

END_NS()
