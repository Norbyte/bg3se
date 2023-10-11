#pragma once

#if defined(_DEBUG)
#define CHECK(expr) if (!(expr)) { TryDebugBreak(); return false; }
#else
#define CHECK(expr) if (!(expr)) return false;
#endif
#define CHECKR(expr) if (!(expr)) return false;

BEGIN_NS(lua)

template <class T>
bool ValidateAny(T * v);

inline bool Validate(bool* b, Overload<bool>)
{
	// Bool is false (0) / true (1)
	CHECK(*(uint8_t*)b <= 1);
	return true;
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
	CHECK(i->Size <= i->Capacity);
	// Sanity check for very large values
	CHECK(i->Capacity <= 0x1000000);

	if (i->Capacity > 0xF) {
		CHECK(!IsBadReadPtr(i->Ptr, (uint64_t)i->Capacity + 1));
		// Check null terminator
		CHECK(i->Ptr[i->Size] == 0x00);
	} else {
		CHECK(i->InlineStr[i->Size] == 0x00);
	}

	return true;
}

inline bool Validate(Path* p, Overload<Path>)
{
	return Validate(&p->Name, Overload<STDString>{});
}

inline bool Validate(FixedString* s, Overload<FixedString>)
{
	CHECK(s->IsValid());
	return true;
}

inline bool Validate(RuntimeStringHandle* h, Overload<RuntimeStringHandle>)
{
	CHECK(h->Handle.IsValid());
	CHECK(h->Version <= 0x1000);
	return true;
}

inline bool Validate(TranslatedString* ts, Overload<TranslatedString>)
{
	CHECKR(Validate(&ts->Handle, Overload<RuntimeStringHandle>{}));
	CHECKR(Validate(&ts->ArgumentString, Overload<RuntimeStringHandle>{}));
	return true;
}

inline bool Validate(Guid* g, Overload<Guid>)
{
	// Heuristic: Consider non-null GUIDs with zero lower/higher dwords to be sus
	CHECK(!*g
		|| (
			(g->Val[0] & 0xffffffff) != 0
			&& (g->Val[0] >> 32) != 0
			&& (g->Val[1] & 0xffffffff) != 0
			&& (g->Val[1] >> 32) != 0
		));
	return true;
}

template <class T>
typename std::enable_if_t<std::is_enum_v<T>, bool> Validate(T* v, Overload<T>)
{
	if constexpr (std::is_base_of_v<BitmaskInfoBase<T>, EnumInfo<T>>) {
		// Disabled for now as it causes unmapped enum elements to be flagged all the time
		// CHECK(((uint64_t)*v & ~EnumInfo<T>::Store->AllowedFlags) == 0);
	} else if constexpr (std::is_base_of_v<EnumInfoBase<T>, EnumInfo<T>>) {
		// CHECK((int64_t)*v < EnumInfo<T>::Store->Labels.size());
	} else {
		assert(false && *T{});
	}

	return true;
}

inline bool Validate(ecs::EntityRef* g, Overload<ecs::EntityRef>)
{
	// TODO - check if World points to a valid EntityWorld?
	CHECK((g->World == nullptr && !g->Handle)
		|| (g->World != nullptr && (bool)g->Handle));
	return true;
}

template <class T>
inline bool Validate(T** p, Overload<T*>)
{
	if (*p != nullptr) {
		CHECK(!IsBadReadPtr(*p, sizeof(T)));
	}

	return true;
}

template <class T>
inline bool ValidatePointer(T* p)
{
	if (p != nullptr) {
		CHECK(!IsBadReadPtr(p, sizeof(*p)));
	}

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
		CHECK(v->size() == 0);
		CHECK(v->capacity() == 0);
	} else {
		CHECK(v->size() <= v->capacity());
		CHECK(v->capacity() <= 0x1000000);
		CHECK(!IsBadReadPtr(v->raw_buf(), v->capacity() * sizeof(TE)));

		if constexpr (!std::is_pointer_v<TE>) {
			for (auto& ele : *v) {
				CHECKR(ValidateAny<TE>(&ele));
			}
		} else {
			for (auto& ele : *v) {
				CHECKR(ValidatePointer(ele));
			}
		}
	}

	return true;
}

template <class TE>
bool ValidateRef(ObjectSet<TE>* v, Overload<ObjectSet<TE>>)
{
	CHECK(v->CapacityIncrementSize <= 0x100000);

	if (v->Buf == nullptr) {
		CHECK(v->Size == 0);
		CHECK(v->Capacity == 0);
	} else {
		CHECK(v->Size <= v->Capacity);
		CHECK(v->Capacity <= 0x1000000);
		CHECK(!IsBadReadPtr(v->Buf, v->Capacity * sizeof(TE)));

		if constexpr (!std::is_pointer_v<TE>) {
			for (auto& ele : *v) {
				CHECKR(ValidateAny<TE>(&ele));
			}
		} else {
			for (auto& ele : *v) {
				CHECKR(ValidatePointer(ele));
			}
		}
	}

	return true;
}

template <class TE>
bool ValidateRef(StaticArray<TE>* v, Overload<StaticArray<TE>>)
{
	if (v->raw_buf() == nullptr) {
		CHECK(v->size() == 0);
	} else {
		CHECK(v->size() <= 0x1000000);
		CHECK(!IsBadReadPtr(v->raw_buf(), v->size() * sizeof(TE)));

		if constexpr (!std::is_pointer_v<TE>) {
			for (auto& ele : *v) {
				CHECKR(ValidateAny<TE>(&ele));
			}
		} else {
			for (auto& ele : *v) {
				CHECKR(ValidatePointer(ele));
			}
		}
	}

	return true;
}

