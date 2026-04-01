#pragma once

BEGIN_SE()

struct LevelComponent : public BaseComponent
{
    DEFINE_COMPONENT(Level, "ls::LevelComponent")

    [[bg3::legacy(field_0)]] EntityHandle RootLevel;
    FixedString LevelName;
};

struct LevelRootComponent : public BaseComponent
{
    DEFINE_COMPONENT(LevelRoot, "ls::LevelRootComponent")

    FixedString LevelName;
};

struct LevelInstanceComponent : public BaseComponent
{
    DEFINE_COMPONENT(LevelInstance, "ls::LevelInstanceComponent")

    FixedString LevelInstanceID;
    FixedString SubLevelName;
    FixedString LevelInstanceTemplate;
    uint8_t LevelType;
    bool Active;
    bool Platform;
    bool MovingPlatform;
    bool DynamicLayer;
    bool NeedsPhysics;
    bool HasTemplateAfterDestruction;
    bool UseSoundOcclusion;
    bool IsCinematic;
    bool Activated;
    Transform Transform;
};

struct LevelInstanceStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(LevelInstanceState, "ls::LevelInstanceStateComponent")

    HashSet<EntityHandle> Children;
    HashSet<EntityHandle> Children2;
    int32_t State;
    AABound LocalBound;
    AABound WorldBound;
    FixedString MergedLevelTemplateUUID;
    FixedString LevelInstanceID;
    FixedString LevelName;
    FixedString LevelName2;
    bool Destroyed;
    bool MovingPlatform;
    uint8_t field_A6;
    Transform Transform;
};

struct LevelInstanceTempDestroyedComponent : public BaseComponent
{
    DEFINE_COMPONENT(LevelInstanceTempDestroyed, "ls::level::LevelInstanceTempDestroyedComponent")

    EntityHandle Level;
};

struct LevelUnloadEventComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(LevelUnloadEvent, "ls::LevelUnloadEventComponent")

    FixedString Level;
};

struct LevelPrepareUnloadEventComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(LevelPrepareUnloadEvent, "ls::LevelPrepareUnloadEventComponent")

    FixedString Level;
};

struct LevelUnloadedOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(LevelUnloaded, "ls::LevelUnloadedOneFrameComponent")

    FixedString Level;
};

struct [[bg3::component]] SceneComponent : public Scene
{
    DEFINE_PROXY_COMPONENT(Scene, "ls::Scene")

    [[bg3::hidden]] void* _PAD;
};

DEFINE_TAG_COMPONENT(ls, SceneRootComponent, SceneRoot)
DEFINE_TAG_COMPONENT(ls, LevelIsOwnerComponent, LevelIsOwner)
DEFINE_TAG_COMPONENT(ls, LevelPrepareUnloadBusyComponent, LevelPrepareUnloadBusy)
DEFINE_TAG_COMPONENT(ls, LevelUnloadBusyComponent, LevelUnloadBusy)
DEFINE_TAG_COMPONENT(ls, LevelInstanceLoadComponent, LevelInstanceLoad)
DEFINE_TAG_COMPONENT(ls::level, LevelInstanceUnloadingComponent, LevelInstanceUnloading)
DEFINE_ONEFRAME_TAG_COMPONENT(ls, LevelInstanceUnloadedOneFrameComponent, LevelInstanceUnloaded)
DEFINE_ONEFRAME_TAG_COMPONENT(ls, LevelInstanceLoadedOneFrameComponent, LevelInstanceLoaded)


struct LevelInstanceAttachRequestSystem : public BaseSystem
{
    DEFINE_SYSTEM(LevelInstanceAttachRequest, "ls::LevelInstanceAttachRequestSystem")

    [[bg3::hidden]] CRITICAL_SECTION LevelSwapRequestsCS;
    [[bg3::hidden]] HashMap<EntityHandle, EntityHandle> LevelSwapRequests;
    [[bg3::hidden]] DWORD LastFrame;

    // Lua call helpers
    //# P_FUN(RequestLevelSwap, LevelInstanceAttachRequestSystem::RequestLevelSwap)
    void RequestLevelSwap(EntityHandle entity, EntityHandle level);
};

struct LevelInstanceLoadRequest
{
    EntityHandle Level;
    bool Load{ true };
};

struct LevelInstanceLoadRequestSystem : public BaseSystem
{
    DEFINE_SYSTEM(LevelInstanceLoadRequest, "ls::LevelInstanceLoadRequestSystem")

    [[bg3::hidden]] SynchronizedMPMCQueueBounded<LevelInstanceLoadRequest> Requests;
};

struct LevelInstanceMoveSystem : public BaseSystem
{
    DEFINE_SYSTEM(LevelInstanceMove, "ls::LevelInstanceMoveSystem")

    HashSet<EntityHandle> LastFrameMovedLevels;
    HashSet<EntityHandle> MoveLevels;
    [[bg3::hidden]] UnknownFunction Signals;
};

struct LevelInstanceAttachSystem : public BaseSystem
{
    DEFINE_SYSTEM(LevelInstanceAttach, "ls::level::LevelInstanceAttachSystem")

    HashSet<EntityHandle> RemoveLevelInstance;
    HashMap<EntityHandle, HashSet<EntityHandle>> DetachLevel;
    HashMap<EntityHandle, HashSet<EntityHandle>> AttachLevel;
    [[bg3::hidden]] LevelInstanceAttachRequestSystem* LevelInstanceAttachRequestSystem;
    [[bg3::hidden]] UnknownSignalSubscriber Replication;
    [[bg3::hidden]] void* qword118;
};

struct LevelInstanceLoadSystem : public BaseSystem
{
    DEFINE_SYSTEM(LevelInstanceLoad, "ls::level::LevelInstanceLoadSystem")

    [[bg3::hidden]] UnknownSignal field_10;
    [[bg3::hidden]] LevelManager* LevelManager;
    [[bg3::hidden]] Queue<EntityHandle> LoadRequests;
    [[bg3::hidden]] CRITICAL_SECTION LoadRequestsCS;
};


END_SE()
