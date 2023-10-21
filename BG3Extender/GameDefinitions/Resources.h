#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>
#include <synchapi.h>

namespace bg3se
{
	struct Resource : public ProtectedGameObject<Resource>
	{
		[[bg3::hidden]] void* VMT;
		Path SourceFile;
		FixedString Guid;
	};

	struct [[bg3::hidden]] Bank : public ProtectedGameObject<Bank>
	{
		void* VMT;
		Map<FixedString, Resource*> Resources;
		SRWLOCK SRWLock;
		ResourceBankType BankTypeId;
	};

	struct [[bg3::hidden]] ResourceContainer : public ProtectedGameObject<ResourceContainer>
	{
		void* VMT;
		Bank* Banks[(unsigned)ResourceBankType::Sentinel];
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
		[[bg3::hidden]] Map<FixedString, Resource*> Resources;
		[[bg3::hidden]] ResourceBank* ResourceBanks[2];
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
		[[bg3::hidden]] Array<void*> Unknown2; // Contains objects of size 0x20
		[[bg3::hidden]] Map<FixedString, void*> Unknown3; // Points to objects of size 0x110
		__int64 field_130; // Unknown pointer; size 0x180
		__int64 field_138; // Unknown pointer; size 0x40. Initialized to same pointer as field_140. Has virtual destructor.
		__int64 UI_M; // Unknown pointer; size 0x40. Initialized to same pointer as field_138. Has virtual destructor.
	};

	struct LockedResource : public Resource
	{
		[[bg3::hidden]] void* LockedResource_VMT;
		__int64 field_30;
		[[bg3::hidden]] SRWLOCK lock;
		__int64 field_40; // Unknown pointer
	};

	struct ResourceWithExtraData : public Resource
	{
		__int16 field_28;
		__int8 field_2a;
		__int8 possibly_unused_field_2b;
		__int32 possibly_unused_field_2c;
	};

	struct PresetData_Data_Data
	{
		StaticArray<int32_t> field_0;
		Array<int32_t> field_10;
		Array<FixedString> field_20;
	};

	struct PresetData_Data
	{
		struct Element1
		{
			FixedString field_0;
			FixedString field_4;
			__int32 field_8;
		};

		PresetData_Data_Data field_0;
		Array<Element1> field_30;
	};

	struct CharacterVisualPresetData_Data
	{
		PresetData_Data_Data field_0;
		Array<FixedString> field_30;
	};

	struct VisualPresetData_Data
	{
		struct Element1
		{
			FixedString field_0;
			__int32 field_4;
			Array<__int16> field_8;
		};

		PresetData_Data_Data field_0;
		Array<Array<Element1>> field_30;
	};

	struct PresetData
	{
		struct Element1
		{
			FixedString field_0;
			__int32 field_4;
			__int32 field_8;
		};

		struct Element2
		{
			FixedString field_0;
			__int32 field_4;
			__int32 field_8;
			__int32 field_c;
		};

		struct Element3
		{
			FixedString field_0;
			__int32 field_4;
			__int32 field_8;
			__int32 field_c;
			__int32 field_10;
		};

		// alignas(16)
		struct Element4
		{
			FixedString field_0;
			__int32 field_4;
			__int64 field_8;
			__int64 field_10;
			__int64 field_18;
			__int64 field_20;
			__int64 field_28;
		};

		struct Element5
		{
			FixedString field_0;
			FixedString field_4;
			__int32 field_8;
		};

		Array<Element1> field_0;
		Array<Element2> field_10;
		Array<Element3> field_20;
		Array<Element4> field_30;
		Array<Element5> field_40;
		Array<Element5> field_50;
		FixedString field_60;
		__int32 field_64;

		PresetData_Data field_68;
	};

	struct AnimationResource : public LockedResource
	{
		__int64 field_48; // Pointer
		__int64 field_50; // Unknown
		FixedString field_58;
		FixedString field_5c;
		FixedString field_60;
		FixedString field_64;
		FixedString field_68;
		__int32 field_6c;
		__int64 field_70;
		__int64 field_78;
	};

	struct AnimationBlueprintResource : public LockedResource
	{
		__int64 field_48; // subobject vtable?
		__int64 field_50; // pointer
		__int64 field_58;
		__int64 field_60;
		__int64 field_68;
	};

