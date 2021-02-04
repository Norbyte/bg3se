#pragma once

#include "BaseTypes.h"

namespace bg3se
{
	struct Version
	{
		inline Version() : Ver(0) {}
		inline Version(uint32_t ver) : Ver(ver) {}
		inline Version(uint32_t minor, uint32_t major, uint32_t revision, uint32_t build)
			: Ver(((major & 0xf) << 28) + ((minor & 0xf) << 24) + ((revision & 0xff) << 16) + (build & 0xffff))
		{}

		inline uint32_t Major() const
		{
			return Ver >> 28;
		}

		inline uint32_t Minor() const
		{
			return (Ver >> 24) & 0xf;
		}

		inline uint32_t Revision() const
		{
			return (Ver >> 16) & 0xff;
		}

		inline uint32_t Build() const
		{
			return Ver & 0xffff;
		}

		uint32_t Ver;
	};


	struct ModuleShortDesc
	{
		FixedString ModuleUUID;
		STDWString Name;
		Version ModVersion;
		Version PublishVersion;
		STDString MD5;
		STDWString Folder;
	};

	struct ModuleSettings : public ProtectedGameObject<ModuleSettings>
	{
		using ValidateProc = int(ModuleSettings& self, ModuleSettings& host, ObjectSet<ModuleShortDesc>& mismatches);

		void *VMT;
		ObjectSet<ModuleShortDesc> Mods;
		ObjectSet<FixedString> ModOrder;
	};

	struct ModuleInfo
	{
		FixedString ModuleUUIDString;
		GUID ModuleUUID;
		STDWString Name;
		FixedString StartLevelName;
		FixedString MenuLevelName;
		FixedString LobbyLevelName;
		FixedString CharacterCreationLevelName;
		FixedString PhotoBoothLevelName;
		FixedString MainMenuBackgroundVideo;
		Version ModVersion{ 0 };
		Version PublishVersion{ 0 };
		STDString Hash;
		STDWString Directory;
		ObjectSet<void*> Scripts; // ScriptDataList*
		STDWString Author;
		STDWString Description;
		ObjectSet<STDWString, GameMemoryAllocator, true> Tags;
		uint8_t NumPlayers{ 4 };
		FixedString GMTemplate;
		ObjectSet<FixedString, GameMemoryAllocator, true> TargetModes;
		FixedString ModuleType;
	};

	struct Module
	{
		using HashProc = bool (Module* self);

		void* VMT{ nullptr };
		ModuleInfo Info;
		ObjectSet<Module, GameMemoryAllocator, true> LoadOrderedModules;
		ObjectSet<Module, GameMemoryAllocator, true> ContainedModules;
		ObjectSet<Module, GameMemoryAllocator, true> DependentModules;
		ObjectSet<Module, GameMemoryAllocator, true> AddonModules;
		bool HasValidHash{ true };
		bool UsesLsfFormat{ false };
		bool FinishedLoading{ false };
		uint64_t DataHashList[3]{ 0 };
		bool BFSReset_M{ false };
	};

	struct ModManager : public ProtectedGameObject<ModManager>
	{
		typedef void (*CollectAvailableMods)(ObjectSet<Module, GameMemoryAllocator, true>& mods);

		void * VMT;
		uint64_t Unknown[4];
		Module BaseModule;
		uint64_t Unknown2;
		ObjectSet<Module, GameMemoryAllocator, true> AvailableMods;
		ModuleSettings Settings;

		Module const * FindModByNameGuid(char const * nameGuid) const;
	};
}
