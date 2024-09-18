#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Animation.h>
#include <GameDefinitions/AllSpark.h>
#include <Lua/LuaHelpers.h>
#include <d3d11.h>
#include <synchapi.h>
#include <variant>

BEGIN_SE()

struct TextureLayerConfigId
{
	uint32_t TextureLayerConfig;
	uint8_t Flag;

	inline bool operator == (TextureLayerConfigId const& o) const
	{
		return o.TextureLayerConfig == TextureLayerConfig
			&& o.Flag == Flag;
	}
};

inline constexpr uint64_t Hash(TextureLayerConfigId const& v)
{
	return v.Flag ^ (v.TextureLayerConfig * 397);
}

struct [[bg3::hidden]] VirtualTextureManagerBase : public ProtectedGameObject<VirtualTextureManagerBase>
{
	struct [[bg3::hidden]] GTSInfo
	{
		int32_t GTSHandle;
		int32_t RefCount;
	};

	struct [[bg3::hidden]] LayerConfig
	{
		uint64_t A;
		uint64_t B;
		uint64_t C;
	};

	virtual ~VirtualTextureManagerBase() = 0;
	virtual void VMT_08() = 0;
	virtual void VMT_10() = 0;
	virtual void VMT_18() = 0;
	virtual void VMT_20() = 0;
	virtual void VMT_28() = 0;
	virtual void VMT_30() = 0;
	virtual bool InitGraphine() = 0;
	virtual void* LoadTexture(int32_t gtsHandle, FixedString const& gtexFileName, uint8_t r9_0) = 0;
	virtual bool UnloadTexture(int32_t gtsHandle, void* graphineTexture) = 0;
	virtual void VMT_50() = 0;
	virtual void VMT_58() = 0;
	virtual void VMT_60() = 0;
	virtual void VMT_68() = 0;
	virtual void VMT_70() = 0;
	virtual void VMT_78() = 0;
	virtual void VMT_80() = 0;
	virtual void VMT_88() = 0;
	virtual void VMT_90() = 0;
	virtual void VMT_98() = 0;
	virtual void VMT_A0() = 0;
	virtual void VMT_A8() = 0;
	virtual void VMT_B0() = 0;
	virtual int32_t LoadGTS(uint32_t textureLayerConfig, STDString const& path, int textureSetId, Array<LayerConfig>& layerConfigs) = 0;
	virtual void ReleaseGTS(uint32_t textureLayerConfig, int32_t gtsHandle, bool releaseTexSet0, bool releaseTexSet1) = 0;

	bool UseChunkedVirtualTextures;
	SRWLOCK GTSLock;
	LegacyRefMap<STDString, GTSInfo> LoadedGTS;
	HashMap<TextureLayerConfigId, Array<STDString>> field_28;
	HashMap<Guid, STDString> TileSets;
};

struct [[bg3::hidden]] VirtualTextureSet
{
	BitSet<> Bits;
	Array<void*> field_10;
};

struct [[bg3::hidden]] VirtualTextureManager : public VirtualTextureManagerBase
{
	struct [[bg3::hidden]] SomeVal
	{
		__int64 field_0;
		int field_8;
	};

	void* Renderer_M;
	void* GraphineContext;
	void* GraphineDevice;
	void* GraphineTranscoder;
	VirtualTextureSet TexSet0;
	VirtualTextureSet TexSet1;
	SRWLOCK Lock2;
	uint64_t field_D0[3];
	SomeVal field_E8[3];
	int field_118;
	SRWLOCK Lock3;
	Array<void*> SomeArrays1[17];
	Array<void*> SomeArrays2[17];
	VirtualTextureSet field_348;
	__int64 TexSet2;
	__int64 field_370;
	Array<void*> field_378;
	int field_388;
	int field_38C;
	int field_390;
	int field_394;
	int field_398;
	int field_39C;
	int field_3A0;
	int field_3A4;
	__int64 VMT2;
	__int64 field_3B0;
	VirtualTextureManagerBase* ThisPtr;
	CRITICAL_SECTION CriticalSection;
	__int64 field_3E8;
	int field_3F0;
	int field_3F4;
	int field_3F8;
	char field_3FC;
	__declspec(align(4)) BYTE field_400;
	char field_401;
	char field_402;
	char field_403;
	int field_404;
	int field_408;
	int field_40C;
	int field_410;
	char field_414;
};

