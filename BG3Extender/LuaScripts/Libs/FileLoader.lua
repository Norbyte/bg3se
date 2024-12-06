--- @class FileLoader
local FileLoader = {
    LoadedFiles = {}
}

function FileLoader:Require(mod, path)
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

    if self.LoadedFiles[fullName] ~= nil then
        return table.unpack(self.LoadedFiles[fullName])
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

    self.LoadedFiles[fullName] = loaded
    return table.unpack(loaded)
end

return FileLoader
