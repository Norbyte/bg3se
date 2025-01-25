#pragma once

#include <GameDefinitions/Controllers/Controller.h>

BEGIN_NS(esv)

struct MoveTask : public Task
{
    virtual void OnArrive() = 0;
    virtual void* GetBehavior() = 0;
    virtual bool GetTarget(glm::vec3&) = 0;

    STDString ArriveEvent;
    [[bg3::hidden]] Array<void*> MoveTaskElement;
    glm::vec3 CurrentTarget;
    float TimeSpent;
    float LastTimeSpent;
    int BehaviorTransactionId;
    int StoryTransactionId;
    uint8_t field_7C;
    bool AvoidAoO;
    Array<EntityHandle> IgnoreEntities;
};

struct OsirisMoveToLocationTask : public MoveTask
{
    static constexpr auto Type = OsirisTaskType::MoveToLocation;

    float TargetRotation;
    float MinDistance;
    float MaxDistance;
    EntityHandle TargetToIgnore;
    bool TargetRotationSet;
};

struct OsirisMoveToObjectTask : public MoveTask
{
    static constexpr auto Type = OsirisTaskType::MoveToObject;

    float MinDistance;
    float MaxDistance;
    EntityHandle Target;
    float SpeedMultiplier;
    float DefaultSpeed;
    bool IncreaseSpeed;
};

struct OsirisFleeFromGridTask : public MoveTask
{
    static constexpr auto Type = OsirisTaskType::FleeFromGrid;

    glm::vec3 Position;
    bool field_9C;
    float Range;
};

struct OsirisMoveInRangeTask : public MoveTask
{
    static constexpr auto Type = OsirisTaskType::MoveInRange;

    EntityHandle Target;
    glm::vec3 TargetPos;
    float MinRange;
    float WantedRange;
    float MaxRange;
    EntityHandle MoveToTarget;
    glm::vec3 MoveToTargetPos;
    float MoveToMinRange;
    float MoveToWantedRange;
    float MoveToMaxRange;
    EntityHandle WeaponHandle;
    glm::vec3 CachedTargetPos;
    glm::vec3 CachedTarget;
    float CachedCloseEnough;
    bool CachedResult;
    bool MustBeInTrigger;
    bool FallbackMoveCloser;
    bool OnlyUseMoveSpellIfRequired;
    uint8_t field_F8;
    glm::vec3 OverrideSourcePosition;
    [[bg3::hidden]] Array<void*> pAIHintAreaTrigger;
    SpellId Spell;
    stats::SpellPrototype* SpellPrototype;
    __int64 field_148;
    __int64 field_150;
    uint8_t field_158;
    __int64 field_160;
    uint8_t field_168;
};

struct OsirisTeleportToLocationTask : public Task
{
    static constexpr auto Type = OsirisTaskType::TeleportToLocation;

    glm::vec3 Position;
    FixedString PreviousLevel;
    FixedString Level;
    bool SetRotation;
    bool LeaveCombat;
    bool FindPosition;
    bool FindFleePosition;
    bool Executed;
    uint8_t field_51;
    bool UnchainFollowers;
    bool SnapToGround;
    bool AllowUnloadedLevel;
    glm::quat Rotation;
    STDString ArriveEvent;
};

struct OsirisAppearTask : public Task
{
    static constexpr auto Type = OsirisTaskType::Appear;

    STDString FinishedEvent;
    FixedString Animation;
    EntityHandle Target;
    glm::vec3 TargetPos;
    float Angle;
    float Time;
    int SpawnState;
    bool PlayerSpawn;
    bool OutOfSight;
    bool OnTrail;
    bool PreventForceComplete;
    bool InvisibilityUpdated;
};

struct OsirisDisappearTask : public Task
{
    static constexpr auto Type = OsirisTaskType::Disappear;

    STDString FinishedEvent;
    glm::vec3 TargetPos;
    float Angle;
    int BehaviourTransactionId;
    int StoryTransactionId;
    float SpeedMultiplier;
    float DefaultSpeed;
    int field_70;
    bool IncreaseSpeed;
    bool OutOfSight;
    bool Offstage;
    bool ValidTarget;
};

struct OsirisFollowNPCTask : public Task
{
    static constexpr auto Type = OsirisTaskType::FollowNPC;

    EntityHandle Target;
};

struct OsirisFollowOwnerOrLeaderTask : public Task
{
    static constexpr auto Type = OsirisTaskType::FollowOwnerOrLeader;

    EntityHandle Leader;
    int BehaviourTransactionId;
};

struct OsirisWanderTask : public Task
{
    static constexpr auto Type = OsirisTaskType::Wander;

