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
    virtual void SetStatusSpecificParam1(uint32_t, uint32_t) = 0;
    virtual void SetStatusSpecificParam2(FixedString const&) = 0;
    virtual void VMT07() = 0;
    virtual uint32_t VMT08() = 0;
    virtual bool CanEnter() = 0;
    virtual void Init() = 0;
    virtual void InitStaticProperties() = 0;
    virtual bool Enter() = 0;
    virtual bool Resume() = 0;
    virtual void Update(GameTime const&) = 0;
    virtual void Tick(Guid const& combatGuid, float deltaTime, uint32_t type) = 0;
    virtual void Exit() = 0;
    virtual bool VMT17() = 0;
    virtual bool DisablesTarget() = 0;
    virtual void PrepareForLevelUnloading() = 0;
    virtual EntityHandle GetTarget() const = 0;
    virtual uint32_t OnStatusEvent(uint32_t event, HitDesc const& hit, AttackDesc const& attack, FixedString const&, uint32_t removeCause) = 0;
    virtual bool GetSyncData(ScratchBuffer&) = 0;
    virtual bool VMT23() = 0;
    virtual bool SavegameVisit(ObjectVisitor*) = 0;
    virtual bool VMT25() = 0;
    virtual bool Activate() = 0;
    virtual void Deactivate() = 0;
    virtual void OnSetRequestDelete(bool) = 0;

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
    [[bg3::flags]] StatusFlags Flags;
    [[bg3::flags]] StatusFlags2 Flags2;
    [[bg3::flags]] StatusFlags3 Flags3;
    bool FreezeDuration;
    uint8_t ExitCause;
    [[bg3::legacy(field_105)]] SurfaceType SourceSurface;
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
    uint32_t Time;
    FixedString StatusId;
};

struct Aura : public ProtectedGameObject<Aura>
{
    [[bg3::hidden]] void* VMT;
    float Radius;
    stats::Functors* Functors;
    uint8_t Flags;
    Array<Array<EntityAuraStatus>> EntityAuraStatuses;
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
    static constexpr auto Type = StatusType::BOOST;

    Array<FixedString> Spell;
    Array<FixedString> Items;
    bool LoseControl;
    Array<EntityHandle> ItemHandles;
    float EffectTime;
    glm::vec3 SourceDirection;
    Array<uint32_t> SurfaceChanges;
    bool HasStatusEffectOnTurn;
};

struct StatusClimbing : public Status
{
    static constexpr auto Type = StatusType::CLIMBING;

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
    static constexpr auto Type = StatusType::INCAPACITATED;

    float CurrentFreezeTime;
    float FreezeTime;
    uint8_t IncapacitateFlags;
};

struct StatusDowned : public StatusIncapacitated
{
    static constexpr auto Type = StatusType::DOWNED;

    int NumStableSuccess;
    int NumStableFailed;
    int StableRollDC;
    StatsExpressionRef StableRoll;
    stats::Functors* OnRollsFailed;
    stats::Functors* OnSuccess;
    [[bg3::hidden]] UnknownFunction RollSignals;
    bool IsStable;
    bool IsHealed;
    int RollSuccesses;
    int RollFailures;
    int DamageFailures;
};

struct StatusDying : public Status
{
    static constexpr auto Type = StatusType::DYING;
};

struct StatusHeal : public Status
{
    static constexpr auto Type = StatusType::HEAL;

    float EffectTime;
    int HealAmount;
    int HealEffect;
    FixedString HealEffectId;
    int TargetHealth;
};

struct SurfaceLayerCheck
{
    uint8_t EvaluateSurfaceApplyTypes;
    float SurfaceDistanceCheck;
    float SurfaceOnMoveDistanceMultiplier;
    int OnMoveCount;
    uint8_t field_10;
    bool FullyEntered;
    bool Inside;
    glm::vec3 Translate;
    int field_20;
};

struct SurfaceTypeCheck
{
    float TurnTimerCheck;
    HashSet<EntityHandle> Entities;
};