struct [[bg3::hidden]] ViewDescriptor
{
	uint16_t Format;
	uint8_t BaseMip;
	uint8_t MipCount;
	uint16_t BaseSlice;
	uint16_t SliceCount;
	uint16_t unk;
};

struct [[bg3::hidden]] ImageView
{
	uint64_t unk;
	uint64_t View;
};

struct [[bg3::hidden]] Image
{
	Array<ViewDescriptor> ViewDescriptors;
	uint32_t Width;
	uint32_t Height;
	uint32_t Depth;
	uint16_t unk;
	uint16_t MipCount;
	uint16_t ArraySize;
	uint16_t Usage;
	uint8_t Dimension;
	uint8_t Format;
	uint8_t SampleCount;
	uint8_t Access;
	uint8_t NamedViews;
};

struct [[bg3::hidden]] ResourceView
{
	uint64_t unk;
	ID3D11ShaderResourceView* View;
};

struct [[bg3::hidden]] TextureDescriptorDX11
{
	ID3D11Resource* Resource;
	Array<void *> unkArr;
	uint32_t Width;
	uint32_t Height;
	uint32_t Depth;
	uint8_t unk[16];
	Array<ResourceView> Views;
};

struct [[bg3::hidden]] TextureDescriptorVulkan
{
	Image ImageData;
	Array<ImageView*> Views;
};

union [[bg3::hidden]] TextureDescriptor
{
	~TextureDescriptor() {}

	TextureDescriptorDX11 DX11;
	TextureDescriptorVulkan Vulkan;
};

struct [[bg3::hidden]] TextureData
{
	TextureDescriptor* Descriptor;
    uint32_t LoadRequestCount;
    uint32_t RefCount;
    uint8_t State[8];
	STDString Path;
    uint64_t EventObject;
    CRITICAL_SECTION CriticalSection;
    Array<void*> field_60;
    uint8_t field_70;
    uint8_t Flags[3];
    uint32_t field_74;
    uint64_t field_78;
};

struct [[bg3::hidden]] TextureManager
{
    using UnloadTextureProc = bool (TextureManager* self, FixedString const& textureGuid);

    SRWLOCK Lock;
    HashMap<TextureDescriptor*, FixedString> Names;
    HashMap<FixedString, TextureData*> Textures;
};

struct [[bg3::hidden]] UVValues
{
	glm::vec2 UV0;
	glm::vec2 UV1;
};

struct [[bg3::hidden]] TextureAtlas
{
    void* VMT;
    LegacyMap<FixedString, UVValues*> Icons;
    STDString Path;
    STDString TexturePath;
    uint32_t IconWidth;
    uint32_t IconHeight;
    uint32_t TextureWidth;
    uint32_t TextureHeight;
    FixedString TextureUuid;
	resource::TextureResource* Texture;
};

struct [[bg3::hidden]] TextureAtlasMap
{
    void* VMT;
    LegacyRefMap<STDString, TextureAtlas*> AtlasMap;
    LegacyMap<FixedString, TextureAtlas*> IconMap;
	uint32_t Unknown;
};

struct [[bg3::hidden]] Bank : public ProtectedGameObject<Bank>
{
	void* VMT;
	LegacyMap<FixedString, resource::Resource*> Resources;
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
	LegacyMap<FixedString, ResourcePackage*> Packages;
	void* LoadHelper;
	FixedString ModName;
};

struct [[bg3::hidden]] ResourcePackage
{
	void* VMT;
	ResourceBank* Bank;
	FixedString PackageName;
	STDString Path;
	LegacyRefMap<FixedString, void*> field_38;
};

struct [[bg3::hidden]] ResourceManager
{
	__int64 field_0;
	LegacyMap<FixedString, resource::Resource*> Resources;
	std::array<ResourceBank*, 2> ResourceBanks;
	void* MeshProxyFactory;
	void* VisualFactory;
	Array<FixedString> AnimationPreloadList;
	__int64 EffectManager;
	__int64 EffectFactory;
	__int64 field_60; // Unknown pointer; size 0x50
	__int64 field_68; // Unknown pointer; size 0x50
	__int64 field_70; // Unknown pointer; size 0x50
	TextureManager* TextureManager;
	__int64 field_80; // Unknown pointer; size 0x23e0
	WwiseManager* SoundManager;
	__int64 VideoManager;
	__int64 VideoManager2;
	__int64 GameAnalytics;
	VirtualTextureManager* VirtualTextureManager;
	CRITICAL_SECTION CriticalSection;
	LegacyRefMap<STDString, void*> ResourceDependencies;
	LegacyMap<FixedString, Path*> Sources;
	Array<void*> VisualLoaders;
	LegacyMap<FixedString, void*> GenomeAnimationManagers;
	void* BlueprintManager;
	ui::UIManager* UIManager;
	ui::UIManager* UIManagerSwap;
};

