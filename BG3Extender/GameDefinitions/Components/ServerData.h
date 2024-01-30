#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/RootTemplates.h>

BEGIN_NS(esv)

struct RecruitedByComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerRecruitedBy;
	static constexpr auto EngineClass = "esv::recruit::RecruitedByComponent";

	EntityHandle RecruitedBy;
};

struct GameTimerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerGameTimer;
	static constexpr auto EngineClass = "esv::GameTimerComponent";

	FixedString field_18;
	EntityHandle field_20;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	uint8_t field_38;
};

struct ExperienceGaveOutComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerExperienceGaveOut;
	static constexpr auto EngineClass = "esv::exp::ExperienceGaveOutComponent";

	int Experience;
};

DEFINE_TAG_COMPONENT(esv::summon, IsUnsummoningComponent, ServerIsUnsummoning)

struct ActivationGroupContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerActivationGroupContainer;
	static constexpr auto EngineClass = "esv::ActivationGroupContainerComponent";

	struct ActivationGroup
	{
		FixedString field_0;
		FixedString field_4;
	};

	Array<ActivationGroup> Groups;
};

struct AnubisTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerAnubisTag;
	static constexpr auto EngineClass = "esv::tags::AnubisTagComponent";

	Array<Guid> Tags;
};

struct DialogTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerDialogTag;
	static constexpr auto EngineClass = "esv::tags::DialogTagComponent";

	Array<Guid> Tags;
};

struct RaceTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerRaceTag;
	static constexpr auto EngineClass = "esv::tags::RaceTagComponent";

	Array<Guid> Tags;
};

struct TemplateTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerTemplateTag;
	static constexpr auto EngineClass = "esv::tags::TemplateTagComponent";

	Array<Guid> Tags;
};

struct BoostTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerBoostTag;
	static constexpr auto EngineClass = "esv::tags::BoostTagComponent";

	Array<Guid> Tags;
};

struct SafePositionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerSafePosition;
	static constexpr auto EngineClass = "esv::SafePositionComponent";

	glm::vec3 Position;
	bool field_24;
};

struct LeaderComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerLeader;
	static constexpr auto EngineClass = "esv::LeaderComponent";

	MultiHashSet<EntityHandle> Followers_M;
};

struct BreadcrumbComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerBreadcrumb;
	static constexpr auto EngineClass = "esv::BreadcrumbComponent";

	struct Element
	{
		int field_0;
		uint8_t field_4;
		glm::vec3 field_8;
		glm::vec3 field_14;
	};

	std::array<Element, 8> field_18;
	glm::vec3 field_118;
};


struct DelayDeathCauseComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerDelayDeathCause;
	static constexpr auto EngineClass = "esv::death::DelayDeathCauseComponent";

	int DelayCount;
	int Blocked_M;
};

struct KillerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerKiller;
	static constexpr auto EngineClass = "esv::death::KillerComponent";

	MultiHashSet<EntityHandle> Killers;
};

struct StateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerDeathState;
	static constexpr auto EngineClass = "esv::death::StateComponent";

	uint32_t Flags;
};

DEFINE_TAG_COMPONENT(esv::death, DeathContinueComponent, ServerDeathContinue)


struct PickpocketComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerPickpocket;
	static constexpr auto EngineClass = "esv::pickpocket::PickpocketComponent";

	struct PickpocketEntry
	{
		[[bg3::legacy(field_0)]] EntityHandle Target;
		[[bg3::legacy(field_8)]] EntityHandle Item;
		[[bg3::legacy(field_10)]] int Amount;
		bool field_14;
		EntityHandle field_18;
	};

	[[bg3::legacy(field_18)]] Array<PickpocketEntry> Items;
};

struct BaseDataComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerBaseData;
	static constexpr auto EngineClass = "esv::BaseDataComponent";

	std::array<std::array<ResistanceBoostFlags, 7>, 2> Resistances;
	int Weight;
	uint32_t Flags;
};

struct BaseSizeComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerBaseSize;
	static constexpr auto EngineClass = "esv::BaseSizeComponent";

	uint8_t GameSize;
	uint8_t SoundSize;
};

struct BaseStatsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerBaseStats;
	static constexpr auto EngineClass = "esv::BaseStatsComponent";

	int Initiative;
};

struct BaseWeaponComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerBaseWeapon;
	static constexpr auto EngineClass = "esv::BaseWeaponComponent";

	struct Damage
	{
		DamageType DamageType;
		DiceValues Roll;
		uint8_t field_10;
	};

	Array<Damage> DamageList;
};