	struct AnimationSetResource : public ResourceWithExtraData
	{
		// RefMap<???, ???>*
		__int64 field_30; // pointer; size 0x20. Possibly a set of FixedString?
	};

	struct AtmosphereResource : public ResourceWithExtraData
	{
		__int64 field_30; // pointer; size 0x2f0. Has virtual destructor
		Array<FixedString> field_38;
	};

	struct BlendSpaceResource : public LockedResource
	{
		STDString field_48;
		STDString field_60;
		__int64 field_78; // Array<???>
		__int64 field_80;

		__int64 field_88; // RefMap<???,???> ?
		__int64 field_90;
		__int64 field_98;

		__int64 field_a0; // Array<???> ?
		__int64 field_a8;
		__int64 field_b0;
		__int64 field_b8;
		__int64 field_c0;
	};

	struct CharacterVisualResource : public ResourceWithExtraData
	{
		struct Element1
		{
			FixedString field_0;
			FixedString field_4;
			FixedString field_8;
		};

		struct Element2
		{
			FixedString field_0;
			FixedString field_4;
		};

		FixedString field_30;
		__int32 field_34;
		Array<Element1> field_38;
		Array<Element2> field_48;
		PresetData field_58;
		Map<FixedString, PresetData> field_100;
		CharacterVisualPresetData_Data field_118;
		CharacterVisualPresetData_Data field_158;
		FixedString field_198;
		__int32 field_19c;
		FixedString field_1a0;
		__int32 field_1a4;
	};

	struct ClothColliderResource : public ResourceWithExtraData
	{
		struct Data
		{
			__int64 field_0;
			__int64 field_8;
			FixedString field_10;
			__int32 field_14;
			Array<FixedString> field_18;
			FixedString field_28;
			__int32 field_2c;
		};

		Array<Data> field_30;
	};

	struct ColorListResource : public ResourceWithExtraData
	{
		struct Element
		{
			__int64 field_0;
			__int64 field_8;
			__int64 field_10;
			__int64 field_18;
		};

		// Arrays of trivial type, it seems; no easy information to get. Both the same type, though
		__int64 field_30; // Dynamic array vtable
		Array<Element> field_38;
		__int64 field_48; // Dynamic array other info

		__int64 field_50; // Dynamic array vtable
		Array<Element> field_58;
		__int64 field_68; // Dynamic array other info

		__int64 field_70;
	};

	struct DialogResource : public ResourceWithExtraData
	{
		struct Data
		{
			__int64 field_0;
			__int64 field_8;
		};

		__int64 field_30;
		__int64 field_38;
		Array<__int8> field_40;
		Array<Data> field_50;
		__int64 field_60;
		STDString field_68;
	};

	struct DiffusionProfileResource : public ResourceWithExtraData
	{
		__int64 field_30;
		__int64 field_38;
		__int64 field_40;
		__int64 field_48;
		__int64 field_50;
		__int64 field_58;
		__int64 field_60;
	};

	struct EffectResource : public LockedResource
	{
		FixedString field_48;
		__int32 field_4c;
		__int64 field_50;
		__int64 field_58;
		StaticArray<FixedString> field_60;
		__int64 field_70; // pointer to structure, size 0x50
		__int64 field_78;
		__int64 field_80;
		__int64 field_88;
		__int64 field_90;
		__int64 field_98;
		__int64 field_a0;
	};

	struct FCurveResource : public LockedResource
	{
		Array<__int64> field_48; // Array of size 0x20 objects
		__int64 field_58;
		__int64 field_60;
		__int64 field_68; // Pointer to array(?) of unknown objects
		__int64 field_70;
	};

	struct IKRigResource : public ResourceWithExtraData
	{
		struct MapValue
		{
			__int64 field_0;
			__int64 field_8;
			__int64 field_10;
			__int64 field_18;
			__int64 field_20;
		};

		struct ArrayStruct
		{
			FixedString field_0;
			FixedString field_4;
			__int32 field_8;
			FixedString field_c;
			FixedString field_10;
			__int32 field_14;
			__int64 field_18;
			__int64 field_20;
			__int64 field_28;
			__int64 field_30;
			__int64 field_38;
		};

		struct ArrayStruct2
		{
			__int64 field_0;
			__int64 field_8;
		};

		RefMap<FixedString, MapValue> field_30;

		[[bg3::hidden]] void* field_40; // DynamicArray VMT
		StaticArray<ArrayStruct> field_48;
		__int64 field_58; // DynamicArray extra data

