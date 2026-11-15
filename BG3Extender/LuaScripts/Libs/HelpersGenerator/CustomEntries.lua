---@diagnostic disable

local SubscribableEventType = [[--- @class SubscribableEvent<T>:{ (Subscribe:fun(self:SubscribableEvent, callback:fun(e:T|LuaEventBase), opts:{Priority:integer, Once:boolean}?):integer), (Unsubscribe:fun(self:SubscribableEvent, index:integer))}

--- Developer functions for the SubscribableEvent type. 
--- Throw can be used to manually throw the event, but special care may be needed to ensure the table used for the event data is valid.  
--- @class SubscribableEventDev<T>:{ (Throw:fun(self:SubscribableEvent, e:T|LuaEventBase))}
]]
return {
    Specific = {SubscribableEventType = SubscribableEventType},

    Builtins = {
[[
--- @alias FunctionRef function
]]
    },

    Misc = {
[[--#region Extender Functions / Globals

--- @alias NetListenerCallback fun(channel:string, payload:string, user:UserId?)

--- Registers a listener that is called when a network message is received on the specified channel
--- @param channel string Network channel name
--- @param handler NetListenerCallback Lua handler
function Ext.RegisterNetListener(channel, handler) end

--- Loads the specified Lua file; each file is only executed once, subsequent calls return the values returned by the first load
--- @param mod string Path of Lua file relative to Mods/<Mod>/ScriptExtender/Lua of the current mod, or the UUID of the mod to load from if `path` is specified
--- @param path string? Path of Lua file, relative to Mods/<Mod>/ScriptExtender/Lua of the mod specified by `mod`
--- @see Ext_Utils#Include
--- @return any ...
function Ext.Require(mod, path) end

--- Returns whether the code is executing in a client context
--- @return boolean
function Ext.IsClient() end

--- Returns whether the code is executing in a server context
--- @return boolean
function Ext.IsServer() end

--- Convenience helper for Ext.Dump
_D = Ext.Dump

--- Convenience helper for Ext.DumpShallow
_DS = Ext.DumpShallow

--- Convenience helper for Ext.Log.Print
_P = Ext.Log.Print

--- Convenience helper for Ext.Log.PrintWarning
_PW = Ext.Log.PrintWarning

--- Convenience helper for Ext.Log.PrintError
_PE = Ext.Log.PrintError

--- Console window helper to get current player character 
--- This is the host on the server, or the hotbar character on the client  
--- @return EntityHandle
_C = function() end

--- Console window helper to get character being examined on the client-side  
--- @return EntityHandle
_E = function() end

--- Console window helper to get the host's equipped weapon on the server-side  
--- @return EntityHandle
_W = function() end

--- Helper for dumping variables to the console  
--- This is essentially `Ext.Log.Print(Ext.DumpExport(val))`  
--- @param val any
function Ext.Dump(val) end

--- Helper for dumping variables to the console, only expanding the first level of tables/objects and the first 3 elements of arrays
--- @param val any
function Ext.DumpShallow(val) end

--- Helper for dumping variables to a string
--- @param val any
--- @return string
function Ext.DumpExport(val) end

--- Register a callback that runs on the next tick, and is then unregistered afterwards  
--- @param callback fun(e:LuaTickEventParams)
function Ext.OnNextTick(callback) end

--- Registers a listener that is called when a console command is entered in the dev console
--- @param cmd string Console command
--- @param handler fun(cmd:string, ...:string)
function Ext.RegisterConsoleCommand(cmd, handler) end

--- 🔨**Server-Only**🔨
--- @class Ext_ServerOsiris
local Ext_ServerOsiris = {}

--- Registers a listener that is called when the specified Osiris event, call or query is executed
--- @param name string Name of the Osiris function/event
--- @param arity integer Number of parameters of the Osiris function/event
--- @param typeName string When the listener is triggered; 'before', 'after', 'beforeDelete' or 'afterDelete'
--- @param callback fun(...:string|number)
--- @return integer subscriberId ID that can be passed to UnregisterListener
function Ext_ServerOsiris.RegisterListener(name, arity, typeName, callback) end

--- Removes a listener registered with RegisterListener
--- @param subscriberId integer
--- @return boolean removed
function Ext_ServerOsiris.UnregisterListener(subscriberId) end

Ext.Osiris = Ext_ServerOsiris

--#endregion
]],
},

    Entity = [[
--- @class EntityHandle
--- @field CreateComponent fun(self:EntityHandle, type:ExtComponentType):BaseComponent
--- @field CreateComponentImmediate fun(self:EntityHandle, type:ExtComponentType):BaseComponent
--- @field RemoveComponent fun(self:EntityHandle, type:ExtComponentType):boolean
--- @field RemoveComponentImmediate fun(self:EntityHandle, type:ExtComponentType):boolean
--- @field MarkChanged fun(self:EntityHandle, type:ExtComponentType)
--- @field WasChanged fun(self:EntityHandle, type:ExtComponentType)
--- @field WasAdded fun(self:EntityHandle, type:ExtComponentType)
--- @field WasRemoved fun(self:EntityHandle, type:ExtComponentType)
--- @field WasEntityAdded fun(self:EntityHandle)
--- @field WasEntityRemoved fun(self:EntityHandle)
--- @field GetComponent fun(self:EntityHandle, type:ExtComponentType):BaseComponent
--- @field HasRawComponent fun(self:EntityHandle, type:string):boolean
--- @field GetAllComponents fun(self:EntityHandle, warnOnMissing:boolean?):table<ExtComponentType,BaseComponent>
--- @field GetAllComponentNames fun(self:EntityHandle, requireMapped:boolean?):table<number,string>
--- @field IsAlive fun(self:EntityHandle):boolean
--- @field GetNetId fun(self:EntityHandle):number?
--- @field GetReplicationFlags fun(self:EntityHandle, type:ExtComponentType, qword:number?):number
--- @field SetReplicationFlags fun(self:EntityHandle, type:ExtComponentType, flags:number, qword:number?)
--- @field Replicate fun(self:EntityHandle, type:ExtComponentType)
--- @field OnCreate fun(self:EntityHandle, type:ExtComponentType, callback:FunctionRef, deferred:boolean?, once: boolean?):number
--- @field OnCreateDeferred fun(self:EntityHandle, type:ExtComponentType, callback:FunctionRef):number
--- @field OnCreateOnce fun(self:EntityHandle, type:ExtComponentType, callback:FunctionRef):number
--- @field OnCreateDeferredOnce fun(self:EntityHandle, type:ExtComponentType, callback:FunctionRef):number
--- @field OnDestroy fun(self:EntityHandle, type:ExtComponentType, callback:FunctionRef, deferred:boolean?, once: boolean?):number
--- @field OnDestroyDeferred fun(self:EntityHandle, type:ExtComponentType, callback:FunctionRef):number
--- @field OnDestroyOnce fun(self:EntityHandle, type:ExtComponentType, callback:FunctionRef):number
--- @field OnDestroyDeferredOnce fun(self:EntityHandle, type:ExtComponentType, callback:FunctionRef):number
--- @field OnChanged fun(self:EntityHandle, type:ExtComponentType, callback:FunctionRef, flags:number?):number?
--- @field Vars table User variables registered using Ext.Vars]]
}
