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
	}*/

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Requirement& v)
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
				auto param = getfield<UUID>(s.L, "Param");
				s.VisitProperty("Param", v.TagParam);
				v.IntParam = -1;
			} else {
				s.VisitProperty("Param", v.IntParam);
				v.TagParam = UUID{};
			}
		}

		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Object::StatsFunctorInfo& v)
	{
		s.BeginObject();
		P(Name);
		P(Functor);
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CRPGStats_Object::RollConditionInfo& v)
	{
		s.BeginObject();
		P(Name);
		auto stats = GetStaticSymbols().GetStats();
		if (s.IsWriting) {
			auto conditions = stats->GetConditions(v.ConditionsId);
			if (conditions) {
				s.VisitProperty("Conditions", *conditions);
			} else {
				STDString conditionsStr;
				s.VisitProperty("Conditions", conditionsStr);
			}
		} else {
			int conditionsId{ -1 };
			auto conditions = stats->GetOrCreateConditions(conditionsId);
			s.VisitProperty("Conditions", *conditions);
			v.ConditionsId = conditionsId;
		}
		s.EndObject();
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, StatsFunctorBase& v)
	{
		static long gIndex{ 0 };
		auto stats = GetStaticSymbols().GetStats();

		s.VisitProperty("Type", v.TypeId);
		P(PropertyContext);
		
		if (s.IsWriting) {
			auto conditions = stats->GetConditions(v.StatsConditionsId);
			if (conditions) {
				setfield(s.L, "Condition", **conditions);
			}
		} else {
			STDString name = std::to_string(gIndex++).c_str();
			v.UniqueName = FixedString(name.c_str());

			auto conditions = getfield<char const*>(s.L, "Condition");
			if (conditions && *conditions) {
				int conditionsId{ -1 };
				auto cond = stats->GetOrCreateConditions(conditionsId);
				*cond = conditions;
			} else {
				v.StatsConditionsId = -1;
			}
		}

		PO(IsSelf, false);
		PO(StoryActionId, 0);

		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CustomDescriptionFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(Description);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, ResurrectFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		PO(Probability, 1.0f);
		PO(HealthPercentage, 1.0f);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, SabotageFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		PO(Amount, 1);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, SummonFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		PO(MovingObject, GFS.strEmpty);
		PO(field_24, GFS.strEmpty);
		PO(SpawnLifetime, 6.0f);
		PO(StatusesToApply, ObjectSet<FixedString>{});
		PO(field_48, GFS.strEmpty);
		// LuaSerializeStatsEnum(s, "StatusHealType", GFS.strStatusHealType, v.StatusHealType);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, ForceFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		PO(Arg0, 8.0f);
		PO(Origin, ForceFunctorOrigin::OriginToEntity);
		PO(Aggression, ForceFunctorAggression::Aggressive);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, DouseFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		PO(field_20, -1.0f);
		PO(field_24, 1.0f);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, SwapPlacesFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		PO(Arg0, GFS.strEmpty);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, EqualizeFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		PO(HealType, StatusHealType::None);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, PickupFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		PO(Arg0, GFS.strEmpty);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CreateSurfaceFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(SurfaceType);
		PO(Radius, -1.0f);
		PO(Duration, -1.0f);
		PO(Arg4, 0.0f);
		PO(IsControlledByConcentration, true);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CreateConeSurfaceFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(Arg2);
		PO(Arg0, -1.0f);
		PO(Arg1, -1.0f);
		PO(Arg3, 1.0f);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, RemoveStatusFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(StatusId);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, ExecuteWeaponFunctorsFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(Type);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, TeleportSourceFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, SetStatusDurationFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(StatusId);
		P(Duration);
		PO(SetIfLonger, false);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, UseAttackFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		PO(IgnoreChecks, false);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, BreakConcentrationFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, RestoreResourceFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(ActionResourceUUID);
		P(Hex);
		P(field_34);
		// FIXME - P(LuaAmount);
		P(Amount);
		P(IsPercentage);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, SpawnFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(TemplateId);
		PO(Arg1, GFS.strEmpty);
		PO(StatusesToApply, ObjectSet<FixedString>{});
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, StabilizeFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, UnlockFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, ResetCombatTurnFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, RemoveAuraByChildStatusFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, ApplyStatusFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(StatusId);
		PO(StringParam, GFS.strEmpty);
		// FIXME - add conditions parsing!
		PO(StatsConditions, STDString{});
		PO(StatsConditionsId, -1);
		PO(Duration, 6.0f);
		PO(Param1, -1);
		PO(Param2, -1);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, DealDamageFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(DamageType);
		P(WeaponType);
		P(WeaponDamageType);
		// FIXME - PO(Damage);
		PO(Nonlethal, false);
		PO(Magical, false);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, UseActionResourceFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(ActionResourceUUID);
		P(ResourceIndex);
		P(Amount);
		P(IsPercentage);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, CreateExplosionFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(SpellId);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, SurfaceChangeFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		PO(Chance, 1.0f);
		PO(field_24, 0.0f);
		PO(field_28, 0.0f);
		PO(field_2C, -1.0f);
		P(SurfaceChange);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, ApplyEquipmentStatusFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(EquipmentSlot);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, RegainHitPointsFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		// FIXME - P(HitPoints);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, UseSpellFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		P(SpellId);
		PO(IgnoreHasSpell, false);
		PO(IgnoreChecks, false);
		PO(Arg3, false);
		return s;
	}

	LuaSerializer& operator << (LuaSerializer& s, ExtenderFunctor& v)
	{
		s << static_cast<StatsFunctorBase&>(v);
		return s;
	}

	void SerializeObjectProperty(LuaSerializer& s, StatsFunctorBase*& v)
	{
		s.BeginObject();
		if (s.IsWriting) {
			if (!v) {
				lua_pushnil(s.L);
			} else {
				#define V(cls) case cls::FunctorId: \
					s << *static_cast<cls*>(v); \
					break;

				switch (v->TypeId) {
					V(CustomDescriptionFunctor)
					V(ResurrectFunctor)
					V(SabotageFunctor)
					V(SummonFunctor)
					V(ForceFunctor)
					V(DouseFunctor)
					V(SwapPlacesFunctor)
					V(EqualizeFunctor)
					V(PickupFunctor)
					V(CreateSurfaceFunctor)
					V(CreateConeSurfaceFunctor)
					V(RemoveStatusFunctor)
					V(ExecuteWeaponFunctorsFunctor)
					V(TeleportSourceFunctor)
					V(SetStatusDurationFunctor)
					V(UseAttackFunctor)
					V(BreakConcentrationFunctor)
					V(RestoreResourceFunctor)
					V(SpawnFunctor)
					V(StabilizeFunctor)
					V(UnlockFunctor)
					V(ResetCombatTurnFunctor)
					V(RemoveAuraByChildStatusFunctor)
					V(ApplyStatusFunctor)
					V(DealDamageFunctor)
					V(UseActionResourceFunctor)
					V(CreateExplosionFunctor)
					V(SurfaceChangeFunctor)
					V(ApplyEquipmentStatusFunctor)
					V(RegainHitPointsFunctor)
					V(UseSpellFunctor)
					V(ExtenderFunctor)

					default:
						ERR("Unable to serialize unknown stats functor type %d to Lua!", v->TypeId);
				}
				#undef V
			}
		} else {
			if (v == nullptr) {
				StatsFunctorActionId type;
				s.VisitProperty("Type", type);
				v = GetStaticSymbols().GetStats()->ConstructFunctor(type);
			}

			if (v) {
				#define V(cls) case cls::FunctorId: \
					s << *static_cast<cls*>(v); \
					break;

				switch (v->TypeId) {
					V(CustomDescriptionFunctor)
					V(ResurrectFunctor)
					V(SabotageFunctor)
					V(SummonFunctor)
					V(ForceFunctor)
					V(DouseFunctor)
					V(SwapPlacesFunctor)
					V(EqualizeFunctor)
					V(PickupFunctor)
					V(CreateSurfaceFunctor)
					V(CreateConeSurfaceFunctor)
					V(RemoveStatusFunctor)
					V(ExecuteWeaponFunctorsFunctor)
					V(TeleportSourceFunctor)
					V(SetStatusDurationFunctor)
					V(UseAttackFunctor)
					V(BreakConcentrationFunctor)
					V(RestoreResourceFunctor)
					V(SpawnFunctor)
					V(StabilizeFunctor)
					V(UnlockFunctor)
					V(ResetCombatTurnFunctor)
					V(RemoveAuraByChildStatusFunctor)
					V(ApplyStatusFunctor)
					V(DealDamageFunctor)
					V(UseActionResourceFunctor)
					V(CreateExplosionFunctor)
					V(SurfaceChangeFunctor)
					V(ApplyEquipmentStatusFunctor)
					V(RegainHitPointsFunctor)
					V(UseSpellFunctor)
					V(ExtenderFunctor)

					default:
						ERR("Unable to serialize unknown object property type %d to Lua!", v->TypeId);
				}
				#undef V
			}
		}
		s.EndObject();
	}

	LuaSerializer& operator << (LuaSerializer& s, StatsFunctorSet& v)
	{
		s.BeginObject();
		if (s.IsWriting) {
			int index{ 1 };
			for (auto& functor : v.FunctorList) {
				push(s.L, index++);
				SerializeObjectProperty(s, functor);
				lua_settable(s.L, -3);
			}
		} else {
			for (auto idx : iterate(s.L, -1)) {
				StatsFunctorBase* functor{ nullptr };
				SerializeObjectProperty(s, functor);
				if (functor) {
					v.VMT->AddOrUpdate(&v, functor);
				}
			}
		}
		s.EndObject();
		return s;
	}

	/*LuaSerializer& operator << (LuaSerializer& s, SurfaceTemplate::StatusData& v)
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