struct StatusInSurface : public Status
{
    static constexpr auto Type = StatusType::INSURFACE;

    HashMap<EntityHandle, std::array<SurfaceLayerCheck, 2>> TargetData;
    HashMap<SurfaceType, SurfaceTypeCheck> TypeData;
    HashMap<FixedString, bool> AlreadyAppliedStatuses;
};

struct StatusInvisible : public StatusBoost
{
    static constexpr auto Type = StatusType::INVISIBLE;

    glm::vec3 InvisiblePosition;
};

struct StatusKnockedDown : public Status
{
    static constexpr auto Type = StatusType::KNOCKED_DOWN;
};

struct StatusMaterial : public Status
{
    static constexpr auto Type = StatusType::MATERIAL;

    FixedString MaterialUUID;
    uint8_t ApplyFlags;
    bool IsOverlayMaterial;
    bool Fading;
    bool ApplyNormalMap;
    bool Force;
};

struct StatusPolymorphed : public StatusBoost
{
    static constexpr auto Type = StatusType::POLYMORPHED;

    Guid Id;
};

struct StatusReaction : public Status
{
    static constexpr auto Type = StatusType::REACTION;

    ecs::EntityRef Source;
    ecs::EntityRef Target;
    glm::vec3 TargetPosition;
    ComponentHandle PartnerStatus;
    SpellId Spell;
    uint8_t UpdatesSinceSpellCastRequest;
    bool IgnoreHasSpell;
    bool IgnoreChecks;
    bool FromInterrupt;
    bool ShowOverhead;
    bool CheckProjectileTargets;
};

struct StatusStoryFrozen : public Status
{
    static constexpr auto Type = StatusType::STORY_FROZEN;

};

struct StatusRotate : public Status
{
    static constexpr auto Type = StatusType::ROTATE;

    float Yaw;
    float RotationSpeed;
};

struct StatusTeleportFalling : public Status
{
    static constexpr auto Type = StatusType::TELEPORT_FALLING;

    glm::vec3 Target;
    float ReappearTime;
    SpellId Spell;
    bool HasDamage;
    bool HasDamageBeenApplied;
};

struct StatusUnlock : public Status
{
    static constexpr auto Type = StatusType::UNLOCK;

    EntityHandle Source;
    FixedString field_128;
    bool Success;
    int Unlocked;
};

struct StatusFear : public StatusBoost
{
    static constexpr auto Type = StatusType::FEAR;

};

struct StatusSmelly : public Status
{
    static constexpr auto Type = StatusType::SMELLY;

};

struct StatusSneaking : public StatusBoost
{
    static constexpr auto Type = StatusType::SNEAKING;

    bool ClientRequestStop;
};

struct StatusEffect : public Status
{
    static constexpr auto Type = StatusType::EFFECT;

};

struct StatusDeactivated : public StatusBoost
{
    static constexpr auto Type = StatusType::DEACTIVATED;

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
    HashMap<FixedString, Status*> field_50;
    uint32_t field_90;
    uint32_t field_94;
    UnknownFunction field_98;
    void* field_D8;
    Array<void*> RemoveStatusRequests;
};

END_NS()


BEGIN_NS(ecl)

struct [[bg3::hidden]] StatusVFXVMT
{
    using DestroyProc = void(StatusVFXVMT*, bool);
    using UpdateVisualsProc = void(StatusVFXVMT*);
    using RecreateVisualsProc = void(StatusVFXVMT*, void*, bool);
    using DestroyVisualsProc = void(StatusVFXVMT*);
    using ApplyProc = void(StatusVFXVMT*, void*);

    DestroyProc* Destroy;
    UpdateVisualsProc* UpdateVisuals;
    RecreateVisualsProc* RecreateVisuals;
    DestroyVisualsProc* DestroyVisuals;
    ApplyProc* Apply;
};

struct StatusVFX : public ProtectedGameObject<StatusVFX>
{
    [[bg3::hidden]] StatusVFXVMT* VMT;
    ComponentHandle Status;
};

