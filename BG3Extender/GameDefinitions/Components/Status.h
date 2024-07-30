#pragma once

BEGIN_NS(status)

struct ContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(StatusContainer, "eoc::status::ContainerComponent")

	MultiHashMap<EntityHandle, FixedString> Statuses;
};

struct CauseComponent : public BaseComponent
{
	DEFINE_COMPONENT(StatusCause, "eoc::status::CauseComponent")

	EntityHandle Cause;
};

struct IDComponent : public BaseComponent
{
	DEFINE_COMPONENT(StatusID, "eoc::status::IDComponent")

	FixedString ID;
};

struct IncapacitatedComponent : public BaseComponent
{
	DEFINE_COMPONENT(StatusIncapacitated, "eoc::status::IncapacitatedComponent")

	uint32_t field_0;
	MultiHashMap<int32_t, Array<int32_t>> field_8;
	uint8_t field_48;
};

struct LifetimeComponent : public BaseComponent
{
	DEFINE_COMPONENT(StatusLifetime, "eoc::status::LifetimeComponent")

	int field_0;
	int field_4;
};

struct VisualDisabledComponent : public BaseComponent
{
	DEFINE_COMPONENT(StatusVisualDisabled, "eoc::status::visual::DisabledComponent")

	MultiHashSet<FixedString> Visuals;
};

END_NS()


BEGIN_NS(esv::status)

struct CauseComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerStatusCause, "esv::status::CauseComponent")

	Guid Cause;
	int32_t StoryActionId;
};

struct StatusComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerStatus, "esv::status::StatusComponent")

	EntityHandle Entity;
	ComponentHandle StatusHandle;
	FixedString StatusId;
	StatusType Type;
	Guid SpellCastSourceUuid;
};

struct OwnershipComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerStatusOwnership, "esv::status::OwnershipComponent")

	EntityHandle Owner;
};

struct DifficultyModifiersComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerStatusDifficultyModifiers, "esv::status::DifficultyModifiersComponent")

	MultiHashMap<FixedString, MultiHashSet<FixedString>> Modifiers;
};

struct UniqueComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerStatusUnique, "esv::status::UniqueComponent")

	MultiHashMap<FixedString, ComponentHandle> Unique;
};

struct PerformingComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerStatusPerforming, "esv::status::PerformingComponent")

	FixedString field_0;
};

DEFINE_TAG_COMPONENT(esv::status, ActiveComponent, ServerStatusActive)
DEFINE_TAG_COMPONENT(esv::status, AddedFromSaveLoadComponent, ServerStatusAddedFromSaveLoad)
DEFINE_TAG_COMPONENT(esv::status, AuraComponent, ServerStatusAura)

END_NS()
