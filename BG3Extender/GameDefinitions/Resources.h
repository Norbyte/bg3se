#pragma once

#include <GameDefinitions/BaseTypes.h>
#include <GameDefinitions/Enumerations.h>
#include <GameDefinitions/EntitySystem.h>
#include <synchapi.h>

namespace bg3se
{
	struct Resource : public ProtectedGameObject<Resource>, public HasObjectProxy
	{
		void* VMT;
		Path SourceFile;
		FixedString PackageName;
		FixedString ModName;
		FixedString UUID;
		VirtualArray<FixedString> Tags;
		STDString Name;
		bool IsLocalized;
		bool IsActive;
		bool IsDirty;
		bool IsOriginal;
	};

	struct Bank : public ProtectedGameObject<Bank>
	{
		void* VMT;
		Map<FixedString, Resource*> Resources;
		SRWLOCK SRWLock;
		ResourceBankType BankTypeId;
	};

	struct ResourceContainer : public ProtectedGameObject<ResourceContainer>
	{
		void* VMT;
		Bank* Banks[(unsigned)ResourceBankType::Sentinel];
	};

	struct ResourcePackage;

	struct ResourceBank
	{
		void* VMT;
		ResourceContainer Container;
		Map<FixedString, ResourcePackage*> Packages;
		void* LoadHelper;
		FixedString ModName;
	};

	struct ResourcePackage
	{
		void* VMT;
		ResourceBank* Bank;
		FixedString PackageName;
		STDString Path;
		RefMap<FixedString, void*> field_38;
	};

	struct ResourceManager
	{
		void* VMT;
		__int64 field_8;
		Map<FixedString, Resource*> Resources;
		ResourceBank* ResourceBanks[2];
		__int64 field_38;
		__int64 field_40;
		__int64 field_48;
		__int64 field_50;
		__int64 EffectFactory;
		__int64 field_60;
		__int64 field_68;
		__int64 field_70;
		__int64 field_78;
		__int64 field_80;
		__int64 field_88;
		__int64 SoundManager_M;
		__int64 field_98;
		__int64 field_A0;
		__int64 GameAnalytics_M;
		__int64 field_B0;
		CRITICAL_SECTION CriticalSection;
		RefMap<FixedString, void*> Unknown;
		Map<FixedString, Path*> Paths;
		__int64 field_108;
		__int64 field_110;
		__int64 field_118;
		Map<FixedString, void*> GenomeManagers;
		__int64 field_138;
		__int64 UI_M;
		__int64 field_148;
		int field_150;
		char field_154;
	};

}
