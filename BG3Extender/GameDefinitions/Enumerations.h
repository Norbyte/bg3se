#pragma once

#include <cstdint>

BEGIN_SE()

// Type used to store enumeration and bitmask values internally.
// Must be a superset of all enum/bitmask types used ingame.
using EnumUnderlyingType = uint64_t;

template <class T>
struct BitmaskInfoStore
{
	Vector<FixedString> Labels;
	Map<FixedString, T> Values;
	T AllowedFlags{ 0 };
	FixedString EnumName;
	FixedString LuaName;
	int RegistryIndex{ -1 };

	BitmaskInfoStore(unsigned sizeHint, FixedString const& enumName, FixedString const& luaName)
	{
		Labels.reserve(sizeHint);
		Values.ResizeHashtable(GetNearestLowerPrime(sizeHint));
		EnumName = enumName;
		LuaName = luaName;
	}

	void __declspec(noinline) Add(T val, char const* label)
	{
		DWORD index;
		if (_BitScanForward64(&index, (uint64_t)val)) {
			index++;
		} else {
			index = 0;
		}

		if (Labels.size() <= index) {
			Labels.resize(index + 1);
		}
			
		FixedString fs(label);
		Labels[index] = fs;
		Values.insert(std::move(fs), val);
		AllowedFlags |= val;
	}

	std::optional<T> Find(FixedString const& name) const
	{
		auto val = Values.find(name);
		if (!val) {
			return {};
		} else {
			return val.Value();
		}
	}

	FixedString Find(T val) const
	{
		DWORD index;
		if (_BitScanForward64(&index, val)) {
			index++;
		} else {
			index = 0;
		}

		if (index >= Labels.size()) {
			return FixedString{};
		}

		return Labels[index];
	}
};

template <class T>
struct BitmaskInfoBase
{
	using UnderlyingType = EnumUnderlyingType;
	static BitmaskInfoStore<UnderlyingType>* Store;

	static void Init(unsigned sizeHint, char const* enumName, char const* luaName)
	{
		Store = GameAlloc<BitmaskInfoStore<UnderlyingType>>(sizeHint, FixedString(enumName), FixedString(luaName));
	}

	static void Add(T val, char const* label)
	{
		Store->Add((UnderlyingType)val, label);
	}

	static std::optional<T> Find(FixedString const& name)
	{
		auto val = Store->Find(name);
		if (!val) {
			return {};
		} else {
			return (T)*val;
		}
	}

	static FixedString Find(T val)
	{
		return Store->Find((UnderlyingType)val);
	}
};

template <class T>
struct EnumInfoStore
{
	Vector<FixedString> Labels;
	Map<FixedString, T> Values;
	FixedString EnumName;
	FixedString LuaName;
	int RegistryIndex{ -1 };

	EnumInfoStore(unsigned sizeHint, FixedString const& enumName, FixedString const& luaName)
	{
		Labels.reserve(sizeHint);
		Values.ResizeHashtable(GetNearestLowerPrime(sizeHint));
		EnumName = enumName;
		LuaName = luaName;
	}

	void __declspec(noinline) Add(T val, char const* label)
	{
		FixedString fs(label);
		auto index = static_cast<uint32_t>(val);

		if (Labels.size() <= index) {
			Labels.resize(index + 1);
		}

		Labels[index] = fs;
		Values.insert(std::move(fs), val);
	}

	std::optional<T> Find(FixedString const& name) const
	{
		auto val = Values.find(name);
		if (val != Values.end()) {
			return (T)val.Value();
		} else {
			return {};
		}
	}

	FixedString Find(T val) const
	{
		if (val >= Labels.size()) {
			return FixedString{};
		}

		return Labels[(uint32_t)val];
	}
};

template <class T>
struct EnumInfoBase
{
	using UnderlyingType = EnumUnderlyingType;
	static EnumInfoStore<UnderlyingType>* Store;

	static void Init(unsigned sizeHint, char const* enumName, char const* luaName)
	{
		Store = GameAlloc<EnumInfoStore<UnderlyingType>>(sizeHint, FixedString(enumName), FixedString(luaName));
	}

	static void Add(T val, char const* label)
	{
		Store->Add((UnderlyingType)val, label);
	}

	static std::optional<T> Find(FixedString const& name)
	{
		auto val = Store->Find(name);
		if (val) {
			return (T)*val;
		} else {
			return {};
		}
	}

	static FixedString Find(T val)
	{
		return Store->Find((UnderlyingType)val);
	}
};

template<typename T> struct EnumInfoFakeDep : public std::false_type {};

template <class T>
struct EnumInfo
{
	static_assert(EnumInfoFakeDep<T>::value, "EnumInfo not implemented for this type!");
};

struct EnumRegistry
{
	static EnumRegistry& Get();

	Map<FixedString, EnumInfoStore<EnumUnderlyingType>*> EnumsByName;
	ObjectSet<EnumInfoStore<EnumUnderlyingType>*> EnumsById;

