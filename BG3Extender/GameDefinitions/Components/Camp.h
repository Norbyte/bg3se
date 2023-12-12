#pragma once

BEGIN_NS(camp)

struct ChestComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CampChest;
	static constexpr auto EngineClass = "eoc::camp::ChestComponent";

	STDString field_0;
	int UserID;
	FixedString field_1C;
	EntityHandle field_20;
	int field_28;
};

struct EndTheDayStateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CampEndTheDayState;
	static constexpr auto EngineClass = "eoc::camp::EndTheDayStateComponent";

	uint8_t State;
	EntityHandle field_8;
};

struct QualityComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CampQuality;
	static constexpr auto EngineClass = "eoc::camp::QualityComponent";

	int field_0;
	int field_4;
};

struct SettingsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CampSettings;
	static constexpr auto EngineClass = "eoc::camp::SettingsComponent";

	uint8_t field_0;
	uint8_t field_1;
	int field_4;
};

struct SupplyComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CampSupply;
	static constexpr auto EngineClass = "eoc::camp::SupplyComponent";

	int Amount;
};

struct TotalSuppliesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CampTotalSupplies;
	static constexpr auto EngineClass = "eoc::camp::TotalSuppliesComponent";

	int Amount;
};

DEFINE_TAG_COMPONENT(eoc::camp, PresenceComponent, CampPresence)

END_NS()