END_SE()

BEGIN_NS(resource)

struct Resource : public ProtectedGameObject<Resource>
{
	virtual ~Resource() = 0;
	virtual void* GetLogInfo(void*) = 0;
	virtual STDString* ToLogString(STDString&) = 0;
	virtual void SetUUID(FixedString const&) = 0;
	virtual void VMT18(FixedString const&) = 0;
	virtual void Destroy(ResourceManager* mgr) = 0;
	virtual bool Load(ResourceManager* mgr) = 0;
	virtual bool Release(ResourceManager* mgr) = 0;
	virtual bool IsLoaded() = 0;
	virtual bool IsLoadFailed() = 0;
	virtual uint32_t GetType() = 0;
	virtual Resource* Clone() = 0;
	virtual bool Visit(ObjectVisitor& visitor) = 0;
	virtual bool DoLoad(ResourceManager* mgr) = 0;
	virtual bool DoUnload(ResourceManager* mgr) = 0;

	Path SourceFile;
	[[bg3::readonly]] FixedString Guid;
	[[bg3::readonly]] bool IsModded;
};

struct TwoStepLoadableResource : public Resource
{
	[[bg3::hidden]] void* TwoStepLoadableResource_VMT;
	__int64 field_30;
	[[bg3::hidden]] SRWLOCK lock;
	[[bg3::hidden]] void* LoadJob;
};

struct LoadableResource : public Resource
{
	using LoadProc = bool (LoadableResource* self, ResourceManager* mgr);
	using UnloadProc = bool (LoadableResource* self, ResourceManager* mgr);
	using TranscodeProc = __int64 (void* self, void* transcodeData, void* source, __int64 sourceSize, int width, int height, void* pSaveMip, void* paramBlock, int a9, void* destination, int expectedBCFormat);

	struct [[bg3::hidden]] VMT
	{
		void* Dtor;
		void* GetMetadata;
		void* DebugDump;
		void* SetGuid;
		void* VMT18;
		void* ForceUnload;
		void* IncRef;
		void* DecRef;
		void* IsInState2;
		void* IsInState3;
		void* GetType;
		void* Clone;
		void* Visit;
		LoadProc* Load;
		UnloadProc* Unload;
	};

	virtual bool Load(ResourceManager* mgr) = 0;
	virtual bool Unload(ResourceManager* mgr) = 0;

	[[bg3::readonly]] uint16_t RefCount;
	[[bg3::readonly]] uint8_t State;
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

	HashMap<FixedString, Mapped> MaterialPresets;
};

struct AnimationResource : public TwoStepLoadableResource
{
	struct Event
	{
		FixedString ID;
		float Time;
		float Length;
		uint8_t Track;

		TextKeyTypeProperties* Properties;
	};

	Array<Event*> Events;
	FixedString Template;
	FixedString AdditiveLoopingAnimationID;
	FixedString LeftTransitionAnimation;
	FixedString RightTransitionAnimation;
	// FixedString PreviewVisualResource;
	FixedString SkeletonResource;
	float TimeStep;
	float Duration;
	float Offset;
	bool Looping;
	// bool IsPoseBank;
	uint8_t AnchorHand;
	uint8_t SupportingLeg;
};

struct AnimationBlueprintResource : public TwoStepLoadableResource
{
	[[bg3::hidden]] void* field_48;              // Deleter
	Array<__int64> field_50; // Vtable pointers??? Why???

	[[bg3::hidden]] LegacyRefMap<bg3se::Guid, gn::GenomeVariant> Params;
};

struct AnimationSetResource : public LoadableResource
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

		LegacyRefMap<FixedString, Animations> Animation;
		FixedString FallBackSubSet;
		[[bg3::hidden]] __int32 field_14;
	};

	struct Bank
	{
		LegacyRefMap<FixedString, Subset> AnimationSubSets;
		bg3se::Guid ShortNameSetId;
	};

	Bank* AnimationBank;
};

