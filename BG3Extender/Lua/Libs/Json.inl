#include <Lua/Libs/Json.h>

#include <CoreLib/JsonLibs.h>
#include <fstream>
#include <unordered_set>
#include <lstate.h>

/// <lua_module>Json</lua_module>
BEGIN_NS(lua::json)

using namespace rapidjson;

using Val = rapidjson::Value;

void Parse(lua_State * L, Val const& val);

void ParseArray(lua_State * L, Val::ConstArray const& val)
{
    lua_createtable(L, (int)val.Size(), 0);
    int idx = 1;
    for (auto& it : val) {
        push(L, idx++);
        Parse(L, it);
        lua_rawset(L, -3);
    }
}

void ParseObject(lua_State * L, Val::ConstObject const& val)
{
    lua_createtable(L, 0, (int)val.MemberCount());
    for (auto& it : val) {
        Parse(L, it.name);
        Parse(L, it.value);
        lua_rawset(L, -3);
    }
}

void Parse(lua_State * L, Val const& val)
{
    switch (val.GetType()) {
    case Type::kNullType:
        lua_pushnil(L);
        break;

    case Type::kFalseType:
        push(L, false);
        break;

    case Type::kTrueType:
        push(L, true);
        break;

    case Type::kObjectType:
        ParseObject(L, val.GetObj());
        break;

    case Type::kArrayType:
        ParseArray(L, val.GetArray());
        break;

    case Type::kNumberType:
        if (val.IsDouble()) {
            push(L, val.GetDouble());
        } else if (val.IsInt64()) {
            push(L, val.GetInt64());
        } else {
            luaL_error(L, "Unsupported JSON number format");
        }
        break;

    case Type::kStringType:
        push(L, val.GetString());
        break;

    default:
        luaL_error(L, "Attempted to parse unknown JSON value");
    }
}

bool Parse(lua_State * L, StringView json)
{
    StackCheck _(L, 1);
    Document root;
    if (root.Parse(json.data(), json.size()).HasParseError()) {
        ERR("Unable to parse JSON");
        return false;
    }

    Parse(L, root);
    return true;
}

UserReturn LuaParse(lua_State * L)
{
    StackCheck _(L, 1);
    size_t length;
    auto json = luaL_checklstring(L, 1, &length);

    Document root;
    if (root.Parse(json, length).HasParseError()) {
        return luaL_error(L, "Unable to parse JSON");
    }

    Parse(L, root);
    return 1;
}

TValue* GetStackElem(lua_State* L, int idx)
{
    CallInfo* ci = L->ci;
    if (idx > 0) {
        return ci->func + idx;
    } else if (idx > LUA_REGISTRYINDEX) {
        return L->top + idx;
    } else {
        return nullptr;
    }
}

void* GetTablePointer(lua_State* L, int index)
{
    luaL_checktype(L, index, LUA_TTABLE);
    auto val = GetStackElem(L, index);
    if (val) {
        return hvalue(val);
    } else {
        return nullptr;
    }
}

void* GetLightCppObjectPointer(lua_State* L, int index)
{
    auto meta = lua_get_lightcppany(L, index);

    // Value-type userdata has no pointer (we don't want to mark same enum values/entities as false recursion)
    if (meta.MetatableTag == MetatableTag::EnumValue
        || meta.MetatableTag == MetatableTag::BitfieldValue
        || meta.MetatableTag == MetatableTag::Entity
        || meta.MetatableTag == MetatableTag::OsiFunctionName
        || meta.MetatableTag == MetatableTag::SystemMap) {
        return nullptr;
    }

    return (void*)((uintptr_t)meta.Ptr | ((uint64_t)meta.PropertyMapTag << 48) | ((uint64_t)meta.MetatableTag << 62));
}

void* GetPointerValue(lua_State* L, int index)
{
    switch (lua_type(L, index)) {
    case LUA_TTABLE:
        return GetTablePointer(L, index);

    case LUA_TLIGHTCPPOBJECT:
    case LUA_TCPPOBJECT:
        return GetLightCppObjectPointer(L, index);

    default:
        return nullptr;
    }
}

