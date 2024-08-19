#pragma once

BEGIN_SE()

struct Timeline;

END_SE()

BEGIN_NS(aspk)

struct Property : public ProtectedGameObject<Property>
{
	struct Parameter
	{
		STDString Name;
		float Value;
		[[bg3::hidden]] __int32 field_1c;
	};

	virtual ~Property() = 0;
	virtual void Clone(Property const& other) = 0;
	virtual bool Visit(ObjectVisitor* visitor) = 0;
	virtual PropertyType GetPropertyType() = 0;
	virtual void SetInputValueString(FixedString const&) = 0;
	virtual void SetInputValueVec4(glm::vec4 const&) = 0;
	virtual void SetInputValueVec3(glm::vec3 const&) = 0;
	virtual void SetInputValueVec2(glm::vec2 const&) = 0;
	virtual void SetInputValueFloat(float) = 0;

	FixedString FullName;
	FixedString AttributeName;
	LegacyArray<Parameter> Parameters;
	FixedString Input;
};

struct BooleanProperty : public Property
{
	bool Value;
};

struct IntegerProperty : public Property
{
	int32_t Value;
};

struct IntegerRangeProperty : public Property
{
	int32_t Min;
	int32_t Max;
};

struct ColorARGBKeyFrameData
{
	float Time;
	[[bg3::hidden]] __int32 field_4;
	[[bg3::hidden]] __int64 field_8;
	glm::fvec4 Color;
};

struct ColorARGBKeyFrameProperty : public Property
{
	LegacyArray<ColorARGBKeyFrameData>* Frames;
};

struct FloatProperty : public Property
{
	float Value;
};

struct FloatRangeProperty : public Property
{
	float Min;
	float Max;
};

struct FloatKeyFrame : public ProtectedGameObject<FloatKeyFrame>
{
	virtual ~FloatKeyFrame() = 0;
	virtual void Clone(FloatKeyFrame const&) = 0;
	virtual bool Visit(ObjectVisitor*) = 0;
};

struct LinearFloatKeyFrame : public FloatKeyFrame
{
	float Time;
	[[bg3::hidden]] __int32 field_c; // Padding
	float Value;
};


struct CubicFloatKeyFrame : public FloatKeyFrame
{
	float Time;
	[[bg3::hidden]] __int32 field_c; // Padding
	float A;
	float B;
	float C;
	float D;
};


struct KeyFrameData : public ProtectedGameObject<KeyFrameData>
{
	virtual ~KeyFrameData() = 0;
	virtual int GetType() = 0;
};

struct LinearFloatKeyFrameData : public KeyFrameData
{
	LegacyArray<LinearFloatKeyFrame> Frames;
};

struct CubicFloatKeyFrameData : public KeyFrameData
{
	LegacyArray<CubicFloatKeyFrame> Frames;
};

struct FloatKeyFrameProperty : public Property
{
	LegacyArray<KeyFrameData*> KeyFrames;
};

struct StringProperty : public Property
{
	STDString Value;
};

struct Vector3Property : public Property
{
	Vector3 Value;
	[[bg3::hidden]] __int32 field_44;
};

struct FixedFunctionProperty : public Property
{
	// Types: 0, 1, 2, 3 is null
	struct VirtualFunction : public ProtectedGameObject<VirtualFunction>
	{
		virtual ~VirtualFunction() = 0;
		virtual void unknown1() = 0;
		virtual void unknown2() = 0;
		virtual void unknown3() = 0;
		virtual int GetFunctionType() = 0;
	};

	struct Function0 : public VirtualFunction
	{
		float A;
		float B;
	};

	struct Function1 : public VirtualFunction
	{
		float A;
		float B;
		float C;
	};

	struct Function2 : public VirtualFunction
	{
		float A;
		float B;
		float C;
		float D;
	};

	VirtualFunction* Function;
};

struct FixedStringProperty : public Property
{
	FixedString Value;
	[[bg3::hidden]] __int32 field_44;
};

struct BaseProperty : public Property
{
};

