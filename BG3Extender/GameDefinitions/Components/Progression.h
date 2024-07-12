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
	Guid Race;
	int field_40;
	uint8_t field_44;
	int field_48;
	EntityHandle Owner;
	MultiHashMap<Guid, Guid> field_50;
};

END_NS()

BEGIN_NS(esv::progression)

struct ChangedContainersComponent : public BaseComponent
{
	DEFINE_COMPONENT(ProgressionChangedContainers, "esv::progression::ChangedContainersComponent")

	MultiHashMap<EntityHandle, MultiHashMap<EntityHandle, Array<EntityHandle>>> Changes;
};

END_NS()
