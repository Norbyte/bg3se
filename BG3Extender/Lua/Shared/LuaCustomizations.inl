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

// Lua C++ objects store an additional lifetime in the Lua value; however, for optimization purposes
// only the lower 48 bits of the pointer are stored.
// (Even though pointers in x64 are 64-bit, it only actually uses 48 bits; upper 16 bits are reserved.)
// HOWEVER, this means that this storage format should not be used for values that use all 64 bits
// (namely, bitfields).
// Note: The type tag and meta tag fields must be kept at the same offset in each layout for cross-format compatibility.
// 
// 0        8        16       24       32       40       48       56       64
// |--------|--------|--------|--------|--------|--------|--------|--------|
// |  Property Map   |                        Pointer                      |
// |--------|--------|--------|--------|--------|--------|--------|--------|
// |                        Lifetime                     |  Meta  |  Type  |
// |--------|--------|--------|--------|--------|--------|--------|--------|
//
struct CppLightObjectVal
{
    // HW pointer size on x64 hardware; upper 16 bits always 0
    static constexpr unsigned PointerBits = 48;
    static constexpr unsigned PropertyMapTagBits = sizeof(std::uintptr_t)*8 - PointerBits;
    static constexpr std::uintptr_t PointerMask = (1ull << PointerBits) - 1;
    static constexpr std::uintptr_t PropertyMapTagMask = (1ull << PropertyMapTagBits) - 1;
    // First byte of extra data reserved for Lua type tag (tt_)
    static constexpr unsigned TypeTagBits = 8;
    static constexpr unsigned MetatableTagBits = 7;
    static constexpr std::uintptr_t TypeTagMask = (1ull << TypeTagBits) - 1;
    static constexpr std::uintptr_t MetatableTagMask = (1ull << MetatableTagBits) - 1;
    static_assert(LifetimeHandle::HandleBits + TypeTagBits <= 56);

    CppLightObjectVal(TValue* val)
        : value_(val)
    {}
    
    CppLightObjectVal(lua_State* L, int idx)
        : value_(lua_index2addr(L, idx))
    {
        if (!IsCppObject(value_)) {
            luaL_error(L, "Param %d: expected a light C++ object, got %s", idx, GetDebugName(L, idx));
        }
    }

    static inline bool IsCppObject(TValue* val)
    {
        return ttislightcppobject(val)
            && ValueClassFromExtra(valextra(val)) == 1;
    }

    static inline void* PtrFromRaw(uint64_t rawValue)
    {
        return (void*)(rawValue & PointerMask);
    }

    static inline uint16_t PropertyMapTagFromRaw(uint64_t rawValue)
    {
        return (uint16_t)((rawValue >> PointerBits) & PropertyMapTagMask);
    }

    static inline LifetimeHandle LifetimeFromExtra(uint64_t extra)
    {
        return LifetimeHandle((extra >> TypeTagBits) & LifetimeHandle::HandleMask);
    }

    static inline uint64_t TypeTagFromExtra(uint64_t extra)
    {
        return (extra & TypeTagMask);
    }

    static inline MetatableTag MetatableTagFromExtra(uint64_t extra)
    {
        return (lua::MetatableTag)(TypeTagFromExtra(extra) & MetatableTagMask);
    }

    static inline uint8_t ValueClassFromExtra(uint64_t extra)
    {
        return (uint8_t)(TypeTagFromExtra(extra) >> MetatableTagBits);
    }

    static inline uint64_t MakeRaw(void const* ptr, uint16_t propertyMapTag)
    {
        return ((uint64_t)ptr & PointerMask) | (((uint64_t)propertyMapTag & PropertyMapTagMask) << PointerBits);
    }

    static inline uint64_t MakeExtra(MetatableTag metatableTag, LifetimeHandle const& lifetime)
    {
        return (((uint64_t)lifetime.handle_ & LifetimeHandle::HandleMask) << TypeTagBits)
            | (1ull << MetatableTagBits) // Value class
            | ((uint64_t)metatableTag & MetatableTagMask);
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
        return PtrFromRaw(RawValue());
    }

    inline uint16_t PropertyMapTag() const
    {
        return PropertyMapTagFromRaw(RawValue());
    }

    inline LifetimeHandle Lifetime(lua_State* L) const
    {
        return LifetimeFromExtra(Extra());
    }

