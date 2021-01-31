local _G = _G

Ext._LoadedFiles = {}
-- Table to hold debugger expression evaluation results
Ext._EVAL_ROOTS_ = {}
Mods = {}

Ext._WarnDeprecated = function (msg)
	Ext.PrintError(msg)
	Ext.PrintError("See https://github.com/Norbyte/ositools/blob/master/LuaAPIDocs.md#migrating-from-v41-to-v42 for more info.")
end

Ext._Notify = function (event, ...)
    for i,callback in pairs(Ext._Listeners[event]) do
        local status, err = xpcall(callback, debug.traceback, ...)
        if not status then
            Ext.PrintError("Error during " .. event .. ": ", err)
        end
    end
end

Ext._EngineCallback1 = function (event, ...)
    for i,callback in pairs(Ext._Listeners[event]) do
        local status, result = xpcall(callback, debug.traceback, ...)
        if status then
			if result ~= nil then
				return result
			end
		else
            Ext.PrintError("Error during " .. event .. ": ", result)
        end
    end
end

Ext._EngineCallback2 = function (event, ...)
    for i,callback in pairs(Ext._Listeners[event]) do
        local status, result1, result2 = xpcall(callback, debug.traceback, ...)
        if status then
			if result1 ~= nil then
				return result1, result2
			end
		else
            Ext.PrintError("Error during " .. event .. ": ", result1)
        end
    end
end

Ext._OnGameSessionLoading = function ()
    Ext._Notify("SessionLoading")
end

Ext._OnGameSessionLoaded = function ()
    Ext._Notify("SessionLoaded")
end

Ext._OnModuleLoadStarted = function ()
    Ext._Notify("ModuleLoadStarted")
end

Ext._OnModuleLoading = function ()
    Ext._Notify("ModuleLoading")
end

Ext._OnStatsLoaded = function ()
    Ext._Notify("StatsLoaded")
end

Ext._OnModuleResume = function ()
    Ext._Notify("ModuleResume")
end

Ext._GameStateChanged = function (...)
    Ext._Notify("GameStateChanged", ...)
end

Ext._GetHitChance = function (...)
    return Ext._EngineCallback1("GetHitChance", ...)
end

Ext._GetSkillAPCost = function (...)
    return Ext._EngineCallback2("GetSkillAPCost", ...)
end

Ext._NetListeners = {}

Ext.RegisterNetListener = function (channel, fn)
	if Ext._NetListeners[channel] == nil then
		Ext._NetListeners[channel] = {}
	end

	table.insert(Ext._NetListeners[channel], fn)
end

Ext._NetMessageReceived = function (channel, payload, userId)
	if Ext._NetListeners[channel] ~= nil then
		for i,callback in pairs(Ext._NetListeners[channel]) do
			local ok, err = xpcall(callback, debug.traceback, channel, payload, userId)
			if not ok then
				Ext.PrintError("Error during NetMessageReceived: ", err)
			end
		end
	end
end

Ext.Require = function (mod, path)
	if ModuleUUID == nil then
		Ext.PrintWarning("Calling Ext.Require() after the module was loaded is deprecated!");
	end

	local fullName
	if path == nil then
		fullName = ModuleUUID .. "/" .. mod
	else
		fullName = mod .. "/" .. path
	end

	if Ext._LoadedFiles[fullName] ~= nil then
		return Ext._LoadedFiles[fullName]
	end
	
	local env
	-- LuaJIT workaround
	if getfenv ~= nil then
		env = getfenv(2)
	end

	local loaded
	if path == nil then
		loaded = {Ext.Include(ModuleUUID, mod, env)}
	else
		loaded = {Ext.Include(mod, path, env)}
	end

	Ext._LoadedFiles[fullName] = loaded
	return table.unpack(loaded)
end

Ext._LoadBootstrap = function (path, modTable)
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
	Ext.Include(ModuleUUID, path, env)
end

Ext._ConsoleCommandListeners = {}

Ext.DoConsoleCommand = function (cmd)
	local params = {}
	for param in string.gmatch(cmd, "%S+") do
		table.insert(params, param)
	end

	local listeners = Ext._ConsoleCommandListeners[params[1]]
	if listeners ~= nil then
		for i,callback in pairs(listeners) do
			local status, result = xpcall(callback, debug.traceback, table.unpack(params))
			if not status then
				Ext.PrintError("Error during console command callback: ", result)
			end
		end
	end
end

Ext.RegisterConsoleCommand = function (cmd, fn)
	if Ext._ConsoleCommandListeners[cmd] == nil then
		Ext._ConsoleCommandListeners[cmd] = {}
	end

	table.insert(Ext._ConsoleCommandListeners[cmd], fn)
end

-- Used by the Lua debug adapter to store intermediate evaluation results.
-- Should not be used manually!
Ext.DebugEvaluate = function (retval)
	if type(retval) ~= "table" then
		return retval
	else
		local idx = #Ext._EVAL_ROOTS_ + 1
		Ext._EVAL_ROOTS_[idx] = retval
		return retval, idx
	end
end

-- Helper for dumping variables in console
Ext.Dump = function (val)
	Ext.Print(Ext.JsonStringify(val, true, true))
end

-- Custom skill property registration
Ext._SkillPropertyTypes = {}

Ext.RegisterSkillProperty = function (name, proto)
	Ext._SkillPropertyTypes[name] = proto
end
