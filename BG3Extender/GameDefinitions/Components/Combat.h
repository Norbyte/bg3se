#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
{
	struct CombatParticipantComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CombatParticipant;

		EntityHandle CombatHandle;
		FixedString CombatGroupId;
		int field_24;
		__int64 field_28;
		int Flags;
		FixedString Archetype;
		Guid AiHint;
		int field_48;
		char field_4C;
		bool CombatGroupChangedFlag_M;
	};

	struct CombatStateComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CombatState;

		Guid MyGuid;
		VirtualMultiHashMap<EntityHandle, int32_t> Initiatives;
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
		
		uint8_t field_18;
		uint8_t field_19;
		uint8_t field_1A;
		int field_1C;
		uint8_t field_20;
		uint8_t field_21;
		int field_24;
		uint8_t field_28;
		int field_2C;
		uint8_t field_30;
		Guid field_38;
	};

	struct TurnOrderComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::TurnOrder;

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
