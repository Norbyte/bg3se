#pragma once

#include <GameDefinitions/Controller.h>

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
    float TargetRotation;
    float MinDistance;
    float MaxDistance;
    EntityHandle TargetToIgnore;
    bool TargetRotationSet;
};

struct OsirisMoveToObjectTask : public MoveTask
{
    float MinDistance;
    float MaxDistance;
    EntityHandle Target;
    float SpeedMultiplier;
    float DefaultSpeed;
    bool IncreaseSpeed;
};

struct OsirisFleeFromGridTask : public MoveTask
{
    glm::vec3 field_90;
    uint8_t field_9C;
    float Range;
};

struct OsirisMoveInRangeTask : public MoveTask
{
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
    char MustBeInTrigger;
    char FallbackMoveCloser;
    uint8_t field_F7;
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
    STDString FinishedEvent;
    FixedString Animation;
    EntityHandle Target;
    glm::vec3 TargetPos;
    float Angle;
    int field_70;
    int SpawnState;
    bool PlayerSpawn;
    bool OutOfSight;
    bool OnTrail;
    bool PreventForceComplete;
    uint8_t field_7C;
};

struct OsirisDisappearTask : public Task
{
    STDString FinishedEvent;
    glm::vec3 TargetPos;
    float Angle;
    int field_60;
    int StoryTransactionID;
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
    EntityHandle Target;
};

struct OsirisFollowOwnerOrLeaderTask : public Task
{
    EntityHandle Leader;
    int field_40;
};

struct OsirisWanderTask : public Task
{
    float Range;
    EntityHandle Trigger;
    std::variant<EntityHandle, glm::vec3> Anchor;
    float Duration;
    glm::vec3 Start;
    int field_68;
};

struct OsirisSteerTask : public Task
{
    EntityHandle Target;
    glm::vec3 TargetPos;
    float AngleTolerance;
    bool LookAt;
    bool SnapToTarget;
    int field_54;
};

struct OsirisDropTask : public Task
{
    EntityHandle Item;
    glm::vec3 TargetPos;
    std::optional<glm::vec3> DesiredPosition;
};

struct OsirisPickupItemTask : public Task
{
    EntityHandle Item;
    STDString ArriveEvent;
    int field_58;
    bool MoveAvoidAoO;
};

struct OsirisUseItemTask : public Task
{
    EntityHandle Item;
    bool IsInteraction;
    bool UseItem;
    bool MoveAvoidAoO;
    STDString ArriveEvent;
    int field_60;
};

struct OsirisMoveItemTask : public Task
{
    EntityHandle Item;
    glm::vec3 Position;
    int Amount;
    STDString ArriveEvent;
    int field_68;
    bool SplitProcessed;
    bool CheckSplitEvent;
    bool CheckRemovedFromInventory;
};

struct OsirisResurrectTask : public Task
{
    float Chance;
    float HPPercentage;
    bool IsResurrected;
    FixedString Animation;
};

struct OsirisUseSpellTask : public Task
{
    SpellId Spell;
    glm::vec3 CastPosition;
    EntityHandle Target;
    glm::vec3 TargetPos;
    char field_84;
    EntityHandle Target2;
    glm::vec3 TargetPos2;
    uint8_t field_9C;
    EntityHandle Item;
    bool Success;
    uint8_t field_A9;
    uint8_t field_AA;
    bool WithoutMove;
    bool MoveAvoidAoO;
    uint8_t SpellCastOptions;
    ActionOriginator Originator;
    Guid SpellCastGuid;
};

struct OsirisMoveToAndTalkTask : public Task
{
    EntityHandle Target;
    int field_40;
    FixedString Speed;
    FixedString DialogInstance;
    float Timeout;
};

struct OsirisLookAtTask : public Task
{
    Guid TransactionGuid;
    EntityHandle EntityHandle;
    FixedString Bone;
    float Duration;
    int TransactionId;
};

struct OsirisTimerTask : public Task
{
    float Time;
};

struct OsirisAutomatedDialogTask : public Task
{
    [[bg3::hidden]] void* DialogEventListenerVMT;
    Guid Dialog;
    Guid Trigger;
    Array<EntityHandle> SpeakerList;
    bool WaitForCompletion;
    int DialogInstanceId;
};

struct OsirisRateLimitedAutomatedDialogTask : public Task
{
    Guid Dialog;
    bool WaitForCompletion;
    Array<EntityHandle> SpeakerList;
    float MinDelay;
    float CullingDistance;
    Guid AdHistoryId;
};

struct OsirisFleeFromRelationTask : public MoveTask
{
    glm::vec3 Position;
    bool field_9C;
    float field_A0;
    int field_A4;
    int FleeFromRelation;
};

struct OsirisFleeFromEntityTask : public MoveTask
{
    glm::vec3 Position;
    bool field_9C;
    float field_A0;
    int field_A4;
    EntityHandle FleeFromEntity;
};

struct OsirisCombineTask : public Task
{
    EntityHandle CombineTarget;
    Array<EntityHandle> CombineIngredients;
    int CombineAmount;
};


END_NS()
