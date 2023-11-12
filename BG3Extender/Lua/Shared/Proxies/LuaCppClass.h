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

template <class T>
class LightCppObjectMetatable
{
public:
	template <class TObj>
	inline static void Make(lua_State* L, TObj* object, LifetimeHandle const& lifetime)
	{
		auto const& pm = StaticLuaPropertyMap<TObj>::PropertyMap;
		lua_push_cppobject(L, T::MetaTag, pm.RegistryIndex, object, lifetime);
	}

	static std::optional<CppObjectMetadata> TryGet(lua_State* L, int index)
	{
		CppObjectMetadata meta;
		if (lua_try_get_cppobject(L, index, T::MetaTag, meta)) {
			return meta;
		} else {
			return {};
		}
	}

	static void* Get(lua_State* L, int index, int propertyMapIndex)
	{
		CppObjectMetadata meta;
		if (lua_try_get_cppobject(L, index, meta)) {
			auto& pm = LuaGetPropertyMap(meta.PropertyMapTag);
			if (pm.IsA(propertyMapIndex)) {
				return meta.Ptr;
			} else {
				luaL_error(L, "Argument %d: Expected object of type '%s', got '%s'", index,
					LuaGetPropertyMap(propertyMapIndex).Name.GetString(),
					pm.Name.GetString());
				return nullptr;
			}
		} else {
			luaL_error(L, "Argument %d: Expected object of type '%s', got '%s'", index,
				LuaGetPropertyMap(propertyMapIndex).Name.GetString(),
				lua_typename(L, lua_type(L, index)));
			return nullptr;
		}
	}

	template <class T>
	static T* GetTyped(lua_State* L, int index)
	{
		auto ptr = Get(L, index, StaticLuaPropertyMap<T>::PropertyMap.RegistryIndex);
		return reinterpret_cast<T*>(ptr);
	}

	static int CallProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Callable, T>) {
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, T::MetaTag, self);
			return T::Call(L, self);
		} else {
			return luaL_error(L, "Not callable!");
		}
	}

	static int IndexProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Indexable, T>) {
			StackCheck _(L, 1);
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, T::MetaTag, self);

			if (!self.Lifetime.IsAlive(L)) {
				luaL_error(L, "Attempted to read '%s' whose lifetime has expired", T::GetTypeName(L, self));
				return 0;
			}

			return T::Index(L, self);
		} else {
			return luaL_error(L, "Not indexable!");
		}
	}

	static int NewIndexProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<NewIndexable, T>) {
			StackCheck _(L, 0);
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, T::MetaTag, self);

			if (!self.Lifetime.IsAlive(L)) {
				luaL_error(L, "Attempted to write '%s' whose lifetime has expired", T::GetTypeName(L, self));
				return 0;
			}

			return T::NewIndex(L, self);
		} else {
			return luaL_error(L, "Not newindexable!");
		}
	}

	static int LengthProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Lengthable, T>) {
			StackCheck _(L, 1);
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, T::MetaTag, self);

			if (!self.Lifetime.IsAlive(L)) {
				luaL_error(L, "Attempted to get length of '%s' whose lifetime has expired", T::GetTypeName(L, self));
				push(L, nullptr);
				return 1;
			}

			return T::Length(L, self);
		} else {
			return luaL_error(L, "Not lengthable!");
		}
	}

	static int PairsProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Iterable, T>) {
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, T::MetaTag, self);
			return T::Pairs(L, self);
		} else {
			return luaL_error(L, "Not iterable!");
		}
	}

	static int ToStringProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<Stringifiable, T>) {
			StackCheck _(L, 1);
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, T::MetaTag, self);
			return T::ToString(L, self);
		} else {
			return luaL_error(L, "Not stringifiable!");
		}
	}

	static int EqualProxy(lua_State* L)
	{
		if constexpr (std::is_base_of_v<EqualityComparable, T>) {
			StackCheck _(L, 1);
			CppObjectMetadata self, other;
			lua_get_cppobject(L, 1, T::MetaTag, self);

			bool equal;
			if (lua_try_get_cppobject(L, 2, T::MetaTag, other)) {
				equal = T::IsEqual(L, self, other);
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
		if constexpr (std::is_base_of_v<Iterable, T>) {
			CppObjectMetadata self;
			lua_get_cppobject(L, 1, T::MetaTag, self);

			if (!self.Lifetime.IsAlive(L)) {
				luaL_error(L, "Attempted to iterate '%s' whose lifetime has expired", T::GetTypeName(L, self));
				return 0;
			}

			return T::Next(L, self);
		} else {
			return luaL_error(L, "Not iterable!");
		}
	}

	static int NameProxy(lua_State* L)
	{
		StackCheck _(L, 1);
		CppObjectMetadata self;
		lua_get_cppobject(L, 1, T::MetaTag, self);
		push(L, T::GetTypeName(L, self));
		return 1;
	}

	static void PopulateMetatable(lua_State* L, CMetatable* mt)
	{
		// Add custom metatable items by overriding this in subclasses
	}

	static void RegisterMetatable(lua_State* L)
	{
		auto mt = lua_alloc_cmetatable(L);

		if constexpr (std::is_base_of_v<Callable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Call, &CallProxy);
		}

		if constexpr (std::is_base_of_v<Indexable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Index, &IndexProxy);
		}

		if constexpr (std::is_base_of_v<NewIndexable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::NewIndex, &NewIndexProxy);
		}

		if constexpr (std::is_base_of_v<Lengthable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Len, &LengthProxy);
		}

		if constexpr (std::is_base_of_v<Iterable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Pairs, &PairsProxy);
		}

		if constexpr (std::is_base_of_v<Stringifiable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::ToString, &ToStringProxy);
		}

		// Light cppobjects are not garbage collected (they're passed by value)

		if constexpr (std::is_base_of_v<EqualityComparable, T>) {
			lua_cmetatable_set(L, mt, (int)MetamethodName::Eq, &EqualProxy);
		}

		lua_cmetatable_set(L, mt, (int)MetamethodName::Name, &NameProxy);

		T::PopulateMetatable(L, mt);
		CppMetatableManager::FromLua(L).RegisterMetatable(T::MetaTag, mt);
	}
};

END_NS()
