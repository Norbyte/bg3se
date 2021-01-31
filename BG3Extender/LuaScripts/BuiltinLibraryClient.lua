Ext._Listeners = {
	ModuleLoadStarted = {},
	ModuleLoading = {},
	StatsLoaded = {},
	ModuleResume = {},
	SessionLoading = {},
	SessionLoaded = {},
	GameStateChanged = {},
	SkillGetDescriptionParam = {},
	StatusGetDescriptionParam = {},
	GetSkillDamage = {},
	GetSkillAPCost = {},
	GetHitChance = {},
	UIObjectCreated = {},
	UIInvoke = {},
	UICall = {},
	AfterUIInvoke = {},
	AfterUICall = {}
}

Ext._SkillGetDescriptionParam = function (...)
    return Ext._EngineCallback1("SkillGetDescriptionParam", ...)
end

Ext._StatusGetDescriptionParam = function (...)
    return Ext._EngineCallback1("StatusGetDescriptionParam", ...)
end

Ext._GetSkillDamage = function (...)
    for i,callback in pairs(Ext._Listeners.GetSkillDamage) do
        local status, damageList, deathType = xpcall(callback, debug.traceback, ...)
        if status then
			if damageList ~= nil then
				return damageList, deathType
			end
		else
            Ext.PrintError("Error during GetSkillDamage: ", damageList)
        end
    end
end

Ext.RegisterListener = function (type, fn)
	if Ext._Listeners[type] ~= nil then
		table.insert(Ext._Listeners[type], fn)
	elseif type == "CalculateTurnOrder" or type == "ComputeCharacterHit" or type == "StatusGetEnterChance" then
		Ext._WarnDeprecated("Cannot register listeners for event '" .. type .. "' from client!")
	else
		Ext.PrintError("Unknown listener type: " .. type)
	end
end

Ext.IsClient = function ()
	return true
end

Ext.IsServer = function ()
	return false
end

Ext.NewCall = function ()
	Ext._WarnDeprecated("Calling Ext.NewCall() from a client context is deprecated!")
end

Ext.NewQuery = function ()
	Ext._WarnDeprecated("Calling Ext.NewQuery() from a client context is deprecated!")
end

Ext.NewEvent = function ()
	Ext._WarnDeprecated("Calling Ext.NewEvent() from a client context is deprecated!")
end

Ext.GetCombat = function ()
	Ext._WarnDeprecated("Calling Ext.GetCombat() from a client context is deprecated!")
end

Ext.GenerateIdeHelpers = function ()
	Ext._WarnDeprecated("Calling Ext.GenerateIdeHelpers() from a client context is deprecated!")
end

Ext.EnableStatOverride = function ()
	Ext._WarnDeprecated("Calling Ext.EnableStatOverride() is no longer neccessary!")
end

Ext._UIObjectCreated = function (uiObject)
	Ext._Notify("UIObjectCreated", uiObject)
end

Ext._UIExternalInterfaceHandleListeners = { Before = {}, After = {} }
Ext._UIExternalInterfaceTypeListeners = { Before = {}, After = {} }
Ext._UIExternalInterfaceNameListeners = { Before = {}, After = {} }

Ext.RegisterUICall = function (uiObject, call, fn, type)
	type = type or "Before"
	local handle = uiObject:GetHandle()
	if Ext._UIExternalInterfaceHandleListeners[type][handle] == nil then
		Ext._UIExternalInterfaceHandleListeners[type][handle] = {}
	end
	
	if Ext._UIExternalInterfaceHandleListeners[type][handle][call] == nil then
		Ext._UIExternalInterfaceHandleListeners[type][handle][call] = {}
	end
	
	table.insert(Ext._UIExternalInterfaceHandleListeners[type][handle][call], fn)
	uiObject:CaptureExternalInterfaceCalls()
end

Ext.RegisterUITypeCall = function (typeId, call, fn, type)
	type = type or "Before"
	if Ext._UIExternalInterfaceTypeListeners[type][typeId] == nil then
		Ext._UIExternalInterfaceTypeListeners[type][typeId] = {}
	end
	
	if Ext._UIExternalInterfaceTypeListeners[type][typeId][call] == nil then
		Ext._UIExternalInterfaceTypeListeners[type][typeId][call] = {}
	end
	
	table.insert(Ext._UIExternalInterfaceTypeListeners[type][typeId][call], fn)
end

Ext.RegisterUINameCall = function (call, fn, type)
	type = type or "Before"
	if Ext._UIExternalInterfaceNameListeners[type][call] == nil then
		Ext._UIExternalInterfaceNameListeners[type][call] = {}
	end
	
	table.insert(Ext._UIExternalInterfaceNameListeners[type][call], fn)
end

