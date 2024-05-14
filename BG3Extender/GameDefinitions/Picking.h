#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(ecl)

struct PlayerPickingHelperInner2
{
	glm::vec3 CharacterPosition;
	glm::vec3 DeadCharacterPosition;
	glm::vec3 ItemPosition;
	glm::vec3 field_24;
	EntityHandle Character;
	EntityHandle DeadCharacter;
	EntityHandle Item;
	EntityHandle field_48;
	EntityHandle GameObject;
	EntityHandle field_58;
	uint8_t Flags;
};


struct PlayerPickingHelperInner
{
	std::array<PlayerPickingHelperInner2, 2> Inner;
	glm::vec3 Position0;
	glm::vec3 Direction0;
	glm::vec3 Position1;
	glm::vec3 Direction1;
	glm::vec3 Position2;
	glm::vec3 Direction2;
	glm::vec3 field_118;
	glm::vec3 field_124;
	EntityHandle field_130_EH;
	EntityHandle field_138_EH;
	uint64_t field_140_Flags2;
	float field_148;
	float field_14C;
	float field_150;
	uint8_t field_154_Flags;
};

struct PlayerPickingHelper : public ProtectedGameObject<PlayerPickingHelper>
{
	struct MHEntry
	{
		int field_0;
		int field_4;
		uint8_t field_8;
	};

	struct Field170Entry
	{
		EntityHandle field_0_Entity;
		int field_8_TurnOrder;
		uint8_t field_C_InCombat;
		float field_10;
		float field_14;
		float field_18;
		float field_1C;
		float field_20;
		float field_24;
	};

	[[bg3::hidden]] void* VMT;
	PlayerPickingHelperInner Inner;
	[[bg3::hidden]] void* field_160;
	uint16_t PlayerId;
	uint8_t field_16A;
	Array<Field170Entry> field_170;
	std::optional<Field170Entry> field_180;
	float field_1AC;
	float field_1B0;
	uint8_t field_1B4;
	uint8_t field_1B5;
	uint8_t field_1B6;
	uint8_t field_1B7;
	uint8_t field_1B8;
	uint8_t field_1B9;
	EntityHandle field_1C0;
	float field_1C8_SteeringRotM;
	int field_1CC;
	float field_1D0_SteeringRotM2;
	float ControllerNudge;
	float ControllerNudge2;
	bool field_1DC;
	glm::vec2 WindowCursorPos;
	[[bg3::hidden]] void* field_1E8;
	[[bg3::hidden]] void* field_1F0;
	[[bg3::hidden]] void* field_1F8;
	[[bg3::hidden]] void* field_200;
	[[bg3::hidden]] void* field_208;
	uint8_t field_210;
	glm::vec3 field_218;
	MultiHashMap<EntityHandle, MHEntry> field_228_EH_unk;
	int field_268_NextIndex;
	float field_26C;
	MultiHashMap<EntityHandle, MHEntry> field_270_EH_unk;
	int field_2B0;
	float field_2B4;
	MultiHashMap<EntityHandle, MHEntry> field_2B8_EH_MHentry;
	int field_2F8;
	bool IsMoving;
};


struct [[bg3::hidden]] PickingHelperManager : public ProtectedGameObject<PickingHelperManager>
{
	static constexpr auto SystemType = ExtSystemType::PickingHelperManager;

	void* VMT;
	void* field_8;
	RefMap<uint16_t, PlayerPickingHelper*> PlayerHelpers;
	void* field_20;
};


END_NS()
