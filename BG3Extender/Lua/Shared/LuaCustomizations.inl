BEGIN_NS(lua)

struct LuaInternalState
{
    TValue canonicalizationCache;
};

struct CppObjectUdata
{
    union
    {
        uint64_t _Pad; // Force 8-byte alignment
        struct
        {
            MetatableTag Metatable;
            uint32_t PropertyMap;
        };
    };
};

struct CachedFixedString
{
    FixedString Str;
    bool IsCached;
};

#define BIT_SUBFIELD(name, ty, offset, bits) \
    static constexpr unsigned name##Bits = bits; \
    static constexpr unsigned name##Offset = offset; \
    static constexpr uint64_t name##Mask = (1ull << name##Bits) - 1; \
    static_assert(name##Bits + name##Offset <= 64); \
    static_assert(name##Bits <= sizeof(ty)*8); \
    \
    inline static constexpr ty Decode##name(uint64_t val) \
    { \
        return (ty)((val >> name##Offset) & name##Mask); \
    } \
    \
    inline static constexpr uint64_t Encode##name(ty val) \
    { \
        return ((uint64_t)val << name##Offset); \
    }


// Lua C++ objects store an additional lifetime in the Lua value; however, for optimization purposes
// only the lower 48 bits of the pointer are stored.
// (Even though pointers in x64 are 64-bit, it only actually uses 48 bits; upper 16 bits are reserved.)
// HOWEVER, this means that this storage format should not be used for values that use all 64 bits
// (namely, bitfields).
// Note: The type tag and meta tag fields must be kept at the same offset in each layout for cross-format compatibility.
// 
// 0        8        16       24       32       40       48       56       64
// |--------|--------|--------|--------|--------|--------|--------|--------|
// |   Lifetime 1    |                        Pointer                      |
// |--------|--------|--------|--------|--------|--------|--------|--------|
// |             Lifetime 2            |  Property Map   |  Meta  |  Type  |
// |--------|--------|--------|--------|--------|--------|--------|--------|
//
// The heavy-object variant (LUA_TCPPOBJECT) keeps a copy of the C++ object as a Lua userdata.
// This format has no pointer hacks, as Lua dereferences the full 64-bit value (GCobject); in addition,
// all of the metadata needs to be kept behind the udata pointer, we can't just rely on the extra_ field.
// As the object is managed by the Lua GC, there are no lifetime constraints on these objects.
// 
// 0        8        16       24       32       40       48       56       64
// |--------|--------|--------|--------|--------|--------|--------|--------|
// |                                Pointer                                |
// |--------|--------|--------|--------|--------|--------|--------|--------|
// |               Unused              |  Property Map   |  Meta  |  Type  |
// |--------|--------|--------|--------|--------|--------|--------|--------|
struct CppLightObjectVal
{
    BIT_SUBFIELD(RawPointer, uint64_t, 0, 48)
    BIT_SUBFIELD(Lifetime1, uint64_t, 48, 16)
       
    // Note: Extra data starts at offset 8, i.e. Metatype is at 0 here
    BIT_SUBFIELD(MetatableTag, uint8_t, 0, 7)
    BIT_SUBFIELD(ValueClass, uint8_t, 7, 1)
    BIT_SUBFIELD(PropertyMapTag, uint16_t, 8, 16)
    BIT_SUBFIELD(Lifetime2, uint64_t, 24, 32)

    inline CppLightObjectVal(TValue* val)
        : value_(val)
    {}
    
    inline CppLightObjectVal(lua_State* L, int idx)
        : value_(lua_index2addr(L, idx))
    {
        if (!IsCppObject(value_)) {
            luaL_error(L, "Param %d: expected a light C++ object, got %s", idx, GetDebugName(L, idx));
        }
    }

    static inline bool IsCppObject(TValue* val)
    {
        return (ttislightcppobject(val) || ttiscppobject(val))
            && DecodeValueClass(val) == 1;
    }

    static inline LifetimeHandle DecodeLifetime(uint64_t rawValue, uint64_t extra)
    {
        return LifetimeHandle((DecodeLifetime1(rawValue) << 32) | DecodeLifetime2(extra));
    }

    static inline void* DecodeRawPointer(TValue* value)
    {
        return (void*)DecodeRawPointer(lcppvalue(value));
    }

    static inline uint16_t DecodePropertyMapTag(TValue* value)
    {
        return DecodePropertyMapTag(valextra(value));
    }

    static inline LifetimeHandle DecodeLifetime(TValue* value)
    {
        return DecodeLifetime(lcppvalue(value), valextra(value));
    }

    static inline MetatableTag DecodeMetatableTag(TValue* value)
    {
        return (lua::MetatableTag)(DecodeMetatableTag(valextra(value)));
    }

    static inline uint8_t DecodeValueClass(TValue* value)
    {
        return DecodeValueClass(valextra(value));
    }

    static inline uint64_t MakeRaw(void const* ptr, LifetimeHandle lifetime)
    {
        return EncodeRawPointer((uint64_t)ptr) | EncodeLifetime1(lifetime.handle_ >> 32);
    }

    static inline uint64_t MakeExtra(MetatableTag metatableTag, uint16_t propertyMapTag, LifetimeHandle lifetime)
    {
        return EncodeMetatableTag((uint8_t)metatableTag)
            | EncodeValueClass(1)
            | EncodePropertyMapTag(propertyMapTag)
            | EncodeLifetime2(lifetime.handle_ & 0xffffffffull);
    }

    static inline CppObjectUdata* GetUdata(TValue* value)
    {
        auto val = cppvalue(value);
        return (CppObjectUdata*)getcppmem(val);
    }

    static inline void* DecodeObjectPointer(TValue* value)
    {
        if (ttislightcppobject(value)) {
            return DecodeRawPointer(value);
        } else {
            auto udata = GetUdata(value);
            return (void*)(udata + 1);
        }
    }

    inline uint64_t RawValue() const
    {
        return lcppvalue(value_);
    }

    inline uint64_t Extra() const
    {
        return valextra(value_);
    }

    inline void* Ptr() const
    {
        return DecodeObjectPointer(value_);
    }

    inline uint16_t PropertyMapTag() const
    {
        return DecodePropertyMapTag(value_);
    }

    inline LifetimeHandle Lifetime() const
    {
        return DecodeLifetime(value_);
    }

    inline MetatableTag MetatableTag() const
    {
        return DecodeMetatableTag(value_);
    }

    TValue* value_;
};




// Light C++ object storage format that keeps all 64-bits of the value;
// the property map tag is moved to the "extra" word, and no lifetime is kept.
// 
// 0        8        16       24       32       40       48       56       64
// |--------|--------|--------|--------|--------|--------|--------|--------|
// |                                 Value                                 |
// |--------|--------|--------|--------|--------|--------|--------|--------|
// |               Unused              |  Property Map   |  Meta  |  Type  |
// |--------|--------|--------|--------|--------|--------|--------|--------|
//
struct CppValue
{
    // NOTE: Offsets must match offset in CppPointerVal!
    // Extra data starts at offset 8, i.e. Metatype is at 0 here
    BIT_SUBFIELD(MetatableTag, uint8_t, 0, 7)
    BIT_SUBFIELD(ValueClass, uint8_t, 7, 1)
    BIT_SUBFIELD(PropertyMapTag, uint16_t, 8, 16)


    CppValue(TValue* val)
        : value_(val)
    {}
    
    CppValue(lua_State* L, int idx)
        : value_(lua_index2addr(L, idx))
    {
        if (!IsCppObject(value_)) {
            luaL_error(L, "Param %d: expected a C++ value, got %s", idx, GetDebugName(L, idx));
        }
    }

    static inline bool IsCppObject(TValue* val)
    {
        return ttislightcppobject(val)
            && DecodeValueClass(valextra(val)) == 0;
    }

    static inline uint64_t DecodeValue(TValue* value)
    {
        return lcppvalue(value);
    }

    static inline uint16_t DecodePropertyMapTag(TValue* value)
    {
        return DecodePropertyMapTag(valextra(value));
    }

    static inline MetatableTag DecodeMetatableTag(TValue* value)
    {
        return (lua::MetatableTag)(DecodeMetatableTag(valextra(value)));
    }

    static inline uint8_t DecodeValueClass(TValue* value)
    {
        return DecodeValueClass(valextra(value));
    }

    static inline uint64_t MakeRaw(uint64_t value)
    {
        return value;
    }

    static inline uint64_t MakeExtra(MetatableTag metatableTag, uint16_t propertyMapTag)
    {
        return EncodeMetatableTag((uint8_t)metatableTag)
            | EncodeValueClass(0)
            | EncodePropertyMapTag(propertyMapTag);
    }


    inline uint64_t RawValue() const
    {
        return lcppvalue(value_);
    }

    inline uint64_t Extra() const
    {
        return valextra(value_);
    }

    inline uint64_t Value() const
    {
        return RawValue();
    }

    inline uint16_t PropertyMapTag() const
    {
        return DecodePropertyMapTag(value_);
    }

    inline MetatableTag MetatableTag() const
    {
        return DecodeMetatableTag(value_);
    }

    TValue* value_;
};


void* lua_push_newcppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, uint32_t size)
{
    se_assert(propertyMapIndex >= 0);
    auto sz = size + sizeof(CppObjectUdata);
    auto extra = CppLightObjectVal::MakeExtra(metatableTag, (uint16_t)propertyMapIndex, LifetimeHandle());
    auto obj = reinterpret_cast<CppObjectUdata*>(lua_newcppobject(L, extra, sz));
    obj->Metatable = metatableTag;
    obj->PropertyMap = (uint32_t)propertyMapIndex;
    return obj + 1;
}

void lua_push_lightcppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, void* object, LifetimeHandle lifetime)
{
    se_assert(propertyMapIndex >= 0);
    auto val = CppLightObjectVal::MakeRaw(object, lifetime);
    auto extra = CppLightObjectVal::MakeExtra(metatableTag, (uint16_t)propertyMapIndex, lifetime);
    lua_pushlightcppobject(L, val, extra);
}

void lua_push_lightcppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, void const* object, LifetimeHandle lifetime)
{
    // TODO - add RO tag
    se_assert(propertyMapIndex >= 0);
    auto val = CppLightObjectVal::MakeRaw(object, lifetime);
    auto extra = CppLightObjectVal::MakeExtra(metatableTag, (uint16_t)propertyMapIndex, lifetime);
    lua_pushlightcppobject(L, val, extra);
}

CppObjectMetadata lua_get_lightcppobject(lua_State* L, int idx)
{
    CppLightObjectVal val(L, idx);
    return CppObjectMetadata{
        .Ptr = val.Ptr(),
        .MetatableTag = val.MetatableTag(),
        .PropertyMapTag = val.PropertyMapTag(),
        .Lifetime = val.Lifetime()
    };
}

CppObjectMetadata lua_get_unchecked_lightcppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag)
{
    CppLightObjectVal val(L, idx);
    auto metatableTag = val.MetatableTag();
    if (metatableTag != expectedMetatableTag) {
        luaL_error(L, "Param %d must be a %s; got %s", idx, GetDebugName(expectedMetatableTag), GetDebugName(L, idx));
    }

    return CppObjectMetadata{
        .Ptr = val.Ptr(),
        .MetatableTag = metatableTag,
        .PropertyMapTag = val.PropertyMapTag(),
        .Lifetime = val.Lifetime()
    };
}

CppObjectMetadata lua_get_lightcppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag)
{
    CppLightObjectVal val(L, idx);
    auto metatableTag = val.MetatableTag();
    if (metatableTag != expectedMetatableTag) {
        luaL_error(L, "Param %d must be a %s; got %s", idx, GetDebugName(expectedMetatableTag), GetDebugName(L, idx));
    }

    auto lifetime = val.Lifetime();
    if (!lifetime.IsAlive(L)) {
        luaL_error(L, "Param %d: got %s whose lifetime has expired", idx, GetDebugName(L, idx));
    }

    return CppObjectMetadata{
        .Ptr = val.Ptr(),
        .MetatableTag = metatableTag,
        .PropertyMapTag = val.PropertyMapTag(),
        .Lifetime = lifetime
    };
}

