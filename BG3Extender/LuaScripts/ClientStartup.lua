Ext.Utils.Include(nil, "builtin://BuiltinLibrary.lua")
Ext.Utils.Include(nil, "builtin://Libs/Event.lua")
Ext.Utils.Include(nil, "builtin://Libs/Stats.lua")
Ext.Utils.Include(nil, "builtin://BuiltinLibraryClient.lua")

-- Test runner helper
if Ext.Debug.IsDeveloperMode() then
	Ext.Utils.LoadTestLibrary()
	Ext.Utils.Include(nil, "builtin://Libs/DevelopmentHelpers.lua")
end
