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
	// Array<STDString> Tags;
	uint8_t NumPlayers{ 4 };
	//Array<FixedString> EffectDependencies;
	//FixedString ModuleType;
	uint64_t FileSize;
	uint64_t PublishHandle;
	HashMap<uint64_t, uint64_t> field_E0;
	uint64_t field_120;
	uint64_t field_128;
};

struct Module
{
	using HashProc = bool (Module* self);

	[[bg3::hidden]] void* VMT{ nullptr };
	ModuleInfo Info;
	Array<Module> LoadOrderedModules;
	Array<Module> DependentModules;
	Array<Module> AddonModules;
	[[bg3::hidden]] Array<void*> Unkn;
	[[bg3::hidden]] uint64_t Unknown[2];
	bool HasValidHash{ true };
	bool UsesLsfFormat{ false };
	[[bg3::hidden]] uint64_t Unknown2[2];
};

struct ModManager : public ProtectedGameObject<ModManager>
{
	[[bg3::hidden]] void* VMT;
	// FIXME - swap this to a generic callback list
	[[bg3::hidden]] ecs::ComponentCallbackList LoadCallbacks;
	[[bg3::hidden]] ecs::ComponentCallbackList UnloadCallbacks;
	Module BaseModule;
	uint8_t Flag;
	Array<Module> AvailableMods;
	[[bg3::hidden]] Array<void*> field_180;
	[[bg3::hidden]] HashMap<uint64_t, void*> field_190;
	[[bg3::hidden]] uint64_t field_218[1];
	ModuleSettings Settings;

	Module const * FindModByNameGuid(char const * nameGuid) const;
};

END_SE()
