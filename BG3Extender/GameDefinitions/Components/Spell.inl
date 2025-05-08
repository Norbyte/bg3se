#pragma once

#include <GameDefinitions/Components/Components.h>
#include <GameDefinitions/Components/Spell.h>
#include <GameDefinitions/Components/Visual.h>

BEGIN_NS(spell_cast)

std::optional<glm::vec3> BaseTarget::DoGetPosition(EntityHandle entity, ecs::EntityWorld* world, SpellType type) const
{
    if (!entity) return {};

    auto ecs = gExtender->GetECS(world);
    auto baseTransform = ecs->GetComponent<TransformComponent>(entity);

    if (type != SpellType::Throw && type != SpellType::Projectile) {
        auto repose = ecs->GetComponent<ReposeStateComponent>(entity);
        if (repose) {
            auto transform = ecs->GetComponent<TransformComponent>(repose->field_0);
            if (transform) {
                return transform->Transform.Translate;
            }
        }

        if (baseTransform) {
            return baseTransform->Transform.Translate;
        }
    }


    auto physics = ecs->GetComponent<PhysicsComponent>(entity);
    if (physics) {
        // TODO - use bounds from physics comp
        return baseTransform->Transform.Translate;
    } else if (baseTransform) {
        return baseTransform->Transform.Translate;
    } else {
        return {};
    }
}

glm::vec3 BaseTarget::DoGetTargetPosition(ecs::EntityWorld* world, SpellType type, glm::vec3 const* pos) const
{
    if (Position) {
        return *Position;
    }

    auto p = DoGetPosition(TargetOverride ? Target : *TargetOverride, world, type);
    if (p) {
        return *p;
    } else if (pos) {
        return *pos;
    } else {
        return glm::vec3(3.4028235e38f, 3.4028235e38f, 3.4028235e38f);
    }
}

BaseTarget::~BaseTarget()
{}

glm::vec3 BaseTarget::GetTargetPosition(ecs::WorldView& world, glm::vec3 const* pos, std::optional<SpellType> type) const
{
    return DoGetTargetPosition(world.World, type ? *type : SpellType::None, pos);
}

glm::vec3 BaseTarget::LEGACY_GetTargetPosition(ecs::EntityWorld& world) const
{
    return DoGetTargetPosition(&world, SpellType::None, nullptr);
}

std::optional<glm::vec3> BaseTarget::GetEntityPosition(ecs::WorldView& world) const
{
    auto entity = TargetOverride ? Target : *TargetOverride;
    return DoGetPosition(entity, world.World, SpellType::None);
}

bool BaseTarget::IsAlive(ecs::WorldView& world) const
{
    if (!world.World->IsValid(Target)) return false;

    if (TargetOverride && !world.World->IsValid(*TargetOverride)) return false;

    return true;
}

bool BaseTarget::IsValid() const
{
    return Target
        || (*TargetOverride && TargetOverride)
        || Position;
}


glm::vec3 InitialTarget::GetTargetPosition(ecs::WorldView& world, glm::vec3 const* pos, std::optional<SpellType> type) const
{
    return DoGetTargetPosition(world.World, type ? *type : TargetingType, pos);
}

glm::vec3 InitialTarget::LEGACY_GetTargetPosition(ecs::EntityWorld& world) const
{
    return DoGetTargetPosition(&world, TargetingType, nullptr);
}

bool InitialTarget::IsAlive(ecs::WorldView& world) const
{
    return Target2 ? Target2->IsAlive(world) : BaseTarget::IsAlive(world);
}

bool InitialTarget::IsValid() const
{
    return Target2 ? Target2->IsValid() : BaseTarget::IsValid();
}

END_NS()
