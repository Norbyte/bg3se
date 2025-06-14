--- @class Logger
--- @field EnabledLogLevels table<string, boolean|nil>
--- @field EnabledMods table<string, boolean|nil>
--- @field EnabledTopics table<string, boolean|nil>
local Logger = {
    EnabledLogLevels = {},
    EnabledMods = {},
    EnabledTopics = {},
    DispatchingLogEvent = false
}

local LogLevels = {"Debug", "Info", "Warning", "Error"}

function Logger:Log(moduleUuid, level, topic, ...)
    if self.EnabledLogLevels[level] == false then return end
    if self.EnabledMods[moduleUuid] == false then return end
    if self.EnabledTopics[topic] == false then return end

    if not self.DispatchingLogEvent then
        self.DispatchingLogEvent = true
        Ext.Events.Log:Throw({
            Stopped = false,
            Module = moduleUuid,
            Topic = topic,
            Level = level,
            Message = {...}
        })
        self.DispatchingLogEvent = false
    end

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
    log.Debug = function (...)
        self:Log(moduleUuid, "Debug", "", ...)
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
    log.SetLogLevel = function (level)
        self:SetLogLevel(level)
    end
    log.EnableModLogging = function (modUuid, enable)
        self.EnabledMods[modUuid] = enable
    end
    log.EnableTopic = function (topic, enable)
        self.EnabledTopics[topic] = enable
    end
    return log
end

function Logger:SetLogLevel(level)
    local enable = false
    for i,llevel in ipairs(LogLevels) do
        if level == llevel then
            enable = true
        end

        self.EnabledLogLevels[llevel] = enable
    end

    if not enable then
        error("Unknown log level: " .. level)
    end
end

function Logger:CreateUtilsCompatModule(ext, baseExt)
    local utils = {}
    setmetatable(utils, {
        __index = baseExt.Utils,
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
