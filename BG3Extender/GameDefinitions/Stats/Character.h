#pragma once

#include <GameDefinitions/BaseTypes.h>
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

		/*std::optional<int32_t> GetStat(FixedString const& name, bool excludeBoosts);
		std::optional<int32_t> GetHitChance(CDivinityStats_Character* target);
		bool HasTalent(TalentType talent, bool excludeBoosts);
		int32_t GetAbility(AbilityType ability, bool excludeBoosts);
		CDivinityStats_Item* GetItemBySlot(ItemSlot slot, bool mustBeEquipped);
		CDivinityStats_Item* GetMainWeapon();
		CDivinityStats_Item* GetOffHandWeapon();
		int32_t GetPhysicalResistance(bool excludeBoosts);
		int32_t GetPiercingResistance(bool excludeBoosts);
		int32_t GetMagicResistance(bool excludeBoosts);
		int32_t GetCorrosiveResistance(bool excludeBoosts);
		int32_t GetDamageBoost();
		bool IsBoostActive(uint32_t conditionsMask);*/
	};

	/*
typedef int32_t (CDivinityStats_Character__GetStat)(CDivinityStats_Character * self, bool baseStats);
typedef int32_t (CDivinityStats_Character__GetStatNoBoosts)(CDivinityStats_Character * self);
typedef int32_t (CDivinityStats_Character__GetStatWithBoosts)(CDivinityStats_Character * self, bool baseStats, bool excludeBoosts);
typedef int32_t (CDivinityStats_Character__GetStatWithInit)(CDivinityStats_Character * self, bool baseStats, int32_t initialValue);
typedef float (CDivinityStats_Character__GetStatHearing)(CDivinityStats_Character * self, CDivinityStats_Character * other, bool baseStats);
typedef int32_t (CDivinityStats_Character__GetHitChance)(CDivinityStats_Character * attacker, CDivinityStats_Character * target);
typedef bool (CDivinityStats_Character__GetTalent)(CDivinityStats_Character * self, TalentType talentId, char excludeBoosts);
typedef int32_t (CDivinityStats_Character__GetAbility)(CDivinityStats_Character * self, AbilityType abilityId, char excludeBoosts, char noLoneWolfBonus);
*/
}
