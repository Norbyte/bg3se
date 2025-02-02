#pragma once

#include <GameDefinitions/Effect.h>

BEGIN_NS(ecl)

struct GameUIDragController;
struct WorldDragController;
struct CharacterDragController;

struct DragController : public ProtectedGameObject<DragController>
{
    [[bg3::hidden]] void* VMT;
    [[bg3::readonly]] uint16_t PlayerId;
};

struct PlayerDragData : public ProtectedGameObject<PlayerDragData>
{
    Array<EntityHandle> Entities;
    glm::vec2 ScreenPosition;
    bool DraggingAction;
    bool IsDragging;
    bool Active;
    [[bg3::hidden]] DragController* ActiveController;
    [[bg3::hidden]] Array<DragController*> Controllers;

    // Lua getter helpers
    GameUIDragController* LuaGetGameUIDrag() const;
    WorldDragController* LuaGetWorldDrag() const;
    CharacterDragController* LuaGetCharacterDrag() const;

    //# P_GETTER(GameUIDrag, LuaGetGameUIDrag)
    //# P_GETTER(WorldDrag, LuaGetWorldDrag)
    //# P_GETTER(CharacterDrag, LuaGetCharacterDrag)
};


struct [[bg3::hidden]] DragDropManager : public ProtectedGameObject<DragDropManager>
{
    UnknownSignal OnStartDrag;
    UnknownSignal OnStartDragActionEntities;
    UnknownSignal OnDragUpdate;
    UnknownSignal OnStartDragActionPositional;
    UnknownSignal OnStartDragActionPassive;
    UnknownSignal OnStopDrag;
    uint8_t field_90[0x40]; // MPMCQueueBounded maybe?
    LegacyRefMap<uint16_t, PlayerDragData> PlayerData;
    ecs::EntityWorld* EntityWorld;
};

struct GameUIDragController : public DragController
{
    bool Enabled;
};

struct WorldDragController : public DragController
{
    [[bg3::hidden]] void* InputManagerVMT2;
    bool Enabled;
    bool Active;
    uint8_t field_1A;
    uint8_t field_1B;
    EntityHandle Outline;
    Array<EntityHandle> DraggingEntities;
    Array<EntityHandle> Visuals;
    Array<Transform> Transforms;
    [[bg3::readonly]] int PathId;
    [[bg3::readonly]] int PathId2;
    float field_60;
    float Rotation;
    float TimeSpentSq;
    float TimeSpent;
    float RotationRate;
    uint64_t ErrorFlags;
    [[bg3::readonly]] int State;
    [[bg3::hidden]] void* PathPreviewer;
};

struct CharacterDragController : public DragController
{
    [[bg3::hidden]] void* VMT2;
    bool Enabled;
    bool Active;
    uint8_t field_1A;
    bool AnimationEventTriggered;
    EntityHandle Outline1;
    EntityHandle Character;
    EntityHandle VisualEntity;
    EffectHandler Effect;
    [[bg3::hidden]] void* ObjectVisual;
    glm::vec3 WorldPosition;
    glm::vec4 WorldRotate;
    glm::vec4 WorldRotate2;
    float Rotation;
    float TimeSpentSq;
    float TimeSpent;
    float RotationRate;
    int field_1A4;
    uint64_t ErrorFlags;
};

END_NS()
