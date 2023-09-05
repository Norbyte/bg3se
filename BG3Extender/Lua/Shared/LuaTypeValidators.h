#pragma once

BEGIN_NS(lua)

inline bool Validate(bool* b, Overload<bool>)
{
	// Bool is false (0) / true (1)
	return *(uint8_t*)b <= 1;
}

// No validation needed/possible for numeric types
template <class T>
inline typename std::enable_if_t<std::is_integral_v<T>, bool> Validate(T*, Overload<T>)
{
	return true;
}

template <class T>
inline typename std::enable_if_t<std::is_floating_point_v<T>, bool> Validate(T*, Overload<T>)
{
	return true;
}

// No validation possible for vector types
inline bool Validate(glm::ivec2* b, Overload<glm::ivec2>) { return true; }
inline bool Validate(glm::i16vec2* b, Overload<glm::i16vec2>) { return true; }
inline bool Validate(glm::vec2* b, Overload<glm::vec2>) { return true; }
inline bool Validate(glm::vec3* b, Overload<glm::vec3>) { return true; }
inline bool Validate(glm::vec4* b, Overload<glm::vec4>) { return true; }
inline bool Validate(glm::quat* b, Overload<glm::quat>) { return true; }
inline bool Validate(glm::mat3* b, Overload<glm::mat3>) { return true; }
inline bool Validate(glm::mat3x4* b, Overload<glm::mat3x4>) { return true; }
inline bool Validate(glm::mat4x3* b, Overload<glm::mat4x3>) { return true; }
inline bool Validate(glm::mat4* b, Overload<glm::mat4>) { return true; }

// TODO - might add some validation heuristic later?
inline bool Validate(EntityHandle* b, Overload<EntityHandle>) { return true; }
inline bool Validate(ComponentHandle* b, Overload<ComponentHandle>) { return true; }
inline bool Validate(NetId* b, Overload<NetId>) { return true; }
inline bool Validate(UserId* b, Overload<UserId>) { return true; }
inline bool Validate(Version* b, Overload<Version>) { return true; }

struct STDStringInternals
{
	union {
		char* Ptr;
		char InlineStr[16];
	};
	uint32_t Size;
	uint32_t Capacity;
};

inline bool Validate(STDString* s, Overload<STDString>)
{
	auto i = reinterpret_cast<STDStringInternals*>(s);
	if (i->Size > i->Capacity) return false;

	if (i->Capacity >= 0xF) {
		if (IsBadReadPtr(i->Ptr, (uint64_t)i->Capacity + 1)) return false;
		// Check null terminator
		if (i->Ptr[i->Size] != 0x00) return false;
	} else {
		if (i->InlineStr[i->Size] != 0x00) return false;
	}

	return true;
}

inline bool Validate(Path* p, Overload<Path>)
{
	return Validate(&p->Name, Overload<STDString>{});
}

inline bool Validate(FixedString* s, Overload<FixedString>)
{
	return s->IsValid();
}

inline bool Validate(RuntimeStringHandle* h, Overload<RuntimeStringHandle>)
{
	return h->Handle.IsValid()
		&& h->Version <= 0x1000;
}

inline bool Validate(TranslatedString* ts, Overload<TranslatedString>)
{
	return Validate(&ts->Handle, Overload<RuntimeStringHandle>{})
		&& Validate(&ts->ArgumentString, Overload<RuntimeStringHandle>{});
}

inline bool Validate(Guid* g, Overload<Guid>)
{
	// Heuristic: Consider non-null GUIDs with zero lower/higher dwords to be sus
	return !*g
		|| (
			(g->Val[0] & 0xffffffff) != 0
			&& (g->Val[0] >> 32) != 0
			&& (g->Val[1] & 0xffffffff) != 0
			&& (g->Val[1] >> 32) != 0
		);
}

