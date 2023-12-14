#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

struct RulesetComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Ruleset;
	static constexpr auto EngineClass = "eoc::ruleset::RulesetComponent";

	Array<Guid> Rulesets;
	// FIXME: First variant is void
	MultiHashMap<Guid, std::variant<uint8_t, int32_t, float, FixedString, bool>> Modifiers;
};

struct RulesetModifiersComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::RulesetModifiers;
	static constexpr auto EngineClass = "eoc::ruleset::RulesetModifiersComponent";

	// FIXME: First variant is void
	MultiHashMap<Guid, std::variant<uint8_t, int32_t, float, FixedString, bool>> Modifiers;
};

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

	float Hearing;
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

struct FactionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Faction;
	static constexpr auto EngineClass = "eoc::relation::FactionComponent";

	EntityHandle field_0;
	Guid field_8;
	Guid field_18;
	EntityHandle field_28;
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
DEFINE_TAG_COMPONENT(eoc::character, CharacterComponent, IsCharacter)

struct SpeakerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Speaker;
	static constexpr auto EngineClass = "eoc::SpeakerComponent";

	Array<FixedString> field_0;
};


struct CanEnterChasmComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CanEnterChasm;
	static constexpr auto EngineClass = "eoc::chasm::CanEnterChasmComponent";

	bool CanEnter;
};


struct DeadByDefaultComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::DeadByDefault;
	static constexpr auto EngineClass = "eoc::death::DeadByDefaultComponent";

	bool DeadByDefault;
};


struct DeathComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Death;
	static constexpr auto EngineClass = "eoc::death::DeathComponent";

	EntityHandle Target;
	EntityHandle field_8;
	Guid field_10;
	uint8_t DeathType;
	uint8_t field_21;
	uint8_t field_22;
	uint8_t field_23;
	uint8_t field_24;
	int field_28;
	BYTE field_2C;
	glm::vec3 field_30;
	glm::vec3 field_3C;
	uint8_t field_48;
	int field_4C;
	Guid field_50;
	uint8_t field_60;
	uint8_t field_61;
	uint8_t field_62;
	int field_64;
	__int64 field_68;
	__int64 field_70;
};


struct DeathStateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::DeathState;
	static constexpr auto EngineClass = "eoc::death::StateComponent";

	uint32_t State;
};


struct DialogStateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::DialogState;
	static constexpr auto EngineClass = "eoc::dialog::StateComponent";

	uint8_t field_0;
	uint8_t field_1;
	uint8_t field_2;
	int field_4;
	uint8_t field_8;
};


struct EncumbranceStateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::EncumbranceState;
	static constexpr auto EngineClass = "eoc::encumbrance::StateComponent";

	uint32_t State;
};


struct EncumbranceStatsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::EncumbranceStats;
	static constexpr auto EngineClass = "eoc::encumbrance::StatsComponent";

	int field_0;
	int field_4;
	int field_8;
};


struct IdentityComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Identity;
	static constexpr auto EngineClass = "eoc::identity::IdentityComponent";

	uint8_t field_0;
};


struct IdentityStateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::IdentityState;
	static constexpr auto EngineClass = "eoc::identity::StateComponent";

	uint8_t field_0;
};


struct OriginalIdentityComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::OriginalIdentity;
	static constexpr auto EngineClass = "eoc::identity::OriginalIdentityComponent";

	uint8_t field_0;
};


struct EquipmentVisualComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::EquipmentVisual;
	static constexpr auto EngineClass = "eoc::character::EquipmentVisualComponent";

	uint8_t State;
};


struct ReposeStateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Repose;
	static constexpr auto EngineClass = "eoc::repose::StateComponent";

	EntityHandle field_0;
	Guid field_8;
	int field_18;
	glm::vec3 field_1C;
	uint8_t field_28;
};


struct OriginalTemplateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::OriginalTemplate;
	static constexpr auto EngineClass = "eoc::templates::OriginalTemplateComponent";

	std::optional<FixedString> OriginalTemplate;
};


DEFINE_TAG_COMPONENT(eoc::through, CanSeeThroughComponent, CanSeeThrough)
DEFINE_TAG_COMPONENT(eoc::through, CanShootThroughComponent, CanShootThrough)
DEFINE_TAG_COMPONENT(eoc::through, CanWalkThroughComponent, CanWalkThrough)


struct ShootThroughTypeComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ShootThroughType;
	static constexpr auto EngineClass = "eoc::through::ShootThroughTypeComponent";

	uint8_t Type;
};


struct UnsheathStateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Unsheath;
	static constexpr auto EngineClass = "eoc::unsheath::StateComponent";

	EntityHandle field_0;
	EntityHandle field_8;
	int field_10;
	int field_14;
	uint8_t field_18;
	uint8_t field_19;
	uint8_t field_1A;
};

struct UseSocket
{
	int field_0;
	uint8_t field_4;
	uint8_t field_5;
	std::array<uint32_t, 10> field_8;
	std::array<uint32_t, 10> field_30;
	FixedString field_58;
	FixedString field_5C;
	EntityHandle field_60;
};


struct UseSocketComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::UseSocket;
	static constexpr auto EngineClass = "eoc::use::SocketComponent";

	MultiHashMap<uint32_t, UseSocket> Sockets;
};


struct UserAvatarComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::UserAvatar;
	static constexpr auto EngineClass = "eoc::user::AvatarComponent";

	int UserID;
	FixedString field_4;
	uint8_t field_8;
};


struct UserReservedForComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::UserReservedFor;
	static constexpr auto EngineClass = "eoc::user::ReservedForComponent";

	int UserID;
};


struct ShapeshiftStateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ShapeshiftState;
	static constexpr auto EngineClass = "eoc::shapeshift::StateComponent";

	std::optional<uint8_t> field_0;
	std::optional<FixedString> field_4;
	std::optional<FixedString> field_C;
	uint8_t field_14;
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

BEGIN_NS(sight)

struct SightBaseComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Sight;
	static constexpr auto EngineClass = "eoc::sight::BaseComponent";

	int field_0;
	int field_4;
	int field_8;
	int field_C;
};


struct DataComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SightData;
	static constexpr auto EngineClass = "eoc::sight::DataComponent";

	Guid field_0;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
};


struct EntityViewshedComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SightEntityViewshed;
	static constexpr auto EngineClass = "eoc::sight::EntityViewshedComponent";

	MultiHashSet<Guid> field_0;
};


END_NS()
