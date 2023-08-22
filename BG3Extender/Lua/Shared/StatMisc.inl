#include <stdafx.h>
#include <Extender/ScriptExtender.h>
#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <GameDefinitions/Stats/Stats.h>
#include <Lua/Shared/LuaStats.h>

namespace bg3se::lua::stats
{
	char const* const SpellPrototypeProxy::MetatableName = "eoc::SpellPrototype";

	SpellPrototypeProxy::SpellPrototypeProxy(SpellPrototype* obj, std::optional<int> level, LifetimeHandle& lifetime)
		: obj_(obj), level_(level), lifetime_(lifetime)
	{
		stats_ = obj->GetStats();
	}

	int SpellPrototypeProxy::Index(lua_State* L)
	{
		if (!lifetime_.IsAlive(L) || stats_ == nullptr) {
			return luaL_error(L, "Attempted to read property of null spell prototype");
		}

		FixedString attributeName{ luaL_checkstring(L, 2) };

		if (attributeName == GFS.strLevel) {
			push(L, obj_->Level);
			return 1;
		}

		return LuaStatGetAttribute(L, stats_, attributeName, level_);
	}


	char const* const StatsExtraDataProxy::MetatableName = "stats::ExtraData";

	int StatsExtraDataProxy::Index(lua_State* L)
	{
		auto stats = GetStaticSymbols().GetStats();
		if (stats == nullptr || stats->ExtraData == nullptr) return luaL_error(L, "Stats not available");

		auto key = luaL_checkstring(L, 2);
		auto extraData = stats->ExtraData->find(FixedString(key));
		if (extraData != stats->ExtraData->end()) {
			push(L, extraData.Value());
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
		auto value = get<float>(L, 3);
		auto extraData = stats->ExtraData->find(FixedString(key));
		if (extraData != stats->ExtraData->end()) {
			extraData.Value() = value;
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
		auto index = EnumInfo<T>::Find(FixedString(label));
		if (index) {
			push(L, *index);
		} else {
			push(L, nullptr);
		}

		return 1;
	}

#define BEGIN_BITMASK_NS(NS, T, luaName, type)
#define BEGIN_BITMASK(T, type)
#define E(label)
#define EV(label, value)
#define END_ENUM_NS()
#define END_ENUM()

// TODO - this solution has subpar performance
#define BEGIN_ENUM_NS(NS, T, luaName, type) \
	if (strcmp(enumName, #T) == 0) { \
		return EnumIndexToLabel<NS::T>(L, (NS::T)index); \
	}
#define BEGIN_ENUM(T, type) \
	if (strcmp(enumName, #T) == 0) { \
		return EnumIndexToLabel<T>(L, (T)index); \
	}

	int EnumIndexToLabel(lua_State* L)
	{
		auto enumName = get<char const*>(L, 1);
		auto index = get<int>(L, 2);

#include <GameDefinitions/Enumerations.inl>

		auto valueList = GetStaticSymbols().GetStats()->ModifierValueLists.Find(enumName);
		if (valueList) {
			std::optional<FixedString> value;
			for (auto const& kv : valueList->Values) {
				if (kv.Value  == index) {
					value = kv.Key;
				}
			}

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

	// TODO - this solution has subpar performance
#define BEGIN_ENUM_NS(NS, T, luaName, type) \
	if (strcmp(enumName, #T) == 0) { \
		return EnumLabelToIndex<NS::T>(L, label); \
	}
#define BEGIN_ENUM(T, type) \
	if (strcmp(enumName, #T) == 0) { \
		return EnumLabelToIndex<T>(L, label); \
	}


	int EnumLabelToIndex(lua_State* L)
	{
		auto enumName = get<char const*>(L, 1);
		auto label = get<char const*>(L, 2);

#include <GameDefinitions/Enumerations.inl>

		auto valueList = GetStaticSymbols().GetStats()->ModifierValueLists.Find(enumName);
		if (valueList) {
			auto value = valueList->Values.find(FixedString(label));

			if (value != valueList->Values.end()) {
				push(L, value.Value());
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
