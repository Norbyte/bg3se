/// <lua_module>Mod</lua_module>
BEGIN_NS(lua::mod)

/// <summary>
/// Returns whether the module with the specified GUID is loaded.
/// This is equivalent to Osiris `NRD_IsModLoaded`, but is callable when the Osiris scripting runtime is not yet available (i.e. `ModuleLoading˙, etc events).
/// 
/// Example:
/// ```lua
/// if (Ext.IsModLoaded("5cc23efe-f451-c414-117d-b68fbc53d32d")) then
///     Ext.Print("Mod loaded")
/// end
/// ```
/// </summary>
/// <param name="modNameGuid">UUID of mod to check</param>
bool IsModLoaded(char const* modNameGuid)
{
	auto modUuid = Guid::Parse(modNameGuid);
	if (modUuid) {
		auto modManager = gExtender->GetCurrentExtensionState()->GetModManager();
		for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
			if (mod.Info.ModuleUUID == *modUuid) {
				return true;
			}
		}
	}

	return false;
}

/// <summary>
/// Returns the list of loaded module UUIDs in the order they're loaded in.
/// </summary>
/// <returns></returns>
ObjectSet<Guid> GetLoadOrder()
{
	ObjectSet<Guid> loadOrder;
	auto modManager = gExtender->GetCurrentExtensionState()->GetModManager();

	for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
		loadOrder.Add(mod.Info.ModuleUUID);
	}

	return loadOrder;
}

/// <summary>
/// Returns detailed information about the specified (loaded) module.
/// This function is deprecated; use `Ext.Mod.GetMod()` instead.
/// 
/// Example:
/// ```lua
/// local loadOrder = Ext.Mods.GetLoadOrder()
/// for k, uuid in pairs(loadOrder) do
///     local mod = Ext.GetModInfo(uuid)
///     Ext.Dump(mod)
/// end
/// ```
/// </summary>
/// <param name="modNameGuid">Mod UUID to query</param>
UserReturn GetModInfo(lua_State* L, char const* modNameGuid)
{
	StackCheck _(L, 1);
	Module const * module{ nullptr };
	auto modUuid = Guid::Parse(modNameGuid);
	if (modUuid) {
		auto modManager = gExtender->GetCurrentExtensionState()->GetModManager();
		for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
			if (mod.Info.ModuleUUID == *modUuid) {
				module = &mod;
				break;
			}
		}
	}

	if (module != nullptr) {
		lua_newtable(L);
		setfield(L, "UUID", module->Info.ModuleUUID);
		setfield(L, "Name", module->Info.Name);
		setfield(L, "Version", module->Info.ModVersion.Ver);
		setfield(L, "PublishVersion", module->Info.PublishVersion.Ver);
		setfield(L, "Directory", module->Info.Directory);
		setfield(L, "Author", module->Info.Author);
		setfield(L, "Description", module->Info.Description);
		setfield(L, "ModuleType", module->Info.ModuleType);
			
		lua_newtable(L);
		auto & dependents = module->DependentModules;
		for (uint32_t i = 0; i < dependents.Size(); i++) {
			auto const & mod = dependents[i];
			settable(L, i + 1, mod.Info.ModuleUUID);
		}
		lua_setfield(L, -2, "Dependencies");
	} else {
		push(L, nullptr);
	}

	return 1;
}

/// <summary>
/// Returns detailed information about the specified (loaded) module.
/// </summary>
/// <param name="modNameGuid">Mod UUID to query</param>
Module* GetMod(char const* modNameGuid)
{
	Module const * module{ nullptr };
	auto modUuid = Guid::Parse(modNameGuid);
	if (modUuid) {
		auto modManager = gExtender->GetCurrentExtensionState()->GetModManager();
		for (auto& mod : modManager->BaseModule.LoadOrderedModules) {
			if (mod.Info.ModuleUUID == *modUuid) {
				return &mod;
			}
		}
	}

	return nullptr;
}

Module* GetBaseMod()
{
	return &gExtender->GetCurrentExtensionState()->GetModManager()->BaseModule;
}

void RegisterModLib()
{
	DECLARE_MODULE(Mod, Both)
	BEGIN_MODULE()
	MODULE_FUNCTION(IsModLoaded)
	MODULE_FUNCTION(GetLoadOrder)
	MODULE_FUNCTION(GetModInfo)
	MODULE_FUNCTION(GetMod)
	MODULE_FUNCTION(GetBaseMod)
	END_MODULE()
}

END_NS()