bool CheckForRecursion(lua_State* L, int index, StringifyContext& ctx)
{
    if (ctx.AvoidRecursion) {
        auto ptr = GetPointerValue(L, index);
        if (ptr) {
            auto seenIt = ctx.SeenUserdata.find(ptr);
            if (seenIt != ctx.SeenUserdata.end()) {
                return true;
            } else {
                ctx.SeenUserdata.insert(ptr);
            }
        }
    }

    return false;
}

bool TryGetUserdataPairs(lua_State* L, int index)
{
    auto meta = lua_get_lightcppany(L, index);
    auto mt = State::FromLua(L)->GetMetatableManager().GetMetatable(meta.MetatableTag);
    if (lua_cmetatable_push(L, mt, (int)MetamethodName::Pairs)) {
        return true;
    }

    return false;
}

bool IsArrayLikeUserdata(CppObjectMetadata const& meta)
{
    return meta.MetatableTag == MetatableTag::Array
        || meta.MetatableTag == MetatableTag::Set;
}

bool IsMapOrArrayLikeUserdata(CppObjectMetadata const& meta)
{
    return meta.MetatableTag == MetatableTag::Map
        || meta.MetatableTag == MetatableTag::Set
        || meta.MetatableTag == MetatableTag::Array;
}

template <class Writer>
void Stringify(lua_State * L, int index, unsigned depth, StringifyContext& ctx, Writer& writer);

template <class Writer>
bool StringifyLightCppObject(lua_State* L, int index, CppObjectMetadata& meta, unsigned depth, StringifyContext& ctx, Writer& writer)
{
    StackCheck _(L, 0);

    index = lua_absindex(L, index);

    if (CheckForRecursion(L, index, ctx)) {
        writer.String("*RECURSION*");
        return true;
    }

    if (!TryGetUserdataPairs(L, index)) {
        return false;
    }

    bool isArray = IsArrayLikeUserdata(meta);
    bool isMapOrArray = IsMapOrArrayLikeUserdata(meta);

    if (isArray) {
        writer.StartArray();
    } else {
        writer.StartObject();
    }

#if !defined(NDEBUG)
    auto ptrVal = meta.Ptr;
#endif

    // Call __pairs(obj)
    auto nextIndex = lua_absindex(L, -1);
    lua_pushvalue(L, index);
    lua_call(L, 1, 3); // returns __next, obj, nil

    // Push next, obj, k
    lua_pushvalue(L, nextIndex);
    lua_pushvalue(L, nextIndex + 1);
    lua_pushvalue(L, nextIndex + 2);
    // Call __next(obj, k)
    lua_call(L, 2, 2); // returns k, val

    int numElements{ 0 };
    while (lua_type(L, -2) != LUA_TNIL) {
        if (isMapOrArray && ctx.LimitArrayElements != -1 && numElements > ctx.LimitArrayElements) {
            break;
        }

#if !defined(NDEBUG)
        STDString key;
        if (lua_type(L, -2) == LUA_TSTRING) {
            key = lua_tostring(L, -2);
        } else if (lua_type(L, -2) == LUA_TNUMBER) {
            lua_pushvalue(L, -2);
            key = lua_tostring(L, -1);
            lua_pop(L, 1);
        }
#endif

        auto type = lua_type(L, -2);
        if (type == LUA_TSTRING) {
            size_t len{ 0 };
            auto key = lua_tolstring(L, -2, &len);
            writer.Key(key, (uint32_t)len);
        } else if (type == LUA_TNUMBER) {
            auto key = lua_tointeger(L, -2);
            if (isArray) {
                // Using sequential keys, nothing to do
            } else {
                lua_pushvalue(L, -2);
                size_t len{ 0 };
                auto key = lua_tolstring(L, -1, &len);
                writer.Key(key, (uint32_t)len);
                lua_pop(L, 1);
            }
        } else if ((type == LUA_TLIGHTCPPOBJECT || type == LUA_TCPPOBJECT) && ctx.StringifyInternalTypes) {
            int top = lua_gettop(L);
            lua_getglobal(L, "tostring");  /* function to be called */
            lua_pushvalue(L, -3);   /* value to print */
            lua_call(L, 1, 1);
            size_t len{ 0 };
            const char* key = lua_tolstring(L, -1, &len); /* get result */
            if (key) {
                writer.Key(key, (uint32_t)len);
            }
            int top2 = lua_gettop(L);
            lua_pop(L, 1);  /* pop result */
        } else {
            throw std::runtime_error("Can only stringify string or number table keys");
        }

        Stringify(L, -1, depth + 1, ctx, writer);

        // Push next, obj, k
        lua_pushvalue(L, nextIndex);
        lua_pushvalue(L, nextIndex + 1);
        lua_pushvalue(L, nextIndex + 3);
        lua_remove(L, -4);
        lua_remove(L, -4);
        // Call __next(obj, k)
        lua_call(L, 2, 2); // returns k, val
        numElements++;
    }

    lua_pop(L, 2);

    // Pop __next, obj, nil
    lua_pop(L, 3);

    if (isArray) {
        writer.EndArray();
    } else {
        writer.EndObject();
    }

    return true;
}

