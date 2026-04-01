#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

struct StaticPhysicsComponent : public BaseComponent
{
    DEFINE_COMPONENT(StaticPhysics, "ls::StaticPhysicsComponent")

    // Editor only
    // FixedString field_0;
    // GameObjectTemplate* Template;
    phx::PhysicsObject* Physics;
};

struct PhysicsResourceLoadDesciptionComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhysicsResourceLoadDesciption, "ls::PhysicsResourceLoadDesciptionComponent")

    FixedString PhysicsTemplate;
    bool field_4{ false };
};

struct PhysicsLoadComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhysicsLoad, "ls::PhysicsLoadComponent")

    aio::Priority Priority;
    [[bg3::hidden]] void* LoadCallback{ nullptr };
    int field_10{ 0 };
};

struct PhysicsLoaderRegisteredComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhysicsLoaderRegistered, "ls::PhysicsLoaderRegisteredComponent")

    int field_0{ -1 };
};

struct PhysicsPathLoadDesciptionComponent : public BaseComponent
{
    DEFINE_COMPONENT(PhysicsPathLoadDesciption, "ls::PhysicsPathLoadDesciptionComponent")

    Path Path;
    uint32_t field_20{ 0 };
};

struct PhysicsComponent : public BaseComponent
{
    DEFINE_COMPONENT(Physics, "ls::PhysicsComponent")

    phx::PhysicsObject* Physics;
    PhysicsGroupFlags PhysicsGroup;
    PhysicsGroupFlags CollidesWith;
    PhysicsExtraFlags ExtraFlags;
    [[bg3::legacy(field_14)]] bool HasPhysics;
    uint8_t field_15;
    [[bg3::legacy(field_16)]] bool IsClustered;
};

DEFINE_TAG_COMPONENT(ls, PhysicsStreamLoadComponent, PhysicsStreamLoad)
DEFINE_TAG_COMPONENT(ls, PhysicsLoadedComponent, PhysicsLoaded)
DEFINE_TAG_COMPONENT(ls, PhysicsReloadOneFrameComponent, PhysicsReloadOneFrame)

END_SE()
