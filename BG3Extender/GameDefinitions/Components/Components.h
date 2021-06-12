#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Components/Boosts.h>

namespace bg3se
{
	struct TransformComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Transform;

		float field_18;
		float field_1C;
		Transform Transform;
		float field_9C;
	};

	struct ActionResourcesComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResources;

		struct Amount
		{
			struct SubAmount
			{
				double Amount;
				double MaxAmount;
			};

			UUID ResourceUUID;
			int ResourceId;
			double Amount;
			double MaxAmount;
			std::array<SubAmount, 7> SubAmounts;
			bool SubAmountsValid;
		};


		VirtualMultiHashMap<UUID, Array<Amount>> Resources;
	};

	struct SenseComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Sense;

		float Sight;
		float Hearing;
		float FOV;
	};

	struct SpellBookEntry
	{
		SpellId Id;
		UUID SpellUUID;
		__int64 field_38;
		SpellCooldownType CooldownType;
		uint8_t field_41;
		uint8_t field_42;
		AbilityId SpellCastingAbility;
		Array<void*> InnerEntries;
	};

	struct SpellBookComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SpellBook;

		Array<SpellBookEntry> Spells;
	};

	
	struct SurfacePathInfluence
	{
		SurfaceType SurfaceType;
		bool IsCloud;
		int Influence;
	};

	struct SurfacePathInfluencesComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SurfacePathInfluences;

		Array<SurfacePathInfluence> PathInfluences;
	};

	struct UseComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Use;

		__int64 field_18_PTR;
		int field_20;
		int field_24;
		__int64 field_28;
		int Charges;
		int MaxCharges;
		uint8_t ItemUseType;
		uint8_t field_39;
		uint8_t ItemComboFlag1;
		uint8_t ItemComboFlag2;
		uint8_t ItemUseType_M;
		uint8_t field_3D;
		uint8_t field_3E;
		uint8_t field_3F;
		Array<BoostParameters> Boosts;
	};

	struct WieldingComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Wielding;

		EntityHandle Owner;
	};

	struct ActionResourceConsumeMultiplierBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceConsumeMultiplierBoost;

		UUID ResourceUUID;
		int Multiplier;
		__int64 field_30;
	};

	struct SpellContainerComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SpellContainer;

		struct Spell
		{
			SpellIdBase SpellId;
			EntityHandle ItemHandle;
			SpellChildSelectionType SelectionType;
			uint8_t field_29;
			UUID SpellUUID;
			AbilityId SpellCastingAbility;
			SpellCooldownType CooldownType;
			FixedString field_44;
			uint8_t field_48;
		};

		Array<Spell> Spells;
	};

	struct TagComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Tag;

		Array<UUID> Tags;
	};

	struct OsirisTagComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerOsirisTag;

		Array<UUID> Tags;
	};

	struct SpellBookPreparesComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SpellBookPrepares;

		Array<SpellIdBase> PreparedSpells;
		VirtualMultiHashMap<UUID, int> field_30;
		VirtualMultiHashMap<UUID, int> field_88;
	};

	struct RelationComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Relation;

		UUID field_18;
		UUID field_28;
		EntityHandle field_38;
	};

	struct CanInteractComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanInteract;

		uint16_t Flags;
	};

	struct CanSpeakComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanSpeak;

		uint16_t Flags;
	};

	struct OriginComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Origin;

		UUID field_18;
		FixedString Origin;
	};

	struct LevelComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Level;

		FixedString LevelName;
		EntityHandle field_20;
		bool field_28;
	};

	namespace esv
	{
		struct ActiveComponent : public BaseComponent
		{
			static constexpr ExtComponentType ComponentType = ExtComponentType::ServerActive;
		};
	}
}
