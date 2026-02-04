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

struct LoseControlComponent : public BaseComponent
{
    DEFINE_COMPONENT(StatusLoseControl, "eoc::status::LoseControlComponent")

    bool LoseControl;
};

struct VisualDisabledComponent : public BaseComponent
{
    DEFINE_COMPONENT(StatusVisualDisabled, "eoc::status::visual::DisabledComponent")

    HashSet<FixedString> Visuals;
};

DEFINE_TAG_COMPONENT(eoc::status, IndicateDarknessComponent, StatusIndicateDarkness)

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
    [[bg3::legacy(field_0)]] FixedString Status;
    int field_4;
    [[bg3::legacy(field_8)]] HashSet<EntityHandle> Targets;
};

struct AuraContainerComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerStatusAuraContainer, "esv::status::aura::ContainerComponent")

    Array<Aura> Auras;
};

struct CreateStatusRequest
{
    FixedString StatusId;
    ComponentHandle StatusHandle;
    bool byte10{ false };
    bool SavegameLoading{ false };
    uint8_t byte12{ 0 };
    std::optional<SpellId> Spell;
    EntityHandle Owner;
    EntityHandle SourceEquippedItem;
    float Duration{ -1.0f };
    EntityHandle SourceUsedItem;
    int32_t StoryActionId{ 0 };
    FixedString DifficultyStatus;
    bool ApplyKnockedOut{ false };
    bool KeepAlive{ false };
    bool Force{ false };
    EntityHandle Cause;
    CauseType CauseType{ CauseType::None };
};

struct DestroyStatusRequest
{
    DestroyStatusRequestType Type{ DestroyStatusRequestType::StatusType };
    EntityHandle Owner;
    FixedString StatusCause;
    EntityHandle CauseHandle;
    FixedString StatusId;
    StatusType StatusType{ 0 };
    EntityHandle Source;
    EntityHandle Status;
    ComponentHandle StatusHandle;
    FixedString DifficultyStatus;
    Guid Transform;
    uint64_t ExcludePropertyFlags{ 0 };
};

struct DestroyedEventRequest
{
    FixedString StatusId;
    ComponentHandle StatusHandle;
    EntityHandle Target;
    EntityHandle Status;
    EntityHandle Source;
    EntityHandle field_28;
    Guid CauseGuid;
    Guid field_40;
    Guid CastSource;
    Guid SpellCastGuid;
    int StoryActionId;
    StatusType Type;
    bool IsFromItem;
    bool IsUnique;
    bool HasTriedEntering;
    bool IsDeleting;
    EntityHandle field_80;
};

struct OnStatusEventRequest
{
    EntityHandle Subject;
    uint32_t Event{ 0 };
    HitDesc Hit;
    AttackDesc Attack;
    EntityHandle field_1E0;
};

struct OnStatusTurnStartRequest
{
    ComponentHandle Status;
    EntityHandle Owner;
};

struct CreatedEventRequest
{
    ComponentHandle Status;
    FixedString StatusId;
    EntityHandle Owner;
    bool field_18{ false };
    bool SavegameLoading{ false };
    std::optional<SpellId> Spell;
};

struct AttemptedEventRequest
{
    ComponentHandle Status;
    StatusType Type{ 0 };
    int32_t StoryActionId{ 0 };
    EntityHandle Owner;
    EntityHandle Source;
    FixedString StatusId;
    Guid Cause;
};

struct AppliedEventRequest
{
    ComponentHandle Status;
    EntityHandle Owner;
    FixedString StatusID;
    StatusType Type{ 0 };
};

struct RefreshRequest
{
    ComponentHandle Status;
    ComponentHandle RefreshedStatus;
    Guid Cause;
    int32_t StoryActionId{ 0 };
};

struct StatusRequestsComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerStatusRequests, "esv::status::StatusRequestsComponent")

    Array<CreateStatusRequest> Create;
    HashSet<ComponentHandle> Add;
    Array<DestroyStatusRequest> Destroy;
    HashSet<EntityHandle> Deactivate;
    HashSet<EntityHandle> Activate;
    HashSet<EntityHandle> UnregisterMachineUpdate;
    HashSet<EntityHandle> RegisterMachineUpdate;
    HashSet<EntityHandle> UpdateLifetime;
    Array<OnStatusEventRequest> OnEvent;
    Array<OnStatusTurnStartRequest> OnTurnStart;
    Array<CreatedEventRequest> CreatedEvent;
    Array<AttemptedEventRequest> AttemptedEvent;
    Array<AppliedEventRequest> AppliedEvent;
    Array<RefreshRequest> Refresh;
    Array<DestroyedEventRequest> DestroyedEvent;
    // Editor only
    // bool ActivateStatusVisuals;
    // bool DeactivateStatusVisuals;
};

struct AddedStatusAuraEffectEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ServerAddedStatusAuraEffectEvent, "esv::status::aura::AddedStatusAuraEffectEventOneFrameComponent")

    FixedString StatusId;
    int StoryActionId;
    EntityHandle Source;
    EntityHandle Subject;
};

struct RemovedStatusAuraEffectEventOneFrameComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerRemovedStatusAuraEffectEvent, "esv::status::aura::RemovedStatusAuraEffectEventOneFrameComponent")

    FixedString StatusId;
    int StoryActionId;
    EntityHandle Source;
    EntityHandle Subject;
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
    Guid field_0;
    int32_t StoryActionId;
    uint32_t field_14;
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
    bool IsFromItem;
    bool IsDeleting;
    Guid field_30;
    Guid CastSource;
    Guid SpellCastGuid;
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


struct RequestSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerStatusRequest, "esv::status::RequestSystem")

    [[bg3::hidden]] UnknownFunction field_10;
    [[bg3::hidden]] void* EntityManager;
    [[bg3::hidden]] void* Stats;
    [[bg3::hidden]] void* StatusPrototypeManager;
    [[bg3::hidden]] void* EncumbranceTypesManager;
    Array<CreateStatusRequest> Create;
    HashSet<ComponentHandle> Add;
    Array<DestroyStatusRequest> Destroy;
    Array<DestroyStatusRequest> Destroy2;
    Array<DestroyStatusRequest> Destroy3;
    HashMap<EntityHandle, uint8_t> Activate;
    HashMap<EntityHandle, uint8_t> Deactivate;
    HashMap<EntityHandle, uint8_t> RegisterMachineUpdate;
    HashMap<EntityHandle, uint8_t> UnregisterMachineUpdate;
    Array<OnStatusEventRequest> OnEvent;
    HashSet<EntityHandle> UpdateLifetime;
    Array<OnStatusTurnStartRequest> OnTurnStart;
    HashSet<EntityHandle> ApplyDefault;
    Array<CreatedEventRequest> CreatedEvent;
    Array<AttemptedEventRequest> AttemptedEvent;
    Array<AppliedEventRequest> AppliedEvent;
    Array<RefreshRequest> Refresh;
    Array<DestroyedEventRequest> DestroyedEvent;
    HashMap<EntityHandle, uint32_t> StatusEvents;
    // Editor only
    // HashMap<EntityHandle, FixedString> field_2F0;
    // bool ActivateStatusVisuals;
    // bool DeactivateStatusVisuals;
};


END_NS()
