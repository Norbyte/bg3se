#pragma once

BEGIN_NS(esv)

struct SteeringMachine : public ProtectedGameObject<SteeringMachine>
{
    [[bg3::hidden]] std::array<SteeringState*, 5> CachedStates;
    bool Active;
    std::array<SteeringState*, 2> Layers;
    ecs::EntityRef Entity;
};

struct SteeringState : public ProtectedGameObject<SteeringState>
{
    virtual ~SteeringState() = 0;
    virtual SteeringType GetType() const = 0;
    virtual bool Enter() = 0;
    virtual bool Continue() = 0;
    virtual void Update(GameTime const&) = 0;
    virtual bool VMT28() = 0;
    virtual void VMT30() = 0;
    virtual bool VMT38() = 0;
    virtual void Init(bool) = 0;
    virtual bool GetSyncData(ScratchBuffer&) = 0;
    virtual bool SyncData(ScratchBuffer&) = 0;
    virtual bool SavegameVisit(ObjectVisitor&) = 0;

    SteeringMachine* SteeringMachine;
    bool field_10;
    int TransactionId;

    // Lua getter helpers
    SteeringType LuaGetType() const;
    //# P_GETTER(Type, LuaGetType)
};

struct SSSteer : public SteeringState
{
    static constexpr auto Type = SteeringType::Steer;

    float SpeedMultiplier;
    bool field_1C;
};

struct SSSteerTunnel : public SteeringState
{
    static constexpr auto Type = SteeringType::SteerTunnel;

    glm::vec3 TargetDir;
    glm::vec3 field_24;
    uint8_t field_30;
};

struct SSSteerTo : public SteeringState
{
    static constexpr auto Type = SteeringType::SteerTo;

    EntityHandle Target;
    EntityHandle field_20;
    glm::vec3 TargetPos;
    glm::vec3 field_34;
    float TimeRemaining;
    float MinDistance;
    float AngleTolerance;
    float SteerSpeedMultiplier;
    bool SnapToTarget;
    uint8_t field_51;
    uint8_t field_52;
    uint8_t field_53;
};

struct SSLookAtParams
{
    Guid SettingsTransactionId;
    EntityHandle Target;
    FixedString Bone;
    float Duration;
};

struct SSLookAt : public SteeringState
{
    static constexpr auto Type = SteeringType::LookAt;

    SSLookAtParams Params;
    SSLookAtParams CurrentParams;
    int field_58;
};

END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(esv::SteeringState);

END_NS()
