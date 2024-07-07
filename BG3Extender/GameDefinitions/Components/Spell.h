#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Hit.h>
#include <GameDefinitions/Interrupt.h>

BEGIN_NS(interrupt)

struct ActionStateComponent : public BaseComponent
{
	DEFINE_COMPONENT(InterruptActionState, "eoc::interrupt::ActionStateComponent")

	InterruptVariant2 Variant;
	Array<InterruptEntities> Arr_EHx2;
	Guid field_118;
};

struct ConditionallyDisabledComponent : public BaseComponent
{
	DEFINE_COMPONENT(InterruptConditionallyDisabled, "eoc::interrupt::ConditionallyDisabledComponent")

	uint8_t Dummy;
};

struct ContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(InterruptContainer, "eoc::interrupt::ContainerComponent")

	Array<EntityHandle> Interrupts;
};

struct DataComponent : public BaseComponent
{
	DEFINE_COMPONENT(InterruptData, "eoc::interrupt::DataComponent")

	FixedString Interrupt;
	uint8_t field_4;
	EntityHandle field_8;
	EntityHandle field_10;
	FixedString field_18;
};

struct DecisionComponent : public BaseComponent
{
	DEFINE_COMPONENT(InterruptDecision, "eoc::interrupt::DecisionComponent")

	MultiHashMap<InterruptVariant2, uint8_t> Decisions;
};

struct PreferencesComponent : public BaseComponent
{
	DEFINE_COMPONENT(InterruptPreferences, "eoc::interrupt::PreferencesComponent")

	MultiHashMap<FixedString, uint8_t> Preferences;
};

struct PreparedComponent : public BaseComponent
{
	DEFINE_COMPONENT(InterruptPrepared, "eoc::interrupt::PreparedComponent")

	uint8_t Dummy;
};

struct ZoneComponent : public BaseComponent
{
	DEFINE_COMPONENT(InterruptZone, "eoc::interrupt::ZoneComponent")

	Guid field_0;
};

struct ZoneParticipantComponent : public BaseComponent
{
	DEFINE_COMPONENT(InterruptZoneParticipant, "eoc::interrupt::ZoneParticipantComponent")

	MultiHashMap<EntityHandle, uint8_t> field_0;
};

struct ZoneSourceComponent : public BaseComponent
{
	DEFINE_COMPONENT(InterruptZoneSource, "eoc::interrupt::ZoneSourceComponent")

	uint8_t Dummy;
};

END_NS()

BEGIN_NS(spell)

struct Modification
{
	struct ModifyAreaRadius
	{
		uint64_t Value;
	};

	struct ModifyMaximumTargets
	{
		uint64_t Value;
	};

	struct ModifyNumberOfTargets
	{
		glm::vec3 Value;
	};

	struct ModifySavingThrowDisadvantage
	{
		uint8_t Value;
	};

	struct ModifySpellFlags
	{
		uint8_t Value;
		uint8_t field_1;
	};

	struct ModifySpellRoll
	{
		STDString field_0;
		STDString field_18;
		int field_30;
	};

	struct ModifyStatusDuration
	{
		uint64_t Value;
	};

	struct ModifySummonDuration
	{
		uint64_t Value;
	};

	struct ModifySurfaceDuration
	{
		uint64_t Value;
	};

	struct ModifyTargetRadius
	{
		uint64_t Value;
	};

	struct ModifyUseCosts
	{
		uint8_t Type;
		Guid field_10;
		STDString field_18;
		int field_30;
		Guid field_38;
	};

	struct ModifyVisuals
	{
		uint8_t Value;
	};

	struct ModifyIconGlow
	{
		uint8_t Value;
	};

	struct ModifyTooltipDescription
	{
		uint8_t Value;
	};

	using Variant = std::variant<ModifyAreaRadius, ModifyMaximumTargets, ModifyNumberOfTargets, ModifySavingThrowDisadvantage, ModifySpellFlags, ModifySpellRoll, ModifyStatusDuration, ModifySummonDuration, ModifySurfaceDuration, ModifyTargetRadius, ModifyUseCosts, ModifyVisuals, ModifyIconGlow, ModifyTooltipDescription>;

	uint8_t field_0;
	FixedString field_4;
	Variant Modification;
	MultiHashSet<SpellId> Spells;
};

