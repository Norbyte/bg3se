#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
{
	struct ArmorComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Armor;

		int ArmorType;
		int ArmorClass;
		int Ability_Modifier_Cap;
		char Armor_Class_Ability;
		char Shield;
	};

	struct BaseHpComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::BaseHp;

		int Vitality;
		int VitalityBoost;
	};

	struct CustomStatsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CustomStats;

		Map<FixedString, int> Stats;
	};


	struct DataComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Data;

		float Weight_M;
		FixedString StatsId;
		int StatusImmunityFlags;
		float field_24;
		int StepsType;
	};

	struct ExperienceComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Experience;

		int Level;
		int NextLevelExperience;
		int Experience_M;
		int SomeExperience;
		char field_28;
	};

	struct HealthComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Health;

		void* VMT2_M;
		__int64 field_20;
		__int64 field_28;
		__int64 field_30;
		__int64 field_38;
		__int64 field_40;
		__int64 field_48;
		int CurrentHealth;
		int MaxHealth;
		int BoostHealth_M;
		int field_5C;
		int field_60;
		int field_64;
		int field_68;
		int field_6C;
		int field_70;
		int field_74;
		int field_78;
		int field_7C;
		int field_80;
		int field_84;
		int field_88;
		int field_8C;
		int field_90;
		int field_94;
		int field_98;
		char field_9C;
	};



	struct StatsComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Stats;

		struct EquipmentEntry
		{
			int field_0;
			EntityWorldHandle Handle;
			char field_10;
			char field_11;
		};

		int Abilities[8];
		__int64 field_38;
		__int64 field_40;
		__int64 field_48;
		int Skills[18];
		uint8_t field_98;
		char field_99;
		char field_9A;
		uint8_t field_9B;
		uint8_t SpellCastingAbility;
		int field_A0;
		int field_A4;
		int AdditionalDamage_M;
		int field_AC;
		int field_B0;
		char field_B4;
		VirtualMultiHashMap<int, EquipmentEntry> Equipment;
		int ArmorType;
		int field_114;
		int Level;
		char Flanked_M;
		Array<uint64_t> Classes_M;
		uint8_t UnarmedAttackAbility;
		uint8_t SomeAttackAbility;
		__int64 field_140;
	};

	struct StatusImmunitiesComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::StatusImmunities;

		VirtualMultiHashMap<FixedString, UUID> PersonalStatusImmunities;
	};

	struct ValueComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Value;

		int Value;
		char Rarity;
		char Unique;
	};

	struct GenderComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Gender;

		uint8_t Gender;
		uint8_t Gender2;
	};

	struct WeaponComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Weapon;

		RefMap<void*, void*> field_18;
		RefMap<void*, void*> field_28;
		float WeaponRange;
		float Damage_Range;
		__int64 WeaponFunctors;
		int Weapon_Properties;
		char Weapon_Group;
		char field_4D;
	};


}
