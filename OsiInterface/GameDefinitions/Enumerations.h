#pragma once

#include <cstdint>

namespace dse
{
	template <class T>
	struct BitmaskInfoBase
	{
		static std::vector<FixedString> Labels;
		static Map<FixedString, T> Values;

		static void Init(unsigned sizeHint)
		{
			Labels.reserve(sizeHint);
			Values.Init(GetNearestLowerPrime(sizeHint));
		}

		static void __declspec(noinline) Add(T val, char const* label)
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
			Values.Insert(fs, val);
		}

		static std::optional<T> Find(FixedString const& name)
		{
			auto val = Values.Find(name);
			if (!val) {
				return {};
			} else {
				return *val;
			}
		}

		static std::optional<T> Find(char const* name)
		{
			// TODO - remove when all refs are gone
			return Find(FixedString(name));
		}

		static FixedString Find(T val)
		{
			DWORD index;
			if (_BitScanForward64(&index, (uint64_t)val)) {
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
	struct EnumInfoBase
	{
		static std::vector<FixedString> Labels;
		static Map<FixedString, T> Values;

		static void Init(unsigned sizeHint)
		{
			Labels.reserve(sizeHint);
			Values.Init(GetNearestLowerPrime(sizeHint));
		}

		static void __declspec(noinline) Add(T val, char const* label)
		{
			FixedString fs(label);
			auto index = static_cast<uint32_t>(val);

			if (Labels.size() <= index) {
				Labels.resize(index + 1);
			}

			Labels[index] = fs;
			Values.Insert(fs, val);
		}

		static std::optional<T> Find(FixedString const& name)
		{
			auto val = Values.Find(name);
			if (!val) {
				return {};
			} else {
				return *val;
			}
		}

		static std::optional<T> Find(char const* name)
		{
			// TODO - remove when all refs are gone
			return Find(FixedString(name));
		}

		static FixedString Find(T val)
		{
			auto v = (uint32_t)val;
			if (v >= Labels.size()) {
				return FixedString{};
			}

			return Labels[v];
		}
	};

	template<typename T> struct EnumInfoFakeDep : public std::false_type {};

	template <class T>
	struct EnumInfo
	{
		static_assert(EnumInfoFakeDep<T>::value, "EnumInfo not implemented for this type!");
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
	typename std::enable_if_t<IsBitmaskV<T>, T> operator | (T lhs, T rhs)
	{
		using underlying = std::underlying_type_t<T>;
		return static_cast<T>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
	}

	template <class T>
	typename std::enable_if_t<IsBitmaskV<T>, T> operator & (T lhs, T rhs)
	{
		using underlying = std::underlying_type_t<T>;
		return static_cast<T>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
	}

	template <class T>
	typename std::enable_if_t<IsBitmaskV<T>, T> operator ^ (T lhs, T rhs)
	{
		using underlying = std::underlying_type_t<T>;
		return static_cast<T>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
	}

	template <class T>
	typename std::enable_if_t<IsBitmaskV<T>, T> operator ~ (T v)
	{
		return static_cast<T>(~static_cast<std::underlying_type_t<T>>(v));
	}

	template <class T>
	typename std::enable_if_t<IsBitmaskV<T>, T> operator |= (T& lhs, T rhs)
	{
		using underlying = std::underlying_type_t<T>;
		lhs = static_cast<T>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
		return lhs;
	}

	template <class T>
	typename std::enable_if_t<IsBitmaskV<T>, T> operator &= (T& lhs, T rhs)
	{
		using underlying = std::underlying_type_t<T>;
		lhs = static_cast<T>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
		return lhs;
	}

	template <class T>
	typename std::enable_if_t<IsBitmaskV<T>, T> operator ^= (T& lhs, T rhs)
	{
		using underlying = std::underlying_type_t<T>;
		lhs = static_cast<T>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
		return lhs;
	}

	void InitializeEnumerations();

#define BEGIN_BITMASK_NS(NS, T, type) namespace NS { \
	enum class T : type {
#define BEGIN_ENUM_NS(NS, T, type) namespace NS { \
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


#define BEGIN_BITMASK_NS(NS, T, type) \
	template<> struct IsBitmask<NS::T> { \
		static const bool value = true; \
	}; \
	template <> struct EnumInfo<NS::T> : public BitmaskInfoBase<NS::T> { \
		static constexpr char const * Name = #T; \
	};
#define BEGIN_ENUM_NS(NS, T, type) \
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

}