struct Component : public ProtectedGameObject<Component>
{
	virtual ~Component() = 0;
	virtual void Unknown1() = 0;
	virtual bool parseXML(void* XMLReader) = 0;
	virtual bool Unknown2() = 0;
	virtual const FixedString& GetType() = 0;

	Guid GU;
	[[bg3::hidden]] Timeline* Timeline;
	uint32_t TrackGroupIndex;
	float StartTime;
	float EndTime;
	bool IsActive;
	bool StayAlive;
};

// Note: There are a LOT of different EffectComponents, all seemingly over 0x100 in size
// The polymorphic choice should be made based on vtable[4]'s FixedString return value
struct BaseEffectComponent : public Component
{
	struct Module
	{
		STDString Name;
		Array<FixedString> FullName;
		Array<Property*> Properties;
	};
	__int64 field_48;
	__int64 field_50;
	__int64 field_58;
	__int64 field_60;
	__int64 field_68;
	LegacyMap<FixedString, Property*> PropertiesByFullName;
	Array<Module*> Modules;
	Array<Property*> Properties;
	__int64 field_a8;
	__int64 field_b0;
};

struct TLActor
{
	[[bg3::hidden]] void* vtable;
	bg3se::Guid UUID;
	bool PeanutOverride;
};

struct TLBaseComponent : public Component
{
	TLActor Actor;
	uint8_t field_68x;
	uint32_t TypeId;
	int64_t PhaseIndex;
	bool IsSnappedToEnd;
	bool IsMimicry;
};

BEGIN_BARE_NS(keys)
	struct KeyBase : public ProtectedGameObject<KeyBase>
	{
		[[bg3::hidden]] void* vtable;
		float Time;
		uint8_t InterpolationType;
		[[bg3::hidden]] __int8 field_d;
		[[bg3::hidden]] __int16 field_e;
	};
END_BARE_NS()

template<typename Key>
struct TLKeyBaseComponentComponent : public TLBaseComponent
{
	float field_68;
	[[bg3::hidden]] Array<Key*> Keys;
	bool HasTargetKey;
	Key* TargetKey;
	bool NeedsKeyUpdate;
	bool field_91;
	std::array<uint32_t, 2> Random;
};

template struct TLKeyBaseComponentComponent<keys::KeyBase>;

template<typename Key>
struct TLInterpolationKeyComponentComponent : public TLKeyBaseComponentComponent<Key>
{
};

template struct TLInterpolationKeyComponentComponent<keys::KeyBase>;

template<typename Key>
struct TLEventKeyComponentComponent : public TLKeyBaseComponentComponent<Key>
{
};

template struct TLEventKeyComponentComponent<keys::KeyBase>;

