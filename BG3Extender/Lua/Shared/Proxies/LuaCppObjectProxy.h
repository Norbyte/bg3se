#pragma once

#include <Lua/Shared/Proxies/LuaCppClass.h>

BEGIN_NS(lua)

struct CppObjectProxyHelpers
{
	static int Next(lua_State* L, GenericPropertyMap const& pm, void* object, LifetimeHandle const& lifetime, FixedString const& key);
};


class LightObjectProxyByRefMetatable : public LightCppObjectMetatable<LightObjectProxyByRefMetatable>,
	public Indexable, public NewIndexable, public Iterable, public Stringifiable, public EqualityComparable
{
public:
	static constexpr MetatableTag MetaTag = MetatableTag::ObjectProxyByRef;
	static constexpr bool HasLifetime = true;

	inline static void Make(lua_State* L, GenericPropertyMap& pm, void* object, LifetimeHandle const& lifetime)
	{
		lua_push_cppobject(L, MetaTag, pm.RegistryIndex, object, lifetime);
	}

	inline static void Make(lua_State* L, GenericPropertyMap& pm, void const* object, LifetimeHandle const& lifetime)
	{
		// TODO - add RO tag
		lua_push_cppobject(L, MetaTag, pm.RegistryIndex, object, lifetime);
	}

	template <class T>
	inline static void Make(lua_State* L, T* object, LifetimeHandle const& lifetime)
	{
		auto const& pm = GetStaticPropertyMap<T>();
		lua_push_cppobject(L, MetaTag, pm.RegistryIndex, object, lifetime);
	}

	template <class T>
	inline static void Make(lua_State* L, T const* object, LifetimeHandle const& lifetime)
	{
		// TODO - add RO tag
		auto const& pm = GetStaticPropertyMap<T>();
		lua_push_cppobject(L, MetaTag, pm.RegistryIndex, object, lifetime);
	}

	static GenericPropertyMap& GetPropertyMap(CppObjectMetadata const& meta);
	static void* TryGetGeneric(lua_State* L, int index, int propertyMapIndex);
	static void* GetGeneric(lua_State* L, int index, int propertyMapIndex);

	template <class T>
	static T* TryGet(lua_State* L, int index)
	{
		auto const& pm = GetStaticPropertyMap<T>();
		return reinterpret_cast<T*>(TryGetGeneric(L, index, pm.PropertyMapTag));
	}

	template <class T>
	static T* Get(lua_State* L, int index)
	{
		auto ptr = GetGeneric(L, index, StructID<T>::ID);
		return reinterpret_cast<T*>(ptr);
	}

	static int Index(lua_State* L, CppObjectMetadata& self);
	static int NewIndex(lua_State* L, CppObjectMetadata& self);
	static int ToString(lua_State* L, CppObjectMetadata& self);
	static bool IsEqual(lua_State* L, CppObjectMetadata& self, CppObjectMetadata& other);
	static int Next(lua_State* L, CppObjectMetadata& self);
	static char const* GetTypeName(lua_State* L, CppObjectMetadata& self);
};


class ObjectProxy
{
public:
	inline static void MakeRef(lua_State* L, GenericPropertyMap& pm, void* object, LifetimeHandle const& lifetime)
	{
		if (!pm.ValidatePropertyMap(object)) {
			push(L, nullptr);
		} else {
			LightObjectProxyByRefMetatable::Make(L, pm, object, lifetime);
		}
	}
	
	template <class T>
	inline static void MakeRef(lua_State* L, T* object, LifetimeHandle const& lifetime)
	{
		if (!GetStaticPropertyMap<T>().ValidatePropertyMap(object)) {
			push(L, nullptr);
		} else {
			LightObjectProxyByRefMetatable::Make(L, object, lifetime);
		}
	}

	static void* GetRaw(lua_State* L, int index, GenericPropertyMap const& pm);

	template <class T>
	inline static T* Get(lua_State* L, int index)
	{
		return reinterpret_cast<T*>(GetRaw(L, index, GetStaticPropertyMap<T>()));
	}
};

template <class T>
inline T* GetObjectRef(lua_State* L, int index)
{
	return ObjectProxy::Get<T>(L, index);
}

END_NS()
