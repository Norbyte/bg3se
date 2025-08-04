#pragma once

#if defined(_DEBUG)
#define CHECK(expr) if (!(expr)) { TryDebugBreak(); return false; }
#else
#define CHECK(expr) if (!(expr)) return false;
#endif
#define CHECKR(expr) if (!(expr)) return false;

#if defined(_DEBUG)
#undef ENABLE_FLAKY_HEURISTICS
#undef ENABLE_GUESSWORK_HEURISTICS
#endif

BEGIN_NS(lua)

// Max plausible string length
static constexpr unsigned MaxStringLengthHeuristic = 0x1000000;

template <class T>
bool ValidateAny(T const* v);

bool IsAliveEntity(EntityHandle const& handle);

inline bool Validate(bool const* b, Overload<bool>)
{
    // Bool is false (0) / true (1)
    CHECK(*(uint8_t*)b <= 1);
    return true;
}

// No validation needed/possible for numeric types
template <class T>
inline typename std::enable_if_t<std::is_integral_v<T>, bool> Validate(T const*, Overload<T>)
{
    return true;
}

inline bool Validate(int64_t const* v, Overload<int64_t>)
{
#if defined(ENABLE_GUESSWORK_HEURISTICS)
    CHECK(!IsAliveEntity(*reinterpret_cast<EntityHandle const*>(v)));
    CHECK(*v < 0x10000000 || *v > 0x0000ffffffffffffull || IsBadReadPtr(v, 1));
#endif
    return true;
}

inline bool Validate(uint64_t const* v, Overload<uint64_t>)
{
#if defined(ENABLE_GUESSWORK_HEURISTICS)
    CHECK(!IsAliveEntity(*reinterpret_cast<EntityHandle const*>(v)));
    CHECK(*v < 0x10000000 || *v > 0x0000ffffffffffffull || IsBadReadPtr(v, 1));
#endif
    return true;
}

inline bool Validate(float const* v, Overload<float>)
{
#if defined(ENABLE_FLAKY_HEURISTICS)
    CHECK((*v > -100000000.0f && *v < 100000000.0f) 
        // Various max boundary values seen in the game
        || *v == 3.06254467e+38f // max LOD distance
        || *v == 3.40282347e+38f
        || *v == -3.40282347e+38f
        || *v == 3.78091496e+37f);
#endif
    return true;
}

inline bool Validate(double const*, Overload<double>)
{
    return true;
}

// No validation possible for vector types
inline bool Validate(glm::ivec2 const* b, Overload<glm::ivec2>) {return true; }
inline bool Validate(glm::ivec4 const* b, Overload<glm::ivec4>) { return true; }
inline bool Validate(glm::i16vec2 const* b, Overload<glm::i16vec2>) { return true; }

inline bool Validate(glm::vec2 const* b, Overload<glm::vec2>)
{
    CHECKR(Validate(&b->x, Overload<float>{}));
    CHECKR(Validate(&b->y, Overload<float>{}));
    return true;
}

inline bool Validate(glm::vec3 const* b, Overload<glm::vec3>)
{
    CHECKR(Validate(&b->x, Overload<float>{}));
    CHECKR(Validate(&b->y, Overload<float>{}));
    CHECKR(Validate(&b->z, Overload<float>{}));
    return true;
}

inline bool Validate(glm::vec4 const* b, Overload<glm::vec4>)
{
    CHECKR(Validate(&b->x, Overload<float>{}));
    CHECKR(Validate(&b->y, Overload<float>{}));
    CHECKR(Validate(&b->z, Overload<float>{}));
    CHECKR(Validate(&b->w, Overload<float>{}));
    return true;
}

inline bool Validate(glm::aligned_highp_vec4 const* b, Overload<glm::aligned_highp_vec4>)
{
    CHECKR(Validate(&b->x, Overload<float>{}));
    CHECKR(Validate(&b->y, Overload<float>{}));
    CHECKR(Validate(&b->z, Overload<float>{}));
    CHECKR(Validate(&b->w, Overload<float>{}));
    return true;
}

