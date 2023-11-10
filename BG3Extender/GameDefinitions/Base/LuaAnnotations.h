#pragma once

BEGIN_SE()

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

END_SE()

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


// Indicates that a type should be pushed through the polymorphic MakeObjectRef()
// implementation, since there are separate property maps for the different subclasses
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
