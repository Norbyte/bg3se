#pragma once

BEGIN_NS(esv)

struct MovementState : public ProtectedGameObject<MovementState>
{
    virtual ~MovementState() = 0;
    virtual MovementType GetType() const = 0;
    virtual bool Enter() = 0;
    virtual bool Continue() = 0;
    virtual void Update(GameTime const&, float) = 0;
    virtual bool VMT28() = 0;
    virtual void Exit() = 0;
    virtual void VMT38() = 0;
    virtual bool VMT40() = 0;
    virtual void VMT48() = 0;
    virtual void OnAnimationEvent(void*) = 0;

    MovementMachine* MovementMachine;
    int TransactionId;

    // Lua getter helpers
    MovementType LuaGetType() const;
    //# P_GETTER(Type, LuaGetType)
};


struct MovementMachine : public ProtectedGameObject<MovementMachine>
{
    [[bg3::hidden]] std::array<MovementState*, 6> CachedStates;
    bool Active;
    std::array<MovementState*, 2> Layers;
    EntityHandle Entity;
};


struct MSMoveToSettings
{
    glm::vec3 TargetOriginal;
    glm::vec3 TargetAdjusted;
    EntityHandle TargetHandle;
    Array<EntityHandle> IgnoreEntities;
    uint32_t AiTargetCheck;
    uint64_t CollisionMaskMove;
    uint64_t CollisionMaskStand;
    uint16_t SearchHorizon;
    uint8_t PathClimbingMode;
    uint8_t PathDroppingMode;
    std::variant<uint8_t, float> Speed;
    uint32_t SurfaceAvoidance;
    uint8_t DangerousAuraAvoidance;
    float CloseEnoughMin;
    float CloseEnoughMax;
    float CloseEnoughFloor;
    int field_118;
    int CloseEnoughPreference;
    uint8_t WeightFuncType;
    bool AddSourceBoundsToMargin;
    bool IgnoreDynamics;
    bool IsAvoidingObstacles;
    bool IsAvoidingFallDamage;
    bool IsPreciseItemInteraction;
    bool HasSpliningNodes;
    bool HasTurningNodes;
};

struct MSMoveTo : public MovementState
{
    static constexpr auto Type = MovementType::MoveTo;

    int PathId2;
    int PathId;
    glm::vec3 TargetOriginal2;
    glm::vec3 TargetAdjusted2;
    glm::vec3 TargetAdjusted3;
    float NextTickTime;
    int field_48;
    float NextPathfindingTime;
    glm::vec3 field_50;
    int field_5C;
    bool Finished;
    int State;
    uint8_t field_68;
    bool PathfindingStarted;
    bool CheckEndPosition;
    bool IsAiEntity;
    bool NeedsPathfinding;
    [[bg3::hidden]] UnknownFunction DestinationFunc;
    MSMoveToSettings Settings;
    MSMoveToSettings CurrentSettings;
};

struct MSIdle : public MovementState
{
    static constexpr auto Type = MovementType::Idle;
};

struct MSMoveToSync : public MovementState
{
    static constexpr auto Type = MovementType::MoveToSync;

    Array<glm::vec3> Positions;
    Array<bool> field_28;
    Array<bool> field_38;
    glm::vec3 field_48;
    int field_54;
    int field_58;
    uint8_t field_5C;
    uint8_t field_5D;
    int PathId;
    glm::vec3 NextPosition;
};

struct ClimbResult
{
    int field_0;
    int field_4;
    int field_8;
    glm::vec3 field_C;
    int field_18;
};

struct ClimbBlendspaceData
{
    FixedString WorldClimbingBlendspace;
    float BlendedDuration;
    float StepUpHeightDelta;
};

struct ClimbData
{
    glm::vec3 CurrentPosition;
    glm::vec3 ClimbStartPosition;
    glm::vec3 ClimbEndPosition;
    glm::vec3 ClimbEndPosition2;
    glm::vec3 ClimbEndPosition3;
    float Bounds;
    float HeightDelta;
    float StepHeight;
    float WorldClimbingHeight;
    float MovementSpeed;
    bool ClimbUp;
};

struct MSClimbing : public MovementState
{
    static constexpr auto Type = MovementType::Climbing;

    ClimbBlendspaceData Blendspace;
    ClimbData ClimbData;
    ClimbResult ClimbResult;
    int State;
    [[bg3::hidden]] Array<void*> ClimbNodes;
    float TimeRemaining;
    bool GenomeUpdated;
};

struct MSFalling : public MovementState
{
    static constexpr auto Type = MovementType::Falling;

    float Height;
    glm::vec3 StartPosition;
    glm::vec3 FallPosition;
    bool FallAnimationStarted;
    bool ExpectedFall;
    bool OnDestroyedPlatform;
    uint8_t field_37;
    float TimeSpent;
    int State;
};

struct MSRepose : public MovementState
{
    static constexpr auto Type = MovementType::Repose;

    bool EndRepose;
};

END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(esv::MovementState);

END_NS()
