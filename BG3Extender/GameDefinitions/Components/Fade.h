#pragma once

BEGIN_SE()

struct FadeChildrenAddedOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(FadeChildrenAdded, "ls::FadeChildrenAddedOneFrameComponent")

    Array<Guid> Children;
};

struct FadeChildrenRemovedOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(FadeChildrenRemoved, "ls::FadeChildrenRemovedOneFrameComponent")

    Array<Guid> Children;
};

struct FadeableObstructionHierarchySingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(FadeableObstructionHierarchy, "ls::FadeableObstructionHierarchySingletonComponent")

    HashMap<EntityHandle, HashMap<FixedString, HashSet<EntityHandle>>> field_0;
    HashMap<Guid, Array<EntityHandle>> field_40;
};

struct FadeGroupComponent : public BaseComponent
{
    DEFINE_COMPONENT(FadeGroup, "ls::FadeGroupComponent")

    FixedString FadeGroup;
    HashSet<EntityHandle> Entities;
    HashSet<EntityHandle> Entities2;
};

struct FadeGroupMappingSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(FadeGroupMapping, "ls::FadeGroupMappingSingletonComponent")

    HashMap<FixedString, EntityHandle> Mappings;
};

struct FadeableObstructionComponent : public BaseComponent
{
    DEFINE_COMPONENT(FadeableObstruction, "ls::FadeableObstructionComponent")

    Guid Uuid;
    FixedString FadeGroup;
    // Editor only
    // bool HierarchyOnlyFade;
    // bool OnlyFadeChildrenFromGroupChanged;
    HashSet<Guid> FadeChildren;
};

struct FadeableObstructionMappingSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(FadeableObstructionMapping, "ls::FadeableObstructionMappingSingletonComponent")

    HashMap<Guid, EntityHandle> Mappings;
};

struct FadeableAddedOrRemovedData
{
    Guid Uuid;
    EntityHandle Entity;
    Array<Guid> FadeChildren;
    FixedString FadeGroup;
    bool OnlyFadeChildrenFromGroupChanged{ false };
    bool Added{ true };
};

struct FadeChildrenChange
{
    Array<Guid> Children;
    EntityHandle Entity;
    bool Added{ true };
};

struct FadeGroupChange
{
    EntityHandle Entity;
    FixedString OldFadeGroup;
    FixedString NewFadeGroup;
};

struct OnlyFadeChildrenFromGroupChange
{
    EntityHandle Entity;
    bool OnlyFadeChildrenFromGroup{ false };
};


struct FadeableObstructionMappingSystem : public BaseSystem
{
    DEFINE_SYSTEM(FadeableObstructionMapping, "ls::FadeableObstructionMappingSystem")

    [[bg3::hidden]] UnknownFunction qword10;
    Array<FadeableAddedOrRemovedData> AddRemove;
    Array<FadeChildrenChange> FadeChildrenChanges;
    Array<FadeGroupChange> FadeGroupChanges;
    Array<OnlyFadeChildrenFromGroupChange> OnlyFadeChildrenFromGroupChanges;
};


END_SE()
