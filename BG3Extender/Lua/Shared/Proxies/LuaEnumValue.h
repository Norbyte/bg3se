#pragma once

#include <Lua/Shared/Proxies/LuaCppValue.h>

BEGIN_NS(lua)

class EnumValueMetatable : public LightCppValueMetatable<EnumValueMetatable>,
	public Indexable, public Stringifiable, public EqualityComparable
{
public:
	static constexpr MetatableTag MetaTag = MetatableTag::EnumValue;
	static constexpr bool HasLifetime = false;

	inline static void Make(lua_State* L, EnumUnderlyingType value, int registryIndex)
	{
		lua_push_cppvalue(L, MetaTag, registryIndex, static_cast<uint64_t>(value));
	}

	template <class T>
	inline static void Make(lua_State* L, T value)
	{
		Make(L, static_cast<EnumUnderlyingType>(value), EnumID<T>::ID);
	}

	static EnumInfoStore* GetEnumInfo(CppValueMetadata const& val);
	static FixedString GetLabel(CppValueMetadata const& self);
	static EnumUnderlyingType GetValue(CppValueMetadata const& self);

	static int Index(lua_State* L, CppValueMetadata& self);
	static int ToString(lua_State* L, CppValueMetadata& self);
	static bool IsEqual(lua_State* L, CppValueMetadata& self, int otherIndex);
	static char const* GetTypeName(lua_State* L, CppValueMetadata& self);
};

END_NS()
