local _G = _G

Ext._Internal = {}
local _I = Ext._Internal

_I._LoadedFiles = {}
-- Table to hold debugger expression evaluation results
_I._EVAL_ROOTS_ = {}
Mods = {}

_I._Notify = function (event, ...)
    for i,callback in pairs(_I._Listeners[event]) do
        local status, err = xpcall(callback, debug.traceback, ...)
        if not status then
            Ext.Utils.PrintError("Error during " .. event .. ": ", err)
        end
    end
end

_I._EngineCallback1 = function (event, ...)
    for i,callback in pairs(_I._Listeners[event]) do
        local status, result = xpcall(callback, debug.traceback, ...)
        if status then
			if result ~= nil then
				return result
			end
		else
            Ext.Utils.PrintError("Error during " .. event .. ": ", result)
        end
    end
end

_I._EngineCallback2 = function (event, ...)
    for i,callback in pairs(_I._Listeners[event]) do
        local status, result1, result2 = xpcall(callback, debug.traceback, ...)
        if status then
			if result1 ~= nil then
				return result1, result2
			end
		else
            Ext.Utils.PrintError("Error during " .. event .. ": ", result1)
        end
    end
end

_I._OnGameSessionLoading = function ()
    _I._Notify("SessionLoading")
end

_I._OnGameSessionLoaded = function ()
    _I._Notify("SessionLoaded")
end

_I._OnModuleLoadStarted = function ()
    _I._Notify("ModuleLoadStarted")
end

_I._OnModuleLoading = function ()
    _I._Notify("ModuleLoading")
end

_I._OnStatsLoaded = function ()
    _I._Notify("StatsLoaded")
end

_I._OnModuleResume = function ()
    _I._Notify("ModuleResume")
end

_I._GameStateChanged = function (...)
    _I._Notify("GameStateChanged", ...)
end

_I._GetHitChance = function (...)
    return _I._EngineCallback1("GetHitChance", ...)
end

_I._GetSkillAPCost = function (...)
    return _I._EngineCallback2("GetSkillAPCost", ...)
end

_I._NetListeners = {}

_I.RegisterNetListener = function (channel, fn)
	if _I._NetListeners[channel] == nil then
		_I._NetListeners[channel] = {}
	end

	table.insert(_I._NetListeners[channel], fn)
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

_I._ConsoleCommandListeners = {}

_I.DoConsoleCommand = function (cmd)
	local params = {}
	for param in string.gmatch(cmd, "%S+") do
		table.insert(params, param)
	end

	local listeners = _I._ConsoleCommandListeners[params[1]]
	if listeners ~= nil then
		for i,callback in pairs(listeners) do
			local status, result = xpcall(callback, debug.traceback, table.unpack(params))
			if not status then
				Ext.Utils.PrintError("Error during console command callback: ", result)
			end
		end
	end
end

Ext.RegisterConsoleCommand = function (cmd, fn)
	if _I._ConsoleCommandListeners[cmd] == nil then
		_I._ConsoleCommandListeners[cmd] = {}
	end

	table.insert(_I._ConsoleCommandListeners[cmd], fn)
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
Ext.Dump = function (val)
	Ext.Utils.Print(Ext.Json.Stringify(val, true, true))
end

-- Custom skill property registration
_I._SkillPropertyTypes = {}

Ext.RegisterSkillProperty = function (name, proto)
	_I._SkillPropertyTypes[name] = proto
end
