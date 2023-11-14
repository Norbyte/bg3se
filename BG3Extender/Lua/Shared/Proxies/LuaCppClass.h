#pragma once

#include <Lua/LuaHelpers.h>
#include <Lua/Shared/Proxies/LuaPropertyMap.h>

BEGIN_NS(lua)

class ArrayProxyImplBase;
class MapProxyImplBase;
class SetProxyImplBase;

class CppPropertyMapManager
{
public:
	int RegisterPropertyMap(GenericPropertyMap* mt);
	GenericPropertyMap* GetPropertyMap(int index);
	
	int RegisterArrayProxy(ArrayProxyImplBase* mt);
	ArrayProxyImplBase* GetArrayProxy(int index);
	
	int RegisterMapProxy(MapProxyImplBase* mt);
	MapProxyImplBase* GetMapProxy(int index);
	
	int RegisterSetProxy(SetProxyImplBase* mt);
	SetProxyImplBase* GetSetProxy(int index);

private:
	Array<GenericPropertyMap*> propertyMaps_;
	Array<ArrayProxyImplBase*> arrayProxies_;
	Array<MapProxyImplBase*> mapProxies_;
	Array<SetProxyImplBase*> setProxies_;
};

struct CppObjectMetadata
{
	void* Ptr;
	MetatableTag MetatableTag;
	uint16_t PropertyMapTag;
	LifetimeHandle Lifetime;
};

struct CppValueMetadata
{
	uint64_t Value;
	MetatableTag MetatableTag;
	uint32_t PropertyMapTag;
};

class CppMetatableManager
{
public:
	CppMetatableManager();
	void RegisterMetatable(MetatableTag tag, CMetatable* mt);
	CMetatable* GetMetatable(MetatableTag tag);

	static CppMetatableManager& FromLua(lua_State* L);

private:
	Array<CMetatable *> metatables_;
};

// NOTE: Must match TMS enumeration in Lua
enum class MetamethodName : int
{
	Index,
	NewIndex,
	GC,
	Mode,
	Len,
	Eq,
	Add,
	Sub,
	Mul,
	Mod,
	Pow,
	Div,
	IDiv,
	BAnd,
	BOr,
	BXor,
	Shl,
	Shr,
	Unm,
	BNot,
	Lt,
	Le,
	Concat,
	Call,
	Pairs,
	ToString,
	Name
};

template <class TSubclass>
class LightCppObjectMetatable
{
public:
	static std::optional<CppObjectMetadata> TryGetMetadata(lua_State* L, int index)
	{
		CppObjectMetadata meta;
		if (lua_try_get_cppobject(L, index, TSubclass::MetaTag, meta)) {
			return meta;
		} else {
			return {};
		}
	}

