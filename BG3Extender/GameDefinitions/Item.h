#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/GameState.h>

BEGIN_NS(esv)

struct Item
{
	void* VMT;
	void* VMT2;
	EntityHandle field_10;
	ItemFlags Flags;
	FixedString Level;
	ecs::EntityRef MyHandle;
	Array<UserId> UpdatePeerIds;
	void* Template;
	void* OriginalTemplate;
	void* ItemMachine;
	void* PlanManager;
	void* VariableManager;
	void* StatusManager;
	stats::Object* StatsObject;
	uint64_t field_58;
	uint64_t field_60;
	EntityHandle field_68;
	FixedString field_70;
	FixedString Stats;
	FixedString ItemType;
	FixedString PreviousLevel;
	int32_t TreasureLevel;
	int32_t Amount;
	uint8_t MovingCount;
	uint8_t InheritedForceSynch;
	uint8_t ForceSynch;
	uint8_t DisableUse;
	ItemFlags2 Flags2;
	uint64_t field_90;
	uint64_t field_98;
	uint64_t field_A0;
	uint64_t field_A8;
};

struct ItemComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerItem;
	static constexpr auto EngineClass = "esv::Item";

	esv::Item* Item;
};

END_NS()
