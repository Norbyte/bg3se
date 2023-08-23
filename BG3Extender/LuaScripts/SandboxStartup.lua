
local extMetatable = {
	__index = Ext,
    
    __newindex = function (t, k, v)
		Ext.Utils.PrintError("Couldn't set Ext." .. k .. ": Please avoid extending the Ext table - it is dangerous and may break compatibility!")
	end
}

local oldExt = Ext
Ext = {}
setmetatable(Ext, extMetatable)


dofile = function ()
	error("dofile() has been disabled for security reasons")
end

loadfile = function ()
	error("loadfile() has been disabled for security reasons")
end

load = function ()
	error("load() has been disabled for security reasons")
end

loadstring = function ()
	error("loadstring() has been disabled for security reasons")
end

local oldDebug = debug
debug = {
	traceback = oldDebug.traceback
}
oldDebug = nil

-- math.random replaced by custom implementation
math.random = Ext.Random
-- math.randomseed not implemented
math.randomseed = function ()
	error("math.randomseed() not implemented")
end
