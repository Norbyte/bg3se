#pragma once

#include <cstdint>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <optional>

#include <GameDefinitions/Base/Base.h>
#include <Lua/Shared/LuaReference.h>
#include <Lua/Shared/LuaTypeTraits.h>

BEGIN_NS(lua)

#if !defined(NDEBUG)
// Ensures that the size of the Lua stack matches the expected value.
// Since there are no built-in checks in Lua function boundaries, an incorrect push
// may keep populating the stack with unused items and cause a stack overflow after some time.
struct StackCheck
{
	inline StackCheck(lua_State* state, int delta = 0)
		: L(state)
	{
		expectedTop = lua_gettop(L) + delta;
	}

	~StackCheck()
	{
		// During stack unwinding the topmost frame will clean up the stack,
		// so frames where we didn't reach the catch{} handler yet may see incorrect results
		if (std::uncaught_exceptions() > 0) return;

		int newTop = lua_gettop(L);
		if (newTop != expectedTop) {
			// DebugBreak() crashes without a debugger
			if (IsDebuggerPresent()) {
				TryDebugBreak();
			} else {
				luaL_error(L, "Stack check failed! Top is %d, expected %d", newTop, expectedTop);
			}
		}
	}

	lua_State* L;
	int expectedTop;
};
#else
struct StackCheck
{
	inline StackCheck(lua_State* state, int delta = 0)
	{}
};
#endif

// Helper type for getting any Lua value as parameter
struct AnyRef
{
	int Index;
};

// Helper type for getting functions as parameters
struct FunctionRef
{
	int Index;
};

// Helper type for getting userdata/cpplightuserdata as parameters
struct AnyUserdataRef
{
	int Index;
};

bool ProtectedCallC(lua_State* L, lua_CFunction fun, void* context, void* context2, char const* funcDescription, char const*& error);

// LuaEnumValue forward declarations
void push_enum_value(lua_State* L, EnumUnderlyingType value, EnumInfoStore<EnumUnderlyingType> const& store);
EnumUnderlyingType get_enum_value(lua_State* L, int index, EnumInfoStore<EnumUnderlyingType> const& store);
std::optional<EnumUnderlyingType> try_get_enum_value(lua_State* L, int index, EnumInfoStore<EnumUnderlyingType> const& store);

EnumUnderlyingType get_bitfield_value(lua_State* L, int index, BitmaskInfoStore<EnumUnderlyingType> const& store, bool maskInvalidBits = false);
std::optional<EnumUnderlyingType> try_get_bitfield_value(lua_State* L, int index, BitmaskInfoStore<EnumUnderlyingType> const& store, bool maskInvalidBits);
void push_bitfield_value(lua_State* L, EnumUnderlyingType value, BitmaskInfoStore<EnumUnderlyingType> const& store);

struct MathParam
{
	union {
		float f;
		glm::vec3 vec3;
		glm::vec4 vec4;
		glm::quat quat;
		glm::mat3 mat3;
		glm::mat4 mat4;
	};
	uint32_t Arity;
};

struct TableIterationHelper
{
	struct EndIterator {};

	struct Iterator
	{
		lua_State* L;
		int Index;
		bool End;

		inline Iterator(lua_State* _L, int index)
			: L(_L), Index(index), End(false)
		{}

		inline Iterator operator ++ ()
		{
			lua_pop(L, 1);
			Iterator it(L, Index);
			it.End = lua_next(L, Index) == 0;
			End = it.End;
			return it;
		}

		inline Iterator& operator ++ (int)
		{
			lua_pop(L, 1);
			End = lua_next(L, Index) == 0;
			return *this;
		}

		inline bool operator == (EndIterator const&)
		{
			return End;
		}

		inline bool operator != (EndIterator const&)
		{
			return !End;
		}

		inline int operator * ()
		{
			return -1;
		}
	};

	lua_State* L;
	int Index;

	inline Iterator begin()
	{
		lua_pushnil(L);

		Iterator it(L, Index);
		it.End = lua_next(L, Index) == 0;
		return it;
	}

	inline EndIterator end()
	{
		return EndIterator{};
	}
};

inline TableIterationHelper iterate(lua_State* L, int index)
{
	TableIterationHelper helper;
	helper.L = L;
	helper.Index = lua_absindex(L, index);
	return helper;
}

template <class T>
void Serialize(lua_State* L, T* obj);

template <class T>
PropertyOperationResult Unserialize(lua_State* L, int index, T* obj);

END_NS()

#include <Lua/Shared/LuaLifetime.h>
#include <Lua/Shared/LuaCustomizations.h>

#include <Lua/Helpers/LuaPush.h>
#include <Lua/Helpers/LuaGet.h>

