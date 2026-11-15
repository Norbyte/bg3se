---@diagnostic disable

return {
Ext_Types = {
    After = [[
--- @class GenerateIdeHelpersOptions
--- @field AddAliasEnums boolean? Add the enums in alias format, for string comparison. Defaults to true.
--- @field UseBaseExtraData boolean? Only include the base ExtraData keys/values in Shared, instead of grabbing whatever the current keys are in the mod environment.
--- @field GenerateExtraDataAsClass boolean? Annotate ExtraData as a class, so it only has fields with no fixed/hardcoded values.

--- Generate an ExtIdeHelpers file
--- @param outputPath string? Optional path to save the generated helper file, relative to the `%LOCALAPPDATA%\Larian Studios\Baldur's Gate 3\Script Extender` folder
--- @param opts GenerateIdeHelpersOptions? The optional settings to use
--- @return string fileContents Returns the file contents, for use with Ext.IO.SaveFile
function Ext_Types.GenerateIdeHelpers(outputPath, opts) end
]]},

Ext_Json = {
    After = [[
--- @class JsonStringifyOptions
--- @field Beautify boolean? Sorts the output table, and indents with tabs. Defaults to true.
--- @field StringifyInternalTypes boolean? Defaults to false.
--- @field IterateUserdata boolean? Defaults to false.
--- @field AvoidRecursion boolean? Defaults to false.
--- @field Binary boolean? Produce a binary blob instead of JSON; parse it with `Ext.Json.Parse(str, true)`. Defaults to false.
--- @field MaxDepth integer? Defaults to 64, the maximum value.
--- @field LimitDepth integer? Defaults to -1 (off).
--- @field LimitArrayElements integer? Defaults to -1 (off).]]
},

Ext_Vars = {
    After = [[
--- @class UserVariableOptions
--- @field Server boolean? Variable is present on server entities. Defaults to true.
--- @field Client boolean? Variable is present on client entities. Defaults to false.
--- @field WriteableOnServer boolean? Variable can be modified on server side. Defaults to true.
--- @field WriteableOnClient boolean? Variable can be modified on client side. Defaults to false.
--- @field Persistent boolean? Variable is written to/restored from savegames. Defaults to true.
--- @field SyncToClient boolean? Server-side changes to the variable are synced to all clients. Defaults to false.
--- @field SyncToServer boolean? Client-side changes to the variable are synced to the server. Defaults to false.
--- @field SyncOnTick boolean? Client-server sync is performed once per game loop tick. Defaults to true.
--- @field SyncOnWrite boolean? Client-server sync is performed immediately when the variable is written. Defaults to false.
--- @field DontCache boolean? Disable Lua caching of variable values. Defaults to false.]]
},

ImguiHandle = {
    Replace = [[
--- @alias ImguiHandle ExtuiStyledRenderable
]]
},
}