struct AtmosphereResource : public LoadableResource
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
		FixedString GUID;
		uint32_t InheritanceFlags;
		glm::fmat4x4 field_10;
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
		// void* GradingLutTextureData_butagain?; std::array<__int64, 18> field_e0;
		std::array<__int64, 19> field_d8;
		__int32 field_170;
		float WhiteBalanceTemperature;
		float WhiteBalanceTint;
		bool GradingLutEnabled;
		[[bg3::hidden]] __int8 field_17d;
		[[bg3::hidden]] __int16 field_17e;
		FixedString GradingLut;
		__int32 field_184;
		// void* GradingLutTextureData?
		__int64 field_188;
		__int32 field_190;
		bool DOF;
		bool DOFNearOnly;
		bool DOFFarOnly;
		[[bg3::hidden]] __int8 field_197;
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
		[[bg3::hidden]] __int8 field_229;
		[[bg3::hidden]] __int16 field_22a;
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
		[[bg3::hidden]] __int8 field_25f;
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
		[[bg3::hidden]] __int8 field_291;
		[[bg3::hidden]] __int16 field_292;
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
		[[bg3::hidden]] __int16 field_2b6;
		glm::fvec3 LocalLightSourceColor;
		float LocalLightSourceIntensity;
		bg3se::Guid TimelineAutomaticLightingDefaultSetup;
		bool TimelineAutomaticLightingDisableFlip;
		[[bg3::hidden]] __int8 field_2d9;
		[[bg3::hidden]] __int16 field_2da;
		[[bg3::hidden]] __int32 field_2dc;
		bg3se::Guid CharacterLightSetup;
	};
	AtmosphereData* Atmosphere;
	Array<FixedString> Labels;
};

struct BlendSpaceResource : public TwoStepLoadableResource
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

	struct Adjustment
	{
		glm::fvec3 Adjustment;
		std::array<uint16_t, 3> indexes;
		uint16_t index;
	};

	struct FieldB8Entry
	{
		glm::fvec2 field_0;
		glm::fvec2 field_8;
		glm::fvec2 field_10;
		glm::fvec2 field_18;
		__int32 field_20;
		__int32 field_24;
		__int32 field_28;
		glm::fvec2 field_2c;
		__int32 field_34;
		__int32 field_38;
		__int32 field_3c;
		glm::fvec2 field_40;
		__int32 field_48;
		__int32 field_4c;
		__int32 field_50;
		glm::fvec2 field_54;
		__int32 field_5c;
	};

	Array<Adjustment> Adjustments;

	Array<InputInfo*> Inputs;
	bool NeedsAdjustmentCalculation;
	[[bg3::hidden]] __int8 field_b1;
	[[bg3::hidden]] __int16 field_b2;
	[[bg3::hidden]] __int32 field_b4;
	Array<FieldB8Entry> field_b8;
};

struct VisualSet
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
	Array<Slot> Slots;
	Array<LocatorAttachment> LocatorAttachments;
	PresetData MaterialOverrides;
	LegacyMap<FixedString, PresetData> Materials;
	HashMap<FixedString, FixedString> RealMaterialOverrides;
	HashMap<FixedString, FixedString> field_158;
	FixedString ID;
	bool ShowEquipmentVisuals;
};

struct CharacterVisualResource : public LoadableResource
{
	VisualSet VisualSet;
	FixedString BaseVisual;
};

struct ClothColliderResource : public LoadableResource
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

struct ColorListResource : public LoadableResource
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

struct DialogResource : public LoadableResource
{
	Array<bg3se::Guid> ChildResources;
	Array<uint8_t> SpeakerSlotsWithLines;
	STDString Name;
	uint8_t TimelineSplitScreenPriority;
	uint8_t Flags;
	uint8_t Flags2;
};

struct DiffusionProfileResource : public LoadableResource
{
	glm::fvec3 ScatterColor;
	float ScatterDistance;
	glm::fvec3 TransmissionTint;
	float ThicknessRemapMin;
	float ThicknessRemapMax;
	uint8_t ThickObjectTransmission; // Sometimes filled with junk values, unknown why
	float DualSpecularRoughnessA;
	float DualSpecularRoughnessB;
	float DualSpecularMix;
};