template <class T>
typename std::enable_if_t<std::is_enum_v<T>, bool> Validate(T* v, Overload<T>)
{
	if constexpr (std::is_base_of_v<BitmaskInfoBase<T>, EnumInfo<T>>) {
		return ((uint64_t)*v & ~EnumInfo<T>::Store->AllowedFlags) == 0;
	} else if constexpr (std::is_base_of_v<EnumInfoBase<T>, EnumInfo<T>>) {
		return (int64_t)*v < EnumInfo<T>::Store->Labels.size();
	} else {
		assert(false && *T{});
	}
}

inline bool Validate(ecs::EntityRef* g, Overload<ecs::EntityRef>)
{
	// TODO - check if World points to a valid EntityWorld?
	return (g->World == nullptr && !g->Handle)
		|| (g->World != nullptr && (bool)g->Handle);
}

template <class T>
inline bool Validate(T** b, Overload<T*>)
{
	return true;
}

// No need to validate internal special types
inline bool ValidateRef(TypeInformationRef* b, Overload<TypeInformationRef>) { return true; }
inline bool Validate(TypeInformationRef* b, Overload<TypeInformationRef>) { return true; }

inline bool ValidateRef(RuntimeStringHandle* h, Overload<RuntimeStringHandle>)
{
	return Validate(h, Overload<RuntimeStringHandle>{});
}

inline bool ValidateRef(TranslatedString* h, Overload<TranslatedString>)
{
	return Validate(h, Overload<TranslatedString>{});
}

template <class TE>
bool ValidateRef(Array<TE>* v, Overload<Array<TE>>)
{
	if (v->raw_buf() == nullptr) {
		return v->size() == 0 && v->capacity() == 0;
	} else {
		if (v->size() > v->capacity()) return false;
		if (v->capacity() > 0x1000000) return false;
		return !IsBadReadPtr(v->raw_buf(), v->capacity() * sizeof(TE));
		// TODO - check contents (shallow ptr scan)
	}
}

template <class TE>
bool ValidateRef(ObjectSet<TE>* v, Overload<ObjectSet<TE>>)
{
	if (v->CapacityIncrementSize > 0x100000) return false;

	if (v->Buf == nullptr) {
		return v->Size == 0 && v->Capacity == 0;
	} else {
		if (v->Size > v->Capacity) return false;
		if (v->Capacity > 0x1000000) return false;
		return !IsBadReadPtr(v->Buf, v->Capacity * sizeof(TE));
		// TODO - check contents (shallow ptr scan)
	}
}

template <class TE>
bool ValidateRef(StaticArray<TE>* v, Overload<StaticArray<TE>>)
{
	if (v->raw_buf() == nullptr) {
		return v->size() == 0;
	} else {
		if (v->size() > 0x1000000) return false;
		return !IsBadReadPtr(v->raw_buf(), v->size() * sizeof(TE));
		// TODO - check contents (shallow ptr scan)
	}
}

template <class TK, class TV>
bool ValidateRef(Map<TK, TV>* v, Overload<Map<TK, TV>>)
{
	auto m = reinterpret_cast<MapInternals<TK, TV>*>(v);
	if (m->HashTable == nullptr) {
		return m->HashSize == 0 && m->ItemCount == 0;
	} else {
		if (m->HashSize == 0) return false;
		if (IsBadReadPtr(m->HashTable, m->HashSize * sizeof(void*))) return false;
		// TODO - deeper checks on the table?
		// TODO - check keys (shallow ptr scan)
		return true;
	}
}

template <class TK, class TV>
bool ValidateRef(RefMap<TK, TV>* v, Overload<RefMap<TK, TV>>)
{
	auto m = reinterpret_cast<RefMapInternals<TK, TV>*>(v);
	if (m->HashTable == nullptr) {
		return m->HashSize == 0 && m->ItemCount == 0;
	} else {
		if (m->HashSize == 0) return false;
		if (IsBadReadPtr(m->HashTable, m->HashSize * sizeof(void*))) return false;
		// TODO - deeper checks on the table?
		// TODO - check keys (shallow ptr scan)
		return true;
	}
}

