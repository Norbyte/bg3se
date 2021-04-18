#include <stdafx.h>
#include <ScriptExtender.h>
#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <GameDefinitions/Stats/Stats.h>
#include <Lua/Shared/LuaStats.h>

namespace bg3se::lua::stats
{
	int LuaStatGetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, std::optional<int> level);
	int LuaStatSetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, int valueIdx);


	char const* const SpellPrototypeProxy::MetatableName = "eoc::SpellPrototype";

	SpellPrototypeProxy::SpellPrototypeProxy(SpellPrototype* obj, std::optional<int> level, LifetimeHolder& lifetime)
		: obj_(obj), level_(level), lifetime_(lifetime)
	{
		stats_ = obj->GetStats();
	}

	int SpellPrototypeProxy::Index(lua_State* L)
	{
		if (!lifetime_.IsAlive() || stats_ == nullptr) {
			return luaL_error(L, "Attempted to read property of null spell prototype");
		}

		auto attributeName = luaL_checkstring(L, 2);

		if (strcmp(attributeName, "Level") == 0) {
			push(L, obj_->Level);
			return 1;
		}

		return LuaStatGetAttribute(L, stats_, attributeName, level_);
	}


	char const* const StatsExtraDataProxy::MetatableName = "CRPGStats_ExtraData";

	int StatsExtraDataProxy::Index(lua_State* L)
	{
		auto stats = GetStaticSymbols().GetStats();
		if (stats == nullptr || stats->ExtraData == nullptr) return luaL_error(L, "Stats not available");

		auto key = luaL_checkstring(L, 2);
		auto extraData = stats->ExtraData->Find(FixedString(key));
		if (extraData != nullptr) {
			push(L, *extraData);
		}
		else {
			push(L, nullptr);
		}

		return 1;
	}

	int StatsExtraDataProxy::NewIndex(lua_State* L)
	{
		auto stats = GetStaticSymbols().GetStats();
		if (stats == nullptr || stats->ExtraData == nullptr) return luaL_error(L, "Stats not available");

		auto key = luaL_checkstring(L, 2);
		auto value = checked_get<float>(L, 3);
		auto extraData = stats->ExtraData->Find(FixedString(key));
		if (extraData != nullptr) {
			*extraData = value;
		}
		else {
			LuaError("Cannot set nonexistent ExtraData value '" << key << "'");
		}

		return 0;
	}


	void RegisterStatsObjects(lua_State* L)
	{
		StatsExtraDataProxy::RegisterMetatable(L);
		StatsProxy::RegisterMetatable(L);
		SpellPrototypeProxy::RegisterMetatable(L);
	}

	int GetModifierAttributes(lua_State* L)
	{
		auto modifierName = checked_get<FixedString>(L, 1);
		auto stats = GetStaticSymbols().GetStats();
		if (!stats) return luaL_error(L, "Stats not available");

		auto modifierList = stats->ModifierLists.Find(modifierName);
		if (!modifierList) {
			OsiError("No such modifier list: " << modifierName);
			push(L, nullptr);
			return 1;
		}

		lua_newtable(L);
		for (auto const& modifier : modifierList->Attributes.Primitives) {
			auto enumeration = GetStaticSymbols().GetStats()->ModifierValueLists.Find(modifier->RPGEnumerationIndex);
			settable(L, modifier->Name, enumeration->Name);
		}

		return 1;
	}

	template <class T>
	int EnumIndexToLabel(lua_State* L, T index)
	{
		auto label = EnumInfo<T>::Find(index);
		if (label) {
			push(L, label);
		} else {
			push(L, nullptr);
		}

		return 1;
	}

	template <class T>
	int EnumLabelToIndex(lua_State* L, char const* label)
	{
		auto index = EnumInfo<T>::Find(label);
		if (index) {
			push(L, *index);
		} else {
			push(L, nullptr);
		}

		return 1;
	}

#define BEGIN_BITMASK_NS(NS, T, type)
#define BEGIN_BITMASK(T, type)
#define E(label)
#define EV(label, value)
#define END_ENUM_NS()
#define END_ENUM()

#define BEGIN_ENUM_NS(NS, T, type) \
	if (enumName == GFS.str##T) { \
		return EnumIndexToLabel<NS::T>(L, (NS::T)index); \
	}
#define BEGIN_ENUM(T, type) \
	if (enumName == GFS.str##T) { \
		return EnumIndexToLabel<T>(L, (T)index); \
	}

	int EnumIndexToLabel(lua_State* L)
	{
		auto enumName = checked_get<FixedString>(L, 1);
		auto index = checked_get<int>(L, 2);

#include <GameDefinitions/Enumerations.inl>

		auto valueList = GetStaticSymbols().GetStats()->ModifierValueLists.Find(enumName);
		if (valueList) {
			std::optional<FixedString> value;
			valueList->Values.Iterate([&value, index](auto const& k, auto const& idx) {
				if (idx == index) {
					value = k;
				}
			});

			if (value) {
				push(L, *value);
				return 1;
			} else {
				OsiError("Enumeration '" << enumName << "' has no label with index " << index);
				push(L, nullptr);
				return 1;
			}
		}

		OsiError("No such enumeration: " << enumName);
		push(L, nullptr);
		return 1;
	}

#undef BEGIN_ENUM_NS
#undef BEGIN_ENUM

#define BEGIN_ENUM_NS(NS, T, type) \
	if (enumName == GFS.str##T) { \
		return EnumLabelToIndex<NS::T>(L, label); \
	}
#define BEGIN_ENUM(T, type) \
	if (enumName == GFS.str##T) { \
		return EnumLabelToIndex<T>(L, label); \
	}


	int EnumLabelToIndex(lua_State* L)
	{
		auto enumName = checked_get<FixedString>(L, 1);
		auto label = checked_get<char const*>(L, 2);

#include <GameDefinitions/Enumerations.inl>

		auto valueList = GetStaticSymbols().GetStats()->ModifierValueLists.Find(enumName);
		if (valueList) {
			auto value = valueList->Values.Find(FixedString(label));

			if (value) {
				push(L, *value);
				return 1;
			} else {
				OsiError("Enumeration '" << enumName << "' has no label named '" << label << "'");
				push(L, nullptr);
				return 1;
			}
		}

		OsiError("No such enumeration: " << enumName);
		push(L, nullptr);
		return 1;
	}

#undef BEGIN_BITMASK_NS
#undef BEGIN_ENUM_NS
#undef BEGIN_BITMASK
#undef BEGIN_ENUM
#undef E
#undef EV
#undef END_ENUM_NS
#undef END_ENUM

}
