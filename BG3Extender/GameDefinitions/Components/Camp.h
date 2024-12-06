#pragma once

BEGIN_NS(camp)

struct ChestComponent : public BaseComponent
{
    DEFINE_COMPONENT(CampChest, "eoc::camp::ChestComponent")

    STDString field_0;
    int UserID;
    FixedString field_1C;
    EntityHandle field_20;
    int field_28;
};

struct EndTheDayStateComponent : public BaseComponent
{
    DEFINE_COMPONENT(CampEndTheDayState, "eoc::camp::EndTheDayStateComponent")

    uint8_t State;
    EntityHandle field_8;
};

struct QualityComponent : public BaseComponent
{
    DEFINE_COMPONENT(CampQuality, "eoc::camp::QualityComponent")

    int field_0;
    int field_4;
};

struct SettingsComponent : public BaseComponent
{
    DEFINE_COMPONENT(CampSettings, "eoc::camp::SettingsComponent")

    uint8_t field_0;
    uint8_t field_1;
    int field_4;
};

struct SupplyComponent : public BaseComponent
{
    DEFINE_COMPONENT(CampSupply, "eoc::camp::SupplyComponent")

    int Amount;
};

struct TotalSuppliesComponent : public BaseComponent
{
    DEFINE_COMPONENT(CampTotalSupplies, "eoc::camp::TotalSuppliesComponent")

    int Amount;
};

DEFINE_TAG_COMPONENT(eoc::camp, PresenceComponent, CampPresence)

END_NS()
