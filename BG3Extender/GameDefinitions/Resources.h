#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <Lua/LuaHelpers.h>
#include <synchapi.h>

BEGIN_SE()

struct [[bg3::hidden]] Bank : public ProtectedGameObject<Bank>
{
	void* VMT;
	Map<FixedString, resource::Resource*> Resources;
	SRWLOCK SRWLock;
	ResourceBankType BankTypeId;
};

struct [[bg3::hidden]] ResourceContainer : public ProtectedGameObject<ResourceContainer>
{
	void* VMT;
	std::array<Bank*, (unsigned)ResourceBankType::Sentinel> Banks;
};

struct [[bg3::hidden]] ResourcePackage;

struct [[bg3::hidden]] ResourceBank
{
	void* VMT;
	ResourceContainer Container;
	Map<FixedString, ResourcePackage*> Packages;
	void* LoadHelper;
	FixedString ModName;
};

struct [[bg3::hidden]] ResourcePackage
{
	void* VMT;
	ResourceBank* Bank;
	FixedString PackageName;
	STDString Path;
	RefMap<FixedString, void*> field_38;
};

struct ResourceManager
{
	__int64 field_0;
	[[bg3::hidden]] Map<FixedString, resource::Resource*> Resources;
	[[bg3::hidden]] std::array<ResourceBank*, 2> ResourceBanks;
	__int64 field_30; // Unknown pointer; size 1
	__int64 field_38;
	Array<FixedString> field_40;
	__int64 field_50; // Unknown pointer; size 0x50
	__int64 EffectFactory;
	__int64 field_60; // Unknown pointer; size 0x50
	__int64 field_68; // Unknown pointer; size 0x50
	__int64 field_70; // Unknown pointer; size 0x50
	__int64 field_78; // Unknown pointer; size 0x90
	__int64 field_80; // Unknown pointer; size 0x23e0
	__int64 field_88; // Unknown pointer; size 8
	__int64 SoundManager_M;
	__int64 field_98;
	__int64 field_A0;
	__int64 GameAnalytics_M;
	__int64 field_B0; // Unknown pointer; has vtable with a deleting destructor
	[[bg3::hidden]] CRITICAL_SECTION CriticalSection;
	[[bg3::hidden]] RefMap<STDString, void*> Unknown; // std::string -> ???
	__int64 field_e0;
	__int64 field_e8;
	[[bg3::hidden]] Map<FixedString, Path*> Paths;
	[[bg3::hidden]] Array<void*> Unknown2;            // Contains objects of size 0x20
	[[bg3::hidden]] Map<FixedString, void*> Unknown3; // Points to objects of size 0x110
	__int64 field_130;                                // Unknown pointer; size 0x180
	__int64 field_138; // Unknown pointer; size 0x40. Initialized to same pointer as field_140.
					   // Has virtual destructor.
	__int64 UI_M;      // Unknown pointer; size 0x40. Initialized to same pointer as field_138. Has
					   // virtual destructor.
};

END_SE()

BEGIN_NS(resource)

struct [[bg3::hidden]] BG3Variant;

struct [[bg3::hidden]] BG3Type
{
	__int64 Assign;
	__int64 Move_maybe;
	__int64 XMLRead;

	void (*FromString)(void* data, char* str);
	void (*ToString)(void* data, STDString* out);

	__int64 Destroy_maybe;
	__int64 operatorless_maybe;
	FixedString NormalTypeName_maybe;
	__int32 field_3c;
	uint64_t size;
	uint64_t alignment;
	FixedString TypeName;
	bool Trivial;
	[[bg3::hidden]] __int8 field_55;
	[[bg3::hidden]] __int16 field_56;
	BG3Variant* DefaultValue;
};

struct BG3Variant
{
	__int64 Value;
	__int64 Type;
	//BG3Type* Type;
};

struct Resource : public ProtectedGameObject<Resource>
{
	[[bg3::hidden]] void* VMT;
	Path SourceFile;
	FixedString Guid;
};

struct LockedResource : public resource::Resource
{
	[[bg3::hidden]] void* LockedResource_VMT;
	__int64 field_30;
	[[bg3::hidden]] SRWLOCK lock;
	__int64 field_40; // Unknown pointer
};

struct ResourceWithExtraData : public resource::Resource
{
	__int16 field_28;
	__int8 field_2a;
	[[bg3::hidden]] __int8 field_2b;
	[[bg3::hidden]] __int32 field_2c;
};

struct PresetData
{
	struct ScalarParameter
	{
		FixedString Parameter;
		float Value;
		bool Enabled;
		bool Color;
		bool Custom;
		[[bg3::hidden]] __int8 field_b;
	};

	struct Vector2Parameter
	{
		FixedString Parameter;
		glm::fvec2 Value;
		bool Enabled;
		bool Color;
		bool Custom;
		[[bg3::hidden]] __int8 field_f;
	};

	struct Vector3Parameter
	{
		FixedString Parameter;
		Vector3 Value;
		bool Enabled;
		bool Color;
		bool Custom;
		[[bg3::hidden]] __int8 field_13;
	};

	// alignas(16)
	struct VectorParameter
	{
		FixedString Parameter;
		[[bg3::hidden]] __int32 field_4;
		[[bg3::hidden]] __int64 field_8;
		glm::fvec4 Value; // alignas(16) for some reason, I guess?
		bool Enabled;
		bool Color;
		bool Custom;
		[[bg3::hidden]] __int8 field_23;
		[[bg3::hidden]] __int32 field_24;
		[[bg3::hidden]] __int64 field_28;
	};

	struct Texture2DParameter
	{
		FixedString Parameter;
		FixedString Value;
		bool Enabled;
		bool Color;
		bool Custom;
		[[bg3::hidden]] __int8 field_b;
	};

	struct VirtualTextureParameter
	{
		FixedString Parameter;
		FixedString Value;
		bool Enabled;
		bool Color;
		bool Custom;
		[[bg3::hidden]] __int8 field_b;
	};

