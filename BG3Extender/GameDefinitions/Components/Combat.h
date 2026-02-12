#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_NS(eoc::combat)

DEFINE_TAG_COMPONENT(eoc::combat, IsInCombatComponent, IsInCombat)
DEFINE_TAG_COMPONENT(eoc::combat, DelayedFanfareComponent, CombatDelayedFanfare)

struct IsThreatenedComponent : public BaseComponent
{
    DEFINE_COMPONENT(CombatIsThreatened, "eoc::combat::IsThreatenedComponent")

    Array<EntityHandle> ThreatenedBy;
};

struct ParticipantComponent : public BaseComponent
{
    DEFINE_COMPONENT(CombatParticipant, "eoc::combat::ParticipantComponent")

    EntityHandle CombatHandle;
    FixedString CombatGroupId;
    [[bg3::legacy(field_C)]] int InitiativeRoll;
    CombatParticipantFlags Flags;
    Guid AiHint;
};

struct CombatPosition
{
    EntityHandle Entity;
    glm::vec3 field_8;
    float field_14;
    double field_18;
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
    Array<CombatPosition> field_B8;
    uint8_t field_D0;
};

struct TurnBasedComponent : public BaseComponent
{
    DEFINE_COMPONENT(TurnBased, "eoc::TurnBasedComponent")

    [[bg3::legacy(IsInCombat_M)]] bool IsActiveCombatTurn;
    [[bg3::legacy(field_A)]] bool Removed;
    bool RequestedEndTurn;
    [[bg3::legacy(field_C), bg3::legacy(ItemCanAct)]] bool TurnActionsCompleted;
    bool ActedThisRoundInCombat;
    bool HadTurnInCombat;
    [[bg3::legacy(CanAct_M)]] bool CanActInCombat;
    [[bg3::legacy(field_18)]] std::optional<float> Timeout;
    [[bg3::legacy(field_20)]] std::optional<float> PauseTimer;
    [[bg3::legacy(field_28)]] std::optional<float> EndTurnHoldTimer;
    [[bg3::legacy(Combat)]] Guid CombatTeam;
};

struct TurnBasedEntityInfo
{
    EntityHandle Entity;
    int32_t Initiative{ 0 };
};

struct EndTurnRequest
{
    Guid Team;
    int Round;
    bool field_14;
    uint64_t field_18;
};

struct TurnBasedGroup
{
    [[bg3::legacy(Handles)]] Array<TurnBasedEntityInfo> Members;
    [[bg3::legacy(Participant)]] Guid Team;
    [[bg3::legacy(field_28)]] uint32_t Round{ 0 };
    int32_t Initiative{ 0 };
    [[bg3::legacy(field_30)]] bool IsPlayer{ false };
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
    float field_4C;
};

struct IsCombatPausedComponent : public BaseComponent
{
    DEFINE_COMPONENT(IsCombatPaused, "eoc::combat::IsCombatPausedComponent")

    [[bg3::legacy(Paused)]] uint8_t PauseSourceTypes;
};

struct ThreatRangeComponent : public BaseComponent
{
    DEFINE_COMPONENT(ThreatRange, "eoc::combat::ThreatRangeComponent")

    float Range;
    float TargetCeiling;
    float TargetFloor;
};

END_NS()

BEGIN_NS(esv::combat)

struct CombatGroupMappingComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerCombatGroupMapping, "esv::combat::CombatGroupMappingComponent")

    EntityHandle Entity;
    HashMap<FixedString, HashSet<EntityHandle>> CombatGroups;
};

struct CombatSwitchedComponent : public BaseComponent
{
    DEFINE_COMPONENT(CombatSwitched, "esv::combat::CombatSwitchedComponent")

    EntityHandle field_0;
    Guid field_8;
    Guid field_18;
    EntityHandle field_28;
};

struct FleeRequestComponent : public BaseComponent
{
    DEFINE_COMPONENT(CombatFleeRequest, "esv::combat::FleeRequestComponent")

    Guid RequestGuid;
    UserId UserID;
};

struct GlobalCombatRequest
{
    EntityHandle Source;
    EntityHandle Target;
    uint32_t Action{ 0 }; // 0 = OnHitJoinCombat, 1 = RequestCombat
    EntityHandle Combat;
};