struct SpellContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellContainer, "eoc::spell::ContainerComponent")

	struct Spell
	{
		SpellIdBase SpellId;
		EntityHandle ItemHandle;
		[[bg3::legacy(SelectionType)]] SpellLearningStrategy LearningStrategy;
		uint8_t field_29;
		Guid SpellUUID;
		AbilityId SpellCastingAbility;
		SpellCooldownType CooldownType;
		FixedString ContainerSpell;
		uint8_t field_48;
	};

	Array<Spell> Spells;
};

struct PlayerPrepareSpellComponent : public BaseComponent
{
	DEFINE_COMPONENT(PlayerPrepareSpell, "eoc::spell::PlayerPrepareSpellComponent")

	Array<SpellIdBase> Spells;
	uint8_t field_30;
};

struct CCPrepareSpellComponent : public BaseComponent
{
	DEFINE_COMPONENT(CCPrepareSpell, "eoc::spell::CCPrepareSpellComponent")

	Array<SpellIdBase> Spells;
};

struct SpellModificationContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellModificationContainer, "eoc::spell::ModificationContainerComponent")

	MultiHashMap<FixedString, Array<Modification>> Modifications;
};

struct AddedSpellsComponent : public BaseComponent
{
	DEFINE_COMPONENT(AddedSpells, "eoc::spell::AddedSpellsComponent")

	Array<SpellContainerComponent::Spell> Spells;
};

struct SpellBookCooldownsComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellBookCooldowns, "eoc::spell::BookCooldownsComponent")

	struct Cooldown
	{
		SpellId SpellId;
		SpellCooldownType CooldownType;
		uint8_t field_29;
		float Cooldown;
		Guid field_30;
	};

	Array<Cooldown> Cooldowns;
};

struct LearnedSpellsComponent : public BaseComponent
{
	DEFINE_COMPONENT(LearnedSpells, "eoc::spell::LearnedSpellsComponent")

	MultiHashMap<Guid, MultiHashSet<FixedString>> field_18;
	MultiHashSet<uint8_t> field_70;
};

struct SpellAiConditionsComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellAiConditions, "eoc::spell::AiConditionsComponent")

	MultiHashMap<FixedString, uint64_t> field_18;
};

struct SpellBookEntry
{
	struct InnerEntry
	{
		uint8_t field_0;
		int32_t field_4;
	};

	SpellId Id;
	Guid SpellUUID;
	int32_t field_38;
	int32_t field_3C;
	SpellCooldownType CooldownType;
	[[bg3::legacy(field_41)]] bool Charged;
	[[bg3::legacy(field_42)]] SpellPrepareType PrepareType;
	AbilityId SpellCastingAbility;
	Array<InnerEntry> InnerEntries;
};

struct BookComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellBook, "eoc::spell::BookComponent")

	uint64_t field_0;
	Array<SpellBookEntry> Spells;
};

struct BookPreparesComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellBookPrepares, "eoc::spell::BookPreparesComponent")

	Array<SpellIdBase> PreparedSpells;
	MultiHashMap<Guid, int> field_30;
	MultiHashMap<Guid, int> field_88;
};

END_NS()

BEGIN_NS(spell_cast)

struct AnimationInfoComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellCastAnimationInfo, "eoc::spell_cast::AnimationInfoComponent")

	uint8_t field_0;
	glm::vec3 TargetPosition;
	uint8_t field_10;
	glm::vec3 TargetRotation;
	EntityHandle Target;
	uint8_t field_28;
	uint8_t field_29;
	uint8_t field_2A;
	uint8_t field_2B;
	uint8_t field_2C;
	uint8_t field_2D;
	uint8_t field_2E;
};

struct CacheComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellCastCache, "eoc::spell_cast::CacheComponent")

	uint8_t field_0;
	uint32_t field_4;
};

struct CanBeTargetedComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellCastCanBeTargeted, "eoc::spell_cast::CanBeTargetedComponent")

	uint8_t Dummy;
};

struct IsCastingComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellCastIsCasting, "eoc::spell_cast::IsCastingComponent")
	
	EntityHandle Cast;
};

struct InterruptResultsComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellCastInterruptResults, "eoc::spell_cast::InterruptResultsComponent")
	
	char field_0;
	MultiHashSet<EntityHandle> Results;
};

