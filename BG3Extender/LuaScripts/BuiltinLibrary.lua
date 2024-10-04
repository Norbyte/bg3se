local _G = _G

Ext._Internal = {}
local _I = Ext._Internal

_I.LoadedMods = {}

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
	"StatsStructureLoaded"
}

_I._DoStartup = function ()
	_I.EventManager:RegisterEvents()
end

_I._LoadBootstrap = function (path, modTable)
	local env = {
		-- Put frequently used items directly into the table for faster access
		type = type,
		tostring = tostring,
		tonumber = tonumber,
		pairs = pairs,
		ipairs = ipairs,
		print = print,
		error = error,
		next = next,

		string = string,
		math = math,
		table = table,

		Ext = Ext,
		Osi = Osi,
		Game = Game,
		Sandboxed = true,

		ModuleUUID = ModuleUUID
	}
	-- The rest are accessed via __index
	setmetatable(env, {__index = _G})
	Mods[modTable] = env
	_I.LoadedMods[modTable] = true
	_I.BootstrappingMod = modTable
	
	env._G = env
	Ext.Utils.Include(ModuleUUID, path, env)
	_I.BootstrappingMod = nil
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
	Ext.Utils.Print(Ext.DumpExport(val))
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
	Ext.Utils.Print(Ext.Json.Stringify(val, opts))
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

-- Global helper aliases for Ext.Dump, Ext.Utils.Print
_D = Ext.Dump
_DS = Ext.DumpShallow
_P = Ext.Utils.Print
_PW = Ext.Utils.PrintWarning
_PE = Ext.Utils.PrintError
Print = Ext.Utils.Print
print = Ext.Utils.Print

-- Backward compatibility with v3
Ext.Definition = Ext.StaticData
Ext.Utils.Round = Ext.Math.Round
Ext.Utils.Random = Ext.Math.Random
