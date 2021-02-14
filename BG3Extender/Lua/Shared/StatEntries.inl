#include <GameDefinitions/Stats/Stats.h>
#include <Lua/Shared/LuaStats.h>
#include <Lua/LuaSerializers.h>

namespace bg3se::lua::stats
{
	/*int GetSkillSet(lua_State * L)
	{
		auto skillSetName = luaL_checkstring(L, 1);
		auto stats = GetStaticSymbols().GetStats();
		auto skillSet = stats->SkillSetManager->Find(skillSetName);
		return LuaWrite(L, skillSet);
	}

	int UpdateSkillSet(lua_State* L)
	{
		StackCheck _(L);
		luaL_checktype(L, 1, LUA_TTABLE);
		auto name = checked_getfield<FixedString>(L, "Name", 1);

		auto stats = GetStaticSymbols().GetStats();
		auto skillSet = stats->SkillSetManager->Find(name);
		bool isNew = (skillSet == nullptr);

		lua_pushvalue(L, 1);
		LuaRead(L, skillSet);
		lua_pop(L, 1);

		if (isNew) {
			stats->SkillSetManager->Add(name, skillSet);
		}

		return 0;
	}

	int GetEquipmentSet(lua_State * L)
	{
		auto equipmentSetName = luaL_checkstring(L, 1);
		auto stats = GetStaticSymbols().GetStats();
		auto equipmentSet = stats->EquipmentSetManager->Find(equipmentSetName);
		return LuaWrite(L, equipmentSet);
	}

	int UpdateEquipmentSet(lua_State* L)
	{
		StackCheck _(L);
		luaL_checktype(L, 1, LUA_TTABLE);
		auto name = checked_getfield<FixedString>(L, "Name", 1);

		auto stats = GetStaticSymbols().GetStats();
		auto equipmentSet = stats->EquipmentSetManager->Find(name);
		bool isNew = (equipmentSet == nullptr);

		lua_pushvalue(L, 1);
		LuaRead(L, equipmentSet);
		lua_pop(L, 1);

		if (isNew) {
			stats->EquipmentSetManager->Add(name, equipmentSet);
		}

		return 0;
	}*/

	int GetTreasureTable(lua_State* L, char const* tableName)
	{
		auto stats = GetStaticSymbols().GetStats();
		auto table = stats->TreasureTables.Find(tableName);
		return LuaWrite(L, table);
	}

	int UpdateTreasureTable(lua_State* L)
	{
		StackCheck _(L);
		luaL_checktype(L, 1, LUA_TTABLE);
		auto name = checked_getfield<FixedString>(L, "Name", 1);

		auto stats = GetStaticSymbols().GetStats();
		auto table = stats->TreasureTables.Find(name);
		bool isNew = (table == nullptr);

		lua_pushvalue(L, 1);
		LuaRead(L, *table);
		lua_pop(L, 1);

		if (isNew) {
			stats->TreasureTables.Add(name, table);
		}

		return 0;
	}

	int GetTreasureCategory(lua_State* L, char const* categoryName)
	{
		auto const* stats = GetStaticSymbols().GetStats();
		auto category = stats->TreasureCategories.Find(categoryName);
		return LuaWrite(L, *category);
	}

	int UpdateTreasureCategory(lua_State* L)
	{
		StackCheck _(L);
		auto name = checked_get<FixedString>(L, 1);
		luaL_checktype(L, 2, LUA_TTABLE);

		auto stats = GetStaticSymbols().GetStats();
		auto category = stats->TreasureCategories.Find(name);
		bool isNew = (category == nullptr);

		lua_pushvalue(L, 2);
		LuaRead(L, *category);
		lua_pop(L, 1);

		if (isNew) {
			stats->TreasureCategories.Add(name, category);
		}

		return 0;
	}

