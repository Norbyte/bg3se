#pragma once

#include <cstdint>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <optional>

#include <GameDefinitions/BaseTypes.h>

namespace bg3se::lua
{
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
					DebugBreak();
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

	template <class TValue>
	inline void setfield(lua_State* L, char const* k, TValue const& v, int index = -2)
	{
		push(L, v);
		lua_setfield(L, index, k);
	}

	template <class TKey, class TValue>
	inline void settable(lua_State* L, TKey const& k, TValue const& v, int index = -3)
	{
		push(L, k);
		push(L, v);
		lua_settable(L, index);
	}


	inline void push(lua_State * L, nullptr_t v)
	{
		lua_pushnil(L);
	}

	inline void push(lua_State * L, bool v)
	{
		lua_pushboolean(L, v ? 1 : 0);
	}

	inline void push(lua_State * L, int32_t v)
	{
		lua_pushinteger(L, (lua_Integer)v);
	}

	inline void push(lua_State * L, uint32_t v)
	{
		lua_pushinteger(L, (lua_Integer)v);
	}

	inline void push(lua_State * L, int64_t v)
	{
		lua_pushinteger(L, v);
	}

	inline void push(lua_State * L, uint64_t v)
	{
		lua_pushinteger(L, (lua_Integer)v);
	}

	inline void push(lua_State * L, double v)
	{
		lua_pushnumber(L, v);
	}

	inline void push(lua_State * L, char const * v)
	{
		if (v) {
			lua_pushstring(L, v);
		} else {
			lua_pushnil(L);
		}
	}

	inline void push(lua_State * L, FixedString const& v)
	{
		lua_pushstring(L, v.GetString());
	}

	inline void push(lua_State * L, StringView v)
	{
		lua_pushlstring(L, v.data(), v.size());
	}

	inline void push(lua_State * L, WStringView v)
	{
		push(L, ToUTF8(v));
	}

	inline void push(lua_State* L, ObjectHandle const& h)
	{
		lua_pushlightuserdata(L, (void*)h.Handle);
	}

	inline void push(lua_State* L, EntityHandle const& h)
	{
		lua_pushlightuserdata(L, (void*)h.Handle);
	}

	inline void push(lua_State* L, UUID const& u)
	{
		push(L, FormatUuid(u));
	}

	inline void push(lua_State* L, glm::ivec2 const& v)
	{
		lua_newtable(L);
		settable(L, 1, v.x);
		settable(L, 2, v.y);
	}

	inline void push(lua_State* L, glm::vec2 const& v)
	{
		lua_newtable(L);
		settable(L, 1, v.x);
		settable(L, 2, v.y);
	}

	inline void push(lua_State* L, glm::vec3 const& v)
	{
		lua_newtable(L);
		settable(L, 1, v.x);
		settable(L, 2, v.y);
		settable(L, 3, v.z);
	}

	inline void push(lua_State* L, glm::vec4 const& v)
	{
		lua_newtable(L);
		settable(L, 1, v.x);
		settable(L, 2, v.y);
		settable(L, 3, v.z);
		settable(L, 4, v.w);
	}

	inline void push(lua_State* L, glm::mat3 const& m)
	{
		lua_newtable(L);
		for (auto i = 0; i < 9; i++) {
			settable(L, i + 1, m[i / 3][i % 3]);
		}
	}

	template <class T>
	inline typename std::enable_if_t<std::is_enum_v<T>, void> push(lua_State* L, T v)
	{
		auto label = EnumInfo<T>::Find(v);
		if (label) {
			push(L, label);
		} else {
			lua_pushnil(L);
		}
	}

	template <class T>
	inline typename std::enable_if_t<std::is_pointer_v<T>, std::enable_if_t<!std::is_same_v<T, char const*>, std::enable_if_t<!std::is_same_v<T, char*>, void>>> push(lua_State* L, T v)
	{
		static_assert(false, "Can't push pointers to Lua");
	}

	template <class T>
	inline void push(lua_State* L, std::optional<T> const& v)
	{
		if (v) {
			push(L, *v);
		} else {
			lua_pushnil(L);
		}
	}

