#include <stdafx.h>
#include <GameDefinitions/Symbols.h>
#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <Extender/Shared/ExtensionHelpers.h>
#include <Extender/ScriptExtender.h>

#define P(name) s.VisitProperty(#name, v.name)
#define PO(name, default) s.VisitOptionalProperty(#name, v.name, default)


namespace bg3se::lua
{
	using namespace bg3se::stats;

	LuaSerializer& operator << (LuaSerializer& s, TranslatedString& v)
	{
		//  FIXME - migrate TranslatedStrings to by-ref types later
		s << v.Handle.Handle;
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, TypeInformationRef& v)
	{
		if (s.IsWriting) {
			if (v) {
				MakeObjectRef(s.L, v.GetStatic()->Type);
			} else {
				push(s.L, nullptr);
			}
		}

		return s;
	}
	
	LuaSerializer& operator << (LuaSerializer& s, ecs::EntityRef& v)
	{
		s << v.Handle;
		if (v.World == nullptr && !s.IsWriting) {
			auto state = State::FromLua(s.L);
			v.World = state->GetEntityWorld();
		}
		return s;
	}

	/*
	LuaSerializer& operator << (LuaSerializer& s, CEquipmentSet& v)
	{
		s.BeginObject();
		P(Name);
		P(Groups);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CEquipmentGroup& v)
	{
		s.BeginObject();
		// Name property is unused
		// P(Name);
		P(Equipment);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CSkillSet& v)
	{
		s.BeginObject();
		P(Name);
		P(Skills);
		s.EndObject();
		return s;
	}*/

	LuaSerializer& operator << (LuaSerializer& s, TreasureTable& v)
	{
		s.BeginObject();
		P(Name);
		PO(MinLevel, 0);
		PO(MaxLevel, 0);
		PO(IgnoreLevelDiff, false);
		PO(UseTreasureGroupContainers, false);
		PO(CanMerge, false);
		P(SubTables);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, TreasureSubTable::Category& v)
	{
		auto stats = GetStaticSymbols().GetStats();

		s.BeginObject();
		PO(Frequency, 1);

		if (s.IsWriting) {
			if (v.IsTreasureTable) {
				auto refTable = stats->TreasureTables.Find(v.Index);
				if (refTable) {
					s.VisitProperty("TreasureTable", refTable->Name);
				}
			} else {
				auto refCategory = stats->TreasureCategories.Find(v.Index);
				if (refCategory) {
					s.VisitProperty("TreasureCategory", refCategory->Category);
				}
			}
		} else {
			FixedString treasureTable, treasureCategory;
			s.VisitOptionalProperty("TreasureTable", treasureTable, GFS.strEmpty);
			s.VisitOptionalProperty("TreasureCategory", treasureCategory, GFS.strEmpty);

			if (treasureTable && treasureTable != GFS.strEmpty) {
				auto idx = stats->TreasureTables.FindIndex(treasureTable);
				if (idx) {
					v.Index = *idx;
				} else {
					luaL_error(s.L, "Treasure table '%s' does not exist!", treasureTable.GetString());
				}

				v.IsTreasureTable = true;
				v.IsTreasureTable2 = true;
			} else {
				auto idx = stats->TreasureCategories.FindIndex(treasureCategory);
				if (idx) {
					v.Index = *idx;
				} else {
					luaL_error(s.L, "Treasure category '%s' does not exist!", treasureCategory.GetString());
				}

				v.IsTreasureTable = false;
				v.IsTreasureTable2 = false;
			}
		}

		for (int i = 0; i < 7; i++) {
			s.VisitOptionalProperty(stats->TreasureRarities[i].GetString(), v.Frequencies[i], (uint16_t)0);
		}

		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, TreasureSubTable::DropCount& v)
	{
		s.BeginObject();
		P(Chance);
		P(Amount);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, TreasureSubTable& v)
	{
		s.BeginObject();
		P(TotalCount);
		PO(StartLevel, 0);
		PO(EndLevel, 0);
		P(Categories);
		P(DropCounts);

		if (!s.IsWriting) {
			v.TotalFrequency = 0;
			v.CategoryFrequencies.Reallocate(v.Categories.Size());
			for (uint32_t i = 0; i < v.Categories.Size(); i++) {
				v.CategoryFrequencies[i] = v.Categories[i]->Frequency;
				v.TotalFrequency += v.Categories[i]->Frequency;
			}

			// Allow negative TotalCount if there are no DropCounts, since a negative value indicates a "guaranteed" drop
			if (v.DropCounts.Size() > 0 || v.TotalCount > 0) {
				v.TotalCount = 0;
				for (uint32_t i = 0; i < v.DropCounts.Size(); i++) {
					v.TotalCount += v.DropCounts[i].Amount;
				}
			}

			v.Amounts.Reallocate(v.DropCounts.Size());
			for (uint32_t i = 0; i < v.DropCounts.Size(); i++) {
				v.Amounts[i] = v.DropCounts[i].Amount;
			}
		}

		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, TreasureCategory::Item& v)
	{
		s.BeginObject();
		P(Name);
		PO(Priority, 1);
		PO(MinAmount, 1);
		PO(MaxAmount, 1);
		P(ActPart);
		PO(Unique, 0);
		PO(MinLevel, 0);
		PO(MaxLevel, 0);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, bg3se::stats::TreasureCategory& v)
	{
		s.BeginObject();
		P(Category);
		P(Items);
		s.EndObject();
		return s;
	}

