#pragma once

#include <GameDefinitions/Effect.h>

BEGIN_NS(esv)

struct Status : public ProtectedGameObject<Status>
{
    virtual ~Status() = 0;
    virtual void SetHandle(uint64_t handle) = 0;
    virtual ComponentHandle GetHandle(uint64_t handle) = 0;
    virtual StatusType GetStatusId() = 0;
    // 0 - Stackable
    // 1 - Apply only the first instance, discard new ones
    // 2 - Apply only one instance, new instances replace old ones
    // 3 - Apply only the first instance; triggers combat?
    virtual uint32_t GetStatusType() = 0;
    // TODO - map VMTs later

    Guid field_8;
    __int64 field_18;
    int StoryActionID;
    ActionOriginator Originator;
    double field_48;
    FixedString StatusId;
    float StartTimer;
    float LifeTime;
    float CurrentLifeTime;
    float TurnTimer;
    float Strength;
    uint8_t CauseType;
    uint8_t OriginCauseType;
    EntityHandle SourceEquippedItem;
    EntityHandle SourceUsedItem;
    ComponentHandle StatusHandle;
    EntityHandle SyncEntity;
    EntityHandle Owner;
    Array<EntityHandle> StatusOwner;
    ecs::EntityRef Cause;
    Guid CauseGUID;
    EntityHandle StatusSource;
    ecs::EntityRef RootCause;
    EntityHandle field_E0;
    Guid field_E8;
    stats::ConditionId RemoveConditions;
    uint32_t RemoveEvents;
    StatusFlags Flags;
    StatusFlags2 Flags2;
    StatusFlags3 Flags3;
    bool FreezeDuration;
    uint8_t ExitCause;
    uint8_t field_105;
    uint8_t SpellCastingAbility;
    SpellId SourceSpell;
    Guid SpellCastSourceUuid;
    FixedString StackId;
    int StackPriority;
    bool IsRecoverable;
    FixedString DifficultyStatus;
    Guid StackedSpellCastSourceUuid;
    ConditionRolls ConditionRolls;
    uint8_t TickType;
    StatusFlags4 Flags4;
    // Flags5 is gone in patch3?
    // StatusFlags5 Flags5;
};

struct EntityAuraStatus
{
    EntityHandle Entity;
    int StatusId;
};

struct Aura : public ProtectedGameObject<Aura>
{
    [[bg3::hidden]] void* VMT;
    float Radius;
    stats::Functors* Functors;
    uint8_t Flags;
    Array<EntityAuraStatus> EntityAuraStatuses;
    ecs::EntityRef Source;
    int32_t StoryActionId;
    Guid SpellCastSourceUuid;
};

struct StatusAura : public Status
{
    Aura* Aura;
};

struct StatusBoost : public StatusAura
{
    ObjectSet<FixedString> Spell;
    ObjectSet<FixedString> Items;
    bool LoseControl;
    ObjectSet<EntityHandle> ItemHandles;
    float EffectTime;
    FixedString BoostStackId;
    glm::vec3 SourceDirection;
    ObjectSet<uint32_t> SurfaceChanges;
};

struct StatusClimbing : public Status
{
    __int64 field_120;
    int field_128;
    int field_12C;
    glm::vec3 MoveDirection_M;
    int field_13C;
    __int64 field_140;
    EntityHandle Item;
    FixedString Level;
    uint8_t Status;
    bool Direction;
    bool JumpUpLadders_M;
    bool Incapacitated_M;
    bool Started_M;
};

struct StatusIncapacitated : public StatusBoost
{
    float CurrentFreezeTime;
    float FreezeTime;
    uint8_t IncapacitateFlags;
    bool IncapacitationAnimationFinished;
};

struct StatusDowned : public StatusIncapacitated
{
    int NumStableSuccess;
    int NumStableFailed;
    int StableRollDC;
    [[bg3::hidden]] void* StableRoll_LuaExpression;
    [[bg3::hidden]] void* OnRollsFailed;
    [[bg3::hidden]] void* OnSuccess;
    [[bg3::hidden]] void* Roll_M;
    bool IsStable;
    bool IsHealed;
    int RollSuccesses;
    int RollFailures;
    int DamageFailures;
};

struct StatusDying : public Status
{
    ecs::EntityRef Source;
    __int64 field_128;
    EntityHandle field_130;
    HitDesc HitDescription;
    Guid Combat;
    uint8_t DyingFlags;
};

