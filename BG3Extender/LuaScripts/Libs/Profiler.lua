--- @class Profiler
local Profiler = {}

function Profiler:ShouldLikelyReport(time)
    -- Base warning threshold is assumed to be the smallest, so we don't check the others
    return Ext.Config.PerfMessagesEnabled and time >= Ext.Config.ProfilerCallbackWarningThreshold
end

function Profiler:ShouldReport(time)
    if self:IsLoading() then
        return Ext.Config.PerfMessagesEnabled and time >= Ext.Config.ProfilerLoadCallbackWarningThreshold
    else
        return Ext.Config.PerfMessagesEnabled and time >= Ext.Config.ProfilerCallbackWarningThreshold
    end
end

function Profiler:IsLoading()
    local state = Ext.Utils.GetGameState()
    return state ~= "Running" and state ~= "Paused"
end

function Profiler:IsWarning(time)
    if self:IsLoading() then
        return time < Ext.Config.ProfilerLoadCallbackErrorThreshold
    else
        return time < Ext.Config.ProfilerCallbackErrorThreshold
    end
end

function Profiler:Report(time, desc)
    if self:ShouldReport(time) then
        local msg = desc .. " took " .. (Ext.Math.Round(time) / 1000) .. " ms"
        if self:IsWarning(time) then
            Ext.Log.PrintWarning(msg)
        else
            Ext.Log.PrintError(msg)
        end
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