	struct Mapped
	{
		FixedString GroupName;
		FixedString MaterialPresetResource;
		bool ForcePresetValues;
		[[bg3::hidden]] int8_t field_9;
		[[bg3::hidden]] int16_t field_a;
	};

	Array<ScalarParameter> ScalarParameters;
	Array<Vector2Parameter> Vector2Parameters;
	Array<Vector3Parameter> Vector3Parameters;
	Array<VectorParameter> VectorParameters;
	Array<Texture2DParameter> Texture2DParameters;
	Array<VirtualTextureParameter> VirtualTextureParameters;
	FixedString MaterialResource;
	[[bg3::hidden]] __int32 field_64;

	MultiHashMap<FixedString, Mapped> MaterialPresets;
};

struct AnimationResource : public resource::LockedResource
{
	struct Event
	{
		struct PropertiesHolder : public ProtectedGameObject<PropertiesHolder>
		{
			// struct PropertiesHolderVTable
			// {
			// 	void (*DeletingDestructor)(PropertiesHolder*, unsigned int);
			// 	bool (*parseXML)(PropertiesHolder*, void* XMLReader);
			// 	AnimationEventPropertyType (*GetPropertiesType)(PropertiesHolder*);
			// };

			// PropertiesHolderVTable* vtable;
			virtual ~PropertiesHolder() = 0;
			virtual bool parseXML(void* XMLReader) = 0;
			virtual AnimationEventPropertyType GetPropertyType() = 0;
		};

		struct SoundObjectProperties : public resource::AnimationResource::Event::PropertiesHolder
		{
			uint8_t SoundObjectIndex;
			[[bg3::hidden]] __int8 field_9;
			[[bg3::hidden]] __int8 field_a;
			[[bg3::hidden]] __int8 field_b;
			FixedString Resource;
		};

		struct FootProperties : public resource::AnimationResource::Event::PropertiesHolder
		{
			uint8_t FootID;
			bool Slide;
			bool PlaySound;
			bool PlayScuff;
			bool PlayStepFX;
			bool PlaySlideFX;
			bool PlayHearingFX;
			[[bg3::hidden]] __int8 field_f;
		};

		struct BoneEffectProperties : public resource::AnimationResource::Event::PropertiesHolder
		{
			FixedString Effect;
			FixedString Bone;
			[[bg3::hidden]] __int32 field_10;
			bool HardAttach;
			bool InterruptLoopAtEnd;
			[[bg3::hidden]] __int16 field_16;
		};

		struct BoneAttachBoneProperties : public resource::AnimationResource::Event::PropertiesHolder
		{
			FixedString Bone;
			FixedString AttachBone;
			bool Detach;
			[[bg3::hidden]] __int8 field_11;
			[[bg3::hidden]] __int16 field_12;
			[[bg3::hidden]] __int32 field_14;
		};

		struct WeaponEffectProperties : public resource::AnimationResource::Event::PropertiesHolder
		{
			int Weapon;
			int EffectType;
		};

		struct Unknown7Properties : public resource::AnimationResource::Event::PropertiesHolder
		{
			// Empty
		};

		struct Unknown8Properties : public resource::AnimationResource::Event::PropertiesHolder
		{
			// Empty
		};

		struct Prop9Properties : public resource::AnimationResource::Event::PropertiesHolder
		{
			float AngularVelocityModifier;
			float LinearVelocityModifier;
			float StiffnessMultiplier;
			float DampingMultiplier;
			float DurationHardLimit;
			float DurationSoftLimit;
			float HitImpulseThreshold;
			bool HoldLastFrame;
			[[bg3::hidden]] __int8 field_25;
			[[bg3::hidden]] __int16 field_26;
			FixedString AnimationResourceID;
			bool LockLimits;
			[[bg3::hidden]] __int8 field_2d;
			[[bg3::hidden]] __int16 field_2e;
		};

		struct Prop10Properties : public resource::AnimationResource::Event::PropertiesHolder
		{
			uint32_t VisualFlag;
			bool Enabled;
			[[bg3::hidden]] __int8 field_d;
			[[bg3::hidden]] __int16 field_e;
		};

		struct FloatDataProperties : public resource::AnimationResource::Event::PropertiesHolder
		{
			float Data;
			[[bg3::hidden]] __int32 field_c;
		};

		struct EFoleySoundObjectProperties : public resource::AnimationResource::Event::SoundObjectProperties
		{
			uint8_t EFoleyType;
			uint8_t EFoleyIntensity;
			[[bg3::hidden]] __int16 field_12;
			[[bg3::hidden]] __int32 field_14;
		};

		struct EVocalSoundObjectProperties : public resource::AnimationResource::Event::SoundObjectProperties
		{
			uint8_t EVocalType;
			[[bg3::hidden]] __int8 field_11;
			[[bg3::hidden]] __int16 field_12;
			[[bg3::hidden]] __int32 field_14;
		};

		struct U8DataProperties : public resource::AnimationResource::Event::PropertiesHolder
		{
			uint8_t Data;
			[[bg3::hidden]] __int8 field_9;
			[[bg3::hidden]] __int16 field_a;
			[[bg3::hidden]] __int32 field_c;
		};

		struct Unknown15Properties : public resource::AnimationResource::Event::PropertiesHolder
		{
			// Empty
		};

		FixedString ID;
		float Time;
		float Length;
		uint8_t Track;
		[[bg3::hidden]] __int8 field_d;
		[[bg3::hidden]] __int16 field_e;

		PropertiesHolder* Properties;
	};

	Array<Event*> Events;
	FixedString Template;
	FixedString AdditiveLoopingAnimationID;
	FixedString LeftTransitionAnimation;
	FixedString RightTransitionAnimation;
	FixedString SkeletonResource;
	float TimeStep;
	float Duration;
	float Offset;
	bool Looping;
	uint8_t AnchorHand;
	uint8_t SupportingLeg;
	[[bg3::hidden]] __int8 field_7b;
	[[bg3::hidden]] __int32 field_7c;
};