struct MovementComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellCastMovement, "eoc::spell_cast::MovementComponent")

	glm::vec3 field_0;
	glm::vec3 field_C;
	bool field_18;
};

struct RollsComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellCastRolls, "eoc::spell_cast::RollsComponent")

	struct RollHit
	{
		FixedString field_0;
		Hit Hit;
	};

	struct Roll
	{
		EntityHandle Target;
		std::optional<EntityHandle> field_8;
		Array<RollHit> Hits;
		MultiHashMap<FixedString, int32_t> MHS_FS_i32;
		int field_68;
		uint8_t field_6C;
		__int64 field_70;
		__int64 field_78;
		__int64 field_80;
		uint8_t field_88;
	};

	Array<Roll> Rolls;
};

struct TargetInfo : public ProtectedGameObject<TargetInfo>
{
	[[bg3::hidden]] void* VMT;
	EntityHandle Target;
	std::optional<EntityHandle> field_10;
	std::optional<glm::vec3> Position;
};

struct MultiTargetInfo : public TargetInfo
{
	SpellType SpellType;
	std::optional<TargetInfo> Target2;
};

struct MultiTargetInfo2 : public TargetInfo
{
	MultiTargetInfo Target2;
	uint8_t field_A0;
};

struct StateComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellCastState, "eoc::spell_cast::StateComponent")

	StateComponent(const StateComponent&) = delete;
	StateComponent& operator = (const StateComponent&) = delete;

	EntityHandle Entity;
	EntityHandle Caster;
	SpellId SpellId;
	int field_38;
	Array<MultiTargetInfo> Targets;
	std::optional<glm::vec3> CasterMoveToPosition;
	std::optional<glm::vec3> field_60;
	glm::vec3 CasterStartPosition;
	EntityHandle field_80;
	int field_88;
	Guid field_90;
	STDString field_A0;
};

struct SyncTargetingComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellSyncTargeting, "eoc::spell_cast::SyncTargetingComponent")

	SyncTargetingComponent(const SyncTargetingComponent&) = delete;
	SyncTargetingComponent& operator = (const SyncTargetingComponent&) = delete;

	[[bg3::hidden]]
	void* VMT;
	EntityHandle field_8;
	std::optional<EntityHandle> field_10;
	std::optional<glm::vec3> field_20;
	Array<MultiTargetInfo> Targets;
	uint8_t field_40;
	int field_44;
	std::optional<glm::vec3> field_48;
	std::optional<glm::vec3> field_58;
	std::optional<glm::vec3> CasterPosition;
	std::optional<glm::vec3> CasterMoveToPosition;
	std::optional<glm::vec3> field_88;
};

END_NS()

BEGIN_NS(esv::interrupt)

struct ActionRequest1
{
	__int64 field_0;
	__int64 field_8;
	bg3se::interrupt::InterruptVariant2 field_10;
	MultiHashMap<EntityHandle, MultiHashSet<EntityHandle>> MHM_EH_MHS_EH;
	__int64 field_158;
};

struct ActionRequest2
{
	__int64 field_0;
	__int64 field_8;
	bg3se::interrupt::InterruptVariant2 field_10;
	__int64 field_118;
};

struct ActionRequest3
{
	__int64 field_0;
	__int64 field_8;
	MultiHashMap<EntityHandle, MultiHashSet<EntityHandle>> MHM_EH_MHS_EH;
};

struct ActionRequest4
{
	__int64 field_0;
	Array<stats::SpellPrototype::UseCostGroup> UseCosts;
	MultiHashMap<EntityHandle, MultiHashMap<bg3se::interrupt::InterruptVariant2, ConditionRoll>> field_18;
};

END_NS()

BEGIN_NS(esv::spell)

struct OnDamageSpell
{
	FixedString Spell;
	int field_4;
	uint8_t field_8;
};

struct OnDamageSpellsComponent : public BaseComponent
{
	DEFINE_COMPONENT(OnDamageSpells, "esv::spell::OnDamageSpellsComponent")

	Array<OnDamageSpell> Spells;
};


END_NS()

BEGIN_NS(esv::spell_cast)

struct CastState
{
	Guid field_0;
	FixedString field_10;
	FixedString field_14;
	FixedString field_18;
	uint8_t field_1C;
};

