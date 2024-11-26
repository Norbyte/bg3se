#pragma once

#include <Lua/Shared/Proxies/LuaCppValue.h>

BEGIN_NS(lua)

class BitfieldValueMetatable : public LightCppValueMetatable<BitfieldValueMetatable>,
	public Indexable, public Lengthable, public Iterable, public Stringifiable, public EqualityComparable, public HasBinaryOps
{
public:
	static constexpr MetatableTag MetaTag = MetatableTag::BitfieldValue;

	inline static void Make(lua_State* L, EnumUnderlyingType value, int registryIndex)
	{
		lua_push_cppvalue(L, MetaTag, registryIndex, static_cast<uint64_t>(value));
	}

	template <class T>
	inline static void Make(lua_State* L, T value)
	{
		Make(L, static_cast<EnumUnderlyingType>(value), BitfieldID<T>::ID);
	}

	static BitfieldInfoStore* GetBitfieldInfo(CppObjectMetadata const& val);
	static EnumUnderlyingType GetValue(CppObjectMetadata const& self);
	static std::optional<EnumUnderlyingType> GetValueAtIndex(CppObjectMetadata const& self, int index);
	static Json::Value ToJson(CppObjectMetadata& self);
	static STDString GetValueAsString(CppObjectMetadata& self);

	static int Index(lua_State* L, CppObjectMetadata& self);
	static int ToString(lua_State* L, CppObjectMetadata& self);
	static bool IsEqual(lua_State* L, CppObjectMetadata& self, int otherIndex);
	static int Length(lua_State* L, CppObjectMetadata& self);
	static int Next(lua_State* L, CppObjectMetadata& self);
	static int BAnd(lua_State* L, CppObjectMetadata& self, int otherIndex);
	static int BOr(lua_State* L, CppObjectMetadata& self, int otherIndex);
	static int BXor(lua_State* L, CppObjectMetadata& self, int otherIndex);
	static int BNot(lua_State* L, CppObjectMetadata& self);
	static char const* GetTypeName(lua_State* L, CppObjectMetadata& self);
};

END_NS()
