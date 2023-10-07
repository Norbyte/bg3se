#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

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

	[[bg3::hidden]]
	void* VMT;
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
	[[bg3::hidden]]
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

	[[bg3::hidden]]
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
	[[bg3::hidden]]
	void * VMT;
	uint64_t Unknown;
	[[bg3::hidden]]
	Array<void*> Unknowns;
	Module BaseModule;
	uint8_t Flag;
	ObjectSet<Module> AvailableMods;
	ModuleSettings Settings;

	Module const * FindModByNameGuid(char const * nameGuid) const;
};

END_SE()
