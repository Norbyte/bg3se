#pragma once

BEGIN_NS(status)

struct ContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(StatusContainer, "eoc::status::ContainerComponent")

	HashMap<EntityHandle, FixedString> Statuses;
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
	HashMap<int32_t, Array<int32_t>> field_8;
	uint8_t field_48;
};

struct LifetimeComponent : public BaseComponent
{
	DEFINE_COMPONENT(StatusLifetime, "eoc::status::LifetimeComponent")

	int field_0;
	[[bg3::legacy(field_4)]] float Lifetime;
};

struct VisualDisabledComponent : public BaseComponent
{
	DEFINE_COMPONENT(StatusVisualDisabled, "eoc::status::visual::DisabledComponent")

	HashSet<FixedString> Visuals;
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

	HashMap<FixedString, HashSet<FixedString>> Modifiers;
};

struct UniqueComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerStatusUnique, "esv::status::UniqueComponent")

	HashMap<FixedString, ComponentHandle> Unique;
};

struct PerformingComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerStatusPerforming, "esv::status::PerformingComponent")

	FixedString PerformEvent;
};

struct Aura
{
	FixedString field_0;
	int field_4;
	HashSet<EntityHandle> field_8;
};

struct AuraContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerStatusAuraContainer, "esv::status::aura::ContainerComponent")

	Array<Aura> Auras;
};

struct AddedStatusAuraEffectEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ServerAddedStatusAuraEffectEvent, "esv::status::aura::AddedStatusAuraEffectEventOneFrameComponent")

	FixedString field_0;
	int field_4;
	EntityHandle field_8;
	EntityHandle field_10;
};

struct RemovedStatusAuraEffectEventOneFrameComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerRemovedStatusAuraEffectEvent, "esv::status::aura::RemovedStatusAuraEffectEventOneFrameComponent")

	FixedString field_0;
	int field_4;
	EntityHandle field_8;
	EntityHandle field_10;
};

struct AddEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ServerStatusAddEvent, "esv::status::AddEventOneFrameComponent")

	EntityHandle Target;
	FixedString StatusId;
	int32_t Duration;
};

struct ApplyEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ServerStatusApplyEvent, "esv::status::ApplyEventOneFrameComponent")

	EntityHandle Status;
	EntityHandle Target;
	FixedString StatusId;
	Guid field_18;
};

struct AttemptEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ServerStatusAttemptEvent, "esv::status::AttemptEventOneFrameComponent")

	StatusType Type;
	EntityHandle Target;
	EntityHandle Source;
	int StoryActionId;
	FixedString StatusId;
	Guid field_20;
};

struct AttemptFailedEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ServerStatusAttemptFailedEvent, "esv::status::AttemptFailedEventOneFrameComponent")

	FixedString StatusId;
	EntityHandle Target;
	int StoryActionId;
	Guid field_18;
};

struct DispelRollCheckOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ServerStatusDispelRollCheck, "esv::status::DispelRollCheckOneFrameComponent")

	uint8_t field_0;
	uint8_t field_1;
	EntityHandle field_8;
	EntityHandle field_10;
};

struct RefreshCause
{
	__int64 field_0;
	__int64 field_8;
	__int64 field_10;
};


struct RefreshedOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ServerStatusRefreshed, "esv::status::RefreshedOneFrameComponent")

	Array<RefreshCause> Causes;
};

struct RemoveEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ServerStatusRemoveEvent, "esv::status::RemoveEventOneFrameComponent")

	FixedString StatusId;
	StatusType Type;
	EntityHandle Target;
	EntityHandle field_10;
	EntityHandle Status;
	EntityHandle Source;
	int StoryActionId;
	uint8_t field_2C;
	uint8_t field_2D;
	Guid field_30;
	Guid field_40;
	Guid field_50;
};

struct DestroyedEventRequest
{
	FixedString StatusId;
	ComponentHandle StatusHandle;
	EntityHandle Target;
	EntityHandle Status;
	EntityHandle Source;
	EntityHandle field_28;
	Guid SourceUuid;
	Guid field_40;
	Guid field_50;
	Guid field_60;
	int field_70;
	StatusType Type;
	uint8_t field_78;
	uint8_t field_79;
	uint8_t field_7A;
	uint8_t field_7B;
	EntityHandle field_80;
};

struct ScheduledForDeletionOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ServerStatusScheduledForDeletion, "esv::status::ScheduledForDeletionOneFrameComponent")

	DestroyedEventRequest Request;
};

struct StatusEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ServerStatusEvent, "esv::status::StatusEventOneFrameComponent")

	uint32_t Events;
};

struct TurnStartEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ServerStatusTurnStartEvent, "esv::status::TurnStartEventOneFrameComponent")

	EntityHandle Status;
};

struct UpdateTargetTrackingOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(ServerStatusUpdateTargetTracking, "esv::status::UpdateTargetTrackingOneFrameComponent")

	Guid field_0;
	HashSet<EntityHandle> Targets;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::status, DownedChangedEventOneFrameComponent, ServerStatusDownedChangedEvent)

DEFINE_TAG_COMPONENT(esv::status, ActiveComponent, ServerStatusActive)
DEFINE_TAG_COMPONENT(esv::status, AddedFromSaveLoadComponent, ServerStatusAddedFromSaveLoad)
DEFINE_TAG_COMPONENT(esv::status, AuraComponent, ServerStatusAura)

END_NS()