    float Range;
    EntityHandle Trigger;
    std::variant<EntityHandle, glm::vec3> Anchor;
    float Duration;
    glm::vec3 Start;
    int BehaviourTransactionId;
};

struct OsirisSteerTask : public Task
{
    static constexpr auto Type = OsirisTaskType::Steer;

    EntityHandle Target;
    glm::vec3 TargetPos;
    float AngleTolerance;
    bool LookAt;
    bool SnapToTarget;
    int BehaviourTransactionId;
};

struct OsirisDropTask : public Task
{
    static constexpr auto Type = OsirisTaskType::Drop;

    EntityHandle Item;
    glm::vec3 TargetPos;
    std::optional<glm::vec3> DesiredPosition;
};

struct OsirisPickupItemTask : public Task
{
    static constexpr auto Type = OsirisTaskType::PickupItem;

    EntityHandle Item;
    STDString ArriveEvent;
    int BehaviourTransactionId;
    bool MoveAvoidAoO;
};

struct OsirisUseItemTask : public Task
{
    static constexpr auto Type = OsirisTaskType::UseItem;

    EntityHandle Item;
    bool IsInteraction;
    bool UseItem;
    bool MoveAvoidAoO;
    STDString ArriveEvent;
    int BehaviourTransactionId;
};

struct OsirisMoveItemTask : public Task
{
    static constexpr auto Type = OsirisTaskType::MoveItem;

    EntityHandle Item;
    glm::vec3 Position;
    int Amount;
    STDString ArriveEvent;
    int BehaviourTransactionId;
    bool SplitProcessed;
    bool CheckSplitEvent;
    bool CheckRemovedFromInventory;
};

struct OsirisResurrectTask : public Task
{
    static constexpr auto Type = OsirisTaskType::Resurrect;

    float Chance;
    float HPPercentage;
    bool IsResurrected;
    FixedString Animation;
};

struct OsirisUseSpellTask : public Task
{
    static constexpr auto Type = OsirisTaskType::UseSpell;

    SpellId Spell;
    glm::vec3 CastPosition;
    EntityHandle Target;
    glm::vec3 TargetPos;
    bool HasTargetPos;
    EntityHandle Target2;
    glm::vec3 TargetPos2;
    bool HasTargetPos2;
    EntityHandle Item;
    bool Success;
    uint8_t field_A9;
    uint8_t State;
    bool WithoutMove;
    bool MoveAvoidAoO;
    uint8_t SpellCastOptions;
    ActionOriginator Originator;
    Guid SpellCastGuid;
};

struct OsirisMoveToAndTalkTask : public Task
{
    static constexpr auto Type = OsirisTaskType::MoveToAndTalk;

    EntityHandle Target;
    int BehaviourTransactionId;
    FixedString Speed;
    FixedString DialogInstance;
    float Timeout;
};

struct OsirisLookAtTask : public Task
{
    static constexpr auto Type = OsirisTaskType::LookAt;

    Guid TransactionGuid;
    EntityHandle EntityHandle;
    FixedString Bone;
    float Duration;
    int TransactionId;
};

struct OsirisTimerTask : public Task
{
    static constexpr auto Type = OsirisTaskType::Timer;

    float Time;
};

struct OsirisAutomatedDialogTask : public Task
{
    static constexpr auto Type = OsirisTaskType::AutomatedDialog;

    [[bg3::hidden]] void* DialogEventListenerVMT;
    Guid Dialog;
    Guid Trigger;
    Array<EntityHandle> SpeakerList;
    bool WaitForCompletion;
    int DialogInstanceId;
};

struct OsirisRateLimitedAutomatedDialogTask : public Task
{
    static constexpr auto Type = OsirisTaskType::RateLimitedAutomatedDialog;

    Guid Dialog;
    bool WaitForCompletion;
    Array<EntityHandle> SpeakerList;
    float MinDelay;
    float CullingDistance;
    Guid AdHistoryId;
};

struct OsirisFleeFromRelationTask : public MoveTask
{
    static constexpr auto Type = OsirisTaskType::FleeFromRelation;

    glm::vec3 Position;
    bool field_9C;
    float FleeRange;
    int field_A4;
    int FleeFromRelation;
};

struct OsirisFleeFromEntityTask : public MoveTask
{
    static constexpr auto Type = OsirisTaskType::FleeFromEntity;

    glm::vec3 Position;
    bool field_9C;
    float FleeRange;
    int field_A4;
    EntityHandle FleeFromEntity;
};

struct OsirisCombineTask : public Task
{
    static constexpr auto Type = OsirisTaskType::Combine;

    EntityHandle CombineTarget;
    Array<EntityHandle> CombineIngredients;
    int CombineAmount;
};


END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(esv::Task);

END_NS()