BEGIN_BARE_NS(keys)
	template <typename T>
	struct ValueKey : public KeyBase
	{
		T Value;
	};

	struct AtmosphereAndLightingChannelKey : public KeyBase
	{
		bg3se::Guid id;
		float fadeTime;
		[[bg3::hidden]] __int32 field_24;
	};

	struct AttachToEventKey : public KeyBase
	{
		bg3se::Guid Target;
		FixedString Bone;
		bool IsPersistent;
		[[bg3::hidden]] __int8 field_25;
		[[bg3::hidden]] __int16 field_26;
		glm::fvec3 offset;
		[[bg3::hidden]] __int32 field_34;
	};

	struct AttitudeEventKey : public KeyBase
	{
		FixedString Pose;
		FixedString Transition;
	};

	struct CameraDoFChannelKey : public KeyBase
	{
		float Value;
		[[bg3::hidden]] __int32 field_14;
	};

	struct CameraFoVKey : public KeyBase
	{
		float FoV;
		[[bg3::hidden]] __int32 field_14;
	};

	struct CameraLookAtKey : public KeyBase
	{
		bg3se::Guid Target;
		FixedString Bone;
		glm::fvec2 Framing;
		glm::fvec2 FramingOffset;
		glm::fvec2 FreeBounds;
		glm::fvec2 SoftBounds;
		float DampingStrength;
		float FreeZoneDelay;
		float SoftZoneDelay;
		float SoftZoneRampTime;
		[[bg3::hidden]] __int32 field_54;
	};

	struct EffectPhaseEventKey : public KeyBase
	{
		int32_t EffectPhase;
		[[bg3::hidden]] __int32 field_14;
	};

	struct EmotionEventKey : public KeyBase
	{
		int32_t Emotion;
		int32_t Variation;
		bool IsSustainedEmotion;
		bool AppliesMaterials;
		[[bg3::hidden]] __int16 field_1a;
		[[bg3::hidden]] __int32 field_1c;
	};

	struct FloatRTPCKey : public KeyBase
	{
		float FloatRTPCValue;
		[[bg3::hidden]] __int32 field_14;
	};

	struct GenomeTextEventKey : public KeyBase
	{
		FixedString EventName;
		[[bg3::hidden]] __int32 field_14;
	};

	struct HandsIKKey : public KeyBase
	{
		bool InverseKinematics;
		[[bg3::hidden]] __int8 field_11;
		[[bg3::hidden]] __int16 field_12;
		[[bg3::hidden]] __int32 field_14;
	};

	struct LookAtEventKey : public KeyBase
	{
		bg3se::Guid Target;
		FixedString Bone;
		uint8_t TurnMode;
		uint8_t TrackingMode;
		[[bg3::hidden]] __int16 field_26;
		float TurnSpeedMultiplier;
		float TorsoTurnSpeedMultiplier;
		float HeadTurnSpeedMultiplier;
		[[bg3::hidden]] __int32 field_34;
		std::optional<double> Weight;
		std::optional<double> SaveZoneAngle;
		std::optional<double> HeadSafeZoneAngle;
		glm::fvec3 Offset;
		bool Reset;
		uint8_t LookAtMode;
		uint8_t LookAtInterpMode;
		bool IsEyeLookAtEnabled;
		bg3se::Guid EyeLookAtTargetId;
		FixedString EyeLookAtBone;
		glm::fvec3 EyeLookAtOffset;
	};

	struct MaterialTextureKeyKey : public KeyBase
	{
		bg3se::Guid Value;
	};

	struct PhysicsKey : public KeyBase
	{
		bool InverseKinematics;
		[[bg3::hidden]] __int8 field_11;
		[[bg3::hidden]] __int16 field_12;
		[[bg3::hidden]] __int32 field_14;
	};

	struct PlayEffectEventKey : public KeyBase
	{
		bool PlayEffect;
		[[bg3::hidden]] __int8 field_11;
		[[bg3::hidden]] __int16 field_12;
		[[bg3::hidden]] __int32 field_14;
	};

	struct PlayRateKey : public KeyBase
	{
		float Speed;
		[[bg3::hidden]] __int32 field_14;
	};

	struct ShapeShiftKey : public KeyBase
	{
		bg3se::Guid TemplateId;
	};

	struct ShowArmorChannelKey : public KeyBase
	{
		bool Value;
		[[bg3::hidden]] __int8 field_11;
		[[bg3::hidden]] __int16 field_12;
		[[bg3::hidden]] __int32 field_14;
	};

	struct ShowHUDKey : public KeyBase
	{
		bool ShowHUD;
		[[bg3::hidden]] __int8 field_11;
		[[bg3::hidden]] __int16 field_12;
		[[bg3::hidden]] __int32 field_14;
	};

	struct ShowPeanutsKey : public KeyBase
	{
		bool ShowPeanuts;
		[[bg3::hidden]] __int8 field_11;
		[[bg3::hidden]] __int16 field_12;
		[[bg3::hidden]] __int32 field_14;
	};

	struct ShowVisualKey : public KeyBase
	{
		bool ShowVisual;
		[[bg3::hidden]] __int8 field_11;
		[[bg3::hidden]] __int16 field_12;
		[[bg3::hidden]] __int32 field_14;
	};

	struct ShowWeaponKey : public KeyBase
	{
		bool ShowWeapon;
		[[bg3::hidden]] __int8 field_11;
		[[bg3::hidden]] __int16 field_12;
		[[bg3::hidden]] __int32 field_14;
	};

	struct SoundEventKey : public KeyBase
	{
		struct Type1
		{
			bg3se::Guid SoundEventID;
			uint8_t SoundObjectIndex;
			bool KeepAlive;
			__int16 field_2a;
			float LoopLifetime;
		};
		struct Type2
		{
			bg3se::Guid SoundEventID;
		};
		struct Type4
		{
			uint8_t FoleyType;
			uint8_t FoleyIntensity;
			uint8_t SoundObjectIndex;
		};
		struct Type5
		{
			uint8_t VocalType;
			uint8_t SoundObjectIndex;
		};
		std::variant<Type1, Type2, Type2, Type2, Type4, Type5> data;
	};

	struct SplatterChannelKey : public KeyBase
	{
		float Value;
		uint8_t SplatterChangeMode;
		[[bg3::hidden]] __int8 field_15;
		[[bg3::hidden]] __int16 field_16;
	};

	struct SpringsKey : public KeyBase
	{
		bool EnableSprings;
		[[bg3::hidden]] __int8 field_11;
		[[bg3::hidden]] __int16 field_12;
		[[bg3::hidden]] __int32 field_14;
	};

	struct SteppingFadeKey : public KeyBase
	{
		bool SteppingInOut;
		[[bg3::hidden]] __int8 field_11;
		[[bg3::hidden]] __int16 field_12;
		[[bg3::hidden]] __int32 field_14;
	};

	struct SwitchLocationEventKey : public KeyBase
	{
		bg3se::Guid SwitchLocationEventID;
		bg3se::Guid s_LevelTemplateID;
	};

	struct SwitchStageEventKey : public KeyBase
	{
		bg3se::Guid SwitchStageEventID;
		bool ForceTransformUpdate;
		bool ForceUpdateCameraBehavior;
		[[bg3::hidden]] __int16 field_22;
		[[bg3::hidden]] __int32 field_24;
	};

	struct TransformChannelFloatKey : public KeyBase
	{
		float Value;
		[[bg3::hidden]] __int32 field_14;
	};

	struct TransformChannelFrameOfReferenceKey : public KeyBase
	{
		struct FrameOfReference
		{
			bg3se::Guid targetId;
			FixedString targetBone;
			bool OneFrameOnly;
			bool KeepScale;
			[[bg3::hidden]] __int16 field_16;
		};
		FrameOfReference frameOfReference;
	};

	struct TransformChannelQuatKey : public KeyBase
	{
		glm::fvec4 Value;
	};