struct AnimationBlueprintResource : public resource::LockedResource
{
	[[bg3::hidden]] void* field_48;              // Deleter
	Array<__int64> field_50; // Vtable pointers??? Why???

	// Some sort of variant? I have no idea how to handle this
	// Appears to use TypeInformationRepository information to construct itself
	RefMap<bg3se::Guid, BG3Variant> Params;
};

struct AnimationSetResource : public resource::ResourceWithExtraData
{
	struct Subset
	{
		struct Animations
		{
			FixedString ID;
			AnimationSetAnimationFlags flags;
			[[bg3::hidden]] __int8 field_5;
			[[bg3::hidden]] __int16 field_6;
			[[bg3::hidden]] __int32 field_8;
		};

		RefMap<FixedString, Animations> Animation;
		FixedString FallBackSubSet;
		[[bg3::hidden]] __int32 field_14;
	};

	struct Bank
	{
		RefMap<FixedString, Subset> AnimationSubSets;
		bg3se::Guid ShortNameSetId;
	};

	Bank* AnimationBank;
};

struct AtmosphereResource : public resource::ResourceWithExtraData
{
	struct AtmosphereData
	{
		struct Range
		{
			glm::fvec2 XYOffset;
			float Saturation;
			float ExposureShift;
		};
		struct ExposureSetting
		{
			float ExposureMin;
			float ExposureMax;
			float ExposureCompensation;
			float ExposureMinPercentile;
			float ExposureMaxPercentile;
			int32_t ExposureMeteringMode;
			bool Exposure;
			__int8 field_19;
			__int16 field_1a;
			__int32 field_1c;
			__int32 field_20;
			__int32 field_24;
			__int32 field_28;
		};
		[[bg3::hidden]] void* VMT;
		FixedString field_8;
		uint32_t InheritanceFlags;
		std::array<__int64, 8> field_10;
		Range GlobalRange;
		Range DarkRange;
		Range ShadowRange;
		Range MidtonesRange;
		Range HighlightRange;
		Range SpecularRange;
		glm::fvec2 LutBlendFactors;
		int32_t LutApplyMode;
		float Hue;
		float Saturation;
		float Contrast;
		float Pivot;
		float Shadows;
		float Highlights;
		__int32 field_d4;
		std::array<__int64, 19> field_d8;
		__int32 field_170;
		float WhiteBalanceTemperature;
		float WhiteBalanceTint;
		bool GradingLutEnabled;
		__int8 field_17d;
		__int16 field_17e;
		FixedString GradingLut;
		__int32 field_184;
		__int64 field_188;
		__int32 field_190;
		bool DOF;
		bool DOFNearOnly;
		bool DOFFarOnly;
		__int8 field_197;
		float DOFFocalDistance;
		float DOFNearSharpDistance;
		float DOFFarSharpDistance;
		float DOFAperature;
		__int64 field_1a8;
		__int64 field_1b0;
		__int32 field_1b8;
		std::array<ExposureSetting, 2> ExposureSettings;
		float LensFlareTreshold; // Yes, the game misspells it
		float LensFlareIntensity;
		float LensFlareGhostDispersal;
		float LensFlareHaloWidth;
		float LensFlareChromaticDistortion;
		bool LensFlareEnabled;
		__int8 field_229;
		__int16 field_22a;
		float Threshold;
		float Amount;
		float GodRaysIntensity;
		float GodRaysPower;
		float GodRaysThreshold;
		float ToneMapHighlightsFixAmount;
		__int32 field_224;
		glm::fvec3 VignetteColor;
		float VignetteIntensity;
		float VignettePower;
		bool Bloom;
		bool GodRays;
		bool Vignette;
		__int8 field_25f;
		float WindDirection;
		float WindSpeed;
		float ClothWindSpeed;
		float ClothWindVariance;
		float ClothMainWindSpeed;
		float ClothMainWindFrequency;
		float ClothWindDirectionOffsetFrequency;
		float ClothMaxWindDirectionOffset;
		float NearPlane;
		float FarPlane;
		bool EnvironmentEffectGlobalEnabled;
		bool EnvironmentEffectEnabled;
		bool EnvironmentEffectEnabled1;
		bool EnvironmentEffectEnabled2;
		bool EnvironmentEffectEnabled3;
		bool EnvironmentEffectEnabledForTimeline;
		bool EnvironmentEffectEnabledForTimeline1;
		bool EnvironmentEffectEnabledForTimeline2;
		bool EnvironmentEffectEnabledForTimeline3;
		__int8 field_291;
		__int16 field_292;
		FixedString EnvironmentEffect;
		FixedString EnvironmentEffect1;
		FixedString EnvironmentEffect2;
		FixedString EnvironmentEffect3;
		float EnvironmentEffectOffset;
		float EnvironmentEffectOffset1;
		float EnvironmentEffectOffset2;
		float EnvironmentEffectOffset3;
		bool LocalLightSourceEnabled;
		bool LocalLightSourceOverrideSettings;
		__int16 field_2b6;
		glm::fvec3 LocalLightSourceColor;
		float LocalLightSourceIntensity;
		bg3se::Guid TimelineAutomaticLightingDefaultSetup;
		bool TimelineAutomaticLightingDisableFlip;
		__int8 field_2d9;
		__int16 field_2da;
		__int32 field_2dc;
		__int64 field_2e0;
		__int64 field_2e8;
	};
	AtmosphereData* Atmosphere;
	Array<FixedString> Labels;
};

struct BlendSpaceResource : public resource::LockedResource
{
	struct [[bg3::hidden]] BlendSpaceInternals;

	struct Input
	{
		glm::vec2 Position;
		FixedString AnimationID;
		FixedString ShortNameID;
	};