struct ManagedStatusEffect
{
    Guid Group;
    uint8_t Type;
};


struct Status : public ProtectedGameObject<Status>
{
    virtual ~Status() = 0;
    virtual StatusType GetStatusId() = 0;
    virtual uint32_t GetStatusType() = 0;
    virtual void Init() = 0;
    virtual bool Enter() = 0;
    virtual bool Resume() = 0;
    virtual void Update(GameTime const&) = 0;
    virtual bool RequiresTick() = 0;
    virtual void Tick(Guid const& combatGuid, float deltaTime) = 0;
    virtual void Exit() = 0;
    virtual void VMT10() = 0;
    virtual FixedString GetIcon() const = 0;
    virtual int GetLevel() const = 0;
    virtual void CollectEffects(Array<StatusVFX*>) = 0;
    virtual bool GetManagedStatusEffectGroup(ManagedStatusEffect&) = 0;
    virtual void ApplyEndEffect(void*) = 0;
    virtual bool ShouldShowOverhead() const = 0;
    virtual bool HasVisuals() const = 0;
    virtual bool SyncData(ScratchBuffer&) = 0;
    virtual void DestroyVisuals() = 0;
    virtual void VMT20() = 0;
    virtual void RecreateVisuals() = 0;
    virtual void VMT22() = 0;
    virtual void OnSetRequestDelete(bool) = 0;

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
    static constexpr auto Type = StatusType::BOOST;

    //bool HasData;
    FixedString Icon;
    StatusEffectData BoostEffect;
    StatusEffectData EffectOnTurn;
    ManagedStatusEffect ManagedStatusEffect;
    FixedString ApplyEffect;
    FixedString EndEffect;
    StatusVFXBoostMaterialDefinition Material;
    int field_12C;
    StatusVisualDefinition VisualDefinition;
    //bool NeedsEffectStart;
    EffectHandler EffectHandler;
};

struct StatusSneaking : public StatusBoost
{
    static constexpr auto Type = StatusType::SNEAKING;

    bool SneakingFailed;
    bool SneakingFailed2;
    bool IsPlayer;
    EntityHandle Effect;
    uint8_t Obscurity;
};

struct StatusInvisible : public StatusBoost
{
    static constexpr auto Type = StatusType::INVISIBLE;

};

struct StatusIncapacitated : public StatusBoost
{
    static constexpr auto Type = StatusType::INCAPACITATED;

    float field_2B0;
    float field_2B4;
};

struct StatusDowned : public StatusIncapacitated
{
    static constexpr auto Type = StatusType::DOWNED;

    uint8_t field_2B8;
    uint8_t field_2B9;
    uint8_t field_2BA;
    int field_2BC;
    int field_2C0;
    int field_2C4;
    uint8_t Entered;
    uint8_t DownedFlags;
};

struct StatusFear : public StatusBoost
{
    static constexpr auto Type = StatusType::FEAR;
};

struct StatusInSurface : public Status
{
    static constexpr auto Type = StatusType::INSURFACE;
};

struct StatusPolymorphed : public StatusBoost
{
    static constexpr auto Type = StatusType::POLYMORPHED;
};

struct StatusDeactivated : public StatusBoost
{
    static constexpr auto Type = StatusType::DEACTIVATED;
};

struct StatusUnlock : public Status
{
    static constexpr auto Type = StatusType::UNLOCK;

    int State;
};

struct StatusTeleportFalling : public Status
{
    static constexpr auto Type = StatusType::TELEPORT_FALLING;

    glm::vec3 field_98;
    float FallTimer;
    SpellId TeleportSpell;
    uint8_t field_D0;
    EffectHandler EffectHandler;
};

struct StatusStoryFrozen : public Status
{
    static constexpr auto Type = StatusType::STORY_FROZEN;
};

struct StatusSmelly : public Status
{
    static constexpr auto Type = StatusType::SMELLY;

