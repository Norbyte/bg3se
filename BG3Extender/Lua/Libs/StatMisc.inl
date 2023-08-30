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


	Map<FixedString, FixedString> GetModifierAttributes(FixedString const& modifierName)
	{
		auto stats = GetStaticSymbols().GetStats();
		if (!stats) {
			OsiError("Stats not available");
			return {};
		}

		auto modifierList = stats->ModifierLists.Find(modifierName);
		if (!modifierList) {
			OsiError("No such modifier list: " << modifierName);
			return {};
		}

		Map<FixedString, FixedString> modifiers;
		for (auto const& modifier : modifierList->Attributes.Primitives) {
			auto enumeration = GetStaticSymbols().GetStats()->ModifierValueLists.Find(modifier->EnumerationIndex);
			modifiers.insert(modifier->Name, enumeration->Name);
		}

		return modifiers;
	}

}