struct GlobalCombatRequests : public BaseComponent
{
    DEFINE_COMPONENT(GlobalCombatRequests, "esv::combat::GlobalCombatRequests")

    Array<GlobalCombatRequest> Requests;
};

struct JoiningComponent : public BaseComponent
{
    DEFINE_COMPONENT(CombatJoining, "esv::combat::JoiningComponent")

    float field_0;
};

struct LateJoinPenaltyComponent : public BaseComponent
{
    DEFINE_COMPONENT(CombatLateJoinPenalty, "esv::combat::LateJoinPenaltyComponent")

    float field_0;
};

struct MergeComponent : public BaseComponent
{
    DEFINE_COMPONENT(CombatMerge, "esv::combat::MergeComponent")

    EntityHandle Combat1;
    EntityHandle Combat2;
};

struct EnterRequestComponent : public BaseComponent
{
    DEFINE_COMPONENT(ServerEnterRequest, "esv::combat::EnterRequestComponent")

    HashSet<EntityHandle> EnterRequests;
};

struct SurfaceTeamSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(CombatSurfaceTeamSingleton, "esv::combat::SurfaceTeamSingletonComponent")

    HashSet<EntityHandle> field_0;
    HashMap<EntityHandle, Array<Guid>> field_30;
};


DEFINE_TAG_COMPONENT(esv::combat, LeaveRequestComponent, ServerCombatLeaveRequest)
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

struct JoinEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(CombatantJoinEvent, "esv::combat::JoinEventOneFrameComponent")

    EntityHandle Combat;
    int32_t Initiative;
    bool ImmediateJoin;
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
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, FleeSuccessOneFrameComponent, CombatFleeSuccess)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, JoinInCurrentRoundFailedEventOneFrameComponent, CombatJoinInCurrentRoundFailedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, JoinInCurrentRoundOneFrameComponent, CombatJoinInCurrentRound)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, RequestCompletedEventOneFrameComponent, CombatRequestCompletedEvent)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, SurprisedJoinRequestOneFrameComponent, CombatSurprisedJoinRequest)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, SurprisedStealthRequestOneFrameComponent, CombatSurprisedStealthRequest)
DEFINE_ONEFRAME_TAG_COMPONENT(esv::combat, ThreatRangeChangedEventOneFrameComponent, CombatThreatRangeChangedEvent)

struct CombatGroupUpdate
{
    EntityHandle Combat;
    FixedString CombatGroup;
    bool field_C{ false };
};

struct NarrativeCombatRequest
{
    NarrativeCombatRequestType Action{ NarrativeCombatRequestType::Create };
    EntityHandle Entity;
    Guid CombatGuid;
};


struct System : public BaseSystem
{
    DEFINE_SYSTEM(ServerCombat, "esv::combat::System")

    [[bg3::hidden]] void* VMT_NetEventListener;
    [[bg3::hidden]] __int64 field_18;
    [[bg3::hidden]] void* VMT_GameEventListener;
    [[bg3::hidden]] UnknownSignal field_28;
    [[bg3::hidden]] UnknownSignal field_40;
    [[bg3::hidden]] UnknownSignal field_58;
    [[bg3::hidden]] UnknownSignal field_70;
    [[bg3::hidden]] void* FactionContainer;
    [[bg3::hidden]] void* EoCGlobalSwitches;
    [[bg3::hidden]] void* GameControl;
    [[bg3::hidden]] void* StatusPrototypeManager;
    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] void* SpellPrototypeManager;
    [[bg3::hidden]] void* ShroudManager;
    [[bg3::hidden]] void* AiHelpers;
    [[bg3::hidden]] void* StreamRollManager;
    [[bg3::hidden]] void* CombatSystemHelper;
    HashSet<Guid> EndCombat;
    HashSet<Guid> SkipCombatStartDelay;
    HashSet<FixedString> DestroyCombats;
    HashMap<EntityHandle, CombatPauseSourceType> ResumeCombat;
    HashMap<EntityHandle, CombatPauseSourceType> PauseCombat;
    HashSet<EntityHandle> ResetCombatTimer;
    HashMap<EntityHandle, EntityHandle> JoinCombat;
    HashMap<EntityHandle, EntityHandle> JoinNarrativeCombat;
    Array<CombatGroupUpdate> CombatGroupUpdates;
    HashSet<EntityHandle> ResetTurn;
    HashSet<EntityHandle> LeaveCombat;
    Array<GlobalCombatRequest> CombatRequests;
    Array<NarrativeCombatRequest> NarrativeCombatRequests;
    bool field_328;
    bool field_329;
    [[bg3::hidden]] UnknownFunction field_330;
    HashMap<EntityHandle, UserId> Flee;
    HashSet<uint32_t> ActivatedPeers_PeerId;
    HashSet<uint32_t> field_3E0_PeerId;
    HashSet<uint32_t> field_410_PeerId;
};


struct TurnOrderAssignEntityToUserRequestOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(TurnOrderAssignEntityToUserRequest, "esv::TurnOrderAssignEntityToUserRequestOneFrameComponent")

    UserId UserID;
    Guid field_8;
};

struct TurnEndedInfo
{
    int Round;
    Guid Team;
    float ActionPoints;
    float BonusActionPoints;
    float MovementPoints;
    int field_24;
};

struct TurnStartedInfo
{
    Guid Team;
    int Round;
};

struct TurnEndedEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(TurnEndedEvent, "esv::TurnEndedEventOneFrameComponent")

    Array<TurnEndedInfo> TurnEnded;
};

struct TurnStartedEventOneFrameComponent : public BaseComponent
{
    DEFINE_ONEFRAME_COMPONENT(TurnStartedEvent, "esv::TurnStartedEventOneFrameComponent")

    Array<TurnStartedInfo> TurnStarted;
};

DEFINE_TAG_COMPONENT(esv, TurnOrderSkippedComponent, TurnOrderSkipped)
DEFINE_ONEFRAME_TAG_COMPONENT(esv, TurnOrderTimedOutOneFrameComponent, TurnOrderTimedOut)

struct TurnActionCompletedRequest
{
    EntityHandle Entity;
    bool ActionsCompleted;
};

struct TurnOrderSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerTurnOrder, "esv::TurnOrderSystem")

    [[bg3::hidden]] void* GameStateEventListenerVMT;
    [[bg3::hidden]] UnknownSignal field_18;
    [[bg3::hidden]] UnknownSignal field_30;
    HashMap<EntityHandle, float> SetTimeout;
    Array<TurnActionCompletedRequest> TurnCompleted;
    Array<EntityHandle> ClearTimeout;
    Array<EntityHandle> EndTurn;
    Array<EntityHandle> CancelEndTurn;
    Array<EntityHandle> BeginCancelEndTurn;
    Array<EntityHandle> EndCancelEndTurn;
    bool GameState13;
    [[bg3::hidden]] void* EocServer;
    [[bg3::hidden]] void* LevelManager;
    [[bg3::hidden]] void* StoryGameEventManager;
    [[bg3::hidden]] void* StatusPrototypeManager;
    [[bg3::hidden]] void* TurnOrderSystemHelper;
};


END_NS()

BEGIN_NS(esv::turn)

struct SurfaceTeamSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(TurnSurfaceTeamSingleton, "esv::turn::SurfaceTeamSingletonComponent")

    HashSet<EntityHandle> EndRound;
    HashMap<EntityHandle, Array<Guid>> EndTurn;
};

struct SurfaceTrackingComponent : public BaseComponent
{
    DEFINE_COMPONENT(TurnSurfaceTracking, "esv::turn::SurfaceTrackingComponent")

    HashSet<EntityHandle> Surfaces;
};

DEFINE_ONEFRAME_TAG_COMPONENT(esv::turn, RoundEndedEventOneFrameComponent, TurnRoundEndedEvent)

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

    uint8_t field_0;
    HashMap<FixedString, float> Modifiers;
    //HashMap<FixedString, HashMap<int, ModifierChange>> Changes;
};

END_NS()


BEGIN_NS(eoc::ftb)

struct ParticipantComponent : public BaseComponent
{
    DEFINE_COMPONENT(FTBParticipant, "eoc::ftb::ParticipantComponent")

    EntityHandle field_18;
};

