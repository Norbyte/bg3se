#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/RootTemplates.h>
#include <GameDefinitions/Hit.h>
#include <GameDefinitions/Stats/Common.h>

BEGIN_NS(spell)

struct Modification
{
	struct ModifyAreaRadius
	{
		SpellModificationValueType ValueType;
		float AreaRadius;
	};

	struct ModifyMaximumTargets
	{
		SpellModificationValueType ValueType;
		int32_t MaximumTargets;
	};

	struct ModifyNumberOfTargets
	{
		SpellModificationValueType ValueType;
		int32_t NumberOfTargets;
	};

	struct ModifySavingThrowDisadvantage
	{
		uint8_t SavingThrowDisadvantage;
	};

	struct ModifySpellFlags
	{
		uint8_t SpellFlags;
		uint8_t field_1;
	};

	struct ModifySpellRoll
	{
		STDString Conditions;
		STDString Conditions2;
		int field_30;
	};

	struct ModifyStatusDuration
	{
		SpellModificationValueType ValueType;
		float StatusDuration;
	};

	struct ModifySummonDuration
	{
		SpellModificationValueType ValueType;
		float SummonDuration;
	};

	struct ModifySurfaceDuration
	{
		SpellModificationValueType ValueType;
		float SurfaceDuration;
	};

	struct ModifyTargetRadius
	{
		SpellModificationValueType ValueType;
		float TargetRadius;
	};

	struct ModifyUseCosts
	{
		uint8_t Type;
		Guid Resource;
		STDString Expression;
		int Level;
		Guid Resource2;
	};

	struct ModifyVisuals
	{
		uint8_t Visuals;
	};

	struct ModifyIconGlow
	{
		uint8_t IconGlow;
	};

	struct ModifyTooltipDescription
	{
		uint8_t TooltipDescription;
	};

	using Variant = std::variant<ModifyAreaRadius, ModifyMaximumTargets, ModifyNumberOfTargets, ModifySavingThrowDisadvantage, ModifySpellFlags, ModifySpellRoll, ModifyStatusDuration, ModifySummonDuration, ModifySurfaceDuration, ModifyTargetRadius, ModifyUseCosts, ModifyVisuals, ModifyIconGlow, ModifyTooltipDescription>;

	uint8_t field_0;
	FixedString Source;
	Variant Modification;
	HashSet<SpellId> Spells;
};

struct SpellMeta
{
	SpellMetaId SpellId;
	EntityHandle BoostHandle;
	[[bg3::legacy(SelectionType)]] SpellLearningStrategy LearningStrategy;
	uint8_t field_29;
	[[bg3::legacy(SpellUUID)]] Guid PreferredCastingResource;
	AbilityId SpellCastingAbility;
	SpellCooldownType CooldownType;
	FixedString ContainerSpell;
	uint8_t field_48;
};

struct SpellContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellContainer, "eoc::spell::ContainerComponent")

	Array<SpellMeta> Spells;
};

struct PlayerPrepareSpellComponent : public BaseComponent
{
	DEFINE_COMPONENT(PlayerPrepareSpell, "eoc::spell::PlayerPrepareSpellComponent")

	Array<SpellMetaId> Spells;
	uint8_t field_30;
};

struct CCPrepareSpellComponent : public BaseComponent
{
	DEFINE_COMPONENT(CCPrepareSpell, "eoc::spell::CCPrepareSpellComponent")

	Array<SpellMetaId> Spells;
};

struct SpellModificationContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellModificationContainer, "eoc::spell::ModificationContainerComponent")

	HashMap<FixedString, Array<Modification>> Modifications;
};

struct AddedSpellsComponent : public BaseComponent
{
	DEFINE_COMPONENT(AddedSpells, "eoc::spell::AddedSpellsComponent")

	Array<SpellMeta> Spells;
};

struct CooldownData
{
	SpellId SpellId;
	SpellCooldownType CooldownType;
	uint8_t field_29;
	float Cooldown;
	Guid field_30;
};