	struct InputInfo
	{
		[[bg3::hidden]] void* ContainingInternals; // Points to XAxisName
		glm::vec2 Position;
		FixedString AnimationID;
		FixedString ShortNameID;
	};

	STDString XAxisName;
	STDString YAxisName;
	int32_t XElements;
	int32_t YElements;
	float MinXValue;
	float MaxXValue;
	float MinYValue;
	float MaxYValue;

	struct Field90Entry
	{
		__int32 field_0;
		__int32 field_4;
		__int32 field_8;
		__int32 field_c;
		__int16 field_10;
		__int16 field_12;
	};

	struct FieldB8Entry
	{
		__int32 field_0;
		__int32 field_4;
		__int32 field_8;
		__int32 field_c;
		__int32 field_10;
		__int32 field_14;
	};

	Array<Field90Entry> field_90;

	Array<InputInfo*> MappedInputs;
	bool NeedsAdjustment_maybe;
	[[bg3::hidden]] __int8 field_b1;
	[[bg3::hidden]] __int16 field_b2;
	[[bg3::hidden]] __int32 field_b4;
	Array<FieldB8Entry> field_b8;
};

struct CharacterVisualResource : public resource::ResourceWithExtraData
{
	struct Slot
	{
		FixedString Slot;
		FixedString VisualResource;
		FixedString Bone;
	};

	struct LocatorAttachment
	{
		FixedString LocatorId;
		FixedString VisualResource;
	};

	FixedString BodySetVisual;
	[[bg3::hidden]] __int32 field_34;
	Array<Slot> Slots;
	Array<LocatorAttachment> LocatorAttachments;
	PresetData MaterialOverrides;
	Map<FixedString, PresetData> Materials;
	MultiHashMap<FixedString, FixedString> RealMaterialOverrides;
	MultiHashMap<FixedString, FixedString> field_158;
	FixedString ID;
	bool ShowEquipmentVisuals;
	[[bg3::hidden]] __int8 field_19d;
	[[bg3::hidden]] __int16 field_19e;
	FixedString BaseVisual;
	[[bg3::hidden]] __int32 field_1a4;
};

struct ClothColliderResource : public resource::ResourceWithExtraData
{
	struct Sphere
	{
		glm::fvec3 Position;
		float Radius;
		FixedString Name;
		[[bg3::hidden]] __int32 field_14;
		Array<FixedString> Links;
		FixedString AttachedName;
		[[bg3::hidden]] __int32 field_2c;
	};

	Array<Sphere> Spheres;
};

struct ColorListResource : public resource::ResourceWithExtraData
{
	[[bg3::hidden]] __int64 field_30; // Dynamic array vtable
	Array<uint32_t> Colors;
	[[bg3::hidden]] __int64 field_48; // Dynamic array other info

	[[bg3::hidden]] __int64 field_50; // Dynamic array vtable
	Array<int32_t> Ids;
	[[bg3::hidden]] __int64 field_68; // Dynamic array other info

	int32_t IdGenerator;
	[[bg3::hidden]] __int32 field_74;
};

struct DialogResource : public resource::ResourceWithExtraData
{
	bool automated;
	bool isWorld;
	bool isBehaviour;
	bool EnableTimeline;
	bool BlockFTB;
	bool isSubbedDialog;
	bool isPrivateDialog;
	bool AllowDeadSpeakers;
	bool IsAllowingJoinCombat;
	[[bg3::hidden]] __int8 field_39;
	[[bg3::hidden]] __int16 field_3a;
	[[bg3::hidden]] __int32 field_3c;
	Array<bool> SpeakerSlotsWithLines;
	Array<bg3se::Guid> childResources;
	uint8_t TimelineSplitScreenPriority;
	[[bg3::hidden]] __int8 field_61;
	[[bg3::hidden]] __int16 field_62;
	[[bg3::hidden]] __int32 field_64;
	STDString Name;
};

struct DiffusionProfileResource : public resource::ResourceWithExtraData
{
	glm::fvec3 ScatterColor;
	float ScatterDistance;
	glm::fvec3 TransmissionTint;
	float ThicknessRemapMin;
	float ThicknessRemapMax;
	bool ThickObjectTransmission;
	[[bg3::hidden]] __int8 field_55;
	[[bg3::hidden]] __int16 field_56;
	float DualSpecularRoughnessA;
	float DualSpecularRoughnessB;
	float DualSpecularMix;
	[[bg3::hidden]] __int32 field_64;
};

struct EffectResource : public resource::LockedResource
{
	// In RE, this is TimelineContent
	struct Data
	{
		struct Subdata
		{
			__int64 field_0;
			__int64 field_8;
			__int64 field_10;
			__int64 field_18;
			__int64 field_20;
			__int64 field_28;
			__int64 field_30;
			__int64 field_38;
			__int64 field_40;
			__int64 field_48;
		};

		__int64 field_0;
		[[bg3::hidden]] __int64 field_8; // Dynamic array VMT
		Array<Subdata*> field_10;
		[[bg3::hidden]] __int64 field_20; // Dynamic array extra data
		Array<__int64> field_28;          // pointers to ????
		Array<__int64> field_38;          // pointers to ????
		__int32 field_40;
		__int32 field_44;
		__int8 field_48;
		__int8 field_49;
		__int8 field_4a;
		__int8 field_4b;
		FixedString field_4c;
	};

	struct EffectComponent
	{
		struct Property : public ProtectedGameObject<Property>
		{
			struct Parameter
			{
				STDString Name;
				float Value;
				[[bg3::hidden]] __int32 field_1c;
			};

			virtual ~Property() = 0;
			virtual void UnknownFunc1(__int64, __int64, __int64) = 0;
			virtual bool parseXML(void* XMLReader) = 0;
			virtual EffectPropertyType GetPropertyType() = 0;