Ext._UICall = function (uiObject, call, type, ...)
    local listeners = Ext._UIExternalInterfaceHandleListeners[type][uiObject:GetHandle()]
	if listeners ~= nil and listeners[call] ~= nil then
		for i,callback in pairs(listeners[call]) do
			local status, err = xpcall(callback, debug.traceback, uiObject, call, ...)
			if not status then
				Ext.PrintError("Error in UI ExternalInterface handler for '" .. call .. "': ", err)
			end
		end
	end

    local listeners = Ext._UIExternalInterfaceTypeListeners[type][uiObject:GetTypeId()]
	if listeners ~= nil and listeners[call] ~= nil then
		for i,callback in pairs(listeners[call]) do
			local status, err = xpcall(callback, debug.traceback, uiObject, call, ...)
			if not status then
				Ext.PrintError("Error in UI ExternalInterface handler for '" .. call .. "': ", err)
			end
		end
	end

    local listeners = Ext._UIExternalInterfaceNameListeners[type][call]
	if listeners ~= nil then
		for i,callback in pairs(listeners) do
			local status, err = xpcall(callback, debug.traceback, uiObject, call, ...)
			if not status then
				Ext.PrintError("Error in UI ExternalInterface handler for '" .. call .. "': ", err)
			end
		end
	end

	if type == "Before" then
		Ext._Notify("UICall", uiObject, call, ...)
	else
		Ext._Notify("AfterUICall", uiObject, call, ...)
	end
end

Ext._UIInvokeHandleListeners = { Before = {}, After = {} }
Ext._UIInvokeTypeListeners = { Before = {}, After = {} }
Ext._UIInvokeNameListeners = { Before = {}, After = {} }

Ext.RegisterUIInvokeListener = function (uiObject, method, fn, type)
	type = type or "Before"
	local handle = uiObject:GetHandle()
	if Ext._UIInvokeHandleListeners[type][handle] == nil then
		Ext._UIInvokeHandleListeners[type][handle] = {}
	end
	
	if Ext._UIInvokeHandleListeners[type][handle][method] == nil then
		Ext._UIInvokeHandleListeners[type][handle][method] = {}
	end
	
	table.insert(Ext._UIInvokeHandleListeners[type][handle][method], fn)
	uiObject:CaptureInvokes()
end

Ext.RegisterUITypeInvokeListener = function (typeId, method, fn, type)
	type = type or "Before"
	if Ext._UIInvokeTypeListeners[type][typeId] == nil then
		Ext._UIInvokeTypeListeners[type][typeId] = {}
	end
	
	if Ext._UIInvokeTypeListeners[type][typeId][method] == nil then
		Ext._UIInvokeTypeListeners[type][typeId][method] = {}
	end
	
	table.insert(Ext._UIInvokeTypeListeners[type][typeId][method], fn)
end

Ext.RegisterUINameInvokeListener = function (method, fn, type)
	type = type or "Before"
	if Ext._UIInvokeNameListeners[type][method] == nil then
		Ext._UIInvokeNameListeners[type][method] = {}
	end
	
	table.insert(Ext._UIInvokeNameListeners[type][method], fn)
end

Ext._UIInvoke = function (uiObject, method, type, ...)
    local listenersByHandle = Ext._UIInvokeHandleListeners[type][uiObject:GetHandle()]
	if listenersByHandle ~= nil and listenersByHandle[method] ~= nil then
		for i,callback in pairs(listenersByHandle[method]) do
			local status, err = xpcall(callback, debug.traceback, uiObject, method, ...)
			if not status then
				Ext.PrintError("Error in UI Invoke listener for '" .. method .. "': ", err)
			end
		end
	end

    local listenersByType = Ext._UIInvokeTypeListeners[type][uiObject:GetTypeId()]
	if listenersByType ~= nil and listenersByType[method] ~= nil then
		for i,callback in pairs(listenersByType[method]) do
			local status, err = xpcall(callback, debug.traceback, uiObject, method, ...)
			if not status then
				Ext.PrintError("Error in UI Invoke listener for '" .. method .. "': ", err)
			end
		end
	end

    local listenersByName = Ext._UIInvokeNameListeners[type][method]
	if listenersByName ~= nil then
		for i,callback in pairs(listenersByName) do
			local status, err = xpcall(callback, debug.traceback, uiObject, method, ...)
			if not status then
				Ext.PrintError("Error in UI Invoke listener for '" .. method .. "': ", err)
			end
		end
	end
	
	if type == "Before" then
		Ext._Notify("UIInvoke", uiObject, method, ...)
	else
		Ext._Notify("AfterUIInvoke", uiObject, method, ...)
	end
end

-- Update version label with Script Extender version number when the main menu is rendered
Ext.RegisterUITypeInvokeListener(28, "openMenu", function (menu, ...)
    local ver = Ext.GameVersion() .. " (Script Extender v" .. Ext.Version() .. ")"
    menu:Invoke("setDebugText", ver)
end)

-- Subscribe to main menu calls
Ext.RegisterListener("UIObjectCreated", function (ui)
    if ui:GetTypeId() == 28 then
        ui:CaptureExternalInterfaceCalls()
        ui:CaptureInvokes()
    end
end)

Ext._GetSkillPropertyDescription = function (prop)
	local propType = Ext._SkillPropertyTypes[prop.Action]
	if propType ~= nil and propType.GetDescription ~= nil then
		return propType.GetDescription(prop)
	end
end
