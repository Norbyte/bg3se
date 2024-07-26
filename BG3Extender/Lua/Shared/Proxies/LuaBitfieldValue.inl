#include <Lua/Shared/Proxies/LuaBitfieldValue.h>
#include <Extender/ScriptExtender.h>
#include <json/json.h>

BEGIN_NS(lua)

BitfieldInfoStore* BitfieldValueMetatable::GetBitfieldInfo(CppValueMetadata const& self)
{
	return BitfieldRegistry::Get().BitfieldsById[self.PropertyMapTag];
}

EnumUnderlyingType BitfieldValueMetatable::GetValue(CppValueMetadata const& self)
{
	return static_cast<EnumUnderlyingType>(self.Value);
}

std::optional<EnumUnderlyingType> BitfieldValueMetatable::GetValueAtIndex(CppValueMetadata const& self, int index)
{
	auto ei = GetBitfieldInfo(self);
	auto v = self.Value & ei->AllowedFlags;
	int i = 0;
	DWORD bitIndex = 0;
	while (i <= index && _BitScanForward64(&bitIndex, v)) {
		v &= ~(1ull << bitIndex);
		i++;
	}

	if (i > index) {
		return (1ull << bitIndex);
	} else {
		return {};
	}
}

STDString BitfieldValueMetatable::GetValueAsString(CppValueMetadata& self)
{
	STDString labels;
	auto ei = GetBitfieldInfo(self);
	for (auto const& val : ei->Values) {
		if ((self.Value & val.Value) == val.Value) {
			if (!labels.empty()) labels += " | ";
			labels += val.Key.GetString();
		}
	}

	return labels;
}

Json::Value BitfieldValueMetatable::ToJson(CppValueMetadata& self)
{
	Json::Value arr(Json::arrayValue);
	auto ei = GetBitfieldInfo(self);
	for (auto const& val : ei->Values) {
		if ((self.Value & val.Value) == val.Value) {
			arr.append(val.Key.GetString());
		}
	}
	return arr;
}

int BitfieldValueMetatable::Index(lua_State* L, CppValueMetadata& self)
{
	switch (lua_type(L, 2)) {
	case LUA_TSTRING:
	{
		auto index = get<FixedString>(L, 2);
		if (index == GFS.str__Labels) {
			auto ei = GetBitfieldInfo(self);
			lua_newtable(L);
			int i = 1;
			for (auto const& val : ei->Values) {
				if ((self.Value & val.Value) == val.Value) {
					settable(L, i, val.Key, -3);
					i++;
				}
			}
		} else if (index == GFS.str__Value) {
			push(L, GetValue(self));
		} else if (index == GFS.str__EnumName) {
			push(L, GetBitfieldInfo(self)->EnumName);
		} else {
			auto ei = GetBitfieldInfo(self);
			auto val = ei->Find(index);
			if (val) {
				push(L, (self.Value & *val) == *val);
			} else {
				luaL_error(L, "Bitfield of type '%s' has no enum label or special property named '%s'", ei->EnumName.GetString(), index.GetString());
				push(L, nullptr);
			}
		}
		break;
	}

	case LUA_TNUMBER:
	{
		auto index = get<int>(L, 2);
		if (index < 1) {
			push(L, nullptr);
		} else {
			auto val = GetValueAtIndex(self, index - 1);
			if (val) {
				auto ei = GetBitfieldInfo(self);
				push(L, ei->Find(*val));
			} else {
				push(L, nullptr);
			}
		}
		break;
	}

	default:
		luaL_error(L, "Bitfield __index parameter must be an integer, enum label, or special property name; got %s", lua_typename(L, lua_type(L, 2)));
		push(L, nullptr);
		break;
	}

	return 1;
}

int BitfieldValueMetatable::ToString(lua_State* L, CppValueMetadata& self)
{
	StackCheck _(L, 1);
	STDString labels;
	auto ei = GetBitfieldInfo(self);
	for (auto const& val : ei->Values) {
		if ((self.Value & val.Value) == val.Value) {
			if (!labels.empty()) labels += ',';
			labels += val.Key.GetString();
		}
	}

	if (self.Value & ~ei->AllowedFlags) {
		if (!labels.empty()) labels += ',';
		labels += "Unknowns:";
		labels += std::to_string(self.Value & ~ei->AllowedFlags);
	}

	push(L, STDString(ei->EnumName.GetString()) + "(" + labels + ")");
	return 1;
}

bool BitfieldValueMetatable::IsEqual(lua_State* L, CppValueMetadata& self, int otherIndex)
{
	auto ei = GetBitfieldInfo(self);
	auto other = try_get_bitfield_value(L, otherIndex, self.PropertyMapTag, false);
	return other && *other == self.Value;
}

int BitfieldValueMetatable::Length(lua_State* L, CppValueMetadata& self)
{
	auto ei = GetBitfieldInfo(self);
	auto len = (unsigned)_mm_popcnt_u64(self.Value & ei->AllowedFlags);
	push(L, len);
	return 1;
}

int BitfieldValueMetatable::Next(lua_State* L, CppValueMetadata& self)
{
	int key;
	if (lua_type(L, 2) == LUA_TNIL) {
		key = 0;
	} else {
		key = get<int>(L, 2);
	}

	if (key >= 0) {
		auto val = GetValueAtIndex(self, key);
		if (val) {
			auto ei = GetBitfieldInfo(self);
			push(L, ++key);
			push(L, ei->Find(*val));
			return 2;
		}
	}

	return 0;
}

