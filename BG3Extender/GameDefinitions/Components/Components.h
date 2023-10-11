#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

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
		std::optional<std::array<SubAmount, 7>> SubAmounts;
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

	Array<stats::Requirement> Requirements;
	int Charges;
	int MaxCharges;
	uint8_t ItemUseType;
	uint8_t field_19;
	uint8_t field_1A;
	uint8_t field_1B;
	Array<BoostParameters> Boosts;
	Array<BoostParameters> BoostsOnEquipMainHand;
	Array<BoostParameters> BoostsOnEquipOffHand;
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

struct RelationComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Relation;
	static constexpr auto EngineClass = "eoc::relation::RelationComponent";

	struct GuidAndHandle
	{
		Guid field_0;
		EntityHandle field_10;

		inline bool operator == (GuidAndHandle const& o) const
		{
			return field_0 == o.field_0
				&& field_10 == o.field_10;
		}
	};

	MultiHashMap<uint32_t, uint8_t> field_0;
	MultiHashMap<uint32_t, uint8_t> field_40;
	MultiHashMap<uint32_t, uint8_t> field_80;
	MultiHashMap<GuidAndHandle, uint8_t> field_C0;
	MultiHashMap<uint32_t, uint8_t> field_100;
	MultiHashSet<uint32_t> field_140;
	MultiHashSet<uint32_t> field_170;
};

template <>
inline uint64_t MultiHashMapHash<RelationComponent::GuidAndHandle>(RelationComponent::GuidAndHandle const& v)
{
	return HashMulti(v.field_0, v.field_10);
}

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

struct MaterialParameterOverrideComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::MaterialParameterOverride;
	static constexpr auto EngineClass = "eoc::MaterialParameterOverrideComponent";

	struct Param
	{
		STDString field_0;
		FixedString field_18;
	};

	Array<Guid> field_0;
	Array<Param> field_10;
};

DEFINE_TAG_COMPONENT(eoc, OffStageComponent, OffStage)
DEFINE_TAG_COMPONENT(eoc, PickingStateComponent, PickingState)
DEFINE_TAG_COMPONENT(eoc, PlayerComponent, Player)
DEFINE_TAG_COMPONENT(eoc, SimpleCharacterComponent, SimpleCharacter)
DEFINE_TAG_COMPONENT(eoc, WeaponSetComponent, WeaponSet)

struct SpeakerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Speaker;
	static constexpr auto EngineClass = "eoc::SpeakerComponent";

	Array<FixedString> field_0;
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

	EntityHandle field_0;
	FixedString LevelName;
};
	
struct TransformComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Transform;
	static constexpr auto EngineClass = "ls::TransformComponent";

	Transform Transform;
};

END_SE()