END_BARE_NS()

BEGIN_BARE_NS(channels)
	template <typename T>
	struct EventKeyChannel : public TLEventKeyComponentComponent<keys::ValueKey<T>>
	{
		T defaultVal;
	};
			
	struct TimelineActorPropertiesReflectionKeyComponentComponent : public TLInterpolationKeyComponentComponent<keys::ValueKey<float>>
	{
		float KeyDefault;
		[[bg3::hidden]] __int32 field_b4;
	};

	struct TLAtmosphereAndLightingChannelComponent : public TLInterpolationKeyComponentComponent<keys::AtmosphereAndLightingChannelKey>
	{
		__int64 field_b0;
		__int64 field_b8;
		__int64 field_c0;
	};

	struct TLCameraDoFChannelComponent : public TLInterpolationKeyComponentComponent<keys::CameraDoFChannelKey>
	{
		float field_b0;
		uint8_t channelNum;
		__int8 field_b5;
		__int16 field_b6;
	};

	struct TLCameraExposureChannelComponent : public TLCameraDoFChannelComponent
	{
	};

	struct TLMaterialKeyComponentComponent : public TLInterpolationKeyComponentComponent<keys::ValueKey<float>>
	{
		float defaultVal;
		__int32 field_b4;
	};

	struct TLMaterialTextureKeyComponentComponent : public TLInterpolationKeyComponentComponent<keys::MaterialTextureKeyKey>
	{
		__int64 field_b0;
		__int64 field_b8;
	};

	struct TLShowArmorChannelComponent : public TLEventKeyComponentComponent<keys::ShowArmorChannelKey>
	{
		__int64 field_b0;
	};

	struct TLSplatterChannelComponent : public TLInterpolationKeyComponentComponent<keys::SplatterChannelKey>
	{
		__int64 field_b0;
		uint8_t SplatterType;
		[[bg3::hidden]] __int8 field_b9;
		[[bg3::hidden]] __int16 field_ba;
		[[bg3::hidden]] __int32 field_bc;
	};

	struct TLTransformChannelFloatComponent : public TLInterpolationKeyComponentComponent<keys::TransformChannelFloatKey>
	{
		float defaultVal;
		__int32 field_b4;
		Array<__int64> field_b8;
	};

	struct TLTransformChannelFrameOfReferenceComponent : public TLInterpolationKeyComponentComponent<keys::TransformChannelFrameOfReferenceKey>
	{
		keys::TransformChannelFrameOfReferenceKey::FrameOfReference defaultVal;
		Array<__int64> field_b8;
	};

	struct TLTransformChannelQuatComponent : public TLInterpolationKeyComponentComponent<keys::TransformChannelQuatKey>
	{
		glm::fvec4 defaultVal;
		Array<__int64> field_c0;
	};