		__int64 field_60;
		StaticArray<__int32> field_68;
		Array<__int32> field_78;
		Array<ArrayStruct2> field_88;
		Array<FixedString> field_98;
	};

	struct LightCookieResource : public LockedResource
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

	struct LightingResource : public ResourceWithExtraData
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

	struct MaterialResource : public LockedResource
	{
		struct Element1
		{
			FixedString field_0;
			__int32 field_4;
			__int64 field_8;
		};

		struct Element2
		{
			FixedString field_0;
			__int32 field_4;
			__int64 field_8;
			__int64 field_10;
		};

		struct Element3
		{
			FixedString field_0;
			__int32 field_4;
			__int32 field_8;
			__int32 field_c;
			__int32 field_10;
			__int32 field_14;
			__int32 field_18;
			__int32 field_1c;
			__int32 field_20;
		};

		// Guess: contains GUID
		// alignas(16)
		struct Element4
		{
			FixedString field_0;
			__int32 field_4;
			__int64 field_8;
			__int64 field_10;
			__int64 field_18;
			__int64 field_20;
			__int64 field_28;
			__int64 field_30;
			__int64 field_38;
		};

		struct Element5
		{
			FixedString field_0;
			__int32 field_4;
			FixedString field_8;
		};

		struct Element6
		{
			FixedString field_0;
			__int32 field_4;
			__int32 field_8;
			FixedString field_c;
		};

		__int64 field_48;
		StaticArray<Element1> field_50;
		StaticArray<Element2> field_60;
		StaticArray<Element3> field_70;
		StaticArray<Element4> field_80;
		StaticArray<Element5> field_90;
		StaticArray<Element6> field_a0;
		FixedString field_b0;
		__int16 field_b4;
		__int16 field_b6;
	};

	struct MaterialPresetResource : public ResourceWithExtraData
	{
		PresetData field_30;
	};

	// I think this is unimplemented and std::terminates on construction
	struct MaterialSetResource : public LockedResource
	{
	};

	struct MeshProxyResource : public LockedResource
	{
		__int64 field_48;
		FixedString field_50;
		__int32 field_54;
	};

	struct PhysicsResource : public ResourceWithExtraData
	{
		__int64 field_30; // Might be a pointer to something?
		__int64 field_38;
		Array<FixedString> field_40;
		FixedString field_50;
		__int8 field_54;
		__int8 field_55;
		__int16 field_56;
	};

	struct ScriptResource : public ResourceWithExtraData
	{
		// Not sure what mapped-to type is. Pointer of some kind?
		Map<FixedString, __int64> field_30;
	};

	struct SkeletonResource : public LockedResource
	{
		struct Element1
		{
			__int32 field_0;
			FixedString field_4;
			FixedString field_8;
		};

		struct Element2
		{
			FixedString field_0;
			FixedString field_4;
			FixedString field_8;
			__int32 field_c;
			__int32 field_10;
			__int32 field_14;
			__int32 field_18;
			__int32 field_20;
			__int32 field_24;
			__int32 field_28;
		};

		RefMap<FixedString, Array<Element1>> field_48;
		[[bg3::hidden]] void* field_58; // DynamicArray VMT
		StaticArray<Element2> field_60;
		__int64 field_70; // DynamicArray extra data
		FixedString field_78;
		FixedString field_7c;
		FixedString field_80;
		FixedString field_84;
		FixedString field_88;
		FixedString field_8c;
		FixedString field_90;
		FixedString field_94;
		__int64 field_98;
	};

	struct SkeletonMirrorTableResource : public LockedResource
	{
		struct FixedStringPair
		{
			FixedString field_0;
			FixedString field_4;
		};

		Array<FixedStringPair> field_48;
	};

	struct SkinPresetResource : public ResourceWithExtraData
	{
		PresetData field_30;
	};

	struct SoundResource : public LockedResource
	{
		__int32 field_48;
		FixedString field_4c;
		FixedString field_50;
		__int32 field_54;
		__int64 field_58;
		__int64 field_60;
		__int64 field_68;
		__int64 field_70;
	};

