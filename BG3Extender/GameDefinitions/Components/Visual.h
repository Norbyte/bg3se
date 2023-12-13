#pragma once

BEGIN_SE()

struct CustomIconComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CustomIcon;
	static constexpr auto EngineClass = "eoc::CustomIconComponent";

	[[bg3::hidden]]
	ScratchBuffer Buffer;
};

struct IconComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Icon;
	static constexpr auto EngineClass = "eoc::IconComponent";

	FixedString Icon;
};

struct ActiveCharacterLightComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ActiveCharacterLight;
	static constexpr auto EngineClass = "eoc::light::ActiveCharacterLightComponent";

	FixedString Light;
};

struct VisualComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Visual;
	static constexpr auto EngineClass = "ls::VisualComponent";

	__int64 field_18;
	uint8_t field_20;
	uint8_t field_21;
};

struct GameObjectVisualData
{
	struct AppearanceElement
	{
		Guid Material;
		Guid Color;
		float ColorIntensity;
		uint32_t MetallicTint;
		float GlossyTint;
	};

	[[bg3::hidden]]
	ScratchBuffer Buffer;
	Guid HairColor;
	Guid SkinColor;
	Guid EyeColor;
	Guid SecondEyeColor;
	Guid field_98;
	Array<Guid> Visuals;
	Array<AppearanceElement> Elements;
	Array<int32_t> AdditionalChoices;
};

struct GameObjectVisualComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::GameObjectVisual;
	static constexpr auto EngineClass = "eoc::GameObjectVisualComponent";

	FixedString RootTemplateId;
	uint8_t RootTemplateType;
	FixedString Icon;
	float Scale;
	uint8_t Type;
};

struct AppearanceOverrideComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::AppearanceOverride;
	static constexpr auto EngineClass = "eoc::object_visual::AppearanceOverrideComponent";

	GameObjectVisualData Visual;
};

struct DisplayNameComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::DisplayName;
	static constexpr auto EngineClass = "eoc::DisplayNameComponent";

	TranslatedString NameKey;
	TranslatedString UnknownKey;
	STDString Name;
};

struct GameplayLightComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::GameplayLight;
	static constexpr auto EngineClass = "eoc::GameplayLightComponent";

	int field_18;
	uint8_t field_1C;
	int field_20;
	int field_24;
	int field_28;
};

struct StaticPhysicsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::StaticPhysics;
	static constexpr auto EngineClass = "ls::StaticPhysicsComponent";

	[[bg3::hidden]]
	void* field_0;
};

struct PhysicsComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::Physics;
	static constexpr auto EngineClass = "ls::PhysicsComponent";

	[[bg3::hidden]]
	void* Physics;
	int field_8;
	int field_C;
	int field_10;
	uint8_t field_14;
	uint8_t field_15;
	uint8_t field_16;
};

struct CharacterCreationAppearanceComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::CharacterCreationAppearance;
	static constexpr auto EngineClass = "eoc::character_creation::AppearanceComponent";

	Array<Guid> Visuals;
	Array<GameObjectVisualData::AppearanceElement> Elements;
	Array<int32_t> AdditionalChoices;
	Guid SkinColor;
	Guid EyeColor;
	Guid SecondEyeColor;
	Guid HairColor;
};

END_SE()

BEGIN_NS(esv)

struct DisplayName
{
	TranslatedString NameKey;
	uint8_t field_10;
	STDString Name;
};

struct DisplayNameTranslatedString
{
	TranslatedString NameKey;
	uint8_t field_10;
};

struct DisplayNameListComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerDisplayNameList;
	static constexpr auto EngineClass = "esv::DisplayNameListComponent";

	Array<DisplayName> Names;
	Array<DisplayNameTranslatedString> TranslatedStrings;
};

struct IconListComponent : public BaseComponent
{
	static constexpr ExtComponentType ComponentType = ExtComponentType::ServerIconList;
	static constexpr auto EngineClass = "esv::IconListComponent";

	struct Icon
	{
		FixedString Icon;
		uint32_t field_4;
	};

	Array<Icon> Icons;
	uint8_t field_30;
};

END_NS()
