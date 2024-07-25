#pragma once

BEGIN_NS(character_creation)

struct AppearanceMaterialSetting
{
	Guid Material;
	Guid Color;
	float ColorIntensity;
	uint32_t MetallicTint;
	float GlossyTint;
};

struct Appearance
{
	[[bg3::hidden]]
	ScratchBuffer Buffer;
	Guid HairColor;
	Guid SkinColor;
	Guid EyeColor;
	Guid SecondEyeColor;
	[[bg3::legacy(field_98)]] Guid AccessorySet;
	Array<Guid> Visuals;
	Array<AppearanceMaterialSetting> Elements;
	Array<float> AdditionalChoices;
};


END_NS()
