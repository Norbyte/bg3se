local _LoadedFiles = {}

Ext.Require = function (mod, path)
	local fullName
	if path == nil then
		if mod:sub(1, 10) == "builtin://" then
			fullName = mod
		else
			if ModuleUUID == nil then
				error("Ext.Require() called without a mod, but the current mod UUID is not known!");
			end
		
			fullName = ModuleUUID .. "/" .. mod
		end
	else
		fullName = mod .. "/" .. path
	end

	if _LoadedFiles[fullName] ~= nil then
		return table.unpack(_LoadedFiles[fullName])
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

	_LoadedFiles[fullName] = loaded
	return table.unpack(loaded)
end

Ext.CoreLib = function (name)
    return Ext.Require("builtin://Libs/" .. name .. ".lua")
end

Class = Ext.CoreLib("Class")

Ext.Utils.Include(nil, "builtin://BuiltinLibrary.lua")
Ext.Utils.Include(nil, "builtin://Libs/Event.lua")
Ext.Utils.Include(nil, "builtin://Libs/Stats.lua")

-- Unclobber global namespace
Class = nil
