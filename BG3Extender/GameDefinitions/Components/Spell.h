#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Hit.h>

BEGIN_NS(interrupt)

struct InterruptType0
{
	FixedString field_0;
	Guid field_8;
	SpellId field_18;
};

struct InterruptType1
{
	__int64 field_0;
	Guid field_8;
	int field_18;
	uint8_t field_1C;
	int field_20;
	Array<EntityHandle> field_28;
	EntityHandle field_38;
	SpellId field_40;
	uint8_t field_68;
	uint8_t field_69;
};

struct InterruptType2
{
	Guid field_0;
	FixedString field_10;
	Guid field_18;
	uint8_t field_28;
	uint8_t _Pad[3];
	uint8_t field_2C;
	uint8_t field_2D;
	int field_30;
	int field_34;
	int field_38;
	uint8_t field_3C;
	uint8_t field_3D;
	int field_40;
	uint8_t field_44;
	int field_48;
	uint8_t field_4C;
	uint8_t _Pad2[3];
	uint8_t field_50;
	uint8_t field_51;
	uint8_t field_52;
	Guid field_58;
	SpellId field_68;
};

struct InterruptType3
{
	__int64 field_0;
	__int64 field_8;
	__int64 field_10;
	__int64 field_18;
	__int64 field_20;
	__int64 field_28;
	FixedString field_30;
	uint8_t field_34;
	__int64 field_38;
	__int64 field_40;
	__int64 field_48;
	int field_50;
	uint8_t field_54;
	uint8_t field_55;
};

struct InterruptType4
{
	FixedString field_0;
	glm::vec3 field_4;
};

struct InterruptType5
{
	FixedString field_0;
	glm::vec3 field_4;
};

struct InterruptType6
{
	Guid field_0;
	FixedString field_10;
	Guid field_18;
	STDString field_28;
	char field_40;
	int field_44;
	char field_48;
	char field_49;
	Guid field_50;
	char field_60;
	Guid field_68;
	char field_78;
	SpellId field_80;
};

struct InterruptType7
{
	Guid field_0;
	Guid field_10;
	FixedString field_20;
	Guid field_28;
	Guid field_38;
	uint8_t field_48;
	__int64 field_50;
	FixedString field_58;
	int field_5C;
};

struct InterruptType8
{
	Guid field_0;
	FixedString field_10;
	Guid field_18;
	Guid field_28;
	int field_38;
	__int64 field_40;
	char field_48;
	int field_4C;
};

struct InterruptVariant2
{
	std::variant<InterruptType0, InterruptType1, InterruptType2, InterruptType3, InterruptType4, InterruptType5, InterruptType6, InterruptType7, InterruptType8> Variant;
	EntityHandle field_B0;
	EntityHandle field_B8;
	EntityHandle field_C0;
	EntityHandle field_C8;
	Array<std::pair<EntityHandle, EntityHandle>> field_D0;
	__int64 field_E0;
	__int64 field_E8;
	__int64 field_F0;
	__int64 field_F8;
	uint8_t field_100;
};

struct InterruptVariantContainer
{
	InterruptVariant2 Variant;
	bool field_108;
	InterruptVariant2 field_110;
	MultiHashMap<EntityHandle, MultiHashSet<EntityHandle>> MHM_EH_MHS_EH;
};


struct ActionStateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InterruptActionState;
	static constexpr auto EngineClass = "eoc::interrupt::ActionStateComponent";

	InterruptVariant2 Variant;
	Array<std::pair<EntityHandle, EntityHandle>> Arr_EHx2;
	Guid field_118;
};

struct ConditionallyDisabledComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InterruptConditionallyDisabled;
	static constexpr auto EngineClass = "eoc::interrupt::ConditionallyDisabledComponent";

	uint8_t Dummy;
};

struct ContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InterruptContainer;
	static constexpr auto EngineClass = "eoc::interrupt::ContainerComponent";

	Array<EntityHandle> Interrupts;
};