	template <class T>
	inline void push_flags(lua_State* L, T value)
	{
		static_assert(std::is_base_of_v<BitmaskInfoBase<T>, EnumInfo<T>>, "Can only push bitmask fields!");

		lua_newtable(L);
		int i = 1;
		EnumInfo<T>::Values.Iterate([value, L, &i](auto const& k, auto const& v) {
			if ((value & v) == v) {
				push(L, i++);
				push(L, k);
				lua_settable(L, -3);
			}
		});
	}


	// Dummy pin class for values that need no special pinning/unpinning behavior
	struct NullPin {};

	template <class T>
	inline typename std::enable_if_t<std::is_integral_v<T>, NullPin> push_pin(lua_State * L, T v)
	{
		push(L, v);
		return {};
	}

	template <class T, typename std::enable_if_t<std::is_invocable_v<T, lua_State *>, int> * = nullptr>
	inline auto push_pin(lua_State * L, T v)
	{
		return v(L);
	}

	// Specifies additional actions taken when the object is pushed to a Lua call
	enum class PushPolicy
	{
		None,
		// Indicates that the we should call Unbind() on the specified object after
		// it goes out of scope (in Lua)
		Unbind
	};

	template <PushPolicy TPolicy>
	struct Pushable
	{};

	template <class T>
	class UnbindablePin
	{
	public:
		inline UnbindablePin(T * object)
			: object_(object)
		{}

		inline ~UnbindablePin()
		{
			if (object_) object_->Unbind();
		}

		inline UnbindablePin(UnbindablePin const &) = delete;

		inline UnbindablePin(UnbindablePin && other)
			: object_(other.object_)
		{
			other.object_ = nullptr;
		}

		UnbindablePin & operator = (UnbindablePin const &) = delete;


	private:
		T * object_;
	};

#define NULL_PIN(type) inline NullPin push_pin(lua_State * L, type v) \
	{ \
		push(L, v); \
		return {}; \
	}

	NULL_PIN(nullptr_t)
	NULL_PIN(double)
	NULL_PIN(char const *)
	NULL_PIN(FixedString)
	NULL_PIN(StringView)
	NULL_PIN(WStringView)


	template <class TValue>
	TValue get(lua_State * L, int index = -1);

	template <>
	inline bool get<bool>(lua_State * L, int index)
	{
		return lua_toboolean(L, index) == 1;
	}

	template <>
	inline int32_t get<int32_t>(lua_State * L, int index)
	{
		return (int32_t)lua_tointeger(L, index);
	}

	template <>
	inline int64_t get<int64_t>(lua_State * L, int index)
	{
		return lua_tointeger(L, index);
	}

	template <>
	inline double get<double>(lua_State * L, int index)
	{
		return lua_tonumber(L, index);
	}

	template <>
	inline char const * get<char const *>(lua_State * L, int index)
	{
		return lua_tostring(L, index);
	}

	template <>
	inline FixedString get<FixedString>(lua_State* L, int index)
	{
		auto str = lua_tostring(L, index);
		if (str) {
			return FixedString{ str };
		} else {
			return FixedString{};
		}
	}

	template <>
	inline ObjectHandle get<ObjectHandle>(lua_State* L, int index)
	{
		return ObjectHandle{ (uint64_t)lua_touserdata(L, index) };
	}

	template <>
	inline EntityHandle get<EntityHandle>(lua_State* L, int index)
	{
		return EntityHandle{ (uint64_t)lua_touserdata(L, index) };
	}

	template <>
	inline UUID get<UUID>(lua_State* L, int index)
	{
		auto str = lua_tostring(L, index);
		if (str) {
			auto uuid = ParseGuidString(std::string_view(str, strlen(str)));
			if (uuid) {
				return *uuid;
			}
		}

		return UUID{};
	}


	template <class T, typename std::enable_if_t<std::is_same_v<T, bool>, int> * = nullptr>
	inline bool checked_get(lua_State * L, int index)
	{
		luaL_checktype(L, index, LUA_TBOOLEAN);
		return lua_toboolean(L, index) == 1;
	}

	template <class T, typename std::enable_if_t<std::is_integral_v<T>, std::enable_if_t<!std::is_same<T, bool>::value, int>> * = nullptr>
	inline T checked_get(lua_State * L, int index)
	{
		return (T)luaL_checkinteger(L, index);
	}

