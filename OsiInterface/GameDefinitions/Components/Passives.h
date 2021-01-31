#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>

namespace dse
{
	struct PassiveContainerComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::PassiveContainer;

		Array<void*> field_18;
		Array<void*> field_30;
	};

	struct PassiveComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Passive;

		int field_18;
		FixedString PassiveId;
		__int64 field_20;
		__int64 field_28;
		__int64 field_30;
		int field_38;
		int field_3C;
		int field_40;
		RefMap<void*, void*> field_48;
		uint8_t StatsFunctorContext;
		int field_5C;
	};
}
