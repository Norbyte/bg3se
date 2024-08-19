#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(gn)

struct [[bg3::hidden]] GenomeVariant;

struct [[bg3::hidden]] GenomeVarTypeDesc
{
	void (*Assign)(GenomeVariant* this_, GenomeVariant* other_);
	void (*AssignFromRawValue)(GenomeVariant* this_, void* data);
	bool (*ParseXMLNode)(void* XMLReader, FixedString NodeID, GenomeVariant* this_);

	void (*FromString)(void* data, char* str);
	void (*ToString)(void* data, STDString* out);

	void (*Destroy)(void* data);
	void (*operator_less)(void* this_, void* other);

	uint32_t TypeHash;
	uint32_t TypeHash2;
	uint64_t Size;
	uint64_t Alignment;
	FixedString TypeName;
	bool Trivial;
	GenomeVariant* DefaultValue;
};

struct [[bg3::hidden]] GenomeVariant
{
	void* Value;
	GenomeVarTypeDesc* Type;
	bool Assigned;
	bool Constant;
};

struct [[bg3::hidden]] GenomeParametrizedEventArgs
{
	[[bg3::hidden]] void* VMT;
	std::array<GenomeVariant, 22> Args;
};

END_NS()

BEGIN_SE()

struct TextKeyProperties : public ProtectedGameObject<TextKeyProperties>
{
	virtual ~TextKeyProperties() = 0;
	virtual bool ParseXML(void* XMLReader) = 0;
	virtual TextKeyPropertiesType GetPropertyType() = 0;
};

struct TextKeySoundTypeProperties : public TextKeyProperties
{
	uint8_t SoundObjectIndex;
	FixedString Resource;
};

struct TextKeyEffectTypeProperties : public TextKeyProperties
{
	FixedString Effect;
	FixedString Bone;
	[[bg3::hidden]] __int32 field_10;
	bool HardAttach;
	bool InterruptLoopAtEnd;
};

struct TextKeyFootStepTypeProperties : public TextKeyProperties
{
	uint8_t FootID;
	bool Slide;
	bool PlaySound;
	bool PlayScuff;
	bool PlayStepFX;
	bool PlaySlideFX;
	bool PlayHearingFX;
};

struct TextKeyAttachTypeProperties : public TextKeyProperties
{
	FixedString Bone;
	FixedString AttachBone;
	bool Detach;
};

struct TextKeyWeaponEffectTypeProperties : public TextKeyProperties
{
	int Weapon;
	int EffectType;
};

struct TextKeyGenomeTypeProperties : public TextKeyProperties
{
	// Empty
};

struct TextKeyAttackTypeProperties : public TextKeyProperties
{
	// Empty
};

struct TextKeyRagdollTypeProperties : public TextKeyProperties
{
	float AngularVelocityModifier;
	float LinearVelocityModifier;
	float StiffnessMultiplier;
	float DampingMultiplier;
	float DurationHardLimit;
	float DurationSoftLimit;
	float HitImpulseThreshold;
	bool HoldLastFrame;
	FixedString AnimationResourceID;
	bool LockLimits;
};

struct TextKeyVisualCullFlagTypeProperties : public TextKeyProperties
{
	uint32_t VisualFlag;
	bool Enabled;
};

struct TextKeyFloatDataTypeProperties : public TextKeyProperties
{
	float Data;
};

struct TextKeyFoleyTypeProperties : public TextKeyProperties
{
	uint8_t FoleyType;
	uint8_t FoleyIntensity;
};

struct TextKeyVocalTypeProperties : public TextKeyProperties
{
	uint8_t VocalType;
};

struct TextKeyFootMoveTypeProperties : public TextKeyProperties
{
	uint8_t Data;
};

struct TextKeyReactTypeProperties : public TextKeyProperties
{
	// Empty
};

struct TextKeyEvent
{
	FixedString TextKey;
	int field_4;
	int field_8;
	uint8_t field_C;
	TextKeyProperties* Properties;
};


END_SE()

BEGIN_NS(lua)

LUA_POLYMORPHIC(TextKeyProperties);

END_NS()
