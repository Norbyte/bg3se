#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Components/Boosts.h>

namespace bg3se
{
	struct UuidComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Uuid;

		Guid EntityUuid;
	};
	
	struct UuidToHandleMappingComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::UuidToHandleMapping;

		MultiHashMap<Guid, EntityHandle> Mappings;
	};
	
	struct TransformComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Transform;

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

			Guid ResourceUUID;
			int ResourceId;
			double Amount;
			double MaxAmount;
			uint64_t field_28;
			uint64_t field_30;
			std::array<SubAmount, 7> SubAmounts;
			bool SubAmountsValid;
		};


		MultiHashMap<Guid, Array<Amount>> Resources;
	};

	struct HearingComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Hearing;

		float Sight;
		float Hearing;
		float FOV;
	};

	struct SpellBookEntry
	{
		struct InnerEntry
		{
			uint8_t field_0;
			MultiHashSet<int32_t> field_8;
		};

		SpellId Id;
		Guid SpellUUID;
		int32_t field_38;
		int32_t field_3C;
		SpellCooldownType CooldownType;
		uint8_t field_41;
		uint8_t field_42;
		AbilityId SpellCastingAbility;
		Array<InnerEntry> InnerEntries;
	};

	struct SpellBookComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SpellBook;

		uint64_t field_0;
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

	struct ActionResourceConsumeMultiplierBoostComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceConsumeMultiplierBoost;

		Guid ResourceUUID;
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
			Guid SpellUUID;
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

		Array<Guid> Tags;
	};

	struct OsirisTagComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ServerOsirisTag;

		Array<Guid> Tags;
	};

	struct SpellBookPreparesComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SpellBookPrepares;

		Array<SpellIdBase> PreparedSpells;
		MultiHashMap<Guid, int> field_30;
		MultiHashMap<Guid, int> field_88;
	};

	struct RelationComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Relation;

		Guid field_18;
		Guid field_28;
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

		Guid field_18;
		FixedString Origin;
	};

	struct LevelComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Level;

		FixedString LevelName;
		EntityHandle field_20;
		bool field_28;
	};

	struct ActiveComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Active;
	};
}
