#include <stdafx.h>
#include <GameDefinitions/Symbols.h>
#include <Lua/LuaBinding.h>
#include <Lua/LuaSerializers.h>
#include <ExtensionHelpers.h>

#define P(name) s.VisitProperty(#name, v.name)
#define PO(name, default) s.VisitOptionalProperty(#name, v.name, default)

namespace bg3se::lua
{
	LuaSerializer& operator << (LuaSerializer& s, TranslatedString& v)
	{
		// TODO - is this enough?
		s << v.Handle.Handle;
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

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Treasure_Table& v)
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

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Treasure_SubTable_Description::Category& v)
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

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Treasure_SubTable_Description::DropCount& v)
	{
		s.BeginObject();
		P(Chance);
		P(Amount);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Treasure_SubTable_Description& v)
	{
		s.BeginObject();
		P(TotalCount);
		PO(StartLevel, 0);
		PO(EndLevel, 0);
		P(Categories);
		P(DropCounts);

		if (!s.IsWriting) {
			v.TotalFrequency = 0;
			v.CategoryFrequencies.Reallocate(v.Categories.Size);
			for (uint32_t i = 0; i < v.Categories.Size; i++) {
				v.CategoryFrequencies[i] = v.Categories[i]->Frequency;
				v.TotalFrequency += v.Categories[i]->Frequency;
			}

			// Allow negative TotalCount if there are no DropCounts, since a negative value indicates a "guaranteed" drop
			if (v.DropCounts.Size > 0 || v.TotalCount > 0) {
				v.TotalCount = 0;
				for (uint32_t i = 0; i < v.DropCounts.Size; i++) {
					v.TotalCount += v.DropCounts[i].Amount;
				}
			}

			v.Amounts.Reallocate(v.DropCounts.Size);
			for (uint32_t i = 0; i < v.DropCounts.Size; i++) {
				v.Amounts[i] = v.DropCounts[i].Amount;
			}
		}

		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Treasure_Category::Item& v)
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

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Treasure_Category& v)
	{
		s.BeginObject();
		P(Category);
		P(Items);
		s.EndObject();
		return s;
	}

	/*LuaSerializer& operator << (LuaSerializer& s, CItemCombination& v)
	{
		s.BeginObject();
		P(Name);
		P(RecipeCategory);
		PO(CraftingStation, CraftingStationType::None);
		PO(AutoLevel, false);
		P(Ingredients);
		P(Results);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CItemCombinationIngredient& v)
	{
		s.BeginObject();
		P(Object);
		P(IngredientType);
		PO(Transform, IngredientTransformType::None);
		PO(ItemRarity, ItemDataRarity::Sentinel);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CItemCombinationResult& v)
	{
		s.BeginObject();
		PO(Requirement, AbilityType::Sentinel);
		PO(ReqLevel, 0);
		P(PreviewStatsId);
		PO(PreviewIcon, GFS.strEmpty);
		PO(PreviewTooltip, GFS.strEmpty);
		P(Name);
		P(Results);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CItemCombinationResult::CombinationResult& v)
	{
		s.BeginObject();
		P(Result);
		PO(Boost, GFS.strEmpty);
		PO(ResultAmount, 1);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CItemCombinationPreviewData& v)
	{
		s.BeginObject();
		P(Name);
		P(Type);
		P(StatsId);
		P(Tooltip);
		P(Icon);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CItemCombinationProperty& v)
	{
		s.BeginObject();
		P(Name);
		P(PreviewIcon);
		P(PreviewTooltip);
		P(Entries);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CItemCombinationProperty::ItemCombinationPropertyEntry& v)
	{
		s.BeginObject();
		P(ObjectId);
		P(IngredientType);
		P(Result);
		s.EndObject();
		return s;
	}

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
	}

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Requirement& v)
	{
		s.BeginObject();
		s.VisitProperty("Requirement", v.RequirementId);
		s.VisitProperty("Not", v.Negate);

		if (s.IsWriting) {
			if (v.RequirementId == RequirementType::Tag) {
				s.VisitProperty("Param", v.StringParam);
			} else {
				s.VisitProperty("Param", v.IntParam);
			}
		} else {
			if (v.RequirementId == RequirementType::Tag) {
				auto param = getfield<char const*>(s.L, "Param");
				s.VisitProperty("Param", v.StringParam);
				v.IntParam = -1;
			} else {
				s.VisitProperty("Param", v.IntParam);
				v.StringParam = FixedString{};
			}
		}

		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CDivinityStats_Object_Property_Data& v)
	{
		static long gIndex{ 0 };
		auto stats = GetStaticSymbols().GetStats();

		P(Context);
		s.VisitProperty("Type", v.TypeId);
		
		if (s.IsWriting) {
			if (v.Conditions) {
				STDString name(v.Name.Str);
				if (name[name.length() - 1] == ')') {
					auto ifPos = name.find("_IF(");
					if (ifPos != std::string::npos) {
						auto condition = name.substr(ifPos + 4, name.length() - ifPos - 5);
						setfield(s.L, "Condition", condition);
					}
				}
			}
		} else {
			STDString name = std::to_string(gIndex++).c_str();

			auto conditions = getfield<char const*>(s.L, "Condition");
			if (conditions && *conditions) {
				name += "_IF(";
				name += conditions;
				name += ")";

				auto scriptCheckBlock = stats->BuildScriptCheckBlockFromProperties(conditions);
				if (scriptCheckBlock) {
					auto statConditions = GameAlloc<CDivinityStats_Condition>();
					statConditions->ScriptCheckBlock = scriptCheckBlock;
					statConditions->Name = MakeFixedString(name.c_str());
					v.Conditions = statConditions;
				}
				else {
					OsiWarn("Failed to parse conditions: " << conditions);
				}
			}

			v.Name = MakeFixedString(name.c_str());
		}

		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CDivinityStats_Object_Property_Custom& v)
	{
		s << static_cast<CDivinityStats_Object_Property_Data&>(v);
		s.VisitProperty("Action", v.Name);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CDivinityStats_Object_Property_Status& v)
	{
		s << static_cast<CDivinityStats_Object_Property_Data&>(v);
		s.VisitProperty("Action", v.Status);
		PO(StatusChance, 100.0f);
		PO(Duration, 1.0f);
		auto statsId = getfield<FixedString>(s.L, "Arg3");
		if (statsId) {
			v.StatsId = statsId;
		} else {
			PO(StatsId, GFS.strEmpty);
		}
		PO(Arg4, -1);
		PO(Arg5, -1);
		PO(SurfaceBoost, false);
		PO(SurfaceBoosts, ObjectSet<SurfaceType>{});
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CDivinityStats_Object_Property_SurfaceChange& v)
	{
		s << static_cast<CDivinityStats_Object_Property_Data&>(v);
		LuaSerializeStatsEnum(s, "Action", GFS.strSurfaceChange, v.SurfaceChange);
		PO(SurfaceChance, 1.0f);
		PO(Lifetime, 0.0f);
		PO(StatusChance, 0.0f);
		PO(Radius, -1.0f);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CDivinityStats_Object_Property_GameAction& v)
	{
		s << static_cast<CDivinityStats_Object_Property_Data&>(v);
		LuaSerializeStatsEnum(s, "Action", GFS.strGameAction, v.GameAction);
		PO(Arg1, -1.0f);
		PO(Arg2, -1.0f);
		PO(Arg3, GFS.strEmpty);
		PO(Arg4, 1.0f);
		PO(Arg5, 0.0f);
		LuaSerializeStatsEnum(s, "StatusHealType", GFS.strStatusHealType, v.StatusHealType);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CDivinityStats_Object_Property_OsirisTask& v)
	{
		s << static_cast<CDivinityStats_Object_Property_Data&>(v);
		LuaSerializeStatsEnum(s, "Action", GFS.strOsirisTask, v.OsirisTask);
		PO(Chance, 1.0f);
		PO(VitalityOnRevive, -1);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CDivinityStats_Object_Property_Sabotage& v)
	{
		s << static_cast<CDivinityStats_Object_Property_Data&>(v);
		PO(Amount, 1);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CDivinityStats_Object_Property_Summon& v)
	{
		s << static_cast<CDivinityStats_Object_Property_Data&>(v);
		P(Template);
		PO(Duration, 6.0f);
		PO(IsTotem, false);
		PO(Skill, GFS.strEmpty);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CDivinityStats_Object_Property_Force& v)
	{
		s << static_cast<CDivinityStats_Object_Property_Data&>(v);
		P(Distance);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Object_Property_CustomDescription& v)
	{
		s << static_cast<CDivinityStats_Object_Property_Data&>(v);
		s.VisitProperty("TextLine1", v.TextLine1);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Object_Property_Extender& v)
	{
		s << static_cast<CDivinityStats_Object_Property_Data&>(v);
		s.VisitProperty("Action", v.PropertyName);
		PO(Arg1, 100.0f);
		PO(Arg2, 1.0f);
		auto arg3 = getfield<FixedString>(s.L, "Arg3");
		if (arg3) {
			v.Arg3 = arg3;
		}
		else {
			PO(Arg3, GFS.strEmpty);
		}
		PO(Arg4, -1);
		PO(Arg5, -1);
		return s;
	}

	void SerializeObjectProperty(LuaSerializer& s, CDivinityStats_Object_Property_Data*& v)
	{
		s.BeginObject();
		if (s.IsWriting) {
			if (!v) {
				lua_pushnil(s.L);
			} else {
				#define V(type, cls) case CRPGStats_Object_Property_Type::type: \
					s << *static_cast<cls*>(v); \
					break;

				switch (v->TypeId) {
					V(Custom, CDivinityStats_Object_Property_Custom)
					V(Status, CDivinityStats_Object_Property_Status)
					V(SurfaceChange, CDivinityStats_Object_Property_SurfaceChange)
					V(GameAction, CDivinityStats_Object_Property_GameAction)
					V(OsirisTask, CDivinityStats_Object_Property_OsirisTask)
					V(Sabotage, CDivinityStats_Object_Property_Sabotage)
					V(Summon, CDivinityStats_Object_Property_Summon)
					V(Force, CDivinityStats_Object_Property_Force)
					V(CustomDescription, CRPGStats_Object_Property_CustomDescription)
					V(Extender, CRPGStats_Object_Property_Extender)

					default:
						ERR("Unable to serialize unknown object property type %d to Lua!", v->TypeId);
				}
				#undef V
			}
		} else {
			if (v == nullptr) {
				CRPGStats_Object_Property_Type type;
				s.VisitProperty("Type", type);
				v = GetStaticSymbols().GetStats()->ConstructProperty(type);
			}

			if (v) {
				#define V(type, cls) case CRPGStats_Object_Property_Type::type: \
					s << *static_cast<cls*>(v); \
					break;

				switch (v->TypeId) {
					V(Custom, CDivinityStats_Object_Property_Custom)
					V(Status, CDivinityStats_Object_Property_Status)
					V(SurfaceChange, CDivinityStats_Object_Property_SurfaceChange)
					V(GameAction, CDivinityStats_Object_Property_GameAction)
					V(OsirisTask, CDivinityStats_Object_Property_OsirisTask)
					V(Sabotage, CDivinityStats_Object_Property_Sabotage)
					V(Summon, CDivinityStats_Object_Property_Summon)
					V(Force, CDivinityStats_Object_Property_Force)
					V(CustomDescription, CRPGStats_Object_Property_CustomDescription)
					V(Extender, CRPGStats_Object_Property_Extender)

					default:
						ERR("Unable to serialize unknown object property type %d to Lua!", v->TypeId);
				}
				#undef V
			}
		}
		s.EndObject();
	}

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Object_Property_List& v)
	{
		s.BeginObject();
		if (s.IsWriting) {
			int index{ 1 };
			v.Properties.NameHashMap.Iterate([&s, &v, &index](auto const& k, auto& idx) {
				push(s.L, index++);
				SerializeObjectProperty(s, v.Properties.Primitives[idx]);
				lua_settable(s.L, -3);
			});
		} else {
			for (auto idx : iterate(s.L, -1)) {
				CDivinityStats_Object_Property_Data* prop{ nullptr };
				SerializeObjectProperty(s, prop);
				if (prop) {
					v.Properties.Add(prop->Name, prop);
					v.AllPropertyContexts |= prop->Context;
				}
			}
		}
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_DeltaModifier& v)
	{
		auto stats = GetStaticSymbols().GetStats();

		s.BeginObject();

		P(ModifierType);
		PO(SlotType, ItemSlot32::Sentinel);
		PO(WeaponType, WeaponType::Sentinel);
		PO(ArmorType, ArmorType::Sentinel);
		PO(Handedness, HandednessType::Any);
		P(Name);
		P(BoostType);
		PO(MinLevel, -1);
		PO(MaxLevel, -1);
		PO(Frequency, 1);

		if (s.IsWriting) {
			lua_newtable(s.L);
			int index = 1;
			for (uint32_t i = 0; i < v.BoostIndices.Size; i++) {
				auto boost = stats->objects.Find(v.BoostIndices[i]);
				if (boost != nullptr) {
					push(s.L, index++);
					lua_newtable(s.L);
					s.VisitProperty("Boost", boost->Name);
					s.VisitProperty("Count", v.BoostCounts[i]);
					lua_settable(s.L, -3);
				}
			}
			lua_setfield(s.L, -2, "Boosts");
		} else {
			v.BoostCounts.Clear();
			v.BoostIndices.Clear();
			lua_getfield(s.L, 1, "Boosts");

			luaL_checktype(s.L, -1, LUA_TTABLE);
			for (auto valueIndex : iterate(s.L, -1)) {
				FixedString boost;
				int count;
				s.VisitProperty("Boost", boost);
				s.VisitOptionalProperty("Count", count, 1);

				auto object = stats->objects.FindIndex(boost);
				if (object) {
					v.BoostIndices.Add(*object);
					v.BoostCounts.Add(count);
				} else {
					OsiError("DeltaMod references nonexistent boost '" << boost << "'");
				}
			}

			lua_pop(s.L, 1);
		}

		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, SurfaceTemplate::StatusData& v)
	{
		s.BeginObject();
		P(StatusId);
		PO(Chance, 1.0f);
		P(Duration);
		PO(RemoveStatus, false);
		PO(OnlyWhileMoving, false);
		PO(ApplyToCharacters, true);
		PO(ApplyToItems, true);
		PO(KeepAlive, true);
		PO(VanishOnReapply, false);
		PO(ForceStatus, false);
		s.EndObject();
		return s;
	}*/
}

#undef P
