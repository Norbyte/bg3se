#pragma once

BEGIN_SE()


// Minimalist version of container requirements
template <class T>
concept IsArray = requires(T a)
{
    typename T::value_type;
    { a[0] } -> std::same_as<typename T::value_type&>;
}
// For containers without ref return
|| requires(T a)
{
    typename T::value_type;
    { a[0] } -> std::same_as<typename T::value_type>;
}
|| std::derived_from<T, ::Noesis::BaseCollection>;

template <class T>
concept IsMap = requires(T a)
{
    typename T::key_type;
    typename T::value_type;
    { a.try_get(typename T::key_type{}) } -> std::same_as<typename T::value_type*>;
};

template <class T>
concept IsSet = requires(T a)
{
    typename T::value_type;
    { a.contains(typename T::value_type{}) } -> std::same_as<bool>;
    a.insert(typename T::value_type{});
};

template <class T>
constexpr bool IsVariantLike = false;

template <class... Args>
constexpr bool IsVariantLike<std::variant<Args...>> = true;

template <class T>
concept IsVariant = IsVariantLike<T>;

template <class T>
constexpr bool IsOptionalType = false;

template <class T>
constexpr bool IsOptionalType<std::optional<T>> = true;

template <class T>
concept IsOptional = IsOptionalType<T>;


// IsX<T> template correctness tests
// Array types
static_assert(IsArray<Array<int>>);
static_assert(IsArray<StaticArray<int>>);
static_assert(IsArray<LegacyArray<int>>);
static_assert(IsArray<BitArray<uint64_t, 1>>);
static_assert(IsArray<Vector<int>>);
static_assert(IsArray<Set<int>>);
static_assert(IsArray<ObjectSet<int>>);
static_assert(IsArray<TrackedCompactSet<int>>);
static_assert(IsArray<MiniCompactSet<int>>);
static_assert(IsArray<Queue<int>>);
static_assert(IsArray<::Noesis::BaseCollection>);
static_assert(IsArray<std::array<int, 10>>);
static_assert(IsArray<std::vector<int>>);
static_assert(IsArray<std::span<int, 10>>);

// Map types
static_assert(IsMap<LegacyMap<int, STDString>>);
static_assert(IsMap<LegacyRefMap<int, STDString>>);
static_assert(IsMap<HashMap<int, STDString>>);
static_assert(IsMap<VirtualHashMap<int, STDString>>);

// Set types
static_assert(IsSet<HashSet<STDString>>);
static_assert(IsSet<VirtualMultiHashSet<STDString>>);

// Variants
static_assert(IsVariant<std::variant<int, STDString>>);



// Tag indicating whether the type is a "strongly-typed integer" masquerading as an enumeration
// (i.e. declared as an enum class solely to benefit from stronger C++ type checking)
template <class T>
constexpr bool IsIntegralAlias = false;

#define MARK_INTEGRAL_ALIAS(ty) template <> constexpr bool IsIntegralAlias<ty> = true;



// Tag indicating whether a specific type should be handled as a value (by-val) 
// or as an object via an object/array proxy (by-ref)
template <class T>
constexpr bool ByValType = std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_enum_v<T> || IsIntegralAlias<T>;

template <class T>
constexpr bool ByValType<std::optional<T>> = ByValType<T>;

template <class T>
constexpr bool ByValType<lua::LuaDelegate<T>> = true;

template <class T>
constexpr bool ByValType<OverrideableProperty<T>> = ByValType<T>;

template <class T>
concept IsByVal = ByValType<T>;

#define MARK_BY_VALUE_TYPE(cls) \
    static_assert(std::is_default_constructible_v<cls>, "By-value types must be default constructible"); \
    template<> constexpr bool ByValType<cls> = true;


MARK_BY_VALUE_TYPE(bool)
MARK_BY_VALUE_TYPE(ComponentHandle)
MARK_BY_VALUE_TYPE(EntityHandle)
MARK_BY_VALUE_TYPE(FixedString)
MARK_BY_VALUE_TYPE(STDString)
MARK_BY_VALUE_TYPE(STDWString)
MARK_BY_VALUE_TYPE(StringView)
MARK_BY_VALUE_TYPE(WStringView)
MARK_BY_VALUE_TYPE(LSStringView)
MARK_BY_VALUE_TYPE(Noesis::String)
MARK_BY_VALUE_TYPE(Noesis::Symbol)
MARK_BY_VALUE_TYPE(ScratchBuffer)
MARK_BY_VALUE_TYPE(ScratchString)
MARK_BY_VALUE_TYPE(Guid)
MARK_BY_VALUE_TYPE(Path)
MARK_BY_VALUE_TYPE(NetId)
MARK_BY_VALUE_TYPE(UserId)
MARK_BY_VALUE_TYPE(glm::ivec2)
MARK_BY_VALUE_TYPE(glm::ivec3)
MARK_BY_VALUE_TYPE(glm::ivec4)
MARK_BY_VALUE_TYPE(glm::vec2)
MARK_BY_VALUE_TYPE(glm::vec3)
MARK_BY_VALUE_TYPE(glm::vec4)
MARK_BY_VALUE_TYPE(glm::aligned_highp_vec4)
MARK_BY_VALUE_TYPE(glm::quat)
MARK_BY_VALUE_TYPE(glm::mat3)
MARK_BY_VALUE_TYPE(glm::mat3x4)
MARK_BY_VALUE_TYPE(glm::mat4x3)
MARK_BY_VALUE_TYPE(glm::mat4)

END_SE()
