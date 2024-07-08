#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/RootTemplates.h>

BEGIN_NS(esv)

struct RecruitedByComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerRecruitedBy, "esv::recruit::RecruitedByComponent")

	EntityHandle RecruitedBy;
};

struct GameTimerComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerGameTimer, "esv::GameTimerComponent")

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
	DEFINE_COMPONENT(ServerExperienceGaveOut, "esv::exp::ExperienceGaveOutComponent")

	int Experience;
};

DEFINE_TAG_COMPONENT(esv::summon, IsUnsummoningComponent, ServerIsUnsummoning)

struct ActivationGroupContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerActivationGroupContainer, "esv::ActivationGroupContainerComponent")

	struct ActivationGroup
	{
		FixedString field_0;
		FixedString field_4;
	};

	Array<ActivationGroup> Groups;
};

struct AnubisTagComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerAnubisTag, "esv::tags::AnubisTagComponent")

	Array<Guid> Tags;
};

struct DialogTagComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerDialogTag, "esv::tags::DialogTagComponent")

	Array<Guid> Tags;
};

struct RaceTagComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerRaceTag, "esv::tags::RaceTagComponent")

	Array<Guid> Tags;
};

struct TemplateTagComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerTemplateTag, "esv::tags::TemplateTagComponent")

	Array<Guid> Tags;
};

struct BoostTagComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerBoostTag, "esv::tags::BoostTagComponent")

	Array<Guid> Tags;
};

struct SafePositionComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSafePosition, "esv::SafePositionComponent")

	glm::vec3 Position;
	bool field_24;
};

struct LeaderComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerLeader, "esv::LeaderComponent")

	MultiHashSet<EntityHandle> Followers_M;
};

struct BreadcrumbComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerBreadcrumb, "esv::BreadcrumbComponent")

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
	DEFINE_COMPONENT(ServerDelayDeathCause, "esv::death::DelayDeathCauseComponent")

	int DelayCount;
	int Blocked_M;
};

struct KillerComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerKiller, "esv::death::KillerComponent")

	MultiHashSet<EntityHandle> Killers;
};

struct StateComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerDeathState, "esv::death::StateComponent")

	uint32_t Flags;
};

DEFINE_TAG_COMPONENT(esv::death, DeathContinueComponent, ServerDeathContinue)


struct PickpocketComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerPickpocket, "esv::pickpocket::PickpocketComponent")

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
	DEFINE_COMPONENT(ServerBaseData, "esv::BaseDataComponent")

	std::array<std::array<ResistanceBoostFlags, 7>, 2> Resistances;
	int Weight;
	uint32_t Flags;
};

struct BaseSizeComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerBaseSize, "esv::BaseSizeComponent")

	uint8_t GameSize;
	uint8_t SoundSize;
};

struct BaseStatsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerBaseStats, "esv::BaseStatsComponent")

	int Initiative;
};

struct BaseWeaponComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerBaseWeapon, "esv::BaseWeaponComponent")

	struct Damage
	{
		DamageType DamageType;
		RollDefinition Roll;
		uint8_t field_10;
	};

	Array<Damage> DamageList;
};

struct BaseProficiencyComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerBaseProficiency, "esv::stats::proficiency::BaseProficiencyComponent")

	ProficiencyGroupFlags ProficiencyGroup;
	FixedString Proficiency;
};

struct ProficiencyGroupStatsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerProficiencyGroupStats, "esv::stats::proficiency::ProficiencyGroupStatsComponent")

	FixedString Stats;
};

struct GameplayLightEquipmentComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerGameplayLightEquipment, "esv::GameplayLightEquipmentComponent")

	MultiHashSet<EntityHandle> field_0;
};

struct TargetUUIDComponent : public BaseComponent
{
	DEFINE_COMPONENT(HistoryTargetUUID, "esv::history::TargetUUIDComponent")

	Guid Target;
};

struct GameplayLightChangesComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerGameplayLightChanges, "esv::light::GameplayLightChangesComponent")

  __int64 field_0;
  uint8_t field_8;
  uint8_t field_9;
  uint8_t field_A;
};

struct PeersInRangeComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerPeersInRange, "esv::replication::PeersInRangeComponent")

	Array<int32_t> Peers;
};

struct SurfaceComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSurface, "esv::surface::SurfaceComponent")

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
	DEFINE_COMPONENT(ServerDisarmAttempt, "esv::trap::DisarmAttemptComponent")

	Array<DisarmAttempt> Attempts;
};

struct UnsheathDefaultComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerUnsheath, "esv::unsheath::DefaultComponent")

	int field_0;
	bool field_4;
};

struct UnsheathScriptOverrideComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerUnsheathScriptOverride, "esv::unsheath::ScriptOverrideComponent")

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
	DEFINE_COMPONENT(JumpFollow, "esv::JumpFollowComponent")

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
