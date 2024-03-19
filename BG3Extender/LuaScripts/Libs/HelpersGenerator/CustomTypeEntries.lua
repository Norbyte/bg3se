---@diagnostic disable

return {
Ext_Types = {
	After = [[
--- @class GenerateIdeHelpersOptions
--- @field AddAliasEnums boolean Add the enums in alias format, for string comparison. Defaults to true.
--- @field UseBaseExtraData boolean Only include the base ExtraData keys/values in Shared, instead of grabbing whatever the current keys are in the mod environment.
--- @field GenerateExtraDataAsClass boolean Annotate ExtraData as a class, so it only has fields with no fixed/hardcoded values.

--- Generate an ExtIdeHelpers file
--- @param outputPath string|nil Optional path to save the generated helper file, relative to the `%LOCALAPPDATA%\Larian Studios\Baldur's Gate 3\Script Extender` folder
--- @param opts GenerateIdeHelpersOptions|nil The optional settings to use
--- @return string fileContents Returns the file contents, for use with Ext.IO.SaveFile
function Ext_Types.GenerateIdeHelpers(outputPath, opts) end
]]},
Ext_Json = {
	After = [[
--- @class JsonStringifyOptions
--- @field Beautify boolean Sorts the output table, and indents with tabs. Defaults to true.
--- @field StringifyInternalTypes boolean Defaults to false.
--- @field IterateUserdata boolean Defaults to false.
--- @field AvoidRecursion boolean Defaults to false.
--- @field MaxDepth integer Defaults to 64, the maximum value.
--- @field LimitDepth integer Defaults to -1 (off).
--- @field LimitArrayElements integer Defaults to -1 (off).]]
},
}