CppObjectOpaque* lua_get_opaque_lightcppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag)
{
    CppLightObjectVal val(L, idx);
    auto metatableTag = val.MetatableTag();
    if (metatableTag != expectedMetatableTag) {
        luaL_error(L, "Param %d must be a %s; got %s", idx, GetDebugName(expectedMetatableTag), GetDebugName(L, idx));
    }

    auto lifetime = val.Lifetime();
    if (!lifetime.IsAlive(L)) {
        luaL_error(L, "Param %d: got %s whose lifetime has expired", idx, GetDebugName(L, idx));
    }

    return (CppObjectOpaque*)val.value_;
}

LifetimeHandle lua_get_opaque_lifetime(CppObjectOpaque* meta)
{
    return CppLightObjectVal::DecodeLifetime((TValue*)meta);
}

uint16_t lua_get_opaque_property_map(CppObjectOpaque* meta)
{
    return CppLightObjectVal::DecodePropertyMapTag((TValue*)meta);
}

void* lua_get_opaque_ptr(CppObjectOpaque* meta)
{
    return CppLightObjectVal::DecodeObjectPointer((TValue*)meta);
}

CppObjectMetadata lua_get_lightcppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag, int expectedPropertyMap)
{
    CppLightObjectVal val(L, idx);
    auto metatableTag = val.MetatableTag();
    if (metatableTag != expectedMetatableTag) {
        luaL_error(L, "Param %d must be a %s; got %s", idx, GetDebugName(expectedMetatableTag, expectedPropertyMap), GetDebugName(L, idx));
    }

    auto propertyMap = val.PropertyMapTag();
    if (propertyMap != expectedPropertyMap) {
        luaL_error(L, "Param %d must be a %s; got %s", idx, GetDebugName(expectedMetatableTag, expectedPropertyMap), GetDebugName(L, idx));
    }

    auto lifetime = val.Lifetime();
    if (!lifetime.IsAlive(L)) {
        luaL_error(L, "Param %d: got %s whose lifetime has expired", idx, GetDebugName(L, idx));
    }

    return CppObjectMetadata{
        .Ptr = val.Ptr(),
        .MetatableTag = metatableTag,
        .PropertyMapTag = propertyMap,
        .Lifetime = lifetime
    };
}

