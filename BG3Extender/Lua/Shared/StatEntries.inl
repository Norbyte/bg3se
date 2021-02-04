#include <GameDefinitions/Stats/Stats.h>
#include <Lua/LuaSerializers.h>

namespace bg3se::lua
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
	}

	int GetTreasureTable(lua_State* L)
	{
		auto tableName = luaL_checkstring(L, 1);
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
		LuaRead(L, table);
		lua_pop(L, 1);

		if (isNew) {
			stats->TreasureTables.Add(name, table);
		}

		return 0;
	}

	int GetTreasureCategory(lua_State* L)
	{
		auto categoryName = luaL_checkstring(L, 1);
		auto const* stats = GetStaticSymbols().GetStats();
		auto category = stats->TreasureCategories.Find(categoryName);
		return LuaWrite(L, category);
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
		LuaRead(L, category);
		lua_pop(L, 1);

		if (isNew) {
			stats->TreasureCategories.Add(name, category);
		}

		return 0;
	}*/

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
}
