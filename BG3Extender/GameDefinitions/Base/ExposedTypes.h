#pragma once

#include <GameDefinitions/TranslatedString.h>

BEGIN_SE()

struct GameTime
{
    double Time{ 0 };
    float DeltaTime{ 0 };
    int32_t Ticks{ 0 };
    double Unknown{ 0 };
};

struct ActionOriginator
{
    Guid ActionGuid;
    FixedString PassiveId;
    FixedString StatusId;
    FixedString InterruptId;
    bool CanApplyConcentration{ false };
};

struct SpellMetaId
{
    FixedString OriginatorPrototype;
    uint8_t _Pad[4];
    SpellSourceType SourceType{ SpellSourceType::Sentinel };
    Guid Source;
    Guid ProgressionSource;
};

struct SpellId : public SpellMetaId
{
    FixedString Prototype;

    inline bool operator == (SpellId const& o) const
    {
        return
            OriginatorPrototype == o.OriginatorPrototype
            && SourceType == o.SourceType
            && Source == o.Source
            && ProgressionSource == o.ProgressionSource
            && Prototype == o.Prototype;
    }
};

template <>
inline uint64_t HashMapHash<SpellId>(SpellId const& v)
{
    return HashMulti(v.ProgressionSource, v.SourceType, v.Prototype, v.OriginatorPrototype);
}

struct SpellIdWithPrototype : public SpellId
{
    stats::SpellPrototype* SpellProto{ nullptr };
    std::optional<Guid> SpellCastSource;
};

struct DamagePair
{
    int32_t Amount{ 0 };
    DamageType DamageType{ DamageType::None };
};

struct RollDefinition
{
    DiceSizeId DiceValue{ DiceSizeId::D6 };
    uint8_t AmountOfDices{ 0 };
    int DiceAdditionalValue{ 0 };
    [[bg3::legacy(field_8)]] bool DiceNegative{ false };
};

struct BoostParameters
{
    Guid Owner;
    uint8_t field_10{ 0 };
};

struct BoostSource
{
    BoostSourceType Type{ BoostSourceType::Undefined };
    FixedString Cause;
    EntityHandle Entity;
    ComponentHandle Status;
};

struct BoostDescription
{
    FixedString Boost;
    FixedString Params;
    FixedString Params2;
};

struct ResourceRollDefinition
{
    Guid field_0;
    uint8_t field_10{ 0 };
};

struct PathSettings
{
    uint8_t PathClimbingMode{ 0 };
    uint8_t PathDroppingMode{ 0 };
    std::variant<uint8_t, float> Speed;
    bool HasSmoothingNodes{ false };
    bool HasSpliningNodes{ false };
    bool HasTurningNodes{ false };
};

struct TextLine : TranslatedString
{
    bool field_10{ false };
};

struct TemplateInfo
{
    FixedString TemplateId;
    uint8_t TemplateType{ 0 };
};

END_SE()

BEGIN_NS(path)

struct Bezier3Trajectory
{
    float DistanceMin;
    float DistanceMax;
    float OffsetMin[2];
    float OffsetMax[2];
    float ShiftMin;
    float ShiftMax;
};

struct Bezier4Trajectory
{
    float DistanceMin;
    float DistanceMax;
    float OffsetAMin[2];
    float OffsetAMax[2];
    float OffsetBMin[2];
    float OffsetBMax[2];
    float ShiftAMin;
    float ShiftAMax;
    float ShiftBMin;
    float ShiftBMax;
};

struct ConstantVelocity
{
    float ConstantVelocity;
};

struct LinearVelocity
{
    float Acceleration;
    float InitialSpeed;
};

struct MappedVelocity
{
    FixedString Mapping;
};

struct Settings
{
    std::variant<Bezier3Trajectory, Bezier4Trajectory> Trajectory;
    [[bg3::hidden]] uint32_t _Pad;
    PathRotateMode RotateMode{ PathRotateMode::Static };
    std::variant<ConstantVelocity, LinearVelocity, MappedVelocity> Velocity;
};

struct PathMover : public Settings
{
    glm::vec3 SourcePosition;
    glm::quat SourceRotation;
    glm::vec3 TargetPosition;
    glm::quat TargetRotation;
    float InterpolateValue;
    std::array<float, 32> ComputedTrajectoryValues;
    Array<glm::vec3> ComputedVelocityValues;
    bool Initialized{ false };
};

END_NS()

BEGIN_NS(navigation)

struct TargetInfo
{
    glm::vec3 Position;
    float CloseEnoughMin;
    float CloseEnoughMax;
    float CloseEnoughFloor;
    float CloseEnoughCeiling;
    int CloseEnoughPreference;
    EntityHandle Target;
    Array<EntityHandle> IgnoreEntities;
    uint32_t AiTargetCheck;
    uint16_t CoverFlags;
    bool AddSourceBoundsToMargin;
    bool field_3F;
    bool SurfaceAvoidance;
    bool IsAvoidingObstacles;
    bool IsPreciseItemInteraction;
    EntityHandle PickupEntity;
    std::optional<glm::vec3> PickupPosition;
    uint8_t WeightFuncType;
};

END_NS()
