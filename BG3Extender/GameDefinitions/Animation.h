#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_NS(gn)

struct [[bg3::hidden]] GenomeVariant;

struct [[bg3::hidden]] GenomeVarTypeDesc
{
	using AssignProc = void (GenomeVariant* variant, GenomeVariant* other);
	using AssignFromRawValueProc = void (GenomeVariant* this_, void* data);
	using VisitProc = bool (ObjectVisitor* visitor, FixedString const& node, GenomeVariant* variant);
	using FromStringProc = void (GenomeVariant* variant, char const* str);
	using ToStringProc = void (GenomeVariant* variant, STDString* out);
	using DestroyProc = void (GenomeVariant* variant);
	using ComparatorProc = void (void* this_, void* other);

	AssignProc* Assign;
	AssignFromRawValueProc* AssignFromRawValue;
	VisitProc* Visit;
	FromStringProc* FromString;
	ToStringProc* ToString;
	DestroyProc* Destroy;
	ComparatorProc* Comparator;

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
#if 0
	// Only present in editor build
	// bool Assigned;
	// bool Constant;
#endif

	FixedString GetTypeName() const;
	UserReturn LuaGetValue(lua_State* L) const;
};

struct [[bg3::hidden]] GenomeParametrizedEventArgs
{
	[[bg3::hidden]] void* VMT;
	std::array<GenomeVariant, 22> Args;
};

END_NS()

BEGIN_SE()

struct TextKeyTypeProperties : public ProtectedGameObject<TextKeyTypeProperties>
{
	virtual ~TextKeyTypeProperties() = 0;
	virtual bool Visit(ObjectVisitor* visitor) = 0;
	virtual TextKeyType GetType() = 0;
	virtual void Init(TextKeyTypeProperties* props) = 0;
	virtual void Prepare(void* prepareData) = 0;
	virtual void Unprepare() = 0;
};

struct TextKeySoundTypeProperties : public TextKeyTypeProperties
{
	uint8_t SoundObjectIndex;
	FixedString Resource;
};

struct TextKeyEffectTypeProperties : public TextKeyTypeProperties
{
	FixedString Effect;
	FixedString Bone;
	FixedString field_10;
	bool HardAttach;
	bool InterruptLoopAtEnd;
	int16_t BoneId;
};

struct TextKeyFootStepTypeProperties : public TextKeyTypeProperties
{
	uint8_t FootID;
	bool Slide;
	bool PlaySound;
	bool PlayScuff;
	bool PlayStepFX;
	bool PlaySlideFX;
	bool PlayHearingFX;
};

struct TextKeyAttachTypeProperties : public TextKeyTypeProperties
{
	FixedString Bone;
	FixedString AttachBone;
	bool Detach;
};

struct TextKeyWeaponEffectTypeProperties : public TextKeyTypeProperties
{
	uint32_t Weapon;
	uint32_t EffectType;
};

struct TextKeyGenomeTypeProperties : public TextKeyTypeProperties
{
	// Empty
};

struct TextKeyAttackTypeProperties : public TextKeyTypeProperties
{
	// Empty
};

struct TextKeyRagdollTypeProperties : public TextKeyTypeProperties
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

struct TextKeyVisualCullFlagTypeProperties : public TextKeyTypeProperties
{
	uint32_t VisualFlag;
	bool Enabled;
};

struct TextKeyFloatDataTypeProperties : public TextKeyTypeProperties
{
	float Data;
};

struct TextKeyFoleyTypeProperties : public TextKeySoundTypeProperties
{
	uint8_t FoleyType;
	uint8_t FoleyIntensity;
};

struct TextKeyVocalTypeProperties : public TextKeyTypeProperties
{
	uint8_t VocalType;
};

struct TextKeyFootMoveTypeProperties : public TextKeyTypeProperties
{
	uint8_t Data;
};

struct TextKeyReactTypeProperties : public TextKeyTypeProperties
{
	// Empty
};

struct TextKeyEvent
{
	FixedString ID;
	float Time;
	float Length;
	uint8_t Track;
	TextKeyTypeProperties* Properties;
};


END_SE()

BEGIN_NS(lua)

LUA_POLYMORPHIC(TextKeyTypeProperties);

END_NS()
