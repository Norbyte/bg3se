local _I = Ext._Internal

_I._Listeners = {
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

_I._SkillGetDescriptionParam = function (...)
    return _I._EngineCallback1("SkillGetDescriptionParam", ...)
end

_I._StatusGetDescriptionParam = function (...)
    return _I._EngineCallback1("StatusGetDescriptionParam", ...)
end

_I._GetSkillDamage = function (...)
    for i,callback in pairs(_I._Listeners.GetSkillDamage) do
        local status, damageList, deathType = xpcall(callback, debug.traceback, ...)
        if status then
			if damageList ~= nil then
				return damageList, deathType
			end
		else
            Ext.Utils.PrintError("Error during GetSkillDamage: ", damageList)
        end
    end
end

Ext.RegisterListener = function (type, fn)
	if _I._Listeners[type] ~= nil then
		table.insert(_I._Listeners[type], fn)
	else
		Ext.Utils.PrintError("Unknown listener type: " .. type)
	end
end

Ext.IsClient = function ()
	return true
end

Ext.IsServer = function ()
	return false
end

_I._GetSkillPropertyDescription = function (prop)
	local propType = _I._SkillPropertyTypes[prop.Action]
	if propType ~= nil and propType.GetDescription ~= nil then
		return propType.GetDescription(prop)
	end
end