    inline MetatableTag MetatableTag() const
    {
        return MetatableTagFromExtra(Extra());
    }

    TValue* value_;
};


// C++ object storage format that keeps a copy of the C++ object as a Lua userdata.
// This format has no pointer hacks, as Lua dereferences the full 64-bit value (GCobject); in addition,
// all of the metadata needs to be kept behind the udata pointer, we can't add data to the extra_ field.
struct CppObjectVal
{
    CppObjectVal(TValue* val)
        : value_(val)
    {}
    
    CppObjectVal(lua_State* L, int idx)
        : value_(lua_index2addr(L, idx))
    {
        if (!IsCppObject(value_)) {
            luaL_error(L, "Param %d: expected a C++ object, got %s", idx, GetDebugName(L, idx));
        }
    }

    static inline bool IsCppObject(TValue* val)
    {
        return ttiscppobject(val);
    }

    inline CppObjectUdata* GetUdata() const
    {
        auto val = cppvalue(value_);
        return (CppObjectUdata *)getcppmem(val);
    }

    inline void* Ptr() const
    {
        auto udata = GetUdata();
        return (void*)(udata + 1);
    }

    inline uint16_t PropertyMapTag() const
    {
        auto udata = GetUdata();
        return udata->PropertyMap;
    }

    inline LifetimeHandle Lifetime(lua_State* L) const
    {
        return State::FromLua(L)->GetGlobalLifetime();
    }

    inline MetatableTag MetatableTag() const
    {
        auto udata = GetUdata();
        return udata->Metatable;
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
    // NOTE: TypeTagOffset must match offset in CppPointerVal!
    static constexpr unsigned TypeTagOffset = 0;
    static constexpr unsigned TypeTagBits = 8;
    static constexpr unsigned MetatableTagBits = 7;
    static constexpr uint64_t TypeTagMask = (1ull << TypeTagBits) - 1;
    static constexpr uint64_t MetatableTagMask = (1ull << MetatableTagBits) - 1;

    static constexpr unsigned PropertyMapTagOffset = TypeTagBits;
    static constexpr unsigned PropertyMapTagBits = sizeof(std::uintptr_t) * 8 - TypeTagBits - 8 /* Lua tt_ */;
    static constexpr uint64_t PropertyMapTagMask = (1ull << PropertyMapTagBits) - 1;

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
            && ValueClassFromExtra(valextra(val)) == 0;
    }

    static inline uint64_t TypeTagFromExtra(uint64_t extra)
    {
        return ((extra >> TypeTagOffset) & TypeTagMask);
    }

    static inline MetatableTag MetatableTagFromExtra(uint64_t extra)
    {
        return (lua::MetatableTag)(TypeTagFromExtra(extra) & MetatableTagMask);
    }

    static inline uint8_t ValueClassFromExtra(uint64_t extra)
    {
        return (uint8_t)(TypeTagFromExtra(extra) >> MetatableTagBits);
    }

    static inline uint32_t PropertyMapTagFromExtra(uint64_t extra)
    {
        return (uint32_t)((extra >> PropertyMapTagOffset) & PropertyMapTagMask);
    }

    static inline uint64_t MakeRaw(uint64_t value)
    {
        return value;
    }

    static inline uint64_t MakeExtra(MetatableTag metatableTag, uint32_t propertyMapTag)
    {
        return (((uint64_t)propertyMapTag & PropertyMapTagMask) << PropertyMapTagOffset)
            | (0ull << MetatableTagBits) // Value class
            | (((uint64_t)metatableTag & MetatableTagMask) << TypeTagOffset);
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
        return PropertyMapTagFromExtra(Extra());
    }

    inline MetatableTag MetatableTag() const
    {
        return MetatableTagFromExtra(Extra());
    }

    TValue* value_;
};


void* lua_push_newcppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, uint32_t size)
{
    assert(propertyMapIndex >= 0);
    auto sz = size + sizeof(CppObjectUdata);
    auto obj = reinterpret_cast<CppObjectUdata*>(lua_newcppobject(L, 0, 0, sz));
    obj->Metatable = metatableTag;
    obj->PropertyMap = (uint32_t)propertyMapIndex;
    return obj + 1;
}