struct SpellBookCooldownsComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellBookCooldowns, "eoc::spell::BookCooldownsComponent")

	Array<CooldownData> Cooldowns;
};

struct LearnedSpellsComponent : public BaseComponent
{
	DEFINE_COMPONENT(LearnedSpells, "eoc::spell::LearnedSpellsComponent")

	HashMap<Guid, HashSet<FixedString>> field_18;
	HashSet<SpellSchoolId> SpellSchools;
};

struct SpellAiConditionsComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellAiConditions, "eoc::spell::AiConditionsComponent")

	HashMap<FixedString, AiActionConditions> Conditions;
};

struct CastRequirements
{
	uint8_t field_0;
	int32_t field_4;
};

struct SpellData
{
	SpellId Id;
	[[bg3::legacy(SpellUUID)]] Guid PreferredCastingResource;
	int32_t field_38;
	int32_t field_3C;
	SpellCooldownType CooldownType;
	[[bg3::legacy(field_41)]] bool Charged;
	[[bg3::legacy(field_42)]] SpellPrepareType PrepareType;
	AbilityId SpellCastingAbility;
	Array<CastRequirements> CastRequirements;
	// uint32_t NumCharges;
	// uint32_t UsedCharges;
};

struct BookComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellBook, "eoc::spell::BookComponent")

	uint64_t field_0;
	Array<SpellData> Spells;
};

struct BookPreparesComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellBookPrepares, "eoc::spell::BookPreparesComponent")

	Array<SpellMetaId> PreparedSpells;
	HashMap<Guid, int> field_30;
	HashMap<Guid, int> field_88;
};

END_NS()

BEGIN_NS(spell_cast)

struct AnimationInfoComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellCastAnimationInfo, "eoc::spell_cast::AnimationInfoComponent")

	[[bg3::legacy(field_0)]] uint8_t Event;
	std::optional<glm::vec3> TargetPosition;
	glm::vec3 TargetRotation;
	EntityHandle Target;
	uint8_t field_28;
	uint8_t field_29;
	[[bg3::legacy(field_2A)]] uint8_t LoopingCastAnimation;
	uint8_t field_2B;
	[[bg3::legacy(field_2C)]] uint8_t DualWielding;
	[[bg3::legacy(field_2D)]] uint8_t HasWeapon;
	[[bg3::legacy(field_2E)]] uint8_t ObjectSize;
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
	HashSet<EntityHandle> Results;
};

struct MovementComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellCastMovement, "eoc::spell_cast::MovementComponent")

	glm::vec3 field_0;
	std::optional<glm::vec3> field_C;
	// FixedString TextKey;
};

struct SpellRollCastEventData
{
	[[bg3::legacy(field_0)]] FixedString CastKey;
	HitDesc Hit;
};

struct SpellRollTargetInfo
{
	uint64_t Flags;
	glm::vec3 Position;
};

struct SpellRollData
{
	EntityHandle Target;
	std::optional<EntityHandle> field_8;
	[[bg3::legacy(Hits)]] Array<SpellRollCastEventData> Casts;
	HashMap<FixedString, int32_t> NameToCastIndex;
	int NextReaction;
	SpellMetaConditionType SpellConditionsType;
	std::optional<SpellRollTargetInfo> TargetInfo;
};

struct RollsComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellCastRolls, "eoc::spell_cast::RollsComponent")

	Array<SpellRollData> Rolls;
};

struct BaseTarget : public ProtectedGameObject<BaseTarget>
{
	[[bg3::hidden]] void* VMT;
	EntityHandle Target;
	std::optional<EntityHandle> field_10;
	std::optional<glm::vec3> Position;
};

struct InitialTarget : public BaseTarget
{
	SpellType SpellType;
	std::optional<BaseTarget> Target2;
};