	/*int GetItemCombo(lua_State* L)
	{
		auto comboName = luaL_checkstring(L, 1);
		auto combo = GetStaticSymbols().GetStats()->ItemCombinationManager->Find(comboName);
		return LuaWrite(L, *combo);
	}

	int UpdateItemCombo(lua_State* L)
	{
		StackCheck _(L);
		luaL_checktype(L, 1, LUA_TTABLE);
		auto name = checked_getfield<FixedString>(L, "Name", 1);

		auto stats = GetStaticSymbols().GetStats();
		auto combo = stats->ItemCombinationManager->Find(name);
		bool isNew = (combo == nullptr);

		lua_pushvalue(L, 1);
		LuaRead(L, combo);
		lua_pop(L, 1);

		if (isNew) {
			stats->ItemCombinationManager->Add(name, combo);
		}

		return 0;
	}

	int GetItemComboPreviewData(lua_State* L)
	{
		auto comboName = ToFixedString(luaL_checkstring(L, 1));
		auto preview = GetStaticSymbols().GetStats()->ItemCombinationManager->PreviewData.Find(comboName);
		return LuaWrite(L, *preview);
	}

	int UpdateItemComboPreviewData(lua_State* L)
	{
		StackCheck _(L);
		luaL_checktype(L, 1, LUA_TTABLE);
		auto name = checked_getfield<FixedString>(L, "Name", 1);

		auto stats = GetStaticSymbols().GetStats();
		auto existing = stats->ItemCombinationManager->PreviewData.Find(name);
		CItemCombinationPreviewData* previewData = existing ? *existing : nullptr;
		bool isNew = (previewData == nullptr);

		lua_pushvalue(L, 1);
		LuaRead(L, previewData);
		lua_pop(L, 1);

		if (isNew) {
			stats->ItemCombinationManager->PreviewData.Insert(name, previewData);
		}

		return 0;
	}

	int GetItemComboProperty(lua_State* L)
	{
		auto propertyName = ToFixedString(luaL_checkstring(L, 1));
		auto prop = GetStaticSymbols().GetStats()->ItemCombinationManager->ComboProperties.Find(propertyName);
		return LuaWrite(L, *prop);
	}

	int UpdateItemComboProperty(lua_State* L)
	{
		StackCheck _(L);
		luaL_checktype(L, 1, LUA_TTABLE);
		auto name = checked_getfield<FixedString>(L, "Name", 1);

		auto stats = GetStaticSymbols().GetStats();
		auto existing = stats->ItemCombinationManager->ComboProperties.Find(name);
		CItemCombinationProperty* comboProperty = existing ? *existing : nullptr;
		bool isNew = (comboProperty == nullptr);

		lua_pushvalue(L, 1);
		LuaRead(L, comboProperty);
		lua_pop(L, 1);

		if (isNew) {
			stats->ItemCombinationManager->ComboProperties.Insert(name, comboProperty);
		}

		return 0;
	}


	int GetItemGroup(lua_State* L)
	{
		auto name = ToFixedString(luaL_checkstring(L, 1));
		auto group = GetStaticSymbols().GetStats()->ItemProgressionManager->ItemGroups.Find(name);
		return LuaWrite(L, *group);
	}


	int GetNameGroup(lua_State* L)
	{
		auto name = ToFixedString(luaL_checkstring(L, 1));
		auto nameGroup = GetStaticSymbols().GetStats()->ItemProgressionManager->NameGroups.Find(name);
		return LuaWrite(L, *nameGroup);
	}*/


	/*int GetDeltaMod(lua_State* L)
	{
		StackCheck _(L, 1);
		auto name = checked_get<char const *>(L, 1);
		auto modifierType = checked_get<char const*>(L, 2);

		auto stats = GetStaticSymbols().GetStats();
		if (stats == nullptr) {
			OsiError("RPGStats not available");
			push(L, nullptr);
			return 1;
		}

		auto deltaModType = stats->DeltaMods.Find(modifierType);
		if (deltaModType == nullptr) {
			OsiError("Unknown DeltaMod ModifierType: " << modifierType);
			push(L, nullptr);
			return 1;
		}

		auto deltaMod = deltaModType->Find(name);
		return LuaWrite(L, deltaMod);
	}

	int UpdateDeltaMod(lua_State* L)
	{
		StackCheck _(L, 0);
		luaL_checktype(L, 1, LUA_TTABLE);
		auto name = checked_getfield<char const*>(L, "Name", 1);
		auto modifierType = checked_getfield<char const*>(L, "ModifierType", 1);

		auto stats = GetStaticSymbols().GetStats();
		if (stats == nullptr) {
			OsiError("RPGStats not available");
			return 0;
		}

		auto deltaModType = stats->DeltaMods.Find(modifierType);
		if (deltaModType == nullptr) {
			OsiError("Unknown DeltaMod ModifierType: " << modifierType);
			return 0;
		}

		auto deltaMod = deltaModType->Find(name);
		bool isNew = (deltaMod == nullptr);

		lua_pushvalue(L, 1);
		LuaRead(L, deltaMod);
		lua_pop(L, 1);

		if (isNew) {
			deltaModType->Add(MakeFixedString(name), deltaMod);
		}

		return 0;
	}*/

	int GetStat(lua_State* L)
	{
		StackCheck _(L, 1);
		auto statName = luaL_checkstring(L, 1);
		auto statType = checked_get<FixedString>(L, 3);

		std::optional<int> level;
		if (lua_gettop(L) >= 3 && !lua_isnil(L, 3)) {
			level = (int32_t)luaL_checkinteger(L, 3);
		}

		if (statType == GFS.strCharacter
			|| statType == GFS.strArmor
			|| statType == GFS.strObject
			|| statType == GFS.strWeapon
			|| statType == GFS.strSpellData
			|| statType == GFS.strStatusData
			|| statType == GFS.strPassiveData) {
			auto object = StatFindObject(statName);
			if (object != nullptr) {
				StatsProxy::New(L, object, level);
			} else {
				push(L, nullptr);
			}

			return 1;
		} else if (statType == GFS.strTreasureTable) {
			return GetTreasureTable(L, statName);
		} else if (statType == GFS.strTreasureCategory) {
			return GetTreasureCategory(L, statName);
		} else if (statType == GFS.strSpellSet
			|| statType == GFS.strEquipmentSet
			|| statType == GFS.strItemCombination
			|| statType == GFS.strItemGroup
			|| statType == GFS.strNameGroup) {
			LuaError("Stat type '" << statType << "' not implemented yet!");
			push(L, nullptr);
			return 1;
		} else {
			LuaError("Stat type '" << statType << "' does not exist!");
			push(L, nullptr);
			return 1;
		}
	}

