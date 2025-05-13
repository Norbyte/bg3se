#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

struct ActionResourceDiceValue
{
    double Amount;
    double MaxAmount;
};

struct ActionResourceEntry
{
    Guid ResourceUUID;
    [[bg3::legacy(ResourceId)]] int Level;
    double Amount;
    double MaxAmount;
    [[bg3::legacy(field_28)]] ResourceReplenishType ReplenishType;
    [[bg3::legacy(SubAmounts)]] std::optional<std::array<ActionResourceDiceValue, 7>> DiceValues;
    uint8_t field_A8;
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
    bool Succeeded;
};

struct ActionResourceSpendEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(ActionResourceSpendEvent, "eoc::ActionResourceSpendEventOneFrameComponent")

    Array<ActionResourceSpendResult> Results;
};

END_SE()
