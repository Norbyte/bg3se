local _I = Ext._Internal

_I._Listeners = {
	ModuleLoadStarted = {},
	ModuleLoading = {},
	StatsLoaded = {},
	ModuleResume = {},
	SessionLoading = {},
	SessionLoaded = {},
	ResetCompleted = {},
	GameStateChanged = {},

	OnDealDamage = {},
	OnExecuteFunctor = {},

	-- FIXME - IMPLEMENT
	GetSkillDamage = {},
	GetSkillAPCost = {},
	ComputeCharacterHit = {},
	CalculateTurnOrder = {},
	GetHitChance = {},
	StatusGetEnterChance = {},
	StatusHitEnter = {},
	BeforeCharacterApplyDamage = {},
	TreasureItemGenerated = {},
	BeforeCraftingExecuteCombination = {},
	AfterCraftingExecuteCombination = {},
	BeforeShootProjectile = {},
	ShootProjectile = {},
	ProjectileHit = {},
	GroundHit = {}
}

_I._OnDealDamage = function (...)
    return _I._Notify("OnDealDamage", ...)
end

_I._OnExecuteFunctor = function (...)
    return _I._Notify("OnExecuteFunctor", ...)
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

_I._ComputeCharacterHit = function (...)
    return _I._EngineCallback1("ComputeCharacterHit", ...)
end

_I._CalculateTurnOrder = function (...)
    return _I._EngineCallback1("CalculateTurnOrder", ...)
end

_I._StatusGetEnterChance = function (...)
    return _I._EngineCallback1("StatusGetEnterChance", ...)
end

_I._StatusHitEnter = function (...)
    return _I._Notify("StatusHitEnter", ...)
end

Ext.RegisterListener = function (type, fn)
	if _I._Listeners[type] ~= nil then
		table.insert(_I._Listeners[type], fn)
	else
		Ext.Utils.PrintError("Unknown listener type: " .. type)
	end
end

Ext.IsClient = function ()
	return false
end

Ext.IsServer = function ()
	return true
end

Ext._GetModPersistentVars = function (modTable)
	local tab = Mods[modTable]
	if tab ~= nil then
		local persistent = tab.PersistentVars
		if persistent ~= nil then
			return Ext.Json.Stringify(persistent)
		end
	end
end

_I._RestoreModPersistentVars = function (modTable, vars)
	local tab = Mods[modTable]
	if tab ~= nil then
		tab.PersistentVars = Ext.Json.Parse(vars)
	end
end

_I._BeforeCharacterApplyDamage = function (target, attacker, hit, causeType, impactDirection, context)
    for i,callback in pairs(_I._Listeners.BeforeCharacterApplyDamage) do
        local status, result = xpcall(callback, debug.traceback, target, attacker, hit, causeType, impactDirection, context)
        if not status then
            Ext.Utils.PrintError("Error during BeforeCharacterApplyDamage: ", result)
        end
    end

	return hit
end

_I._TreasureItemGenerated = function (...)
    return _I._EngineCallback1("TreasureItemGenerated", ...)
end

_I._BeforeCraftingExecuteCombination = function (...)
    for i,callback in pairs(_I._Listeners["BeforeCraftingExecuteCombination"]) do
        local status, result = xpcall(callback, debug.traceback, ...)
        if status then
			if result == true then
				return result
			end
		else
            Ext.Utils.PrintError("Error during BeforeCraftingExecuteCombination: ", result)
        end
    end
end

_I._AfterCraftingExecuteCombination = function (...)
    return _I._Notify("AfterCraftingExecuteCombination", ...)
end

_I._OnBeforeShootProjectile = function (...)
    return _I._Notify("BeforeShootProjectile", ...)
end

_I._OnShootProjectile = function (...)
    return _I._Notify("ShootProjectile", ...)
end

_I._OnProjectileHit = function (...)
    return _I._Notify("ProjectileHit", ...)
end

_I._OnGroundHit = function (...)
    return _I._Notify("GroundHit", ...)
end

_I._ExecutePropertyDataOnPosition = function (prop, ...)
	local propType = _I._SkillPropertyTypes[prop.Action]
	if propType ~= nil and propType.ExecuteOnPosition ~= nil then
		propType.ExecuteOnPosition(prop, ...)
	end
end

_I._ExecutePropertyDataOnTarget = function (prop, ...)
	local propType = _I._SkillPropertyTypes[prop.Action]
	if propType ~= nil and propType.ExecuteOnTarget ~= nil then
		propType.ExecuteOnTarget(prop, ...)
	end
end