bool lua_try_get_lightcppobject(lua_State* L, int idx, MetatableTag expectedTypeTag, CppObjectMetadata& obj)
{
    auto value = lua_index2addr(L, idx);
    if (!CppLightObjectVal::IsCppObject(value)) {
        return false;
    }

    CppLightObjectVal val(value);
    auto metatableTag = val.MetatableTag();
    if (metatableTag != expectedTypeTag) {
        return false;
    }

    obj.Ptr = val.Ptr();
    obj.MetatableTag = metatableTag;
    obj.PropertyMapTag = val.PropertyMapTag();
    obj.Lifetime = val.Lifetime();
    return true;
}

bool lua_try_get_lightcppobject(lua_State* L, int idx, CppObjectMetadata& obj)
{
    auto value = lua_index2addr(L, idx);
    if (!CppLightObjectVal::IsCppObject(value)) {
        return false;
    }

    CppLightObjectVal val(value);
    obj.Ptr = val.Ptr();
    obj.MetatableTag = val.MetatableTag();
    obj.PropertyMapTag = val.PropertyMapTag();
    obj.Lifetime = val.Lifetime();
    return true;
}



void lua_push_cppvalue(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, uint64_t object)
{
    se_assert(propertyMapIndex >= 0);
    auto val = CppValue::MakeRaw(object);
    auto extra = CppValue::MakeExtra(metatableTag, propertyMapIndex);
    lua_pushlightcppobject(L, val, extra);
}

