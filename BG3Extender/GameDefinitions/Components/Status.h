#pragma once

BEGIN_NS(status)

struct CauseComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::StatusCause;
	static constexpr auto EngineClass = "eoc::status::CauseComponent";

	EntityHandle Cause;
};

struct IDComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::StatusID;
	static constexpr auto EngineClass = "eoc::status::IDComponent";

	FixedString ID;
};

struct IncapacitatedComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::StatusIncapacitated;
	static constexpr auto EngineClass = "eoc::status::IncapacitatedComponent";

	uint32_t field_0;
	MultiHashMap<int32_t, Array<int32_t>> field_8;
	uint8_t field_48;
};

struct LifetimeComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::StatusLifetime;
	static constexpr auto EngineClass = "eoc::status::LifetimeComponent";

	int field_0;
	int field_4;
};

struct VisualDisabledComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::StatusVisualDisabled;
	static constexpr auto EngineClass = "eoc::status::visual::DisabledComponent";

	MultiHashSet<FixedString> Visuals;
};

END_NS()
