local _I = Ext._Internal

_I._PublishedEvents = {
	"ModuleLoadStarted",
	"ModuleLoading",
	"StatsLoaded",
	"ModuleResume",
	"SessionLoading",
	"SessionLoaded",
	"ResetCompleted",
	"GameStateChanged",
	"Tick",
	"DoConsoleCommand",

	"DealDamage",
	"ExecuteFunctor",
	"AfterExecuteFunctor"
}

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

_I._DoStartup()

-- Test runner helper
if Ext.Utils.IsDeveloperMode() then
	Ext.Utils.LoadTestLibrary()
	Ext.Events.GameStateChanged:Subscribe(function (e)
		if e.ToState == "Running" then
			local args = Ext.Utils.GetCommandLineParams()
			for i,arg in pairs(args) do
				if arg == "test" then
					Ext.Test.RunTests()
				end
			end
		end
	end)
end