struct DataComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InterruptData;
	static constexpr auto EngineClass = "eoc::interrupt::DataComponent";

	FixedString field_0;
	uint8_t field_4;
	EntityHandle field_8;
	EntityHandle field_10;
	FixedString field_18;
};

struct DecisionComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InterruptDecision;
	static constexpr auto EngineClass = "eoc::interrupt::DecisionComponent";

	// We cannot serialize composite keys yet!
	[[bg3::hidden]]
	MultiHashMap<InterruptVariant2, uint8_t> Decisions;
};

struct PreferencesComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InterruptPreferences;
	static constexpr auto EngineClass = "eoc::interrupt::PreferencesComponent";

	MultiHashMap<FixedString, uint8_t> Preferences;
};

struct PreparedComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InterruptPrepared;
	static constexpr auto EngineClass = "eoc::interrupt::PreparedComponent";

	uint8_t Dummy;
};

struct ZoneComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InterruptZone;
	static constexpr auto EngineClass = "eoc::interrupt::ZoneComponent";

	Guid field_0;
};

struct ZoneParticipantComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InterruptZoneParticipant;
	static constexpr auto EngineClass = "eoc::interrupt::ZoneParticipantComponent";

	MultiHashMap<EntityHandle, uint8_t> field_0;
};

struct ZoneSourceComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::InterruptZoneSource;
	static constexpr auto EngineClass = "eoc::interrupt::ZoneSourceComponent";

	uint8_t Dummy;
};

END_NS()

BEGIN_NS(spell)

struct SpellContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellContainer;
	static constexpr auto EngineClass = "eoc::spell::ContainerComponent";

	struct Spell
	{
		SpellIdBase SpellId;
		EntityHandle ItemHandle;
		SpellChildSelectionType SelectionType;
		uint8_t field_29;
		Guid SpellUUID;
		AbilityId SpellCastingAbility;
		SpellCooldownType CooldownType;
		FixedString ContainerSpell;
		uint8_t field_48;
	};

	Array<Spell> Spells;
};

struct PlayerPrepareSpellComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::PlayerPrepareSpell;
	static constexpr auto EngineClass = "eoc::spell::PlayerPrepareSpellComponent";

	struct Spell
	{
		FixedString field_0;
		int32_t _Pad;
		uint8_t field_8;
		Guid field_10;
	};

	Array<Spell> Spells;
	uint8_t field_30;
};

struct CCPrepareSpellComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCPrepareSpell;
	static constexpr auto EngineClass = "eoc::spell::CCPrepareSpellComponent";

	Array<PlayerPrepareSpellComponent::Spell> Spells;
};

struct SpellCastComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellCast;
	static constexpr auto EngineClass = "eoc::spell::CastComponent";

	FixedString field_18;
	uint8_t field_1C;
	__int64 field_20;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	uint8_t field_38;
	uint8_t field_39;
	uint8_t field_3A;
	uint8_t field_3B;
};

struct SpellModificationContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellModificationContainer;
	static constexpr auto EngineClass = "eoc::spell::ModificationContainerComponent";

	struct Variant0
	{
		uint64_t Value;
	};

	struct Variant1
	{
		uint64_t Value;
	};

	struct Variant2
	{
		glm::vec3 Value;
	};

	struct Variant3
	{
		uint8_t Value;
	};

	struct Variant4
	{
		uint8_t Value;
		uint8_t field_1;
	};

	struct Variant5
	{
		STDString field_0;
		STDString field_18;
		int field_30;
	};

	struct Variant6
	{
		uint64_t Value;
	};

	struct Variant7
	{
		uint64_t Value;
	};

	struct Variant8
	{
		uint64_t Value;
	};

	struct Variant9
	{
		uint64_t Value;
	};

	struct Variant10
	{
		Guid field_0;
		char field_10;
		STDString field_18;
		int field_30;
		Guid field_38;
	};

	struct Variant11
	{
		uint8_t Value;
	};

	struct Variant12
	{
		uint8_t Value;
	};

	struct Variant13
	{
		uint8_t Value;
	};

	struct Modification
	{
		uint8_t field_0;
		FixedString field_4;
		std::variant<Variant0, Variant1, Variant2, Variant3, Variant4, Variant5, Variant6, Variant6, Variant8, Variant9, Variant10, Variant11, Variant12, Variant13> field_8;
		MultiHashSet<SpellId> Spells;
	};

	MultiHashMap<FixedString, Array<Modification>> Modifications;
};

