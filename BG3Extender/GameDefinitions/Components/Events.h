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
