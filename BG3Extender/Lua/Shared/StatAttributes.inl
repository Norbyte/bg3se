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

	char const* const StatsProxy::MetatableName = "CRPGStats_Object";

	int StatsProxy::Index(lua_State* L)
	{
		if (!lifetime_.IsAlive() || obj_ == nullptr) {
			return luaL_error(L, "Attempted to read property of null CRPGStats_Object object");
		}

		auto attributeName = luaL_checkstring(L, 2);

		return LuaStatGetAttribute(L, obj_, attributeName, level_);
	}

	int StatsProxy::NewIndex(lua_State* L)
	{
		if (!lifetime_.IsAlive() || obj_ == nullptr) {
			return luaL_error(L, "Attempted to write property of null CRPGStats_Object object");
		}

		auto attributeName = luaL_checkstring(L, 2);
		return LuaStatSetAttribute(L, obj_, attributeName, 3);
	}


	int LuaStatGetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, std::optional<int> level)
	{
		StackCheck _(L, 1);
		auto stats = GetStaticSymbols().GetStats();

		FixedString attributeFS(attributeName);
		if (!attributeFS) {
			OsiError("Invalid stats attribute name: " << attributeName);
			push(L, nullptr);
			return 1;
		}

		if (attributeFS == GFS.strLevel) {
			push(L, object->Level);
			return 1;
		} else if (attributeFS == GFS.strName) {
			push(L, object->Name);
			return 1;
		} else if (attributeFS == GFS.strModId) {
			push(L, gExtender->GetStatLoadOrderHelper().GetStatsEntryMod(object->Name));
			return 1;
		} else if (attributeFS == GFS.strUsing) {
			if (object->Using) {
				auto parent = stats->Objects.Find(object->Using);
				if (parent != nullptr) {
					push(L, parent->Name);
					return 1;
				}
			}

			push(L, nullptr);
			return 1;
		} else if (attributeFS == GFS.strAIFlags) {
			push(L, object->AIFlags);
			return 1;
		}

		int attributeIndex{ -1 };
		auto attrInfo = object->GetAttributeInfo(attributeFS, attributeIndex);
		if (!attrInfo) {
			OsiError("Stat object '" << object->Name << "' has no attribute named '" << attributeFS << "'");
			push(L, nullptr);
			return 1;
		}

		switch (attrInfo->GetPropertyType()) {
		case RPGEnumerationType::Int:
		{
			std::optional<int> value;
			if (level) {
				if (*level == -1) {
					*level = object->Level;
				}

				value = object->GetIntScaled(attributeFS, *level);
			} else {
				value = object->GetInt(attributeFS);
			}

			LuaWrite(L, value);
			break;
		}

		case RPGEnumerationType::Int64:
		{
			auto value = object->GetInt64(attributeFS);
			LuaWrite(L, value);
			break;
		}

		case RPGEnumerationType::Float:
		{
			auto value = object->GetFloat(attributeFS);
			LuaWrite(L, value);
			break;
		}

		case RPGEnumerationType::FixedString:
		case RPGEnumerationType::Enumeration:
		case RPGEnumerationType::Conditions:
		case RPGEnumerationType::RollConditions:
		{
			auto value = object->GetString(attributeFS);
			if (value) {
				push(L, *value);
			} else {
				push(L, "");
			}
			break;
		}

		case RPGEnumerationType::GUID:
		{
			auto value = object->GetGuid(attributeFS);
			LuaWrite(L, value);
			break;
		}

		case RPGEnumerationType::Flags:
		{
			auto value = object->GetFlags(attributeFS);
			LuaWrite(L, value);
			break;
		}

		case RPGEnumerationType::Requirements:
		{
			LuaWrite(L, object->Requirements);
			break;
		}

		case RPGEnumerationType::StatsFunctors:
		{
			auto functors = object->GetStatsFunctors(attributeFS);
			if (functors && (*functors)->Size == 1 && (**functors)[0].Name == GFS.strDefault) {
				LuaWrite(L, (**functors)[0].Functor);
			} else {
				push(L, nullptr);
			}
			break;
		}

		default:
			OsiError("Don't know how to fetch values of type '" << attrInfo->Name << "'");
			push(L, nullptr);
			break;
		}

		return 1;
	}

	int StatGetAttributeScaled(lua_State* L)
	{
		auto statName = luaL_checkstring(L, 1);
		auto attributeName = luaL_checkstring(L, 2);

		auto object = StatFindObject(statName);
		if (!object) {
			push(L, nullptr);
			return 1;
		}

		return LuaStatGetAttribute(L, object, attributeName, {});
	}

	int LuaStatSetAttribute(lua_State* L, CRPGStats_Object* object, char const* attributeName, int valueIdx)
	{
		StackCheck _(L);
		LuaVirtualPin lua(gExtender->GetCurrentExtensionState());
		if (lua->RestrictionFlags & State::ScopeModulePreLoad) {
			return luaL_error(L, "Stat functions unavailable during module preload");
		}

		if (!(lua->RestrictionFlags & State::ScopeModuleLoad)) {
			static bool syncWarningShown{ false };
			if (!syncWarningShown) {
				OsiWarn("Stats edited after ModuleLoad must be synced manually; make sure that you call SyncStat() on it when you're finished!");
				syncWarningShown = true;
			}
		}

		FixedString attributeFS(attributeName);
		if (!attributeFS) {
			OsiError("Invalid stats attribute name: " << attributeName);
			return 0;
		}

		if (attributeFS == GFS.strLevel) {
			object->Level = (int32_t)luaL_checkinteger(L, valueIdx);
			return 0;
		} else if (attributeFS == GFS.strAIFlags) {
			object->AIFlags = FixedString(lua_tostring(L, valueIdx));
			return 0;
		}

		auto stats = GetStaticSymbols().GetStats();
		
		int index;
		auto attrInfo = object->GetAttributeInfo(attributeFS, index);
		if (!attrInfo) {
			LuaError("Object '" << object->Name << "' has no attribute named '" << attributeName << "'");
			return 0;
		}

		auto attrType = attrInfo->GetPropertyType();

		switch (lua_type(L, valueIdx)) {
		case LUA_TSTRING:
		{
			auto value = luaL_checkstring(L, valueIdx);
			object->SetString(attributeFS, value);
			break;
		}

		case LUA_TNUMBER:
		{
			switch (attrType) {
			case RPGEnumerationType::Int64:
				object->SetInt64(attributeFS, (int64_t)luaL_checkinteger(L, valueIdx));
				break;

			case RPGEnumerationType::Float:
				object->SetFloat(attributeFS, (float)luaL_checknumber(L, valueIdx));
				break;

			default:
				object->SetInt(attributeFS, (int32_t)luaL_checkinteger(L, valueIdx));
				break;
			}
			break;
		}

		case LUA_TTABLE:
		{
			switch (attrType) {
			case RPGEnumerationType::Flags:
			{
				ObjectSet<STDString> flags;
				lua_pushvalue(L, valueIdx);
				LuaRead(L, flags);
				lua_pop(L, 1);
				object->SetFlags(attributeFS, flags);
				break;
			}

			case RPGEnumerationType::StatsFunctors:
			{
				StatsFunctorSet* functor = stats->ConstructFunctorSet(attributeFS);
				lua_pushvalue(L, valueIdx);
				LuaRead(L, functor);
				lua_pop(L, 1);

				Array<CRPGStats_Object::StatsFunctorInfo> functors;
				if (functor) {
					CRPGStats_Object::StatsFunctorInfo functorInfo;
					functorInfo.Name = GFS.strDefault;
					functorInfo.Functor = functor;
					functors.Add(functorInfo);
				}

				object->SetStatsFunctors(attributeFS, functors);
				break;
			}

			case RPGEnumerationType::Requirements:
			{
				ObjectSet<CRPGStats_Requirement> requirements;
				lua_pushvalue(L, valueIdx);
				LuaRead(L, requirements);
				lua_pop(L, 1);
				object->Requirements = requirements;
				break;
			}

			default:
				LuaError("Cannot use table value for stat properties of type " << (unsigned)attrType << "!");
				break;
			}
			break;
		}

		case LUA_TNIL:
		{
			switch (attrType) {
			case RPGEnumerationType::Float:
				object->SetFloat(attributeFS, {});
				break;

			case RPGEnumerationType::GUID:
				object->SetGuid(attributeFS, {});
				break;

			case RPGEnumerationType::StatsFunctors:
				object->SetStatsFunctors(attributeFS, {});
				break;

			default:
				LuaError("Cannot use nil value for stat properties of type " << (unsigned)attrType << "!");
				break;
			}
			break;
		}

		default:
			LuaError("Lua property values of type '" << lua_typename(L, lua_type(L, valueIdx)) << "' are not supported");
			break;
		}

		return 0;
	}

	struct CRPGStats_CustomLevelMap : public CRPGStats_LevelMap
	{
		RegistryEntry Function;
		CRPGStats_LevelMap* OriginalLevelMap{ nullptr };

		CRPGStats_CustomLevelMap() {}
		~CRPGStats_CustomLevelMap() override {}

		void SetModifierList(int modifierListIndex, int modifierIndex) override
		{
			OsiError("Not supported!");
		}

		void SetModifierList(FixedString const& modifierListName, FixedString const& modifierName) override
		{
			OsiError("Not supported!");
		}

		int64_t GetScaledValue(int attributeValue, int level) override
		{
			auto value = LuaGetScaledValue(attributeValue, level);
			if (value) {
				return *value;
			}
			else {
				return OriginalLevelMap->GetScaledValue(attributeValue, level);
			}
		}

		std::optional<int64_t> LuaGetScaledValue(int attributeValue, int level)
		{
			// GetScaledValue must always use the client pin, as the override function is
			// reigstered from the client state
			ecl::LuaClientPin pin(ecl::ExtensionState::Get());
			if (!pin) return {};

			Restriction restriction(*pin, State::RestrictAll);

			auto L = pin->GetState();
			Function.Push();

			push(L, attributeValue);
			push(L, level);

			if (lua_pcall(L, 2, 1, 0) != 0) { // stack: retval
				OsiError("Level scaled value fetch failed: " << lua_tostring(L, -1));
				lua_pop(L, 1);
				return {};
			}

			if (lua_type(L, -1) != LUA_TNUMBER) {
				OsiErrorS("Level scaled fetcher returned non-numeric value");
				return {};
			}

			auto value = lua_tointeger(L, -1);
			lua_pop(L, 1); // stack: -
			return value;
		}
	};

	void RestoreLevelMaps(std::unordered_set<int32_t> const& levelMapIds)
	{
		auto& levelMaps = GetStaticSymbols().GetStats()->LevelMaps.Primitives;
		for (auto levelMapIndex : levelMapIds) {
			auto levelMap = static_cast<CRPGStats_CustomLevelMap*>(levelMaps.Buf[levelMapIndex]);
			levelMaps.Buf[levelMapIndex] = levelMap->OriginalLevelMap;
		}

		if (!levelMapIds.empty()) {
			OsiWarn("Restored " << levelMapIds.size() << " level map overrides (Lua VM deleted)");
		}
	}

	int StatSetLevelScaling(lua_State* L)
	{
		StackCheck _(L);
		auto modifierListName = luaL_checkstring(L, 1);
		auto modifierName = luaL_checkstring(L, 2);
		luaL_checktype(L, 3, LUA_TFUNCTION);

		LuaVirtualPin lua(gExtender->GetCurrentExtensionState());
		if (lua->RestrictionFlags & State::ScopeModulePreLoad) {
			return luaL_error(L, "Stat functions unavailable during module preload");
		}

		if (!(lua->RestrictionFlags & (State::ScopeModuleLoad | State::ScopeModuleResume))) {
			return luaL_error(L, "StatSetLevelScaling() can only be called during module load/resume");
		}

		auto stats = GetStaticSymbols().GetStats();
		auto modifier = stats->GetModifierInfo(FixedString(modifierListName), FixedString(modifierName));
		if (modifier == nullptr) {
			OsiError("Modifier list '" << modifierListName << "' or modifier '" << modifierName << "' does not exist!");
			return 0;
		}

		if (modifier->LevelMapIndex == -1) {
			OsiError("Modifier list '" << modifierListName << "', modifier '" << modifierName << "' is not level scaled!");
			return 0;
		}

		CRPGStats_LevelMap* originalLevelMap;
		auto currentLevelMap = stats->LevelMaps.Find(modifier->LevelMapIndex);

		auto it = lua->OverriddenLevelMaps.find(modifier->LevelMapIndex);
		if (it != lua->OverriddenLevelMaps.end()) {
			auto overridden = static_cast<CRPGStats_CustomLevelMap*>(currentLevelMap);
			originalLevelMap = overridden->OriginalLevelMap;
		}
		else {
			originalLevelMap = currentLevelMap;
		}

		auto levelMap = GameAlloc<CRPGStats_CustomLevelMap>();
		levelMap->ModifierListIndex = originalLevelMap->ModifierListIndex;
		levelMap->ModifierIndex = originalLevelMap->ModifierIndex;
		levelMap->RPGEnumerationIndex = originalLevelMap->RPGEnumerationIndex;
		levelMap->Name = originalLevelMap->Name;
		levelMap->Function = RegistryEntry(L, 3);
		levelMap->OriginalLevelMap = originalLevelMap;

		stats->LevelMaps.Primitives.Buf[modifier->LevelMapIndex] = levelMap;
		lua->OverriddenLevelMaps.insert(modifier->LevelMapIndex);

		return 0;
	}

}
