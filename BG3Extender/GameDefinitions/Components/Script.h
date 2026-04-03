#pragma once

BEGIN_NS(anubis)

struct ConfigComponent : public BaseComponent
{
    DEFINE_COMPONENT(AnubisConfig, "ls::anubis::ConfigComponent")

    STDString field_0;
    std::optional<Array<GlobalConfigParameter>> Params;
    uint8_t field_30;
};

struct AnubisTreeComponent : public BaseComponent
{
    DEFINE_COMPONENT(AnubisTree, "ls::anubis::AnubisTreeComponent")

    int16_t field_0;
};

struct TreeComponent : public BaseComponent
{
    DEFINE_COMPONENT(AnubisTree2, "ls::anubis::TreeComponent")

    int32_t field_0;
};

struct AiMove
{
    glm::vec3 TargetPosition;
    EntityHandle TargetEntity;
    bool Success;
    bool ThrowExceptionOnFail;
    bool MoveAvoidAoO;
    Array<EntityHandle> IgnoreEntities;
};

struct Attack
{
    EntityOrVec3Variant Target;
    bool ForceSuccess;
    bool WithoutMove;
    int32_t FailReason;
};

struct FollowOwnerOrLeader
{
    bool Success;
};


struct Combine
{
    EntityHandle CombineTarget;
    Array<EntityHandle> CombineIngredients;
    int32_t CombineAmount;
    bool Success;
};

struct DropItem
{
    EntityHandle Item;
    bool Success;
};

struct DropItemTo
{
    EntityHandle Item;
    glm::vec3 TargetPosition;
    bool Success;
};

struct FleeFrom
{
    int32_t Relation;
    float Range;
    glm::vec3 Target;
    bool Success;
    bool ThrowExceptionOnFail;
};

struct FleeFromDangerousSurface
{
    glm::vec3 Target;
    bool Success;
    bool ThrowExceptionOnFail;
};

struct FleeFromEntity
{
    EntityHandle Source;
    float Range;
    glm::vec3 Target;
    bool Success;
    bool ThrowExceptionOnFail;
};

struct Follow
{
    EntityHandle Target1;
    EntityHandle Target2;
    bool Success;
};

struct LookAtEntity
{
    EntityHandle Target;
    float Duration;
};

struct LookFrom
{
    EntityOrVec3Variant Source;
    bool Snap;
};

struct MoveInRange
{
    EntityOrVec3Variant Target;
    float MinDistance;
    float MaxDistance;
    uint8_t PathMovementSpeed;
    bool StayInTriggers;
    Array<EntityHandle> AIHintTriggers;
    bool Success;
    bool ThrowExceptionOnFail;
    uint8_t FailReason;
};

struct MoveItem
{
    EntityHandle Item;
    glm::vec3 TargetPosition;
    int Amount;
    bool ForceSuccess;
    bool Success;
};


struct MoveTo
{
    EntityOrVec3Variant Target;
    float MinDistance;
    float MaxDistance;
    uint8_t PathClimbingMode;
    uint8_t PathDroppingMode;
    uint8_t PathMovementSpeed;
    bool ShouldArrive;
    bool LongPath;
    bool AvoidAoO;
    bool Success;
    bool ThrowExceptionOnFail;
    uint8_t FailReason;
};

struct PickUpItem
{
    EntityHandle Item;
    bool MoveAvoidAoO;
    bool FailReason;
};

struct PlayAnimation
{
    Array<STDString> StartAnimations;
    Array<STDString> LoopAnimations;
    Array<STDString> EndAnimations;
    bool WaitForCompletion;
    bool Success;
};

struct Sleep
{
    float Time;
};

struct StartAutomatedDialog
{
    Guid Dialog;
    Array<EntityHandle> SpeakerList;
    bool WaitForCompletion;
    bool Success;
    uint32_t ErrorCode;
};

struct StartAutomatedDialogRateLimited
{
    Guid Dialog;
    Guid FallbackDialog;
    float Delay;
    float CullingDistance;
    Array<EntityHandle> SpeakerList;
    bool WaitForCompletion;
    bool Success;
    uint32_t ErrorCode;
};

struct StartBehaviorDialog
{
    Guid Dialog;
    Guid TriggerId;
    Array<EntityHandle> SpeakerList;
    bool WaitForCompletion;
    bool Success;
    uint32_t ErrorCode;
};

struct SteerTo
{
    EntityOrVec3Variant Target;
    bool Snap;
    int32_t Angle;
};

struct UseItem
{
    EntityHandle Item;
    bool ShouldArrive;
    bool LongPath;
    bool Success;
    bool AvoidAoO;
    uint8_t FailReason;
};

struct UseSpell
{
    STDString SpellID;
    glm::vec3 CastPosition;
    EntityOrVec3Variant Target1;
    EntityOrVec3Variant Target2;
    EntityHandle Item;
    bool IgnoreHasSpell;
    bool IgnoreCastChecks;
    bool IgnoreTargetChecks;
    bool MoveAvoidAoO;
    bool FindCastPosition;
    int32_t FailReason;
};

struct Wander
{
};

struct RuntimeComponent : public BaseComponent
{
    DEFINE_COMPONENT(AnubisRuntime, "ls::anubis::RuntimeComponent")

    using OperationVariant = std::variant<
        MoveTo,
        PlayAnimation,
        UseItem,
        UseSpell,
        LookAtEntity,
        SteerTo,
        LookFrom,
        Attack,
        FleeFrom,
        FleeFromEntity,
        FleeFromDangerousSurface,
        FollowOwnerOrLeader,
        Sleep,
        AiMove,
        StartAutomatedDialog,
        StartAutomatedDialogRateLimited,
        StartBehaviorDialog,
        MoveInRange,
        Wander,
        MoveItem,
        PickUpItem,
        DropItem,
        DropItemTo,
        Combine,
        Follow
    >;

    std::optional<OperationVariant> Operation;
    uint64_t field_70;
    [[bg3::hidden]] uint64_t Fiber[4];
};

DEFINE_TAG_COMPONENT(ls::anubis, EnabledComponent, AnubisEnabled)

END_NS()

BEGIN_NS(constellation)

struct OutputSocketConnection
{
    EntityHandle Entity;
    Guid InputSocket;
};

struct OutputSocket
{
    Array<Guid> field_0;
    Array<OutputSocketConnection> Connections;
};

struct InputSocket
{
    int16_t field_0;
    int16_t field_2;
};

struct Component : public BaseComponent
{
    DEFINE_COMPONENT(Constellation, "ls::constellation::Component")

    HashMap<Guid, InputSocket> Inputs;
    HashMap<Guid, OutputSocket> Outputs;
    HashMap<STDString, Array<Guid>> field_80;
    HashMap<STDString, Array<Guid>> field_C0;
};

END_NS()
