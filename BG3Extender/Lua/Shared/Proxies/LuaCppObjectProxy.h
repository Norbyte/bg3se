#pragma once

#include <Lua/Shared/Proxies/LuaCppClass.h>

BEGIN_NS(lua)

class LightObjectProxyByRefMetatable : public LightCppObjectMetatable<LightObjectProxyByRefMetatable>,
	public Indexable, public NewIndexable, public Iterable, public Stringifiable, public EqualityComparable
{
public:
	static constexpr MetatableTag MetaTag = MetatableTag::ObjectProxyByRef;

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
	template <class T>
	inline static void MakeRef(lua_State* L, T* object, LifetimeHandle const& lifetime)
	{
		if (!StaticLuaPropertyMap<T>::PropertyMap.ValidatePropertyMap(object)) {
			push(L, nullptr);
		} else {
			LightObjectProxyByRefMetatable::Make(L, object, lifetime);
		}
	}

	static void* GetRaw(lua_State* L, int index, GenericPropertyMap const& pm);

	template <class T>
	inline static T* Get(lua_State* L, int index)
	{
		return reinterpret_cast<T*>(GetRaw(L, index, StaticLuaPropertyMap<T>::PropertyMap));
	}
};

END_NS()
