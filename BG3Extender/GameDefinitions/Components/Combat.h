#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
{
	struct CombatParticipantComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CombatParticipant;
		static constexpr auto EngineClass = "eoc::combat::ParticipantComponent";

		EntityHandle CombatHandle;
		FixedString CombatGroupId;
		int field_C;
		CombatParticipantFlags Flags;
		Guid AiHint;
	};

	struct CombatStateComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CombatState;
		static constexpr auto EngineClass = "eoc::combat::StateComponent";

		Guid MyGuid;
		MultiHashMap<EntityHandle, int32_t> Initiatives;
		Array<EntityHandle> Participants;
		EntityHandle field_98;
		EntityHandle field_A0;
		FixedString Level;
		uint8_t field_AC;
		float field_B0;
		Array<EntityHandle> field_B8;
		uint8_t field_D0;
	};

	struct TurnBasedComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::TurnBased;
		static constexpr auto EngineClass = "eoc::TurnBasedComponent";

		EntityHandle Entity;
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
		static constexpr ExtComponentType ComponentType = ExtComponentType::TurnOrder;
		static constexpr auto EngineClass = "eoc::TurnOrderComponent";

		struct ParticipantHandleInfo
		{
			EntityHandle Entity;
			int32_t Initiative;
		};

		struct Participant
		{
			Array<ParticipantHandleInfo> Handles;
			Guid Participant;
			uint32_t field_28;
			int32_t Initiative;
			uint8_t field_30;
		};

		Array<uint64_t> TurnOrderIndices;
		Array<Participant> Participants;
		Array<uint64_t> TurnOrderIndices2;
		Array<Participant> Participants2;
		int field_78;
		int field_7C;
	};
}