			FixedString FullName;
			FixedString AttributeName;
			[[bg3::hidden]] void* ParametersVMT;
			Array<Parameter> Parameters;
			[[bg3::hidden]] __int64 ParametersExtraData;
			FixedString Input;
			[[bg3::hidden]] __int32 field_34;
		};

		struct BoolProperty : public resource::EffectResource::EffectComponent::Property
		{
			bool Value;
			[[bg3::hidden]] __int8 field_39;
			[[bg3::hidden]] __int16 field_3a;
			[[bg3::hidden]] __int32 field_3c;
		};

		struct Int32Property : public resource::EffectResource::EffectComponent::Property
		{
			int32_t Value;
			[[bg3::hidden]] __int32 field_3c;
		};

		struct Int32RangeProperty : public resource::EffectResource::EffectComponent::Property
		{
			int32_t Min;
			int32_t Max;
		};

		struct ColoredFramesProperty : public resource::EffectResource::EffectComponent::Property
		{
			struct FramesArray
			{
				struct Frame
				{
					float Time;
					[[bg3::hidden]] __int32 field_4;
					[[bg3::hidden]] __int64 field_8;
					glm::fvec4 Color;
				};

				[[bg3::hidden]] void* VMT;
				Array<Frame> data;
				[[bg3::hidden]] __int64 field_18;
			};
			FramesArray* Frames;
		};

		struct FloatProperty : public resource::EffectResource::EffectComponent::Property
		{
			float Value;
			[[bg3::hidden]] __int32 field_3c;
		};

		struct FloatRangeProperty : public resource::EffectResource::EffectComponent::Property
		{
			float Min;
			float Max;
		};

		struct FramesProperty : public resource::EffectResource::EffectComponent::Property
		{
			struct VirtualFrames : public ProtectedGameObject<VirtualFrames>
			{
				virtual ~VirtualFrames() = 0;
				virtual int GetType() = 0;
			};
			struct Frames0 : public resource::EffectResource::EffectComponent::FramesProperty::VirtualFrames
			{
				struct Frame
				{
					__int64 field_0;
					float Time;
					__int32 field_c;
					float Value;
					__int32 field_14;
				};
				[[bg3::hidden]] __int64 Frames_VMT;
				Array<Frame> Frames;
				[[bg3::hidden]] __int64 Frames_ExtraData;
			};

			struct Frames1 : public resource::EffectResource::EffectComponent::FramesProperty::VirtualFrames
			{
				struct Frame
				{
					__int64 field_0;
					float Time;
					__int32 field_c;
					float A;
					float B;
					float C;
					float D;
				};
				[[bg3::hidden]] __int64 Frames_VMT;
				Array<Frame> Frames;
				[[bg3::hidden]] __int64 Frames_ExtraData;
			};
			[[bg3::hidden]] void* Frames_VMT;
			Array<VirtualFrames*> Frames;
			[[bg3::hidden]] __int64 Frames_ExtraData;
		};

		struct STDStringProperty : public resource::EffectResource::EffectComponent::Property
		{
			STDString Value;
		};

		struct Vector3Property : public resource::EffectResource::EffectComponent::Property
		{
			Vector3 Value;
			[[bg3::hidden]] __int32 field_44;
		};

		struct FunctionTypeProperty : public resource::EffectResource::EffectComponent::Property
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

			struct Function0 : public resource::EffectResource::EffectComponent::FunctionTypeProperty::VirtualFunction
			{
				float A;
				float B;
			};

			struct Function1 : public resource::EffectResource::EffectComponent::FunctionTypeProperty::VirtualFunction
			{
				float A;
				float B;
				float C;
				[[bg3::hidden]] __int32 field_14;
			};

			struct Function2 : public resource::EffectResource::EffectComponent::FunctionTypeProperty::VirtualFunction
			{
				float A;
				float B;
				float C;
				float D;
			};

			VirtualFunction* Function;
		};

		struct FixedStringProperty : public resource::EffectResource::EffectComponent::Property
		{
			FixedString Value;
			[[bg3::hidden]] __int32 field_44;
		};

		struct BaseProperty : public resource::EffectResource::EffectComponent::Property
		{
		};

		struct Module
		{
			STDString Name;
			Array<FixedString> FullName;
			Array<Property*> Properties;
		};

		[[bg3::hidden]] void* VMT;
		bg3se::Guid GUID;
		STDString Name;
		__int64 field_30;
		uint32_t Track;
		float StartTime;
		float EndTime;
		__int32 field_44;
		__int64 field_48;
		__int64 field_50;
		__int64 field_58;
		__int64 field_60;
		__int64 field_68;
		__int64 field_70;
		__int64 field_78;
		__int64 field_80;
		Array<Module*> Modules;
		Array<Property*> Properties;
		__int64 field_a8;
		__int64 field_b0;
		__int64 field_b8;
	};

	FixedString EffectName;
	[[bg3::hidden]] __int32 field_4c;
	Array<EffectComponent*> EffectComponents;
	Array<FixedString> Dependencies;
	__int64 field_70; // pointer to structure, size 0x50
	__int64 field_78;
	glm::fvec3 BoundsMin;
	glm::fvec3 BoundsMax;
	float CullingDistance;
	float Duration;
	bool Looping;
	bool UseSoundOcclusion;
	[[bg3::hidden]] __int16 field_a2;
	uint32_t InterruptionMode;
};

struct FCurveResource : public resource::LockedResource
{
	struct CurveKey
	{
		[[bg3::hidden]] void* ContainingInternals; // Pointer to the CurveKeys of containing FCurveResource
		glm::fvec2 CurveKeyPosition;
		float CurveKeyLeftTangent;
		float CurveKeyRightTangent;
		int32_t CurveKeyInterpolationType;
		int32_t CurveKeyConstraintType;
	};

	struct CurveLimit
	{
		float MinX;
		float MinY;
		float MaxX;
		float MaxY;
	};