inline bool Validate(glm::quat const* b, Overload<glm::quat>)
{
    CHECKR(Validate(&b->x, Overload<float>{}));
    CHECKR(Validate(&b->y, Overload<float>{}));
    CHECKR(Validate(&b->z, Overload<float>{}));
    CHECKR(Validate(&b->w, Overload<float>{}));
    return true;
}

inline bool Validate(glm::mat3 const* b, Overload<glm::mat3>)
{
    CHECKR(Validate(&(*b)[0], Overload<glm::vec3>{}));
    CHECKR(Validate(&(*b)[1], Overload<glm::vec3>{}));
    CHECKR(Validate(&(*b)[2], Overload<glm::vec3>{}));
    return true;
}

inline bool Validate(glm::mat3x4 const* b, Overload<glm::mat3x4>)
{
    CHECKR(Validate(&(*b)[0], Overload<glm::vec4>{}));
    CHECKR(Validate(&(*b)[1], Overload<glm::vec4>{}));
    CHECKR(Validate(&(*b)[2], Overload<glm::vec4>{}));
    return true;
}

inline bool Validate(glm::mat4x3 const* b, Overload<glm::mat4x3>)
{
    CHECKR(Validate(&(*b)[0], Overload<glm::vec3>{}));
    CHECKR(Validate(&(*b)[1], Overload<glm::vec3>{}));
    CHECKR(Validate(&(*b)[2], Overload<glm::vec3>{}));
    CHECKR(Validate(&(*b)[3], Overload<glm::vec3>{}));
    return true;
}

inline bool Validate(glm::mat4 const* b, Overload<glm::mat4>)
{
    CHECKR(Validate(&(*b)[0], Overload<glm::vec4>{}));
    CHECKR(Validate(&(*b)[1], Overload<glm::vec4>{}));
    CHECKR(Validate(&(*b)[2], Overload<glm::vec4>{}));
    CHECKR(Validate(&(*b)[3], Overload<glm::vec4>{}));
    return true;
}


bool Validate(EntityHandle const* b, Overload<EntityHandle>);
inline bool Validate(ComponentHandle const* b, Overload<ComponentHandle>) { return true; }
inline bool Validate(NetId const* b, Overload<NetId>) { return true; }
inline bool Validate(UserId const* b, Overload<UserId>) { return true; }
inline bool Validate(Version const* b, Overload<Version>) { return true; }

struct STDStringInternals
{
    union {
        char* Ptr;
        char InlineStr[16];
    };
    uint32_t Size;
    uint32_t Capacity;
};

inline bool Validate(STDString const* s, Overload<STDString>)
{
    auto i = reinterpret_cast<STDStringInternals const*>(s);
    CHECK(i->Size <= i->Capacity);
    CHECK(i->Capacity <= MaxStringLengthHeuristic);

    if (i->Capacity > 0xF) {
        CHECK(!IsBadReadPtr(i->Ptr, (uint64_t)i->Capacity + 1));
        // Check null terminator
        CHECK(i->Ptr[i->Size] == 0x00);
    } else {
        CHECK(i->InlineStr[i->Size] == 0x00);
    }

    return true;
}

inline bool Validate(Noesis::String const* s, Overload<Noesis::String>)
{
    CHECK(s->Size() <= s->Capacity());
    CHECK(s->Capacity() <= MaxStringLengthHeuristic);
    CHECK(!IsBadReadPtr(s->Str(), (uint64_t)s->Capacity()));

    return true;
}

inline bool Validate(StringView const* s, Overload<StringView>)
{
    CHECK(s->size() <= MaxStringLengthHeuristic);
    CHECK(!IsBadReadPtr(s->data(), (uint64_t)s->size()));

    return true;
}