struct ZoneBlockReasonComponent : public BaseComponent
{
    DEFINE_COMPONENT(FTBZoneBlockReason, "eoc::ftb::ZoneBlockReasonComponent")

    uint8_t Reason;
};

DEFINE_TAG_COMPONENT(eoc::ftb, IsFtbPausedComponent, FTBPaused)
DEFINE_TAG_COMPONENT(eoc::ftb, IsInFtbComponent, IsInFTB)
DEFINE_TAG_COMPONENT(eoc::ftb, RespectComponent, FTBRespect)

END_NS()


BEGIN_NS(esv::ftb)

struct SurfaceTeamSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(FTBSurfaceTeamSingleton, "esv::ftb::SurfaceTeamSingletonComponent")

    HashSet<EntityHandle> field_0;
    HashMap<EntityHandle, Array<Guid>> field_30;
};

struct TimeFactorRequestsSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(FTBTimeFactorRequests, "esv::ftb::TimeFactorRequestsSingletonComponent")

    HashMap<EntityHandle, float> Requests;
};

struct TimeFactorResetRequestsSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(FTBTimeFactorResetRequests, "esv::ftb::TimeFactorResetRequestsSingletonComponent")

    HashSet<EntityHandle> Requests;
};

struct TurnBasedChangesRequest
{
    std::optional<bool> IsActiveCombatTurn;
    std::optional<bool> Removed;
    std::optional<bool> RequestedEndTurn;
    std::optional<bool> TurnActionsCompleted;
    std::optional<bool> ActedThisRoundInCombat;
    std::optional<bool> HadTurnInCombat;
    std::optional<bool> CanActInCombat;
    std::optional<std::optional<float>> Timeout;
    std::optional<std::optional<float>> PauseTimer;
    std::optional<std::optional<float>> EndTurnHoldTimer;
    std::optional<Guid> CombatTeam;
};

struct TurnBasedChangesRequestSingletonComponent : public BaseComponent
{
    DEFINE_COMPONENT(FTBTurnBasedChangesRequest, "esv::ftb::TurnBasedChangesRequestSingletonComponent")

    HashMap<EntityHandle, TurnBasedChangesRequest> Requests;
};

struct ZoneComponent : public BaseComponent
{
    DEFINE_COMPONENT(FTBZone, "esv::ftb::ZoneComponent")

    EntityHandle Entity;
    uint8_t field_8;
    EntityHandle field_10;
    uint8_t field_18;
    float field_1C;
    FixedString field_20;
    Guid ZoneGuid;
    EntityHandle Creator;
    bool ByPlayer;
    bool Shared;
};

struct ZoneInstigatorComponent : public BaseComponent
{
    DEFINE_COMPONENT(FTBZoneInstigator, "esv::ftb::ZoneInstigatorComponent")

    EntityHandle Instigator;
    bool Shared;
};

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

struct ToggleFTBParameters
{
    bool ByPlayer;
    bool Shared;
    bool Enter;
};

struct ZoneSystem : public BaseSystem
{
    DEFINE_SYSTEM(ServerFTBZone, "esv::ftb::ZoneSystem")

    [[bg3::hidden]] UnknownSignal field_10;
    [[bg3::hidden]] UnknownSignal field_28;
    Array<esv::combat::TurnActionCompletedRequest> SetTurnActionsCompleted;
    HashMap<EntityHandle, ToggleFTBParameters> ToggleForCharacter;
    HashSet<EntityHandle> EndTurn;
    HashSet<EntityHandle> LeaveExistingSharedFTB;
    HashMap<EntityHandle, EntityHandle> EnterExistingSharedFTB;
    HashMap<EntityHandle, Array<EntityHandle>> CancelEndTurn;
    HashMap<EntityHandle, Array<EntityHandle>> BeginCancelEndTurn;
    HashMap<EntityHandle, Array<EntityHandle>> EndCancelEndTurn;
    [[bg3::hidden]] UnknownFunction qword1F0;
    [[bg3::hidden]] void* ZoneSystemHelper;
    [[bg3::hidden]] void* ActivationManager;
    [[bg3::hidden]] void* GameControl;
    [[bg3::hidden]] void* LevelManager;
};

END_NS()
