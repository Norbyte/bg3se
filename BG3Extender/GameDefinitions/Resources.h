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
		__int64 field_40;
		__int64 field_48;
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
		// [[bg3::hidden]] RefMap<STDString, void*> Unknown; // std::string -> ??? TODO: figure out what this is and how to enable compilation with it
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

	struct VisualResource : public LockedResource
	{
		[[bg3::hidden]] void* DynamicArrayVMT;
		Array<FixedString> field_50;
		__int64 DynamicArray_Unknown;
		[[bg3::hidden]] Array<void*> field_68; // struct {FixedString, FixedString, undefined4}
		[[bg3::hidden]] Array<void*> field_78; // struct {FixedString, FixedString}
		[[bg3::hidden]] Array<void*> field_88; // struct {FixedString, undefined[0xb8]}; alignment 4
		__int64 field_98;
		__int64 field_a0;
		Array<FixedString> field_a8;
		[[bg3::hidden]] RefMap<FixedString, void*> field_b8; // struct {undefined[4], FixedString} -> ???
	};

}