struct BaseProficiencyComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerBaseProficiency;
	static constexpr auto EngineClass = "esv::stats::proficiency::BaseProficiencyComponent";

	ProficiencyGroupFlags ProficiencyGroup;
	FixedString Proficiency;
};

struct ProficiencyGroupStatsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerProficiencyGroupStats;
	static constexpr auto EngineClass = "esv::stats::proficiency::ProficiencyGroupStatsComponent";

	FixedString Stats;
};

struct GameplayLightEquipmentComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerGameplayLightEquipment;
	static constexpr auto EngineClass = "esv::GameplayLightEquipmentComponent";

	MultiHashSet<EntityHandle> field_0;
};

struct TargetUUIDComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::HistoryTargetUUID;
	static constexpr auto EngineClass = "esv::history::TargetUUIDComponent";

	Guid Target;
};

struct GameplayLightChangesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerGameplayLightChanges;
	static constexpr auto EngineClass = "esv::light::GameplayLightChangesComponent";

  __int64 field_0;
  uint8_t field_8;
  uint8_t field_9;
  uint8_t field_A;
};

struct PeersInRangeComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerPeersInRange;
	static constexpr auto EngineClass = "esv::replication::PeersInRangeComponent";

	Array<int32_t> Peers;
};

struct SurfaceComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerSurface;
	static constexpr auto EngineClass = "esv::surface::SurfaceComponent";

	__int64 field_0;
	__int64 field_8;
	EntityHandle field_10;
};

struct DisarmAttempt
{
	EntityHandle field_0;
	EntityHandle field_8;
	EntityHandle field_10;
	EntityHandle field_18;
	int field_20;
};

struct DisarmAttemptComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerDisarmAttempt;
	static constexpr auto EngineClass = "esv::trap::DisarmAttemptComponent";

	Array<DisarmAttempt> Attempts;
};

struct UnsheathDefaultComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerUnsheath;
	static constexpr auto EngineClass = "esv::unsheath::DefaultComponent";

	int field_0;
	bool field_4;
};

struct UnsheathScriptOverrideComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerUnsheathScriptOverride;
	static constexpr auto EngineClass = "esv::unsheath::ScriptOverrideComponent";

	Array<int32_t> field_0;
};


DEFINE_TAG_COMPONENT(esv::cover, IsLightBlockerComponent, ServerIsLightBlocker)
DEFINE_TAG_COMPONENT(esv::cover, IsVisionBlockerComponent, ServerIsVisionBlocker)
DEFINE_TAG_COMPONENT(esv::darkness, DarknessActiveComponent, ServerDarknessActive)
DEFINE_TAG_COMPONENT(esv::hotbar, OrderComponent, ServerHotbarOrder)
DEFINE_TAG_COMPONENT(esv::level, InventoryItemDataPopulatedComponent, ServerInventoryItemDataPopulated)

DEFINE_TAG_COMPONENT(esv, VariableManagerComponent, ServerVariableManager)
DEFINE_TAG_COMPONENT(esv, IsMarkedForDeletionComponent, IsMarkedForDeletion)
DEFINE_TAG_COMPONENT(esv, NetComponent, Net)

struct JumpFollowComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::JumpFollow;
	static constexpr auto EngineClass = "esv::JumpFollowComponent";

	glm::vec3 field_0;
	glm::vec3 field_C;
	int field_18;
	int field_1C;
	int field_20;
	glm::vec3 field_24;
	glm::vec3 field_30;
	float field_3C;
	float field_40;
	float field_44;
	char field_48;
	float field_4C;
	SpellId Spell;
	float ProjectileTerrainOffset;
	[[bg3::hidden]] Array<void*> field_80;
	Array<glm::vec3> Arr_vec3;
	uint8_t SpellType;
	glm::vec3 field_A4;
	glm::vec3 field_B0;
	float field_BC;
	glm::vec3 field_C0;
	glm::vec3 field_CC;
	glm::vec3 field_D8;
	uint8_t field_E4;
	__declspec(align(4)) uint8_t field_E8;
	int field_EC;
	uint64_t field_F0;
	uint8_t field_F8;
	int field_FC;
	int field_100;
	float field_104;
	uint8_t field_108;
	uint8_t Flags;
	uint8_t field_10A;
};


END_NS()
