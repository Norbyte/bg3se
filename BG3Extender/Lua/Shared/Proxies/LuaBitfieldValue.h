#pragma once

#include <Lua/Shared/Proxies/LuaCppValue.h>

BEGIN_NS(lua)

class BitfieldValueMetatable : public LightCppValueMetatable<BitfieldValueMetatable>,
	public Indexable, public Lengthable, public Iterable, public Stringifiable, public EqualityComparable, public HasBinaryOps
{
public:
	static constexpr MetatableTag MetaTag = MetatableTag::BitfieldValue;
	static constexpr bool HasLifetime = false;

	inline static void Make(lua_State* L, EnumUnderlyingType value, int registryIndex)
	{
		lua_push_cppvalue(L, MetaTag, registryIndex, static_cast<uint64_t>(value));
	}

	template <class T>
	inline static void Make(lua_State* L, T value)
	{
		Make(L, static_cast<EnumUnderlyingType>(value), BitfieldID<T>::ID);
	}

	static BitfieldInfoStore* GetBitfieldInfo(CppValueMetadata const& val);
	static EnumUnderlyingType GetValue(CppValueMetadata const& self);
	static std::optional<EnumUnderlyingType> GetValueAtIndex(CppValueMetadata const& self, int index);
	static Json::Value ToJson(CppValueMetadata& self);
	static STDString GetValueAsString(CppValueMetadata& self);

	static int Index(lua_State* L, CppValueMetadata& self);
	static int ToString(lua_State* L, CppValueMetadata& self);
	static bool IsEqual(lua_State* L, CppValueMetadata& self, int otherIndex);
	static int Length(lua_State* L, CppValueMetadata& self);
	static int Next(lua_State* L, CppValueMetadata& self);
	static int BAnd(lua_State* L, CppValueMetadata& self, int otherIndex);
	static int BOr(lua_State* L, CppValueMetadata& self, int otherIndex);
	static int BXor(lua_State* L, CppValueMetadata& self, int otherIndex);
	static int BNot(lua_State* L, CppValueMetadata& self);
	static char const* GetTypeName(lua_State* L, CppValueMetadata& self);
};

END_NS()
