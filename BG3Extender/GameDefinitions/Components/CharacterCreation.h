#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Components/Data.h>
#include <GameDefinitions/Components/Visual.h>

BEGIN_NS(character_creation)

struct DefinitionCommon
{
	__int64 field_0;
	__int64 field_8;
	__int64 field_10;
	__int64 field_18;
	__int64 field_20;
	__int64 field_28;
	__int64 field_30;
	__int64 field_38;
	Array<Array<uint8_t>> field_40;
	Array<Array<uint8_t>> field_50;
	Array<Array<uint8_t>> field_60;
	Array<Array<uint8_t>> field_70;
	Array<Array<uint8_t>> field_80;
	Array<Array<uint8_t>> field_90;
	Array<Array<uint8_t>> field_A0;
};


struct SomeDefinitionData
{
	Guid field_20;
	Guid field_30;
	Guid field_40;
	uint8_t field_50;
	uint8_t field_51;
	Guid field_58;
	Guid field_68;
	Guid field_78;
	Guid field_88;
	Guid field_98;
	uint8_t field_A8;
};


struct CharacterDefinition
{
	int field_0;
	STDString field_8;
	SomeDefinitionData field_20;
	GameObjectVisualData Visual;
	std::array<int, 7> Abilities;
	LevelUpData LevelUpData;
};


struct CompanionDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCCompanionDefinition;
	static constexpr auto EngineClass = "eoc::character_creation::CompanionDefinitionComponent";

	Guid field_0;
	Guid field_10;
	uint8_t field_20;
	uint8_t field_21;
	Guid field_28;
	GameObjectVisualData Visual;
	Guid field_110;
	Guid field_120;
	uint8_t field_130;
};


struct DefinitionCommonComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCDefinitionCommon;
	static constexpr auto EngineClass = "eoc::character_creation::DefinitionCommonComponent";

	int field_0;
	__int64 field_8;
	DefinitionCommon Definition;
};


struct LevelUpUnknown
{
	Guid field_0;
	FixedString field_10;
};


struct FullRespecDefinition
{
	STDString field_0;
	SomeDefinitionData Definition;
	GameObjectVisualData Visual;
	std::array<int, 7> Abilities;
	Array<LevelUpData> LevelUpData;
	Array<LevelUpUnknown> field_1B0;
};


struct FullRespecDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCFullRespecDefinition;
	static constexpr auto EngineClass = "eoc::character_creation::FullRespecDefinitionComponent";

	FullRespecDefinition Definition;
	Guid field_1C0;
	Guid field_1D0;
	int field_1E0;
	LevelUpUpgrades LevelUpUpgrades;
	Array<SpellIdBase> Spells;
	uint8_t field_268;
};


struct RespecDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCRespecDefinition;
	static constexpr auto EngineClass = "eoc::character_creation::RespecDefinitionComponent";

	Guid field_0;
	CharacterDefinition Definition;
	uint8_t field_298;
};


struct CharacterDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCCharacterDefinition;
	static constexpr auto EngineClass = "eoc::character_creation::CharacterDefinitionComponent";

	CharacterDefinition Definition;
	uint8_t field_288;
	int field_28C;
	uint8_t field_290;
};


struct SessionCommonComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCSessionCommon;
	static constexpr auto EngineClass = "eoc::character_creation::SessionCommonComponent";

	int NetId;
	int field_4;
	uint8_t field_8;
};


struct StateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCState;
	static constexpr auto EngineClass = "eoc::character_creation::StateComponent";

	uint8_t field_0;
	uint8_t field_1;
	uint8_t field_2;
};


struct LevelUpDefinitionExtra
{
	int field_0;
	Guid field_8;
	[[bg3::hidden]]
	ScratchBuffer Scratch;
	Array<GameObjectVisualData::AppearanceElement> AppearanceElements;
	LevelUpData LevelUpData;
	uint8_t field_160;
	int field_164;
	uint8_t field_168;
	int field_16C;
	EntityHandle field_170;
};


struct LevelUpDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCLevelUpDefinition;
	static constexpr auto EngineClass = "eoc::character_creation::LevelUpDefinitionComponent";

	FullRespecDefinition Definition;
	LevelUpDefinitionExtra LevelUp;
};

END_NS()

BEGIN_NS(character_creation::definition)

struct FullRespecComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCFullRespec;
	static constexpr auto EngineClass = "eoc::character_creation::definition::FullRespecComponent";

	EntityHandle field_0;
	EntityHandle field_8;
};


struct RespecComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCRespec;
	static constexpr auto EngineClass = "eoc::character_creation::definition::RespecComponent";

	EntityHandle field_0;
	EntityHandle field_8;
};


struct CreationComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCCreation;
	static constexpr auto EngineClass = "eoc::character_creation::definition::CreationComponent";

	Array<EntityHandle> field_0;
	uint8_t field_10;
};


struct LevelUpComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCLevelUp;
	static constexpr auto EngineClass = "eoc::character_creation::definition::LevelUpComponent";

	EntityHandle field_0;
	EntityHandle field_8;
	char field_10;
};


END_NS()
