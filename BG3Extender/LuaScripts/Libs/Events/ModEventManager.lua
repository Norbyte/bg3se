local ModEvent = Ext.CoreLib("Events/ModEvent")
local _I = Ext._Internal

local ModEventManager = {}

function ModEventManager:Instantiate()
    return {
        Mods = {},
        Finalized = false
    }
end


function ModEventManager:FinishedLoading()
    for mod,events in pairs(self.Mods) do
        if _I.ModLoader.LoadedMods[mod] ~= true then
            Ext.Log.PrintWarning("Accessing events of mod '" .. mod .. "' that is not loaded!")
        else
            events.Loaded = true

            for name,event in pairs(events.Events) do
                if not event.Registered then
                    Ext.Log.PrintWarning("Accessing nonexistent event '" .. name .. "' of mod '" .. mod .. "'!")
                end
            end
        end
    end

    self.Finalized = true
end


function ModEventManager:CreateModTable()
    local mods = {}
    setmetatable(mods, {
        __index = function (_, mod)
            if self.Mods[mod] == nil then
                if _I.ModLoader.LoadedMods[mod] ~= true and self.Finalized then
                    Ext.Log.PrintWarning("Accessing events of mod '" .. mod .. "' that is not loaded!")
                end

                self.Mods[mod] = self:CreateModEvents(mod)
            end
    
            return self.Mods[mod].PublicTable
        end,
    
        __newindex = function (_, k, v)
            error("Cannot write to Ext.ModEvents directly!")
        end,
    
        __metatable = "ModEvents"
    })
    return mods
end


function ModEventManager:CreateModEvents(mod)
    local events = {
        Events = {},
        PublicTable = {},
        Loaded = false
    }
    setmetatable(events.PublicTable, {
        __index = function (_, event)
            if events.Events[event] == nil then
                if self.Finalized then
                    Ext.Log.PrintWarning("Accessing nonexistent event '" .. event .. "' of mod '" .. mod .. "'!")
                end

                events.Events[event] = ModEvent:New(mod, event)
            end
    
            return events.Events[event].Event
        end,
    
        __newindex = function (_, k, v)
            error("Cannot write to Ext.ModEvents directly!")
        end,
    
        __metatable = "ModEvents"
    })
    return events
end


function ModEventManager:RegisterEvent(mod, event)
    if self.Finalized then
        Ext.Log.PrintWarning("Tried to register mod event after bootstrap phase has ended; this is not allowed.")
        return
    end

    if _I.ModLoader.BootstrappingMod == nil then
        Ext.Log.PrintWarning("Tried to register mod event while not bootstrapping a mod; this is not allowed.")
        return
    end

    if _I.ModLoader.BootstrappingMod ~= mod then
        Ext.Log.PrintWarning("Tried to register mod event for mod '" .. tostring(mod) .. "' while bootstrapping mod '" .. _I.ModLoader.BootstrappingMod .. "'; this is not allowed.")
        return
    end

    if self.Mods[mod] == nil then
        self.Mods[mod] = self:CreateModEvents(mod)
    end

    if self.Mods[mod].Events[event] ~= nil then
        Ext.Log.PrintWarning("Tried to register mod event '" .. mod .. '.' .. event .. "' twice")
        return
    end

    self.Mods[mod].Events[event] = ModEvent:New(mod, event)
end


return Class.Create(ModEventManager)
