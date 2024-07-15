#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

struct PassiveContainerComponent : public BaseComponent
{
	DEFINE_COMPONENT(PassiveContainer, "eoc::PassiveContainerComponent")

	Array<EntityHandle> Passives;
};

struct PassiveComponent : public BaseComponent
{
	DEFINE_COMPONENT(Passive, "eoc::PassiveComponent")

	[[bg3::legacy(field_0)]] uint8_t Type;
	FixedString PassiveId;
	[[bg3::legacy(field_8)]] EntityHandle Source;
	[[bg3::legacy(field_10)]] EntityHandle Item;
	[[bg3::legacy(field_18)]] bool ToggledOn;
	[[bg3::legacy(field_19)]] bool Disabled;
	uint32_t field_1C;
};

struct PassiveUsageCount
{
	uint16_t field_0;
	uint16_t field_2;
	uint16_t field_4;
	uint16_t field_6;
	uint16_t field_8;
};

struct UsageCountComponent : public BaseComponent
{
	DEFINE_COMPONENT(PassiveUsageCount, "eoc::passive::UsageCountComponent")

	MultiHashMap<FixedString, PassiveUsageCount> Passives;
};

END_SE()

BEGIN_NS(esv::passive)

struct ToggledPassivesComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerToggledPassives, "esv::passive::ToggledPassivesComponent")

	MultiHashMap<FixedString, bool> Passives;
};

struct PassiveBaseComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerPassiveBase, "esv::passive::BaseComponent")

	Array<FixedString> field_0;
};

struct PersistentDataComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerPassivePersistentData, "esv::passive::PersistentDataComponent")

	int field_0;
	int field_4;
};

struct ScriptPassivesComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerScriptPassives, "esv::passive::ScriptPassivesComponent")

	MultiHashSet<FixedString> Passives;
};

END_NS()