END_BARE_NS()

struct TimelineActorPropertiesReflectionComponent : public TLBaseComponent
{
	struct Parameter : ProtectedGameObject<Parameter>
	{
		struct Range
		{
			channels::TimelineActorPropertiesReflectionKeyComponentComponent** begin;
			channels::TimelineActorPropertiesReflectionKeyComponentComponent** end;
		};
		virtual ~Parameter() = 0;
		virtual bool readData(void* dataReader) = 0;
		virtual Range& getRange(Range& r) = 0;

		[[bg3::hidden]] void* vtable;
		FixedString ParameterName;
		__int32 field_c;
		__int64 field_10;
		__int64 field_18;
		__int64 field_20;
		__int64 field_28;
		__int64 field_30;
		__int64 field_38;
		__int64 field_40;
		__int64 field_48;
        __int64 field_50;

		[[bg3::getter]] inline std::span<channels::TimelineActorPropertiesReflectionKeyComponentComponent*> PropertyParameters()
		{
			[[bg3::hidden]] Range range;
            getRange(range);
            return std::span(range.begin, range.end - range.begin);
		}
	};
			
	__int64 field_80;
	Array<Parameter*> PropertyParameters;
};

struct TLAnimationComponent : public TLBaseComponent
{
	__int64 field_80;
	bg3se::Guid AnimationSourceId;
	__int64 field_98;
	__int64 field_a0;
	__int64 field_a8;
	// channels::EventKeyChannel<bool> field_b0;
	[[bg3::hidden]] std::array<__int8, 0xb8> raw_field_b0;
	[[bg3::getter]] channels::EventKeyChannel<bool>* field_b0()
	{
		return reinterpret_cast<channels::EventKeyChannel<bool>*>(&raw_field_b0);
	}
	FixedString AnimationSlot;
	bool Continuous;
	bool HoldAnimation;
	bool EnableRootMotion;
	bool IsMirrored;
	double FadeIn;
	double FadeOut;
	double FadeInOffset;
	double FadeOutOffset;
	__int64 field_190;
	double AnimationPlayRate;
	double AnimationPlayStartOffset;
	uint8_t OffsetType;
	__int8 field_1a9;
	__int16 field_1aa;
	__int32 field_1ac;
	bg3se::Guid AnimationGroup;
	struct TargetTransforms
	{
		glm::fvec4 RotationQuat;
		glm::fvec3 Position;
		float field_1c;
		float field_20;
		float Scale;
	};
	TargetTransforms TargetTransform;
	bg3se::Guid BoneGroupId;
	__int64 field_1f8;
	// channels::EventKeyChannel<bool> HideVfxChannel;
	[[bg3::hidden]] std::array<__int8, 0xb8> raw_HideVfxChannel;
	[[bg3::getter]] channels::EventKeyChannel<bool>* HideVfxChannel()
	{
		return reinterpret_cast<channels::EventKeyChannel<bool>*>(&raw_HideVfxChannel);
	}
	__int64 field_2b8;
	__int64 field_2c0;
};

struct TLAdditiveAnimationComponent : public TLAnimationComponent
{
};

struct TLAtmosphereAndLightingComponent : public TLBaseComponent
{
	std::array<channels::TLAtmosphereAndLightingChannelComponent*, 2> Channels;
};