void lua_push_lightcppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, void* object, LifetimeHandle const& lifetime)
{
    assert(propertyMapIndex >= 0);
    auto val = CppLightObjectVal::MakeRaw(object, propertyMapIndex);
    auto extra = CppLightObjectVal::MakeExtra(metatableTag, lifetime);
    lua_pushlightcppobject(L, val, extra);
}

void lua_push_lightcppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, void const* object, LifetimeHandle const& lifetime)
{
    // TODO - add RO tag
    assert(propertyMapIndex >= 0);
    auto val = CppLightObjectVal::MakeRaw(object, propertyMapIndex);
    auto extra = CppLightObjectVal::MakeExtra(metatableTag, lifetime);
    lua_pushlightcppobject(L, val, extra);
}

CppObjectMetadata lua_get_lightcppobject(lua_State* L, int idx)
{
    CppLightObjectVal val(L, idx);
    return CppObjectMetadata{
        .Ptr = val.Ptr(),
        .MetatableTag = val.MetatableTag(),
        .PropertyMapTag = val.PropertyMapTag(),
        .Lifetime = val.Lifetime(L)
    };
}

CppObjectMetadata lua_get_lightcppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag)
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
        .Lifetime = val.Lifetime(L)
    };
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

    return CppObjectMetadata{
        .Ptr = val.Ptr(),
        .MetatableTag = metatableTag,
        .PropertyMapTag = propertyMap,
        .Lifetime = val.Lifetime(L)
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
    obj.Lifetime = val.Lifetime(L);
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
    obj.Lifetime = val.Lifetime(L);
    return true;
}



void lua_push_cppvalue(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, uint64_t object)
{
    assert(propertyMapIndex >= 0);
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
        .PropertyMapTag = val.PropertyMapTag()
    };
}

CppObjectMetadata lua_get_cppvalue(lua_State* L, int idx)
{
    CppValue val(L, idx);
    return CppObjectMetadata{
        .Value = val.Value(),
        .MetatableTag = val.MetatableTag(),
        .PropertyMapTag = val.PropertyMapTag()
    };
}

CppObjectMetadata lua_get_cppvalue(lua_State* L, TValue* v)
{
    CppValue val(v);
    return CppObjectMetadata{
        .Value = val.Value(),
        .MetatableTag = val.MetatableTag(),
        .PropertyMapTag = val.PropertyMapTag()
    };
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
    if (!ttislightcppobject(val)) {
        luaL_error(L, "Param %d: expected a light C++ object, got %s", idx, GetDebugName(L, idx));
    }

    if (CppLightObjectVal::ValueClassFromExtra(valextra(val)) == 0) {
        CppValue v(val);
        return CppObjectMetadata{
            .Value = v.Value(),
            .MetatableTag = v.MetatableTag(),
            .PropertyMapTag = v.PropertyMapTag()
        };
    } else {
        CppLightObjectVal v(val);
        return CppObjectMetadata{
            .Ptr = v.Ptr(),
            .MetatableTag = v.MetatableTag(),
            .PropertyMapTag = v.PropertyMapTag(),
            .Lifetime = v.Lifetime(L)
        };
    }
}

void* LuaCppAlloc(lua_State* L, size_t size)
{
    throw std::runtime_error("Unsupported!");
}

void LuaCppFree(lua_State* L, void* block, size_t size)
{
    throw std::runtime_error("Unsupported!");
}

CMetatable* LuaCppGetLightMetatable(lua_State* L, unsigned long long val, unsigned long long extra)
{
    // Don't check lifetime here, as fetching metatable requires no access to the underlying object.
    // (Also the caller can cache the metamethod so checking here is useless)

    auto metatableTag = CppLightObjectVal::MetatableTagFromExtra(extra);
    return State::FromLua(L)->GetMetatableManager().GetMetatable(metatableTag);
}

CMetatable* LuaCppGetMetatable(lua_State* L, void* val, unsigned long long extra)
{
    throw std::runtime_error("Unsupported!");
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

void push(lua_State* L, FixedString const& v)
{
    lua_lock(L);
    TString* ts;
    if (v) {
        ts = luaS_new(L, v.GetString());
        LuaCacheString(L, ts);
    } else {
        ts = luaS_new(L, "");
    }

    setsvalue2s(L, L->top, ts);
    api_incr_top(L);
    luaC_checkGC(L);
    lua_unlock(L);
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
