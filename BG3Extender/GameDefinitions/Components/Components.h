#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Components/Boosts.h>

BEGIN_SE()

struct ActionResourcesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResources;
	static constexpr auto EngineClass = "eoc::ActionResourcesComponent";

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
	static constexpr auto EngineClass = "eoc::HearingComponent";

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
	static constexpr auto EngineClass = "eoc::spell::BookComponent";

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
	static constexpr auto EngineClass = "eoc::SurfacePathInfluencesComponent";

	Array<SurfacePathInfluence> PathInfluences;
};

struct UseComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Use;
	static constexpr auto EngineClass = "eoc::UseComponent";

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
	static constexpr auto EngineClass = "eoc::WieldingComponent";

	EntityHandle Owner;
};

struct ActionResourceConsumeMultiplierBoostComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ActionResourceConsumeMultiplierBoost;
	static constexpr auto EngineClass = "eoc::ActionResourceConsumeMultiplierBoostComponent";

	Guid ResourceUUID;
	int Multiplier;
	__int64 field_30;
};

struct SpellContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellContainer;
	static constexpr auto EngineClass = "eoc::spell::ContainerComponent";

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
	static constexpr auto EngineClass = "eoc::TagComponent";

	Array<Guid> Tags;
};

struct OsirisTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerOsirisTag;
	static constexpr auto EngineClass = "esv::tags::OsirisTagComponent";

	Array<Guid> Tags;
};

struct SpellBookPreparesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellBookPrepares;
	static constexpr auto EngineClass = "eoc::spell::BookPreparesComponent";

	Array<SpellIdBase> PreparedSpells;
	MultiHashMap<Guid, int> field_30;
	MultiHashMap<Guid, int> field_88;
};

struct RelationComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Relation;
	static constexpr auto EngineClass = "eoc::relation::RelationComponent";

	Guid field_18;
	Guid field_28;
	EntityHandle field_38;
};

struct CanInteractComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CanInteract;
	static constexpr auto EngineClass = "eoc::CanInteractComponent";

	uint16_t Flags;
};

struct CanSpeakComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CanSpeak;
	static constexpr auto EngineClass = "eoc::CanSpeakComponent";

	uint16_t Flags;
};

struct OriginComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Origin;
	static constexpr auto EngineClass = "eoc::OriginComponent";

	Guid field_18;
	FixedString Origin;
};

struct ActiveComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Active;
	static constexpr auto EngineClass = "eoc::ActiveComponent";
};

struct EocLevelComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::EocLevel;
	static constexpr auto EngineClass = "eoc::LevelComponent";

	int Level;
};

struct ClassesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Classes;
	static constexpr auto EngineClass = "eoc::ClassesComponent";

	struct Class
	{
		Guid ClassUUID;
		Guid SubClassUUID;
		int Level;
	};

	Array<Class> Classes;
};

END_SE()

BEGIN_SE()

struct UuidComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Uuid;
	static constexpr auto EngineClass = "ls::uuid::Component";

	Guid EntityUuid;
};
	
struct UuidToHandleMappingComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::UuidToHandleMapping;
	static constexpr auto EngineClass = "ls::uuid::ToHandleMappingComponent";

	MultiHashMap<Guid, EntityHandle> Mappings;
};

struct LevelComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Level;
	static constexpr auto EngineClass = "ls::LevelComponent";

	FixedString LevelName;
	EntityHandle field_20;
	bool field_28;
};
	
struct TransformComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Transform;
	static constexpr auto EngineClass = "ls::TransformComponent";

	Transform Transform;
	float field_9C;
};

END_SE()