struct TLAttachToEventComponent : public TLInterpolationKeyComponentComponent<keys::AttachToEventKey>
{
};

struct TLAttitudeEventComponent : public TLKeyBaseComponentComponent<keys::AttitudeEventKey>
{
	FixedString field_b0;
	__int32 field_b4;
};

struct TLCameraDoFComponent : public TLBaseComponent
{
	Array<channels::TLCameraDoFChannelComponent*> Channels;
};

struct TLCameraExposureComponent : public TLBaseComponent
{
	Array<channels::TLCameraExposureChannelComponent*> Channels;
	__int64 field_90;
};

struct TLCameraFoVComponent : public TLInterpolationKeyComponentComponent<keys::CameraFoVKey>
{
};

struct TLCameraLookAtComponent : public TLInterpolationKeyComponentComponent<keys::CameraLookAtKey>
{
};

struct TLEffectPhaseEventComponent : public TLEventKeyComponentComponent<keys::EffectPhaseEventKey>
{
};

struct TLEmotionEventComponent : public TLEventKeyComponentComponent<keys::EmotionEventKey>
{
};

struct TLFloatRTPCComponent : public TLInterpolationKeyComponentComponent<keys::FloatRTPCKey>
{
	__int64 field_b0;
	FixedString RTPCName;
	uint8_t SoundType;
	uint8_t SoundObjectIndex;
	[[bg3::hidden]] __int16 field_be;
};

struct TLGenomeTextEventComponent : public TLEventKeyComponentComponent<keys::GenomeTextEventKey>
{
};

struct TLHandsIKComponent : public TLEventKeyComponentComponent<keys::HandsIKKey>
{
};

struct TLLayeredAnimationComponent : public TLAnimationComponent
{
};

struct TLLookAtEventComponent : public TLEventKeyComponentComponent<keys::LookAtEventKey>
{
};

struct TLMaterialComponent : public TLBaseComponent
{
	struct Parameter
	{
		struct Range
		{
			Component** begin;
			Component** end;
		};
		virtual ~Parameter() = 0;
		virtual bool parseXML(void*) = 0;
		virtual Range& getRange(Range&) = 0;
		FixedString MaterialParameterName;
		__int32 field_c;
		__int64 field_10;
	};

	struct TextureParameter : public Parameter
	{
		bool TextureParam;
		bool IsVirtual;
		__int16 field_1a;
		__int32 field_1c;

		[[bg3::getter]] inline std::span<channels::TLMaterialTextureKeyComponentComponent*> PropertyParameters()
		{
			[[bg3::hidden]] Range range;
            getRange(range);
            return std::span((channels::TLMaterialTextureKeyComponentComponent**)range.begin, range.end - range.begin);
		}
	};

	struct MaterialParameter : public Parameter
	{
		[[bg3::getter]] inline std::span<channels::TLMaterialKeyComponentComponent*> PropertyParameters()
		{
			[[bg3::hidden]] Range range;
            getRange(range);
            return std::span((channels::TLMaterialKeyComponentComponent**)range.begin, range.end - range.begin);
		}
	};

	Array<Parameter*> MaterialParameters;
	// channels::EventKeyChannel<bool> VisibilityChannel;
	[[bg3::hidden]] std::array<__int8, 0xb8> raw_VisibilityChannel;
	[[bg3::getter]] channels::EventKeyChannel<bool>* VisibilityChannel()
	{
		return reinterpret_cast<channels::EventKeyChannel<bool>*>(&raw_VisibilityChannel);
	}
	bg3se::Guid GroupId;
	bool IsContinuous;
	bool IsOverlay;
	__int16 field_15a;
	float OverlayPriority;
};

struct TLPhysicsComponent : public TLEventKeyComponentComponent<keys::PhysicsKey>
{
};

struct TLPlayEffectEventComponent : public TLEventKeyComponentComponent<keys::PlayEffectEventKey>
{
	__int64 field_b0;
};

struct TLPlayRateComponent : public TLKeyBaseComponentComponent<keys::PlayRateKey>
{
};

struct TLShapeShiftComponent : public TLEventKeyComponentComponent<keys::ShapeShiftKey>
{
};