struct EffectResource : public TwoStepLoadableResource
{
	struct TimelineContentEffects
	{
		struct Phase
		{
			struct QuestionHoldAutomationSettings
			{
				[[bg3::hidden]] void* QuestionHoldAutomationVMT;
				bool IsEnabled;
				[[bg3::hidden]] __int8 field_9;
				[[bg3::hidden]] __int16 field_a;
				float CycleSpeed;
				float CycleSpeedDeviation;
				float StartOffset;
				float StartOffsetDeviation;
				[[bg3::hidden]] __int32 field_1c;
			};
			[[bg3::hidden]] void* VMT;
			float Duration;
			float StartTime;
			int32_t PlayCount;
			__int32 field_14;
			bg3se::Guid DialogNodeId;
			bool IsOverridingTimelineQuestionHoldAutomationSettings;
			[[bg3::hidden]] __int8 field_31;
			[[bg3::hidden]] __int16 field_32;
			[[bg3::hidden]] __int32 field_34;
			QuestionHoldAutomationSettings QuestionHoldAutomation;
		};

		struct Input
		{
			virtual ~Input() = 0;
			virtual int GetType() = 0;
			FixedString Name;
			[[bg3::hidden]] __int32 field_c;
			aspk::Property* Input;
		};

		float Duration;
		[[bg3::hidden]] __int32 field_4;
		[[bg3::hidden]] __int64 field_8; // Dynamic array VMT
		Array<Phase> Phases;
		[[bg3::hidden]] __int64 field_20; // Dynamic array extra data
		Array<Input*> Inputs;
		Array<aspk::Component*> EffectComponents;
		bool filled;
		[[bg3::hidden]] __int8 field_49;
		[[bg3::hidden]] __int16 field_4a;
		FixedString EffectName;
	};

	FixedString EffectName;
	[[bg3::hidden]] __int32 field_4c;
	Array<aspk::Component*> EffectComponents;
	Array<FixedString> Dependencies;
	TimelineContentEffects* TimelineContentEffect;
	// TImelineContent?
	__int64 TimelineContent;
	glm::fvec3 BoundsMin;
	glm::fvec3 BoundsMax;
	float CullingDistance;
	float Duration;
	bool Looping;
	bool UseSoundOcclusion;
	[[bg3::hidden]] __int16 field_a2;
	uint32_t InterruptionMode;
};

struct FCurveResource : public TwoStepLoadableResource
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

struct IKRigResource : public LoadableResource
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
		FixedString field_0;
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

	LegacyRefMap<FixedString, BoneProperty> BoneProperties;

	[[bg3::hidden]] void* field_40; // DynamicArray VMT
	Array<IKTask> IKTasks;
	[[bg3::hidden]] __int64 field_58; // DynamicArray extra data

	bool SolverRoot;
	[[bg3::hidden]] __int8 field_61;
	[[bg3::hidden]] __int16 field_62;
	[[bg3::hidden]] __int32 field_64;
	HashMap<bg3se::Guid, FixedString> BoneCategories;
};

struct LightCookieResource : public TwoStepLoadableResource
{
	BitArray<uint32_t, 0x80> Data;
};

struct LightingResource : public LoadableResource
{
	struct Details
	{
		struct MoonDetails
		{
			bool Enabled;
			bool CastLightEnabled;
			[[bg3::hidden]] __int16 field_2;
			float Yaw;
			float Pitch;
			glm::fvec3 RotationAsVec3;
			float Distance;
			float Radius;
			glm::fvec3 Color;
			glm::fvec3 ColorAdjustedForIntensity;
			bool UseTemperature;
			[[bg3::hidden]] __int8 field_39;
			[[bg3::hidden]] __int16 field_3a;
			float Kelvin;
			glm::fvec3 ColorTemperatureAdjustment;
			float Intensity;
			float Earthshine;
			float MoonGlare;
			float TearsScale;
			float TearsRotate;
			FixedString AlbedoTexResourceGUID;
			FixedString NormalTexResourceGUID;
			FixedString TearsAlbedoTexResourceGUID;
			FixedString TearsNormalTexResourceGUID;
		};

		struct DetailsCoverageSettings
		{
			float StartHeight;
			float EndHeight;
			float HorizonDistance;
			FixedString TexResourceGUID;
			glm::fvec2 Offset;
		};

