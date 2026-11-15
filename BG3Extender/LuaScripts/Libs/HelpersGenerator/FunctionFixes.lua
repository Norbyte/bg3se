local _MissingFuncData = {
    Ext_Json = {
        Parse = {
            Params = {
                {
                    name = "str",
                    arg = "string",
                    description = "String to parse.",
                },
                {
                    name = "binary",
                    arg = "boolean?",
                    description = "Parse a blob produced by Stringify with the Binary option instead of JSON.",
                },
            },
            Return = {
                "any parsedValue"
            }
        },
    },
    Ext_Stats = {
        Get = {
            Params = {
                {
                    name = "statName",
                    arg = "string",
                },
                {
                    name = "level",
                    arg = "int32?",
                },
                {
                    name = "warnOnError",
                    arg = "boolean?",
                },
            },
        },
    },
    Ext_Utils = {
        IsValidHandle = {
            Params = {{
                name = "handle",
                arg = "any",
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
        },

    }
}

local _MathValue = "number|vec3|vec4|mat3|mat4"
local _MathVector = "vec3|vec4"
local _MathMatrix = "mat3|mat4"

-- Signature of math functions that write their result into the `out` param (and return nothing) when it is passed
local function _MathOutOverloads(params, out, ret)
    return "(fun(" .. params .. ", out:" .. out .. "))|(fun(" .. params .. "):" .. ret .. ")"
end

local _MissingFuncFieldData = {
    Ext_Debug = {
        DumpStack = "fun(...:any)",
    },
    Ext_Entity = {
        Get = "fun(entity:Guid|NetId|EntityHandle?):EntityHandle?",
    },
    Ext_Json = {
        Stringify = "(fun(data:any, opts:JsonStringifyOptions?):string)|(fun(data:any, beautify:boolean?, stringifyInternalTypes:boolean?, iterateUserdata:boolean?):string)",
    },
    Ext_Log = {
        Print = "fun(...:any)",
        PrintError = "fun(...:any)",
        PrintWarning = "fun(...:any)",
    },
    Ext_Math = {
        Add = _MathOutOverloads("a:" .. _MathValue .. ", b:" .. _MathValue, "vec3|vec4|mat3|mat4", _MathValue),
        Sub = _MathOutOverloads("a:" .. _MathValue .. ", b:" .. _MathValue, "vec3|vec4|mat3|mat4", _MathValue),
        Mul = _MathOutOverloads("a:" .. _MathValue .. ", b:" .. _MathValue, "vec3|vec4|mat3|mat4", _MathValue),
        Div = _MathOutOverloads("a:" .. _MathValue .. ", b:" .. _MathValue, "vec3|vec4|mat3|mat4", _MathValue),
        Reflect = _MathOutOverloads("i:" .. _MathVector .. ", n:" .. _MathVector, _MathVector, _MathVector),
        Angle = "fun(a:" .. _MathVector .. ", b:" .. _MathVector .. "):number",
        OuterProduct = _MathOutOverloads("c:" .. _MathVector .. ", r:" .. _MathVector, _MathMatrix, _MathMatrix),
        Perpendicular = _MathOutOverloads("x:" .. _MathVector .. ", normal:" .. _MathVector, _MathVector, _MathVector),
        Project = _MathOutOverloads("x:" .. _MathVector .. ", normal:" .. _MathVector, _MathVector, _MathVector),
        Cross = _MathOutOverloads("x:vec3, y:vec3", "vec3", "vec3"),
        Length = "fun(x:number|" .. _MathVector .. "):number",
        Normalize = _MathOutOverloads("x:" .. _MathVector, _MathVector, _MathVector),
        Determinant = "fun(m:" .. _MathMatrix .. "):number",
        Inverse = _MathOutOverloads("m:" .. _MathMatrix, _MathMatrix, _MathMatrix),
        Transpose = _MathOutOverloads("m:" .. _MathMatrix, _MathMatrix, _MathMatrix),
        Rotate = "fun(m:" .. _MathMatrix .. ", angle:number, axis:vec3) Rotates `m` in place.",
        QuatRotate = "fun(q:quat, v:" .. _MathVector .. "):" .. _MathVector,
        QuatMul = "fun(a:quat|vec3, b:quat|vec3):quat|vec3",
        ExtractEulerAngles = "fun(m:" .. _MathMatrix .. "):vec3",
        ExtractAxisAngle = "fun(m:" .. _MathMatrix .. ", axis:vec3):number Returns the rotation angle; the rotation axis is written into `axis`.",
        Random = "fun(m:integer?, n:integer?):number Without arguments, returns a float in [0, 1). With `m` only, returns an integer in [1, m]. With both, returns an integer in [m, n].",
    },
    Ext_StaticData = {
        Create = "fun(type:ExtResourceManagerType, guid:Guid?):ResourceGuidResource?",
        Get = "fun(guid:Guid, type:ExtResourceManagerType):ResourceGuidResource?",
    },
    Ext_StatsTreasureCategory = {
        GetLegacy = "fun(id:FixedString):table?",
        Update = "fun(id:FixedString, tbl:table)",
    },
    Ext_StatsTreasureTable = {
        GetLegacy = "fun(id:FixedString):table?",
        Update = "fun(tbl:table)",
    },
    Ext_Stats = {
        AddEnumerationValue = "fun(typeName:FixedString, enumLabel:FixedString):int32?",
    },
    Ext_Table = {
        Find = "fun(tbl:table?, value:any):any Returns the key of `value` in `tbl`, or nil if not found.",
    },
    Ext_Types = {
        GetFunctionLocation = "fun(func:function):string, integer",
        Serialize = "fun(object:any):table",
        Unserialize = "fun(object:any, data:table)",
        Validate = "fun(object:any):boolean",
    },
    Ext_Utils = {
        GetGameState = "fun():ClientGameState|ServerGameState|nil",
        LoadString = "fun(str:string, env:table?):function?, string? Similar to lua `loadstring`, with extra safeguards. Returns nil and an error message if the string fails to compile.",
    },
    Ext_Vars = {
        GetModVariables = "fun(modUuid:Guid):table",
        RegisterModVariable = "fun(modUuid:Guid, name:FixedString, opts:UserVariableOptions)",
        RegisterUserVariable = "fun(name:FixedString, opts:UserVariableOptions)",
    },
}

return {
    Regular = _MissingFuncData,
    Field = _MissingFuncFieldData,
}