struct TLShotComponent : public TLBaseComponent
{
	__int64 field_80;
	bg3se::Guid FirstCamera;
	Array<bg3se::Guid> CameraContainers;
	bool AutomatedCamera;
	bool AutomatedLighting;
	bool DisableConditionalStaging;
	bool IsLooping;
	bool SwitchInterval;
	bool IsJCutEnabled;
	__int8 field_b1;
	__int16 field_b2;
	float JCutLength;
	__int64 field_b8;
	__int64 field_c0;
	__int64 field_c8;
	__int64 field_d0;
	bool IsLogicEnabled;
	__int8 field_d9;
	__int16 field_da;
	__int32 field_dc;
	bg3se::Guid CompanionCameraA;
	bg3se::Guid CompanionCameraB;
	bg3se::Guid CompanionCameraC;
	__int64 field_110;
	__int64 field_118;
	__int64 field_120;
	__int64 field_128;
	__int64 field_130;
	__int64 field_138;
	__int64 field_140;
	__int64 field_148;
	__int64 field_150;
};

struct TLShotHoldPreviousComponent : public TLBaseComponent
{
};

struct TLShotZoomComponent : public TLBaseComponent
{
	float StartOffset;
	float EndOffset;
};

struct TLShowArmorComponent : public TLBaseComponent
{
	__int64 field_80;
	Array<channels::TLShowArmorChannelComponent*> Channels;
	__int64 field_98;
};

struct TLShowHUDComponent : public TLEventKeyComponentComponent<keys::ShowHUDKey>
{
};

struct TLShowPeanutsComponent : public TLEventKeyComponentComponent<keys::ShowPeanutsKey>
{
};

struct TLShowVisualComponent : public TLEventKeyComponentComponent<keys::ShowVisualKey>
{
};

struct TLShowWeaponComponent : public TLEventKeyComponentComponent<keys::ShowWeaponKey>
{
};

struct TLSoundEventComponent : public TLEventKeyComponentComponent<keys::SoundEventKey>
{
	Array<TLFloatRTPCComponent*> RTPCChannels;
};

struct TLSplatterComponent : public TLBaseComponent
{
	Array<channels::TLSplatterChannelComponent*> Channels;
};

struct TLSpringsComponent : public TLEventKeyComponentComponent<keys::SpringsKey>
{
	uint32_t VisualFlag;
	__int32 field_b4;
};

struct TLSteppingFadeComponent : public TLEventKeyComponentComponent<keys::SteppingFadeKey>
{
};

struct TLSwitchLocationEventComponent : public TLEventKeyComponentComponent<keys::SwitchLocationEventKey>
{
};

struct TLSwitchStageEventComponent : public TLEventKeyComponentComponent<keys::SwitchStageEventKey>
{
	__int64 field_b0;
	__int64 field_b8;
};

struct TLTransformComponent : public TLBaseComponent
{
	channels::TLTransformChannelFloatComponent* field_80;
	channels::TLTransformChannelFloatComponent* field_88;
	channels::TLTransformChannelFloatComponent* field_90;
	channels::TLTransformChannelQuatComponent* field_98;
	channels::TLTransformChannelFloatComponent* field_a0;
	channels::TLTransformChannelFrameOfReferenceComponent* field_a8;
	bool Continuous;
	__int8 field_b1;
	__int16 field_b2;
	__int32 field_b4;
};

struct TLVoiceComponent : public TLBaseComponent
{
	bg3se::Guid DialogNodeId;
	bg3se::Guid ReferenceId;
	uint8_t FaceType;
	__int8 field_a1;
	__int16 field_a2;
	__int32 field_a4;
	double LeftBuffer;
	bool HoldMocap;
	bool DisableMocap;
	bool IsAliased;
	bool IsMirrored;
	__int32 field_b4;
	double PerformanceFade;
	double FadeIn;
	double FadeOut;
	double FadeInOffset;
	double FadeOutOffset;
	__int64 field_e0;
	uint8_t PerformanceDriftType;
	__int8 field_e9;
	__int16 field_ea;
	__int32 field_ec;
	double HeadPitchCorrection;
	double HeadYawCorrection;
	double HeadRollCorrection;
};


