--- @class Logger
--- @field EnabledLogLevels table<string, boolean|nil>
--- @field EnabledMods table<string, boolean|nil>
--- @field EnabledTopics table<string, boolean|nil>
local Logger = {
    EnabledLogLevels = {},
    EnabledMods = {},
    EnabledTopics = {}
}

function Logger:Log(moduleUuid, level, topic, ...)
    if self.EnabledLogLevels[level] == false then return end
    if self.EnabledMods[moduleUuid] == false then return end
    if self.EnabledTopics[topic] == false then return end

    Ext.Log.Print(...)
end

function Logger:CreateLogModule(moduleUuid)
	local log = {}
	-- Forward compatibility with new functions
	setmetatable(log, {
		__index = Ext.Log,
	})
	log.Log = function (level, topic, ...)
		self:Log(moduleUuid, level, topic, ...)
	end
	log.Print = function (...)
		self:Log(moduleUuid, "Info", "", ...)
	end
	log.PrintWarning = function (...)
		self:Log(moduleUuid, "Warning", "", ...)
	end
	log.PrintError = function (...)
		self:Log(moduleUuid, "Error", "", ...)
	end
	log.MakePrinter = function (level, topic)
		return function (...)
            self:Log(moduleUuid, level, topic, ...)
        end
	end
	return log
end

function Logger:CreateUtilsCompatModule(ext)
	local utils = {}
	setmetatable(utils, {
		__index = ext.Utils,
	})
	utils.Print = function (...)
		ext.Log.Print(...)
	end
	utils.PrintWarning = function (...)
		ext.Log.PrintWarning(...)
	end
	utils.PrintError = function (...)
		ext.Log.PrintError(...)
	end
	return utils
end

return Logger
