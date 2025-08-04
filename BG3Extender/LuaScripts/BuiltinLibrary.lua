local _G = _G

local _I = Ext._Internal

Mods = {}

_I._PublishedSharedEvents = {
    "ModuleLoadStarted",
    "StatsLoaded",
    "ModuleResume",
    "SessionLoading",
    "SessionLoaded",
    "GameStateChanged",
    "ResetCompleted",
    "Shutdown",
    "DoConsoleCommand",
    "Tick",
    "StatsStructureLoaded",
    "FindPath",
    "NetMessage",
    "NetModMessage",
    -- Thrown by Log module
    "Log"
}

_I._DoStartup = function ()
    _I.EventManager:RegisterEvents()
end

_I._LoadBootstrap = function (path, modTable)
    _I.ModLoader:LoadBootstrap(path, modTable)
end

-- Helper for dumping variables in console
Ext.DumpExport = function (val)
    local opts = {
        Beautify = true,
        StringifyInternalTypes = true,
        IterateUserdata = true,
        AvoidRecursion = true
    }
    return Ext.Json.Stringify(val, opts)
end

Ext.Dump = function (val)
    Ext.Log.Print(Ext.DumpExport(val))
end

Ext.DumpShallow = function (val)
    local opts = {
        Beautify = true,
        StringifyInternalTypes = true,
        IterateUserdata = true,
        AvoidRecursion = true,
        LimitDepth = 1,
        LimitArrayElements = 3
    }
    Ext.Log.Print(Ext.Json.Stringify(val, opts))
end

Ext.Utils.LoadTestLibrary = function ()
    local env = {}
    env._G = env
    setmetatable(env, {__index = _G})
    Ext.Test = env

    if Ext.IsServer() then
        Ext.Utils.Include(nil, "builtin://Tests/ServerTestRunner.lua", env)
    else
        Ext.Utils.Include(nil, "builtin://Tests/ClientTestRunner.lua", env)
    end
end

Ext.OnNextTick = function (fun)
    Ext.Events.Tick:Subscribe(fun, {Once = true})
end

-- v20 compatibility wrappers for Ext.Utils.Print{x}
Ext.Utils.Print = function (...)
    Ext.Log.Print(...)
end
Ext.Utils.PrintWarning = function (...)
    Ext.Log.PrintWarning(...)
end
Ext.Utils.PrintError = function (...)
    Ext.Log.PrintError(...)
end

-- Global helper aliases for Ext.Dump, Ext.Log.Print
_D = Ext.Dump
_DS = Ext.DumpShallow
_P = Ext.Log.Print
_PW = Ext.Log.PrintWarning
_PE = Ext.Log.PrintError
Print = Ext.Log.Print
print = Ext.Log.Print

require = function (name)
    if string.sub(name, -4) == ".lua" then
        -- Name is already a path (backwards compatibility with v21)
        return Ext.Require(name)
    else
        -- Convert module name to .lua path
        return Ext.Require(string.gsub(name, "%.", "/") .. ".lua")
    end
end

-- Backwards compatibility with v3
Ext.Definition = Ext.StaticData
Ext.Utils.Round = Ext.Math.Round
Ext.Utils.Random = Ext.Math.Random

-- Backwards compatibility with v21 beta
Ext.Entity.GetTile = Ext.Level.GetTile
Ext.Entity.GetEntitiesOnTile = Ext.Level.GetEntitiesOnTile

-- Backwards compatibility with v23
Ext.Utils.MonotonicTime = Ext.Timer.MonotonicTime
Ext.Utils.MicrosecTime = Ext.Timer.MicrosecTime
Ext.Utils.GameTime = Ext.Timer.GameTime
