#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
{
	struct TransformComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Transform;

		__int64 field_18;
		Transform Transform;
		int field_9C;
	};

	struct ActionResourcesComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResources;

		struct Amount
		{
			UUID field_0;
			int ResourceId;
			double Amount1;
			double Amount2;
			__int64 field_28;
			__int64 field_30;
			__int64 field_38;
			__int64 field_40;
			__int64 field_48;
			__int64 field_50;
			__int64 field_58;
			__int64 field_60;
			__int64 field_68;
			__int64 field_70;
			__int64 field_78;
			__int64 field_80;
			__int64 field_88;
			__int64 field_90;
			__int64 field_98;
		};


		struct Amounts
		{
			Array<Amount> Amounts;
		};


		VirtualMultiHashMap<UUID, Amounts> Resources;
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
		SomeGuidId Id;
		UUID field_28;
		__int64 field_38;
		char field_40;
		char field_41;
		char field_42;
		char field_43;
		Array<void*> field_48;
	};

	struct SpellBookComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SpellBook;

		Array<SpellBookEntry> Spells;
	};
	
	struct SurfacePathInfluence
	{
		uint8_t SurfaceType;
		uint8_t SurfaceTypeFlag;
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

		__int64 field_18;
		int field_20;
		int field_24;
		__int64 field_28;
		int Slot;
		int Charges;
		int MaxCharges;
		char ItemUseType;
		char field_3D;
		char field_3E;
		char field_3F;
		__int64 field_40;
		int field_48;
		int field_4C;
		__int64 field_50;
		__int64 field_58;
		int field_60;
		int field_64;
		__int64 field_68;
	};

	struct WieldingComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Wielding;

		EntityWorldHandle Owner;
	};

	struct ActionResourceConsumeMultiplierBoost : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceConsumeMultiplierBoost;

		UUID ResourceUUID;
		int field_28;
		__int64 field_30;
	};

	struct SpellContainerComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SpellContainer;

		Array<void*> Spells;
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

	struct SpellBookPrepares : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::SpellBookPrepares;

		Array<SomeGuidId> PreparedSpells_SomeGuidId;
		VirtualMultiHashMap<int, UUID> field_30;
		VirtualMultiHashMap<int, UUID> field_88;
	};

	struct RelationComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Relation;

		GUID field_18;
		GUID field_28;
		EntityWorldHandle field_38;
	};

	struct CanInteractComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanInteract;

	     uint32_t Flags;
	};

	struct CanSpeakComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CanSpeak;

		uint32_t Flags;
	};

	struct OriginComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Origin;

		__int64 field_18;
		__int64 field_20;
		FixedString Origin;
	};

	struct LevelComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Level;

		FixedString LevelName;
		EntityWorldHandle field_20;
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