	template <class T, typename std::enable_if_t<std::is_floating_point_v<T>, int> * = nullptr>
	inline T checked_get(lua_State * L, int index)
	{
		return (T)luaL_checknumber(L, index);
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, char const *>, int> * = nullptr>
	inline char const * checked_get(lua_State * L, int index)
	{
		return luaL_checkstring(L, index);
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, FixedString>, int>* = nullptr>
	inline FixedString checked_get(lua_State* L, int index)
	{
		auto str = luaL_checkstring(L, index);
		FixedString fs(str);
		if (!fs && fs != GFS.strEmpty) {
			luaL_error(L, "Argument %d: expected a valid FixedString value, got '%s'", index, str);
			return {};
		} else {
			return fs;
		}
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, UUID>, int>* = nullptr>
	inline UUID checked_get(lua_State* L, int index)
	{
		auto str = luaL_checkstring(L, index);
		auto uuid = ParseGuidString(std::string_view(str, strlen(str)));
		if (!uuid) {
			luaL_error(L, "Argument %d: expected a valid GUID or NameGUID value, got '%s'", index, str);
		}
		
		return *uuid;
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, NetId>, int>* = nullptr>
	inline T checked_get(lua_State* L, int index)
	{
		return T(luaL_checkinteger(L, index));
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, UserId>, int>* = nullptr>
	inline T checked_get(lua_State* L, int index)
	{
		return T((int32_t)luaL_checkinteger(L, index));
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, ObjectHandle>, int>* = nullptr>
	inline ObjectHandle checked_get(lua_State* L, int index)
	{
		luaL_checktype(L, index, LUA_TLIGHTUSERDATA);
		return ObjectHandle{ (uint64_t)lua_touserdata(L, index) };
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, EntityHandle>, int>* = nullptr>
	inline EntityHandle checked_get(lua_State* L, int index)
	{
		luaL_checktype(L, index, LUA_TLIGHTUSERDATA);
		return EntityHandle{ (uint64_t)lua_touserdata(L, index) };
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, glm::ivec2>, int>* = nullptr>
	inline glm::ivec2 checked_get(lua_State* L, int index)
	{
		auto i = (index < 0) ? (index - 1) : index;
		glm::ivec2 val;
		luaL_checktype(L, index, LUA_TTABLE);
		push(L, 1);
		lua_rawget(L, i);
		val.x = checked_get<int32_t>(L, -1);
		lua_pop(L, 1);
		push(L, 2);
		lua_rawget(L, i);
		val.y = checked_get<int32_t>(L, -1);
		lua_pop(L, 1);

		return val;
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, glm::vec2>, int>* = nullptr>
	inline glm::vec2 checked_get(lua_State* L, int index)
	{
		auto i = (index < 0) ? (index - 1) : index;
		glm::vec2 val;
		luaL_checktype(L, index, LUA_TTABLE);
		push(L, 1);
		lua_rawget(L, i);
		val.x = checked_get<float>(L, -1);
		lua_pop(L, 1);
		push(L, 2);
		lua_rawget(L, i);
		val.y = checked_get<float>(L, -1);
		lua_pop(L, 1);

		return val;
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, glm::vec3>, int>* = nullptr>
	inline glm::vec3 checked_get(lua_State* L, int index)
	{
		auto i = (index < 0) ? (index - 1) : index;
		glm::vec3 val;
		luaL_checktype(L, index, LUA_TTABLE);

		push(L, 1);
		lua_rawget(L, i);
		val.x = checked_get<float>(L, -1);
		lua_pop(L, 1);

		push(L, 2);
		lua_rawget(L, i);
		val.y = checked_get<float>(L, -1);
		lua_pop(L, 1);

		push(L, 3);
		lua_rawget(L, i);
		val.z = checked_get<float>(L, -1);
		lua_pop(L, 1);

		return val;
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, glm::vec4>, int>* = nullptr>
	inline glm::vec4 checked_get(lua_State* L, int index)
	{
		auto i = (index < 0) ? (index - 1) : index;
		glm::vec4 val;
		luaL_checktype(L, index, LUA_TTABLE);

		push(L, 1);
		lua_rawget(L, i);
		val.x = checked_get<float>(L, -1);
		lua_pop(L, 1);

		push(L, 2);
		lua_rawget(L, i);
		val.y = checked_get<float>(L, -1);
		lua_pop(L, 1);

		push(L, 3);
		lua_rawget(L, i);
		val.z = checked_get<float>(L, -1);
		lua_pop(L, 1);

		push(L, 4);
		lua_rawget(L, i);
		val.w = checked_get<float>(L, -1);
		lua_pop(L, 1);

		return val;
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, STDString>, int>* = nullptr>
	inline STDString checked_get(lua_State* L, int index)
	{
		return STDString(luaL_checkstring(L, index));
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, STDWString>, int>* = nullptr>
	inline STDWString checked_get(lua_State* L, int index)
	{
		auto str = luaL_checkstring(L, index);
		return FromUTF8(str);
	}


	template <class T, typename std::enable_if_t<std::is_enum_v<T>, int> * = nullptr>
	T checked_get(lua_State * L, int index)
	{
		switch (lua_type(L, index)) {
		case LUA_TSTRING:
		{
			auto val = lua_tostring(L, index);
			auto index = EnumInfo<T>::Find(val);
			if (index) {
				return (T)*index;
			} else {
				luaL_error(L, "Param %d is not a valid '%s' enum label: %s", index, EnumInfo<T>::Name, val);
			}
			break;
		}

		case LUA_TNUMBER:
		{
			auto val = lua_tointeger(L, index);
			auto index = EnumInfo<T>::Find((T)val);
			if (index) {
				return (T)val;
			} else {
				luaL_error(L, "Param %d is not a valid '%s' enum index: %d", index, EnumInfo<T>::Name, val);
			}
			break;
		}

		default:
			luaL_error(L, "Param %d: expected integer or string '%s' enumeration value, got %s", index,
				EnumInfo<T>::Name, lua_typename(L, lua_type(L, index)));
		}

		return (T)0;
	}



	template <class T, typename std::enable_if_t<std::is_enum_v<T>, int> * = nullptr>
	std::optional<T> safe_get(lua_State * L, int index = -1)
	{
		switch (lua_type(L, index)) {
		case LUA_TSTRING:
		{
			auto val = lua_tostring(L, index);
			auto index = EnumInfo<T>::Find(val);
			if (index) {
				return (T)*index;
			} else {
				ERR("'%s' is not a valid enumeration label in '%s'", val, EnumInfo<T>::Name);
				return {};
			}
			break;
		}

		case LUA_TNUMBER:
		{
			auto val = lua_tointeger(L, index);
			auto index = EnumInfo<T>::Find((T)val);
			if (index) {
				return (T)val;
			} else {
				ERR("'%d' is not a valid enumeration index in '%s'", val, EnumInfo<T>::Name);
				return {};
			}
			break;
		}

		default:
			return {};
		}
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, bool>, int> * = nullptr>
	inline std::optional<T> safe_get(lua_State * L, int index)
	{
		if (lua_type(L, index) == LUA_TBOOLEAN) {
			return lua_toboolean(L, index) == 1;
		} else {
			return {};
		}
	}

	template <class T, typename std::enable_if_t<std::is_integral_v<T>, std::enable_if_t<!std::is_same_v<T, bool>, int>> * = nullptr>
	inline std::optional<T> safe_get(lua_State * L, int index)
	{
		if (lua_type(L, index) == LUA_TNUMBER) {
			return (T)lua_tointeger(L, index);
		} else {
			return {};
		}
	}

	template <class T, typename std::enable_if_t<std::is_floating_point_v<T>, int> * = nullptr>
	inline std::optional<T> safe_get(lua_State * L, int index)
	{
		if (lua_type(L, index) == LUA_TNUMBER) {
			return (T)lua_tonumber(L, index);
		} else {
			return {};
		}
	}

	template <class T, typename std::enable_if_t<std::is_same_v<T, char const *>, int> * = nullptr>
	inline std::optional<char const *> safe_get(lua_State * L, int index)
	{
		if (lua_type(L, index) == LUA_TSTRING) {
			return lua_tostring(L, index);
		} else {
			return {};
		}
	}


	template <class TValue>
	TValue getfield(lua_State* L, char const* k, int index = -1)
	{
		lua_getfield(L, index, k);
		TValue val = get<TValue>(L, -1);
		lua_pop(L, 1);
		return val;
	}

	template <class TKey, class TValue>
	TValue gettable(lua_State * L, TKey const & k, int index = -2)
	{
		push(L, k);
		lua_gettable(L, index);
		TValue val = get<TValue>(L, -1);
		lua_pop(L, 1);
		return val;
	}

	template <class TValue>
	TValue checked_getfield(lua_State* L, char const* k, int index = -1)
	{
		lua_getfield(L, index, k);
		TValue val = checked_get<TValue>(L, -1);
		lua_pop(L, 1);
		return val;
	}

	template <class TKey, class TValue>
	TValue checked_gettable(lua_State* L, TKey const& k, int index = -2)
	{
		push(L, k);
		lua_gettable(L, index);
		TValue val = checked_get<TValue>(L, -1);
		lua_pop(L, 1);
		return val;
	}


	template <class T, class... Args>
	inline auto Push(Args... args)
	{
		return[args...](lua_State * L) {
			auto obj = T::New(L, args...);
			if constexpr (std::is_base_of_v<Pushable<PushPolicy::Unbind>, T>) {
				return UnbindablePin{ obj };
			} else {
				return NullPin{};
			}
		};
	}

	// Pushes all arguments to the Lua stack and returns a pin that should
	// be destroyed after the call
	template <class ...Args>
	inline auto PushArguments(lua_State * L, std::tuple<Args...> args)
	{
		return std::apply([=](const auto &... elem)
		{
			return std::tuple{ push_pin(L, elem)... };
		}, args);
	}

	// Helper for indicating return type of a Lua function
	template <class... Args>
	struct ReturnType {};

	// Helper struct to allow function overloading without (real) template-dependent parameters
	template <class>
	struct Overload {};

	// Fetches a required return value (i.e. succeeded = false if arg doesn't exist or is nil)
	template <class T>
	T CheckedGetReturnValue(lua_State * L, int & index, bool & succeeded, Overload<T>)
	{
		auto i = index--;
		if (lua_isnil(L, i)) {
			ERR("Return value %d must not be missing or nil", -i);
			succeeded = false;
			return {};
		} else {
			auto val = safe_get<T>(L, i);
			if ((bool)val) {
				return *val;
			} else {
				ERR("Failed to fetch return value %d, incorrect type?", -i);
				succeeded = false;
				return {};
			}
		}
	}

	// Fetches an optional return value (i.e. succeeded = true if arg doesn't exist or is nil)
	template <class T>
	std::optional<T> CheckedGetReturnValue(lua_State * L, int & index, bool & succeeded, Overload<std::optional<T>>)
	{
		auto i = index--;
		if (lua_isnil(L, i)) {
			return {};
		} else {
			auto val = safe_get<T>(L, i);
			if ((bool)val) {
				return val;
			} else {
				ERR("Failed to fetch return value %d, incorrect type?", -i);
				succeeded = false;
				return {};
			}
		}
	}

	// Fetch Lua return values into a tuple
	// Sets succeeded=false if validation of any return value failed.
	// Tuple size *must* match lua_call nres, otherwise it'll corrupt the Lua heap!
	template <class... Args>
	auto CheckedGetReturnValues(lua_State * L, bool & succeeded)
	{
		int index{ -1 };
		return std::tuple{CheckedGetReturnValue(L, index, succeeded, Overload<Args>{})...};
	}

	// Fetch Lua return values into a tuple
	// Returns {} if validation of any return value failed, rval tuple otherwise.
	// Tuple size *must* match lua_call nres, otherwise it'll corrupt the Lua heap!
	template <class... Args>
	auto CheckedPopReturnValues(lua_State * L)
	{
		bool succeeded{ true };
		auto rval = CheckedGetReturnValues<Args...>(L, succeeded);
		lua_pop(L, (int)sizeof...(Args));
		if (succeeded) {
			return std::optional(rval);
		} else {
			return std::optional<decltype(rval)>();
		}
	}

	// Calls Lua function and fetches Lua return values into a tuple.
	// Function and arguments must be already pushed to the Lua stack.
	// Returns {} if call or return value fetch failed, rval tuple otherwise.
	// Function name only needed for error reporting purposes
	template <class... Args>
	auto CheckedCall(lua_State * L, int numArgs, char const * functionName)
	{
		if (CallWithTraceback(L, numArgs, sizeof...(Args)) != 0) { // stack: errmsg
			ERR("%s Lua call failed: %s", functionName, lua_tostring(L, -1));
			lua_pop(L, 1);
			return decltype(CheckedPopReturnValues<Args...>(L))();
		}

		auto result = CheckedPopReturnValues<Args...>(L);
		if (!result) {
			ERR("Got incorrect return values from %s", functionName);
		}

		return result;
	}

	// Overload helper for fetching a parameter for a Lua -> C++ function call
	template <class T>
	T CheckedGetParam(lua_State* L, int i, Overload<T>)
	{
		return checked_get<T>(L, i);
	}

	// Overload helper for fetching a parameter for a Lua -> C++ function call
	template <class T>
	std::optional<T> CheckedGetParam(lua_State* L, int i, Overload<std::optional<T>>)
	{
		if (lua_gettop(L) < i || lua_isnil(L, i)) {
			return {};
		} else {
			return checked_get<T>(L, i);
		}
	}

	template <class... Args, size_t... Is>
	inline void PushReturnTupleInner(lua_State* L, std::tuple<Args...> t, std::index_sequence<Is...>)
	{
		(push(L, std::get<Is>(t)), ...);
	}

	// Pushes all elements of a tuple (in order) to the Lua stack
	template <class... Args>
	inline void PushReturnTuple(lua_State* L, std::tuple<Args...> t)
	{
		PushReturnTupleInner(L, std::move(t), std::index_sequence_for<Args...>{});
	}

	// Lua -> C++ wrapper -- no return case
	template <class... Args, size_t... Is>
	inline int LuaCallWrapper2(lua_State* L, void(*fun)(lua_State* L, Args... args), std::index_sequence<Is...>)
	{
		StackCheck _(L, 0);
		fun(L, CheckedGetParam(L, 1 + Is, Overload<Args>{})...);
		return 0;
	}

	// Lua -> C++ wrapper -- single return value case
	template <class R, class... Args, size_t... Is>
	inline int LuaCallWrapper2(lua_State* L, R(*fun)(lua_State* L, Args... args), std::index_sequence<Is...>)
	{
		StackCheck _(L, 1);
		auto retval = fun(L, CheckedGetParam(L, 1 + Is, Overload<Args>{})...);
		push(L, retval);
		return 1;
	}

	// Lua -> C++ wrapper -- multiple return values case
	template <class... R, class... Args, size_t... Is>
	inline int LuaCallWrapper2(lua_State* L, std::tuple<R...>(*fun)(lua_State* L, Args... args), std::index_sequence<Is...>)
	{
		StackCheck _(L, sizeof...(R));
		auto retval = fun(L, CheckedGetParam(L, 1 + Is, Overload<Args>{})...);
		PushReturnTuple(L, std::move(retval));
		return sizeof...(R);
	}

	// Untyped wrapper function for calling C++ functions with a typed signature
	template <class R, class... Args>
	int LuaCallWrapper(lua_State* L, R(*fun)(lua_State* L, Args... args))
	{
		return LuaCallWrapper2(L, fun, std::index_sequence_for<Args...>{});
	}

#define WrapLuaFunction(fun) \
	int fun##Wrapper(lua_State* L) \
	{ \
		return LuaCallWrapper(L, fun); \
	}


	class RegistryEntry
	{
	public:
		RegistryEntry();
		RegistryEntry(lua_State * L, int index);
		~RegistryEntry();

		RegistryEntry(RegistryEntry const &) = delete;
		RegistryEntry(RegistryEntry &&);

		RegistryEntry & operator = (RegistryEntry const &) = delete;
		RegistryEntry & operator = (RegistryEntry &&);

		void Push() const;

	private:
		lua_State * L_;
		int ref_;
	};

	int CallWithTraceback(lua_State * L, int narg, int nres);

	class Callable {};
	class Indexable {};
	class NewIndexable {};
	class Lengthable {};

	template <class T>
	class Userdata
	{
	public:
		static T * AsUserData(lua_State * L, int index)
		{
			if (lua_type(L, index) == LUA_TUSERDATA) {
				auto obj = luaL_testudata(L, index, T::MetatableName);
				return reinterpret_cast<T *>(obj);
			} else {
				return nullptr;
			}
		}

		static T * CheckUserData(lua_State * L, int index)
		{
			return reinterpret_cast<T *>(luaL_checkudata(L, index, T::MetatableName));
		}

		template <class... Args>
		static T * New(lua_State * L, Args... args)
		{
			auto obj = reinterpret_cast<T *>(lua_newuserdata(L, sizeof(T)));
			new (obj) T(std::forward<Args>(args)...);
			luaL_setmetatable(L, T::MetatableName);
			return obj;
		}

		static int CallProxy(lua_State * L)
		{
			if constexpr (std::is_base_of_v<Callable, T>) {
				auto self = CheckUserData(L, 1);
				return self->LuaCall(L);
			} else {
				return luaL_error(L, "Not callable!");
			}
		}

		static int IndexProxy(lua_State * L)
		{
			if constexpr (std::is_base_of_v<Indexable, T>) {
				auto self = CheckUserData(L, 1);
				return self->Index(L);
			} else {
				return luaL_error(L, "Not indexable!");
			}
		}

		static int NewIndexProxy(lua_State * L)
		{
			if constexpr (std::is_base_of_v<NewIndexable, T>) {
				auto self = CheckUserData(L, 1);
				return self->NewIndex(L);
			} else {
				return luaL_error(L, "Not newindexable!");
			}
		}

		static int LengthProxy(lua_State * L)
		{
			if constexpr (std::is_base_of_v<Lengthable, T>) {
				auto self = CheckUserData(L, 1);
				return self->Length(L);
			} else {
				return luaL_error(L, "Not lengthable!");
			}
		}

		static void PopulateMetatable(lua_State * L)
		{
			// Add custom metatable items by overriding this in subclasses
		}

		static void RegisterMetatable(lua_State * L)
		{
			lua_register(L, T::MetatableName, nullptr);
			luaL_newmetatable(L, T::MetatableName); // stack: mt

			push(L, T::MetatableName);
			lua_setfield(L, -2, "__metatable");

			if constexpr (std::is_base_of_v<Callable, T>) {
				lua_pushcfunction(L, &CallProxy); // stack: mt, &LuaCall
				lua_setfield(L, -2, "__call"); // mt.__call = &LuaCall; stack: mt
			}

			if constexpr (std::is_base_of_v<Indexable, T>) {
				lua_pushcfunction(L, &IndexProxy); // stack: mt, &Index
				lua_setfield(L, -2, "__index"); // mt.__index = &Index; stack: mt
			}

			if constexpr (std::is_base_of_v<NewIndexable, T>) {
				lua_pushcfunction(L, &NewIndexProxy); // stack: mt, &NewIndex
				lua_setfield(L, -2, "__newindex"); // mt.__index = &NewIndex; stack: mt
			}

			if constexpr (std::is_base_of_v<Lengthable, T>) {
				lua_pushcfunction(L, &LengthProxy); // stack: mt, &Length
				lua_setfield(L, -2, "__len"); // mt.__index = &Length; stack: mt
			}

			T::PopulateMetatable(L);

			lua_pop(L, 1); // stack: -
		}
	};

	template <class T>
	inline std::optional<T *> safe_get_userdata(lua_State * L, int index)
	{
		if (lua_isnil(L, index)) {
			return {};
		} else {
			auto val = T::AsUserData(L, index);
			if (val) {
				return val;
			} else {
				ERR("Expected userdata of type '%s'", T::MetatableName);
				return {};
			}
		}
	}

	template <class T, typename std::enable_if_t<std::is_base_of_v<Userdata<std::remove_pointer_t<T>>, std::remove_pointer_t<T>>, int> * = nullptr>
	inline std::optional<T> safe_get(lua_State * L, int index)
	{
		return safe_get_userdata<std::remove_pointer_t<T>>(L, index);
	}


	template <class T, typename std::enable_if_t<std::is_base_of_v<Userdata<std::remove_pointer_t<T>>, std::remove_pointer_t<T>>, int> * = nullptr>
	inline T checked_get(lua_State * L, int index)
	{
		return std::remove_pointer_t<T>::CheckUserData(L, index);
	}


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
			if (Index < 0) Index--;

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
		helper.Index = index;
		return helper;
	}

	template <class T>
	inline T checked_get_flags(lua_State* L, int index)
	{
		static_assert(std::is_base_of_v<BitmaskInfoBase<T>, EnumInfo<T>>, "Can only fetch bitmask fields!");

		luaL_checktype(L, index, LUA_TTABLE);
		T flags = (T)0;
		for (auto idx : iterate(L, index)) {
			auto label = checked_get<FixedString>(L, idx);
			auto val = EnumInfo<T>::Find(label);
			if (val) {
				flags |= *val;
			} else {
				luaL_error(L, "Label '%s' is not valid for enumeration '%s'", label.GetString(), EnumInfo<T>::Name);
			}
		}

		return flags;
	}
}
