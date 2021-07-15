function Assert(expr)
    if not expr then
        error("Assertion failed")
    end
end

function AssertEquals(value, expectation)
    local equals
    if type(expectation) == "table" then
        equals = (Ext.Json.Stringify(expectation) == Ext.Json.Stringify(value))
    else
        equals = (expectation == value)
    end

    if not equals then
        error("Expressions not equal: expected " .. Ext.Json.Stringify(expectation, false, true) .. " , got " .. Ext.Json.Stringify(value, false, true))
    end
end

function AssertEqualsFloat(value, expectation)
    if math.abs(expectation - value) > 0.00001 then
        error("Expressions not equal: expected " .. expectation .. " , got " .. value)
    end
end

function AssertPropertyEquals(k, expectation, value)
    if Ext.Utils.IsIterable(expectation) then
        if type(value) == "table" then
            AssertEqualsProperties(expectation, value)
        else
            local mt = getmetatable(value)
            if mt == "bg3se::Array" then
                AssertEqualsArray(expectation, value)
            elseif mt == "bg3se::Set" then
                AssertEqualsSet(expectation, value)
            elseif mt == "bg3se::Map" or mt == "bg3se::Object" then
                AssertEqualsProperties(expectation, value)
            else
                error("Don't know how to assert userdata: " .. mt)
            end
        end
        return
    end
    
    local equals
    if type(expectation) == "number" then
        equals = (math.abs(expectation - value) < 0.00001)
    else
        equals = (expectation == value)
    end

    if not equals then
        if type(expectation) == "number" then
            error("Property value not equal: " .. k .. ": " .. expectation .. " = " .. value)
        else
            error("Property value not equal: " .. k .. ": " .. Ext.Json.Stringify(expectation, false, true) .. " = " .. Ext.Json.Stringify(value, false, true))
        end
    end
end

function AssertEqualsArray(expectation, value)
    if #expectation ~= #value then
        error("Array length not equal: " .. #expectation .. " = " .. #value)
    end

    for k,exp in ipairs(expectation) do
        local val = value[k]
        AssertPropertyEquals(k, exp, val)
    end
end

function AssertEqualsSet(expectation, value)
    if #expectation ~= #value then
        error("Set length not equal: " .. #expectation .. " = " .. #value)
    end

    for k,exp in ipairs(expectation) do
        if value[exp] ~= true then
            error("Value not in set: " .. exp)
        end
    end
end

function AssertEqualsProperties(expectation, value)
    for k,exp in pairs(expectation) do
        local val = value[k]

        AssertPropertyEquals(k, exp, val)
    end
end

function RunTest(name, fun)
    local result, err = xpcall(fun, debug.traceback)
    if result then
        Ext.Utils.Print("Test OK: " .. name)
    else
        Ext.Utils.PrintError("Test FAILED: " .. name)
        Ext.Utils.PrintError(err)
    end
end