struct StatusHeal : public Status
{
    float EffectTime;
    int HealAmount;
    int HealEffect;
    FixedString HealEffectId;
    uint8_t HealType; // TODO enum
    int AbsorbSurfaceRange;
    uint8_t TargetDependentHeal;
    ObjectSet<SurfaceType> AbsorbSurfaceTypes;
    int TargetDependentValue;
    int TargetDependentHealAmount;
};

struct SurfaceLayerCheck
{
    uint8_t EvaluateSurfaceApplyTypes;
    float TurnTimerCheck;
    float SurfaceOnMoveDistanceMultiplier;
    float SurfaceDistanceCheck;
    int OnMoveCount;
    bool FullyEntered;
    bool Inside;
    bool Unknown;
};

struct StatusInSurface : public Status
{
    std::array<SurfaceLayerCheck, 2> LayerChecks;
    glm::vec3 Translate;
    HashMap<FixedString, bool> Unknown;
};

struct StatusInvisible : public StatusBoost
{
    glm::vec3 InvisiblePosition;
};

struct StatusKnockedDown : public Status
{
    uint8_t KnockedDownState;
    bool IsInstant;
};

struct StatusMaterial : public Status
{
    FixedString MaterialUUID;
    uint8_t ApplyFlags;
    bool IsOverlayMaterial;
    bool Fading;
    bool ApplyNormalMap;
    bool Force;
};

struct StatusPolymorphed : public StatusBoost
{
    Guid Id;
};

struct StatusReaction : public Status
{
    ecs::EntityRef Source;
    ecs::EntityRef Target;
    glm::vec3 TargetPosition;
    ComponentHandle PartnerStatus;
    SpellId Spell;
    bool IgnoreHasSpell;
    bool IgnoreChecks;
    bool FromInterrupt;
    bool ShowOverhead;
    bool CheckProjectileTargets;
};

struct StatusStoryFrozen : public Status
{
};

struct StatusRotate : public Status
{
    float Yaw;
    float RotationSpeed;
};

struct StatusTeleportFalling : public Status
{
    glm::vec3 Target;
    float ReappearTime;
    SpellId Spell;
    bool HasDamage;
    bool HasDamageBeenApplied;
};

struct StatusUnlock : public Status
{
    EntityHandle Source;
    FixedString field_128;
    bool Success;
    int Unlocked;
};

struct StatusFear : public StatusBoost
{
};

struct StatusSmelly : public Status
{
};

struct StatusSneaking : public StatusBoost
{
    bool ClientRequestStop;
};

struct StatusEffect : public Status
{
};

struct StatusDeactivated : public StatusBoost
{
};


struct [[bg3::hidden]] StatusMachine
{
    using CreateStatusProc = Status* (StatusMachine* self, FixedString* statusId, uint64_t statusHandle, bool unkn1, bool unkn2);
    using ApplyStatusProc = Status* (StatusMachine* self, Status* status);

    Status* GetStatus(FixedString const& statusId) const;

    EntityHandle Owner;
    uint8_t field_18;
    Array<Status*> NewlyAddedStatuses;
    Array<Status*> Statuses;
    Array<Status*> CleanseStatuses;
    Array<Status*> UnknownStatuses;
    __int64 field_58;
    __int64 field_60;
    __int64 field_68;
    __int64 field_70;
    __int64 field_78;
    __int64 field_80;
    __int64 field_88;
    __int64 field_90;
    __int64 field_98;
    __int64 field_A0;
    Array<void*> RemoveStatusRequests;
};

END_NS()


BEGIN_NS(ecl)

struct Status : public ProtectedGameObject<Status>
{
    [[bg3::hidden]] void* VMT;
    Guid UUID;
    NetId NetworkId;
    EntityHandle Target;
    ComponentHandle StatusHandle;
    ecs::EntityRef Cause;
    ecs::EntityRef TickingOwner;
    FixedString StatusId;
    uint8_t TickType;
    uint8_t CauseType;
    SpellId Spell;
    int StillAnimationType;
    int StillAnimationPriority;
    float TickTime;
    float LifeTime;
    float TickTimeLeft;
    StatusFlags Flags;
    StatusFlags2 Flags2;
    bool DisableTicking;
};


struct StatusAuraData
{
    FixedString FX;
    float Radius;
};


struct StatusAura : public Status
{
    StatusAuraData Aura;
};

struct StatusEffectData
{
    Array<Guid> Tags;
    FixedString Effect;
};

struct ManagedStatusEffect
{
    Guid Group;
    uint8_t Type;
};

struct StatusMaterialParam
{
    FixedString Param;
    FixedString Value;
};

