#pragma once

BEGIN_NS(esv)

struct BehaviourMachine : public ProtectedGameObject<BehaviourMachine>
{
    [[bg3::hidden]] std::array<BehaviourState*, 10> CachedStates;
    std::array<BehaviourState*, 2> Layers;
    EntityHandle Entity;
};

struct BehaviourState : public ProtectedGameObject<BehaviourMachine>
{
    virtual ~BehaviourState() = 0;
    virtual BehaviourType GetType() const = 0;
    virtual bool Enter() = 0;
    virtual bool Continue() = 0;
    virtual void Update(GameTime const&) = 0;
    virtual bool CanEnter() = 0;
    virtual void Exit() = 0;
    virtual void Exit2() = 0;
    virtual bool CanExit() = 0;
    virtual bool VMT48() = 0;
    virtual bool VMT50() = 0;
    virtual bool SavegameVisit(ObjectVisitor&) = 0;
    virtual bool HasStarted() = 0;
    virtual bool SyncData(ScratchBuffer&) = 0;
    virtual bool VMT70() = 0;

    BehaviourMachine* BehaviourMachine;
    int TransactionId;

    // Lua getter helpers
    BehaviourType LuaGetType() const;
    //# P_GETTER(Type, LuaGetType)
};

struct BSMoveCheckpoint : public AiPathCheckpoint
{
    int field_30;
    int field_34;
};

struct NavigationTargetInfo
{
    glm::vec3 Position;
    float CloseEnoughMin;
    float CloseEnoughMax;
    float CloseEnoughFloor;
    int field_18;
    int CloseEnoughPreference;
    EntityHandle TargetHandle;
    Array<EntityHandle> IgnoreEntities;
    uint32_t AiTargetCheck;
    uint8_t field_3C;
    uint8_t field_3D;
    bool AddSourceBoundsToMargin;
    uint8_t field_3F;
    uint8_t field_40;
    bool IsAvoidingObstacles;
    bool IsPreciseItemInteraction;
    EntityHandle IgnoreEntity2;
    std::optional<glm::vec3> CharacterPositionOverride;
    uint8_t WeightFuncType;
};

struct BSMove : public BehaviourState
{
    static constexpr auto Type = BehaviourType::Move;

    Array<BSMoveCheckpoint> Checkpoints;
    uint16_t SearchHorizon;
    NavigationTargetInfo TargetInfo;
    float PathDistanceXZ;
    uint8_t Flags;
    PathSettings PathingSettings;
    PathSettings PathingSettings2;
    uint32_t SurfaceAvoidance;
    uint8_t DangerousAuraAvoidance;
    glm::vec3 LadderUsePosition;
    int State;
    int MovementTransactionId;
    int ActionTransactionId;
    EntityHandle UseItem;
    AiPathId PathId;
    AiPathId PathId2;
};

struct BSMoveAndX : public BSMove
{
    PathSettings ActionPathSettings;
    uint16_t ActionSearchHorizon;
    bool field_102;
};

struct BSMoveToAndTalk : public BSMoveAndX
{
    static constexpr auto Type = BehaviourType::MoveToAndTalk;

    EntityHandle TalkTo;
    FixedString Speed;
    FixedString DialogInstance;
    bool Failed;
    bool DialogRequested;
    float Timeout;
    float TimeSpent;
};

struct BSMoveItem : public BSMoveAndX
{
    static constexpr auto Type = BehaviourType::MoveItem;

    EntityHandle Item;
    glm::vec3 MoveToPosition;
    int Amount;
    int ItemActionTransactionId;
    int SteeringTransactionId;
    bool Failed;
};

struct BSMoveAndUseItem : public BSMoveAndX
{
    static constexpr auto Type = BehaviourType::MoveAndUseItem;

    EntityHandle Item;
    int ItemActionTransactionId;
    uint8_t Flags2;
    bool IsInteraction;
    bool Use;
};

struct BSPickupItem : public BSMoveAndX
{
    static constexpr auto Type = BehaviourType::PickupItem;

    EntityHandle Item;
    EntityHandle Inventory;
    Guid field_118;
    int Amount;
    int16_t Slot;
    bool Failed;
    uint8_t FailReason;
    uint8_t field_130;
};

struct BSWander : public BehaviourState
{
    static constexpr auto Type = BehaviourType::Wander;

    [[bg3::hidden]] void* Flood;
    float TimeRemaining;
    float Range;
    EntityHandle Target;
    std::variant<EntityHandle, glm::vec3> Anchor;
    glm::vec3 TargetPosition;
    PathSettings WanderPathSettings;
    bool Failed;
    int MovementTransactionId;
};

struct BSAppear : public BehaviourState
{
    static constexpr auto Type = BehaviourType::Appear;

    FixedString SpawnEffect;
    int AppearActionTransactionId;
    bool Finished;
};

struct BSFollowCharacter : public BehaviourState
{
    static constexpr auto Type = BehaviourType::FollowCharacter;

    [[bg3::hidden]] uint64_t FollowOutput[4];
    EntityHandle FollowCharacter;
    int BehaviourTransactionId;
    bool FollowParty;
    bool IsRegisteredToPartyGroup;
};

struct BSPatrol : public BehaviourState
{
    static constexpr auto Type = BehaviourType::Patrol;

    [[bg3::hidden]] uint64_t PatrollingOutput[4];
};

struct BSForce : public BehaviourState
{
    static constexpr auto Type = BehaviourType::Force;

    bool HasAction;
    bool ActionLayer;
    ActionState* Action;
    bool HasMovement;
    int MovementLayer;
    MovementState* Movement;
    bool HasSteering;
    int SteeringLayer;
    SteeringState* Steering;
};


END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(esv::BehaviourState);

END_NS()
