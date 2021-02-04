#pragma once

#include <cstdint>

namespace bg3se
{

	// Base class for game objects that cannot be copied.
	template <class T>
	class Noncopyable
	{
	public:
		inline Noncopyable() {}

		Noncopyable(const Noncopyable&) = delete;
		T& operator = (const T&) = delete;
		Noncopyable(Noncopyable&&) = delete;
		T& operator = (T&&) = delete;
	};

	// Base class for game objects that are managed entirely
	// by the game and we cannot create/copy them.
	template <class T>
	class ProtectedGameObject
	{
	public:
		ProtectedGameObject(const ProtectedGameObject&) = delete;
		T& operator = (const T&) = delete;
		ProtectedGameObject(ProtectedGameObject&&) = delete;
		T& operator = (T&&) = delete;

	protected:
		ProtectedGameObject() = delete;
		//~ProtectedGameObject() = delete;
	};


	// Prevents implicit casting between aliases of integral types (eg. NetId and UserId)
	// Goal is to prevent accidental mixups between different types
	template <class TValue, class Tag>
	class TypedIntegral
	{
	public:
		using ValueType = TValue;

		inline constexpr TypedIntegral() : value_() {}
		inline constexpr TypedIntegral(TypedIntegral<TValue, Tag> const& t) : value_(t.value_) {}
		inline constexpr TypedIntegral(TValue const& val) : value_(val) {}

		inline TypedIntegral<TValue, Tag>& operator = (TValue const& val)
		{
			value_ = val;
			return *this;
		}

		inline TypedIntegral<TValue, Tag>& operator = (TypedIntegral<TValue, Tag> const& val)
		{
			value_ = val.value_;
			return *this;
		}

		inline constexpr explicit operator TValue () const
		{
			return value_;
		}

		inline constexpr bool operator == (TypedIntegral<TValue, Tag> const& val) const
		{
			return value_ == val.value_;
		}

		inline constexpr bool operator == (TValue const& val) const
		{
			return value_ == val;
		}

		inline constexpr bool operator != (TypedIntegral<TValue, Tag> const& val) const
		{
			return value_ != val.value_;
		}

		inline constexpr bool operator != (TValue const& val) const
		{
			return value_ != val;
		}

		inline constexpr bool operator <= (TypedIntegral<TValue, Tag> const& val) const
		{
			return value_ <= val.value_;
		}

		inline constexpr bool operator <= (TValue const& val) const
		{
			return value_ <= val;
		}

		inline constexpr bool operator >= (TypedIntegral<TValue, Tag> const& val) const
		{
			return value_ >= val.value_;
		}

		inline constexpr bool operator >= (TValue const& val) const
		{
			return value_ >= val;
		}

		inline constexpr bool operator < (TypedIntegral<TValue, Tag> const& val) const
		{
			return value_ < val.value_;
		}

		inline constexpr bool operator < (TValue const& val) const
		{
			return value_ < val;
		}

		inline constexpr bool operator > (TypedIntegral<TValue, Tag> const& val) const
		{
			return value_ > val.value_;
		}

		inline constexpr bool operator > (TValue const& val) const
		{
			return value_ > val;
		}

		inline constexpr TValue Value() const
		{
			return value_;
		}

		friend std::ostream& operator << (std::ostream& os, TypedIntegral<TValue, Tag> const& v)
		{
			return os << (int64_t)v.value_;
		}

	private:
		TValue value_;
	};


	template <class T>
	uint64_t Hash(T const&)
	{
		static_assert(false, "Hash<T> needs to be specialized!");
	}

	template <>
	inline uint64_t Hash<uint8_t>(uint8_t const& v)
	{
		return v;
	}

	template <>
	inline uint64_t Hash<uint16_t>(uint16_t const& v)
	{
		return v;
	}

	template <>
	inline uint64_t Hash<uint32_t>(uint32_t const& v)
	{
		return v;
	}

	template <>
	inline uint64_t Hash<int32_t>(int32_t const& v)
	{
		return v;
	}
}
