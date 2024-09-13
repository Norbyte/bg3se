/// <lua_module>Mod</lua_module>
BEGIN_NS(lua::mod)

/// <summary>
/// Returns whether the module with the specified GUID is loaded.
/// This is equivalent to Osiris `NRD_IsModLoaded`, but is callable when the Osiris scripting runtime is not yet available (i.e. `ModuleLoadStarted˙, etc events).
/// 
/// Example:
/// ```lua
/// if (Ext.IsModLoaded("5cc23efe-f451-c414-117d-b68fbc53d32d")) then
///     Ext.Print("Mod loaded")
/// end
/// ```
/// </summary>
/// <param name="modNameGuid">UUID of mod to check</param>
bool IsModLoaded(lua_State* L, FixedString modNameGuid)
{
	auto modManager = ExtensionStateBase::FromLua(L).GetModManager();

	auto modUuid = Guid::Parse(modNameGuid.GetStringView());
	if (modUuid) {
		for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
			if (mod.Info.ModuleUUID == *modUuid) {
				return true;
			}
		}
	}
	
	for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
		if (mod.Info.ModuleUUIDString == modNameGuid) {
			return true;
		}
	}

	return false;
}

/// <summary>
/// Returns the list of loaded module UUIDs in the order they're loaded in.
/// </summary>
/// <returns></returns>
Array<FixedString> GetLoadOrder(lua_State* L)
{
	Array<FixedString> loadOrder;
	auto modManager = ExtensionStateBase::FromLua(L).GetModManager();

	for (auto const& mod : modManager->BaseModule.LoadOrderedModules) {
		loadOrder.Add(mod.Info.ModuleUUIDString);
	}

	return loadOrder;
}

/// <summary>
/// Returns detailed information about the specified (loaded) module.
/// </summary>
/// <param name="modNameGuid">Mod UUID to query</param>
Module* GetMod(lua_State* L, FixedString modNameGuid)
{
	auto modManager = ExtensionStateBase::FromLua(L).GetModManager();

	auto modUuid = Guid::Parse(modNameGuid.GetStringView());
	if (modUuid) {
		for (auto& mod : modManager->BaseModule.LoadOrderedModules) {
			if (mod.Info.ModuleUUID == *modUuid) {
				return &mod;
			}
		}
	}

	for (auto& mod : modManager->BaseModule.LoadOrderedModules) {
		if (mod.Info.ModuleUUIDString == modNameGuid) {
			return &mod;
		}
	}

	return nullptr;
}

Module* GetBaseMod(lua_State* L)
{
	return &ExtensionStateBase::FromLua(L).GetModManager()->BaseModule;
}

ModManager* GetModManager(lua_State* L)
{
	return ExtensionStateBase::FromLua(L).GetModManager();
}

void RegisterModLib()
{
	DECLARE_MODULE(Mod, Both)
	BEGIN_MODULE()
	MODULE_FUNCTION(IsModLoaded)
	MODULE_FUNCTION(GetLoadOrder)
	MODULE_FUNCTION(GetMod)
	MODULE_FUNCTION(GetBaseMod)
	MODULE_FUNCTION(GetModManager)
	END_MODULE()
}

END_NS()