CppObjectMetadata lua_get_cppvalue(lua_State* L, int idx, MetatableTag expectedMetatableTag)
{
    CppValue val(L, idx);
    auto metatableTag = val.MetatableTag();
    if (metatableTag != expectedMetatableTag) {
        luaL_error(L, "Param %d must be a %s; got %s", idx, GetDebugName(expectedMetatableTag), GetDebugName(L, idx));
    }

    return CppObjectMetadata{
        .Value = val.Value(),
        .MetatableTag = metatableTag,
        .PropertyMapTag = val.PropertyMapTag(),
        .Lifetime = NullLifetime
    };
}

CppValueOpaque* lua_get_opaque_cppvalue(lua_State* L, int idx, MetatableTag expectedMetatableTag)
{
    CppValue val(L, idx);
    auto metatableTag = val.MetatableTag();
    if (metatableTag != expectedMetatableTag) {
        luaL_error(L, "Param %d must be a %s; got %s", idx, GetDebugName(expectedMetatableTag), GetDebugName(L, idx));
    }

    return (CppValueOpaque*)val.value_;
}

uint32_t lua_get_opaque_property_map(CppValueOpaque* meta)
{
    return CppValue::DecodePropertyMapTag((TValue*)meta);
}

uint64_t lua_get_opaque_value(CppValueOpaque* meta)
{
    return CppValue::DecodeValue((TValue*)meta);
}

