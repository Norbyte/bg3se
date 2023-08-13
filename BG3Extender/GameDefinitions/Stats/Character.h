#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/Stats/Common.h>

namespace bg3se
{
	struct IGameObject;

	struct CharacterEquippedItem
	{
		int32_t ItemStatsHandle;
		ItemSlot ItemSlot;
		bool IsEquipped;
		bool Unknown;
	};

	struct CDivinityStats_CharacterDynamic : public ProtectedGameObject<CDivinityStats_CharacterDynamic>
	{
		int32_t CharacterInstanceId;
		CDivinityStats* DivStats;

		int Strength;
		int Wisdom;
		int Intelligence;
		int Speed;
		int MovementSpeedBoost;
		int Dexterity;
		int Charisma;
		int Constitution;

		int Willpower;
		int Bodybuilding;

		int Sight;
		int Hearing;
		int FOV;
		int APMaximum;
		int APStart;
		int APRecovery;
		int CriticalChance;
		int Initiative;
		int Vitality;
		int VitalityBoost;
		int MagicPoints;
		int Level;
		int Gain;

		int Armor;
		int ArmorBoost;
		int ArmorBoostGrowthPerLevel;
		int DamageBoost;
		int DamageBoostGrowthPerLevel;
		int AccuracyBoost;
		int DodgeBoost;

		int LifeSteal;
		int Weight;
		int ChanceToHitBoost;
		int RangeBoost;
		int APCostBoost;
		int SPCostBoost;
		int MaxSummons;

		int Skills[18];
		int BonusWeaponDamageMultiplier;
		FixedString StatsId;
		FixedString BonusWeapon;
		void* Reflection;
		int AttributeHandle;
		int field_104;
		int64_t AttributeFlagsId;
	};

	struct CDivinityStats_Character : public CRPGStats_ObjectInstance
	{
		int32_t CurrentVitality;
		int32_t CurrentArmor;
		int32_t field_160;
		int32_t APMaximum;
		int32_t BonusActionPoints;
		int32_t Experience;
		int32_t ExperienceX;
		bool Flanked;
		uint8_t gap175[7];
		StatCharacterFlags Flags;
		int field_180;
		int field_184;
		IGameObject* Character;
		int field_190;
		int field_194;
		Vector<CDivinityStats_CharacterDynamic*> DynamicStats;
		CDivinityStats_CharacterDynamic* StatsFromStatsEntry;
		Vector<CharacterEquippedItem*> EquippedItems;
		GameRandom DamageRng;
		GameRandom CriticalHitRng;
		int32_t MaxVitality;
		int32_t BaseMaxVitality;
		int32_t MaxArmor;
		int32_t BaseMaxArmor;
		float Sight;
		float BaseSight;
		StatAttributeFlags AttributeFlags;
		StatAttributeFlags BaseAttributeFlags;
		StatAttributeFlags ItemBoostedAttributeFlags;
		bool AttributeFlagsUpdated;
		int32_t MaxSummons;
		int32_t BaseMaxSummons;
		int32_t MaxMpOverride;
	};
}