int BitfieldValueMetatable::BAnd(lua_State* L, CppValueMetadata& self, int otherIndex)
{
	auto other = get_bitfield_value(L, otherIndex, self.PropertyMapTag, true);
	Make(L, self.Value & other, self.PropertyMapTag);
	return 1;
}

int BitfieldValueMetatable::BOr(lua_State* L, CppValueMetadata& self, int otherIndex)
{
	auto other = get_bitfield_value(L, otherIndex, self.PropertyMapTag, false);
	Make(L, self.Value | other, self.PropertyMapTag);
	return 1;
}

int BitfieldValueMetatable::BXor(lua_State* L, CppValueMetadata& self, int otherIndex)
{
	auto other = get_bitfield_value(L, otherIndex, self.PropertyMapTag, false);
	Make(L, self.Value ^ other, self.PropertyMapTag);
	return 1;
}

int BitfieldValueMetatable::BNot(lua_State* L, CppValueMetadata& self)
{
	auto ei = GetBitfieldInfo(self);
	Make(L, (~self.Value & ei->AllowedFlags), self.PropertyMapTag);
	return 1;
}

char const* BitfieldValueMetatable::GetTypeName(lua_State* L, CppValueMetadata& self)
{
	auto ei = GetBitfieldInfo(self);
	return ei->LuaName.GetString();
}

EnumUnderlyingType get_bitfield_value(lua_State* L, int index, BitfieldTypeId typeId, bool maskInvalidBits)
{
	auto const& store = *BitfieldRegistry::Get().BitfieldsById[typeId];

	switch (lua_type(L, index)) {
	case LUA_TSTRING:
	{
		auto val = do_get(L, index, Overload<FixedString>{});
		auto valueIndex = store.Find(val);
		if (valueIndex) {
			return *valueIndex;
		} else {
			luaL_error(L, "Param %d: not a valid '%s' bitfield value: %s", index, store.EnumName.GetString(), val.GetString());
		}
		break;
	}

	case LUA_TNUMBER:
	{
		auto val = (EnumUnderlyingType)lua_tointeger(L, index);
		if ((val & ~store.AllowedFlags) == 0) {
			return val;
		} else if (maskInvalidBits) {
			return (val & store.AllowedFlags);
		} else {
			luaL_error(L, "Param %d: not a valid '%s' bitfield value: %p", index, store.EnumName.GetString(), val);
		}
		break;
	}

	case LUA_TTABLE:
	{
		EnumUnderlyingType val{ 0 };
		for (auto valueIdx : iterate(L, index)) {
			auto label = do_get(L, valueIdx, Overload<FixedString>{});
			auto index = store.Find(label);
			if (index) {
				val |= *index;
			} else {
				luaL_error(L, "Param %d: not a valid '%s' bitfield value: %s", 
					index, store.EnumName.GetString(), label.GetString());
			}
		}

		return val;
	}

	case LUA_TLIGHTCPPOBJECT:
	{
		CppValueMetadata meta;
		lua_get_cppvalue(L, lua_absindex(L, index), meta);
		if (meta.MetatableTag == BitfieldValueMetatable::MetaTag && meta.PropertyMapTag == (unsigned)store.RegistryIndex) {
			return static_cast<EnumUnderlyingType>(meta.Value);
		} else {
			luaL_error(L, "Param %d: expected a '%s' bitfield value, got type %d", index, store.EnumName.GetString(), meta.MetatableTag);
		}
		break;
	}

	default:
		luaL_error(L, "Param %d: expected integer, string or table '%s' bitfield value, got %s", index,
			store.EnumName.GetString(), lua_typename(L, lua_type(L, index)));
		break;
	}

	return 0;
}

std::optional<EnumUnderlyingType> try_get_bitfield_value(lua_State* L, int index, BitfieldTypeId typeId, bool maskInvalidBits)
{
	auto const& store = *BitfieldRegistry::Get().BitfieldsById[typeId];

	switch (lua_type(L, index)) {
	case LUA_TSTRING:
	{
		auto val = do_get(L, index, Overload<FixedString>{});
		auto valueIndex = store.Find(val);
		if (valueIndex) {
			return *valueIndex;
		}
		break;
	}

	case LUA_TNUMBER:
	{
		auto val = (EnumUnderlyingType)lua_tointeger(L, index);
		if ((val & ~store.AllowedFlags) == 0) {
			return val;
		} else if (maskInvalidBits) {
			return (val & store.AllowedFlags);
		}
		break;
	}

	case LUA_TTABLE:
	{
		EnumUnderlyingType val{ 0 };
		for (auto valueIdx : iterate(L, index)) {
			auto label = do_get(L, valueIdx, Overload<FixedString>{});
			auto index = store.Find(label);
			if (index) {
				val |= *index;
			} else {
				return {};
			}
		}

		return val;
	}

	case LUA_TLIGHTCPPOBJECT:
	{
		CppValueMetadata meta;
		lua_get_cppvalue(L, lua_absindex(L, index), meta);
		if (meta.MetatableTag == BitfieldValueMetatable::MetaTag && meta.PropertyMapTag == (unsigned)store.RegistryIndex) {
			return static_cast<EnumUnderlyingType>(meta.Value);
		}
		break;
	}
	}

	return {};
}

void push_bitfield_value(lua_State* L, EnumUnderlyingType value, BitfieldTypeId id)
{
	BitfieldValueMetatable::Make(L, value, id);
}

END_NS()