template <class Writer>
void StringifyTableAsObjectOrdered(lua_State * L, int index, unsigned depth, StringifyContext& ctx, Writer& writer)
{
    // Collect keys from object
    Array<std::variant<char const*, int64_t, double>> keys;

    lua_pushnil(L);
    if (index < 0) index--;

    while (lua_next(L, index) != 0) {
        if (lua_type(L, -2) == LUA_TSTRING) {
            auto key = lua_tostring(L, -2);
            keys.push_back(key);
        } else if (lua_type(L, -2) == LUA_TNUMBER) {
            if (lua_isinteger(L, -2)) {
                keys.push_back(lua_tointeger(L, -2));
            } else {
                keys.push_back(lua_tonumber(L, -2));
            }
        } else {
            throw std::runtime_error("Can only stringify string or number table keys");
        }

        lua_pop(L, 1);
    }

    // Sort keys
    std::sort(keys.begin(), keys.end(), [] (std::variant<char const*, int64_t, double> const& a, std::variant<char const*, int64_t, double> const& b) {
        // Separate int, double, string keys
        if (a.index() != b.index()) {
            return a.index() < b.index();
        }

        switch (a.index()) {
        case 0: return strcmp(std::get<char const*>(a), std::get<char const*>(b)) < 0;
        case 1: return std::get<int64_t>(a) < std::get<int64_t>(b);
        case 2: return std::get<double>(a) < std::get<double>(b);
        default: throw std::runtime_error("Missing key");
        }
    });

    // Fetch and write values
    writer.StartObject();

    for (auto const& key : keys) {
        switch (key.index()) {
        case 0:
        {
            auto k = std::get<char const*>(key);
            push(L, k);
            lua_rawget(L, index);
            writer.Key(k);
            break;
        }
        case 1:
        {
            auto k = std::get<int64_t>(key);
            lua_rawgeti(L, index, k);
            char iv[100];
            auto len = _snprintf_s(iv, std::size(iv), "%lld", k);
            writer.Key(iv, (uint32_t)len);
            break;
        }
        case 2:
        {
            auto k = std::get<double>(key);
            push(L, k);
            lua_rawget(L, index);
            char iv[100];
            auto len = _snprintf_s(iv, std::size(iv), "%lf", k);
            writer.Key(iv, (uint32_t)len);
            break;
        }
        default:
            throw std::runtime_error("Missing key");
        }

        Stringify(L, -1, depth + 1, ctx, writer);
        lua_pop(L, 1);
    }

    writer.EndObject();
}

