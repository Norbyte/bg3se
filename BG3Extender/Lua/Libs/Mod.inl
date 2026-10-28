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
        for (auto const& mod : modManager->LoadOrderedModules) {
            if (mod.Info.ModuleUUID == *modUuid) {
                return true;
            }
        }
    }
    
    for (auto const& mod : modManager->LoadOrderedModules) {
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

    for (auto const& mod : modManager->LoadOrderedModules) {
        loadOrder.push_back(mod.Info.ModuleUUIDString);
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
        for (auto& mod : modManager->LoadOrderedModules) {
            if (mod.Info.ModuleUUID == *modUuid) {
                return &mod;
            }
        }
    }

    for (auto& mod : modManager->LoadOrderedModules) {
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

/// <summary>
/// Writes the client's active mod list (`GetModManager().Settings.Mods`, changed from Lua or not)
/// to the profile's `modsettings.lsx`, as the game's mod manager does when a mod is turned on or
/// off. The game puts each mod's dependencies before it while saving, and loads the list on its
/// next start. Client only: the game's mod manager saves the client's list.
/// </summary>
void SaveSettings(lua_State* L)
{
    if (!State::FromLua(L)->IsClient()) {
        luaL_error(L, "Ext.Mod.SaveSettings is client-only");
        return;
    }

    auto save = GetStaticSymbols().ls__ModManager__SaveSettings;
    if (save == nullptr) {
        luaL_error(L, "ModManager::SaveSettings was not found in this game version");
        return;
    }

    auto modManager = GetModManager(L);
    if (modManager == nullptr) {
        luaL_error(L, "No mod manager");
        return;
    }

    save(modManager);
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
    MODULE_FUNCTION(SaveSettings)
    END_MODULE()
}

END_NS()