	struct TerrainBrushResource : public ResourceWithExtraData
	{
		FixedString field_30;
		FixedString field_34;
		FixedString field_38;
		FixedString field_3c;
		__int64 field_40;
		__int64 field_48;
		__int64 field_50;
		__int64 field_58;
		__int64 field_60;
		__int64 field_68;
		__int64 field_70;
		__int64 field_78;
		__int64 field_80;
		__int64 field_88;
	};

	struct TextureResource : public ResourceWithExtraData
	{
		__int64 field_30;
		FixedString field_38;
		__int32 field_3c;
		__int64 field_40;
		__int64 field_48;
		__int64 field_50;
	};

	struct TileSetResource : public ResourceWithExtraData
	{
		struct Value
		{
			FixedString field_0;
			FixedString field_4;
			FixedString field_c;
			__int32 field_10;
		};

		struct ExtendedRefMap : public ProtectedGameObject<ExtendedRefMap>
		{
			RefMap<bg3se::Guid, Value> field_0;
			__int64 field_10;
		};

		ExtendedRefMap* field_30;
	};

	struct TimelineResource : public ResourceWithExtraData
	{
		__int64 field_30;
		__int64 field_38;
		__int64 field_40;
		__int64 field_48;
	};

	struct TimelineSceneResource : public ResourceWithExtraData
	{
		Array<FixedString> field_30;
		__int64 field_40;
	};

	struct VirtualTextureResource : public ResourceWithExtraData
	{
		__int64 field_30;
		__int64 field_38;
		FixedString field_40;
		__int32 field_44;
		__int64 field_48;
		__int64 field_50;
	};

	struct VisualResource : public LockedResource
	{
		struct Element1
		{
			FixedString field_0;
			FixedString field_4;
			__int32 field_8;
		};

		struct Element2
		{
			FixedString field_0;
			FixedString field_4;
		};

		struct Element3
		{
			FixedString field_0;
			__int32 field_4;
			__int32 field_8;
			__int32 field_c;
			__int32 field_10;
			__int32 field_14;
			__int32 field_18;
			__int32 field_1c;
			__int32 field_20;
			__int32 field_24;
			__int32 field_28;
			__int32 field_2c;
			__int32 field_30;
			__int32 field_34;
			__int32 field_38;
			__int32 field_3c;
			__int32 field_40;
			__int32 field_44;
			__int32 field_48;
			__int32 field_4c;
			__int32 field_50;
			__int32 field_54;
			__int32 field_58;
			__int32 field_5c;
			__int32 field_60;
			__int32 field_64;
			__int32 field_68;
			__int32 field_6c;
			__int32 field_70;
			__int32 field_74;
			__int32 field_78;
			__int32 field_7c;
			__int32 field_80;
			__int32 field_84;
			__int32 field_88;
			__int32 field_8c;
			__int32 field_90;
			__int32 field_94;
			__int32 field_98;
			__int32 field_9c;
			__int32 field_a0;
			__int32 field_a4;
			__int32 field_a8;
			__int32 field_ac;
			__int32 field_b0;
			__int32 field_b4;
			__int32 field_b8;
		};

		struct Element4
		{
			__int64 field_0;
			__int64 field_8;
			FixedString field_10;
			__int32 field_14;
		};

		struct Mapped
		{
			__int64 field_0;
			__int64 field_8;
			__int64 field_10;
		};

		[[bg3::hidden]] void* field_48; // DynamicArray VMT
		StaticArray<FixedString> field_50;
		__int64 field_60; // DynamicArray extra data
		Array<Element1> field_68;
		Array<Element2> field_78;
		Array<Element3> field_88;
		Array<Element4> field_98;
		Array<FixedString> field_a8;
		RefMap<FixedString, Mapped> field_b8;
		[[bg3::hidden]] void* field_c8; // DynamicArray VMT
		StaticArray<FixedString> field_d0;
		__int64 field_e0; // DynamicArray extra data
		FixedString field_e8;
		FixedString field_ec;
		FixedString field_f0;
		FixedString field_f4;
		FixedString field_f8;
		FixedString field_fc;
		FixedString field_100;
		FixedString field_104;
		FixedString field_108;
		FixedString field_10c;
		FixedString field_110;
		__int32 field_114;

		VisualPresetData_Data field_118;

		__int64 field_158;
		__int64 field_160;
		__int64 field_168;
		__int64 field_170;
	};

	struct VisualSetResource : public ResourceWithExtraData
	{
	};

	struct VoiceBarkResource : public ResourceWithExtraData
	{
	};
}
