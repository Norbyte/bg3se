local _MissingFuncData = {
    Ext_Json = {
        Parse = {
            Params = {{
                name = "str",
                arg = "string",
                description = "String to convert to a table.",
            }},
            Return = {
                "table parsedTable"
            }
        },
        Stringify = {
            Override = true,
            Params = {
                {
                    name = "data",
                    arg = "table",
                    description = "Table to convert to a string.",
                },
                {
                    name = "opts",
                    arg = "JsonStringifyOptions?",
                    description = "Optional settings for the stringify operation",
                },
            },
            Return = {
                "string tableAsString"
            }
        },
    },
    Ext_Stats = {
        Get = {
            Params = {{
                name = "statType",
                arg = "StatType?",
                description = "Optional stat type to filter stats by.",
            }},
            Return = {
                "FixedString[]"
            }
        },
    },
    Ext_Utils = {
        Print = {
            Params = {{
                name = "...",
                arg = "any",
            }}
        },
        PrintError = {
            Params = {{
                name = "...",
                arg = "any",
            }}
        },
        PrintWarning = {
            Params = {{
                name = "...",
                arg = "any",
            }}
        },
        GetDifficulty = {
            Return = {"GameDifficultyValue difficulty"}
        },
        IsValidHandle = {
            Params = {{
                name = "handle",
                arg = "ComponentHandle",
            }}
        },
        GetHandleType = {
            Params = {{
                name = "handle",
                arg = "ComponentHandle",
            }}
        },
        GetValueType = {
            Params = {{
                name = "value",
                arg = "any",
            }},
            Return = {
                [[string|"nil"|"number"|"string"|"boolean"|"table"|"function"|"thread"|"userdata"|"lightuserdata" valueType # Returns a base type, lightuserdata, or an extender type.]]
            }
        },
        MakeHandleObject = {
            Params = {{
                name = "handle",
                arg = "ComponentHandle",
            }},
            Return = {
                "ComponentHandleProxy handleObject"
            }
        },
        Include = {
            Params = {
                {
                    name = "modGUID",
                    arg = "string?",
                    description = "The ModuleUUID value",
                },
                {
                    name = "path",
                    arg = "string",
                    description = "The path to the script, relative to the Lua folder",
                },
                {
                    name = "replaceGlobals",
                    arg = "table?",
                    description = "If set, the global environment of the script is replaced with this table",
                },
            },
            Return = {
                "any returnedValue Whatever the script returned, if anything"
            }
        }
    },
    Ext_Types = {
        GetObjectType = {
            Description = "--- Get the type name for any userdata, lightuserdata, or cppobject.",
            Params = {{
                name = "object",
                arg = "any",
                description = "Any userdata, lightuserdata, or cppobject type."
            }},
            Return = {
                "FixedString|\"userdata\"|\"lightuserdata\" objectType"
            }
        },
        
    }
}

local _MissingFuncFieldData = {
    Ext_StatsTreasureCategory = {
        GetLegacy = "fun(id:FixedString):StatTreasureCategory",
        Update = "fun(id:FixedString, tbl:StatTreasureCategory)",
    },
    Ext_StatsTreasureTable = {
        GetLegacy = "fun(id:FixedString):StatTreasureTable",
        Update = "fun(tbl:StatTreasureTable)",
    },
    Ext_Stats = {
        AddEnumerationValue = "fun(typeName:FixedString, enumLabel:FixedString):int32",
        AddRequirement = "fun(id:FixedString, overwriteBuiltIn:boolean?):CustomRequirementDescriptor",
    },
    Ext_Utils = {
        LoadString = "fun(str:string):UserReturn Similar to lua `loadstring`, with extra safeguards.",
    }
}

return {
    Regular = _MissingFuncData,
    Field = _MissingFuncFieldData,
}