	Array<CurveKey*> CurveKeys;

	CurveLimit CurveLimits;

	struct ProcessedCurveKey
	{
		glm::fvec2 CurveKeyPosition;
		float Tan_CurveKeyLeftTangent;
		float Tan_CurveKeyRightTangent;
		int32_t CurveKeyInterpolationType;
	};

	Array<ProcessedCurveKey> ProcessedCurveKeys;
};

struct IKRigResource : public resource::ResourceWithExtraData
{
	struct BoneProperty
	{
		bool Enabled;
		bool DoFIncludesX;
		bool DoFIncludesY;
		bool DoFIncludesZ;
		bool LimitEnabled;
		uint8_t LimitAxis;
		[[bg3::hidden]] __int16 field_6;
		glm::fvec3 Weight;
		glm::fvec3 Min;
		glm::fvec3 Max;
	};

	struct IKTask
	{
		__int32 field_0;
		FixedString Bone;
		int32_t IKBoneType;
		FixedString IKBoneName;
		FixedString IKBoneCachedName;
		bool PositionEnabled;
		bool RotationEnabled; // RotationEnabled?
		bool PositionDoFIncludesX;
		bool PositionDoFIncludesY;
		bool PositionDoFIncludesZ;
		bool RotationDoFIncludesX;
		bool RotationDoFIncludesY;
		bool RotationDoFIncludesZ;
		int32_t PositionDepth;
		int32_t RotationDepth;
		float PositionWeight;
		float RotationWeight;
		glm::fvec3 LookAtAxis;
		bool IsPoleObject;
		__int8 field_39;
		bool IsParentTask;
		bool LookAtLocalSpace;
		bool LookAt;
		[[bg3::hidden]] __int8 field_3d;
		[[bg3::hidden]] __int16 field_3e;
	};

	RefMap<FixedString, BoneProperty> BoneProperties;

	[[bg3::hidden]] void* field_40; // DynamicArray VMT
	StaticArray<IKTask> IKTasks;
	[[bg3::hidden]] __int64 field_58; // DynamicArray extra data

	bool SolverRoot;
	[[bg3::hidden]] __int8 field_61;
	[[bg3::hidden]] __int16 field_62;
	[[bg3::hidden]] __int32 field_64;
	MultiHashMap<bg3se::Guid, FixedString> BoneCategories;
};

struct LightCookieResource : public resource::LockedResource
{
	__int64 field_48;
	__int64 field_50;
	__int64 field_58;
	__int64 field_60;
	__int64 field_68;
	__int64 field_70;
	__int64 field_78;
	__int64 field_80;
	__int64 field_88;
	__int64 field_90;
	__int64 field_98;
	__int64 field_a0;
	__int64 field_a8;
	__int64 field_b0;
	__int64 field_b8;
	__int64 field_c0;
	__int64 field_c8;
};

struct LightingResource : public resource::ResourceWithExtraData
{
	struct Details : public ProtectedGameObject<Details>
	{
		[[bg3::hidden]] void* VMT;
		FixedString field_8;
		std::array<__int8, 0x368> data;
	};

	// Lighting details; pointer to 0x378 size structure. Very scary constructor and destructor;
	// can't make heads nor tails of it
	Details* field_48;
};

struct MaterialResource : public resource::LockedResource
{
	struct ScalarParameter
	{
		FixedString ParameterName;
		bool Enabled;
		[[bg3::hidden]] __int8 field_5;
		[[bg3::hidden]] __int16 field_6;
		float Value;
		float BaseValue; // Reversed?
	};

	struct Vector2Parameter
	{
		FixedString ParameterName;
		bool Enabled;
		[[bg3::hidden]] __int8 field_5;
		[[bg3::hidden]] __int16 field_6;
		glm::fvec2 Value;
		glm::fvec2 BaseValue;
	};

	struct Vector3Parameter
	{
		FixedString ParameterName;
		bool Enabled;
		[[bg3::hidden]] __int8 field_5;
		[[bg3::hidden]] __int16 field_6;
		glm::fvec3 Value;
		glm::fvec3 BaseValue;
		bool IsColor;
		[[bg3::hidden]] __int8 field_21;
		[[bg3::hidden]] __int16 field_22;
	};

	// alignas(16)
	struct VectorParameter
	{
		FixedString ParameterName;
		bool Enabled;
		[[bg3::hidden]] __int8 field_5;
		[[bg3::hidden]] __int16 field_6;
		[[bg3::hidden]] __int64 field_8;
		glm::fvec4 Value;
		glm::fvec4 BaseValue;
		bool IsColor;
		[[bg3::hidden]] __int8 field_31;
		[[bg3::hidden]] __int16 field_32;
		[[bg3::hidden]] __int32 field_34;
		[[bg3::hidden]] __int64 field_38;
	};

	struct Texture2DParameter
	{
		FixedString ParameterName;
		bool Enabled;
		[[bg3::hidden]] __int8 field_5;
		[[bg3::hidden]] __int16 field_6;
		FixedString ID;
	};

	struct VirtualTextureParameter
	{
		FixedString Parameter;
		bool Enabled;
		[[bg3::hidden]] __int8 field_5;
		[[bg3::hidden]] __int16 field_6;
		int32_t Index;
		FixedString ID;
	};

	__int64 field_48;
	Array<ScalarParameter> ScalarParameters;
	Array<Vector2Parameter> Vector2Parameters;
	Array<Vector3Parameter> Vector3Parameters;
	Array<VectorParameter> VectorParameters;
	Array<Texture2DParameter> Texture2DParameters;
	Array<VirtualTextureParameter> VirtualTextureParameters;
	FixedString DiffusionProfileUUID;
	uint8_t MaterialType;
	uint8_t RenderChannel;
	[[bg3::hidden]] __int16 field_b6;
};

struct MaterialPresetResource : public resource::ResourceWithExtraData
{
	PresetData Presets;
};

