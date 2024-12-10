#pragma once

BEGIN_NS(progression)

struct LevelUpComponent : public BaseComponent
{
    DEFINE_COMPONENT(LevelUp, "eoc::progression::LevelUpComponent")

    [[bg3::legacy(field_18)]] Array<LevelUpData> LevelUps;
};

struct ContainerComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProgressionContainer, "eoc::ProgressionContainerComponent")

    Array<Array<EntityHandle>> Progressions;
};

struct MetaComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProgressionMeta, "eoc::progression::MetaComponent")

    uint8_t field_18;
    Guid Progression;
    [[bg3::legacy(Race)]] Guid Source;
    [[bg3::legacy(field_40)]] int ClassLevel;
    [[bg3::legacy(field_44)]] uint8_t ProgressionType;
    [[bg3::legacy(field_48)]] int Level;
    EntityHandle Owner;
    HashMap<Guid, Guid> field_50;
};

END_NS()

BEGIN_NS(esv::progression)

struct ChangedContainersComponent : public BaseComponent
{
    DEFINE_COMPONENT(ProgressionChangedContainers, "esv::progression::ChangedContainersComponent")

    HashMap<EntityHandle, HashMap<EntityHandle, Array<EntityHandle>>> Changes;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::progression, LevelUpChangedOneFrameComponent, ProgressionLevelUpChanged)

END_NS()
