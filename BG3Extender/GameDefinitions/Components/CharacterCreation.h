#pragma once

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
	Guid Origin;
	Guid Race;
	Guid Subrace;
	uint8_t field_50;
	uint8_t field_51;
	Guid RootTemplate;
	Guid Background;
	Guid field_78;
	Guid Voice;
	Guid VOLines;
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
	STDString Name;
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

struct ChangeAppearanceDefinitionBase
{
	STDString Name;
	char field_18;
	char field_19;
	Guid RootTemplate;
	Guid Voice;
	char field_40;
	GameObjectVisualData Visual;
};

struct ChangeAppearanceDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCChangeAppearanceDefinition;
	static constexpr auto EngineClass = "eoc::character_creation::ChangeAppearanceDefinitionComponent";

	FullRespecDefinition Definition;
	ChangeAppearanceDefinitionBase Appearance;
	int32_t field_2E0;
	uint8_t field_2E4;
	EntityHandle field_2E8;
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

	bool HasDummy;
	bool Canceled;
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
};


struct LevelUpDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCLevelUpDefinition;
	static constexpr auto EngineClass = "eoc::character_creation::LevelUpDefinitionComponent";

	FullRespecDefinition Definition;
	LevelUpDefinitionExtra LevelUp;

	uint8_t field_160;
	int field_164;
	uint8_t field_168;
	int field_16C;
	EntityHandle field_170;
};

struct LevelUpComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCLevelUp;
	static constexpr auto EngineClass = "eoc::character_creation::LevelUpComponent";

	Array<LevelUpData> LevelUps;
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
	static constexpr ExtComponentType ComponentType = ExtComponentType::CCDefinitionLevelUp;
	static constexpr auto EngineClass = "eoc::character_creation::definition::LevelUpComponent";

	EntityHandle field_0;
	EntityHandle field_8;
	char field_10;
};


END_NS()


BEGIN_NS(ecl::character_creation)

struct DefinitionStateComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ClientCCDefinitionState;
	static constexpr auto EngineClass = "ecl::character_creation::DefinitionStateComponent";

	EntityHandle field_0;
	float field_8;
	uint32_t field_C;
	int field_10;
	bg3se::character_creation::DefinitionCommon Definition;
};

struct DefinitionStateExComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ClientCCDefinitionStateEx;
	static constexpr auto EngineClass = "ecl::character_creation::DefinitionStateExComponent";

	uint8_t field_0;
	uint8_t field_1;
	uint8_t field_2;
	[[bg3::hidden]] Array<void*> field_8;
	uint32_t field_18;
	uint32_t field_1C;
	uint8_t field_20;
};

struct BaseDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ClientCCBaseDefinitionState;
	static constexpr auto EngineClass = "ecl::character_creation::BaseDefinitionComponent";

	bg3se::character_creation::CharacterDefinition Definition;
};

struct CompanionDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ClientCCCompanionDefinition;
	static constexpr auto EngineClass = "ecl::character_creation::CompanionDefinitionComponent";

	Guid field_0;
	Guid field_10;
	char field_20;
	char field_21;
	Guid field_28;
	GameObjectVisualData Visual;
	Guid field_110;
	Guid field_120;
	char field_130;
};

struct LevelUpDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ClientCCLevelUpDefinition;
	static constexpr auto EngineClass = "ecl::character_creation::LevelUpDefinitionComponent";

	bg3se::character_creation::LevelUpDefinitionExtra Definition;
};

struct ChangeAppearanceDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ClientCCChangeAppearanceDefinition;
	static constexpr auto EngineClass = "ecl::character_creation::ChangeAppearanceDefinitionComponent";

	bg3se::character_creation::ChangeAppearanceDefinitionBase Definition;
};

struct FullRespecDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ClientCCFullRespecDefinition;
	static constexpr auto EngineClass = "ecl::character_creation::FullRespecDefinitionComponent";

	Guid field_0;
	Guid field_10;
	int field_20;
	LevelUpUpgrades LevelUpUpgrades;
	[[bg3::hidden]] Array<void*> SpellIds;
};

struct DummyDefinitionComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ClientCCDummyDefinition;
	static constexpr auto EngineClass = "ecl::character_creation::DummyDefinitionComponent";

	EntityHandle field_0;
	int field_8;
	int field_C;
	int field_10;
	MultiHashMap<uint64_t, FixedString> field_18_Map_unk_FS;
	uint8_t field_58;
	uint8_t field_59;
	uint8_t field_5A;
	uint8_t field_5B;
	Guid field_60;
	Guid field_70;
	uint8_t field_80;
	uint8_t field_81;
	Guid field_88;
	Guid field_98;
	[[bg3::hidden]] Array<void*> field_A8;
	[[bg3::hidden]] Array<void*> field_B8;
	GameObjectVisualData Visual;
	uint8_t field_1A0;
	EntityHandle field_1A8;
};


END_NS()


BEGIN_NS(esv::character_creation)

struct AppearanceVisualTagComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerCCAppearanceVisualTag;
	static constexpr auto EngineClass = "esv::character_creation::AppearanceVisualTagComponent";

	Array<Guid> Tags;
};

struct EquipmentSetRequestComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerCCEquipmentSetRequest;
	static constexpr auto EngineClass = "esv::character_creation::EquipmentSetRequestComponent";

	MultiHashMap<EntityHandle, Array<FixedString>> Requests;
};

struct GodComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerCCGod;
	static constexpr auto EngineClass = "esv::character_creation::GodComponent";

	Guid God;
};

struct UpdatesComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerCCUpdates;
	static constexpr auto EngineClass = "esv::character_creation::UpdatesComponent";

	MultiHashMap<EntityHandle, int32_t> Updates;
};

DEFINE_TAG_COMPONENT(esv::character_creation, IsCustomComponent, ServerCCIsCustom)

END_NS()
