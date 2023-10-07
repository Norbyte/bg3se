#pragma once

#include <cstdint>
#include <cassert>
#include <type_traits>
#include <iostream>

#define BEGIN_SE() namespace bg3se {
#define END_SE() }

#define BEGIN_NS(ns) namespace bg3se::ns {
#define END_NS() }

namespace bg3se
{
	// Helper struct to allow function overloading without (real) template-dependent parameters
	template <class>
	struct Overload {};

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

	// Tag indicating whether a specific type should be handled as a value (by-val) 
	// or as an object via an object/array proxy (by-ref)
	template <class T>
	struct ByVal {
		static constexpr bool Value = std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_enum_v<T>;
	};

	template <class T>
	struct ByVal<std::optional<T>> { static constexpr bool Value = ByVal<T>::Value; };

	template <class T>
	constexpr bool IsByVal = ByVal<T>::Value;

#define BY_VAL(cls) template<> struct ByVal<cls> { static constexpr bool Value = true; }


	template <class T>
	struct IsOptional { 
		static constexpr bool Value = false;
	};

	template <class T>
	struct IsOptional<std::optional<T>> { 
		static constexpr bool Value = true;
		using ValueType = T;
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

		inline constexpr TypedIntegral<TValue, Tag>& operator = (TValue const& val)
		{
			value_ = val;
			return *this;
		}

		inline constexpr TypedIntegral<TValue, Tag>& operator = (TypedIntegral<TValue, Tag> const& val)
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


	inline constexpr uint64_t Hash(uint8_t v)
	{
		return v;
	}

	inline constexpr uint64_t Hash(uint16_t v)
	{
		return v;
	}

	inline constexpr uint64_t Hash(uint32_t v)
	{
		return v;
	}

	inline constexpr uint64_t Hash(int32_t v)
	{
		return v;
	}

	inline constexpr uint64_t Hash(uint64_t v)
	{
		return v;
	}

	template <class T>
	inline typename std::enable_if_t<std::is_enum_v<T>, uint64_t> Hash(T v)
	{
		return Hash(std::underlying_type_t<T>(v));
	}

	inline constexpr uint64_t HashMix(uint64_t x, uint64_t y)
	{
		constexpr uint64_t K = 0x9ddfea08eb382d69ull;

		uint64_t r1 = K * (x ^ y);
		uint64_t r2 = r1 ^ (r1 >> 47);
		uint64_t r3 = (y ^ r2) * K;
		return K * (r3 ^ (r3 >> 47));
	}

	template <class T1, class T2>
	inline uint64_t HashMulti(T1 const& a, T2 const& b)
	{
		return HashMix(Hash(a), Hash(b));
	}

	template <class T1, class T2, class T3>
	inline uint64_t HashMulti(T1 const& a, T2 const& b, T3 const& c)
	{
		auto h1 = HashMix(Hash(a), Hash(b));
		return HashMix(h1, Hash(c));
	}

	template <class T1, class T2, class T3, class T4>
	inline uint64_t HashMulti(T1 const& a, T2 const& b, T3 const& c, T4 const& d)
	{
		auto h1 = HashMix(Hash(a), Hash(b));
		auto h2 = HashMix(h1, Hash(c));
		return HashMix(h2, Hash(d));
	}

	// Return type indicating that Lua return values are pushed to the stack by the function
	struct UserReturn
	{
		inline UserReturn(int n)
			: num(n)
		{}

		inline operator int() const
		{
			return num;
		}

		int num;
	};

	// Return type indicating that the return value should be passed to Lua LuaWrite(), i.e. the value should be serialized
	template <class T>
	struct ByValReturn
	{
		inline constexpr ByValReturn()
			: Object(nullptr)
		{}
	
		inline constexpr ByValReturn(T* obj)
			: Object(obj)
		{}

		inline constexpr operator T*() const
		{
			return Object;
		}

		T* Object;
	};

	// Return type indicating that the return value should be passed to Lua using an object proxy
	template <class T>
	struct RefReturn
	{
		inline constexpr RefReturn()
			: Object(nullptr)
		{}
	
		inline constexpr RefReturn(T* obj)
			: Object(obj)
		{}

		inline constexpr operator T*() const
		{
			return Object;
		}

		T* Object;
	};

	// Parameter type indicating that the value should be passed from Lua using an object proxy
	template <class T>
	struct ProxyParam
	{
		inline constexpr ProxyParam()
			: Object(nullptr)
		{}
	
		inline constexpr ProxyParam(T* obj)
			: Object(obj)
		{}

		inline constexpr operator T*() const
		{
			return Object;
		}

		inline constexpr T* operator ->() const
		{
			return Object;
		}

		T* Object;
	};

	template <class T>
	struct OverrideableProperty
	{
		using Type = T;

		T Value;
		bool IsOverridden;
	};

	enum PropertyOperationResult
	{
		Success,
		NoSuchProperty,
		ReadOnly,
		UnsupportedType,
		Unknown
	};

	enum class DebugMessageType
	{
		Debug,
		Info,
		Osiris,
		Warning,
		Error
	};
}

BEGIN_NS(lua)

// Indicates that a type should be pushed through the polymorphic MakeObjectRef()
// implementation, since there are separate property maps for the different subclasses
template <class T>
struct LuaPolymorphic {
	static constexpr bool IsPolymorphic = false;
};

#define LUA_POLYMORPHIC(cls) \
	template <> \
	struct LuaPolymorphic<cls> { \
		static constexpr bool IsPolymorphic = true; \
		static void MakeRef(lua_State* L, cls* value, LifetimeHandle const& lifetime); \
	};


// Indicates that the type is not bound to the normal lifetime scoping rules
// and has infinite lifetime
template <class T>
struct LuaLifetimeInfo {
	static constexpr bool HasInfiniteLifetime = false;
};

#define LUA_INFINITE_LIFETIME(cls) \
	template <> \
	struct LuaLifetimeInfo<cls> { \
		static constexpr bool HasInfiniteLifetime = true; \
	};


END_NS()

namespace std
{
	template<class UnderlyingType, class Tag> 
	struct hash<bg3se::TypedIntegral<UnderlyingType, Tag>>
	{
		typedef bg3se::TypedIntegral<UnderlyingType, Tag> argument_type;
		typedef std::size_t result_type;

		constexpr result_type operator()(argument_type const& v) const noexcept
		{
			return std::hash<UnderlyingType>{}(v.Value());
		}
	};
}