CppObjectMetadata lua_get_cppvalue(lua_State* L, int idx)
{
    CppValue val(L, idx);
    return CppObjectMetadata{
        .Value = val.Value(),
        .MetatableTag = val.MetatableTag(),
        .PropertyMapTag = val.PropertyMapTag(),
        .Lifetime = NullLifetime
    };
}

CppObjectMetadata lua_get_cppvalue(lua_State* L, TValue* v)
{
    CppValue val(v);
    return CppObjectMetadata{
        .Value = val.Value(),
        .MetatableTag = val.MetatableTag(),
        .PropertyMapTag = val.PropertyMapTag(),
        .Lifetime = NullLifetime
    };
}

bool lua_typecheck_cppvalue(lua_State* L, int idx)
{
    auto val = lua_index2addr(L, idx);
    return ttislightcppobject(val) || ttiscppobject(val);
}

bool lua_typecheck_cppvalue(lua_State* L, int idx, MetatableTag expectedMetatableTag)
{
    auto val = lua_index2addr(L, idx);
    return (ttislightcppobject(val) || ttiscppobject(val))
        && CppValue::DecodeMetatableTag(val) == expectedMetatableTag;
}

bool lua_typecheck_cppvalue(lua_State* L, int idx, MetatableTag expectedMetatableTag, int propertyMapTag)
{
    auto val = lua_index2addr(L, idx);
    return (ttislightcppobject(val) || ttiscppobject(val))
        && CppValue::DecodeMetatableTag(val) == expectedMetatableTag
        && CppValue::DecodePropertyMapTag(val) == propertyMapTag;
}

bool lua_try_get_cppvalue(lua_State* L, int idx, MetatableTag expectedTypeTag, CppObjectMetadata& obj)
{
    auto value = lua_index2addr(L, idx);
    if (!CppValue::IsCppObject(value)) {
        return false;
    }

    CppValue val(value);
    auto metatableTag = val.MetatableTag();
    if (metatableTag != expectedTypeTag) {
        return false;
    }

    obj.Value = val.Value();
    obj.MetatableTag = metatableTag;
    obj.PropertyMapTag = val.PropertyMapTag();
    return true;
}

