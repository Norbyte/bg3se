#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/GameState.h>

BEGIN_NS(esv)

struct Item : public BaseProxyComponent
{
	DEFINE_COMPONENT(ServerItem, "esv::Item")

	void* VMT;
	void* VMT2;
	EntityHandle field_10;
	ItemFlags Flags;
	ecs::EntityRef MyHandle;
	FixedString Level;
	FixedString ItemType;
	Array<UserId> UpdatePeerIds;
	ItemTemplate* Template;
	ItemTemplate* OriginalTemplate;
	void* ItemMachine;
	void* PlanManager;
	void* VariableManager;
	StatusMachine* StatusManager;
	stats::Object* StatsObject;
	uint64_t field_58;
	uint64_t field_60;
	EntityHandle field_68;
	FixedString field_70;
	FixedString Stats;
	FixedString PreviousLevel;
	int32_t TreasureLevel;
	int32_t Amount;
	ItemFlags2 Flags2;
	[[bg3::hidden]] void* _PAD;

	RefReturn<Item> LuaGetSelf() const;
};

END_NS()

BEGIN_NS(ecl)

struct SceneryInitSound
{
	int16_t Attenuation;
	FixedString InitSound;
	FixedString LoopSound;
};

struct Scenery : public BaseProxyComponent
{
	DEFINE_COMPONENT(Scenery, "ecl::Scenery")

	void* VMT;
	void* VMT2;
	EntityHandle Entity2;
	SceneryFlags Flags;
	FixedString CurrentLevel;
	ecs::EntityRef Entity;
	Guid Uuid;
	SceneryInitSound* Sound;
	SceneryTemplate* Template;
	FixedString Visual;
	uint16_t field_54;
	STDString field_58;
	Guid TemplateId;
};

END_NS()
