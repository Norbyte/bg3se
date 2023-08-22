#include <GameDefinitions/Stats/Stats.h>
#include <Lua/LuaBinding.h>

namespace bg3se::lua
{
	using namespace bg3se::stats;

	/*void FetchSkillSetEntries(lua_State * L, RPGStats * stats)
	{
		int32_t index = 1;
		for (auto skillSet : stats->SkillSetManager->Primitives) {
			settable(L, index++, skillSet->Name);
		}
	}

	void FetchItemGroupEntries(lua_State* L, RPGStats* stats)
	{
		int32_t index = 1;
		stats->ItemProgressionManager->ItemGroups.Iterate([L, &index](FixedString const& key, CItemGroup*) {
			settable(L, index++, key);
		});
	}

	void FetchItemNameGroupEntries(lua_State* L, RPGStats* stats)
	{
		int32_t index = 1;
		stats->ItemProgressionManager->NameGroups.Iterate([L, &index](FixedString const& key, CNameGroup*) {
			settable(L, index++, key);
		});
	}*/

	/*void FetchEquipmentSetEntries(lua_State * L, RPGStats * stats)
	{
		int32_t index = 1;
		for (auto equipmentSet : stats->EquipmentSetManager->Primitives) {
			settable(L, index++, equipmentSet->Name);
		}
	}*/

	void FetchTreasureTableEntries(lua_State* L, RPGStats* stats)
	{
		int32_t index = 1;
		for (auto treasureTable : stats->TreasureTables.Primitives) {
			settable(L, index++, treasureTable->Name);
		}
	}

	void FetchTreasureCategoryEntries(lua_State* L, RPGStats* stats)
	{
		int32_t index = 1;
		for (auto treasureCategory : stats->TreasureCategories.Primitives) {
			settable(L, index++, treasureCategory->Category);
		}
	}

	void FetchStatEntries(lua_State* L, RPGStats* stats, FixedString const& statType)
	{
		ModifierList* modifierList{ nullptr };
		if (statType) {
			modifierList = stats->ModifierLists.Find(statType);
			if (modifierList == nullptr) {
				OsiError("Unknown stats entry type: " << statType);
				return;
			}
		}

		int32_t index = 1;
		for (auto object : stats->Objects.Primitives) {
			if (statType) {
				auto type = stats->GetTypeInfo(object);
				if (modifierList != nullptr && type != modifierList) {
					continue;
				}
			}

			settable(L, index++, object->Name);
		}
	}

	void FetchStatEntriesBefore(lua_State* L, RPGStats* stats, FixedString const& modId, FixedString const& statType)
	{
		ModifierList* modifierList{ nullptr };
		if (statType) {
			modifierList = stats->ModifierLists.Find(statType);
			if (modifierList == nullptr) {
				OsiError("Unknown stats entry type: " << statType);
				return;
			}
		}

		auto entries = gExtender->GetStatLoadOrderHelper().GetStatsLoadedBefore(modId);

		int32_t index = 1;
		for (auto object : entries) {
			if (statType) {
				auto type = stats->GetTypeInfo(object);
				if (modifierList != nullptr && type != modifierList) {
					continue;
				}
			}

			settable(L, index++, object->Name);
		}
	}

	int GetAllStats(lua_State* L)
	{
		StackCheck _(L, 1);
		FixedString statType;
		if (lua_gettop(L) >= 1 && !lua_isnil(L, 1)) {
			statType = get<FixedString>(L, 1);
		}

		lua_newtable(L);
		auto stats = GetStaticSymbols().GetStats();
		if (stats == nullptr) {
			OsiError("RPGStats not available");
			return 1;
		}

		/*if (statType == GFS.strSkillSet) {
			FetchSkillSetEntries(L, stats);
		} else if (statType == GFS.strEquipmentSet) {
			FetchEquipmentSetEntries(L, stats);
		} else*/ if (statType == GFS.strTreasureTable) {
			FetchTreasureTableEntries(L, stats);
		}
		else if (statType == GFS.strTreasureCategory) {
			FetchTreasureCategoryEntries(L, stats);
		}
		else /*if (statType == GFS.strItemGroup) {
			FetchItemGroupEntries(L, stats);
		} else if (statType == GFS.strNameGroup) {
			FetchItemNameGroupEntries(L, stats);
		} else*/ {
			FetchStatEntries(L, stats, statType);
		}

		return 1;
	}

	int GetStatsLoadedBefore(lua_State* L)
	{
		StackCheck _(L, 1);
		auto modId = get<FixedString>(L, 1);
		FixedString statType;
		if (lua_gettop(L) >= 2 && !lua_isnil(L, 2)) {
			statType = get<FixedString>(L, 2);
		}

		lua_newtable(L);
		auto stats = GetStaticSymbols().GetStats();
		if (stats == nullptr) {
			OsiError("RPGStats not available");
			return 1;
		}

		FetchStatEntriesBefore(L, stats, modId, statType);

		return 1;
	}
}
