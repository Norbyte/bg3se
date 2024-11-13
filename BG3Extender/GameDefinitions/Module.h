#pragma once

#include <GameDefinitions/Base/Base.h>
#include <GameDefinitions/EntitySystem.h>

BEGIN_SE()

struct ModuleShortDesc
{
	FixedString ModuleUUIDString;
	Guid ModuleUUID;
	STDString Name;
	Version ModVersion;
	Version PublishVersion;
	STDString Hash;
	STDString Folder;
	uint64_t PublishHandle;
};

struct ModuleSettings : public ProtectedGameObject<ModuleSettings>
{
	[[bg3::hidden]]
	void* VMT;
	Array<ModuleShortDesc> Mods;
};

struct UGCModDependency
{
	uint64_t DependencyHandle;
	STDString DependencyName;
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
	Version ModVersion{ 0 };
	Version PublishVersion{ 0 };
	STDString Hash;
	STDString Directory;
	[[bg3::hidden]] Array<void*> Scripts; // ScriptDataList*
	STDString Author;
	STDString Description;
	uint8_t NumPlayers{ 4 };
	uint64_t FileSize;
	uint64_t PublishHandle;
};

struct Module
{
	using HashProc = bool (Module* self);

	[[bg3::hidden]] void* VMT{ nullptr };
	ModuleInfo Info;
	Array<Module> LoadOrderedModules;
	Array<Module> DependentModules;
	Array<Module> AddonModules;
	Array<ModuleShortDesc> Dependencies;
	Array<ModuleShortDesc> ModConflicts;
	Array<ModuleShortDesc> Addons;
	uint8_t UnknownFlag;
};

struct ModManager : public ProtectedGameObject<ModManager>
{
	[[bg3::hidden]] void* VMT;
	// FIXME - swap this to a generic callback list
	[[bg3::hidden]] UnknownSignal LoadCallbacks;
	[[bg3::hidden]] UnknownSignal UnloadCallbacks;
	Module BaseModule;
	uint8_t Flag;
	uint8_t Flag2;
	Array<Module> LoadOrderedModules;
	Array<Module> AvailableMods;
	[[bg3::hidden]] HashMap<uint64_t, void*> field_190;
	[[bg3::hidden]] uint64_t field_218;
	ModuleSettings Settings;

	Module const * FindModByNameGuid(char const * nameGuid) const;
};

END_SE()