		struct DetailsVolumetricCloudSettings
		{
			bool Enabled;
			[[bg3::hidden]] __int8 field_1;
			[[bg3::hidden]] __int16 field_2;
			float Intensity;
			float AmbientLightFactor;
			float SunLightFactor;
			float ShadowFactor;
			glm::fvec3 BaseColor;
			glm::fvec3 TopColor;
			float BottomFade;
			float HorizonFade;
			float SampleScalar;
			float SampleThreshold;
			float DetailScale;
			float ErosionEdgeSize;
			float Distortion;
			float DistortionScale;
			float Density;
			float ForwardScatteringG;
			float BackwardScatteringG;
			float DarkOutline;
			float SunRayLength;
			float ConeRadius;
			float LODDistance;
			DetailsCoverageSettings CoverageSettings;
			float CoverageStartDistance;
			float CoverageEndDistance;
			float CoverageWindSpeed;
			float RainCoverageMaxInfluence;
		};

		struct SkyLightDetails
		{
			glm::fvec3 Color;
			float Intensity;
			float Kelvin;
			bool UseTemperature;
			glm::fvec3 ColorTemperatureAdjustment;
			glm::fvec3 ColorAdjustedForIntensity;
			[[bg3::hidden]] __int8 field_2d;
			[[bg3::hidden]] __int16 field_2e;
			FixedString SkydomeTex;
			bool SkydomeEnabled;
			bool RotateSkydomeEnabled;
			[[bg3::hidden]] __int16 field_36;
			int32_t PhysicalModel;
			float ScatteringIntensity;
			float ScatteringSunIntensity;
			glm::fvec3 ScatteringSunColor;
			bool ScatteringEnabled;
			[[bg3::hidden]] __int8 field_51;
			[[bg3::hidden]] __int16 field_52;
			float ProcStarsIntensity;
			float ProcStarsAmount;
			float ProcStarsShimmer;
			glm::fvec2 ProcStarsSaturation;
			bool ProcStarsEnabled;
			[[bg3::hidden]] __int8 field_69;
			[[bg3::hidden]] __int16 field_6a;
			float CirrusCloudsIntensity;
			float CirrusCloudsAmount;
			glm::fvec3 CirrusCloudsColor;
			bool CirrusCloudsEnabled_FixedString;
			[[bg3::hidden]] __int8 field_81;
			[[bg3::hidden]] __int16 field_82;
		};

		struct DetailsSSAOSettings
		{
			bool Enabled;
			[[bg3::hidden]] __int8 field_1;
			[[bg3::hidden]] __int16 field_2;
			float Radius;
			float Bias;
			float Intensity;
			float DirectLightInfluence;
		};

		struct FogLayer
		{
			glm::fvec3 Albedo;
			float Density0;
			float Density1;
			float Height0;
			float Height1;
			float NoiseCoverage;
			glm::fvec3 NoiseFrequency;
			glm::fvec3 NoiseRotation;
			glm::fvec3 NoiseWind;
			bool Enabled;
			[[bg3::hidden]] __int8 field_45;
			[[bg3::hidden]] __int16 field_46;
		};

		struct FogSettings
		{
			float Phase;
			float RenderDistance;
			FogLayer FogLayer0;
			FogLayer FogLayer1;
		};

		struct SunDetails
		{
			glm::fvec3 SunColor;
			float Kelvin;
			float SunIntensity;
			bool UseTemperature;
			[[bg3::hidden]] __int8 field_15;
			[[bg3::hidden]] __int16 field_16;
			glm::fvec3 ColorTemperatureAdjustment;
			glm::fvec3 ColorAdjustedForIntensity;
			glm::fvec3 RotationAsVec3;
			float Yaw;
			float Pitch;
			bool ShadowEnabled;
			[[bg3::hidden]] __int8 field_45;
			[[bg3::hidden]] __int16 field_46;
			float ShadowFarPlane;
			float ShadowNearPlane;
			float ShadowFade;
			float ShadowBias;
			float LightSize;
			float LightDistance;
			float CascadeSpeed;
			uint8_t CascadeCount;
			uint8_t ShadowObscurity;
			uint8_t SunlightObscurity;
			[[bg3::hidden]] __int8 field_67;
			DetailsCoverageSettings CoverageSettings;
			float LocalCoverageScalar;
			bool LocalCoverageEnabled;
			[[bg3::hidden]] __int8 field_85;
			[[bg3::hidden]] __int16 field_86;
			float ScatteringIntensityScale;
		};