bool lua_try_get_cppvalue(lua_State* L, int idx, CppObjectMetadata& obj)
{
    auto value = lua_index2addr(L, idx);
    if (!CppValue::IsCppObject(value)) {
        return false;
    }

    CppValue val(value);
    obj.Value = val.Value();
    obj.MetatableTag = val.MetatableTag();
    obj.PropertyMapTag = val.PropertyMapTag();
    return true;
}


CppObjectMetadata lua_get_lightcppany(lua_State* L, int idx)
{
    auto val = lua_index2addr(L, idx);
    if (!ttislightcppobject(val) && !ttiscppobject(val)) {
        luaL_error(L, "Param %d: expected a light C++ object, got %s", idx, GetDebugName(L, idx));
    }

    if (CppLightObjectVal::DecodeValueClass(valextra(val)) == 0) {
        CppValue v(val);
        return CppObjectMetadata{
            .Value = v.Value(),
            .MetatableTag = v.MetatableTag(),
            .PropertyMapTag = v.PropertyMapTag(),
            .Lifetime = NullLifetime
        };
    } else {
        CppLightObjectVal v(val);
        return CppObjectMetadata{
            .Ptr = v.Ptr(),
            .MetatableTag = v.MetatableTag(),
            .PropertyMapTag = v.PropertyMapTag(),
            .Lifetime = v.Lifetime()
        };
    }
}

void* LuaCppAlloc(lua_State* L, size_t size)
{
    return GameAllocRaw(size);
}

void LuaCppFree(lua_State* L, void* block, size_t size)
{
    GameFree(block);
}

CMetatable* LuaCppGetLightMetatable(lua_State* L, unsigned long long val, unsigned long long extra)
{
    // Don't check lifetime here, as fetching metatable requires no access to the underlying object.
    // (Also the caller can cache the metamethod so checking here is useless)

    auto metatableTag = (MetatableTag)CppLightObjectVal::DecodeMetatableTag(extra);
    return State::FromLua(L)->GetMetatableManager().GetMetatable(metatableTag);
}

CMetatable* LuaCppGetMetatable(lua_State* L, void* val, unsigned long long extra)
{
    auto metatableTag = (MetatableTag)CppLightObjectVal::DecodeMetatableTag(extra);
    return State::FromLua(L)->GetMetatableManager().GetMetatable(metatableTag);
}

void LuaCppFinalize(lua_State* L, void* udata)
{
    auto obj = (CppObjectUdata*)udata;
    if (obj->Metatable == MetatableTag::ObjectRef) {
        auto pm = gStructRegistry.Get(obj->PropertyMap);
        pm->Destroy((void*)(obj + 1));
    } else {
        se_assert(false);
    }
}

LuaInternalState* lua_new_internal_state()
{
    auto st = GameAlloc<LuaInternalState>();
    setnilvalue(&st->canonicalizationCache);
    return st;
}

void lua_release_internal_state(LuaInternalState* state)
{
    GameDelete(state);
}

char const* lua_get_function_location(lua_State* L, int index, int& line)
{
    StkId o = index2addr(L, index);
    if (ttisLclosure(o)) {
        auto proto = clLvalue(o)->p;
        line = proto->linedefined;
        return getstr(proto->source);
    }

    line = 0;
    return "";
}

char const* lua_get_function_location(lua_State* L, RegistryEntry const& reg, int& line)
{
    if (reg) {
        reg.Push(L);
        auto source = lua_get_function_location(L, -1, line);
        lua_pop(L, 1);
        return source;
    } else {
        line = 0;
        return "";
    }
}

char const* lua_get_function_location(lua_State* L, Ref const& reg, int& line)
{
    if (reg) {
        reg.Push(L);
        auto source = lua_get_function_location(L, -1, line);
        lua_pop(L, 1);
        return source;
    } else {
        line = 0;
        return "";
    }
}