    StatusEffectData Effect;
    StatusVFXData Material;
    __int64 field_C0;
};

struct StatusRotate : public Status
{
    static constexpr auto Type = StatusType::ROTATE;

    float field_98;
    float field_9C;
};

struct StatusReaction : public Status
{
    static constexpr auto Type = StatusType::REACTION;

    ecs::EntityRef ReactionSource;
    ecs::EntityRef ReactionTarget;
    uint8_t field_B8;
};

struct StatusMaterial : public Status
{
    static constexpr auto Type = StatusType::MATERIAL;

    ecl::StatusVFXData VFX;
};

struct StatusKnockedDown : public Status
{
    static constexpr auto Type = StatusType::KNOCKED_DOWN;

    StatusEffectData Effect;
};

struct StatusHeal : public Status
{
    static constexpr auto Type = StatusType::HEAL;

    float HealTimer;
    int HealAmount;
    int HealEffect;
    int field_A4;
    StatusEffectData Effect;
};

struct StatusBeamEffectData
{
    FixedString BeamEffect;
    EntityHandle Source;
};

struct StatusEffect : public Status
{
    static constexpr auto Type = StatusType::EFFECT;

    bool IsPlayer;
    bool PlayerSameParty;
    bool PeaceOnly;
    FixedString PlayerTag;
    StatusEffectData Effect;
    StatusBeamEffectData BeamEffect;
};

struct StatusDying : public Status
{
    static constexpr auto Type = StatusType::DYING;
};

struct StatusClimbing : public Status
{
    static constexpr auto Type = StatusType::CLIMBING;

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


struct EntityStatusData
{
    Array<StatusVFX*> VFX;
    bool HasVFX1;
    bool HasVFX2;
};


struct ManagedStatusFXGroup
{
    int References0;
    int References1;
    uint8_t Type;
    StatusVFX* VFX;
};


struct ManagedStatusFXs
{
    HashMap<Guid, ManagedStatusFXGroup> FXs;
    bool field_40;
};


struct [[bg3::hidden]] StatusMachine : public NetworkObjectFactory2
{
    void* VMT2;
    EntityHandle OwnerHandle;
    uint8_t Flags;
    HashMap<EntityHandle, EntityStatusData> StatusFX;
    HashMap<EntityHandle, ManagedStatusFXs> ManagedFX;
    HashSet<ComponentHandle> StatusHandles;
    void* unk;
    Array<Status*> Statuses;
    Array<Status*> ExternalStatuses;
    uint32_t unk2;
};


struct ManagedStatusVFX : public StatusVFX
{
    ManagedStatusEffect ManagedStatusEffect;
    EffectHandler EffectHandler;
};

struct StatusVisual : public StatusVFX
{
    StatusVisualDefinition* Definition;
    EntityHandle Entity;
    uint8_t State;
    [[bg3::hidden]] void* VisualLoader;
    float FadeOut;
};

struct StatusVFXMaterial : public StatusVFX
{
    ecl::StatusVFXData* VFX;
    bool Created;
};

struct StatusVFXEffect : public StatusVFX
{
    StatusEffectData* Effect;
    EffectHandler EffectHandler;
};

struct StatusVFXApplyEffect : public StatusVFX
{
    FixedString* Effect;
    EffectHandler EffectHandler;
};

struct StatusVFXBoostMaterial : public StatusVFX
{
    StatusVFXBoostMaterialDefinition* Material;
    int MaterialType;
    FixedString MaterialUUID;
    bool Created;
};

struct StatusVFXBeam : public StatusVFX
{
    StatusBeamEffectData* Beam;
    EffectHandler EffectHandler;
    bool Initialized;
    bool Created;
};

struct StatusVFXAura : public StatusVFX
{
    StatusAuraData* Aura;
    EntityHandle Effect;
};


END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(esv::Status)
LUA_POLYMORPHIC(ecl::Status)
LUA_POLYMORPHIC(ecl::StatusVFX)

END_NS()