	bool CopyStats(CRPGStats_Object* obj, char const* copyFrom)
	{
		auto stats = GetStaticSymbols().GetStats();
		auto copyFromObject = stats->Objects.Find(FixedString(copyFrom));
		if (copyFromObject == nullptr) {
			OsiError("Cannot copy stats from nonexistent object: " << copyFrom);
			return false;
		}

		if (obj->ModifierListIndex != copyFromObject->ModifierListIndex) {
			auto objModifier = stats->ModifierLists.Find(obj->ModifierListIndex);
			auto copyModifier = stats->ModifierLists.Find(obj->ModifierListIndex);
			OsiError("Cannot copy stats from object '" << copyFrom << "' (a " << copyModifier->Name
				<< ") to an object of type " << objModifier->Name);
			return false;
		}

		obj->Level = copyFromObject->Level;
		obj->AIFlags = copyFromObject->AIFlags;

		for (size_t i = 0; i < obj->IndexedProperties.size(); i++) {
			obj->IndexedProperties[i] = copyFromObject->IndexedProperties[i];
		}

		/*copyFromObject->PropertyList.Iterate([obj](auto const& key, auto propertyList) {
			// TODO - is reusing property list objects allowed?
			obj->PropertyList.Insert(key, propertyList);
		});

		copyFromObject->ConditionList.Iterate([obj](auto const& key, auto propertyList) {
			// TODO - is reusing condition objects allowed?
			obj->ConditionList.Insert(key, propertyList);
		});

		obj->Requirements = copyFromObject->Requirements;
		obj->MemorizationRequirements = copyFromObject->MemorizationRequirements;
		obj->ComboCategories = copyFromObject->ComboCategories;*/

		return true;
	}

	int CreateStat(lua_State* L)
	{
		StackCheck _(L, 1);
		auto statName = luaL_checkstring(L, 1);
		auto modifierName = luaL_checkstring(L, 2);
		char const* copyFrom{ nullptr };
		if (lua_gettop(L) >= 3) {
			copyFrom = luaL_checkstring(L, 3);
		}

		LuaVirtualPin lua(gOsirisProxy->GetCurrentExtensionState());
		if (lua->RestrictionFlags & State::ScopeModulePreLoad) {
			return luaL_error(L, "Stat functions unavailable during module preload");
		}

		if (!(lua->RestrictionFlags & State::ScopeModuleLoad)) {
			if (gOsirisProxy->IsInServerThread()) {
				static bool syncWarningShown{ false };
				if (!syncWarningShown) {
					OsiWarn("Stats entres created after ModuleLoad must be synced manually; make sure that you call SyncStat() on it when you're finished!");
					syncWarningShown = true;
				}
			}
			else {
				OsiError("Cannot call CreateStat() on client after module load!");
				push(L, nullptr);
				return 1;
			}
		}

		auto stats = GetStaticSymbols().GetStats();
		auto object = stats->CreateObject(FixedString(statName), FixedString(modifierName));
		if (!object) {
			push(L, nullptr);
			return 1;
		}

		if (copyFrom != nullptr) {
			if (!CopyStats(*object, copyFrom)) {
				push(L, nullptr);
				return 1;
			}
		}

		StatsProxy::New(L, *object, -1);
		return 1;
	}

	void SyncStat(lua_State* L, char const* statName, std::optional<bool> persist)
	{
		auto stats = GetStaticSymbols().GetStats();
		auto object = stats->Objects.Find(FixedString(statName));
		if (!object) {
			OsiError("Cannot sync nonexistent stat: " << statName);
			return;
		}

		//stats->SyncWithPrototypeManager(object);

		if (gOsirisProxy->IsInServerThread()) {
			object->BroadcastSyncMessage(false);

			gOsirisProxy->GetServerExtensionState().MarkDynamicStat(FixedString(statName));
			if (persist && *persist) {
				gOsirisProxy->GetServerExtensionState().MarkPersistentStat(FixedString(statName));
			}
		}
	}

	WrapLuaFunction(SyncStat)

	void StatSetPersistence(lua_State* L, char const* statName, bool persist)
	{
		auto stats = GetStaticSymbols().GetStats();
		auto object = stats->Objects.Find(FixedString(statName));
		if (!object) {
			OsiError("Cannot set persistence for nonexistent stat: " << statName);
			return;
		}

		if (persist) {
			gOsirisProxy->GetServerExtensionState().MarkPersistentStat(FixedString(statName));
		} else {
			gOsirisProxy->GetServerExtensionState().UnmarkPersistentStat(FixedString(statName));
		}
	}

	WrapLuaFunction(StatSetPersistence)
}
