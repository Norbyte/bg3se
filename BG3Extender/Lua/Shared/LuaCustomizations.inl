BEGIN_NS(lua)

struct LuaInternalState
{
	TValue canonicalizationCache;
};

struct CppObjectUdata
{
	uint64_t PtrAndPropertyMapTag;
	uint64_t LifetimeAndTypeTag;
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
struct CppPointerVal
{
	// HW pointer size on x64 hardware; upper 16 bits always 0
	static constexpr unsigned PointerBits = 48;
	static constexpr unsigned PropertyMapTagBits = sizeof(std::uintptr_t)*8 - PointerBits;
	static constexpr std::uintptr_t PointerMask = (1ull << PointerBits) - 1;
	static constexpr std::uintptr_t PropertyMapTagMask = (1ull << PropertyMapTagBits) - 1;
	// First byte of extra data reserved for Lua type tag (tt_)
	static constexpr unsigned TypeTagBits = 8;
	static constexpr std::uintptr_t TypeTagMask = (1ull << TypeTagBits) - 1;
	static_assert(LifetimeHandle::HandleBits + TypeTagBits <= 56);

	CppPointerVal(TValue* val)
		: value_(val)
	{}
	
	CppPointerVal(lua_State* L, int idx)
		: value_(lua_index2addr(L, idx))
	{
		if (!IsCppObject(value_)) {
			luaL_error(L, "Param %d: expected a C++ object, got %s", idx, lua_typename(L, lua_type(L, idx)));
		}
	}

	static inline bool IsCppObject(TValue* val)
	{
		return ttislightcppobject(val) || ttiscppobject(val);
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

	static inline MetatableTag MetatableTagFromExtra(uint64_t extra)
	{
		return (lua::MetatableTag)(extra & TypeTagMask);
	}

	static inline uint64_t MakeRaw(void const* ptr, uint16_t propertyMapTag)
	{
		return ((uint64_t)ptr & PointerMask) | (((uint64_t)propertyMapTag & PropertyMapTagMask) << PointerBits);
	}

	static inline uint64_t MakeExtra(MetatableTag metatableTag, LifetimeHandle const& lifetime)
	{
		return (((uint64_t)lifetime.handle_ & LifetimeHandle::HandleMask) << TypeTagBits)
			| ((uint64_t)metatableTag & TypeTagMask);
	}

	inline uint64_t RawValue() const
	{
		return lcppvalue(value_);
	}

	inline uint64_t Extra() const
	{
		return valextra(value_);
	}

	inline CppObjectUdata* GetUdata() const
	{
		assert(ttiscppobject(value_));
		auto val = cppvalue(value_);
		return (CppObjectUdata *)getcppmem(val);
	}

	inline void* Ptr() const
	{
		if (ttislightcppobject(value_)) {
			return PtrFromRaw(RawValue());
		} else {
			auto udata = GetUdata();
			return PtrFromRaw(udata->PtrAndPropertyMapTag);
		}
	}

	inline uint16_t PropertyMapTag() const
	{
		if (ttislightcppobject(value_)) {
			return PropertyMapTagFromRaw(RawValue());
		} else {
			auto udata = GetUdata();
			return PropertyMapTagFromRaw(udata->PtrAndPropertyMapTag);
		}
	}

	inline LifetimeHandle Lifetime() const
	{
		if (ttislightcppobject(value_)) {
			return LifetimeFromExtra(Extra());
		} else {
			auto udata = GetUdata();
			return LifetimeFromExtra(udata->LifetimeAndTypeTag);
		}
	}

	inline MetatableTag MetatableTag() const
	{
		if (ttislightcppobject(value_)) {
			return MetatableTagFromExtra(Extra());
		} else {
			auto udata = GetUdata();
			return MetatableTagFromExtra(udata->LifetimeAndTypeTag);
		}
	}

	TValue* value_;
};


// Light C++ object storage format that keeps all 64-bits of the value;
// the property map tag is moved to the "extra" word, and no lifetime is kept.
struct CppValue
{
	// NOTE: TypeTagOffset must match offset in CppPointerVal!
	static constexpr unsigned TypeTagOffset = 0;
	static constexpr unsigned TypeTagBits = 8;
	static constexpr std::uintptr_t TypeTagMask = (1ull << TypeTagBits) - 1;
	static constexpr unsigned PropertyMapTagOffset = TypeTagBits;
	static constexpr unsigned PropertyMapTagBits = sizeof(std::uintptr_t) * 8 - TypeTagBits - 8 /* Lua tt_ */;
	static constexpr std::uintptr_t PropertyMapTagMask = (1ull << PropertyMapTagBits) - 1;

	CppValue(TValue* val)
		: value_(val)
	{}
	
	CppValue(lua_State* L, int idx)
		: value_(lua_index2addr(L, idx))
	{
		if (!IsCppObject(value_)) {
			luaL_error(L, "Param %d must be a C++ object", idx);
		}
	}

	static inline bool IsCppObject(TValue* val)
	{
		return ttislightcppobject(val);
	}