// I think this is unimplemented and std::terminates on construction
struct MaterialSetResource : public resource::LockedResource
{
};

struct MeshProxyResource : public resource::LockedResource
{
	__int64 field_48;
	FixedString Template;
	[[bg3::hidden]] __int32 field_54;
};

struct PhysicsResource : public resource::ResourceWithExtraData
{
	struct ObjectTemplate
	{
		struct PhysicsObject
		{
			virtual ~PhysicsObject() = 0;
			virtual FixedString* GetType() = 0;
		};

		struct PhysicsBox : public resource::PhysicsResource::ObjectTemplate::PhysicsObject
		{
			[[bg3::hidden]] void* VMT;
			bool Kinematic;
			[[bg3::hidden]] __int8 field_9;
			[[bg3::hidden]] __int16 field_a;
			float Mass;
			FixedString UUID;
			[[bg3::hidden]] __int32 field_14;
			glm::fvec3 Extends;
			[[bg3::hidden]] __int32 field_24;
		};

		struct PhysicsCapsule : public resource::PhysicsResource::ObjectTemplate::PhysicsObject
		{
			[[bg3::hidden]] void* VMT;
			bool Kinematic;
			[[bg3::hidden]] __int8 field_9;
			[[bg3::hidden]] __int16 field_a;
			float Mass;
			FixedString UUID;
			[[bg3::hidden]] __int32 field_14;
			float Height;
			float Radius;
		};

		FixedString UUID;
		[[bg3::hidden]] __int32 field_4;
		// This holds either a PhysicsCapsule or a PhysicsBox. Not sure how to have that
		// displayed properly
		Array<PhysicsObject*> PhysicsObjects;
	};

	Array<ObjectTemplate*> ObjectTemplates;
	Array<FixedString> IgnoreColliderList;
	FixedString Template;
	bool field_54;
	[[bg3::hidden]] __int8 field_55;
	[[bg3::hidden]] __int16 field_56;
};

struct ScriptResource : public resource::ResourceWithExtraData
{
	struct Parameter
	{
		int32_t Type;
		[[bg3::hidden]] __int32 field_4;
		STDString Value;
	};

	// This is a map to a Parameter*, but for some reason the proxy doesn't allow that
	Map<FixedString, __int64> Parameters;
};

struct SkeletonResource : public resource::LockedResource
{
	struct Socket
	{
		FixedString Name;
		FixedString Bone;
		FixedString BoneConstraint;
		uint16_t ConstraintsPos;
		uint16_t ConstraintsRot;
		glm::fvec4 Rotation;
		glm::fvec3 Position;
	};

	struct Bone
	{
		struct BoneGroup
		{
			int32_t IKBoneType;
			FixedString IKBoneName;
			FixedString IKBoneCachedName;
		};
		Array<BoneGroup> BoneGroups;
		glm::fvec3 Position;
		glm::fvec4 Rotation;
		float SoundObjectActivationRange;
		uint8_t SoundObjectIndex;
		bool ShouldLOD;
		[[bg3::hidden]] __int16 field_32;
		[[bg3::hidden]] __int32 field_34;
	};

	RefMap<FixedString, Bone> Bones;
	[[bg3::hidden]] void* field_58; // DynamicArray VMT
	Array<Socket> Sockets;
	[[bg3::hidden]] __int64 field_70; // DynamicArray extra data
	FixedString RagdollResourceID;
	FixedString SpringResourceID;
	FixedString DynamicPhysicsResourceID;
	FixedString SoftbodyResourceID;
	FixedString Template;
	FixedString IKRigResourceID;
	FixedString ClothColliderResourceID;
	FixedString PreviewAnimationResource;
	__int64 field_98;
};

struct SkeletonMirrorTableResource : public resource::LockedResource
{
	struct Entry
	{
		FixedString LeftBone;
		FixedString RightBone;
	};

	Array<Entry> Entries;
};

struct SkinPresetResource : public resource::ResourceWithExtraData
{
	PresetData Presets;
};

struct SoundResource : public resource::LockedResource
{
	uint32_t SoundEventID;
	FixedString SoundEvent;
	FixedString SoundBank;
	[[bg3::hidden]] __int32 field_54;
	bg3se::Guid SoundEventUUID;
	float Duration;
	float MaxDistance;
	bool Preload;
	int8_t SoundCategory;
	int8_t SoundCodec;
	bool Internal;
	[[bg3::hidden]] __int32 field_74;
};

struct TerrainBrushResource : public resource::ResourceWithExtraData
{
	FixedString BaseColorMap;
	FixedString NormalMap;
	FixedString PhysicalMap;
	FixedString DetailNormalMap;
	float HeightScale;
	float HeightOffset;
	float HeightContrast;
	float AlphaThreshold_maybe;
	float Tiling;
	float DetailNormalScale;
	float DetailNormalTiling;
	float GradientHeightBottom;
	float GradientHeightMidPoint;
	float GradientHeightTop;
	float GradientRoughnessReach;
	float GradientRoughnessPower;
	glm::fvec3 GradientBottomHeightColor;
	glm::fvec3 GradientMidHeightColor;
	bool UseDetailNormalMap;
	bool UseHeightGradient_maybe;
	bool IsBlackBrush;
	uint8_t MaterialType;
	[[bg3::hidden]] __int32 field_8c;
};

struct TextureResource : public resource::ResourceWithExtraData
{
	__int64 field_30;
	FixedString Template;
	bool Streaming;
	[[bg3::hidden]] __int8 field_3d;
	[[bg3::hidden]] __int16 field_3e;
	int32_t Type;
	int32_t Width;
	int32_t Height;
	int32_t Depth;
	bool SRGB;
	[[bg3::hidden]] __int8 field_51;
	[[bg3::hidden]] __int16 field_52;
	[[bg3::hidden]] __int32 field_54;
};