inline bool Validate(LSStringView const* s, Overload<LSStringView>)
{
    CHECK(s->size() <= MaxStringLengthHeuristic);
    CHECK(!IsBadReadPtr(s->data(), (uint64_t)s->size()));

    return true;
}

inline bool Validate(WStringView const* s, Overload<WStringView>)
{
    CHECK(s->size() <= MaxStringLengthHeuristic);
    CHECK(!IsBadReadPtr(s->data(), (uint64_t)s->size() * 2));

    return true;
}

inline bool Validate(MemBuffer const* s, Overload<MemBuffer>)
{
    CHECK(s->Size <= MaxStringLengthHeuristic);
    CHECK(!IsBadReadPtr(s->Buffer, s->Size));

    return true;
}

inline bool Validate(ScratchBuffer const* s, Overload<ScratchBuffer>)
{
    CHECKR(Validate(&s->Buffer, Overload<MemBuffer>{}));
    CHECK(s->Size <= s->Buffer.Size);
    CHECK(s->Write.Buffer == s->Buffer.Buffer);
    CHECK(s->Read.Buffer == s->Read.Buffer);

    return true;
}

inline bool Validate(ScratchString const* s, Overload<ScratchString>)
{
    CHECK(s->Capacity <= MaxStringLengthHeuristic);
    CHECK(s->Size <= s->Capacity);
    CHECK(s->Position <= s->Size);
    CHECKR(Validate(&s->Managed, Overload<bool>{}));
    CHECKR(Validate(&s->CanGrow, Overload<bool>{}));
    CHECK(!IsBadReadPtr(s->Buffer, s->Capacity));

    return true;
}

inline bool Validate(Path const* p, Overload<Path>)
{
    return Validate(&p->Name, Overload<STDString>{});
}

inline bool Validate(FixedString const* s, Overload<FixedString>)
{
    CHECK(s->IsValid());
    return true;
}

inline bool Validate(RuntimeStringHandle const* h, Overload<RuntimeStringHandle>)
{
    CHECK(h->Handle.IsValid());
    CHECK(h->Version <= 0x1000);
    return true;
}

inline bool Validate(TranslatedString const* ts, Overload<TranslatedString>)
{
    CHECKR(Validate(&ts->Handle, Overload<RuntimeStringHandle>{}));
    CHECKR(Validate(&ts->ArgumentString, Overload<RuntimeStringHandle>{}));
    return true;
}

inline bool Validate(Guid const* g, Overload<Guid>)
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

inline bool Validate(bg3se::stats::ConditionId const* s, Overload<bg3se::stats::ConditionId>)
{
    CHECK(s->Id >= -1);
    return true;
}

template <class T>
typename std::enable_if_t<std::is_enum_v<T>, bool> Validate(T const* v, Overload<T>)
{
#if defined(ENABLE_FLAKY_HEURISTICS)
    if constexpr (IsBitfieldV<T>) {
        // Disabled for now as it causes unmapped enum elements to be flagged all the time
        //CHECK(((uint64_t)*v & ~BitfieldInfo<T>::GetStore().AllowedFlags) == 0);
    } else {
        CHECK((int64_t)*v < EnumInfo<T>::GetStore().Labels.size());
    }
#endif

    return true;
}

inline bool Validate(ecs::EntityRef const* g, Overload<ecs::EntityRef>)
{
    // TODO - check if World points to a valid EntityWorld?
    CHECK((g->World == nullptr && !g->Handle)
        || (g->World != nullptr && (bool)g->Handle));
    return true;
}

template <class T>
inline bool Validate(T* const* p, Overload<T*>)
{
    if (*p != nullptr) {
        CHECK(!IsBadReadPtr(*p, sizeof(T)));
    }

    return true;
}

template <class T>
inline bool ValidatePointer(T const* p)
{
    if (p != nullptr) {
        CHECK(!IsBadReadPtr(p, sizeof(*p)));
    }

    return true;
}