void* LuaCppCanonicalize(lua_State* L, void* val)
{
    auto v = reinterpret_cast<TValue*>(val);
    auto meta = lua_get_cppvalue(L, v);
    if (meta.MetatableTag != MetatableTag::EnumValue) {
        return nullptr;
    }

    auto state = State::FromLua(L)->GetInternalState();
    auto label = EnumValueMetatable::GetLabel(meta);
    if (label) {
        TString* ts;
        auto sv = label.GetStringView();
        ts = luaS_newlstr(L, sv.data(), sv.size());
        setsvalue2s(L, &state->canonicalizationCache, ts);
    } else {
        setnilvalue(&state->canonicalizationCache);
    }

    return &state->canonicalizationCache;
}

FixedString do_get(lua_State* L, int index, Overload<FixedString>)
{
    StkId o = index2addr(L, index);
    if (ttisstring(o)) {
        auto s = tsvalue(o);
        auto& fs = *reinterpret_cast<CachedFixedString *>(&s->cache);
        if (fs.IsCached) {
            return fs.Str;
        } else {
            return FixedString(StringView(getstr(s), tsslen(s)));
        }
    }

    if (ttisnil(o)) {
        return FixedString{};
    }

    size_t len;
    auto str = luaL_tolstring(L, index, &len);
    auto fs = FixedString(StringView(str, len));
    lua_pop(L, 1);
    return fs;
}

FixedStringNoRef do_get(lua_State* L, int index, Overload<FixedStringNoRef>)
{
    StkId o = index2addr(L, index);
    if (ttisstring(o)) {
        auto s = tsvalue(o);
        auto& fs = *reinterpret_cast<CachedFixedString *>(&s->cache);
        if (fs.IsCached) {
            return FixedStringNoRef{ fs.Str };
        } else {
            return FixedStringNoRef(StringView(getstr(s), tsslen(s)));
        }
    }

    if (ttisnil(o)) {
        return FixedString{};
    }

    size_t len;
    auto str = luaL_tolstring(L, index, &len);
    auto fs = FixedStringNoRef{ StringView(str, len) };
    lua_pop(L, 1);
    return fs;
}

void LuaCacheString(lua_State* L, TString* s, FixedString const& fstr)
{
    static_assert(sizeof(LUA_STRING_EXTRATYPE) == sizeof(CachedFixedString));
    auto fs = reinterpret_cast<CachedFixedString*>(&s->cache);
    new (&fs->Str) FixedString(fstr);
    fs->IsCached = true;
}

void push(lua_State* L, FixedString const& v)
{
    lua_lock(L);
    TString* ts;
    if (v) {
        ts = luaS_new(L, v.GetString());
        LuaCacheString(L, ts, v);
    } else {
        ts = luaS_new(L, "");
    }

    setsvalue2s(L, L->top, ts);
    api_incr_top(L);
    luaC_checkGC(L);
    lua_unlock(L);
}

void push(lua_State* L, FixedStringUnhashed const& v)
{
    push(L, *reinterpret_cast<FixedString const*>(&v));
}

void LuaCacheString(lua_State* L, TString* s)
{
    static_assert(sizeof(LUA_STRING_EXTRATYPE) == sizeof(CachedFixedString));
    auto fs = reinterpret_cast<CachedFixedString*>(&s->cache);
    if (!fs->IsCached && tsslen(s) <= 0x40) {
        // FIXME: Don't create GFS entries if the string doesn't already exist in GFS!
        // was previously: FixedString::DontCreate{}
        new (&fs->Str) FixedString(StringView(getstr(s), tsslen(s)));
        fs->IsCached = true;
    }
}

void LuaReleaseString(lua_State* L, TString* s)
{
    static_assert(sizeof(LUA_STRING_EXTRATYPE) == sizeof(CachedFixedString));
    auto fs = reinterpret_cast<CachedFixedString*>(&s->cache);
    if (fs->IsCached) {
        fs->Str.~FixedString();
        fs->IsCached = false;
    }
}

END_NS()