template <class TE>
bool ValidateRef(StaticArray<TE>* v, uint32_t maxSize, Overload<StaticArray<TE>>)
{
	if (v->raw_buf() == nullptr) {
		CHECK(v->size() == 0);
	} else {
		CHECK(v->size() <= 0x1000000);
		CHECK(!IsBadReadPtr(v->raw_buf(), v->size() * sizeof(TE)));

		if constexpr (!std::is_pointer_v<TE>) {
			for (uint32_t i = 0; i < std::min(v->size(), maxSize); i++) {
				CHECKR(ValidateAny<TE>(&(*v)[i]));
			}
		} else {
			for (uint32_t i = 0; i < std::min(v->size(), maxSize); i++) {
				CHECKR(ValidatePointer((*v)[i]));
			}
		}
	}

	return true;
}

template <class TK, class TV>
bool ValidateRef(Map<TK, TV>* v, Overload<Map<TK, TV>>)
{
	auto m = reinterpret_cast<MapInternals<TK, TV>*>(v);
	if (m->HashTable == nullptr) {
		CHECK(m->HashSize == 0);
		CHECK(m->ItemCount == 0);
	} else {
		CHECK(m->HashSize > 0);
		CHECK(m->HashSize < 0x1000000);
		CHECK(!IsBadReadPtr(m->HashTable, m->HashSize * sizeof(void*)));

		for (uint32_t i = 0; i < m->HashSize; i++) {
			if (m->HashTable[i] != nullptr) {
				CHECK(!IsBadReadPtr(m->HashTable[i], sizeof(m->HashTable[i])));
			}
		}

		// TODO - deeper checks on the table?
		// TODO - check keys (shallow ptr scan)
	}

	return true;
}

template <class TK, class TV>
bool ValidateRef(RefMap<TK, TV>* v, Overload<RefMap<TK, TV>>)
{
	auto m = reinterpret_cast<RefMapInternals<TK, TV>*>(v);
	if (m->HashTable == nullptr) {
		CHECK(m->HashSize == 0);
		CHECK(m->ItemCount == 0);
	} else {
		CHECK(m->HashSize > 0);
		CHECK(m->HashSize < 0x1000000);
		CHECK(!IsBadReadPtr(m->HashTable, m->HashSize * sizeof(void*)));

		for (uint32_t i = 0; i < m->HashSize; i++) {
			if (m->HashTable[i] != nullptr) {
				CHECK(!IsBadReadPtr(m->HashTable[i], sizeof(m->HashTable[i])));
			}
		}

		// TODO - deeper checks on the table?
		// TODO - check keys (shallow ptr scan)
	}

	return true;
}

template <class TK>
bool ValidateRef(MultiHashSet<TK>* v, Overload<MultiHashSet<TK>>)
{
	CHECKR(ValidateRef(&v->HashKeys, Overload<StaticArray<int32_t>>{}));
	CHECKR(ValidateRef(&v->NextIds, Overload<Array<int32_t>>{}));
	CHECKR(ValidateRef(&v->Keys, Overload<Array<TK>>{}));

	CHECK(v->Keys.size() == v->NextIds.size());

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
	CHECKR(ValidateRef(v, Overload<MultiHashSet<TK>>{}));
	CHECKR(ValidateRef(&v->Values, v->Keys.size(), Overload<StaticArray<TV>>{}));

	CHECK(v->Values.size() >= v->Keys.size());

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
	if (*v != nullptr) {
		CHECK(!IsBadReadPtr(*v, sizeof(T)));
	}

	// TODO - pointer to pointer validation?
	return true;
}

template <class T>
bool ValidateRef(T* v, Overload<T>)
{
	return StaticLuaPropertyMap<T>::PropertyMap.ValidateObject(v);
}

template <class... Args>
bool ValidateRef(std::variant<Args...>* v, Overload<std::variant<Args...>>)
{
	CHECK(v->index() < std::variant_size_v<std::variant<Args...>>);
	return std::visit([=](auto& var) { return ValidateAny(&var); }, *v);
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
bool Validate(std::optional<T>* v, Overload<std::optional<T>>)
{
	if (v->has_value()) {
		return Validate(&v->value(), Overload<T>{});
	} else {
		return true;
	}
}

template <class TE, size_t Size>
bool ValidateRef(std::array<TE, Size>* v, Overload<std::array<TE, Size>>);

template <class T>
bool ValidateRef(std::optional<T>* v, Overload<std::optional<T>>)
{
	if (v->has_value()) {
		return ValidateRef(&v->value(), Overload<T>{});
	} else {
		return true;
	}
}

template <class T>
bool ValidateAny(T* v);

template <class T>
typename bool Validate(OverrideableProperty<T>* v, Overload<OverrideableProperty<T>>)
{
	CHECKR(Validate(&v->IsOverridden, Overload<bool>{}));
	CHECKR(ValidateAny(&v->Value));
	return true;
}

template <class T>
typename bool ValidateRef(OverrideableProperty<T>* v, Overload<OverrideableProperty<T>>)
{
	CHECKR(Validate(&v->IsOverridden, Overload<bool>{}));
	CHECKR(ValidateAny(&v->Value));
	return true;
}

template <class TE, size_t Size>
bool ValidateRef(std::array<TE, Size>* v, Overload<std::array<TE, Size>>)
{
	for (size_t i = 0; i < Size; i++) {
		CHECKR(ValidateAny(&(*v)[i]));
	}

	return true;
}

template <class T>
bool ValidateAny(T* v)
{
	if constexpr (IsByVal<T>) {
		return Validate(v, Overload<T>{});
	} else {
		return ValidateRef(v, Overload<T>{});
	}
}

END_NS()

#undef CHECK