// No need to validate internal special types
inline bool ValidateRef(TypeInformationRef const* b, Overload<TypeInformationRef>) { return true; }
inline bool Validate(TypeInformationRef const* b, Overload<TypeInformationRef>) { return true; }
inline bool Validate(Ref const* b, Overload<Ref>) { return true; }
inline bool Validate(PersistentRef const* b, Overload<PersistentRef>) { return true; }
inline bool Validate(RegistryEntry const* b, Overload<RegistryEntry>) { return true; }
inline bool Validate(PersistentRegistryEntry const* b, Overload<PersistentRegistryEntry>) { return true; }

template <class T>
inline bool Validate(LuaDelegate<T> const* b, Overload<LuaDelegate<T>>) { return true; }

inline bool ValidateRef(RuntimeStringHandle const* h, Overload<RuntimeStringHandle>)
{
    return Validate(h, Overload<RuntimeStringHandle>{});
}

inline bool ValidateRef(TranslatedString const* h, Overload<TranslatedString>)
{
    return Validate(h, Overload<TranslatedString>{});
}

template <class TE>
bool ValidateLinearContainer(TE const* buf, uint64_t size, uint64_t capacity)
{
    if (buf == nullptr) {
        CHECK(size == 0);
        CHECK(capacity == 0);
    } else {
        CHECK(size <= capacity);
        CHECK(capacity <= 0x1000000);
        CHECK(!IsBadReadPtr(buf, capacity * sizeof(TE)));

        if constexpr (!std::is_pointer_v<TE>) {
            for (auto i = 0; i < size; i++) {
                CHECKR(ValidateAny<TE>(buf + i));
            }
        } else {
            for (auto i = 0; i < size; i++) {
                CHECKR(ValidatePointer(buf[i]));
            }
        }
    }

    return true;
}

template <class TE>
bool ValidateRef(Array<TE> const* v, Overload<Array<TE>>)
{
    return ValidateLinearContainer(v->raw_buf(), v->size(), v->capacity());
}

template <class TE>
bool ValidateRef(Queue<TE> const* v, Overload<Queue<TE>>)
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
bool ValidateRef(LegacyArray<TE> const* v, Overload<LegacyArray<TE>>)
{
    CHECKR(ValidateRef(v, Overload<Array<TE>>{}));
    return true;
}

template <class TE, class TAllocator>
bool ValidateRef(std::vector<TE, TAllocator> const* v, Overload<std::vector<TE, TAllocator>>)
{
    return ValidateLinearContainer(v->data(), v->size(), v->capacity());
}

template <class TE>
bool ValidateRef(ObjectSet<TE> const* v, Overload<ObjectSet<TE>>)
{
    CHECK(v->CapacityIncrementSize <= 0x100000);
    return ValidateLinearContainer(v->Buf, v->Size, v->Capacity);
}

template <class TE>
bool ValidateRef(TrackedCompactSet<TE> const* v, Overload<TrackedCompactSet<TE>>)
{
    return ValidateLinearContainer(v->Buf, v->Size, v->Capacity);
}

template <class TE>
bool ValidateRef(MiniCompactSet<TE> const* v, Overload<MiniCompactSet<TE>>)
{
    return ValidateLinearContainer(v->Buf, v->Size, v->Capacity);
}

template <class TE, unsigned N>
bool ValidateRef(Noesis::Vector<TE, N> const* v, Overload<Noesis::Vector<TE, N>>)
{
    CHECK(v->CapacityIncrementSize <= 0x100000);

    if constexpr (N == 0) {
        auto mBegin = *reinterpret_cast<void* const*>(v);
        if (mBegin == reinterpret_cast<void const*>(v + 1)) {
            CHECK(v->Size() == 0);
            CHECK(v->Capacity() == 0);
        }
    }

    CHECK(v->Size() <= v->Capacity());
    CHECK(v->Capacity() <= 0x1000000);
    CHECK(!IsBadReadPtr(&v[0], v->Capacity() * sizeof(TE)));

    if constexpr (!std::is_pointer_v<TE>) {
        for (auto& ele : *v) {
            CHECKR(ValidateAny<TE>(&ele));
        }
    } else {
        for (auto& ele : *v) {
            CHECKR(ValidatePointer(ele));
        }
    }

    return true;
}

