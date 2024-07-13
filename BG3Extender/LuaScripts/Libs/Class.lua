local _ClassLib = {}

function _ClassLib.Create(defn)
    local cls = {}
    cls.__index = cls

    for k,v in pairs(defn) do
        cls[k] = v
    end

    function cls:New(...)
        local o = cls:Instantiate(...)
        setmetatable(o, self)
        return o
    end

    return cls
end

return _ClassLib
