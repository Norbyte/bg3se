--- @type Logger
local logger = Ext.CoreLib("Logger")
--- @type NetworkManager
local net = Ext.CoreLib("NetworkManager")

--- @class ModLoader
local ModLoader = {
    BootstrappingMod = nil,
    LoadedMods = {}
}

function ModLoader:CreateModExtensionTable(moduleUuid)
    local ext = {}
    -- Allow mutating Ext during mod load phase
    setmetatable(ext, {
        __index = Ext
    })
    
    ext.Log = logger:CreateLogModule(moduleUuid)
    ext.Utils = logger:CreateUtilsCompatModule(ext, Ext)
    ext.Net = net:CreateNetModule(Ext)

    -- Prevent mutating Ext after we're finished
    setmetatable(ext, {
        __index = Ext,
        
        __newindex = function (t, k, v)
            Ext.Log.PrintError("Couldn't set Ext." .. k .. ": Please avoid extending the Ext table - it is dangerous and may break compatibility!")
        end
    })
    return ext
end

function ModLoader:CreateModEnv()
    local ext = self:CreateModExtensionTable(ModuleUUID)

    local env = {
        -- Put frequently used items directly into the table for faster access
        type = type,
        tostring = tostring,
        tonumber = tonumber,
        pairs = pairs,
        ipairs = ipairs,
        error = error,
        next = next,

        string = string,
        math = math,
        table = table,

        Ext = ext,
        Osi = Osi,
        Sandboxed = true,
        
        -- Global helper aliases for Ext.Log.Print
        _P = ext.Log.Print,
        _PW = ext.Log.PrintWarning,
        _PE = ext.Log.PrintError,
        Print = ext.Log.Print,
        print = ext.Log.Print,

        ModuleUUID = ModuleUUID
    }
    -- The rest are accessed via __index
    setmetatable(env, {__index = _G})
    env._G = env
    return env
end

function ModLoader:LoadBootstrap(path, modTable)
    local env = self:CreateModEnv()

    Mods[modTable] = env
    self.LoadedMods[modTable] = true
    self.BootstrappingMod = modTable
    
    Ext.Utils.Include(ModuleUUID, path, env)
    self.BootstrappingMod = nil
end

return ModLoader