struct AddedSpellsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::AddedSpells;
	static constexpr auto EngineClass = "eoc::spell::AddedSpellsComponent";

	Array<SpellContainerComponent::Spell> Spells;
};

struct SpellBookCooldownsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellBookCooldowns;
	static constexpr auto EngineClass = "eoc::spell::BookCooldownsComponent";

	struct Cooldown
	{
		SpellId SpellId;
		SpellCooldownType CooldownType;
		uint8_t field_29;
		float Cooldown;
		Guid field_30;
	};

	Array<Cooldown> Cooldowns;
};

struct LearnedSpellsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::LearnedSpells;
	static constexpr auto EngineClass = "eoc::spell::LearnedSpellsComponent";

	MultiHashMap<Guid, MultiHashSet<FixedString>> field_18;
	MultiHashSet<uint8_t> field_70;
};

struct SpellAiConditionsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellAiConditions;
	static constexpr auto EngineClass = "eoc::spell::AiConditionsComponent";

	MultiHashMap<FixedString, uint64_t> field_18;
};

struct SpellBookEntry
{
	struct InnerEntry
	{
		uint8_t field_0;
		MultiHashSet<int32_t> field_8;
	};

	SpellId Id;
	Guid SpellUUID;
	int32_t field_38;
	int32_t field_3C;
	SpellCooldownType CooldownType;
	uint8_t field_41;
	uint8_t field_42;
	AbilityId SpellCastingAbility;
	Array<InnerEntry> InnerEntries;
};

struct BookComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellBook;
	static constexpr auto EngineClass = "eoc::spell::BookComponent";

	uint64_t field_0;
	Array<SpellBookEntry> Spells;
};

struct BookPreparesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellBookPrepares;
	static constexpr auto EngineClass = "eoc::spell::BookPreparesComponent";

	Array<SpellIdBase> PreparedSpells;
	MultiHashMap<Guid, int> field_30;
	MultiHashMap<Guid, int> field_88;
};

END_NS()

BEGIN_NS(spell_cast)

struct AnimationInfoComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellCastAnimationInfo;
	static constexpr auto EngineClass = "eoc::spell_cast::AnimationInfoComponent";

	uint8_t field_0;
	glm::vec3 field_4;
	uint8_t field_10;
	glm::vec3 field_14;
	EntityHandle field_20;
	uint8_t field_28;
	uint8_t field_29;
	uint8_t field_2A;
	uint8_t field_2B;
	uint8_t field_2C;
	uint8_t field_2D;
	uint8_t field_2E;
};

struct CacheComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellCastCache;
	static constexpr auto EngineClass = "eoc::spell_cast::CacheComponent";

	uint8_t field_0;
	uint32_t field_4;
};

struct CanBeTargetedComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellCastCanBeTargeted;
	static constexpr auto EngineClass = "eoc::spell_cast::CanBeTargetedComponent";

	uint8_t Dummy;
};

struct IsCastingComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellCastIsCasting;
	static constexpr auto EngineClass = "eoc::spell_cast::IsCastingComponent";
	
	EntityHandle Cast;
};

struct InterruptResultsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellCastInterruptResults;
	static constexpr auto EngineClass = "eoc::spell_cast::InterruptResultsComponent";
	
	char field_0;
	MultiHashSet<EntityHandle> Results;
};

struct MovementComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellCastMovement;
	static constexpr auto EngineClass = "eoc::spell_cast::MovementComponent";

	glm::vec3 field_0;
	glm::vec3 field_C;
	bool field_18;
};

