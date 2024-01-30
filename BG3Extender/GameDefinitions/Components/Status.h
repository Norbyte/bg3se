#pragma once

BEGIN_NS(status)

struct ContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::StatusContainer;
	static constexpr auto EngineClass = "eoc::status::ContainerComponent";

	MultiHashMap<EntityHandle, FixedString> Statuses;
};

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


BEGIN_NS(esv::status)

struct CauseComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerStatusCause;
	static constexpr auto EngineClass = "esv::status::CauseComponent";

	Guid Cause;
	int32_t StoryActionId;
};

struct StatusComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerStatus;
	static constexpr auto EngineClass = "esv::status::StatusComponent";

	EntityHandle Entity;
	ComponentHandle StatusHandle;
	FixedString StatusId;
	StatusType Type;
	Guid SpellCastSourceUuid;
};

struct OwnershipComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerStatusOwnership;
	static constexpr auto EngineClass = "esv::status::OwnershipComponent";

	EntityHandle Owner;
};

struct DifficultyModifiersComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerStatusDifficultyModifiers;
	static constexpr auto EngineClass = "esv::status::DifficultyModifiersComponent";

	MultiHashMap<FixedString, MultiHashSet<FixedString>> Modifiers;
};

struct UniqueComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerStatusUnique;
	static constexpr auto EngineClass = "esv::status::UniqueComponent";

	MultiHashMap<FixedString, EntityHandle> Unique;
};

struct PerformingComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerStatusPerforming;
	static constexpr auto EngineClass = "esv::status::PerformingComponent";

	FixedString field_0;
};

DEFINE_TAG_COMPONENT(esv::status, ActiveComponent, ServerStatusActive)
DEFINE_TAG_COMPONENT(esv::status, AddedFromSaveLoadComponent, ServerStatusAddedFromSaveLoad)
DEFINE_TAG_COMPONENT(esv::status, AuraComponent, ServerStatusAura)

END_NS()