	/*
	LuaSerializer& operator << (LuaSerializer& s, CItemGroup& v)
	{
		s.BeginObject();
		P(Name);
		P(LevelGroups);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CLevelGroup& v)
	{
		s.BeginObject();
		P(MinLevel);
		P(MaxLevel);
		P(Name);
		P(RootGroups);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CRootGroup& v)
	{
		s.BeginObject();
		P(MinLevel);
		P(MaxLevel);
		P(RootGroup);
		P(NameGroupLinks);
		s.VisitOptionalProperty("Unknown", v.field_10, GFS.strEmpty);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CNameGroupLink& v)
	{
		s.BeginObject();
		P(NameGroup);
		P(NoneCoolSuffix);
		P(ItemName);
		s.VisitOptionalProperty("Unknown", v.field_0, 0);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CNameGroup& v)
	{
		s.BeginObject();
		P(Name);
		P(Names);
		P(NamesCool);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CNameGroupName& v)
	{
		s.BeginObject();
		s.VisitProperty("Name", v.Name.Handle.ReferenceString);
		s.VisitProperty("Name2", v.Name2.Handle.ReferenceString);
		s.EndObject();
		return s;
	}

	void LuaSerializeStatsEnum(LuaSerializer& s, char const* key, FixedString const& enumName, int& v)
	{
		auto stats = GetStaticSymbols().GetStats();

		if (s.IsWriting) {
			push(s.L, stats->EnumIndexToLabel(enumName, v));
			lua_setfield(s.L, -2, key);
		} else {
			lua_getfield(s.L, -1, key);
			auto str = luaL_checkstring(s.L, -1);
			auto idx = stats->EnumLabelToIndex(enumName, str);
			if (idx) {
				v = *idx;
			} else {
				luaL_error(s.L, "'%s' is not a valid value for enumeration '%s'", enumName.Str, str);
			}
			lua_pop(s.L, 1);
		}
	}*/

	LuaSerializer& operator << (LuaSerializer& s, Requirement& v)
	{
		s.BeginObject();
		s.VisitProperty("Requirement", v.RequirementId);
		s.VisitProperty("Not", v.Not);

		if (s.IsWriting) {
			if (v.RequirementId == RequirementType::Tag) {
				s.VisitProperty("Param", v.TagParam);
			} else {
				s.VisitProperty("Param", v.IntParam);
			}
		} else {
			if (v.RequirementId == RequirementType::Tag) {
				auto param = checked_getfield<Guid>(s.L, "Param");
				s.VisitProperty("Param", v.TagParam);
				v.IntParam = -1;
			} else {
				s.VisitProperty("Param", v.IntParam);
				v.TagParam = Guid{};
			}
		}

		s.EndObject();
		return s;
	}
}

#undef P
