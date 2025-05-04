#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(ecl)

struct PlayerPickingHelperInner2
{
    glm::vec3 CharacterPosition;
    glm::vec3 DeadCharacterPosition;
    glm::vec3 ItemPosition;
    [[bg3::legacy(field_24)]] glm::vec3 TooltipPosition;
    EntityHandle Character;
    EntityHandle DeadCharacter;
    EntityHandle Item;
    [[bg3::legacy(field_48)]] EntityHandle Tooltip;
    EntityHandle GameObject;
    EntityHandle field_58;
    uint8_t Flags;
};


struct PlayerPickingHelperInner
{
    std::array<PlayerPickingHelperInner2, 2> Inner;
    [[bg3::legacy(Position0)]] glm::vec3 PlaceablePosition;
    [[bg3::legacy(Direction0)]] glm::vec3 PlaceableNormal;
    [[bg3::legacy(Position1)]] glm::vec3 Position;
    [[bg3::legacy(Direction1)]] glm::vec3 Normal;
    [[bg3::legacy(Position2)]] glm::vec3 SceneryPosition;
    [[bg3::legacy(Direction2)]] glm::vec3 SceneryNormal;
    [[bg3::legacy(field_118)]] glm::vec3 WorldPosition;
    [[bg3::legacy(field_124)]] glm::vec3 WorldNormal;
    [[bg3::legacy(field_130_EH)]] EntityHandle PlaceableObject;
    [[bg3::legacy(field_138_EH)]] EntityHandle PlaceableEntity;
    [[bg3::legacy(field_140_Flags2)]] uint64_t AiGridTile;
    [[bg3::legacy(field_148)]] float PlaceableFraction;
    [[bg3::legacy(field_14C)]] float Fraction;
    [[bg3::legacy(field_150)]] float SceneryFraction;
    uint8_t field_154_Flags;
};

struct PlayerPickingInRangeEntity
{
    EntityHandle field_0_Entity;
    std::optional<int> field_8_TurnOrder;
    float field_14;
    glm::vec3 field_18;
};

struct PlayerPickingCacheValue
{
    int field_0;
    int field_4;
    uint8_t field_8;
};


struct PlayerPickingHelper : public ProtectedGameObject<PlayerPickingHelper>
{
    [[bg3::hidden]] void* VMT;
    PlayerPickingHelperInner Inner;
    [[bg3::hidden]] void* Level;
    uint16_t PlayerId;
    bool Activated;
    [[bg3::legacy(field_170)]] Array<PlayerPickingInRangeEntity> SelectableObjects;
    [[bg3::legacy(field_180)]] std::optional<PlayerPickingInRangeEntity> Selection;
    uint8_t field_1A8;
    uint8_t field_1A9;
    [[bg3::legacy(field_1C0)]] EntityHandle UIEntity;
    glm::vec3 PickingDirection;
    std::optional<glm::vec2> ControllerNudge;
    glm::vec2 WindowCursorPos;
    [[bg3::hidden]] void* field_1E8;
    [[bg3::hidden]] void* field_1F0;
    [[bg3::hidden]] void* field_1F8;
    [[bg3::hidden]] void* field_200;
    [[bg3::hidden]] void* field_208;
    uint8_t field_210;
    std::optional<glm::vec3> TargetOverride;
    HashMap<EntityHandle, PlayerPickingCacheValue> SpellConditionsCache;
    int field_268_NextIndex;
    uint32_t field_26C;
    HashMap<EntityHandle, PlayerPickingCacheValue> Cache;
    int field_2B0;
    int field_2B4;
    HashMap<EntityHandle, PlayerPickingCacheValue> SightCheckCache;
    int field_2F8;
    bool IsMoving;
};


struct [[bg3::hidden]] PickingHelperManager : public BaseSystem
{
    DEFINE_SYSTEM(PickingHelperManager, "ecl::PickingHelperManager")

    void* field_8;
    LegacyRefMap<uint16_t, PlayerPickingHelper*> PlayerHelpers;
    void* field_20;
};


END_NS()
