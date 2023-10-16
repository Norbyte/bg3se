---@diagnostic disable

return {
["Ext_Stats.Get"] = {
Before = "--- @return StatEntryType"
},
["Ext_ServerOsiris.RegisterListener"] = {
Replace = [[--- @param name string 
--- @param arity integer 
--- @param typeName OsirisEventType 
--- @param callback fun(...:string|number)
function Ext_ServerOsiris.RegisterListener(name, arity, typeName, callback) end
]]
},
}