struct RollsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellCastRolls;
	static constexpr auto EngineClass = "eoc::spell_cast::RollsComponent";

	struct RollHit
	{
		FixedString field_0;
		Hit Hit;
	};

	struct Roll
	{
		EntityHandle field_0;
		std::optional<EntityHandle> field_8;
		Array<RollHit> Hits;
		MultiHashMap<FixedString, int32_t> MHS_FS_i32;
		int field_68;
		uint8_t field_6C;
		__int64 field_70;
		__int64 field_78;
		__int64 field_80;
		uint8_t field_88;
	};

	Array<Roll> Rolls;
};

struct ReposeState
{
	__int64 field_0;
	EntityHandle field_8;
	std::optional<EntityHandle> field_10;
	std::optional<glm::vec3> field_20;
};

struct ReposeState2
{
	ReposeState Repose;
	uint8_t field_30;
	std::optional<ReposeState> Repose2;
};

struct StateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellCastState;
	static constexpr auto EngineClass = "eoc::spell_cast::StateComponent";

	EntityHandle Entity;
	EntityHandle field_8;
	SpellIdBase SpellId;
	int field_38;
	Array<ReposeState2> Repose;
	std::optional<glm::vec3> field_50;
	std::optional<glm::vec3> field_60;
	glm::vec3 field_70;
	EntityHandle field_80;
	int field_88;
	Guid field_90;
	STDString field_A0;
};

struct SyncTargetingComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::SpellSyncTargeting;
	static constexpr auto EngineClass = "eoc::spell_cast::SyncTargetingComponent";

	[[bg3::hidden]]
	void* VMT;
	EntityHandle field_8;
	std::optional<EntityHandle> field_10;
	std::optional<glm::vec3> field_20;
	Array<ReposeState2> Repose;
	uint8_t field_40;
	int field_44;
	std::optional<glm::vec3> field_48;
	std::optional<glm::vec3> field_58;
	std::optional<glm::vec3> field_68;
	std::optional<glm::vec3> field_78;
	std::optional<glm::vec3> field_88;
};

END_NS()

BEGIN_NS(esv::interrupt)

struct ActionRequest1
{
	__int64 field_0;
	__int64 field_8;
	bg3se::interrupt::InterruptVariant2 field_10;
	MultiHashMap<EntityHandle, MultiHashSet<EntityHandle>> MHM_EH_MHS_EH;
	__int64 field_158;
};

struct ActionRequest2
{
	__int64 field_0;
	__int64 field_8;
	bg3se::interrupt::InterruptVariant2 field_10;
	__int64 field_118;
};

struct ActionRequest3
{
	__int64 field_0;
	__int64 field_8;
	MultiHashMap<EntityHandle, MultiHashSet<EntityHandle>> MHM_EH_MHS_EH;
};

struct ActionRequest4
{
	struct RequestElement
	{
		Array<std::pair<EntityHandle, EntityHandle>> field_0;
		__int64 field_10;
		int field_18;
		__int64 field_20;
		__int64 field_28;
	};

	__int64 field_0;
	Array<RequestElement> field_8;
	[[bg3::hidden]]
	MultiHashMap<EntityHandle, MultiHashMap<bg3se::interrupt::InterruptVariant2, ConditionRoll>> field_18;
};

END_NS()

BEGIN_NS(esv::spell_cast)

struct CastState
{
	Guid field_0;
	FixedString field_10;
	FixedString field_14;
	FixedString field_18;
	uint8_t field_1C;
};

struct CastHitDelayComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerSpellCastHitDelay;
	static constexpr auto EngineClass = "esv::spell_cast::CastHitDelayComponent";

	struct CastHitDelay
	{
		int field_0;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		FixedString field_14;
		int field_18;
	};

	Array<CastHitDelay> CastHitDelays;
	float CastTargetHitDelay;
	float CastTargetHitDelay2;
};

struct CastResponsibleComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerSpellCastResponsible;
	static constexpr auto EngineClass = "esv::spell_cast::CastResponsibleComponent";

	EntityHandle Entity;
};

struct ClientInitiatedComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerSpellClientInitiated;
	static constexpr auto EngineClass = "esv::spell_cast::ClientInitiatedComponent";

	uint8_t Dummy;
};