	static inline MetatableTag MetatableTagFromExtra(uint64_t extra)
	{
		return (lua::MetatableTag)((extra >> TypeTagOffset) & TypeTagMask);
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
			 | (((uint64_t)metatableTag & TypeTagMask) << TypeTagOffset);
	}

	inline uint64_t RawValue() const
	{
		return lcppvalue(value_);
	}

	inline uint64_t Extra() const
	{
		return valextra(value_);
	}

	inline CppObjectUdata* GetUdata() const
	{
		assert(ttiscppobject(value_));
		auto val = cppvalue(value_);
		return (CppObjectUdata *)getcppmem(val);
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


void lua_push_cppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, void* object, LifetimeHandle const& lifetime)
{
	assert(propertyMapIndex >= 0);
	auto val = CppPointerVal::MakeRaw(object, propertyMapIndex);
	auto extra = CppPointerVal::MakeExtra(metatableTag, lifetime);
	lua_pushlightcppobject(L, val, extra);
}

void lua_push_cppobject(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, void const* object, LifetimeHandle const& lifetime)
{
	// TODO - add RO tag
	assert(propertyMapIndex >= 0);
	auto val = CppPointerVal::MakeRaw(object, propertyMapIndex);
	auto extra = CppPointerVal::MakeExtra(metatableTag, lifetime);
	lua_pushlightcppobject(L, val, extra);
}

void lua_get_cppobject(lua_State* L, int idx, MetatableTag expectedMetatableTag, CppObjectMetadata& obj)
{
	CppPointerVal val(L, idx);
	auto metatableTag = val.MetatableTag();
	if (metatableTag != expectedMetatableTag) {
		luaL_error(L, "Param %d must be a C++ of type %d; got %d", idx, expectedMetatableTag, metatableTag);
	}

	obj.Ptr = val.Ptr();
	obj.MetatableTag = metatableTag;
	obj.PropertyMapTag = val.PropertyMapTag();
	obj.Lifetime = val.Lifetime();
}

void lua_get_cppobject(lua_State* L, int idx, CppObjectMetadata& obj)
{
	CppPointerVal val(L, idx);
	obj.Ptr = val.Ptr();
	obj.MetatableTag = val.MetatableTag();
	obj.PropertyMapTag = val.PropertyMapTag();
	obj.Lifetime = val.Lifetime();
}

bool lua_try_get_cppobject(lua_State* L, int idx, MetatableTag expectedTypeTag, CppObjectMetadata& obj)
{
	auto value = lua_index2addr(L, idx);
	if (!CppPointerVal::IsCppObject(value)) {
		return false;
	}

	CppPointerVal val(value);
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

bool lua_try_get_cppobject(lua_State* L, int idx, CppObjectMetadata& obj)
{
	auto value = lua_index2addr(L, idx);
	if (!CppPointerVal::IsCppObject(value)) {
		return false;
	}

	CppPointerVal val(value);
	obj.Ptr = val.Ptr();
	obj.MetatableTag = val.MetatableTag();
	obj.PropertyMapTag = val.PropertyMapTag();
	obj.Lifetime = val.Lifetime();
	return true;
}



void lua_push_cppvalue(lua_State* L, MetatableTag metatableTag, int propertyMapIndex, uint64_t object)
{
	assert(propertyMapIndex >= 0);
	auto val = CppValue::MakeRaw(object);
	auto extra = CppValue::MakeExtra(metatableTag, propertyMapIndex);
	lua_pushlightcppobject(L, val, extra);
}

void lua_get_cppvalue(lua_State* L, int idx, MetatableTag expectedMetatableTag, CppValueMetadata& obj)
{
	CppValue val(L, idx);
	auto metatableTag = val.MetatableTag();
	if (metatableTag != expectedMetatableTag) {
		luaL_error(L, "Param %d must be a C++ of type %d; got %d", idx, expectedMetatableTag, metatableTag);
	}

	obj.Value = val.Value();
	obj.MetatableTag = metatableTag;
	obj.PropertyMapTag = val.PropertyMapTag();
}

void lua_get_cppvalue(lua_State* L, int idx, CppValueMetadata& obj)
{
	CppValue val(L, idx);
	obj.Value = val.Value();
	obj.MetatableTag = val.MetatableTag();
	obj.PropertyMapTag = val.PropertyMapTag();
}

void lua_get_cppvalue(lua_State* L, TValue* v, CppValueMetadata& obj)
{
	CppValue val(v);
	obj.Value = val.Value();
	obj.MetatableTag = val.MetatableTag();
	obj.PropertyMapTag = val.PropertyMapTag();
}

bool lua_try_get_cppvalue(lua_State* L, int idx, MetatableTag expectedTypeTag, CppValueMetadata& obj)
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

bool lua_try_get_cppvalue(lua_State* L, int idx, CppValueMetadata& obj)
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

	auto metatableTag = CppPointerVal::MetatableTagFromExtra(extra);
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
	CppValueMetadata meta;
	lua_get_cppvalue(L, v, meta);
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
