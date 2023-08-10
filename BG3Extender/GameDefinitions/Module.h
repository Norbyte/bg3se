#pragma once

#include <GameDefinitions/Base/Base.h>

namespace bg3se
{
	struct Version
	{
		inline Version() : Ver(0) {}
		inline Version(uint64_t ver) : Ver(ver) {}
		inline Version(uint64_t minor, uint64_t major, uint64_t revision, uint64_t build)
			: Ver(((major & 0x1ffull) << 55) | ((minor & 0x1ffull) << 47) | ((revision & 0x7ffffull) << 31) | (build & 0x7fffffffull))
		{}

		inline uint64_t Major() const
		{
			return Ver >> 55;
		}

		inline uint64_t Minor() const
		{
			return (Ver >> 47) & 0xff;
		}

		inline uint64_t Revision() const
		{
			return (Ver >> 31) & 0x7ffff;
		}

		inline uint64_t Build() const
		{
			return Ver & 0x7fffffffull;
		}

		uint64_t Ver;
	};


	struct ModuleShortDesc
	{
		FixedString ModuleUUID;
		STDString Name;
		Version ModVersion;
		Version PublishVersion;
		STDString Hash;
		STDString Folder;
	};

	struct ModuleSettings : public ProtectedGameObject<ModuleSettings>
	{
		using ValidateProc = int(ModuleSettings& self, ModuleSettings& host, ObjectSet<ModuleShortDesc>& mismatches);

		void *VMT;
		Array<ModuleShortDesc> Mods;
		Array<FixedString> ModOrder;
	};

	struct ModuleInfo
	{
		FixedString ModuleUUIDString;
		Guid ModuleUUID;
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
		Array<void*> Scripts; // ScriptDataList*
		STDString Author;
		STDString Description;
		Array<STDString> Tags;
		uint8_t NumPlayers{ 4 };
		Array<FixedString> TargetModes;
		FixedString ModuleType;
	};

	struct Module
	{
		using HashProc = bool (Module* self);

		void* VMT{ nullptr };
		ModuleInfo Info;
		Array<Module> LoadOrderedModules;
		Array<Module> ContainedModules;
		Array<Module> DependentModules;
		Array<Module> AddonModules;
		bool HasValidHash{ true };
		bool UsesLsfFormat{ false };
		bool FinishedLoading{ false };
	};

	struct ModManager : public ProtectedGameObject<ModManager>
	{
		typedef void (*CollectAvailableMods)(ObjectSet<Module, GameMemoryAllocator, true>& mods);

		void * VMT;
		uint64_t Unknown;
		Array<void*> Unknowns;
		Module BaseModule;
		uint8_t Flag;
		ObjectSet<Module, GameMemoryAllocator, true> AvailableMods;
		ModuleSettings Settings;

		Module const * FindModByNameGuid(char const * nameGuid) const;
	};
}
