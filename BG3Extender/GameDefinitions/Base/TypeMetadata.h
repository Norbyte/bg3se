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
// static_assert(IsArray<gn::TGenomeSet<float>>);
static_assert(IsArray<::Noesis::BaseCollection>);
static_assert(IsArray<std::array<int, 10>>);
static_assert(IsArray<std::vector<int>>);
static_assert(IsArray<std::span<int, 10>>);

static_assert(IsMap<LegacyMap<int, STDString>>);
static_assert(IsMap<LegacyRefMap<int, STDString>>);
static_assert(IsMap<HashMap<int, STDString>>);
static_assert(IsMap<VirtualHashMap<int, STDString>>);

static_assert(IsSet<HashSet<STDString>>);
static_assert(IsSet<VirtualMultiHashSet<STDString>>);

static_assert(IsVariant<std::variant<int, STDString>>);


template <class T>
constexpr bool IsIntegralAlias = false;

#define MARK_INTEGRAL_ALIAS(ty) template <> constexpr bool IsIntegralAlias<ty> = true;

END_SE()