	void Register(EnumInfoStore<EnumUnderlyingType>* ei);
};

struct BitmaskRegistry
{
	static BitmaskRegistry& Get();

	Map<FixedString, BitmaskInfoStore<EnumUnderlyingType>*> BitfieldsByName;
	ObjectSet<BitmaskInfoStore<EnumUnderlyingType>*> BitfieldsById;

	void Register(BitmaskInfoStore<EnumUnderlyingType>* ei);
};

#define MAKE_ENUM_INFO(T, size) \
	template <> struct EnumInfo<T> : public std::enable_if_t<IsBitmask<T>, EnumInfoBase<T>, BitmaskInfoBase<T>> { \
		static constexpr char const * Name = #T; \
	}

template <class T>
struct IsBitmask {
	static const bool value = false;
};

template <class T>
constexpr bool IsBitmaskV = IsBitmask<T>::value;

template <class T>
constexpr typename std::enable_if_t<IsBitmaskV<T>, T> operator | (T lhs, T rhs)
{
	using underlying = std::underlying_type_t<T>;
	return static_cast<T>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template <class T>
constexpr typename std::enable_if_t<IsBitmaskV<T>, T> operator & (T lhs, T rhs)
{
	using underlying = std::underlying_type_t<T>;
	return static_cast<T>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template <class T>
constexpr typename std::enable_if_t<IsBitmaskV<T>, T> operator ^ (T lhs, T rhs)
{
	using underlying = std::underlying_type_t<T>;
	return static_cast<T>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

template <class T>
constexpr typename std::enable_if_t<IsBitmaskV<T>, T> operator ~ (T v)
{
	return static_cast<T>(~static_cast<std::underlying_type_t<T>>(v));
}

template <class T>
constexpr typename std::enable_if_t<IsBitmaskV<T>, bool> operator ! (T v)
{
	return !static_cast<std::underlying_type_t<T>>(v);
}

template <class T>
constexpr typename std::enable_if_t<IsBitmaskV<T>, T> operator |= (T& lhs, T rhs)
{
	using underlying = std::underlying_type_t<T>;
	lhs = static_cast<T>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
	return lhs;
}

template <class T>
constexpr typename std::enable_if_t<IsBitmaskV<T>, T> operator &= (T& lhs, T rhs)
{
	using underlying = std::underlying_type_t<T>;
	lhs = static_cast<T>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
	return lhs;
}

template <class T>
constexpr typename std::enable_if_t<IsBitmaskV<T>, T> operator ^= (T& lhs, T rhs)
{
	using underlying = std::underlying_type_t<T>;
	lhs = static_cast<T>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
	return lhs;
}

#define MARK_AS_BITFIELD(T) \
	template<> struct IsBitmask<T> { \
		static const bool value = true; \
	};

void InitializeEnumerations();

#define BEGIN_BITMASK_NS(NS, T, luaName, type) namespace NS { \
	enum class T : type {
#define BEGIN_ENUM_NS(NS, T, luaName, type) namespace NS { \
	enum class T : type {
#define BEGIN_BITMASK(T, type) enum class T : type {
#define BEGIN_ENUM(T, type) enum class T : type {
#define E(label) label,
#define EV(label, value) label = value,
#define END_ENUM_NS() }; };
#define END_ENUM() };
#include <GameDefinitions/Enumerations.inl>
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
#undef EV
#undef END_ENUM_NS
#undef END_ENUM


#define BEGIN_BITMASK_NS(NS, T, luaName, type) \
	template<> struct IsBitmask<NS::T> { \
		static const bool value = true; \
	}; \
	template <> struct EnumInfo<NS::T> : public BitmaskInfoBase<NS::T> { \
		static constexpr char const * Name = #T; \
	};
#define BEGIN_ENUM_NS(NS, T, luaName, type) \
	template <> struct EnumInfo<NS::T> : public EnumInfoBase<NS::T> { \
		static constexpr char const* Name = #T; \
	};
#define BEGIN_BITMASK(T, type) \
	template<> struct IsBitmask<T> { \
		static const bool value = true; \
	}; \
	template <> struct EnumInfo<T> : public BitmaskInfoBase<T> { \
		static constexpr char const * Name = #T; \
	};
#define BEGIN_ENUM(T, type) \
	template <> struct EnumInfo<T> : public EnumInfoBase<T> { \
		static constexpr char const* Name = #T; \
	};
#define E(label)
#define EV(label, value)
#define END_ENUM_NS()
#define END_ENUM()
#include <GameDefinitions/Enumerations.inl>
#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
#undef EV
#undef END_ENUM_NS
#undef END_ENUM

END_SE()


namespace std
{
	template <class T>
	inline std::enable_if_t<std::is_enum_v<T>, ostream&> operator << (ostream& out, T const& v)
	{
		auto label = bg3se::EnumInfo<T>::Find(v);
		if (label) {
			out << label.GetString();
		} else {
			out << "(unknown)";
		}
		return out;
	}
}