template <class Writer>
void StringifyTableAsObjectUnordered(lua_State * L, int index, unsigned depth, StringifyContext& ctx, Writer& writer)
{
    writer.StartObject();
    lua_pushnil(L);

    if (index < 0) index--;

    while (lua_next(L, index) != 0) {
        if (lua_type(L, -2) == LUA_TSTRING) {
            size_t len{ 0 };
            auto key = lua_tolstring(L, -2, &len);
            writer.Key(key, (uint32_t)len);
        } else if (lua_type(L, -2) == LUA_TNUMBER) {
            lua_pushvalue(L, -2);
            size_t len{ 0 };
            auto key = lua_tolstring(L, -1, &len);
            writer.Key(key, (uint32_t)len);
            lua_pop(L, 1);
        } else {
            throw std::runtime_error("Can only stringify string or number table keys");
        }

        Stringify(L, -1, depth + 1, ctx, writer);
        lua_pop(L, 1);
    }

    writer.EndObject();
}

template <class Writer>
void StringifyTableAsArray(lua_State * L, int index, unsigned depth, StringifyContext& ctx, Writer& writer)
{
    writer.StartArray();
    lua_pushnil(L);

    if (index < 0) index--;

    while (lua_next(L, index) != 0) {
        Stringify(L, -1, depth + 1, ctx, writer);
        lua_pop(L, 1);
    }

    writer.EndArray();
}

template <class Writer>
void StringifyTable(lua_State * L, int index, unsigned depth, StringifyContext& ctx, Writer& writer)
{
    if (CheckForRecursion(L, index, ctx)) {
        writer.String("*RECURSION*");
        return;
    }

    if (lua_is_linear_array(L, index)) {
        StringifyTableAsArray(L, index, depth, ctx, writer);
    } else {
        if (ctx.Beautify) {
            StringifyTableAsObjectOrdered(L, index, depth, ctx, writer);
        } else {
            StringifyTableAsObjectUnordered(L, index, depth, ctx, writer);
        }
    }
}

template <class Writer>
void StringifyInternalType(lua_State * L, int index, StringifyContext& ctx, Writer& writer)
{
    if (ctx.StringifyInternalTypes) {
        writer.String(luaL_tolstring(L, index, NULL));
        lua_pop(L, 1);
    } else {
        throw std::runtime_error("Attempted to stringify unsupported type: " + std::string(GetDebugName(L, index)));
    }
}

template <class Writer>
void StringifyBitfield(CppObjectMetadata& self, Writer& writer)
{
    writer.StartArray();
    auto ei = BitfieldValueMetatable::GetBitfieldInfo(self);
    for (auto const& val : ei->Values) {
        if ((self.Value & val.Value) == val.Value) {
            auto sv = val.Key.GetStringView();
            writer.String(sv.data(), (uint32_t)sv.size());
        }
    }
    
    writer.EndArray();
}

template <class Writer>
void TryStringifyLightCppObject(lua_State* L, int index, unsigned depth, StringifyContext& ctx, Writer& writer)
{
    index = lua_absindex(L, index);
    auto meta = lua_get_lightcppany(L, index);
    if (meta.MetatableTag == EnumValueMetatable::MetaTag) {
        auto sv = EnumValueMetatable::GetLabel(meta).GetStringView();
        writer.String(sv.data(), (uint32_t)sv.size());
        return;
    }

    if (meta.MetatableTag == BitfieldValueMetatable::MetaTag) {
        StringifyBitfield(meta, writer);
        return;
    }

    if (ctx.IterateUserdata) {
        if (ctx.LimitDepth != -1 && depth > (uint32_t)ctx.LimitDepth) {
            writer.String("*DEPTH LIMIT EXCEEDED*");
            return;
        }

        if (StringifyLightCppObject(L, index, meta, depth, ctx, writer)) {
            return;
        }
    }

    StringifyInternalType(L, index, ctx, writer);
}