struct TileSetResource : public resource::ResourceWithExtraData
{
	struct Tile
	{
		FixedString VisualGUID;
		FixedString PhysicsGUID;
		FixedString InvisibleClimbingHelperPhysicsGUID;
		[[bg3::hidden]] __int32 field_10;
	};

	struct ExtendedRefMap
	{
		RefMap<bg3se::Guid, Tile> Tiles;
		bool IsRoof;
		[[bg3::hidden]] __int8 field_11;
		[[bg3::hidden]] __int16 field_12;
		[[bg3::hidden]] __int32 field_14;
	};

	ExtendedRefMap* TileSet;
};

struct TimelineResource : public resource::ResourceWithExtraData
{
	bg3se::Guid DialogResourceId;
	bool FadeOutOnEnd;
	[[bg3::hidden]] __int8 field_41;
	[[bg3::hidden]] __int16 field_42;
	float FadeOutDuration;
	int32_t BodyPartMocap;
	[[bg3::hidden]] __int32 field_4c;
};

struct TimelineSceneResource : public resource::ResourceWithExtraData
{
	Array<FixedString> Labels;
	uint8_t SceneType;
	[[bg3::hidden]] __int8 field_41;
	[[bg3::hidden]] __int16 field_42;
	int32_t AdditionalSpeakerCount;
};

struct VirtualTextureResource : public resource::ResourceWithExtraData
{
	__int32 field_30;
	uint32_t VirtualTextureLayerConfig;
	__int64 field_38;
	FixedString GTexFileName;
	__int32 field_44;
	__int64 field_48;
	bool Prefetch;
	int8_t PrefetchMipLevel;
	__int16 field_52;
	uint32_t ReferencedColorSpaces;
};

struct VisualResource : public resource::LockedResource
{
	struct Object
	{
		FixedString ObjectID;
		FixedString MaterialID;
		__int8 LOD;
		[[bg3::hidden]] __int8 field_9;
		[[bg3::hidden]] __int16 field_10;
	};

	struct Attachment
	{
		FixedString UUID;
		FixedString Name;
	};

	struct ClothParam
	{
		FixedString UUID;
		float LinearStiffness;
		float BendingStiffness;
		float CompressionLimit;
		float StretchLimit;
		float StiffnessMultiplier;
		glm::fvec3 LinearDrag;
		glm::fvec3 AngularDrag;
		glm::fvec3 LinearInertia;
		glm::fvec3 AngularInertia;
		glm::fvec3 CentrifugalInertia;
		glm::fvec3 Damping;
		float CollisionMassScale;
		float DragCoefficient;
		float FluidDensity;
		float Friction;
		float LiftCoefficient;
		float TetherConstraintScale;
		float TetherConstraintStiffness;
		float SolverFrequency;
		float StiffnessFrequency;
		float FrontalWindSpeed;
		float FrontalWindVariance;
		float AnimDriveSpringForce;
		float AnimDriveDampingForce;
		float ClothWindSpeed;
		float ClothMainWindVariance;
		float ClothMainWindSpeed;
		float ClothMainWindFrequency;
		float ClothWindDirectionOffsetFrequency;
		float ClothMaxWindDirectionOffset;
		float SelfCollisionDistance;
		float SelfCollisionStiffness;
		bool EnableVirtualParticles;
		bool EnableCCD;
		bool AtmosphericWindEnabled;
		bool OverrideAtmosphericWind;
		float AnimInfluenceMultiplier;
	};

	struct AnimationSetOverride
	{
		bg3se::Guid PairElement1;
		FixedString PairElement2;
		int32_t strAnimationSetOverrideType;
	};

	struct Bone
	{
		glm::fvec3 Position;
		glm::fvec4 Rotation;
	};

	struct ClothProxy
	{
		FixedString Name;
		[[bg3::hidden]] __int32 field_4;
		Array<__int16> ClosestVertices;
	};

	[[bg3::hidden]] void* field_48; // DynamicArray VMT
	Array<FixedString> Tags;
	[[bg3::hidden]] __int64 field_60; // DynamicArray extra data
	Array<Object> Objects;
	Array<Attachment> Attachments;
	Array<ClothParam> ClothParams;
	Array<AnimationSetOverride> AnimationSetOverrides;
	Array<FixedString> AnimationWaterfall;
	RefMap<FixedString, Bone> Bones;
	[[bg3::hidden]] void* field_c8; // DynamicArray VMT
	Array<FixedString> VertexColorMaskSlots;
	[[bg3::hidden]] __int64 field_e0; // DynamicArray extra data
	FixedString SkeletonResource;
	FixedString ClothColliderResourceID;
	FixedString Template;
	FixedString SkeletonSlot;
	FixedString SoftbodyResourceID;
	FixedString HairPresetResourceId;
	FixedString ScalpMaterialId;
	FixedString Slot;
	FixedString RemapperSlotId;
	FixedString AttachBone;
	FixedString BlueprintInstanceResourceID;
	[[bg3::hidden]] __int32 field_114;

	MultiHashMap<FixedString, Array<ClothProxy>> ClothProxyMapping;

	glm::fvec3 BoundsMin;
	glm::fvec3 BoundsMax;
	bool NeedsSkeletonRemap;
	bool SupportsVertexColorMask;
	[[bg3::hidden]] __int16 field_172;
	[[bg3::hidden]] __int32 field_174;
};

struct VisualSetResource : public resource::ResourceWithExtraData
{
};

struct VoiceBarkResource : public resource::ResourceWithExtraData
{
};

END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(resource::AnimationResource::Event::PropertiesHolder);
LUA_POLYMORPHIC(resource::EffectResource::EffectComponent::Property);
LUA_POLYMORPHIC(resource::EffectResource::EffectComponent::FramesProperty::VirtualFrames);
LUA_POLYMORPHIC(resource::PhysicsResource::ObjectTemplate::PhysicsObject);

END_NS()
