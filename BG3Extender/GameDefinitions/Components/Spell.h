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
	MultiHashSet<SpellId> Spells;
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

	MultiHashMap<FixedString, Array<Modification>> Modifications;
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

	MultiHashMap<Guid, MultiHashSet<FixedString>> field_18;
	MultiHashSet<SpellSchoolId> SpellSchools;
};

struct SpellAiConditionsComponent : public BaseComponent
{
	DEFINE_COMPONENT(SpellAiConditions, "eoc::spell::AiConditionsComponent")

	MultiHashMap<FixedString, AiActionConditions> Conditions;
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
	MultiHashMap<FixedString, int32_t> NameToCastIndex;
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
	int field_38;
	Array<InitialTarget> Targets;
	std::optional<glm::vec3> CasterMoveToPosition;
	std::optional<glm::vec3> field_60;
	glm::vec3 CasterStartPosition;
	EntityHandle field_80;
	int field_88;
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
	uint8_t field_40;
	int field_44;
	std::optional<glm::vec3> field_48;
	std::optional<glm::vec3> field_58;
	std::optional<glm::vec3> CasterPosition;
	std::optional<glm::vec3> CasterMoveToPosition;
	std::optional<glm::vec3> field_88;
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
	CastState State;
	int field_28;
};

struct CacheComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerSpellCastCache, "esv::spell_cast::CacheComponent")

	Array<stats::ActionResourceCost> Costs;
	[[bg3::hidden]] MultiHashMap<int, void*> field_10_MHM_FS_unk;
	__int64 field_50;
	// MultiHashMap<FixedString, MultiHashMap<int, Array<bg3se::spell_cast::IntermediateTarget>>>
	[[bg3::hidden]] MultiHashMap<FixedString, MultiHashMap<int, Array<void*>>> Targets;
	[[bg3::hidden]] MultiHashMap<int, void*> field_98_MHM_FS_unk;
	__int64 field_D8;
	__int64 field_E0;
	__int64 field_E8;
};


END_NS()
