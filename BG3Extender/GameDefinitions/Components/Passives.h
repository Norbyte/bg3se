#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

namespace bg3se
{
	struct PassiveContainerComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::PassiveContainer;
		static constexpr auto EngineClass = "eoc::PassiveContainerComponent";

		Array<void*> field_18;
		Array<void*> field_30;
	};

	struct PassiveComponent : public BaseComponent
	{
		static constexpr ExtComponentType ComponentType = ExtComponentType::Passive;
		static constexpr auto EngineClass = "eoc::PassiveComponent";

		uint8_t field_0;
		FixedString PassiveId;
		EntityHandle field_8;
		EntityHandle field_10;
		uint8_t field_18;
		uint8_t field_19;
		uint32_t field_1C;
	};
}