struct IntermediateTarget : public BaseTarget
{
	[[bg3::legacy(Target2)]] InitialTarget InitialTarget;
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
	[[bg3::legacy(field_38)]] uint32_t Flags;
	Array<InitialTarget> Targets;
	std::optional<glm::vec3> CasterMoveToPosition;
	std::optional<glm::vec3> field_60;
	glm::vec3 CasterStartPosition;
	EntityHandle field_80;
	[[bg3::legacy(field_88)]] uint32_t StartTime;
	Guid SpellCastGuid;
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
	Array<InitialTarget> Targets;
	[[bg3::legacy(field_40)]] uint8_t CanMoveToThrowTarget;
	int field_44;
	std::optional<glm::vec3> field_48;
	[[bg3::legacy(field_58)]] std::optional<glm::vec3> HoverPosition;
	std::optional<glm::vec3> CasterPosition;
	std::optional<glm::vec3> CasterMoveToPosition;
	std::optional<glm::vec3> field_88;
};


struct CastTextKeyEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastTextKeyEvent, "eoc::spell_cast::CastTextKeyEventOneFrameComponent")

	EntityHandle CastEntity;
	FixedString TextKey;
};

struct CastHitEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastHitEvent, "eoc::spell_cast::CastHitEventOneFrameComponent")

	EntityHandle CastEntity;
	FixedString TextKey;
};

struct DestroyEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastDestroyEvent, "eoc::spell_cast::DestroyEventOneFrameComponent")

	Guid SpellCastGuid;
};

struct SpellRollAbortEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastSpellRollAbortEvent, "eoc::spell_cast::SpellRollAbortEventOneFrameComponent")

	HitDesc Hit;
	EntityHandle field_1B0;
	__int64 field_1B8;
	uint8_t field_1C0;
};

struct TargetHitEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastTargetHitEvent, "eoc::spell_cast::TargetHitEventOneFrameComponent")

	HitDesc Hit;
	AttackDesc Attack;
	EntityHandle field_1D0;
	__int64 field_1D8;
	char field_1E0;
	EntityHandle field_1E8;
	__int64 field_1F0;
	__int64 field_1F8;
};

struct TargetHitInterruptEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastTargetHitInterruptEvent, "eoc::spell_cast::TargetHitInterruptEventOneFrameComponent")

	HitDesc Hit;
	AttackDesc Attack;
	EntityHandle field_1D0;
	EntityHandle field_1D8;
	std::optional<glm::vec3> field_1E0;
	std::optional<glm::vec3> field_1F0;
	SpellId Spell;
};

struct TargetReactionEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastTargetReactionEvent, "eoc::spell_cast::TargetReactionEventOneFrameComponent")

	HitDesc Hit;
	EntityHandle field_1B0;
	std::optional<glm::vec3> field_1B8;
	EntityHandle CastEntity;
};

struct TargetsChangedEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastTargetsChangedEvent, "eoc::spell_cast::TargetsChangedEventOneFrameComponent")

	TargetsChangedEventOneFrameComponent(const TargetsChangedEventOneFrameComponent&) = delete;
	TargetsChangedEventOneFrameComponent& operator = (const TargetsChangedEventOneFrameComponent&) = delete;

	Array<InitialTarget> Targets;
};

struct AnimationRequestOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastAnimationRequest, "eoc::spell_cast::AnimationRequestOneFrameComponent")

	uint8_t Event;
	std::optional<glm::vec3> TargetHitPosition;
	glm::vec3 field_14;
	EntityHandle Target;
	uint8_t field_28;
	uint8_t field_29;
	bool LoopingCastAnimation;
	bool CacheFlag256;
	bool DualWielding;
	bool HasWeapon;
	uint8_t ObjectSize;
};

DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, CounteredEventOneFrameComponent, SpellCastCounteredEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, JumpStartEventOneFrameComponent, SpellCastJumpStartEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, LogicExecutionEndEventOneFrameComponent, SpellCastLogicExecutionEndEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, PrepareEndEventOneFrameComponent, SpellCastPrepareEndEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, PrepareStartEventOneFrameComponent, SpellCastPrepareStartEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, PreviewEndEventOneFrameComponent, SpellCastPreviewEndEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(eoc::spell_cast, ThrowPickupPositionChangedEventOneFrameComponent, SpellCastThrowPickupPositionChangedEvent)


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