struct StatusVFXBoostMaterialDefinition
{
    uint32_t MaterialType;
    FixedString Material;
    uint8_t ApplyFlags;
    uint8_t ApplyNormalMap;
    float FadeAmount;
    float OverlayOffset;
    int field_14;
    Array<StatusMaterialParam> MaterialParams;
    int field_28;
    float field_2C;
    int field_30;
};

struct StatusVFXData
{
    FixedString Material;
    uint8_t ApplyFlags;
    bool IsOverlayMaterial;
    bool Fading;
    bool ApplyNormalMap;
    bool Force;
    float FadeTime;
};

struct StatusVisualDefinition
{
    uint64_t field_0;
    HashMap<FixedString, Array<FixedString>> Visuals;
    float FadeIn;
    float FadeOut;
};

struct StatusBoost : public StatusAura
{
    bool HasData;
    FixedString field_A4;
    StatusEffectData BoostEffect;
    StatusEffectData EffectOnTurn;
    ManagedStatusEffect ManagedStatusEffect;
    FixedString ApplyEffect;
    FixedString EndEffect;
    StatusVFXBoostMaterialDefinition Material;
    int field_12C;
    StatusVisualDefinition VisualDefinition;
    bool NeedsEffectStart;
    EffectHandler EffectHandler;
};

struct StatusSneaking : public StatusBoost
{
    bool SneakingFailed;
    bool SneakingFailed2;
    bool IsPlayer;
    EntityHandle Effect;
    uint8_t Obscurity;
};

struct StatusInvisible : public StatusBoost
{
};

struct StatusIncapacitated : public StatusBoost
{
};

struct StatusFear : public StatusBoost
{
};

struct StatusDeactivated : public StatusBoost
{
};

struct StatusUnlock : public Status
{
    int State;
};

struct StatusTeleportFalling : public Status
{
    glm::vec3 field_98;
    float FallTimer;
    SpellId TeleportSpell;
    uint8_t field_D0;
    EffectHandler EffectHandler;
};

struct StatusStoryFrozen : public Status
{
};

struct StatusSmelly : public Status
{
    StatusEffectData Effect;
    StatusVFXData Material;
    __int64 field_C0;
};

struct StatusRotate : public Status
{
    float field_98;
    float field_9C;
};

struct StatusReaction : public Status
{
    ecs::EntityRef ReactionSource;
    ecs::EntityRef ReactionTarget;
    uint8_t field_B8;
};

struct StatusMaterial : public Status
{
    ecl::StatusVFXData VFX;
};

struct StatusKnockedDown : public Status
{
    StatusEffectData Effect;
};

struct StatusHeal : public Status
{
    float HealTimer;
    int HealAmount;
    int HealEffect;
    int field_A4;
    StatusEffectData Effect;
};

struct StatusEffect : public Status
{
    bool IsPlayer;
    bool PlayerSameParty;
    bool PeaceOnly;
    FixedString PlayerTag;
    StatusEffectData Effect;
    FixedString BeamEffect;
    EntityHandle BeamSource;
};

struct StatusDying : public Status
{
};

struct StatusClimbing : public Status
{
    glm::quat Rotation;
    glm::vec3 MoveDirection;
    uint8_t State;
    EntityHandle Ladder;
    bool field_C0;
    uint8_t field_C1;
    uint8_t field_C2;
};


struct [[bg3::hidden]] ObjectFactory2 : public ProtectedGameObject<ObjectFactory2>
{
    __int64 VMT;
    Array<void*> Entries;
    LegacyArray<uint32_t> Salts_Arr_u32;
    Array<void*> field_38;
    Array<void*> Objects;
    int field_58;
    int HandleType;
};

struct [[bg3::hidden]] NetworkObjectFactory2 : public ObjectFactory2
{
    HashMap<Guid, void*> StatusesByGuid;
    Array<void*> field_A0;
    HashSet<uint64_t> NetIdQueue;
    LegacyArray<uint16_t> NetIdSalts;
    __int64 field_100;
    bool CanCreateNetIDs;
};


struct [[bg3::hidden]] StatusMachine : public NetworkObjectFactory2
{
    void* VMT2;
    EntityHandle OwnerHandle;
    uint8_t Flags;
    HashMap<EntityHandle, void*> StatusData_MHM_EH_EntityStatusData;
    HashMap<EntityHandle, void*> ManagedStatusFXs_MHM_EH_ManagedStatusFXs;
    HashSet<ComponentHandle> StatusHandles;
    void* unk;
    Array<Status*> Statuses;
    Array<Status*> ExternalStatuses;
    uint32_t unk2;
};


END_NS()