template <class TK>
bool ValidateRef(MultiHashSet<TK>* v, Overload<MultiHashSet<TK>>)
{
	if (!ValidateRef(&v->HashKeys, Overload<StaticArray<int32_t>>{})
		|| !ValidateRef(&v->NextIds, Overload<Array<int32_t>>{})
		|| !ValidateRef(&v->Keys, Overload<Array<TK>>{})) {
		return false;
	}

	if (v->HashKeys.size() != v->NextIds.size()) return false;
	// TODO - check keys (shallow ptr scan)
	return true;
}

template <class TK>
bool ValidateRef(VirtualMultiHashSet<TK>* v, Overload<VirtualMultiHashSet<TK>>)
{
	return ValidateRef(v, Overload<MultiHashSet<TK>>{});
}

template <class TK, class TV>
bool ValidateRef(MultiHashMap<TK, TV>* v, Overload<MultiHashMap<TK, TV>>)
{
	if (!ValidateRef(v, Overload<MultiHashSet<TK>>{})
		|| !ValidateRef(&v->Values, Overload<StaticArray<TV>>{})) return false;

	if (v->Keys.size() != v->Values.size()) return false;

	// TODO - check values (shallow ptr scan)
	return true;
}

template <class TK, class TV>
bool ValidateRef(VirtualMultiHashMap<TK, TV>* v, Overload<VirtualMultiHashMap<TK, TV>>)
{
	return ValidateRef(v, Overload<MultiHashMap<TK, TV>>{});
}

template <class T>
bool ValidateRef(T** v, Overload<T*>)
{
	// TODO - pointer to pointer validation?
	return true;
}

template <class T>
bool ValidateRef(T* v, Overload<T>)
{
	return StaticLuaPropertyMap<T>::PropertyMap.ValidateObject(v);
}

template <class TE>
bool Validate(Array<TE>* v, Overload<Array<TE>>)
{
	return ValidateRef(v, Overload<Array<TE>>{});
}

template <class TE>
bool Validate(ObjectSet<TE>* v, Overload<ObjectSet<TE>>)
{
	return ValidateRef(v, Overload<ObjectSet<TE>>{});
}

template <class TK, class TV>
bool Validate(Map<TK, TV>* v, Overload<Map<TK, TV>>)
{
	return ValidateRef(v, Overload<Map<TK, TV>>{});
}

template <class TK, class TV>
bool Validate(RefMap<TK, TV>* v, Overload<RefMap<TK, TV>>)
{
	return ValidateRef(v, Overload<RefMap<TK, TV>>{});
}

template <class TE>
bool Validate(MultiHashSet<TE>* v, Overload<MultiHashSet<TE>>)
{
	return ValidateRef(v, Overload<MultiHashSet<TE>>{});
}

template <class TK, class TV>
bool Validate(MultiHashMap<TK, TV>* v, Overload<MultiHashMap<TK, TV>>)
{
	return ValidateRef(v, Overload<MultiHashMap<TK, TV>>{});
}

template <class T>
bool ValidateAny(T* v);

template <class T>
typename bool Validate(OverrideableProperty<T>* v, Overload<OverrideableProperty<T>>)
{
	return Validate(&v->IsOverridden, Overload<bool>{})
		&& ValidateAny(&v->Value);
}

template <class T>
typename bool ValidateRef(OverrideableProperty<T>* v, Overload<OverrideableProperty<T>>)
{
	return Validate(&v->IsOverridden, Overload<bool>{})
		&& ValidateAny(&v->Value);
}

template <class TE, size_t Size>
bool ValidateRef(std::array<TE, Size>* v, Overload<std::array<TE, Size>>)
{
	for (size_t i = 0; i < Size; i++) {
		if (!ValidateAny(&(*v)[i])) return false;
	}

	return true;
}

template <class T>
bool ValidateAny(T* v)
{
	if constexpr (ByVal<T>::Value) {
		return Validate(v, Overload<T>{});
	} else {
		return ValidateRef(v, Overload<T>{});
	}
}

END_NS()
