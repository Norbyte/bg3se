### BG3SE Lua API v1 Documentation

### Table of Contents  

 - [Getting Started](#getting-started)
 - [Early Access Notes](#ea-notes)
 - [Differences between the DOS2 and BG3 Extenders](#dos2-migration)
 - [Client / Server States](#client-server)
    - [Persistent Variables](#persistent-vars)
 - [Console](#console)
 - [Object Identifiers](#object-identifiers)
 - [Calling Osiris from Lua](#calling-osiris-from-lua)
    * [Calls](#o2l_calls)
    * [Queries](#o2l_queries)
    * [Events](#o2l_events)
    * [PROCs](#o2l_procs)
    * [User Queries](#o2l_qrys)
    * [Databases](#o2l_dbs)
 - [Calling Lua from Osiris](#calling-lua-from-osiris)
    * [Calls](#l2o_calls)
    * [Queries](#l2o_queries)
    * [Events](#l2o_events)
    * [Custom Calls](#l2o_custom_calls)
    * [Capturing Events/Calls](#l2o_captures)
 - [Stats](#stats)
    * [Stats Objects](#stats-objects)
    * [Custom Spell Properties](#stats-custom-spell-properties)
 - [ECS](#ecs)
 - [Mod Info](#mod-info) 
<!--- 
 - [Server Objects](#server-objects)
    * [Characters](#server-characters)
    * [Player Custom Data](#player-custom-data)
    * [Items](#server-items)
    * [Projectiles](#server-projectiles)
    * [Surfaces](#server-surfaces)
    * [Statuses](#server-statuses)
    * [Combat](#server-combat)
 - [Surface Actions](#surface-actions)
 - [Root Templates](#root-templates)
     * [Surface](#surface-templates)
 - [Damage lists](#damage-lists) 
-->
 - [Utility functions](#ext-utility)
 - [JSON Support](#json-support)
 - [Engine Events](#engine-events)
    * [Load Events](#event-load-events)
 - [Osiris Preprocessor](#osiris-preprocessor)
 - [Osiris Data Types](#osiris-data-types)
 - [Osiris Functions](#osiris-functions)
   * [Stats](#osi-stat-functions)
   * [Math Functions](#osi-math-functions)
   * [String Functions](#osi-string-functions)
   * [Misc Functions](#osi-misc-functions)



<a id="getting-started"></a>
## Getting Started

To start using the Script Extender in your mod, a configuration file must be created that describes what features are utilized by your mod.

Create a file at `Mods\YourMod_11111111-2222-...\ScriptExtender\Config.json` with the following contents, then tweak the values as desired:
```json
{
    "RequiredVersion": 1,
    "ModTable": "YOUR_MOD_NAME_HERE",
    "FeatureFlags": [
        "Osiris",
        "Lua"
    ]
}
```

Meaning of configuration keys:

| Key | Meaning |
|--|--|
| `RequiredVersion` | Osiris Extender version required to run the mod. It is recommended to use the version number of the Script Extender you used for developing the mod since the behavior of new features and backwards compatibility functions depends on this version number. |
| `ModTable` | Name of the mod in the global mod table (`Mods`) when using Lua. |
| `FeatureFlags` | A list of features that the mod is using. For performance reasons it is recommended to only list features that are actually in use. |

The following features are accepted in `FeatureFlags`:

| Value| Meaning |
|--|--|
| `Lua` | Enables Lua scripting |
| `Osiris` | Enables Osiris (Story) extension functions (see [Osiris Functions](#osiris-functions)) |
| `Preprocessor` | Enables the use of preprocessor definitions in Story scripts. (See [Osiris Preprocessor](#osiris-preprocessor)) |


<a id="ea-notes"></a>
## Early Access Notes

**The Script Extender itself is Early Access!** Many of the features are still in development and are either unavailable or not completely functional. The APIs used to access may change due to redesign or due to changes in the game itself.

**Story Scripting:** Osiris scripting is currently not functional in BG3 because the original story scripts are not available, so it is not possible to compile story code. The Script Extender allows calling Osiris functions without the need to write story scripts, however mods cannot write story code. The game will most likely include raw story files when the full game is released and story scripting will become possible.

**Behavior Scripting:** Behavior scripts seem to be deprecated in BG3 in favor of Lua scripting; therefore no support will be provided for interacting with behavior scripts.

Editing object properties note!

<a id="dos2-migration"></a>
## Differences between the DOS2 and BG3 Extenders

### WIP
### TODO add bg3 general differences:
 - ECS and raw component data exposed 
 - Component editing rules / replication triggers
 - Userdata get/set/iteration support and considerations
 - Lifetime/Scope binding for by-ptr userdata; entity/comp handle persistence
 - ...


<a id="client-server"></a>
## Client and Server Roles

The game is split into two components, a client and a server. When a game is started/loaded, the in-game server is started and the internal client connects to this server. In multiplayer games server component is only present on the host; client components are created on both the host and all players connecting to the host. Because of this, the game technically always runs in multiplayer. Single player is just a special form of multiplayer where only one local player is connected to the server.

Osiris, behavior scripts (gamescripts/itemscripts/characterscripts) and Anubis Lua scripts always run on the server. Since the Script Extender Lua API has access to features that require client-side code (**[[TODO not up to date]]** UI modification, level scaling formulas, status chances, skill damage calculation, etc.) the extender keeps multiple Lua states: one state for the server and one for each client (including the single player "fake client"). These states are completely independent from each other and cannot access the internal state (functions, variables, etc.) of each other.

Because they run in different environments, server and client states have access to a different set of features. Functions/classes in this document are annotated with the following letters, which indicate where they are available:
 - **C** - The function is only available on the client
 - **S** - The function is only available on the server
 - **R** - Restricted; the function is only callable in special contexts/locations

<a id="persistent-vars"></a>
### Persistent Variables

The Lua state and all local variables are reset after each game reload. To carry over data to subsequent play sessions it is possible to store them in the savegame by saving them in the mod-local table `Mods[ModTable].PersistentVars`. (By default this table is uninitialized, i.e. `nil`.) It is the responsibility of the mod to create the table and populate it with data it wishes to store in the savegame. The contents of `PersistentVars` is saved when a savegame is created, and restored before the `SessionLoaded` event is triggered. 

The serialization restrictions for `PersistentVars` are equivalent to the restrictions for `Ext.Json.Stringify` (i.e. no userdata/function support, no recursive structures, no custom metatables, etc). If `PersistentVars` cannot be serialized, it will not be written to the savegame!

Example:
```lua
-- NOTE: The PersistentVars table is not declared as "local". 
-- This is because the global table (_G) of a mod is the "Mods[ModTable]" table.
PersistentVars = {}
[...]
-- Variable will be restored after the savegame finished loading
function doStuff()
    PersistentVars['Test'] = 'Something to keep'
end

local function OnSessionLoaded()
    -- Persistent variables are available only after SessionLoaded is triggered!
    Ext.Utils.Print(PersistentVars['Test'])
end

Ext.RegisterListener("SessionLoaded", OnSessionLoaded)
```


## Console

The Extender allows commands to be entered to the console window.

Press `<enter>` to enter input mode; in this mode the normal log output is disabled to avoid log spam while typing commands.

Client/server Lua contexts can be selected by typing `client` or `server`. This selects which Lua environment the console commands will execute in. By default the console uses the server context.
The `reset` command reinitializes the server and client Lua VM; `reset client` only reinitializes the client VM, while `reset server` only reinitializes the server VM.

Typing `exit` returns to log mode.

Commands prefixed by a `!` will trigger callbacks registered via the `RegisterConsoleCommand` function.
Example:
```lua
local function testCmd(cmd, a1, a2, ...)
    Ext.Utils.Print("Cmd: " .. cmd .. ", args: ", a1, ", ", a2);
end
Ext.RegisterConsoleCommand("test", testCmd);
```
The command `!test 123 456` will call `testCmd("test", 123, 456)` and prints `Cmd: test, args: 123, 456`.

Anything else typed in the console will be executed as Lua code in the current context. (eg. typing `Ext.Utils.Print(1234)` will print `123`). 
The console has full access to the underlying Lua state, i.e. server console commands can also call builtin/custom Osiris functions, so Osiris calls like `CharacterGiveEquipmentSet(GetHostCharacter(), "EQP_CC_Cleric")` are possible using the console.
Console commands are executed in the global table, so mod-specific variables must be accessed through `Mods[ModTable].whatever`, where `ModTable` is the value of the config variable specified in `ScriptExtender\Config.json`.
Variables can be used just like in Lua, i.e. variable in one command can later on be used in another console command. Be careful, console code runs in global context, so make sure console variable names don't conflict with globals (i.e. `Mods`, `Ext`, etc.)! Don't use `local` for console variables, since the lifetime of the local will be one console command. (Each console command is a separate chunk in Lua terms).

<a id="object-identifiers"></a>
## Object Identifiers

Game objects have multiple identifiers that are used for different purposes.

### GUID

The UUID or GUID (Globally Unique IDentifier) is a unique textual identifier (e.g. `123e4567-e89b-12d3-a456-426614174000`). It can reference any character or item. 

Use GUIDs to hardcode references to pre-made characters/items in scripts or to reference newly created characters/items on the server.

Usage Notes:
 - For objects that are created in the editor, the GUID is guaranteed to stay the same in all playthroughs, i.e. it is safe to hardcode the GUID in scripts. This is the identifier Osiris functions use (using the types `GUIDSTRING`, `ITEM`, etc.)
 - Objects that are created during the game are assigned a randomly generated GUID. This GUID will not change during the lifetime of the object, i.e. it's safe to store it to reference the object later on.
 - Some object types (eg. projectiles, statuses, etc.) have no GUID, so they must be referenced using their object handle or NetID. Usually if an object is global (i.e. can appear anywhere in any context), it has a GUID. If it is local (i.e. it can only be assigned to a specific character etc.) it won't have a GUID.
 - GUIDs should not be used to reference objects on the client side (it is safe to use GUIDs on the server), as there are bugs on the client that cause the GUID to not replicate properly or it may be missing entirely. Use object handles (in client-only code) or NetID-s (when communicating with the server) to reference client objects instead. **[[TODO check if this is still bugged in BG3]]**


### Object/Entity Handle

Object handles are used by the game engine internally; they're 64-bit numbers. They're used for performance reasons, i.e. it's significantly faster for the engine to find objects using a handle than by a GUID (it's also smaller). Most references in the savegame are also handles, not GUIDs. eg. the parent inventory, items stored in a container, etc. are all saved using their handle, not their GUID.

Since everything has an object handle, you can use it to reference any object on both the server and the client.

Usage Notes:
 - This is a very commonly used identifier in Lua; most Lua game objects refer to others using object handles.
 - Each object is assigned a new handle when it is created. Unlike GUIDs, handles for objects that are created in the editor will _not_ be the same in different playthroughs, so handles cannot be hardcoded in scripts.
 - Unlike GUIDs, the client and server use different handles to reference the same object. (Technically, client and server characters are different objects altogether.)
 - After an object was created, the _server_ handle will not change during the lifetime of the object, i.e. it's safe to store it to reference the object later on. If a savegame is reloaded, the server handle will stay the same. **[[TODO note about handle -> GUID translation in BG3]]**
 - Client handles can change between play sessions (i.e. after a savegame reload), but they'll remain the same during the play session. They can safely be kept in temporary structures (eg. Lua variables that get reset after a savegame load), but should not be persisted in Osiris databases or via `PersistentVariables`.

### NetID

The NetID (Network Identifier) is a number the game uses in client-server communication to reference objects. Since object handles differ on the server and the client and not every object type has a GUID, NetID is the only identifier that can be reliably used to identify objects on both sides. **[[TODO check if client GUID is fixed in BG3]]**

Usage:
 - Each object is assigned a new NetID at the start of every play session. If a savegame is reloaded, the NetID may change.
 - Unlike object handles, both the server and the client use the same NetID to reference the same object.
 - Since they're only valid for the duration of the session, they can safely be kept in temporary structures (eg. Lua variables that get reset after a savegame load), but should not be persisted in Osiris databases or via `PersistentVariables`.


### Identifier Matrix

This table describes which identifiers are present/can be used for which object.

| Object | GUID | Object Handle | NetID |
|--|--|--|--|
| Server Character | ✔ | ✔ | ✔ |
| Server Item | ✔ | ✔ | ✔ |
| Server Projectile | | ✔ | ✔ |
| Server Status | | ✔ | ✔ |
| Server Surface | | ✔ | ✔ |
| Server Surface Action | | ✔ | |
| Server Game Action | | ✔ | |
| Client Character | * | ✔ | ✔ |
| Client Item | * | ✔ | ✔ |
| Client Projectile | | ✔ | ✔ |
| Client Status | | ✔ | ✔ |

\* Although client characters/items have a GUID, it cannot be used reliably.


<a id="calling-osiris-from-lua"></a>
## Calling Osiris from Lua <sup>S</sup>

Lua server contexts have a special global table called `Osi` that contains every Osiris function, event and database. In addition, built-in functions (calls, queries, events), functions added by the Osiris extender and functions registered from Lua via `Ext.Osiris.NewCall`, `Ext.Osiris.NewQuery` and `Ext.Osiris.NewEvent` are also added to the global table.

<a id="l2o_types"></a>
### Types

The following table describes how Lua values are converted to Osiris values and vice versa.

| Lua Type | Osiris Type | Notes |
|--|--|--|
| `nil` | - | `nil` is not convertible to an Osiris value, however it has a special meaning when calling/returning from queries (see [Queries](#l2o_queries)). |
| `boolean` | `INTEGER` | Osiris has no true boolean type, it uses the integer value 1 to represent `true` and 0 to represent `false`. |
| `number` (integer) | `INTEGER` | Although Lua only has one `number` type, its internal representation can vary depending on whether it stores an integer or floating point number. |
| `number` (float) | `REAL` | |
| `string` | `STRING` | Any `STRING` alias (eg. `GUIDSTRING`, `CHARACTER`, `ITEM`, ...) is also convertible to string. |
| `table` | - | Osiris only supports scalar values, so tables cannot be passed to/from Osiris functions. |


<a id="o2l_calls"></a>
### Calls

Simply call the method from Lua with the same parameters:
```lua
-- Built-in functions are in the global table (_G)
CharacterResetCooldowns(player)
-- Equivalent to the above
Osi.CharacterResetCooldowns(player)
```

*Implementation detail:* Name resolution is only performed when the function is called since Osiris allows multiple overloads of the same name and the function is resolved based on the number of arguments. Because of this, getting any key from the `Osi` table will return a proxy object even if no function with that name exists. Therefore, `Osi.Something ~= nil` and similar checks cannot be used to determine whether a given Osiris symbol exists.

<a id="o2l_queries"></a>
### Queries

The query behavior is a mirror of the one described in the [Custom calls/queries](#l2o_custom_calls) chapter.

For queries with zero OUT arguments, the function will return a boolean indicating whether the query succeeded (true) or failed (false).
```lua
local succeeded = SysIsCompleted("TestGoal")
```

Queries with OUT arguments will have a number of return values corresponding to the number of OUT arguments. 
```lua
-- Single return value
local player = CharacterGetHostCharacter()
-- Multiple return values
local x, y, z = GetPosition(player)
```

<a id="o2l_events"></a>
### Events

Osiris events can be triggered by calling them like a function. Events are not buffered and the event is triggered synchronously, i.e. the function call returns when every Osiris rule that handles the event has finished.
```lua
StoryEvent(player, "event name")
```

<a id="o2l_procs"></a>
### PROCs

Calling PROCs is equivalent to built-in calls, however they are not added to the global table.

```lua
Osi.Proc_CharacterFullRestore(player)
```

<a id="o2l_qrys"></a>
### User Queries

User queries (`QRY`) behave just like built-in queries do. Since they can't have OUT arguments (i.e. can't return values), the function will just return a boolean indicating whether the query succeeded or not. User queries are not added to the global table.

```lua
local succeeded = Osi.Qry_IsHealingStatus("DAMAGE")
```

<a id="o2l_dbs"></a>
### Databases

Databases can be read using the `Get` method. The method checks its parameters against the database and only returns rows that match the query.

The number of parameters passed to `Get` must be equivalent to the number of columns in the target database. Each parameter defines an (optional) filter on the corresponding column; if the parameter is `nil`, the column is not filtered (equivalent to passing `_` in Osiris). If the parameter is not `nil`, only rows with matching values will be returned.

Example:
```lua
-- Fetch all rows from DB_GiveTemplateFromNpcToPlayerDialogEvent
local rows = Osi.DB_GiveTemplateFromNpcToPlayerDialogEvent:Get(nil, nil, nil)

-- Fetch rows where the first column is CON_Drink_Cup_A_Tea_080d0e93-12e0-481f-9a71-f0e84ac4d5a9
local rows = Osi.DB_GiveTemplateFromNpcToPlayerDialogEvent:Get("CON_Drink_Cup_A_Tea_080d0e93-12e0-481f-9a71-f0e84ac4d5a9", nil, nil)
```

It is possible to insert new tuples to Osiris databases by calling the DB like a function.

```lua
Osi.DB_CharacterAllCrimesDisabled(player)
```

The `Delete` method can be used to delete rows from databases. The number of parameters passed to `Delete` must be equivalent to the number of columns in the target database.
Each parameter defines an (optional) filter on the corresponding column; if the parameter is `nil`, the column is not filtered (equivalent to passing `_` in Osiris). If the parameter is not `nil`, only rows with matching values will be deleted.
Example:
```lua
-- Delete all rows from DB_GiveTemplateFromNpcToPlayerDialogEvent
Osi.DB_GiveTemplateFromNpcToPlayerDialogEvent:Delete(nil, nil, nil)

-- Delete rows where the first column is CON_Drink_Cup_A_Tea_080d0e93-12e0-481f-9a71-f0e84ac4d5a9
Osi.DB_GiveTemplateFromNpcToPlayerDialogEvent:Delete("CON_Drink_Cup_A_Tea_080d0e93-12e0-481f-9a71-f0e84ac4d5a9", nil, nil)
```

<a id="calling-lua-from-osiris"></a>
## Calling Lua from Osiris <sup>S</sup>

Functions defined in Lua are not visible from Osiris. To make a Lua function (call/query/event) callable from Osiris, it must first be exported to Osiris using `Ext.Osiris.NewCall`, `NewQuery` or `NewEvent`.

*Technical detail:* During the Lua server bootstrap process, it is possible to declare new functions (calls/queries/events) that will be accessible to Osiris during compilation and execution. Since Osiris only runs on the server, Osiris functions are inaccessible on the client.

Lua functions are registered through the story header (`story_header.div`). This means that each time a function is added, changed or removed, the story header must be regenerated when using the editor. (The game regenerates its own story header, so it is always up to date.)


<a id="l2o_calls"></a>
### Calls

Calls can be registered using the `Ext.NewCall(name, parameters)` function. The first parameter is the name of the call to create. The second parameter is the argument list of the Osiris call; it should follow the same syntax that the Osiris story header uses.

It is strongly recommended to follow the Osiris naming scheme, i.e. the name of calls should start with the name prefix of your mod.

Examples:
```lua
-- Call with a single argument
local function TestLog(msg)
    print(msg)
end
Ext.Osiris.NewCall(TestLog, "NRD_EXT_TestLog", "(STRING)_Msg");

-- Call with multiple arguments
local function Multiply(a, b)
    print(a * b)
end
Ext.Osiris.NewCall(Multiply, "NRD_EXT_Multiply", "(REAL)_A, (REAL)_B");
```

Functions exported from Lua can be called in Osiris just like any other call:
```c
IF
[...]
THEN
NRD_EXT_TestLog("Test");
NRD_EXT_Multiply(10, 5);
```

<a id="l2o_queries"></a>
### Queries

Unlike `QRY`s defined in Osiris code, Lua queries can return values just like the built-in queries.
Queries can be registered using the `Ext.Osiris.NewQuery(name, parameters)` function. The first parameter is the name of the query. The second parameter is the argument list of the Osiris query; it should follow the same syntax that the Osiris story header uses.

Queries have two outcomes: they can either succeed or fail. A successful query returns a value for all of its `out` arguments; a failed query doesn't return any values.
To indicate whether a query succeeded or failed, Lua uses the following mechanism:
 - For 0 `out` parameters (i.e. when the query returns no values) the function should return `true` when it succeeded and `false` when it failed.
 - For N (1 or more) `out` parameters the function should return N non-`nil` values when it succeeded and N `nil` values when it failed. It is not permitted to return a mixture of `nil` and non-`nil` values.

The following table summarizes the expected return values:

| Number of params | Result | Return values |
|--|--|--|
| 0 | Successful | `true` |
| 0 | Failed | `false` |
| 1 | Successful | non-`nil` return value |
| 1 | Failed | `nil` |
| 3 | Successful | 3 non-`nil` return values - (v1, v2, v3) |
| 3 | Failed | `nil, nil, nil` |

Example:
```lua
local function Divide(a, b)
    if b == 0 then
        return nil
    end
    return a / b
end
Ext.Osiris.NewQuery(Divide, "NRD_EXT_Divide", "[in](REAL)_A, [in](REAL)_B, [out](REAL)_Result");
```

Functions exported from Lua can be called in Osiris just like any other call:
```c
IF
[...]
AND
NRD_EXT_Divide(100, 5, _Result)
THEN
[...]
```

<a id="l2o_events"></a>
### Events

New Osiris events can be created by calling `Ext.NewEvent(name, parameters)`. The first parameter is the name of the event. The second parameter is the argument list of the Osiris event; it should follow the same syntax that the Osiris story header uses.

```lua
Ext.Osiris.NewEvent("NRD_EXT_TestEvent", "(STRING)_Name");
```

Custom events can be thrown by calling them like a function:
```LUA
NRD_EXT_TestEvent("Whatever");
```

<a id="l2o_custom_calls"></a>
### Custom calls/queries

It is possible to call Lua functions by name, without exporting them to the Osiris story header. For this purpose multiple polymorphic functions are provided, `NRD_ModCall*(Mod, Func, ...)` and `NRD_ModQuery*(Mod, Func, ...)`.

`NRD_ModCall(Mod, Func, ...)` is a call (i.e. usable from the `THEN` part of the rule) and returns no results. Its first parameters are the mod name in the global table (i.e. the `ModTable` setting from `ScriptExtender/Config.json`) and the function name to call, and it accepts an arbitrary number of arguments to pass to the Lua function. It calls the function `Mods[Mod].Func(...)` in Lua.

```lua
function TestFunc()
    Ext.Utils.Print("Woohoo!")
end

-- 2-arg version
function TestFunc2(str, int)
    Ext.Utils.Print(str .. ", " .. int)
end
```

```c
NRD_ModCall("YourMod", "TestFunc");

// Two argument call
[...]
AND
IntegerSum(1, 2, _Sum)
THEN
NRD_ModCall("YourMod", "TestFunc2", "string arg", (STRING)_Sum);
```

`NRD_ModQuery*(Mod, Func, ...)` is a query (i.e. usable from the `IF` part of the rule). Its first two parameters are the mod name and function name to call, and it accepts an arbitrary number of arguments to pass to the Lua function as well as an arbitrary number of results. The last character of the function name indicates the number of IN parameters (i.e. `NRDModQuery2` for a query with 2 input parameters).

```lua
-- 0 input, 0 output
function TestFunc()
    return "test"
end

-- 2 inputs, 2 outputs
function TestFunc2(str1, str2)
    return str1 .. str2, "something else"
end
```

```c
[...]
AND
// Zero argument, zero return value query
NRD_ModQuery0("YourMod", "TestFunc")
THEN
[...]

[...]
AND
// Two argument, two return value query
NRD_ModQuery2("YourMod", "TestFunc2", "asdf", "ghjk", _Out1, _Out2)
THEN
DebugBreak(_Out1);
```

<a id="l2o_captures"></a>
### Capturing Events/Calls

It is possible to capture Osiris events from Lua without adding Osiris boilerplate code. The `Ext.Osiris.RegisterListener(name, arity, event, handler)` function registers a listener that is called in response to Osiris events.
It currently supports capturing events, built-in queries, databases, user-defined PROCs and user-defined QRYs. Capture support for built-in calls will be added in a later version.

Parameters: 
 - `name` is the function or database name
 - `arity` is the number of columns for DBs or the number of parameters (both IN and OUT) for functions
 - `event` is the type of event to capture; possible values:
   - `before` - Trigger event before a call/DB insert is performed
   - `after` - Trigger event after a call/DB insert is performed
   - `beforeDelete` - Trigger event before a DB delete is performed (databases only!)
   - `afterDelete` - Trigger event after a DB delete is performed (databases only!)
 - `handler` is a Lua function that is called when the specified event is triggered. The function receives all parameters of the original DB/function.

Example:
```lua
Ext.Osiris.RegisterListener("Moved", 1, "after", function (item)
    Ext.Utils.Print("Item moved - " .. item)
end)
```

<a id="stats"></a>
# Stats (Ext.Stats module)

### TODO DOCS - stat creation workflow, stat update workflow

<a id="stats-GetStatEntries"></a>
### Ext.Stats.GetAllStats(type: string): string[]

Returns a table with the names of all stat entries.
When the optional parameter `type` is specified, it'll only return stats with the specified type.
The following types are supported: `StatusData`, `SpellData`, `PassiveData`, `Armor`,  `Weapon`, `Character`, `Object`, `SpellSet`, `EquipmentSet`, `TreasureTable`, `TreasureCategory`,  `ItemGroup`, `NameGroup`


<a id="stats-objects"></a>
## Stats Objects

The following functions are only usable for Spell, Status, Passive, Armor, Weapon, Character and Object stats entries. Other stats types (eg. ItemGroups, TreasureTables) have their own separate sections in the docs and cannot be manipulated using these functions.


### Ext.Stats.GetStatsLoadedBefore(modGuid: string, type: string): string[]

Returns a table with the names of all stat entries that were loaded before the specified mod.
This function is useful for retrieving stats that can be overridden by a mod according to the module load order.
When the optional parameter `type` is specified, it'll only return stats with the specified type. (The type of a stat entry is specified in the stat .txt file itself (eg. `type "StatusData"`).

### Ext.Stats.CreateStat(name: string, type: string, template: string|nil): StatEntry

Creates a new stats entry. 
If a stat object with the same name already exists, the specified modifier type is invalid or the specified template doesn't exist, the function returns `nil`.
After all stat properties were initialized, the stats entry must be synchronized by calling `SyncStat()`. 

 - `name` is the name of stats entry to create; it should be globally unique
 - `type` is the stats entry type (eg. `SkillData`, `StatusData`, `Weapon`, etc.)
 - If the `template` parameter is not null, stats properties are copied from the template entry to the newly created entry
 - If the entry was created on the server, `SyncStat()` will replicate the stats entry to all clients. If the entry was created on the client, `SyncStat()` will only update it locally.

Example:
```lua
local stat = Ext.Stats.CreateStat("NRD_Dynamic_Skill", "SkillData", "Rain_Water")
stat.RainEffect = "RS3_FX_Environment_Rain_Fire_01"
stat.SurfaceType = "Fire"
Ext.Stats.SyncStat("NRD_Dynamic_Skill")
```

### Ext.Stats.GetStat(stat, type, [level]): StatEntry

Returns the specified stats entry as an object for easier manipulation.
If the `level` argument is not specified or is `nil`, the table will contain stat values as specified in the stat entry.
If the `level` argument is not `nil`, the table will contain level-scaled values for the specified level. A `level` value of `-1` will use the level specified in the stat entry.

The behavior of getting a table entry is identical to that of `StatGetAttribute` and setting a table entry is identical to `StatSetAttribute`.

The `StatSetAttribute` example rewritten using `GetStat`:
```lua
-- Swap DamageType from Poison to Air on all skills
for i,name in pairs(Ext.Stats.GetAllStats("SkillData")) do
    local stat = Ext.Stats.GetStat(name)
    if stat.DamageType == "Poison" then
        stat.DamageType = "Air"
    end
end
```

### StatEntry:Sync(persist: bool)

Synchronizes the changes made to the specified stats entry to each client.
`SyncStat` must be called each time a stats entry is modified dynamically (after `ModuleLoading`/`StatsLoaded`) to ensure that the host and all clients see the same properties.
The optional `persist` attribute determines whether the stats entry is persistent, i.e. if it will be written to savegames. If not specified, the `persist` parameter defaults to `true`.

### StatEntry:SetPersistence(persist: bool) <sup>S</sup>

Toggles whether the specified stats entry should be persisted to savegames.
Changes made to non-persistent stats will be lost the next time a game is reloaded. 
If a dynamically created stats entry is marked as non-persistent, the entry will be deleted completely after the next reload. Make sure that you don't delete entries that are still in use as it could break the game in various ways.


### Reading stat attributes

Stat attributes can be retrieved by reading the appropriate property of the StatEntry object:

```lua
local spell = Ext.Stats.GetStat("Shout_FlameBlade", "SpellData")
local useCosts = spell.UseCosts
```

If the stat entry doesn't have the specified attribute or the attribute is not supported, `nil` is returned. 
The list of attributes each stat type supports can be found in `Public\Shared\Stats\Generated\Structure\Modifiers.txt`.

*Technical note:* The StatEntry object has an `__index` metamethod that retrieves the stats property; the StatEntry is not a Lua table and shouldn't be treated as such!

### Writing stat attributes

Stat attributes can be updated using simple table assignment:

```lua
local spell = Ext.Stats.GetStat("Shout_FlameBlade", "SpellData")
spell.UseCosts = "BonusActionPoint:1;SpellSlot:1:1:2"
```

This essentially allows on-the-fly changing of data loaded from stat .txt files without having to override the whole stat entry.
If the function is called while the module is loading (i.e. from a `ModuleLoading`/`StatsLoaded` listener) no additional synchronization is needed. If the function is called after module load, the stats entry must be synchronized with the client via the `StatEntry:Sync()` call. 

*Technical note:* The StatEntry object has a `__newindex` metamethod that performs validation and updates the real stats entry in the background.


Example usage of stats read/write (Disable autocast on all spells):
```lua
for i,name in pairs(Ext.Stats.GetAllEntries("SpellData")) do
    local spell = Ext.Stats.GetStat(name, "SpellData")
    if spell.Autocast == "Yes" then
        spell.Autocast = "No"
    end
end
```

**Note:** When modifying stat attributes that are tables (i.e. `Requirements`, `SpellSuccess`, `SpellProperties` etc.) it is not sufficient to just modify the table, the modified table must be reassigned to the stat property:
```lua
local requirements = spell.Requirements
table.insert(requirements, {Name = "Immobile", Param = -1, Not = false})
-- Reassign table to update Requirements
spell.Requirements = requirements
```

### Stat property type notes

For a list of enumeration types and their possible values see `Public\Shared\Stats\Generated\Structure\Base\ValueLists.txt` or `Enumerations.xml`.

#### Flags 

The `AttributeFlags`, `SpellFlagList`, `WeaponFlags`, `ResistanceFlags`, `PassiveFlags`, `ProficiencyGroupFlags`, `StatsFunctorContext`, `StatusEvent`, `StatusPropertyFlags`, `StatusGroupFlags` and `LineOfSightFlags` enumerations are flags; this means that multiple enumeration values may be assigned to a stats property. 

Reading flags:
```lua
local spell = Ext.Stats.GetStat("Shout_ArmorOfAgathys", "SpellData")
_D(spell.SpellFlags)
-- Prints:
-- ["HasSomaticComponent", "HasVerbalComponent", "IsSpell"]
```

Writing flags:
```lua
local spell = Ext.Stats.GetStat("Shout_ArmorOfAgathys", "SpellData")
spell.SpellFlags = {"HasVerbalComponent", "IsSpell"}
```

#### Requirements

`Requirements` and `MemorizationRequirements` are returned in the following format:
```js
[
    {
        "Not" : true, // Negated condition?
        "Param" : "Tag", // Parameter; number for ability/attribute level, string for Tag
        "Requirement" : "TADPOLE_POWERS_BLOCKED" // Requirement name
    },
    {
        "Not" : true,
        "Param" : -1,
        "Requirement" : "Immobile"
    }
]
```

#### StatsFunctors

`StatsFunctors` is a list of functors (actions) that are executed when certain conditions are met (eg. when a spell succeeds/fails).  The action to perform is determined by the `Type` field. Each action has its own parameter types that can be further specified how the action should be done (eg. damage amount and type for `DealDamage`, etc.).

**[[TODO - list of functor types and properties?]]**

Example format:
```js
[
        {
                "DamageType" : "Acid",
                "IsSelf" : false,
                "Magical" : false,
                "Nonlethal" : false,
                "PropertyContext" :
                [
                        "AOE",
                        "Target"
                ],
                "Type" : "DealDamage",
                "WeaponDamageType" : "None",
                "WeaponType" : "None"
        }
]
```


<!---
### Ext.Stats.StatSetLevelScaling(statType, attribute, func)  <sup>R</sup>

**FIXME - PROBABLY DEPRECATED IN BG3**

Replaces level scaling formula for the specified stat.
This function can only be called from a `ModuleLoading` listener.

`statType` is the type of stat to override (`Character`, `SkillData`, `Potion`, etc). `attribute` is the stat attribute to override (`Strength`, `Constitution`, ...).

`func` must satisfy the following requirements:
 - Must be a Lua function that receives two arguments `(attributeValue, level)` and returns the integer level scaled value.
 - Must have no side effects (i.e. can't set external variables, call external functions, etc)
 - Must always returns the same result when given the same argument values
 - Since the function is called very frequently (up to 50,000 calls during a level load), it should execute as quickly as possible

<a id="stats-custom-spell-properties"></a>
## Custom Spell Properties - WIP!

It is possible to create custom `SpellProperties` actions that will fire when the game executes the `SpellProperties` of a spell. A handler should be registered for the action on both the client and the server.

The client side needs to implement the `GetDescription` function that'll return the description of the spell property (used in tooltips). If the function returns `nil`, no description is displayed.

Example:
```lua
Ext.RegisterSpellProperty("MY_CUSTOM_ACTION", {
    GetDescription = function (property)
        return "Test SpellProperty description"
    end
})
```

The server side needs to implement the `ExecuteOnTarget` method for executing the property on a target character and the `ExecuteOnPosition` for executing the property on the ground.

Example:
```lua
Ext.RegisterSpellProperty("MY_CUSTOM_ACTION", {
    ExecuteOnTarget = function (property, caster, target, position, isFromItem, spell, hit)
        Ext.Utils.PrintWarning("SPELLPROPERTY ExecuteOnTarget!")
        Ext.Utils.PrintWarning(property, caster, target, position, isFromItem, spell, hit)
    end,
    ExecuteOnPosition = function (property, caster, position, areaRadius, isFromItem, spell, hit)
        Ext.Utils.PrintWarning("SPELLPROPERTY ExecuteOnPosition!")
        Ext.Utils.PrintWarning(property, caster, position, areaRadius, isFromItem, spell, hit)
    end
})
```

The stats entry uses the same basic format as other actions, but the function name should be `Extender(...)`. You can pass an arbitrary number of parameters.
Property contexts are supported (i.e. `SELF:`, `TARGET:`, `AOE:`, etc.).
Conditions (i.e. `IF(whatever):`) are not _yet_ supported.

Examples:
`data "SpellProperties" "SELF:Extender(MY_CUSTOM_SKILL)"`
`data "SpellProperties" "Extender(MY_CUSTOM_SKILL,100,1,TEST TEST TEST,1,2)"`

### Executing Spell Properties - WIP!

It is possible to execute the `SpellProperties` of a spell manually (without actually using the spell) or executing a dynamically created spell property using the following functions:

```lua
Ext.ExecuteSpellPropertiesOnPosition(skillId: string, caster: ObjectHandle|int|string, target: ObjectHandle|int|string, position: number[], propertyContext: string, isFromItem: boolean)
Ext.ExecuteSpellPropertiesOnTarget(skillId: string, caster: ObjectHandle|int|string, position: number[], radius: number, propertyContext: string, isFromItem: boolean)
```

 - `skillId` is the stats entry name of the skill to use
 - `caster` and `target` are the server-side object handles, GUIDs or NetID-s of the attacker and target character
 - `position` is the position of the hit
 - `radius` is the radius of the effect (when targeting the ground)
 - `propertyContext` contains the type of properties to execute; it should be one of `Target`, `AoE`, `Self`, `SelfOnHit`, `SelfOnEquip`
 - `isFromItem` determines whether the skill was granted by an item

Example:
```lua
local char = CharacterGetHostCharacter()
local position = Ext.GetCharacter(char).WorldPos

-- Exec on ground
Ext.ExecuteSpellPropertiesOnPosition("Projectile_AcidSpores", char, position, 3.0, "AoE", false)
-- Exec on a target character
Ext.ExecuteSpellPropertiesOnTarget("Shout_CatSwapPlaces", char, ch2, position, "Target", false)
```
-->

### Ext.Stats.ExtraData

`Ext.ExtraData` is an object containing all entries from `Data.txt`.

*Note*: It is possible to add custom `ExtraData` keys by adding a new `Data.txt` to the mod and then retrieve them using Lua.

Example:
```lua
Ext.Utils.Print(Ext.Stats.ExtraData.WisdomTierHigh)
```

# ECS

### TODO - WIP

## Entity class

Game objects in BG3 are called entities. Each entity consists of multiple components that describes certain properties or behaviors of the entity.
The Lua `Entity` class is the represntation of an ingame object (eg. character, item, trigger, etc.).

*Technical note:* For a somewhat more detailed description of the ECS system see:
 - [Entities, components and systems](https://medium.com/ingeniouslysimple/entities-components-and-systems-89c31464240d)
 - [The Entity-Component-System - An awesome game-design pattern in C++ ](https://www.gamasutra.com/blogs/TobiasStein/20171122/310172/The_EntityComponentSystem__An_awesome_gamedesign_pattern_in_C_Part_1.php)


### Entity:HasRawComponent(type) : bool

Returns whether the entity has a component with the given engine type (native C++ class name).

Example:
```lua
local char = Ext.GetCharacter(GetHostCharacter())
_D(char:HasRawComponent("ls::TransformComponent"))
-- Prints:
-- true
```


### Entity:GetComponentHandles() : string[]

Returns all engine component types (native C++ class names) that the entity has.

Example:
```lua
local char = Ext.GetCharacter(GetHostCharacter())
_D(char:GetAllRawComponents())
-- Prints:
-- {
--      "eoc::ActionResourcesComponent" : "eoc::ActionResourcesComponent Object (1c4000010000039e)",
--      "eoc::BackgroundComponent" : "eoc::BackgroundComponent Object (1e000001000003ff)",
--      "eoc::BackgroundPassivesComponent" : "eoc::BackgroundPassivesComponent Object (66c00001000003ff)",
-- ...
```


### Entity:GetAllComponents() : Component[]

Returns all components that are attached to the entity.

*Note:* This method only returns components whose structure is known to the Script Extender. Components with unknown structure are not returned.

Example:
```lua
local entity = Ext.GetCharacter(GetHostCharacter())
_D(entity:GetAllRawComponents())
-- Prints:
-- {
--      "ActionResources" :
--      {
--              "Entity" : "Entity (02c0000100000180)",
--              "GetReplicationFlags" : "function: 00007FFDE482D5E0",
-- ...
```


### Entity:GetComponent(name) : Component|nil

Returns the specified component if it is attached to the entity. If the component is not present the method returns `nil`.

*Note:* This method only returns components whose structure is known to the Script Extender. Components with unknown structure are not returned.

*Note:* Although the type (character, item, etc.) of the entity cannot be determined directly, it can be inferred from the components that are attached to the entity.
Eg. to check if the entity is a character, an `entity:GetComponent("ServerCharacter") ~= nil` check can be used.

Example:
```lua
local entity = Ext.GetCharacter(GetHostCharacter())
_D(entity:GetComponent("DisplayName"))
-- Prints:
-- {
--      "Entity" : "Entity (02c0000100000180)",
--      "Name" : "Tav",
--      "NameKey" : "ResStr_669727657",
-- ...
```

The `__index` metamethod of the Entity object is a shorthand for `GetComponent`:
```lua
local entity = Ext.GetCharacter(GetHostCharacter())
-- The two below are equivalent
local displayName = entity:GetComponent("DisplayName")
local displayName = entity.DisplayName
```


### Entity:IsAlive() : boolean

Returns whether the entity still exists.

### Entity:GetEntityType() : integer

Returns the numeric type ID of the entity.
(For development purposes only.)

### Entity:GetSalt() : integer

Returns the salt value of the entity handle.
(For development purposes only.)

### Entity:GetIndex() : integer

Returns the entity index of the entity handle.
(For development purposes only.)

### TODO - GetComponents() -> move to __index




## ComponentHandle class

The Lua `ComponentHandle` class is a reference to a specific component of an entity.
*Technical detail:* The handle class stores the handle (identifier) of the component; each time an access is made using the handle, the extender looks up the component using its handle.

### ComponentHandle:Get() : Component

Returns the component referenced by the component handle. If the component handle is invalid or the component doesn't exist anymore the method returns `nil`.

*Note:* This method only returns components whose structure is known to the Script Extender. Components with unknown structure are not returned.

Example:
```lua
-- [[ TODO - FIND GOOD EXAMPLE WITH COMPONENT REFS! ]]
```

### ComponentHandle:GetType() : integer

Returns the numeric type ID of the object handle.
(For development purposes only.)

### ComponentHandle:GetTypeName() : string

Returns the type name of the component referenced by the handle.

### ComponentHandle:GetSalt() : integer

Returns the salt value of the object handle.
(For development purposes only.)

### ComponentHandle:GetIndex() : integer

Returns the object index of the object handle.
(For development purposes only.)



# Mod Info

### IsModLoaded(modGuid)

Returns whether the module with the specified GUID is loaded.
This is equivalent to Osiris `NRD_IsModLoaded`, but is callable when the Osiris scripting runtime is not yet available (i.e. `ModuleLoading˙, etc events).

Example:
```lua
if (Ext.IsModLoaded("5cc23efe-f451-c414-117d-b68fbc53d32d"))
    Ext.Utils.Print("Mod loaded")
end
```

### GetModLoadOrder()

Returns the list of loaded module UUIDs in the order they're loaded in.

### GetModInfo(modGuid)

Returns detailed information about the specified (loaded) module.
Example:
```lua
local loadOrder = Ext.GetModLoadOrder()
for k,uuid in pairs(loadOrder) do
    local mod = Ext.GetModInfo(uuid)
    Ext.Utils.Print(Ext.JsonStringify(mod))
end
```

Output:
```json
{
    "Author" : "Larian Studios",
    "Dependencies" :
    [
        "2bd9bdbe-22ae-4aa2-9c93-205880fc6564",
        "eedf7638-36ff-4f26-a50a-076b87d53ba0"
    ],
    "Description" : "",
    "Directory" : "DivinityOrigins_1301db3d-1f54-4e98-9be5-5094030916e4",
    "ModuleType" : "Adventure",
    "Name" : "Divinity: Original Sin 2",
    "PublishVersion" : 905969667,
    "UUID" : "1301db3d-1f54-4e98-9be5-5094030916e4",
    "Version" : 372645092
}
```

<!---
# Server Objects

<a id="server-characters"></a>
## Server Characters <sup>S</sup>

Characters in server contexts can be retrieved using the `Ext.GetCharacter(ref)` call. The function accepts a character GUID, a NetID or an ObjectHandle. If the character cannot be found, the return value is `nil`; otherwise a Character object is returned.

Player objects have the following properties:

| Name | Type | Notes |
|--|--|--|
| Stats | userdata | See [CharacterStats](#character-stats) |
| PlayerCustomData | userdata | See [PlayerCustomData](#player-custom-data) |
| NetID | integer | Network ID of the character |
| MyGuid | string | GUID of the character |
| WorldPos | number[3] | Position of the character |
| CurrentLevel | String | Name of level (map) the character is currently on |
| Scale | number |  |
| AnimationOverride | string |  |
| WalkSpeedOverride | integer |  |
| RunSpeedOverride | integer |  |
| NeedsUpdateCount | integer |  |
| ScriptForceUpdateCount | integer |  |
| ForceSynchCount | integer |  |
| SkillBeingPrepared | string |  |
| LifeTime | number | Used for summons to indicate lifetime |
| PartialAP | number | Movement AP |
| AnimType | integer |  |
| DelayDeathCount | integer |  |
| AnimationSetOverride | string |  |
| CustomTradeTreasure | string |  |
| Archetype | string |  |
| EquipmentColor | string |  |
| IsPlayer | boolean |  |
| Multiplayer | boolean |  |
| InParty | boolean |  |
| HostControl | boolean |  |
| Activated | boolean |  |
| OffStage | boolean |  |
| Dead | boolean |  |
| HasOwner | boolean  |  |
| InDialog | boolean |  |
| Summon | boolean  |  |
| CharacterControl | boolean |  |
| Loaded | boolean  |  |
| InArena | boolean |  |
| CharacterCreationFinished | boolean |  |
| Floating | boolean |  |
| SpotSneakers | boolean |  |
| WalkThrough | boolean |  |
| CoverAmount | boolean |  |
| CanShootThrough | boolean |  |
| PartyFollower | boolean |  |
| Totem | boolean  |  |
| NoRotate | boolean  |  |
| IsHuge | boolean  |  |
| Global | boolean |  |
| HasOsirisDialog | boolean |  |
| HasDefaultDialog | boolean |  |
| TreasureGeneratedForTrader | boolean |  |
| Trader | boolean |  |
| Resurrected | boolean |  |
| IsPet | boolean |  |
| IsSpectating | boolean |  |
| NoReptuationEffects | boolean |  |
| HasWalkSpeedOverride | boolean |  |
| HasRunSpeedOverride | boolean |  |
| IsGameMaster | boolean  |  |
| IsPossessed | boolean |  |


<a id="player-custom-data"></a>
## Player Custom Data

Contains player customization info. Properties:

| Name | Type | 
|--|--|
| CustomLookEnabled | boolean |
| Name | string |
| ClassType | string |
| SkinColor | integer |
| HairColor | integer |
| ClothColor1 | integer |
| ClothColor2 | integer |
| ClothColor3 | integer |
| IsMale | boolean |
| Race | string |
| OriginName | string |
| Icon | string |
| MusicInstrument | string |
| OwnerProfileID | string |
| ReservedProfileID | string |
| AiPersonality | string |
| Speaker | string |


<a id="server-items"></a>
## Server Items <sup>S</sup>

Items on the server can be retrieved using the `Ext.GetItem(ref)` call. The function accepts an item GUID or an ObjectHandle. If the item cannot be found, the return value is `nil`; otherwise an Item object is returned.

Items have the following properties:

| Name | Type | Notes |
|--|--|--|
| Stats | userdata | See [ItemStats](#item-stats) |
| PlayerCustomData | userdata | See [PlayerCustomData](#player-custom-data) |
| NetID | integer | Network ID of the item |
| MyGuid | string | GUID of the item |
| WorldPos | vec3 | Position of the item |
| CurrentLevel | String | Name of level (map) the item is currently on |
| Scale | number |  |
| CustomDisplayName | string | |
| CustomDescription | string | |
| CustomBookContent | string | |
| StatsId | string | Stats entry (eg. `WPN_Dagger`) |
| Slot | integer | |
| Amount | integer | |
| Vitality | integer | |
| Armor | integer | |
| InUseByCharacterHandle | integer | Character currently using the item |
| Key | string | Key used to open the container |
| LockLevel | integer | |
| OwnerHandle | integer | ObjectHandle to the owner of this item |
| ComputedVitality | integer | |
| ItemType | integer | |
| GoldValueOverwrite | integer | |
| WeightValueOverwrite | integer | |
| TreasureLevel | integer | |
| LevelOverride | integer | |
| ForceSynch | boolean | |


<a id="server-projectiles"></a>
## Server Projectiles <sup>S</sup>

Currently projectiles are only available when passed as parameters to event listeners (`GetSkillDamage`, `ComputeCharacterHit`, etc.), and are not retrievable otherwise.

They have the following properties:

| Name | Type | Notes |
|--|--|--|
| NetID | integer | Network ID of the projectile |
| MyGuid | string | GUID of the projectile |
| CasterHandle | integer | |
| SourceHandle | integer | |
| TargetObjectHandle | integer | |
| HitObjectHandle | integer | |
| SourcePosition | vec3 | |
| TargetPosition | vec3 | |
| DamageType | string | (See `DamageType` enumeration) |
| DamageSourceType | string | (See `CauseType` enumeration) |
| LifeTime | number |  |
| HitInterpolation | number |  |
| ExplodeRadius0 | number |  |
| ExplodeRadius1 | number |  |
| DeathType | string | (See `DeathType` enumeration) |
| SkillId | string |  |
| WeaponHandle | integer | |
| MovingEffectHandle | integer |  |
| SpawnEffect | string |  |
| SpawnFXOverridesImpactFX | boolean |  |
| EffectHandle | integer |  |
| RequestDelete | boolean |  |
| Launched | boolean |  |
| IsTrap | boolean |  |
| UseCharacterStats | boolean |  |
| ReduceDurability | boolean |  |
| AlwaysDamage | boolean |  |
| ForceTarget | boolean  |  |
| IsFromItem | boolean |  |
| DivideDamage | boolean  |  |
| IgnoreRoof | boolean |  |
| CanDeflect | boolean  |  |
| IgnoreObjects | boolean |  |
| CleanseStatuses | string |  |
| StatusClearChance | number |  |
| Position | vec3 |  |
| PrevPosition | vec3 |  |
| Velocity | vec3 |  |
| Scale | number|  |
| CurrentLevel | string |  |


<a id="server-surfaces"></a>
## Server Surfaces <sup>S</sup>

**WIP**

| Property | Type | Writeable | Notes |
|--|--|--|--|
| NetId | integer | | |
| MyHandle | ObjectHandle | | |
| SurfaceType | string | | |
| RootTemplate | [SurfaceTemplate](#surface-templates) | | |
| Flags | integer | | |
| TeamId | integer | | Combat team ID (see [Combat Team](#server-combat-team)) |
| OwnerHandle | ObjectHandle| | Character/item that created this surface |
| LifeTime | number | ✓ | Surface lifetime in seconds |
| LifeTimeFromTemplate | boolean | | |
| StatusChance | number | ✓ | |


<a id="server-statuses"></a>
## Server Statuses <sup>S</sup>

Properties available on all statuses:

| Property | Type | Notes |
|--|--|--|
| NetID | integer | Network ID of the status. Since status have no GUID, only the NetID can be used for sending status references between the client and the server. |
| StatusId | string | Name of the associated stat entry |
| StatusHandle | integer | Handle of this status |
| TargetHandle | integer | Character or item that the status was applied to |
| StatusSourceHandle | integer | Character or item that caused the status |
| StartTimer | number |  |
| LifeTime | number | Total lifetime of the status, in seconds. -1 if the status does not expire. |
| CurrentLifeTime | number | Remaining lifetime of the status, in seconds. |
| TurnTimer | number | Elapsed time in the current turn (0..6) |
| Strength | number |  |
| StatsMultiplier | number |  |
| CanEnterChance | integer | Chance of entering status (between 0 and 100) |
| DamageSourceType | string | Cause of status (See `DamageSourceType` enum) |
| KeepAlive | boolean |  |
| IsOnSourceSurface | boolean |  |
| IsFromItem | boolean |  |
| Channeled | boolean |  |
| IsLifeTimeSet | boolean | Does the status have a lifetime or is it infinite? |
| InitiateCombat | boolean |  |
| Influence | boolean |  |
| BringIntoCombat | boolean |  |
| IsHostileAct | boolean |  |
| IsInvulnerable | boolean | The status turns the character invulnerable |
| IsResistingDeath | boolean | The character can't die until the status expires |
| ForceStatus | boolean | Bypass immunity and status enter chance checks. |
| ForceFailStatus | boolean | Forces prerequisite checks to fail. |
| RequestDelete | boolean | The status is being deleted (i.e. it's not active anymore) |
| RequestDeleteAtTurnEnd | boolean | The status will be deleted at the end of the current turn |
| Started | boolean |  |

### `CONSUME` status properties

| Property | Type | Notes |
|--|--|--|
| ResetAllCooldowns | boolean |  |
| ResetOncePerCombat | boolean |  |
| ScaleWithVitality | boolean |  |
| LoseControl | boolean |  |
| ApplyStatusOnTick | boolean |  |
| EffectTime | number |  |
| StatsId | string |  |
| StackId | string |  |
| OriginalWeaponStatsId | string |  |
| OverrideWeaponStatsId | string |  |
| OverrideWeaponHandle | integer |  |
| SavingThrow | string |  |
| SourceDirection | vec3 |  |
| Turn | integer | |
| HealEffectOverride | string | See `HealEffect` enumeration |

### `HIT` status properties

| Property | Type | Notes |
|--|--|--|
| SkillId | string | Stats ID of the skill (`SkillData`) that was used for the attack |
| HitByHandle | integer |  |
| HitWithHandle | integer |  |
| WeaponHandle | integer |  |
| HitReason | integer |  |
| Interruption | boolean |  |
| AllowInterruptAction | boolean |  |
| ForceInterrupt | boolean |  |
| DecDelayDeathCount | boolean |  |
| ImpactPosition | vec3 |  |
| ImpactOrigin | vec3 |  |
| ImpactDirection | vec3 |  |
| Equipment | integer | |
| TotalDamage | integer | Sum of all damages |
| DamageDealt | integer | Damage dealt after ApplyDamage |
| DeathType | string | See `Death Type` enumeration |
| DamageType | string | See `Damage Type` enumeration |
| AttackDirection | string | See `AttackDirection` enumeration. |
| ArmorAbsorption | integer | Armor points consumed during attack |
| LifeSteal | integer |  |
| HitWithWeapon | boolean |  |
| Hit | boolean | The attack hit |
| Blocked | boolean | The attack was blocked |
| Dodged | boolean | The attack was dodged |
| Missed | boolean | The attack missed |
| CriticalHit | boolean |  |
| AlwaysBackstab | boolean | Equivalent to the `AlwaysBackstab` skill property |
| FromSetHP | boolean | Indicates that the hit was called from `CharacterSetHitpointsPercentage` (or similar) |
| DontCreateBloodSurface | boolean | Avoids creating a blood surface when the character is hit |
| Reflection | boolean |  |
| NoDamageOnOwner | boolean |  |
| FromShacklesOfPain | boolean |  |
| DamagedMagicArmor | boolean | Indicates that the hit damaged magic armor |
| DamagedPhysicalArmor | boolean | Indicates that the hit damaged physical armor |
| DamagedVitality | boolean | Indicates that the hit damaged the characters vitality |
| Flanking | boolean | |
| PropagatedFromOwner | boolean |  |
| Surface | boolean | The hit is from a surface (`HitType` was `Surface`) |
| DoT | boolean | The hit is from a DoT attack (`HitType` was `DoT`) |
| ProcWindWalker | boolean | Hit should proc the Wind Walker talent |
| CounterAttack | boolean | Counterattack triggered by Gladiator talent |
| Poisoned | boolean | Character was poisoned when hit |
| Bleeding | boolean | Character was bleeding when hit |
| Burning | boolean | Character was burning when hit |
| NoEvents | boolean | Don't throw `OnHit`/`OnPrepareHit` events for this hit |

### `DAMAGE` status properties

| Property | Type | Notes |
|--|--|--|
| DamageEvent | integer |  |
| HitTimer | number |  |
| TimeElapsed | number | |
| DamageLevel | integer | |
| DamageStats | string |  |
| SpawnBlood | boolean | |

### `DAMAGE_ON_MOVE` status properties

| Property | Type | Notes |
|--|--|--|
| DistancePerDamage | number | |
| DistanceTraveled | number | |

### `HEAL` status properties

| Property | Type | Notes |
|--|--|--|
| EffectTime | number |  |
| HealAmount | integer |  |
| HealEffect | string |  |
| HealEffectId | string | Default `RS3_FX_GP_ScriptedEvent_Regenerate_01` |
| HealType | string | See `StatusHealType` enumeration |
| AbsorbSurfaceRange | integer |  |
| TargetDependentHeal | boolean |  |


### `HEALING` status properties

| Property | Type | Notes |
|--|--|--|
| HealAmount | integer |  |
| TimeElapsed | number |  |
| HealEffect | string | See `HealEffect` enumeration |
| HealEffectId | string | Default `RS3_FX_GP_ScriptedEvent_Regenerate_01` |
| SkipInitialEffect | boolean | |
| HealingEvent | integer |  |
| HealStat | string | See `HealType` enumeration |
| AbsorbSurfaceRange | integer |  |


<a id="server-combat"></a>
## Combat <sup>S</sup>

Each combat in-game is represented by a Combat object in Lua. 

Properties:

| Name | Type | Notes |
|--|--|--|
| CombatId | integer | A number identifying the combat instance |
| LevelName | string | Level where the combat is taking place |
| IsActive | boolean |  |


Methods:

#### GetAllTeams() <sup>S</sup>
Retrieves all participants of the combat. The return value is a table of `Team` objects.

#### GetCurrentTurnOrder() <sup>S</sup>
Retrieves the turn order of the current round. The return value is a table of `Team` objects.

#### GetNextTurnOrder() <sup>S</sup>
Retrieves the turn order of the next round. The return value is a table of `Team` objects.

#### UpdateCurrentTurnOrder(turnOrder) <sup>S</sup>
Updates the turn order of the current round. The `turnOrder` argument should be a reordered version of the table returned by `GetCurrentTurnOrder()`. 

Notes:
 - It is possible to remove or add characters to the current turn by adding/removing their `Team` object from the table. 
 - It is possible to add a character to the current turn more than once, the character will only appear once in the UI however.
 - The character whose turn is currently active (the very first item) should not be removed or reordered. This only applies for `GetCurrentTurnOrder`, the first item can be freely reordered in `GetNextTurnOrder`.
 - Changed performed using this function are synchronized to the client at the end of the current server tick.

#### UpdateNextTurnOrder(turnOrder) <sup>S</sup>
Updates the turn order of the next round. The `turnOrder` argument should be a reordered version of the table returned by `GetNextTurnOrder()`. 

Notes:
 - It is possible to remove or add characters to the next turn by adding/removing their `Team` object from the table. 
 - It is possible to add a character to the next turn more than once, the character will only appear once in the UI however.
 - Changed performed using this function are synchronized to the client at the end of the current server tick.

### CalculateTurnOrder Event <sup>S R</sup>

When the turn order of the next round of a combat is being updated for some reason (new round, character entered combat, etc.) the `CalculateTurnOrder` Ext event is thrown. 
The event receives two parameters:
 - `combat`: the Combat object
 - `order`: the turn order determined by the game (equivalent to calling `combat:GetNextTurnOrder()`)

To change the turn order, reorder the `order` table and return it from the event handler. To leave the turn order untouched, return `nil` (or nothing).

```lua
-- Example for calculating an initiative-based turn order
local function CalcInitiativeTurnOrder(combat, order)
    table.sort(order, function (a, b)
        return a.Initiative > b.Initiative
    end)
    return order
end

Ext.RegisterListener("CalculateTurnOrder", CalcInitiativeTurnOrder)
```

<a id="server-combat-team"></a>
### Combat Team <sup>S</sup>

A `Team` is a combat participant (either a character or an item).

Properties:

| Name | Type | Notes |
|--|--|--|
| TeamId | integer | A number identifying the team instance |
| CombatId | integer | Identifies which combat the team is a participant of |
| Initiative | integer | Computed initiative value of the team |
| StillInCombat | boolean | Can the team currently fight, or is it temporarily out of combat? |
| Character | esv::Character | Character object if the team is a character; `nil` otherwise |
| Item | esv::Item | Item object if the team is an item; `nil` otherwise |


<a id="surface-actions"></a>
# Surface Actions <sup>S</sup>

The surface action API allows creation and manipulation of surfaces.
To perform an action, the following steps must be performed:
 - Create the appropriate action using `Ext.CreateSurfaceAction(type)`
 - Set the required properties on the action object (see below for what settings are available for each action)
 - Launch the action using `Ext.ExecuteSurfaceAction(action)`

To cancel an action before its completed, `Ext.CancelSurfaceAction(handle)` can be used.

Shared properties on all surface actions:

| Property | Type | Notes |
|--|--|--|
| OwnerHandle | ObjectHandle| Character/item that created the surface |
| Duration | number | Surface duration in seconds |
| StatusChance | number | |
| Position | vec3 | |
| SurfaceType | string | Surface type name (eg. `Fire`, `FireBlessed`, etc.) |
| StatusChance | number | |


## ChangeSurfaceOnPath

Transforms surfaces in a path that follows the specified character or item.

Example:
```lua
local objectToFollow = Ext.GetCharacter(CharacterGetHostCharacter())
local surf = Ext.CreateSurfaceAction("ChangeSurfaceOnPathAction")
surf.SurfaceType = "Water"
surf.FollowObject = objectToFollow.Handle
surf.Radius = 3.0
local handle = surf.MyHandle
Ext.ExecuteSurfaceAction(surf)
```

Unlike other actions, `ChangeSurfaceOnPathAction` never expires and must be canceled by script to stop its effects:
```lua
Ext.CancelSurfaceAction(handle)
```

Properties:

| Property | Type | Notes |
|--|--|--|
| FollowObject | ObjectHandle | Character/item the transform will follow |
| Radius | number | Radius around the follow object that will be transformed |
| IgnoreIrreplacableSurfaces | boolean | |
| CheckExistingSurfaces | boolean | |
| SurfaceCollisionFlags | integer | When specified, AI grid cells that don't have any of the flags in `SurfaceCollisionFlags` will be ignored |
| SurfaceCollisionNotOnFlags | integer | When specified, AI grid cells that have any of the flags in `SurfaceCollisionNotOnFlags` will be ignored |
| IgnoreOwnerCells | boolean | Don't transform surfaces that are owned by the owner of this action |


## CreatePuddle

Creates a circular surface.
Example:
```lua
local pos = Ext.GetCharacter(CharacterGetHostCharacter()).WorldPos
local surf = Ext.CreateSurfaceAction("CreatePuddleAction")
surf.Duration = 6.0
surf.SurfaceType = "Fire"
surf.Position = {pos[1] + 10.0, pos[2], pos[3]}
surf.SurfaceCells = 100
surf.GrowSpeed = 0.1
surf.Step = 100
Ext.ExecuteSurfaceAction(surf)
```

Properties:

| Property | Type | Notes |
|--|--|--|
| SurfaceCells | integer | Number of AI grid cells to cover with the surface |
| Step | number | How many cells the surface grows on each tick |
| GrowSpeed | number | Determines the time between grow steps during surface creation |
| IgnoreIrreplacableSurfaces | boolean | |
| GrowTimer | number| |


## ExtinguishFire

Extinguishes fire surfaces in a radius around the target.
Example:
```lua
local surf = Ext.CreateSurfaceAction("ExtinguishFireAction")
surf.Position = {pos[1] + 2.0, pos[2], pos[3]}
surf.Radius = 2.0
surf.Percentage = 1.0
surf.GrowTimer = 0.1
surf.Step = 100
Ext.ExecuteSurfaceAction(surf)
```

Properties:

| Property | Type | Notes |
|--|--|--|
| DamageList | DamageList | Damage that gets applied when a character enters the surface |
| Position | vec3 | Position of surface to extinguish |
| Radius | number | Extinguish radius |
| Percentage | number | % of surface cells to remove (0..1) |
| GrowTimer | number | Time between grow steps |
| Step | number | Number of cells the action processes on each tick |


## RectangleSurface

Creates a rectangular surface.
Example:
```lua
local pos = Ext.GetCharacter(CharacterGetHostCharacter()).WorldPos
local surf = Ext.CreateSurfaceAction("RectangleSurfaceAction")
surf.Duration = 6.0
surf.SurfaceType = "Fire"
surf.Position = {pos[1] + 1.0, pos[2], pos[3]}
surf.Target = {pos[1] + 10.0, pos[2], pos[3]}
surf.Width = 2
surf.Length = 10
surf.GrowTimer = 0.1
surf.GrowStep = 100
Ext.ExecuteSurfaceAction(surf)
```

Properties:

| Property | Type | Notes |
|--|--|--|
| DamageList | DamageList | Damage that gets applied when a character enters the surface |
| Target | vec3 | Position that the surface grows towards |
| SurfaceArea | number | |
| Width | number | Width of rectangle |
| Length | number | Length of rectangle |
| GrowTimer | number | Determines the time between grow steps during surface creation |
| GrowStep | number | How many cells the surface grows on each tick |
| MaxHeight | number | |
| AiFlags | integer | |
| DeathType | string | |
| LineCheckBlock | integer | |


## PolygonSurface

Creates a surface defined by a polygon.

Example:
```lua
local pos = Ext.GetCharacter(CharacterGetHostCharacter()).WorldPos
local surf = Ext.CreateSurfaceAction("PolygonSurfaceAction")
surf.Duration = 12.0
surf.SurfaceType = "Oil"
surf.Vertices = { -- Triangle shape
    {pos[1], pos[3]},
    {pos[1] + 10.0, pos[3]},
    {pos[1] + 10.0, pos[3] + 10.0}
}
Ext.ExecuteSurfaceAction(surf)
```

Properties:

| Property | Type | Notes |
|--|--|--|
| DamageList | DamageList | Damage that gets applied when a character enters the surface |
| Vertices | vec2[] | List of 2D (X,Z) vertices of the polygon |
| GrowTimer | number | Determines the time between grow steps during surface creation |
| GrowStep | number | How many cells the surface grows on each tick |


## SwapSurface

Swaps surfaces between the specified positions.

Example:
```lua
local pos = Ext.GetCharacter(CharacterGetHostCharacter()).WorldPos
local surf = Ext.CreateSurfaceAction("SwapSurfaceAction")
surf.Position = {pos[1] + 10.0, pos[2], pos[3]}
surf.Target = {pos[1] + 5.0, pos[2], pos[3]}
surf.Radius = 5.0
Ext.ExecuteSurfaceAction(surf)
```

Properties:

| Property | Type | Notes |
|--|--|--|
| Radius | number | |
| ExcludeRadius | number | |
| MaxHeight | number | |
| Target | vec3 | Position of second surface that'll be swapped (first position is set using the `Position` property) |
| IgnoreIrreplacableSurfaces | boolean | |
| CheckExistingSurfaces | boolean | |
| SurfaceCollisionFlags | integer | When specified, AI grid cells that don't have any of the flags in `SurfaceCollisionFlags` will be ignored |
| SurfaceCollisionNotOnFlags | integer | When specified, AI grid cells that have any of the flags in `SurfaceCollisionNotOnFlags` will be ignored |
| LineCheckBlock | integer | |
| GrowTimer | number | Determines the time between grow steps during surface swap |
| GrowStep | number | How many cells to swap on each tick |


## Zone

Creates a cone-shaped surface.
Example:

```lua
local pos = Ext.GetCharacter(CharacterGetHostCharacter()).WorldPos
local surf = Ext.CreateSurfaceAction("ZoneAction")
surf.Duration = 6.0
surf.SurfaceType = "Oil"
surf.Position = pos
surf.Target = {pos[1] + 5.0, pos[2], pos[3]}
surf.GrowTimer = 0.02
surf.GrowStep = 10
surf.Shape = 0 -- 0=Cone, 1=Square
surf.Radius = 7.0
surf.AngleOrBase = 60.0
surf.MaxHeight = 2.4
Ext.ExecuteSurfaceAction(surf)
```

Properties:

| Property | Type | Notes |
|--|--|--|
| SkillId | string | Skill to use for `TargetConditions` checks and for executing `SkillProperties` when a target is hit by the surface |
| DamageList | DamageList | |
| Target | vec3 | Direction towards which the cone will be shot |
| Shape | integer | Surface shape (0 = Cone, 1 = Square) |
| Radius | number| Cone range |
| AngleOrBase | number | Cone angle or square base (depending on surface shape) |
| BackStart | number | |
| FrontOffset | number | |
| MaxHeight | number | |
| GrowTimer | number | Determines the time between grow steps during surface creation |
| GrowStep | integer | How many cells to add on each tick |
| AiFlags | integer | |
| DeathType | string | |


<a id="root-templates"></a>
# Root Templates

**Note:** When editing root templates from Lua make sure that the templates are updated on both the server and all clients; otherwise what the client sees may be out of sync with what's actually happening on the server.

<a id="surface-templates"></a>
## Surface Templates

Surface templates can be retrieved using the `Ext.GetSurfaceTemplate(type)` function. The `type` parameter is the name of the surface to fetch (eg. `Fire`, `OilCursed`). If the specified surface type exists, a surface template object is returned with the properties described below.

The properties of surface templates can be changed during module load as well as in runtime.
Example to replace `BURNING` status with `WET` when fire is applied:
```lua
Ext.GetSurfaceTemplate("Fire").Statuses = {{
    Duration = 12.0,
    KeepAlive = true,
    StatusId = "WET"
}}
```

| Property | Type | Writeable | Notes |
|--|--|--|--|
| SurfaceTypeId | integer | | Engine type ID of the surface |
| SurfaceType | string | | Surface name (eg. `Fire`, `FireBlessed`, etc.) |
| DisplayName | string | ✓ | |
| Description | string | ✓ | |
| DecalMaterial | string | ✓ | |
| CanEnterCombat | boolean | ✓ | |
| AlwaysUseDefaultLifeTime | boolean | ✓ | |
| DefaultLifeTime | number | ✓ | |
| SurfaceGrowTimer | number | ✓ | |
| FadeInSpeed | number | ✓ | |
| FadeOutSpeed | number | ✓ | |
| Seed | integer | ✓ | |
| Statuses | [SurfaceTemplateStatus](#surface-template-status)[] | ✓ | List of statuses that the surface applies when it comes in contact with a character |
| DamageWeapon | string | ✓ | |
| Summon | string | ✓ | |
| DamageCharacters | boolean | ✓ | |
| DamageItems | boolean | ✓ | |
| DamageTorches | boolean | ✓ | |
| RemoveDestroyedItems | boolean | ✓ | |
| CanSeeThrough | boolean | ✓ | |
| CanShootThrough | boolean | ✓ | |


<a id="surface-template-status"></a>
### Surface Template Status

This type describes a status that is either applied or removed when a character comes in contact with the surface. Each surface type may apply/remove multiple statuses.

| Property | Type | Writeable | Notes |
|--|--|--|--|
| StatusId | string | ✓ | Stats ID of the status that the surface applies |
| Chance | number | ✓ | Status apply chance (0..1) |
| Duration | string | ✓ | Duration of status in seconds |
| RemoveStatus | boolean | ✓ | Should the surface remove this status instead of apply? |
| OnlyWhileMoving | boolean | ✓ | |
| ApplyToCharacters | boolean | ✓ | |
| ApplyToItems | boolean | ✓ | |
| KeepAlive | boolean  | ✓ | Characters lose the status if they're no longer in the surface |
| VanishOnReapply | boolean | ✓ | Decay the surface when the status is already applied to the character |
| ForceStatus | boolean | ✓ | Equivalent to the `_Force` parameter to `ApplyStatus()` |


## Damage Lists

A damage list is an object that stores the amount of damage dealt for each damage type (`Physical`, `Poison`, etc.).
It is currently used by the `GetSkillDamage` and `ComputeCharacterHit` events to fetch damage information.

Damage lists can be created using the `Ext.NewDamageList()` function.

Methods:

#### Add(damageType, amount)
Increase/decrease the amount of damage in the list. Positive values add, negative values remove damage.
```lua
list:Add("Physical", 10) -- Add 10 points of physical damage
list:Add("Physical", -5) -- Subtract 5 points of physical damage
```

#### Clear([damageType])
Clears the damage list. If `damageType` is specified, only damages with the specified type will be removed.
```lua
list:Clear() -- Remove all
list:Clear("Poison") -- Remove only poison damage
```

#### Multiply(amount)
Multiplies the amount of damage with the specified value.
```lua
list:Add("Physical", 10)
list:Multiply(2.5) -- Physical damage amount is now 25
```

#### Merge(list)
Merge the damage values in the second list into the first one.
```lua
list:Add("Physical", 15)
local list2 = Ext.NewDamageList()
list2:Add("Physical", 25)
list:Merge(list2) -- Physical damage amount is now 40
```

#### ConvertDamageType(damageType)
Converts the damage type of every item in the damage list to the specified value.
```lua
list:Add("Physical", 15)
list:Add("Piercing", 15)
list:ConvertDamageType("Poison") -- Poison damage amount is now 30
```

#### ToTable()
Returns a table containing every item in the list.
```lua
for i,damage in pairs(list:ToTable()) do
    Ext.Utils.Print(damage.DamageType .. ": " .. damage.Amount)
end
```
-->

<a id="ext-utility"></a>
## Utility functions

#### Ext.Require(path) <sup>R</sup>

The `Ext.Require` function is the extender's version of the Lua built-in `require` function. 
The function checks if the file at `Mods/<ModuleUUID>/ScriptExtender/Lua/<path>` was already loaded; if not, it'll load the file, store the return value of the main chunk and return it to the caller. If the file was already loaded, it'll return the stored return value.
**Note:** `Ext.Require` should only be called during module startup (i.e. when loading `BootstrapClient.lua` or `BoostrapServer.lua`). Loading Lua files after module startup is deprecated.

#### Ext.Utils.Print(...)

Prints the specified value(s) to the debug console. Works similarly to the built-in Lua `print()`, except that it also logs the printed messages to the editor messages pane.

#### Ext.GetTranslatedString(key[, fallback])

Returns the text associated with the specified translated string key. If the key doesn't exist, the value of `fallback` is returned. If no fallback value is specified, an empty string (`""`) is returned.

```lua
local str = Ext.GetTranslatedString("h17edbbb2g9444g4c79g9409gdb8eb5731c7c", "[1] cast [2] on the ground")
```

#### Ext.Utils.AddPathOverride(originalPath, newPath)

Redirects file access from `originalPath` to `newPath`. This is useful for overriding built-in files or resources that are otherwise not moddable.
Make sure that the override is added as early as possible (preferably in `ModuleLoading`), as adding path overrides after the game has already loaded the resource has no effect.

Example:
```lua
Ext.AddPathOverride("Public/Game/GUI/Assets/Buttons/BigBtn_d.DDS", "Public/YourMod/GUI/Assets/Buttons/BigBtn_d.DDS")
```

#### Ext.PlayerHasExtender(playerGuid)

Returns whether the player that controls the character `playerGuid` has a compatible Script Extender version installed.
Example:
```lua
for i,player in  ipairs(Osi.DB_IsPlayer:Get(nil)) do
    if  not Ext.PlayerHasExtender(player[1]) then
        OpenMessageBox(player[1], "Install the extender!!!")
    end
end
```

#### Ext.Utils.MonotonicTime()

Returns a monotonic value representing the current system time in milliseconds. Useful for performance measurements / measuring real world time.
(Note: This value is not synchronized between peers and different clients may report different time values!)

Example:
```lua
local startTime = Ext.MonotonicTime()
DoLongTask()
local endTime = Ext.MonotonicTime()
Ext.Utils.Print("Took: " .. tostring(endTime - startTime) .. " ms")
```

## JSON Support

Two functions are provided for parsing and building JSON documents:
 - `Ext.Json.Parse(json: string)`
 - `Ext.Json.Stringify(value[, beautify: bool = true, stringifyInternalTypes: bool = false, iterateUserdata: bool = false]) : string`.

Lua types are encoded in JSON (and vice versa) using the following table:

| Lua Type | JS Type |
|--|--|
| `nil` | `null` |
| `boolean` | `boolean` |
| `number` (integer) | `number` |
| `number` (float) | `number` |
| `string` | `string` |
| `table` (sequential numeric keys [1..n]) | `array` |
| `table` (non-sequential keys) | `object` |

It is not possible to stringify/parse `lightuserdata`, `function` and `thread` values.
Iterable `userdata` values are stringified if the `iterateUserdata` option is set, however JSON decoding them will result in a table, not a userdata object. Non-iterable `userdata` types cannot be stringified.

Since JSON only supports string object keys, Lua `number` (integer/float) keys are saved as `string`.

Usage example:
```lua
local tab = {
    asd = 1234,
    arr = {
        "ab", "bc", 44
    }
}

local json = Ext.Json.Stringify(tab)
Ext.Utils.Print(json)

local decoded = Ext.Json.Parse(json)
Ext.Utils.Print(decoded.arr[1])
```

Expected output:
```
{
    "arr": [
        "ab",
        "bc",
        44
    ],
    "asd" : 1234
}

ab
```

<a id="engine-events"></a>
# Engine Events

<a id="event-load-events"></a>
## Load Events

### ModuleLoadStarted

The `ModuleLoadStarted` event is thrown when the engine has started loading mods. Mod data (stats, localization, root templates, etc.) is not yet loaded when this listener is called, so most mod editing functionality (eg. `Ext.StatSetAttribute`) is inaccessible.
The purpose of this event is to allow adding filesystem-level hooks using `Ext.AddPathOverride` before mod data is loaded.

### StatsLoaded

`StatsLoaded` is thrown after stats entries (weapons, skills, etc.) were cleared and subsequently reloaded. Stat modifications that are valid for every game session should be applied here.

### ModuleLoading

`ModuleLoading` is thrown after the stats manager has finished loading.

### SessionLoading

`SessionLoading` is thrown when the the engine has started setting up a game session (i.e. new game, loading a savegame or joining a multiplayer game).

### SessionLoaded

`SessionLoaded` is thrown when the game session was set up.

<!---
### TODO
 - GameObject calls - HasTag, GetTags, GetStatus, GetStatusByType, GetStatuses + GetInventoryItems
 - GetCharacter, GetItem, GetProjectile, GetGameObject, GetStatus (client/server) + updated property maps
 - RegisterNetListener + networking concepts
 - File IO
 - Reloading Lua and changing exports
-->



<a id="osiris-preprocessor"></a>
## Osiris Preprocessor <sup>S</sup>

To support mods that may not want to depend on the extender but want to take advantages of its features when available, two "preprocessor" constructs are provided that allow conditional compilation of code when the extender is present / not present.
To make use of this feature, enable the `PreprocessStory` variable in [your extension config](#extension-setup).

The first construct allows defining code that only runs when the extender is loaded. To achieve this, the block comment `/* ... */` is tagged so the extender can uncomment the code during compilation if it is present.
Syntax:
```c
/* [EXTENDER_ONLY]
// Code in this block is normally commented out; however,
// the leading and trailing comment tags are removed if
// the extender is present.
IF
Whatever()
THEN
DB_NOOP(1);
*/
```

The second construct is the opposite, i.e. it only removes code when the extender is loaded.
Syntax:
```c
// [BEGIN_NO_EXTENDER]
// This code is executed if the extender is not loaded.
// With the extender, the code between "// [BEGIN..." and "// [END..." is removed entirely.
IF
Whatever()
THEN
DB_NOOP(1);
// [END_NO_EXTENDER]
```

Example usage:
```c
IF
TextEventSet("preprocessor")
THEN
/* [EXTENDER_ONLY]
DebugBreak("This code only runs if the script extender is loaded");
*/
DebugBreak("This always runs");
// [BEGIN_NO_EXTENDER]
DebugBreak("This only runs if the script extender is *NOT* loaded");
// [END_NO_EXTENDER]
```

<a id="osiris-data-types"></a>
## Osiris Data Types <sup>S</sup>
The data types mentioned for Osiris functions should be interpreted as follows:

#### Integer
A 32-bit signed integer; equivalent to the Osiris `INTEGER` type. Can be read/written using the `XyzGetInt()` and `XyzSetInt()` getter/setter functions.

#### Real
Equivalent to the Osiris `REAL` type. Can be read/written using the `XyzGetReal()` and `XyzSetReal()` getter/setter functions.

#### String
Equivalent to the Osiris `STRING` type. Can be read/written using the `XyzGetString()` and `XyzSetString()` getter/setter functions.

#### Flag
An integer value with two allowed values, 0 and 1. 
These are essentially boolean values, but since Osiris lacks boolean support, they are passed as integers. Can be read/written using the `XyzGetInt()` and `XyzSetInt()` getter/setter functions.

#### Enum
An integer value with a list of allowed values. The values can be found in `Enumerations.xml` if it is a standard enumeration, or in the [Enumerations](#enumerations) section of this document if it's a non-standard/undocumented enumeration.

An enum property can be read/written using its index with the `XyzGetInt()` and `XyzSetInt()` getter/setter functions, or using its textual label with the `XyzGetString()` and `XyzSetString()` getter/setter functions.
Example:
```c
// Set DamageSourceType to SurfaceCreate by ID
NRD_StatusSetInt(_Char, _Status, "DamageSourceType", 2);
// Set DamageSourceType to SurfaceCreate by name
NRD_StatusSetString(_Char, _Status, "DamageSourceType", "SurfaceCreate"); 
...
// GetDamageSourceType by ID (_Val == 2)
NRD_StatusGetInt(_Char, _Status, "DamageSourceType", _Val);
// GetDamageSourceType by name(_Val == "SurfaceCreate")
NRD_StatusGetString(_Char, _Status, "DamageSourceType", _Val); 
```

<a id="osiris-functions"></a>
## Osiris Functions <sup>S</sup>

<a id="osi-stat-functions"></a>
### Stat functions

These functions can be used to query stats entries.

#### StatExists
`query NRD_StatExists([in](STRING)_StatsId)`

Checks whether the specified stat entry exists. The query succeeds if the stat entry exists, and fails if it does not.

#### StatAttributeExists
`query NRD_StatAttributeExists([in](STRING)_StatsId, [in](STRING)_Attribute)`

Checks whether the stat entry `_StatsId` has an attribute (data) named `_Attribute`. The query succeeds if the attribute exists, and fails if it does not.

#### StatGetInt
`query NRD_StatGetInt([in](STRING)_StatsId, [in](STRING)_Attribute, [out](INTEGER)_Value)`

Returns the specified `_Attribute` of the stat entry.
If the stat entry does not exist, the stat entry doesn't have an attribute named `_Attribute`, or the attribute isn't convertible to integer, the query fails.

**Notes:**
 - For enumerations, the function will return the index of the value in the enumeration. eg. for Damage Type `Corrosive`, it will return 3.

#### StatGetString
`query NRD_StatGetString([in](STRING)_StatsId, [in](STRING)_Attribute, [out](STRING)_Value)`

Returns the specified `_Attribute` of the stat entry.
If the stat entry does not exist, the stat entry doesn't have an attribute named `_Attribute`, or the attribute isn't convertible to string, the query fails.

**Notes:**
 - For enumerations, the function will return the name of the enumeration value (eg. `Corrosive`).

#### StatGetType
`query NRD_StatGetType([in](STRING)_StatsId, [out](STRING)_Type)`

Returns the type of the specified stat entry. If the stat entry does not exist, the query fails.
Possible return values: `Character`, `Potion`, `Armor`, `Object`, `Shield`, `Weapon`, `SkillData`, `StatusData`.

#### StatGetExtraData
`query NRD_StatGetExtraData([in](STRING)_Key, [out](REAL)_Value)`

Returns the specified key from `Data.txt`. If the key does not exist, the query fails.
 
<a id="osi-math-functions"></a>
### Math functions

#### Sin
`query NRD_Sin([in](REAL)_In, [out](REAL)_Out)`

Computes the sine of the argument `_In` (measured in radians).
  

#### Cos
`query NRD_Cos([in](REAL)_In, [out](REAL)_Out)`

Computes the cosine of the argument `_In` (measured in radians).
  

#### Tan
`query NRD_Tan([in](REAL)_In, [out](REAL)_Out)`

Computes the tangent of the argument `_In` (measured in radians).
  

#### Round
`query NRD_Round([in](REAL)_In, [out](REAL)_Out)`

Computes the nearest integer value to the argument `_In`, rounding halfway cases away from zero.
  

#### Ceil
`query NRD_Ceil([in](REAL)_In, [out](REAL)_Out)`

Computes the smallest integer value not less than the argument.


#### Floor
`query NRD_Floor([in](REAL)_In, [out](REAL)_Out)`

Computes the largest integer value not greater than the argument.
  

#### Abs
`query NRD_Abs([in](REAL)_In, [out](REAL)_Out)`

Computes the absolute value of the argument.
  

#### Pow
`query NRD_Pow([in](REAL)_Base, [in](INTEGER)_Exp, [out](REAL)_Out)`

Computes the value of `_Base` raised to the power `_Exp`.
  

#### Sqrt
`query NRD_Sqrt([in](REAL)_In, [out](REAL)_Out)`

Computes the square root of `_In`.
  

### Exp
`query NRD_Exp([in](REAL)_In, [out](REAL)_Out)`

Computes `e` (Euler's number, 2.7182818...) raised to the given power `_In`.

#### Log
`query NRD_Log([in](REAL)_In, [out](REAL)_Out)`

Computes the natural (base `e`) logarithm of `_In`.


#### Factorial
`query NRD_Factorial([in](INTEGER)_In, [out](INTEGER)_Out)`

Computes the factorial of the value `_In`.

#### Random
```
query NRD_Random([in](INTEGER)_Min, [in](INTEGER)_Max, [out](INTEGER)_Result)
query NRD_RandomReal([in](REAL)_Min, [in](REAL)_Max, [out](REAL)_Result)
```

Returns uniformly distributed random numbers in the range [`_Min` ... `_Max`].

#### Min
```
query NRD_Min([in](INTEGER)_A, [in](INTEGER)_B, [in](INTEGER)_C, [out](INTEGER)_Result)
query NRD_Min([in](INTEGER)_A, [in](INTEGER)_B, [in](INTEGER)_C, [in](INTEGER)_D, [out](INTEGER)_Result)
query NRD_MinReal([in](REAL)_A, [in](REAL)_B, [in](REAL)_C, [out](REAL)_Result)
query NRD_MinReal([in](REAL)_A, [in](REAL)_B, [in](REAL)_C, [in](REAL)_D, [out](REAL)_Result)
```

Returns the smallest input value (`_A`, `_B`, `_C` or `_D`).

#### Max
```
query NRD_Max([in](INTEGER)_A, [in](INTEGER)_B, [in](INTEGER)_C, [out](INTEGER)_Result)
query NRD_Max([in](INTEGER)_A, [in](INTEGER)_B, [in](INTEGER)_C, [in](INTEGER)_D, [out](INTEGER)_Result)
query NRD_MaxReal([in](REAL)_A, [in](REAL)_B, [in](REAL)_C, [out](REAL)_Result)
query NRD_MaxReal([in](REAL)_A, [in](REAL)_B, [in](REAL)_C, [in](REAL)_D, [out](REAL)_Result)
```

Returns the greatest input value (`_A`, `_B`, `_C` or `_D`).

 <a id="osi-string-functions"></a>
### String functions

#### StringCompare
`query NRD_StringCompare([in](STRING)_A, [in](STRING)_B, [out](INTEGER)_Result)`

Compare strings `A` and `B` using lexicographic ordering.
**Value of `Result`:**
 - -1, if `A` comes before `B`
 - 0, if `A` is equal to `B`
 - 1, if `A` comes after `B`

#### StringLength
`query NRD_StringLength([in](STRING)_String, [out](INTEGER)_Length)`

Computes the length of the string `_String`, and returns it in `_Length`.

#### StringFormat
`query NRD_StringFormat([in](STRING)_Format, [out](STRING)_Result, [_Arg1], [_Arg2], [_Arg3], ...)`

Substitutes the placeholders `[1]`, `[2]`, ... with the function arguments `_Arg1`, `_Arg2` and returns the formatted string in `_Result`.
The arguments accept any type (`INTEGER`, `REAL`, ...) as long as it is passed using a `STRING` typecast (i.e. `(STRING)_IntVariable`).

Example:
```c
...
Random(_Modulo, _Random)
AND
NRD_StringFormat("Random modulo [1], number [2]", _Fmt, (STRING)_Modulo, (STRING)_Random)
THEN
DebugBreak(_Fmt);
```

#### Substring
`query NRD_Substring([in](STRING)_String, [in](INTEGER)_From, [in](INTEGER)_Length, [out](STRING)_Result`

Returns a substring of length `_Length` from the position `_From`.
Rules:
 - If `_From` is past the end of the string an empty string is returned.
 - If `_Length` is past the end of the string, the portion of the string between `_From` and the end of the string is returned.
 - If `_From` is negative, `_From` is treated as an offset from the end of the string (i.e. for a 8 character string, a `_From` value of `-2` means offset `6`); this rule is helpful for fetching the last N characters of a string
 - If `_Length` is negative, `_Length` is treated as an offset from the end of the string

Examples:
```c
NRD_Substring("ABCDEFGH", 0, 3, _Res) // "ABC"
NRD_Substring("ABCDEFGH", 9999, 3, _Res) // ""
NRD_Substring("ABCDEFGH", 0, 9999, _Res) // "ABCDEFGH"
NRD_Substring("ABCDEFGH", -2, 2, _Res) // "GH"
NRD_Substring("ABCDEFGH", 0, -2, _Res) // "ABCDEF"
NRD_Substring("ABCDEFGH", 3, -2, _Res) // "DEF"
```

#### RegexMatch
`query NRD_RegexMatch([in](STRING)_String, [in](STRING)_Regex, [in](INTEGER)_FullMatch, [out](INTEGER)_Result)`

Matches the string `_String` against the ECMAScript regex pattern `_Regex`. If `_FullMatch` is 1, the whole string much match the pattern, otherwise a partial match is allowed. The query fails if the specified pattern is not a valid ECMAScript regex pattern . The query returns `1` if the pattern matches the string and `0` otherwise.
Because of limitations in the Osiris parser, the character `^` must be replaced with `#`.

Examples:
```c
NRD_RegexMatch("GetValue", "Get|GetValue", 0, _Ret) // _Ret = 1
NRD_RegexMatch("GetValues", "Get|GetValue", 1, _Ret) // _Ret = 0
```

#### RegexReplace
`query NRD_RegexReplace([in](STRING)_String, [in](STRING)_Regex, [in](STRING)_Replacement, [out](STRING)_Result)`

Replaces all occurrences of the ECMAScript regex pattern `_Regex` in `_String` with the replacement pattern `_Replacement` and returns the result in `_Result`. The query fails if the specified pattern is not a valid ECMAScript regex. 

Examples:
```c
NRD_RegexReplace("Quick brown fox", "a|e|i|o|u", "[$&]", (STRING)_Ret)
// _Ret = "Q[u][i]ck br[o]wn f[o]x"
```

#### GuidString
`query NRD_GuidString([in](STRING)_String, [out](GUIDSTRING)_Result)`

Attempts to convert `_String` to a GuidString. If the conversion succeeds (i.e. the string is a valid NameGUID), the value is returned in `_Result`. If `_String` is not a valid NameGUID, the query fails.


#### StringToInt
`query NRD_StringToInt([in](STRING)_String, [out](INTEGER)_Result)`

Attempts to convert `_String` to an integer value. If the conversion succeeds (i.e. the string is a valid integer), the integer value is returned in `_Result`. If `_String` is not a valid integer, the query fails.

For detailed rules [check the reference](https://en.cppreference.com/w/cpp/string/basic_string/stol)


#### StringToReal
`query NRD_StringToReal([in](STRING)_String, [out](REAL)_Result)`

Attempts to convert `_String` to a real value. If the conversion succeeds (i.e. the string is a valid real), the real value is returned in `_Result`. If `_String` is not a valid real value, the query fails.

For detailed rules see [check the reference](https://en.cppreference.com/w/cpp/string/basic_string/stof)


#### IntegerToString
`query NRD_IntegerToString([in](INTEGER)_Integer, [out](STRING)_Result)`

Converts `_Integer` to a string value.

#### RealToString
`query NRD_RealToString([in](REAL)_Real, [out](STRING)_Result)`

Converts `_Real` to a string value.

<a id="osi-misc-functions"></a>
### Miscellaneous functions

#### ShowErrorMessage
`call NRD_ShowErrorMessage((STRING)_Message)`

Displays an error dialog box with the text `_Message`.


#### DebugLog 
`call NRD_DebugLog((STRING)_Message)`

Functionally equivalent to `DebugBreak()`, except that the `_Message` argument accepts any type, not only strings. To use non-string arguments, cast the variable to the appropriate type.

Example usage:  
```c
IF
StoryEvent((ITEMGUID)_Item, "TEST")
THEN
NRD_DebugLog((STRING)_Item);
```


#### ForLoop
```
call NRD_ForLoop((STRING)_Event, (INTEGER)_Count)
call NRD_ForLoop((GUIDSTRING)_Object, (STRING)_Event, (INTEGER)_Count)
event NRD_Loop((STRING)_Event, (INTEGER)_Num)
event NRD_Loop((GUIDSTRING)_Object, (STRING)_Event, (INTEGER)_Num)
```

Counts from 0 up to `_Count - 1` and throws loop event `_Event` for each value. Unlike regular events, `NRD_Loop` are not queued and are thrown immediately (i.e. during the `NRD_ForLoop` call), so there is no need for an additional cleanup/finalizer event.

Example usage:  
```c
// ...
NRD_ForLoop("MyMod_SomeLoopEvent", 10);

IF
NRD_Loop("MyMod_SomeLoopEvent", _Int)
THEN
NRD_DebugLog((STRING)_Int);
```
  
Example usage with GUIDs:
```c
// ...
NRD_ForLoop(CHARACTERGUID_S_GLO_CharacterCreationDummy_001_da072fe7-fdd5-42ae-9139-8bd4b9fca406, "MyMod_SomeLoopEvent", 10);
  
IF
NRD_Loop((CHARACTERGUID)_Char, "MyMod_SomeLoopEvent", _Int)
THEN
NRD_DebugLog((STRING)_Char);
NRD_DebugLog((STRING)_Int);
```