struct ExternalsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerSpellExternals;
	static constexpr auto EngineClass = "esv::spell_cast::ExternalsComponent";

	Array<Guid> Externals;
};

struct HitRegisterComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerSpellHitRegister;
	static constexpr auto EngineClass = "esv::spell_cast::HitRegisterComponent";

	Array<Guid> Hits;
};

struct InterruptRequestsComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerSpellInterruptRequests;
	static constexpr auto EngineClass = "esv::spell_cast::InterruptRequestsComponent";

	Array<interrupt::ActionRequest1> Requests1;
	Array<interrupt::ActionRequest2> Requests2;
	Array<interrupt::ActionRequest4> Requests4;
	Array<interrupt::ActionRequest3> Requests3;
};

struct InterruptIdentifier
{
	uint64_t field_0;
	uint64_t field_8;
	uint64_t field_10;
};

struct InterruptRollData
{
	__int64 field_0;
	Array<ResolvedUnknown> field_8;
};

struct InterruptResult
{
	MultiHashMap<uint8_t, MultiHashMap<uint8_t, InterruptRollData>> field_0;
	MultiHashMap<uint8_t, MultiHashMap<uint8_t, uint64_t>> field_40;
	Array<uint16_t> field_50;
	MultiHashSet<uint8_t> field_60;
	MultiHashSet<uint8_t> field_90;
};

struct InterruptResult2
{
	__int64 field_0;
	__int64 field_8;
	__int64 field_10;
	int field_18;
	uint8_t field_1C;
	uint32_t _Pad;
	uint8_t field_20;
	uint8_t field_21;
	std::variant<StatsRollType0, StatsRollType1> Roll;
	int field_138;
	__int64 field_140;
	__int64 field_148;
	uint8_t field_150;
	uint8_t field_151;
	uint8_t field_152;
};

struct InterruptResultsComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerSpellInterruptResults;
	static constexpr auto EngineClass = "esv::spell_cast::InterruptResultsComponent";

	// We cannot serialize composite keys yet!
	[[bg3::hidden]]
	MultiHashMap<InterruptIdentifier, InterruptResult> Results;
	Array<InterruptResult2> Results2;
};

struct StateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerSpellCastState;
	static constexpr auto EngineClass = "esv::spell_cast::StateComponent";

	uint8_t field_0;
	int field_4;
	CastState State;
	int field_28;
};


END_NS()

BEGIN_NS(esv::interrupt)

struct AddRemoveRequestsComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerInterruptAddRemoveRequests;
	static constexpr auto EngineClass = "esv::interrupt::AddRemoveRequestsComponent";

	MultiHashMap<EntityHandle, uint8_t> Requests;
};

struct ActionRequestsComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerInterruptActionRequests;
	static constexpr auto EngineClass = "esv::interrupt::ActionRequestsComponent";

	Array<ActionRequest1> Requests1;
	Array<ActionRequest2> Requests2;
	Array<ActionRequest3> Requests3;
};

struct ZoneRequestsComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerInterruptZoneRequests;
	static constexpr auto EngineClass = "esv::interrupt::ZoneRequestsComponent";

	Array<ActionRequest1> Requests1;
	Array<ActionRequest2> Requests2;
};

struct InitialParticipantsComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerInterruptInitialParticipants;
	static constexpr auto EngineClass = "esv::interrupt::InitialParticipantsComponent";
	
	struct Participant
	{
		ActionRequest1 Request1;
		MultiHashSet<EntityHandle> Entities;
	};

	MultiHashMap<EntityHandle, Participant> Participants;
};

struct TurnOrderInZoneComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerInterruptTurnOrderInZone;
	static constexpr auto EngineClass = "esv::interrupt::TurnOrderInZoneComponent";
	
	MultiHashSet<EntityHandle> InZone;
};

END_NS()

BEGIN_SE()

template <>
inline uint64_t MultiHashMapHash<interrupt::InterruptVariant2>(interrupt::InterruptVariant2 const& v)
{
	return Hash(v.field_B0) ^ Hash(v.field_C0);
}

END_SE()