		[[bg3::hidden]] void* VMT;
		FixedString ParentGUID;
		SunDetails Sun;
		MoonDetails Moon;
		SkyLightDetails SkyLight;
		DetailsSSAOSettings SSAOSettings;
		DetailsVolumetricCloudSettings VolumetricCloudSettings;
		FogSettings Fog;
		FogSettings TimelineFog;
		bool TimelineFogOverride;
		[[bg3::hidden]] __int8 field_361;
		[[bg3::hidden]] __int16 field_362;
		[[bg3::hidden]] __int32 field_364;
		__int64 field_368;
		__int64 field_370;
	};

	Details* Lighting;
};

struct MaterialResource : public TwoStepLoadableResource
{
	struct Parameter
	{
		FixedString ParameterName;
		bool Enabled;
	};
	
	struct ScalarParameter : public Parameter
	{
		float Value;
		float BaseValue;
	};

	struct Vector2Parameter : public Parameter
	{
		glm::fvec2 Value;
		glm::fvec2 BaseValue;
	};

	struct Vector3Parameter : public Parameter
	{
		glm::fvec3 Value;
		glm::fvec3 BaseValue;
		bool IsColor;
	};

	// alignas(16)
	struct Vector4Parameter : public Parameter
	{
		[[bg3::hidden]] __int64 field_8; // Padding
		glm::fvec4 Value;
		glm::fvec4 BaseValue;
		bool IsColor;
		[[bg3::hidden]] __int64 field_38; // Padding
	};

	struct Texture2DParameter : public Parameter
	{
		FixedString ID;
	};

	struct VirtualTextureParameter : public Parameter
	{
		int32_t Index;
		FixedString ID;
	};

	Material* Instance;
	Array<ScalarParameter> ScalarParameters;
	Array<Vector2Parameter> Vector2Parameters;
	Array<Vector3Parameter> Vector3Parameters;
	Array<Vector4Parameter> VectorParameters;
	Array<Texture2DParameter> Texture2DParameters;
	Array<VirtualTextureParameter> VirtualTextureParameters;
	FixedString DiffusionProfileUUID;
	uint8_t MaterialType;
	uint8_t RenderChannel;
};

struct MaterialPresetResource : public LoadableResource
{
	PresetData Presets;
};

// I think this is unimplemented and std::terminates on construction
struct MaterialSetResource : public TwoStepLoadableResource
{
};

struct MeshProxyResource : public TwoStepLoadableResource
{
	__int64 field_48; // Size 0x30
	FixedString Template;
	[[bg3::hidden]] __int32 field_54;
};

struct PhysicsResource : public LoadableResource
{
	struct ObjectTemplate
	{
		struct PhysicsObject
		{
			virtual ~PhysicsObject() = 0;
			virtual const FixedString& GetType() = 0;
		};

		struct PhysicsBox : public PhysicsObject
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

		struct PhysicsCapsule : public PhysicsObject
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
		Array<PhysicsObject*> PhysicsObjects;
	};

	Array<ObjectTemplate*> ObjectTemplates;
	Array<FixedString> IgnoreColliderList;
	FixedString Template;
	bool field_54;
	[[bg3::hidden]] __int8 field_55;
	[[bg3::hidden]] __int16 field_56;
};

struct ScriptResource : public LoadableResource
{
	struct Parameter
	{
		int32_t Type;
		[[bg3::hidden]] __int32 field_4;
		STDString Value;
	};

	LegacyMap<FixedString, Parameter*> Parameters;
};

struct SkeletonResource : public TwoStepLoadableResource
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

	LegacyRefMap<FixedString, Bone> Bones;
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

struct SkeletonMirrorTableResource : public TwoStepLoadableResource
{
	struct Entry
	{
		FixedString LeftBone;
		FixedString RightBone;
	};

	Array<Entry> Entries;
};

struct SkinPresetResource : public LoadableResource
{
	PresetData Presets;
};

struct SoundResource : public TwoStepLoadableResource
{
	FixedString SoundEvent;
	uint32_t SoundEventID;
	float Duration;
	float MaxDistance;
	uint8_t SoundCodec;
	uint8_t Flags;
};

