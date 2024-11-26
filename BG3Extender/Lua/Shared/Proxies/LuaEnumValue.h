#pragma once

#include <Lua/Shared/Proxies/LuaCppValue.h>

BEGIN_NS(lua)

class EnumValueMetatable : public LightCppValueMetatable<EnumValueMetatable>,
	public Indexable, public Stringifiable, public EqualityComparable, public LessThanComparable
{
public:
	static constexpr MetatableTag MetaTag = MetatableTag::EnumValue;

	inline static void Make(lua_State* L, EnumUnderlyingType value, int registryIndex)
	{
		lua_push_cppvalue(L, MetaTag, registryIndex, static_cast<uint64_t>(value));
	}

	template <class T>
	inline static void Make(lua_State* L, T value)
	{
		Make(L, static_cast<EnumUnderlyingType>(value), EnumID<T>::ID);
	}

	static EnumInfoStore* GetEnumInfo(CppObjectMetadata const& val);
	static FixedString GetLabel(CppObjectMetadata const& self);
	static EnumUnderlyingType GetValue(CppObjectMetadata const& self);

	static int Index(lua_State* L, CppObjectMetadata& self);
	static int ToString(lua_State* L, CppObjectMetadata& self);
	static bool IsEqual(lua_State* L, CppObjectMetadata& self, int otherIndex);
	static bool IsLessThan(lua_State* L, CppObjectMetadata& self, int otherIndex);
	static bool IsLessThan(lua_State* L, int selfIndex, CppObjectMetadata& other);
	static char const* GetTypeName(lua_State* L, CppObjectMetadata& self);
};

END_NS()
