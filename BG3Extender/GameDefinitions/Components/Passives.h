#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

struct PassiveContainerComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::PassiveContainer;
	static constexpr auto EngineClass = "eoc::PassiveContainerComponent";

	Array<EntityHandle> Passives;
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

struct ToggledPassivesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerToggledPassives;
	static constexpr auto EngineClass = "esv::passive::ToggledPassivesComponent";

	MultiHashMap<FixedString, bool> Passives;
};

END_SE()
