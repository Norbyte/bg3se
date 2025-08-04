#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

struct ActionResourceSetValueRequest
{
    Guid ResourceId;
    double Amount{ .0f };
    double OldAmount{ .0f };
};

struct ActionResourceRefillRequest
{
    EntityHandle Entity;
    uint64_t Flags{ 0 };
};

struct InspirationPointGainedRequest
{
    EntityHandle Entity;
    float Amount{ .0f };
    TranslatedString field_C;
    TranslatedString field_1C;
    EntityHandle Entity2;
};

struct PartyResourceUpdateRequest
{
    EntityHandle Entity;
    EntityHandle Party;
    Guid Resource;
    int Level{ 0 };
    float Amount{ .0f };
    bool AddNewResourceType{ false };
};

struct ActionResourceQuery
{
    Guid ResourceId;
    int BoostAmount{ 0 };
};

struct ActionResourceDiceValue
{
    double Amount{ .0f };
    double MaxAmount{ .0f };
};

struct ActionResourceEntry
{
    Guid ResourceUUID;
    [[bg3::legacy(ResourceId)]] int Level{ 0 };
    double Amount{ .0f };
    double MaxAmount{ .0f };
    [[bg3::legacy(field_28)]] ResourceReplenishType ReplenishType{ ResourceReplenishType::Default };
    [[bg3::legacy(SubAmounts)]] std::optional<std::array<ActionResourceDiceValue, 7>> DiceValues;
    uint8_t field_A8{ 0 };
};

struct ActionResourcesComponent : public BaseComponent
{
    DEFINE_COMPONENT(ActionResources, "eoc::ActionResourcesComponent")

    HashMap<Guid, Array<ActionResourceEntry>> Resources;
};

struct ActionResourceEventsOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ActionResourceEvents, "eoc::ActionResourceEventsOneFrameComponent")

    Array<ActionResourceSetValueRequest> Events;
};

struct ActionResourceSpendResult
{
    Guid Resource;
    bool Succeeded{ true };
};

struct ActionResourceSpendEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ActionResourceSpendEvent, "eoc::ActionResourceSpendEventOneFrameComponent")

    Array<ActionResourceSpendResult> Results;
};

END_SE()

BEGIN_NS(esv)

struct ActionResourceChangeResultsSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(ActionResourceChangeResults, "esv::action_resource::ResourceChangeResultsSingletonComponent")

    Array<EntityHandle> Results;
};

struct ActionResourceSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerActionResource, "esv::ActionResourceSystem")

    [[bg3::hidden]] UnknownSignal ObjectActionResourceValueChanged;
    [[bg3::hidden]] UnknownSignal field_28;
    [[bg3::hidden]] UnknownSignal ObjectActionPointsValueChanged;
    HashMap<EntityHandle, Array<InspirationPointGainedRequest>> IncreaseInspirationResource;
    [[bg3::hidden]] UnknownFunction field_98;
    HashMap<EntityHandle, ActionResourceQuery> ActionResourceBoosts;
    HashMap<EntityHandle, Array<ActionResourceSetValueRequest>> SetResourceValue;
    Array<ActionResourceRefillRequest> RefillResources;
    Array<ActionResourceRefillRequest> RefillResources2;
    Array<PartyResourceUpdateRequest> UpdatePartyResources;
    [[bg3::hidden]] UnknownFunction field_188;
    [[bg3::hidden]] UnknownFunction field_1C8;
    [[bg3::hidden]] void* ThothMachine;
    [[bg3::hidden]] void* RPGStats;
    [[bg3::hidden]] void* ImmutableDataHeadmaster;
    [[bg3::hidden]] void* SpellPrototypeManager;
    [[bg3::hidden]] void* StatsPrototypeManager;
    [[bg3::hidden]] void* UnitTestHelper;
};

END_NS()
