--- @class EngineInternalsModule
--- @field ActivateEvent fun(id: integer)
--- @field DeactivateEvent fun(id: integer)
--- @field GetPublishedEvents fun(): table<string, integer>


--- @class ExtInternals
--- @field ModLoader ModLoader
--- @field FileLoader FileLoader
--- @field Profiler Profiler
--- @field EventManager EventManager
--- @field EngineInternals EngineInternalsModule
--- @field _PublishedLocalEvents string[]
Ext._Internal = {}
local _I = Ext._Internal

_I.EngineInternals = Ext.Internal
_I.FileLoader = Ext.Utils.Include(nil, "builtin://Libs/FileLoader.lua")

Ext.Require = function(mod, path)
    return _I.FileLoader:Require(mod, path)
end

Ext.CoreLib = function (name)
    return Ext.Require("builtin://Libs/" .. name .. ".lua")
end

Class = Ext.CoreLib("Class")

Ext.Utils.Include(nil, "builtin://BuiltinLibrary.lua")
Ext.CoreLib("Event")
Ext.CoreLib("Stats")
_I.ModLoader = Ext.CoreLib("ModLoader")
_I.Profiler = Ext.CoreLib("Profiler")
_I.Profiler:AddUtilsHelpers(Ext.Utils)

-- Unclobber global namespace
Class = nil
Ext.Internal = nil
Ext.ClientInternal = nil
Ext.ServerInternal = nil
