#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_NS(combat)

DEFINE_TAG_COMPONENT(eoc::combat, IsInCombatComponent, IsInCombat)

struct ParticipantComponent : public BaseComponent
{
	DEFINE_COMPONENT(CombatParticipant, "eoc::combat::ParticipantComponent")

	EntityHandle CombatHandle;
	FixedString CombatGroupId;
	[[bg3::legacy(field_C)]] int InitiativeRoll;
	CombatParticipantFlags Flags;
	Guid AiHint;
};

struct StateComponent : public BaseComponent
{
	DEFINE_COMPONENT(CombatState, "eoc::combat::StateComponent")

	Guid MyGuid;
	MultiHashMap<EntityHandle, int32_t> Initiatives;
	Array<EntityHandle> Participants;
	EntityHandle field_98;
	EntityHandle field_A0;
	FixedString Level;
	uint8_t field_AC;
	bool IsInNarrativeCombat;
	float field_B0;
	Array<EntityHandle> field_B8;
	uint8_t field_D0;
};

struct TurnBasedComponent : public BaseComponent
{
	DEFINE_COMPONENT(TurnBased, "eoc::TurnBasedComponent")

	BYTE field_8;
	bool IsInCombat_M;
	bool field_A;
	bool RequestedEndTurn;
	bool field_C;
	int field_10;
	bool ActedThisRoundInCombat;
	bool HadTurnInCombat;
	bool CanAct_M;
	int field_18;
	uint8_t field_1C;
	int field_20;
	uint8_t field_24;
	uint64_t field_28;
	Guid Combat;
};

struct TurnOrderComponent : public BaseComponent
{
	DEFINE_COMPONENT(TurnOrder, "eoc::TurnOrderComponent")

	struct ParticipantHandleInfo
	{
		EntityHandle Entity;
		int32_t Initiative;
	};

	struct TurnBasedGroup
	{
		Array<ParticipantHandleInfo> Handles;
		Guid Participant;
		uint32_t field_28;
		int32_t Initiative;
		uint8_t field_30;
	};

	Array<uint64_t> TurnOrderIndices;
	[[bg3::legacy(Participants)]] Array<TurnBasedGroup> Groups;
	Array<uint64_t> TurnOrderIndices2;
	[[bg3::legacy(Participants2)]] Array<TurnBasedGroup> Groups2;
	int field_40;
	int field_44;
	int field_48;
	int field_4C;
};

struct IsCombatPausedComponent : public BaseComponent
{
	DEFINE_COMPONENT(IsCombatPaused, "eoc::combat::IsCombatPausedComponent")

	bool Paused;
};

struct ThreatRangeComponent : public BaseComponent
{
	DEFINE_COMPONENT(ThreatRange, "eoc::combat::ThreatRangeComponent")

	float field_0;
	int field_4;
	int field_8;
};


END_NS()

BEGIN_NS(esv::combat)

struct CombatGroupMappingComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerCombatGroupMapping, "esv::combat::CombatGroupMappingComponent")

	EntityHandle Entity;
	MultiHashMap<FixedString, MultiHashSet<EntityHandle>> CombatGroups;
};

struct EnterRequestComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerEnterRequest, "esv::combat::EnterRequestComponent")

	MultiHashSet<EntityHandle> EnterRequests;
};

DEFINE_TAG_COMPONENT(esv::combat, CanStartCombatComponent, ServerCanStartCombat)
DEFINE_TAG_COMPONENT(esv::combat, ImmediateJoinComponent, ServerImmediateJoin)
DEFINE_TAG_COMPONENT(esv::combat, FleeBlockedComponent, ServerFleeBlocked)

END_NS()

BEGIN_NS(esv::ai::combat)

struct InterestedInItemsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerAiInterestedInItems, "esv::ai::combat::InterestedInItemsComponent")

	MultiHashSet<EntityHandle> Items;
};

struct InterestingItemComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerAiInterestingItem, "esv::ai::combat::InterestingItemComponent")

	MultiHashSet<EntityHandle> Items;
};

struct ArchetypeComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerAiArchetype, "esv::ai::combat::ArchetypeComponent")

	FixedString field_0;
	FixedString field_4;
	FixedString field_8;
	FixedString field_C;
};

struct ModifierChange
{
	STDString field_0;
	STDString field_18;
	float field_30;
};

struct AiModifiersComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerAiModifiers, "esv::ai::combat::AiModifiersComponent")

	uint64_t field_0;
	MultiHashMap<FixedString, float> Modifiers;
	//MultiHashMap<FixedString, MultiHashMap<int, ModifierChange>> Changes;
};

END_NS()
