#pragma once

#include "BaseTypes.h"

namespace bg3se
{
	struct Version
	{
		inline Version() : Ver(0) {}
		inline Version(uint64_t ver) : Ver(ver) {}
		// TODO - figure out how 
		inline Version(uint64_t minor, uint64_t major, uint64_t revision, uint64_t build)
			: Ver(((major & 0x1ff) << 55) + ((minor & 0x1ff) << 46) + ((revision & 0xffffff) << 28) + (build & 0xfffffff))
		{}

		inline uint64_t Major() const
		{
			return Ver >> 55;
		}

		inline uint64_t Minor() const
		{
			return (Ver >> 46) & 0x1ff;
		}

		inline uint64_t Revision() const
		{
			return (Ver >> 28) & 0xffffff;
		}

		inline uint64_t Build() const
		{
			return Ver & 0xfffffff;
		}

		uint64_t Ver;
	};


	struct ModuleShortDesc
	{
		FixedString ModuleUUID;
		STDString Name;
		Version ModVersion;
		Version PublishVersion;
		STDString MD5;
		STDString Folder;
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
		UUID ModuleUUID;
		STDString Name;
		FixedString StartLevelName;
		FixedString MenuLevelName;
		FixedString LobbyLevelName;
		FixedString CharacterCreationLevelName;
		FixedString PhotoBoothLevelName;
		FixedString MainMenuBackgroundVideo;
		Version ModVersion{ 0 };
		Version PublishVersion{ 0 };
		STDString Hash;
		STDString Directory;
		ObjectSet<void*> Scripts; // ScriptDataList*
		STDString Author;
		STDString Description;
		ObjectSet<STDString, GameMemoryAllocator, true> Tags;
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
