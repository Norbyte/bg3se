#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
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
		ComponentHandle field_20;
		ComponentHandle field_28;
		Array<uint64_t> array_unkn2;
		RefMap<void*, void*> field_48;
		uint8_t StatsFunctorContext;
		int field_5C;
	};
}
