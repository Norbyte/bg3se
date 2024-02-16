#pragma once

BEGIN_NS(lua)

template <class T>
struct IsArrayLike { static constexpr bool Value = false; };

template <class T>
struct IsArrayLike<Array<T>> { static constexpr bool Value = true; using TElement = T; };

template <class T>
struct IsArrayLike<StaticArray<T>> { static constexpr bool Value = true; using TElement = T; };

template <class TWord, unsigned Words>
struct IsArrayLike<BitArray<TWord, Words>> { static constexpr bool Value = true; using TElement = bool; };

template <class T>
struct IsArrayLike<Vector<T>> { static constexpr bool Value = true; using TElement = T; };

template <class T, class Allocator, bool StoreSize>
struct IsArrayLike<Set<T, Allocator, StoreSize>> { static constexpr bool Value = true; using TElement = T; };

template <class T, class Allocator, bool StoreSize>
struct IsArrayLike<ObjectSet<T, Allocator, StoreSize>> { static constexpr bool Value = true; using TElement = T; };

template <class T, size_t Size>
struct IsArrayLike<std::array<T, Size>> { static constexpr bool Value = true; using TElement = T; };

template <class T>
struct IsArrayLike<std::vector<T>> { static constexpr bool Value = true; using TElement = T; };

template <class T, size_t Extent>
struct IsArrayLike<std::span<T, Extent>> { static constexpr bool Value = true; using TElement = T; };

template <class T>
struct IsMapLike { static constexpr bool Value = false; };

template <class TK, class TV>
struct IsMapLike<Map<TK, TV>>
{ 
	static constexpr bool Value = true; 
	using TKey = TK;
	using TValue = TV;
};

template <class TK, class TV>
struct IsMapLike<RefMap<TK, TV>>
{ 
	static constexpr bool Value = true;
	using TKey = TK;
	using TValue = TV;
};

template <class TK, class TV>
struct IsMapLike<MultiHashMap<TK, TV>>
{ 
	static constexpr bool Value = true;
	using TKey = TK;
	using TValue = TV;
};

template <class TK, class TV>
struct IsMapLike<VirtualMultiHashMap<TK, TV>>
{ 
	static constexpr bool Value = true;
	using TKey = TK;
	using TValue = TV;
};

template <class T>
struct IsSetLike { static constexpr bool Value = false; };

template <class TK>
struct IsSetLike<MultiHashSet<TK>>
{ 
	static constexpr bool Value = true;
	using TKey = TK;
};

template <class TK>
struct IsSetLike<VirtualMultiHashSet<TK>>
{ 
	static constexpr bool Value = true;
	using TKey = TK;
};

template <class T>
struct IsVariantLike { static constexpr bool Value = false; };

template <class... Args>
struct IsVariantLike<std::variant<Args...>>
{ 
	static constexpr bool Value = true;
};

END_NS()
