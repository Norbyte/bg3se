#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>

namespace dse
{
	namespace esv
	{
		struct DamagePair
		{
			int32_t Amount;
			DamageType DamageType;
		};

		struct DamageModifierMetadata
		{
			struct DiceValues
			{
				int8_t DiceValue;
				uint8_t AmountOfDices;
				int DiceAdditionalValue;
			};

			uint8_t DamageModifierMetadataType;
			int Value;
			DamageType DamageType;
			uint8_t DamageModifierSourceType;
			union {
				DiceValues Dice;
				int32_t Source;
			};
		};

		struct ConditionRollType1
		{
			uint64_t field_0;
			STDString StatExpression;
			Array<int32_t> IntParams;
			Array<void*> RollParams;
			Array<DamageType> DamageTypeParams;
			int IntIndex;
			int RollIndex;
			int DamageTypeIndex;
		};

		struct DamageResistance
		{
			int ResistanceType;
			DamageType DamageType;
			int Damage;
		};

		struct Hit
		{
			struct Subobj
			{
				RefMap<uint8_t, ObjectSet<void*>> DamageRolls; // ObjectSet<StatRoll> ?
				ObjectSet<DamageModifierMetadata> ModifierMetadata;
				ConditionRollType1 ConditionRoll;
				ObjectSet<DamageModifierMetadata> ModifierMetadata2;
				ObjectSet<DamageResistance> Resistances;
				int AdditionalDamage;
				int TotalDamage;
				int FinalDamage;
				RefMap<uint8_t, int32_t> TotalDamagePerType;
				RefMap<uint8_t, int32_t> FinalDamagePerType;
			};

			int TotalDamageDone;
			int TotalDamagePerceived;
			uint8_t DeathType;
			DamageType DamageType;
			uint8_t CauseType;
			glm::vec3 ImpactPosition;
			glm::vec3 ImpactDirection;
			int ArmorAbsorption;
			int LifeSteal;
			int EffectFlags;
			bool HitWith;
			uint8_t Attribute;
			EntityHandle Inflicter;
			EntityHandle InflicterOwner;
			EntityHandle Throwing;
			int StoryActionID;
			ObjectSet<void*> ConditionRolls;
			Subobj SomeSubobj;
			uint8_t SpellAttackType;
			FixedString SpellId;
			uint8_t SpellSchool;
			int SpellLevel;
			int SpellPowerLevel;
			int TotalHealDone;
			bool InstantKill;
			bool FirstAttack;
			ObjectSet<DamagePair> DamageList;
		};
	}
}
