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
	"DoConsoleCommand"
}

Ext.IsClient = function ()
	return true
end

Ext.IsServer = function ()
	return false
end

_I._DoStartup()
