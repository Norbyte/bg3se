dofile = function ()
    error("dofile() has been disabled for security reasons")
end

loadfile = function ()
    error("loadfile() has been disabled for security reasons")
end

load = Ext.Utils.LoadString
loadstring = Ext.Utils.LoadString

local oldDebug = debug
debug = {
    traceback = oldDebug.traceback,
    getinfo = oldDebug.getinfo
}
oldDebug = nil

-- math.random replaced by custom implementation
math.random = Ext.Math.Random
-- math.randomseed not implemented
math.randomseed = function ()
    error("math.randomseed() not implemented")
end
