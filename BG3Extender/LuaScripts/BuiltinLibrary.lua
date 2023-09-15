local _G = _G

Ext._Internal = {}
local _I = Ext._Internal

_I._LoadedFiles = {}
-- Table to hold debugger expression evaluation results
_I._EVAL_ROOTS_ = {}
Mods = {}

_I._PublishedSharedEvents = {
	"ModuleLoadStarted",
	"ModuleLoading",
	"StatsLoaded",
	"ModuleResume",
	"SessionLoading",
	"SessionLoaded",
	"GameStateChanged",
	"ResetCompleted",
	"DoConsoleCommand",
	"Tick"
}

_I._DoStartup = function ()
	_I._RegisterEvents()
end

_I._NetMessageReceived = function (channel, payload, userId)
	if _I._NetListeners[channel] ~= nil then
		for i,callback in pairs(_I._NetListeners[channel]) do
			local ok, err = xpcall(callback, debug.traceback, channel, payload, userId)
			if not ok then
				Ext.Utils.PrintError("Error during NetMessageReceived: ", err)
			end
		end
	end
end

Ext.Require = function (mod, path)
	if ModuleUUID == nil then
		error("Cannot call Ext.Require() after a module was loaded!");
	end

	local fullName
	if path == nil then
		fullName = ModuleUUID .. "/" .. mod
	else
		fullName = mod .. "/" .. path
	end

	if _I._LoadedFiles[fullName] ~= nil then
		return _I._LoadedFiles[fullName]
	end
	
	local env
	-- LuaJIT workaround
	if getfenv ~= nil then
		env = getfenv(2)
	end

	local loaded
	if path == nil then
		loaded = {Ext.Utils.Include(ModuleUUID, mod, env)}
	else
		loaded = {Ext.Utils.Include(mod, path, env)}
	end

	_I._LoadedFiles[fullName] = loaded
	return table.unpack(loaded)
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
	
	env._G = env
	Ext.Utils.Include(ModuleUUID, path, env)
end

-- Used by the Lua debug adapter to store intermediate evaluation results.
-- Should not be used manually!
Ext.DebugEvaluate = function (retval)
	if type(retval) ~= "table" then
		return retval
	else
		local idx = #_I._EVAL_ROOTS_ + 1
		_I._EVAL_ROOTS_[idx] = retval
		return retval, idx
	end
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
Print = Ext.Utils.Print
print = Ext.Utils.Print

-- Backward compatibility with v3
Ext.Definition = Ext.StaticData