struct CastHitDelayComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSpellCastHitDelay, "esv::spell_cast::CastHitDelayComponent")

	struct CastHitDelay
	{
		int HitNumber;
		int field_4;
		int field_8;
		int field_C;
		float field_10;
		FixedString TextKey;
		int field_18;
	};

	Array<CastHitDelay> CastHitDelays;
	float CastTargetHitDelay;
	float CastTargetHitDelay2;
};

struct CastResponsibleComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSpellCastResponsible, "esv::spell_cast::CastResponsibleComponent")

	EntityHandle Entity;
};

DEFINE_TAG_COMPONENT(esv::spell_cast, ClientInitiatedComponent, ServerSpellClientInitiated)

struct ExternalsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSpellExternals, "esv::spell_cast::ExternalsComponent")

	Array<Guid> Externals;
};

struct HitRegisterComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSpellHitRegister, "esv::spell_cast::HitRegisterComponent")

	Array<Guid> Hits;
};

struct InterruptRequestsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSpellInterruptRequests, "esv::spell_cast::InterruptRequestsComponent")

	Array<interrupt::ActionRequest1> Requests1;
	Array<interrupt::ActionRequest2> Requests2;
	Array<interrupt::ActionRequest4> Requests4;
	Array<interrupt::ActionRequest3> Requests3;
};

struct InterruptIdentifier
{
	Guid field_0;
	uint64_t field_10;

	inline bool operator == (InterruptIdentifier const& o) const
	{
		return field_0 == o.field_0
			&& field_10 == o.field_10;
	}
};

struct InterruptRollData
{
	__int64 field_0;
	Array<ResolvedUnknown> field_8;
};

struct InterruptResult
{
	MultiHashMap<uint8_t, MultiHashMap<uint8_t, InterruptRollData>> field_0;
	MultiHashMap<uint8_t, MultiHashMap<uint8_t, uint64_t>> field_40;
	Array<uint16_t> field_50;
	MultiHashSet<uint8_t> field_60;
	MultiHashSet<uint8_t> field_90;
};

struct InterruptResult2
{
	__int64 field_0;
	__int64 field_8;
	__int64 field_10;
	int field_18;
	uint8_t field_1C;
	uint32_t _Pad;
	uint8_t field_20;
	uint8_t field_21;
	std::variant<StatsRollType0, StatsRollType1> Roll;
	int field_138;
	__int64 field_140;
	__int64 field_148;
	uint8_t field_150;
	uint8_t field_151;
	uint8_t field_152;
};

struct InterruptResultsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSpellInterruptResults, "esv::spell_cast::InterruptResultsComponent")

	MultiHashMap<InterruptIdentifier, InterruptResult> Results;
	Array<InterruptResult2> Results2;
};

struct StateComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSpellCastState, "esv::spell_cast::StateComponent")

	uint8_t Status;
	int field_4;
	CastState State;
	int field_28;
};


END_NS()

BEGIN_NS(esv::interrupt)

struct AddRemoveRequestsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerInterruptAddRemoveRequests, "esv::interrupt::AddRemoveRequestsComponent")

	MultiHashMap<EntityHandle, uint8_t> Requests;
};

struct ActionRequestsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerInterruptActionRequests, "esv::interrupt::ActionRequestsComponent")

	Array<ActionRequest1> Requests1;
	Array<ActionRequest2> Requests2;
	Array<ActionRequest3> Requests3;
};

struct ZoneRequestsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerInterruptZoneRequests, "esv::interrupt::ZoneRequestsComponent")

	Array<ActionRequest1> Requests1;
	Array<ActionRequest2> Requests2;
};

struct InitialParticipantsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerInterruptInitialParticipants, "esv::interrupt::InitialParticipantsComponent")
	
	struct Participant
	{
		ActionRequest1 Request1;
		MultiHashSet<EntityHandle> Entities;
	};

	MultiHashMap<EntityHandle, Participant> Participants;
};

struct TurnOrderInZoneComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerInterruptTurnOrderInZone, "esv::interrupt::TurnOrderInZoneComponent")
	
	MultiHashSet<EntityHandle> InZone;
};

END_NS()

BEGIN_SE()

template <>
inline uint64_t MultiHashMapHash<esv::spell_cast::InterruptIdentifier>(esv::spell_cast::InterruptIdentifier const& v)
{
	return HashMulti(v.field_0, v.field_10);
}

END_SE()
