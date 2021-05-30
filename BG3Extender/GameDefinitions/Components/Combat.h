#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
{
	struct CombatParticipantComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CombatParticipant;

		EntityWorldHandle CombatHandle;
		FixedString CombatGroupId;
		int field_24;
		__int64 field_28;
		int Flags;
		FixedString Archetype;
		UUID AiHint;
		int field_48;
		char field_4C;
		bool CombatGroupChangedFlag_M;
	};

	struct CombatStateComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::CombatState;

		UUID MyGuid;
		__int64 field_28;
		__int64 field_30;
		int field_38;
		__int64 field_40;
		int field_48;
		__declspec(align(8)) int field_50;
		__int64 field_58;
		int field_60;
		__declspec(align(8)) int field_68;
		__int64 field_70;
		int field_78;
		Array<ObjectHandle> CharacterHandles;
		__int64 field_98;
		__int64 field_A0;
		FixedString field_A8;
		char field_AC;
		int field_B0;
		__int64 field_B8;
		int field_C0;
		__declspec(align(8)) int field_C8;
		__declspec(align(8)) char field_D0;
	};

	struct TurnBasedComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::TurnBased;

		EntityWorldHandle CombatHandle_M;
		char field_20;
		char field_21;
		char field_22;
		char field_23;
		char field_24;
		int field_28;
		char field_2C;
		int field_30;
		char field_34;
		__declspec(align(4)) char field_38;
		char field_39;
		__declspec(align(8)) UUID field_40;
	};

	struct TurnOrderComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::TurnOrder;

		struct Participant
		{
			Array<ObjectHandle> Handles;
			__int64 field_18;
			__int64 field_20;
			__int64 field_28;
			__int64 field_30;
		};

		Array<int> TurnOrderIndices;
		Array<Participant> Participants;
		Array<void*> field_48;
		Array<void*> field_60;
		int field_78;
		int field_7C;
	};
}
