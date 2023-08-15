### BG3SE Lua API v1 Documentation

### Table of Contents  
 - [Getting Started](#getting-started)
    - [Bootstrap Scripts](#bootstrap-scripts)
 - [Client / Server States](#client-server)
    - [Persistent Variables](#persistent-vars)
 - [Console](#console)
 - [Calling Lua from Osiris](#calling-lua-from-osiris)
    * [Capturing Events/Calls](#l2o_captures)
 - [Calling Osiris from Lua](#calling-osiris-from-lua)
    * [Calls](#o2l_calls)
    * [Queries](#o2l_queries)
    * [Events](#o2l_events)
    * [PROCs](#o2l_procs)
    * [User Queries](#o2l_qrys)
    * [Databases](#o2l_dbs)
 - [Utility functions](#ext-utility)
 - [JSON Support](#json-support)
 - [Engine Events](#engine-events)


<a id="getting-started"></a>
## Getting Started

To start using the extension in your mod, a configuration file must be created that describes what features are utilized by your mod.

Create a file at `Mods\YourMod\ScriptExtender\Config.json` with the following contents, then tweak the values as desired:
```json
{
    "RequiredVersion": 1,
    "ModTable": "YOUR_MOD_NAME_HERE",
    "FeatureFlags": ["Lua"]
}
```

Meaning of configuration keys:

| Key | Meaning |
|--|--|
| `RequiredVersion` | Osiris Extender version required to run the mod. It is recommended to use the version number of the Script Extender you used for developing the mod since the behavior of new features and backwards compatibility functions depends on this version number. |
| `ModTable` | Name of the mod in the global mod table (`Mods`) when using Lua. This name is required to use Lua scripting, and must be unique. |
| `FeatureFlags` | A list of features that the mod is using. For performance reasons it is recommended to only list features that are actually in use. |

The following features are accepted in `FeatureFlags`:

| Value| Meaning |
|--|--|
| `Lua` | Enables Lua scripting |

<a id="bootstrap-scripts"></a>
### Bootstrap Scripts  

If Lua is enabled for the mod, the extender will attempt to load `BootstrapServer.lua` on the server side, and `BootstrapClient.lua` on the client side. These scripts should be created in this folder (create the Story\RawFiles\Lua folders if necessary):
```
Mods\ModName\ScriptExtender\Lua\
```
**Required Scripts**  
| Name | State |
|--|--|
| `BootstrapServer.lua` | Server Side |
| `BootstrapClient.lua` | Client Side |

From here, these scripts can load other scripts with `Ext.Require`. The path to scripts are relative to the Lua folder, so if you had a file setup like this:
```
BootstrapClient.lua
BootstrapServer.lua
Server/SkillMechanics.lua
```
BootstrapServer would load `SkillMechanics.lua` with `Ext.Require("Server/SkillMechanics.lua")`. Script loading only needs to happen once.

See below for further information on the client/server states, as certain scripting functions are only available on a specific side (i.e. only Osiris functions work on the server-side, in unrestricted contexts).

<a id="client-server"></a>
## Client / Server States

Internally the game is split into two components, a client and a server component. When a new game is started/loaded, a new server is created and client connect to this server. The server component is only created on the host; client components are created on both the host and all peers. Because of this, the game technically always runs in multiplayer. Single player is just a special form of multiplayer where only one local peer is connected to the server.

Osiris and behavior scripts (gamescripts) always run on the server. Since Lua has access to features that require client-side code (UI modification, level scaling formulas, status chances, skill damage calculation, etc.) the extender keeps multiple Lua states: one state for the server and one for each client (including the single player "fake client"). These states are completely separated from each other and cannot access the internal state of each other (Lua globals, functions, variables).

Because they run in different environments, server and client states can access a different set of features. Functions/classes in this document are annotated with the following letters, which indicate where they are available:
 - **C** - The function is only available on the client
 - **S** - The function is only available on the server
 - **R** - Restricted; the function is only callable in special contexts/locations

<a id="persistent-vars"></a>
### Persistent Variables

The Lua state and all local variables are reset after each game reload. For keeping data through multiple play sessions it is possible to store them in the savegame by storing them in the mod-local table `Mods[ModTable].PersistentVars`. By default the table is `nil`, i.e. a mod should create the table and populate it with data it wishes to store in the savegame. The contents of `PersistentVars` is saved when a savegame is created, and restored before the `SessionLoaded` event is triggered.

(Note: There is no global `PersistentVars` table, i.e. mods that haven't set their `ModTable` won't be able to use this feature).

Example:
```lua
PersistentVars = {}
...
-- Variable will be restored after the savegame finished loading
function doStuff()
    PersistentVars['Test'] = 'Something to keep'
end

function OnSessionLoaded()
    -- Persistent variables are only available after SessionLoaded is triggered!
    Ext.Print(PersistentVars['Test'])
end

Ext.RegisterListener("SessionLoaded", OnSessionLoaded)
```


## Console

The extender allows commands to be entered to the console window.

Press `<enter>` to enter console mode; in this mode the normal log output is disabled to avoid log spam while typing commands.

Client/server context can be selected by typing `client` or `server`. This selects in which Lua environment the console commands will execute. By default the console uses the server context.
The `reset` command reinitializes the server and client Lua VM.

Typing `exit` returns to log mode.

Commands prefixed by a `!` will trigger callbacks registered via the `RegisterConsoleCommand` function.
Example:
```lua
local function testCmd(cmd, a1, a2, ...)
    Ext.Print("Cmd: " .. cmd .. ", args: ", a1, ", ", a2);
end
Ext.RegisterConsoleCommand("test", testCmd);
```
The command `!test 123 456` will call `testCmd("test", 123, 456)` and prints `Cmd: test, args: 123, 456`.

Anything else typed in the console will be executed as Lua code in the current context. (eg. typing `Ext.Print(1234)` will print `123`). 
The console has full access to the underlying Lua state, i.e. server console commands can also call builtin/custom Osiris functions, so Osiris calls like `AddExplorationExperience(GetHostCharacter(), 100)` are possible using the console.
Variables can be used just like in Lua, i.e. variable in one command can later on be used in another console command. Be careful, console code runs in global context, so make sure console variable names don't conflict with globals (i.e. `Mods`, `Ext`, etc.)! Don't use `local` for console variables, since the lifetime of the local will be one console command. (Each console command is technically a separate chunk).


## Calling Osiris from Lua <sup>S</sup>

Lua server contexts have a special global table called `Osi` that contains every Osiris symbol. In addition, built-in engine functions (calls, queries, events) are also added to the global table.

<a id="o2l_calls"></a>
### Calls

Simply call the method from Lua with the same parameters:
```lua
-- Built-in functions are in the global table (_G)
CharacterResetCooldowns(player)
-- Equivalent to the above
Osi.CharacterResetCooldowns(player)
```

Implementation detail: Technically, name resolution is only performed when the function is called, since Osiris allows multiple overloads of the same name and the function to call is resolved based on the number of arguments. Because of this, getting any key from the `Osi` table will return an object, even if no function with that name exists. Therefore, `Osi.Something ~= nil` and similar checks cannot be used to determine whether a given Osiris symbol exists.

<a id="o2l_queries"></a>
### Queries

The query behavior is a mirror of the one described in the [Exporting Lua functions to Osiris](#exporting-lua-functions-to-osiris) chapter.

For queries with zero OUT arguments, the function will return a boolean indicating whether the query succeeded (true) or failed (false).
```lua
local succeeded = SysIsCompleted("TestGoal")
```

Queries with OUT arguments will have a number of return values corresponding to the number of OUT arguments. 
```lua
-- Single return value
local player = GetHostCharacter()
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

<a id="l2o_captures"></a>
### Capturing Events/Calls

The `Ext.Ext.Osiris.RegisterListener(name, arity, event, handler)` function registers a listener that is called in response to Osiris events.
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
Ext.Osiris.RegisterListener("TurnEnded", 1, "after", function (characterGuid)
    Ext.Print("TurnEnded- " .. characterGuid)
end)
```

<a id="ext-utility"></a>
## Utility functions

#### Ext.Require(path) <sup>R</sup>

The `Ext.Require` function is the extender's version of the Lua built-in `require` function. 
The function checks if the file at `Mods/<ModuleUUID>/Story/RawFiles/Lua/<path>` was already loaded; if not, it'll load the file, store the return value of the main chunk and return it to the caller. If the file was already loaded, it'll return the stored return value.
**Note:** `Ext.Require` should only be called during module startup (i.e. when loading `BootstrapClient.lua` or `BoostrapServer.lua`). Loading Lua files after module startup is deprecated.

#### Ext.Print(...)

Prints the specified value(s) to the debug console. Works similarly to the built-in Lua `print()`, except that it also logs the printed messages to the editor messages pane.

#### Ext.AddPathOverride(originalPath, newPath)

Redirects file access from `originalPath` to `newPath`. This is useful for overriding built-in files or resources that are otherwise not moddable, eg. UI Flash files.
Make sure that the override is added as early as possible (preferably in `ModuleLoading`), as adding path overrides after the game has already loaded the resource has no effect.

Example:
```lua
Ext.AddPathOverride("Public/Game/GUI/enemyHealthBar.swf", "Public/YourMod/GUI/enemyHealthBar.swf")
```

#### Ext.MonotonicTime()

Returns a monotonic value representing the current system time in milliseconds. Useful for performance measurements / measuring real world time.
(Note: This value is not synchronized between peers and different clients may report different time values!)

Example:
```lua
local startTime = Ext.MonotonicTime()
DoLongTask()
local endTime = Ext.MonotonicTime()
Ext.Print("Took: " .. tostring(endTime - startTime) .. " ms")
```

## JSON Support

Two functions are provided for parsing and building JSON documents, `Ext.Json.Parse` and `Ext.Json.Stringify`.

Lua types are encoded in JSON (and vice versa) using the following table:

| Lua Type | JS Type |
|--|--|
| `nil` | `null` |
| `boolean` | `boolean` |
| `number` (integer) | `number` |
| `number` (float) | `number` |
| `string` | `string` |
| `table` (sequential keys) | `array` |
| `table` (non-sequential) | `object` |

It is not possible to stringify/parse `lightuserdata`, `userdata`, `function` and `thread` values.

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
Ext.Print(json)

local decoded = Ext.Json.Parse(json)
Ext.Print(decoded.arr[1])
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

`ModuleLoading` is thrown after the stats manager has finished loading; this callback is deprecated and `StatsLoaded` should be used instead.

### SessionLoading

`SessionLoading` is thrown when the the engine has started setting up a game session (i.e. new game, loading a savegame or joining a multiplayer game). Stat overrides that use Lua callbacks (`Ext.StatSetLevelScaling`) and custom UI (`Ext.CreateUI`, `Ext.RegisterUICall`, `Ext.RegisterUIInvokeListener`, etc.) should be set up here.

### SessionLoaded

`SessionLoaded` is thrown when the game session was set up.

### ResetCompleted

The `ResetCompleted` event is thrown when a `reset` console command completes on the client or server, indicating that the Lua state was reloaded.

### GameStateChanged

The `GameStateChanged` event indicates that the server/client game state changed (eg. paused, etc).

### Tick

The `Tick` event is thrown after each game engine tick.