template <class TE>
bool ValidateRef(StaticArray<TE> const* v, Overload<StaticArray<TE>>)
{
    return ValidateLinearContainer(v->raw_buf(), v->size(), v->size());
}

template <class TE>
bool ValidateRef(UninitializedStaticArray<TE> const* v, uint32_t initializedSize, Overload<UninitializedStaticArray<TE>>)
{
    return ValidateLinearContainer(v->raw_buf(), initializedSize, v->size());
}

template <class TK, class TV>
bool ValidateRef(LegacyMap<TK, TV> const* v, Overload<LegacyMap<TK, TV>>)
{
    auto m = reinterpret_cast<MapInternals<TK, TV> const*>(v);
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
bool ValidateRef(LegacyRefMap<TK, TV> const* v, Overload<LegacyRefMap<TK, TV>>)
{
    auto m = reinterpret_cast<RefMapInternals<TK, TV> const*>(v);
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
bool ValidateRef(HashSet<TK> const* v, Overload<HashSet<TK>>)
{
    CHECKR(ValidateRef(&v->hash_keys(), Overload<StaticArray<int32_t>>{}));
    CHECKR(ValidateRef(&v->next_ids(), Overload<Array<int32_t>>{}));
    CHECKR(ValidateRef(&v->keys(), Overload<Array<TK>>{}));

    CHECK(v->keys().size() <= v->next_ids().size());

    for (auto k : v->hash_keys()) {
        CHECK(k < (int)v->keys().size());
    }

    for (auto k : v->next_ids()) {
        CHECK(k < (int)v->keys().size());
        CHECK(k >= -1 || (-2 - k) < (int)v->hash_keys().size());
    }

    return true;
}

template <class TK>
bool ValidateRef(VirtualMultiHashSet<TK> const* v, Overload<VirtualMultiHashSet<TK>>)
{
    return ValidateRef(v, Overload<HashSet<TK>>{});
}

template <class TK, class TV>
bool ValidateRef(HashMap<TK, TV> const* v, Overload<HashMap<TK, TV>>)
{
    auto set = reinterpret_cast<HashSet<TK> const*>(v);
    CHECKR(ValidateRef(set, Overload<HashSet<TK>>{}));
    CHECKR(ValidateRef(&v->raw_values(), v->keys().size(), Overload<UninitializedStaticArray<TV>>{}));

    CHECK(v->raw_values().size() >= v->keys().size());

    return true;
}

template <class TK, class TV>
bool ValidateRef(VirtualHashMap<TK, TV> const* v, Overload<VirtualHashMap<TK, TV>>)
{
    return ValidateRef(v, Overload<HashMap<TK, TV>>{});
}

template <class T>
bool ValidateRef(T* const* v, Overload<T*>)
{
    if (*v != nullptr) {
        CHECK(!IsBadReadPtr(*v, sizeof(T)));
    }

    // TODO - pointer to pointer validation?
    return true;
}

template <class T>
bool ValidateRef(T const* v, Overload<T>)
{
    return GetStaticPropertyMap<T>().ValidateObject(v);
}

template <class... Args>
bool ValidateRef(std::variant<Args...> const* v, Overload<std::variant<Args...>>)
{
    CHECK(v->index() < std::variant_size_v<std::variant<Args...>>);
    return std::visit([=](auto& var) { return ValidateAny(&var); }, *v);
}

template <class TE>
bool Validate(Array<TE> const* v, Overload<Array<TE>>)
{
    return ValidateRef(v, Overload<Array<TE>>{});
}

template <class TE>
bool Validate(ObjectSet<TE> const* v, Overload<ObjectSet<TE>>)
{
    return ValidateRef(v, Overload<ObjectSet<TE>>{});
}

template <class TE>
bool Validate(TrackedCompactSet<TE> const* v, Overload<TrackedCompactSet<TE>>)
{
    return ValidateRef(v, Overload<TrackedCompactSet<TE>>{});
}

template <class TE>
bool Validate(MiniCompactSet<TE> const* v, Overload<MiniCompactSet<TE>>)
{
    return ValidateRef(v, Overload<MiniCompactSet<TE>>{});
}

template <class TE, unsigned N>
bool Validate(Noesis::Vector<TE, N> const* v, Overload<Noesis::Vector<TE, N>>)
{
    return ValidateRef(v, Overload<Noesis::Vector<TE, N>>{});
}

template <class TK, class TV>
bool Validate(LegacyMap<TK, TV> const* v, Overload<LegacyMap<TK, TV>>)
{
    return ValidateRef(v, Overload<LegacyMap<TK, TV>>{});
}

template <class TK, class TV>
bool Validate(LegacyRefMap<TK, TV> const* v, Overload<LegacyRefMap<TK, TV>>)
{
    return ValidateRef(v, Overload<LegacyRefMap<TK, TV>>{});
}

template <class TE>
bool Validate(HashSet<TE> const* v, Overload<HashSet<TE>>)
{
    return ValidateRef(v, Overload<HashSet<TE>>{});
}

template <class TK, class TV>
bool Validate(HashMap<TK, TV> const* v, Overload<HashMap<TK, TV>>)
{
    return ValidateRef(v, Overload<HashMap<TK, TV>>{});
}

template <class T>
bool Validate(std::optional<T> const* v, Overload<std::optional<T>>)
{
    // Ensure that _Has_Value is a bool
    auto hasValue = (uint8_t const*)v + sizeof(T);
    CHECK(*hasValue == 0 || *hasValue == 1);

    if (v->has_value()) {
        return Validate(&v->value(), Overload<T>{});
    } else {
        return true;
    }
}

template <class TE, size_t Size>
bool ValidateRef(std::array<TE, Size> const* v, Overload<std::array<TE, Size>>);

template <class T>
bool ValidateRef(std::optional<T> const* v, Overload<std::optional<T>>)
{
    // Ensure that _Has_Value is a bool
    auto hasValue = (uint8_t const*)v + sizeof(T);
    CHECK(*hasValue == 0 || *hasValue == 1);

    if (v->has_value()) {
        return ValidateRef(&v->value(), Overload<T>{});
    } else {
        return true;
    }
}

template <class T>
bool ValidateAny(T const* v);

template <class T>
typename bool Validate(OverrideableProperty<T> const* v, Overload<OverrideableProperty<T>>)
{
    CHECKR(Validate(&v->IsOverridden, Overload<bool>{}));
    CHECKR(ValidateAny(&v->Value));
    return true;
}

template <class T>
typename bool ValidateRef(OverrideableProperty<T> const* v, Overload<OverrideableProperty<T>>)
{
    CHECKR(Validate(&v->IsOverridden, Overload<bool>{}));
    CHECKR(ValidateAny(&v->Value));
    return true;
}

template <class TE, size_t Size>
bool ValidateRef(std::array<TE, Size> const* v, Overload<std::array<TE, Size>>)
{
    for (size_t i = 0; i < Size; i++) {
        CHECKR(ValidateAny(&(*v)[i]));
    }

    return true;
}

template <class TWord, unsigned Words>
bool ValidateRef(BitArray<TWord, Words> const* v, Overload<BitArray<TWord, Words>>)
{
    return true;
}

template <class T>
bool ValidateAny(T const* v)
{
    if constexpr (IsByVal<T>) {
        return Validate(v, Overload<T>{});
    } else {
        return ValidateRef(v, Overload<T>{});
    }
}

END_NS()

#undef CHECK
