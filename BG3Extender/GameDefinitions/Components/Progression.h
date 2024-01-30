#pragma once

BEGIN_NS(progression)

struct LevelUpComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::LevelUp;
	static constexpr auto EngineClass = "eoc::progression::LevelUpComponent";

	Array<LevelUpData> field_18;
};

struct ContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ProgressionContainer;
	static constexpr auto EngineClass = "eoc::ProgressionContainerComponent";

	Array<Array<EntityHandle>> Progressions;
};

struct MetaComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ProgressionMeta;
	static constexpr auto EngineClass = "eoc::progression::MetaComponent";

	uint8_t field_18;
	Guid Progression;
	Guid Race;
	int field_40;
	uint8_t field_44;
	int field_48;
	EntityHandle Owner;
};

END_NS()

BEGIN_NS(esv::progression)

struct ChangedContainersComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ProgressionChangedContainers;
	static constexpr auto EngineClass = "esv::progression::ChangedContainersComponent";

	MultiHashMap<EntityHandle, MultiHashMap<EntityHandle, Array<EntityHandle>>> Changes;
};

END_NS()
