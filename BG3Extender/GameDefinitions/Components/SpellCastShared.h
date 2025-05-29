#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(spell_cast)

struct SpellRollCastEventData
{
    [[bg3::legacy(field_0)]] FixedString CastKey;
    HitDesc Hit;
};

struct SpellRollTargetInfo
{
    uint64_t Flags;
    glm::vec3 Position;
};

struct SpellRollData
{
    EntityHandle Target;
    std::optional<EntityHandle> TargetProxy;
    [[bg3::legacy(Hits)]] Array<SpellRollCastEventData> Casts;
    HashMap<FixedString, int32_t> NameToCastIndex;
    int NextReaction;
    SpellMetaConditionType SpellConditionsType;
    std::optional<SpellRollTargetInfo> TargetInfo;
};

struct BaseTarget
{
    virtual ~BaseTarget();
    virtual glm::vec3 GetTargetPosition(ecs::WorldView& world, glm::vec3 const* pos, std::optional<SpellType> type) const;
    virtual glm::vec3 LEGACY_GetTargetPosition(ecs::EntityWorld& world) const;
    virtual std::optional<glm::vec3> GetEntityPosition(ecs::WorldView& world) const;
    virtual bool IsAlive(ecs::WorldView&) const;
    virtual bool IsValid() const;

    glm::vec3 DoGetTargetPosition(ecs::EntityWorld* world, SpellType type, glm::vec3 const* pos) const;
    std::optional<glm::vec3> DoGetPosition(EntityHandle entity, ecs::EntityWorld* world, SpellType type) const;

    EntityHandle Target;
    [[bg3::legacy(TargetOverride)]] std::optional<EntityHandle> TargetProxy;
    std::optional<glm::vec3> Position;
};

struct InitialTarget : public BaseTarget
{
    glm::vec3 GetTargetPosition(ecs::WorldView& world, glm::vec3 const* pos, std::optional<SpellType> type) const override;
    glm::vec3 LEGACY_GetTargetPosition(ecs::EntityWorld& world) const override;
    bool IsAlive(ecs::WorldView&) const override;
    bool IsValid() const override;

    SpellType TargetingType;
    std::optional<BaseTarget> Target2;
};

struct IntermediateTarget : public BaseTarget
{
    [[bg3::legacy(Target2)]] InitialTarget InitialTarget;
    uint8_t field_A0;
};

END_NS()
