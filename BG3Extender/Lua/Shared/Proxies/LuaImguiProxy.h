#pragma once

#include <Extender/Client/IMGUI/Objects.h>

BEGIN_NS(lua)

class ImguiObjectProxyMetatable : public LightCppValueMetatable<ImguiObjectProxyMetatable>,
	public Indexable, public NewIndexable, public Iterable, public Stringifiable, public EqualityComparable
{
public:
	static constexpr MetatableTag MetaTag = MetatableTag::ImguiObject;
	static constexpr bool HasLifetime = false;

	inline static void Make(lua_State* L, ImguiHandle const& object)
	{
		lua_push_cppvalue(L, MetaTag, 0, object.Handle);
	}

	inline static void Make(lua_State* L, extui::Renderable* object)
	{
		lua_push_cppvalue(L, MetaTag, 0, object->Handle);
	}

	static GenericPropertyMap& GetPropertyMap(CppValueMetadata const& meta);
	static extui::Renderable* GetRenderable(CppValueMetadata const& meta);
	static extui::Renderable* TryGetGeneric(lua_State* L, int index, extui::IMGUIObjectType type);
	static extui::Renderable* GetGeneric(lua_State* L, int index, extui::IMGUIObjectType type);
	static extui::Renderable* GetGeneric(lua_State* L, int index);

	template <class T>
	static T* TryGet(lua_State* L, int index)
	{
		return reinterpret_cast<T*>(TryGetGeneric(L, index, T::ObjectType));
	}

	template <class T>
	static T* Get(lua_State* L, int index)
	{
		return reinterpret_cast<T*>(GetGeneric(L, index, T::ObjectType));
	}

	static int Index(lua_State* L, CppValueMetadata& self);
	static int NewIndex(lua_State* L, CppValueMetadata& self);
	static int ToString(lua_State* L, CppValueMetadata& self);
	static bool IsEqual(lua_State* L, CppValueMetadata& self, int otherIndex);
	static int Next(lua_State* L, CppValueMetadata& self);
	static char const* GetTypeName(lua_State* L, CppValueMetadata& self);
};

END_NS()