	static int CallProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Callable, TSubclass>) {
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, TSubclass::MetaTag, self);
			return TSubclass::Call(L, self);
		} else {
			return luaL_error(L, "Not callable!");
		}
	}

	static int IndexProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Indexable, TSubclass>) {
			StackCheck _(L, 1);
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, TSubclass::MetaTag, self);

			if constexpr (TSubclass::HasLifetime) {
				if (!self.Lifetime.IsAlive(L)) {
					luaL_error(L, "Attempted to read '%s' whose lifetime has expired", TSubclass::GetTypeName(L, self));
					return 0;
				}
			}

			return TSubclass::Index(L, self);
		} else {
			return luaL_error(L, "Not indexable!");
		}
	}

	static int NewIndexProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<NewIndexable, TSubclass>) {
			StackCheck _(L, 0);
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, TSubclass::MetaTag, self);

			if constexpr (TSubclass::HasLifetime) {
				if (!self.Lifetime.IsAlive(L)) {
					luaL_error(L, "Attempted to write '%s' whose lifetime has expired", TSubclass::GetTypeName(L, self));
					return 0;
				}
			}

			return TSubclass::NewIndex(L, self);
		} else {
			return luaL_error(L, "Not newindexable!");
		}
	}

	static int LengthProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Lengthable, TSubclass>) {
			StackCheck _(L, 1);
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, TSubclass::MetaTag, self);

			if constexpr (TSubclass::HasLifetime) {
				if (!self.Lifetime.IsAlive(L)) {
					luaL_error(L, "Attempted to get length of '%s' whose lifetime has expired", TSubclass::GetTypeName(L, self));
					push(L, nullptr);
					return 1;
				}
			}

			return TSubclass::Length(L, self);
		} else {
			return luaL_error(L, "Not lengthable!");
		}
	}

	static int PairsProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Iterable, TSubclass>) {
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, TSubclass::MetaTag, self);
			return TSubclass::Pairs(L, self);
		} else {
			return luaL_error(L, "Not iterable!");
		}
	}

	static int ToStringProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Stringifiable, TSubclass>) {
			StackCheck _(L, 1);
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, TSubclass::MetaTag, self);
			return TSubclass::ToString(L, self);
		} else {
			return luaL_error(L, "Not stringifiable!");
		}
	}

	static int EqualProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<EqualityComparable, TSubclass>) {
			StackCheck _(L, 1);
			CppObjectMetadata self, other;
			lua_get_cppobject(L, 1, TSubclass::MetaTag, self);

			bool equal;
			if (lua_try_get_cppobject(L, 2, TSubclass::MetaTag, other)) {
				equal = TSubclass::IsEqual(L, self, other);
			} else {
				equal = false;
			}

			push(L, equal);
			return 1;
		} else {
			return luaL_error(L, "Not comparable!");
		}
	}

	// Default __pairs implementation
	static int Pairs(lua_State* L, CppObjectMetadata const& self)
	{
		StackCheck _(L, 3);
		lua_pushcfunction(L, &NextProxy);
		lua_pushvalue(L, 1);
		push(L, nullptr);

		return 3;
	}

	static int NextProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Iterable, TSubclass>) {
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, TSubclass::MetaTag, self);

			if (!self.Lifetime.IsAlive(L)) {
				luaL_error(L, "Attempted to iterate '%s' whose lifetime has expired", TSubclass::GetTypeName(L, self));
				return 0;
			}

			return TSubclass::Next(L, self);
		} else {
			return luaL_error(L, "Not iterable!");
		}
	}

	static int NameProxy(lua_State* L)
	{
		StackCheck _(L, 1);
		CppObjectMetadata self;
		lua_get_cppobject(L, 1, TSubclass::MetaTag, self);
		push(L, TSubclass::GetTypeName(L, self));
		return 1;
	}

	static void PopulateMetatable(lua_State* L, CMetatable* mt)
	{
		// Add custom metatable items by overriding this in subclasses
	}

	static void RegisterMetatable(lua_State* L)
	{
		auto mt = lua_alloc_cmetatable(L);

		if constexpr (std::is_base_of_v<Callable, TSubclass>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Call, &CallProxy);
		}

		if constexpr (std::is_base_of_v<Indexable, TSubclass>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Index, &IndexProxy);
		}

		if constexpr (std::is_base_of_v<NewIndexable, TSubclass>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::NewIndex, &NewIndexProxy);
		}

		if constexpr (std::is_base_of_v<Lengthable, TSubclass>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Len, &LengthProxy);
		}

		if constexpr (std::is_base_of_v<Iterable, TSubclass>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Pairs, &PairsProxy);
		}

		if constexpr (std::is_base_of_v<Stringifiable, TSubclass>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::ToString, &ToStringProxy);
		}

		// Light cppobjects are not garbage collected (they're passed by value)

		if constexpr (std::is_base_of_v<EqualityComparable, TSubclass>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Eq, &EqualProxy);
		}

		lua_cmetatable_set(L, mt, (int)MetamethodName::Name, &NameProxy);

		TSubclass::PopulateMetatable(L, mt);
		CppMetatableManager::FromLua(L).RegisterMetatable(TSubclass::MetaTag, mt);
	}
};

END_NS()