struct NewSpellsAddedEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(NewSpellsAddedEvent, "esv::spell::NewSpellsAddedEventOneFrameComponent")

	Array<SpellMetaId> Spells;
};

struct LearnedSpell
{
	FixedString SpellId;
	Guid ProgressionSource;
};

struct SpellsLearnedEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellsLearnedEvent, "esv::spell::SpellsLearnedEventOneFrameComponent")

	Array<LearnedSpell> Spells;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::spell, BookChangedOneFrameComponent, SpellBookChanged)

END_NS()

BEGIN_NS(esv::spell_cast)

struct CastHitDelayInfo
{
	int HitNumber;
	int field_4;
	int field_8;
	int field_C;
	float field_10;
	FixedString TextKey;
	int field_18;
};

struct CastHitDelayComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSpellCastHitDelay, "esv::spell_cast::CastHitDelayComponent")

	Array<CastHitDelayInfo> CastHitDelays;
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

struct HitRegister
{
	EntityHandle field_0;
	uint64_t field_8;
};

struct HitRegisterComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSpellHitRegister, "esv::spell_cast::HitRegisterComponent")

	Array<HitRegister> Hits;
};

struct StateComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSpellCastState, "esv::spell_cast::StateComponent")

	uint8_t Status;
	int field_4;
	ActionOriginator Originator;
	int StoryActionId;
};

struct CacheComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSpellCastCache, "esv::spell_cast::CacheComponent")

	Array<stats::ActionResourceCost> Costs;
	[[bg3::hidden]] HashMap<int, void*> field_10_MHM_FS_unk;
	__int64 field_50;
	// HashMap<FixedString, HashMap<int, Array<bg3se::spell_cast::IntermediateTarget>>>
	[[bg3::hidden]] HashMap<FixedString, HashMap<int, Array<void*>>> Targets;
	[[bg3::hidden]] HashMap<int, void*> field_98_MHM_FS_unk;
	__int64 field_D8;
	__int64 field_E0;
	__int64 field_E8;
};

struct MoveDuringCastUpdateEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastMoveDuringCastUpdateEvent, "esv::spell_cast::MoveDuringCastUpdateEventOneFrameComponent")

	glm::vec3 field_0;
	glm::vec3 field_C;
	glm::vec3 SurfaceTrail;
};

struct MovementAndPrecalculationEndEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastMovementAndPrecalculationEndEvent, "esv::spell_cast::MovementAndPrecalculationEndEventOneFrameComponent")

	uint8_t field_0;
	int field_4;
};

struct RequestTargetTrackingOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastRequestTargetTracking, "esv::spell_cast::RequestTargetTrackingOneFrameComponent")

	Guid field_0;
	HashSet<EntityHandle> Targets;
};

struct UpdateTargetTrackingOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SpellCastUpdateTargetTracking, "esv::spell_cast::UpdateTargetTrackingOneFrameComponent")

	Guid field_0;
	HashSet<EntityHandle> Targets;
};

END_NS()

BEGIN_NS(esv::concentration)

struct ConcentrationChangedOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ConcentrationChanged, "esv::concentration::ConcentrationChangedOneFrameComponent")

	SpellId Started;
	SpellId Ended;
	bool Interrupted;
};

struct DamageCheckOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ConcentrationDamageCheck, "esv::concentration::DamageCheckOneFrameComponent")

	Array<HitDesc> Hits;
};

struct OnConcentrationClearedEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ConcentrationClearedEvent, "esv::concentration::OnConcentrationClearedEventOneFrameComponent")

	EntityHandle Target;
	EntityHandle Status;
	int16_t SurfaceIndex;
	bool field_12;
};

END_NS()