template <class Writer>
void Stringify(lua_State * L, int index, unsigned depth, StringifyContext& ctx, Writer& writer)
{
    if (depth > ctx.MaxDepth) {
        throw std::runtime_error("Recursion depth exceeded while stringifying JSON");
    }

    switch (lua_type(L, index)) {
    case LUA_TNIL:
        writer.Null();
        break;

    case LUA_TBOOLEAN:
        writer.Bool(lua_toboolean(L, index) == 1);
        break;

    case LUA_TNUMBER:
        if (lua_isinteger(L, index)) {
            writer.Int64(lua_tointeger(L, index));
        } else {
            writer.Double(lua_tonumber(L, index));
        }
        break;

    case LUA_TSTRING:
    {
        size_t len{ 0 };
        auto str = lua_tolstring(L, index, &len);
        writer.String(str, (uint32_t)len);
        break;
    }

    case LUA_TTABLE:
        if (ctx.LimitDepth != -1 && depth > (uint32_t)ctx.LimitDepth) {
            writer.String("*DEPTH LIMIT EXCEEDED*");
        } else {
            StringifyTable(L, lua_absindex(L, index), depth, ctx, writer);
        }
        break;

    case LUA_TLIGHTCPPOBJECT:
    case LUA_TCPPOBJECT:
        TryStringifyLightCppObject(L, index, depth, ctx, writer);
        break;

    case LUA_TLIGHTUSERDATA:
    case LUA_TFUNCTION:
    case LUA_TTHREAD:
        StringifyInternalType(L, index, ctx, writer);
        break;

    default:
        throw std::runtime_error("Attempted to stringify an unknown type");
    }
}


std::string Stringify(lua_State * L, StringifyContext& ctx, int index)
{
    StackCheck _(L);

    StringBuffer sb;
    if (ctx.Beautify) {
        PrettyWriter<StringBuffer> writer(sb);
        Stringify(L, index, 0, ctx, writer);
    } else {
        Writer<StringBuffer> writer(sb);
        Stringify(L, index, 0, ctx, writer);
    }

    return sb.GetString();
}

UserReturn LuaStringify(lua_State * L)
{
    StackCheck _(L, 1);
    int nargs = lua_gettop(L);
    if (nargs < 1) {
        return luaL_error(L, "Stringify expects at least one parameter.");
    }

    if (nargs > 4) {
        return luaL_error(L, "Stringify expects at most three parameters.");
    }

    StringifyContext ctx;

    if (nargs >= 2) {
        // New stringify API - Json.Stringify(obj, paramTable)
        if (lua_type(L, 2) == LUA_TTABLE) {
            ctx.Beautify = try_gettable<bool>(L, "Beautify", 2, true);
            ctx.StringifyInternalTypes = try_gettable<bool>(L, "StringifyInternalTypes", 2, false);
            ctx.IterateUserdata = try_gettable<bool>(L, "IterateUserdata", 2, false);
            ctx.AvoidRecursion = try_gettable<bool>(L, "AvoidRecursion", 2, false);
            ctx.MaxDepth = try_gettable<uint32_t>(L, "MaxDepth", 2, 64);
            ctx.LimitDepth = try_gettable<int32_t>(L, "LimitDepth", 2, -1);
            ctx.LimitArrayElements = try_gettable<int32_t>(L, "LimitArrayElements", 2, -1);

            if (ctx.MaxDepth > 64) {
                ctx.MaxDepth = 64;
            }
        } else {
            // Old stringify API - Json.Stringify(obj, beautify, stringifyInternalTypes, iterateUserdata)
            ctx.Beautify = lua_toboolean(L, 2) == 1;

            if (nargs >= 3) {
                ctx.StringifyInternalTypes = lua_toboolean(L, 3) == 1;
            }

            if (nargs >= 4) {
                ctx.IterateUserdata = lua_toboolean(L, 4) == 1;
            }
        }
    }

    DisablePropertyWarnings();
    try {
        push(L, Stringify(L, ctx, 1));
    } catch (std::runtime_error& e) {
        return luaL_error(L, "%s", e.what());
    }
    EnablePropertyWarnings();

    return 1;
}

void RegisterJsonLib()
{
    DECLARE_MODULE(Json, Both)
    BEGIN_MODULE()
    MODULE_NAMED_FUNCTION("Parse", LuaParse)
    MODULE_NAMED_FUNCTION("Stringify", LuaStringify)
    END_MODULE()
}

END_NS()
