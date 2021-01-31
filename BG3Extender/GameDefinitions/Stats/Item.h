#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/Stats/Common.h>

namespace dse
{
	struct IGameObject;

	struct CDivinityStats_Equipment_Attributes : public ProtectedGameObject<CDivinityStats_Equipment_Attributes>
	{
		void* VMT;
		int ObjectInstanceId;
		int field_C;
		struct CDivinityStats* DivinityStats;
		int MaxDurability;
		int DurabilityResetValue;
		int Strength;
		int Dexterity;
		int Intelligence;
		int Constitution;
		int Wisdom;
		int Charisma;
		int Sight;
		int Hearing;
		int Vitality;
		int MagicPoints;
		int APMaximum;
		int APStart;
		int APRecovery;
		int Accuracy;
		int Dodge;
		int LifeSteal;
		int CriticalChance;
		int ChanceToHitBoost;
		int MovementSpeedBoost;
		int RuneSlots;
		int RuneSlots_V1;
		int Speed;
		int Initiative;
		int Willpower;
		int Bodybuilding;
		int MaxSummons;
		int Skills2[18];
		int Value;
		int Weight;
		int IdentifyLevel;
		void* Reflection;
		FixedString Spells;
		FixedString ItemColor;
		int ModifierType;
		FixedString field_F4;
		FixedString field_F8;
		int field_17C;
		int64_t AttributeFlags;
	};

	struct CDivinityStats_Equipment_Attributes_Weapon : public CDivinityStats_Equipment_Attributes
	{
		int DamageType;
		int MinDamage;
		int MaxDamage;
		int DamageBoost;
		int DamageFromBase;
		int CriticalDamage;
		float WeaponRange;
		int CleaveAngle;
		float CleavePercentage;
		int AttackAPCost;
		FixedString Projectile;
	};

	struct CDivinityStats_Equipment_Attributes_Armor : public CDivinityStats_Equipment_Attributes
	{
		int ArmorValue;
		int ArmorBoost;
	};

	struct CDivinityStats_Equipment_Attributes_Shield : public CDivinityStats_Equipment_Attributes
	{
		int ArmorValue;
		int ArmorBoost;
		int Blocking;
	};


	struct CDivinityStats_Item : public CRPGStats_ObjectInstance
	{
		int Version;
		int ItemType;
		int ItemSlot_Enum;
		int WeaponType;
		int AnimType;
		int IsIdentified;
		int field_218;
		int field_21C;
		bool IsTwoHanded;
		bool ShouldSyncStats;
		bool ScaledLevelRequirementsComputed;
		bool HasModifiedSkills;
		bool IsTwoHanded2;
		bool field_17D;
		bool field_17E;
		bool HasModifiedSpells;
		FixedString Spells;
		int DamageTypeOverwrite;
		int Durability;
		int DurabilityCounter;
		FixedString Rarity;
		FixedString ItemGroup;
		Vector<CDivinityStats_Equipment_Attributes*> DynamicAttributes;
		IGameObject* Item;
		StatAttributeFlags ComputedAttributeFlags;
		int MaxCharges;
		int Charges;
		int8_t LevelGroupIndex;
		int8_t RootGroupIndex;
		int8_t NameGroupIndex;
		int8_t NameIndex;
		int8_t NameCool;
		ObjectSet<FixedString> Tags;
		ObjectSet<FixedString> BoostNameSet;

		/*int32_t GetPhysicalResistance();
		int32_t GetPiercingResistance();
		int32_t GetMagicResistance();
		int32_t GetCorrosiveResistance();

		bool HasTalent(TalentType talent);
		int32_t GetAbility(AbilityType ability);*/
	};
}
