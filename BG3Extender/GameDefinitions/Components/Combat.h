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
		int field_10;
		Guid field_18;
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

		EntityHandle field_0;
		int field_8;
		uint8_t field_C;
		int field_10;
		uint8_t field_14;
		uint8_t field_15;
		uint8_t field_16;
		uint64_t field_18;
		uint64_t field_20;
		uint64_t field_28;
		Guid field_30;
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