struct TerrainBrushResource : public LoadableResource
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

struct TextureResource : public LoadableResource
{
	[[bg3::hidden]] void* field_30;
	FixedString Template;
	bool Streaming;
	bool SRGB;
	uint8_t Type;
	int32_t Width;
	int32_t Height;
	int32_t Depth;
};

struct TileSetResource : public LoadableResource
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
		LegacyRefMap<bg3se::Guid, Tile> Tiles;
		bool IsRoof;
		[[bg3::hidden]] __int8 field_11;
		[[bg3::hidden]] __int16 field_12;
		[[bg3::hidden]] __int32 field_14;
	};

	ExtendedRefMap* TileSet;
};

struct TimelineResource : public LoadableResource
{
	bg3se::Guid DialogResourceId;
	bool FadeOutOnEnd;
	[[bg3::hidden]] __int8 field_41;
	[[bg3::hidden]] __int16 field_42;
	float FadeOutDuration;
	int32_t BodyPartMocap;
	[[bg3::hidden]] __int32 field_4c;
};

struct TimelineSceneResource : public LoadableResource
{
	Array<FixedString> Labels;
	uint8_t SceneType;
	[[bg3::hidden]] __int8 field_41;
	[[bg3::hidden]] __int16 field_42;
	int32_t AdditionalSpeakerCount;
};

struct VirtualTextureResource : public LoadableResource
{
	[[bg3::readonly]] uint8_t field_80;
	[[bg3::readonly]] uint32_t VirtualTextureLayerConfig;
	[[bg3::readonly]] uint32_t LoadedVirtualTextureLayerConfig;
	STDString RootPath;
	[[bg3::hidden]] SRWLOCK Lock;
	FixedString TileSetFileName;
	FixedString GTexFileName;
	[[bg3::hidden]] void* GraphineTextureData;
	bool Prefetch;
	int8_t PrefetchMipLevel;
	[[bg3::readonly]] uint8_t field_52;
	uint32_t ReferencedColorSpaces;
};

struct VisualResource : public TwoStepLoadableResource
{
	struct Object
	{
		FixedString ObjectID;
		FixedString MaterialID;
		uint8_t LOD;
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

	struct ClothHighResInfo
	{
		FixedString Name;
		[[bg3::hidden]] Array<uint16_t> ClosestVertices;
		[[bg3::hidden]] void* CompressedVertices;
		[[bg3::hidden]] uint32_t CompressedSize;
		[[bg3::hidden]] uint32_t NbClosestVertices;
	};

	struct ClothData
	{
		HashMap<FixedString, Array<ClothHighResInfo>> Mappings;
		Array<ClothParam> Params;
		FixedString ClothColliderResourceID;
	};

	[[bg3::hidden]] void* field_48; // DynamicArray VMT
	Array<FixedString> Tags;
	[[bg3::hidden]] __int64 field_60; // DynamicArray extra data
	Array<Object> Objects;
	Array<Attachment> Attachments;
	ClothData* Cloth;
	Array<AnimationSetOverride> AnimationSetOverrides;
	Array<FixedString> AnimationWaterfall;
	LegacyRefMap<FixedString, Bone> Bones;
	[[bg3::hidden]] void* field_c8; // DynamicArray VMT
	Array<FixedString> VertexColorMaskSlots;
	[[bg3::hidden]] __int64 field_e0; // DynamicArray extra data
	FixedString SkeletonResource;
	FixedString Template;
	FixedString SkeletonSlot;
	FixedString SoftbodyResourceID;
	FixedString HairPresetResourceId;
	FixedString ScalpMaterialId;
	FixedString Slot;
	FixedString RemapperSlotId;
	FixedString AttachBone;
	FixedString BlueprintInstanceResourceID;

	glm::fvec3 BoundsMin;
	glm::fvec3 BoundsMax;
	bool NeedsSkeletonRemap;
	bool SupportsVertexColorMask;
	uint8_t MaterialType;
	uint8_t HairType;
};

struct VisualSetResource : public LoadableResource
{
};

struct VoiceBarkResource : public LoadableResource
{
};

END_NS()

BEGIN_NS(lua)

LUA_POLYMORPHIC(resource::PhysicsResource::ObjectTemplate::PhysicsObject);

END_NS()
