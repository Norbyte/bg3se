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
	HashMap<EntityHandle, int32_t> Initiatives;
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

	uint8_t field_8;
	[[bg3::legacy(IsInCombat_M)]] bool IsActiveCombatTurn;
	bool field_A;
	bool RequestedEndTurn;
	[[bg3::legacy(field_C)]] bool ItemCanAct;
	int field_10;
	bool ActedThisRoundInCombat;
	bool HadTurnInCombat;
	[[bg3::legacy(CanAct_M)]] bool CanActInCombat;
	int field_18;
	uint8_t field_1C;
	int field_20;
	[[bg3::legacy(field_24)]] bool CanActInCombat2;
	uint64_t field_28;
	Guid Combat;
};

struct TurnBasedEntityInfo
{
	EntityHandle Entity;
	int32_t Initiative;
};

struct TurnBasedGroup
{
	Array<TurnBasedEntityInfo> Handles;
	Guid Participant;
	uint32_t field_28;
	int32_t Initiative;
	uint8_t field_30;
};

struct TurnOrderComponent : public BaseComponent
{
	DEFINE_COMPONENT(TurnOrder, "eoc::TurnOrderComponent")

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

	[[bg3::legacy(Paused)]] uint8_t PauseSourceTypes;
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
	HashMap<FixedString, HashSet<EntityHandle>> CombatGroups;
};

struct EnterRequestComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerEnterRequest, "esv::combat::EnterRequestComponent")

	HashSet<EntityHandle> EnterRequests;
};

DEFINE_TAG_COMPONENT(esv::combat, CanStartCombatComponent, ServerCanStartCombat)
DEFINE_TAG_COMPONENT(esv::combat, ImmediateJoinComponent, ServerImmediateJoin)
DEFINE_TAG_COMPONENT(esv::combat, FleeBlockedComponent, ServerFleeBlocked)



struct CombatantKilledEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(CombatantKilledEvent, "esv::combat::CombatantKilledEventOneFrameComponent")

	EntityHandle field_0;
	EntityHandle field_8;
	uint8_t field_10;
	uint8_t field_11;
	uint8_t field_12;
};

struct LeftEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(CombatLeftEvent, "esv::combat::LeftEventOneFrameComponent")

	EntityHandle field_0;
	Guid field_8;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, CombatScheduledForDeleteOneFrameComponent, CombatScheduledForDelete)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, CombatStartedEventOneFrameComponent, CombatStartedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, DelayedFanfareRemovedDuringCombatEventOneFrameComponent, DelayedFanfareRemovedDuringCombatEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, JoinInCurrentRoundFailedEventOneFrameComponent, CombatJoinInCurrentRoundFailedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, JoinInCurrentRoundOneFrameComponent, CombatJoinInCurrentRound)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, RequestCompletedEventOneFrameComponent, CombatRequestCompletedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, SurprisedJoinRequestOneFrameComponent, CombatSurprisedJoinRequest)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, SurprisedStealthRequestOneFrameComponent, CombatSurprisedStealthRequest)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, ThreatRangeChangedEventOneFrameComponent, CombatThreatRangeChangedEvent)

END_NS()

BEGIN_NS(esv::ai::combat)

struct InterestedInItemsComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerAiInterestedInItems, "esv::ai::combat::InterestedInItemsComponent")

	HashSet<EntityHandle> Items;
};

struct InterestingItemComponent : public BaseComponent
{
	DEFINE_COMPONENT(ServerAiInterestingItem, "esv::ai::combat::InterestingItemComponent")

	HashSet<EntityHandle> Items;
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
	HashMap<FixedString, float> Modifiers;
	//HashMap<FixedString, HashMap<int, ModifierChange>> Changes;
};

END_NS()


BEGIN_NS(esv::ftb)

struct ModeChangedEventOneFrameComponent : public BaseComponent
{
	DEFINE_ONEFRAME_COMPONENT(FTBModeChangedEvent, "esv::ftb::ModeChangedEventOneFrameComponent")

	EntityHandle Entity;
	bool InFTB;
	uint8_t field_9;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::ftb, PlayersTurnEndedEventOneFrameComponent, FTBPlayersTurnEndedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::ftb, PlayersTurnStartedEventOneFrameComponent, FTBPlayersTurnStartedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::ftb, RoundEndedEventOneFrameComponent, FTBRoundEndedEvent)

END_NS()
