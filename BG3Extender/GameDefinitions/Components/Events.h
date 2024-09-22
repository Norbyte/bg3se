#pragma once

BEGIN_NS(esv::templates)

DEFINE_ONEFRAME_TAG_COMPONENT(esv::templates, TemplateChangedOneFrameComponent, ServerTemplateChangedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::templates, TemplateTransformedOneFrameComponent, ServerTemplateTransformedEvent)

END_NS()

BEGIN_NS(esv::tags)

struct TagsChangedEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(TagsChangedEvent, "esv::tags::TagsChangedEventOneFrameComponent")

	Array<Guid> AddedTags;
	Array<Guid> RemovedTags;
};

END_NS()

BEGIN_NS(esv::ecsstats)

struct AbilityCheckEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(AbilityCheckEvent, "esv::stats::AbilityCheckEventOneFrameComponent")

	EntityHandle Source;
	EntityHandle Target;
	AbilityId Ability;
	Guid field_18;
	bool Success;
	uint8_t field_29;
};

struct EquipmentSlotChangedEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(EquipmentSlotChangedEvent, "esv::stats::EquipmentSlotChangedEventOneFrameComponent")

	HashSet<ItemSlot> Slots;
};

struct LevelChangedOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(LevelChanged, "esv::stats::LevelChangedOneFrameComponent")

	int PreviousLevel;
	int NewLevel;
};

struct SavingThrowRolledEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SavingThrowRolledEvent, "esv::stats::SavingThrowRolledEventOneFrameComponent")

	EntityHandle Source;
	EntityHandle Target;
	bool Advantage;
	bool Disadvantage;
	AbilityId Ability;
	stats::RollType RollType;
	int RollBonus;
	bool Success;
	int Total;
	FixedString field_20;
	FixedString field_24;
	uint8_t field_28;
	FixedString field_2C;
	ConditionRoll ConditionRoll;
	Guid SpellCastUuid;
	FixedString field_178;
	uint8_t field_17C;
};

struct SkillCheckEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SkillCheckEvent, "esv::stats::SkillCheckEventOneFrameComponent")

	EntityHandle Target;
	EntityHandle Source;
	SkillId Skill;
	AbilityId Ability;
	FixedString field_14;
	FixedString SpellId;
	ConditionRoll ConditionRoll;
	uint8_t field_158;
	bool Success;
	STDString StatsExpression;
	Guid SpellCastUuid;
	bool Critical;
};

struct WeaponPropertiesChangedEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(WeaponPropertiesChangedEvent, "esv::stats::WeaponPropertiesChangedEventOneFrameComponent")

	uint32_t Properties;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::stats, AttributeFlagsChangedEventOneFrameComponent, AttributeFlagsChangedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::stats, ClassesChangedEventOneFrameComponent, ClassesChangedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::stats, StatsAppliedEventOneFrameComponent, StatsAppliedEvent)

END_NS()

BEGIN_NS(esv::summon)

struct SpawnCreatedEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SummonCreatedEvent, "esv::summon::SpawnCreatedEventOneFrameComponent")

	EntityHandle Entity;
};

struct AddConcentrationRequestOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SummonAddConcentrationRequest, "esv::summon::AddConcentrationRequestOneFrameComponent")

	SpellId Spell;
	EntityHandle Caster;
	uint8_t field_30;
};

struct AddToExistingConcentrationRequestOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SummonAddToExistingConcentrationRequest, "esv::summon::AddToExistingConcentrationRequestOneFrameComponent")

	EntityHandle field_0;
	uint8_t field_8;
};

struct AttachToProjectileRequestOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SummonAttachToProjectileRequest, "esv::summon::AttachToProjectileRequestOneFrameComponent")

	EntityHandle field_0;
};

struct OwnerSetEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SummonOwnerSetEvent, "esv::summon::OwnerSetEventOneFrameComponent")

	EntityHandle Owner;
};

struct PlaceInInventoryRequestOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SummonPlaceInInventoryRequest, "esv::summon::PlaceInInventoryRequestOneFrameComponent")

	EntityHandle Owner;
	bool field_8;
	bool field_9;
	bool CannotRemove;
};

struct SetLifetimeRequestOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(SummonSetLifetimeRequest, "esv::summon::SetLifetimeRequestOneFrameComponent")

	std::optional<float> Lifetime;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::summon, DespawnRequestOneFrameComponent, SummonDespawnRequest)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::summon, ExpiredRequestOneFrameComponent, SummonExpiredRequest)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::summon, LateJoinPenaltyOneFrameComponent, SummonLateJoinPenalty)

END_NS()