struct BillboardComponent : public BaseEffectComponent
{
	std::array<__int64, 77> field_b8;
};

struct BoundingBoxComponent : public BaseEffectComponent
{
	std::array<__int64, 6> field_b8;
};

struct BoundingSphereComponent : public BaseEffectComponent
{
	std::array<__int64, 4> field_b8;
};

struct CameraShakeComponent : public BaseEffectComponent
{
	std::array<__int64, 7> field_b8;
};

struct DecalComponent : public BaseEffectComponent
{
	std::array<__int64, 39> field_b8;
};

struct DeflectorComponent : public BaseEffectComponent
{
	std::array<__int64, 7> field_b8;
};

struct DragForceComponent : public BaseEffectComponent
{
	std::array<__int64, 10> field_b8;
};

struct GravityForceComponent : public BaseEffectComponent
{
	std::array<__int64, 10> field_b8;
};

struct LightComponent : public BaseEffectComponent
{
	std::array<__int64, 35> field_b8;
};

struct ModelComponent : public BaseEffectComponent
{
	std::array<__int64, 4> field_b8;
	__int32 field_d8;
	FixedString field_dc;
	FixedString field_e0;
	FixedString field_e4;
	std::array<__int64, 55> field_e8;
};

struct MovingLevelComponent : public BaseEffectComponent
{
	STDString field_b8;
	std::array<__int64, 5> field_d0;
};

struct OrbitForceComponent : public BaseEffectComponent
{
	std::array<__int64, 8> field_b8;
};

struct OverlayMaterialComponent : public BaseEffectComponent
{
	std::array<__int64, 10> field_b8;
	FixedString field_108;
	__int32 field_10c;
	std::array<__int64, 18> field_110;
};

struct ParticleSystemComponent : public BaseEffectComponent
{
	struct ParticleDatas
	{
		__int32 field_0;
		int32_t Count;
		Array<float> Lifespan;
		Array<float> Ages;
		Array<glm::fvec3> Positions;
		Array<glm::fvec3> Velocities;
		Array<uint16_t> Rotations;
		Array<glm::fvec3> Normals;
		Array<glm::fvec3> UpVectors;
		Array<uint16_t> FlipbookImageIndices;
		__int64 field_88;
		__int64 field_90;
		Array<uint16_t> InitialRotationSpeeds;
		Array<uint32_t> FixedColors;
		Array<uint16_t> FixedScalars;
		Array<float> DiscardValues;
		Array<float> UnitAges;
		Array<glm::fvec3> Scales;
		Array<glm::fvec3> AppliedForces;
	};
	std::array<__int64, 98> field_b8;
	ParticleDatas ParticleData;
	std::array<__int64, 8> field_4d0;
};

struct PostProcessComponent : public BaseEffectComponent
{
	std::array<__int64, 2> field_b8;
};

struct PreRollComponent : public BaseEffectComponent
{
};

struct RadialForceComponent : public BaseEffectComponent
{
	std::array<__int64, 10> field_b8;
};

struct Ribbon2Component : public BaseEffectComponent
{
	std::array<__int64, 63> field_b8;
};

struct SoundComponent : public BaseEffectComponent
{
	std::array<__int64, 5> field_b8;
	FixedString field_f0;
	FixedString field_f4;
	FixedString field_f8;
	__int32 field_fc;
	std::array<__int64, 3> field_100;
};

struct SpinForceComponent : public BaseEffectComponent
{
	std::array<__int64, 10> field_b8;
};

struct TurbulentForceComponent : public BaseEffectComponent
{
	std::array<__int64, 9> field_b8;
};

struct VortexForceComponent : public BaseEffectComponent
{
	std::array<__int64, 10> field_b8;
};

struct WindForceComponent : public BaseEffectComponent
{
	std::array<__int64, 4> field_b8;
};

END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(aspk::Property);
LUA_POLYMORPHIC(aspk::KeyFrameData);
LUA_POLYMORPHIC(aspk::Component);
LUA_POLYMORPHIC(aspk::TLMaterialComponent::Parameter);

END_NS()
