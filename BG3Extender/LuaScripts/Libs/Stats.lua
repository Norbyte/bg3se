Ext.Stats.LoadStatsFile = function (path, debug)
    local file = Ext.IO.LoadFile(path, "data")
    local object = nil
    local entry = nil

    for line in string.gmatch(file, "([^\r\n]+)\r*\n") do
        local key, value = string.match(line, "%s*data%s+\"([^\"]+)\"%s+\"([^\"]*)\"")
        if key ~= nil then
            if object ~= nil then
                if debug then print("\027[0;90m  Set: " .. key .. " = " .. value) end
                object:SetRawAttribute(key, value)
            end
        else
            local type = string.match(line, "%s*type%s+\"([^\"]+)\"")
            if type ~= nil then
                if object == nil and entry ~= nil then
                    if debug then print("\027[0;33mCreate new entry: " .. entry .. ", type " .. type) end
                    object = Ext.Stats.Create(entry, type)
                end
            else
                entry = string.match(line, "%s*new%s+entry%s+\"([^\"]+)\"")
                if entry ~= nil then
                    if object ~= nil then
                        Ext.Stats.Sync(object.Name)
                    end

                    object = Ext.Stats.Get(entry, -1, false)
                    if object ~= nil then
                        if debug then print("\027[0;37mUpdate existing entry: " .. entry) end
                    end
                else
                    local using = string.match(line, "%s*using%s+\"([^\"]+)\"")
                    if using ~= nil then
                        if object ~= nil then
                            if debug then print("\027[0;90m  Inherit from: " .. using) end
                            object:CopyFrom(using)
                        end
                    else
                        if debug then Ext.Utils.PrintWarning("Unrecognized line: " .. line) end
                    end
                end
            end
        end
    end

	if object ~= nil then
		Ext.Stats.Sync(object.Name)
	end
end