#include <Lua/LuaUserdata.h>
#include <Lua/Shared/Proxies/LuaPropertyMap.h>
#include <Lua/Shared/Proxies/LuaCppClass.h>
#include <Lua/Shared/Proxies/LuaArrayProxy.h>
#include <Lua/Shared/Proxies/LuaSetProxy.h>
#include <Lua/Shared/Proxies/LuaMapProxy.h>
#include <Lua/Shared/Proxies/LuaObjectProxy.h>
#include <Lua/Shared/Proxies/LuaCppObjectProxy.h>

#include <Lua/Helpers/LuaSerialize.h>
#include <Lua/Helpers/LuaUnserialize.h>
#include <Lua/Helpers/LuaPushObject.h>
#include <Lua/Helpers/LuaGetObject.h>

BEGIN_NS(lua)

// Pushes all arguments to the Lua stack and returns a pin that should
// be destroyed after the call
template <class ...Args>
inline void PushArguments(lua_State * L, std::tuple<Args...> args)
{
	std::apply([=](const auto &... elem)
	{
		(push(L, elem), ...);
	}, args);
}

// Fetches a required return value (i.e. succeeded = false if arg doesn't exist or is nil)
template <class T>
void CheckedGetReturnValue(lua_State * L, int & index, T& ret, bool & succeeded)
{
	auto i = index--;
	if (lua_isnil(L, i)) {
		ERR("Return value %d must not be missing or nil", -i);
		succeeded = false;
		ret = T{};
	} else {
		auto val = safe_get<T>(L, i);
		if (val.has_value()) {
			ret = *val;
		} else {
			ERR("Failed to fetch return value %d, incorrect type?", -i);
			succeeded = false;
			ret = T{};
		}
	}
}

// Fetches an optional return value (i.e. succeeded = true if arg doesn't exist or is nil)
template <class T>
void CheckedGetReturnValue(lua_State * L, int & index, std::optional<T>& ret, bool& succeeded)
{
	auto i = index--;
	if (lua_isnil(L, i)) {
		ret = {};
		succeeded = true;
	} else {
		// FIXME - add exception handling!
		ret = get<T>(L, i);
		succeeded = true;
	}
}

template <class... Ret, size_t... Is>
inline bool CheckedGetReturnValuesInner(lua_State * L, bool& succeeded, std::tuple<Ret...>& ret, std::index_sequence<Is...>)
{
	int index{ -1 };
	(CheckedGetReturnValue(L, index, std::get<Is>(ret), succeeded), ...);
	return succeeded;
}

// Fetch Lua return values into a tuple
// Returns false if validation of any return value failed.
// Tuple size *must* match lua_call nres, otherwise it'll corrupt the Lua heap!
template <class... Ret>
bool CheckedGetReturnValues(lua_State * L, std::tuple<Ret...>& ret)
{
	bool succeeded{ true };
	CheckedGetReturnValuesInner(L, succeeded, ret, std::index_sequence_for<Ret...>{});
	return succeeded;
}

// Fetch Lua return values into a tuple
// Returns false if validation of any return value failed, true tuple otherwise.
// Tuple size *must* match lua_call nres, otherwise it'll corrupt the Lua heap!
template <class... Ret>
auto CheckedPopReturnValues(lua_State * L, std::tuple<Ret...>& ret)
{
	auto succeeded = CheckedGetReturnValues<Ret...>(L, ret);
	lua_pop(L, (int)sizeof...(Ret));
	return succeeded;
}

int CallWithTraceback(lua_State* L, int narg, int nres);

// Calls Lua function.
// Function and arguments must be already pushed to the Lua stack.
// Returns false if call failed, true tuple otherwise.
// Function name only needed for error reporting purposes
bool CheckedCall(lua_State * L, int numArgs, char const * functionName);

// Calls Lua function and fetches Lua return values into a tuple.
// Function and arguments must be already pushed to the Lua stack.
// Returns false if call or return value fetch failed, true tuple otherwise.
// Function name only needed for error reporting purposes
template <class... Ret>
bool CheckedCall(lua_State * L, int numArgs, std::tuple<Ret...>& ret, char const * functionName)
{
	if (CallWithTraceback(L, numArgs, sizeof...(Ret)) != 0) { // stack: errmsg
		ERR("%s Lua call failed: %s", functionName, lua_tostring(L, -1));
		lua_pop(L, 1);
		return false;
	}

	if (!CheckedPopReturnValues<Ret...>(L, ret)) {
		ERR("Got incorrect return values from %s", functionName);
		return false;
	}

	return true;
}

int CallWithTraceback(lua_State * L, int narg, int nres);

void RegisterLib(lua_State* L, char const* name, luaL_Reg const* lib);

END_NS()
