--- @class Profiler
local Profiler = {}

function Profiler:ShouldLikelyReport(time)
    -- Base error threshold is assumed to be the smallest, so we don't check the others
    return Ext.Config.PerfMessagesEnabled and time >= Ext.Config.ProfilerCallbackErrorThreshold
end

function Profiler:ShouldReport(time)
    if self:IsLoading() then
        return Ext.Config.PerfMessagesEnabled and time >= Ext.Config.ProfilerLoadCallbackErrorThreshold
    else
        return Ext.Config.PerfMessagesEnabled and time >= Ext.Config.ProfilerCallbackErrorThreshold
    end
end

function Profiler:IsLoading()
    local state = Ext.Utils.GetGameState()
    return state ~= "Running" and state ~= "Paused"
end

function Profiler:Report(time, desc)
    if self:ShouldReport(time) then
        local msg = desc .. " took " .. (Ext.Math.Round(time) / 1000) .. " ms"
        Ext.Log.PrintWarning(msg)
    end
end

function Profiler:AddUtilsHelpers(utils)
    if Ext.Config.ProfilerEnabled then
        local profBegin = Ext.Utils.ProfileBegin
        local profEnd = Ext.Utils.ProfileEnd
        utils.Profile = function (fun, ...)
            profBegin(fun)
            local ret = fun(...)
            profEnd()
            return ret
        end
        utils.ProfileNamed = function (msg, fun, ...)
            profBegin(msg)
            local ret = fun(...)
            profEnd()
            return ret
        end
    else
        utils.Profile = function (fun, ...)
            return fun(...)
        end
        utils.ProfileNamed = function (msg, fun, ...)
            return fun(...)
        end
    end
end

return Profiler
