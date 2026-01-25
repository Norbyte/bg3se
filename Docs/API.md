# BG3SE Lua API v29 Documentation

## Table of Contents
- [Getting Started](#getting-started)
  - [Bootstrap Scripts](#bootstrap-scripts)
- [Client / Server States](#client-server)
- [SE Console](#console)
  - [Multiline Mode](#multiline-mode)
  - [Saving output to a file](#save-console-output)
- [General SE Lua Rules](#lua-general)
  - [Object Scopes](#lua-scopes)
  - [Object Behavior](#lua-objects)
  - [Parameter Passing](#lua-parameters)
  - [Enumerations](#lua-enumerations)
  - [Bitfields](#lua-bitfields)
  - [SE Events](#lua-events)
- [Calling Osiris from Lua](#calling-osiris-from-lua)
  - [Osiris Calls](#o2l_calls)
  - [Osiris Queries](#o2l_queries)
  - [Osiris Events](#o2l_events)
  - [PROCs](#o2l_procs)
  - [User Queries](#o2l_qrys)
  - [Databases](#o2l_dbs)
- [Calling Lua from Osiris](#calling-lua-from-osiris)
  - [Capturing Events/Calls](#l2o_captures)
- [Persistence](#persistence)
  - [User Variables](#user-variables)
  - [Mod Variables](#mod-variables)
  - [Synchronization](#synchronization)
  - [Caching Behavior](#caching-behavior)
- [Entity Class](#entity-class)
- [Helper/Aliased Functions](#helper-functions)
- [Networking](#networking)
  - [NetChannel API](#net-channel-api)
  - [NetChannel Examples](#net-channel-examples)
  - [Utility functions](#net-utils)
- [Noesis UI](#noesis-ui)
  - [Custom ViewModels](#noesis-viewmodels)
- [Stats](#stats)
  - [Stats Objects](#stats-objects)
  - [Reading/Writing Stat Attributes](#reading-writing-stats)
- [Input/Output (IO)](#io)
- [Timers](#timers)
- [JSON Support](#json-support)
- [Mod Info](#mod-info)
- [Utils](#utils)
- [Audio](#audio)
- [Localization](#loca)
- [Templates](#templates)
- [Static Data](#static-data)
- [Resources](#resources)
- [Level](#level)
- [Math Library](#math)
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
<!-- | `Osiris` | Enables Osiris scripting? | -->

<a id="bootstrap-scripts"></a>
### Bootstrap Scripts

If Lua is enabled for the mod, the extender will attempt to load `BootstrapServer.lua` on the server side, and `BootstrapClient.lua` on the client side. These scripts should be created in the `Mods\<YOURMOD>\ScriptExtender\Lua\` folder.

The `Ext.Require` function is the extender's version of the Lua built-in `require` function.
The function checks if the file at `Mods/<ModuleUUID>/ScriptExtender/Lua/<path>` was already loaded; if not, it'll load the file, store the return value of the main chunk and return it to the caller. If the file was already loaded, it'll return the stored return value.
**Note:** `Ext.Require` should only be called during module startup (i.e. when loading `BootstrapClient.lua` or `BoostrapServer.lua`). Loading Lua files after module startup is deprecated.

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

<a id="console"></a>
## SE Console

The extender allows commands to be entered to the console window.

Press `<enter>` to enter console mode; in this mode the normal log output is disabled to avoid log spam while typing commands.

Client/server context can be selected by typing `client` or `server`. This selects in which Lua environment the console commands will execute. By default the console uses the server context.
The `reset` command reinitializes the server and client Lua VM.

Typing `exit` returns to log mode.

Commands prefixed by a `!` will trigger callbacks registered via the `RegisterConsoleCommand` function.
Example:
```lua
local function testCmd(cmd, a1, a2, ...)
    _P("Cmd: " .. cmd .. ", args: ", a1, ", ", a2);
end
Ext.RegisterConsoleCommand("test", testCmd);
```
The command `!test 123 456` will call `testCmd("test", 123, 456)` and prints `Cmd: test, args: 123, 456`.

Anything else typed in the console will be executed as Lua code in the current context. (eg. typing `_P(1234)` will print `123`).
The console has full access to the underlying Lua state, i.e. server console commands can also call builtin/custom Osiris functions, so Osiris calls like `AddExplorationExperience(GetHostCharacter(), 100)` are possible using the console.
Variables can be used just like in Lua, i.e. variable in one command can later on be used in another console command. Be careful, console code runs in global context, so make sure console variable names don't conflict with globals (i.e. `Mods`, `Ext`, etc.)! Don't use `local` for console variables, since the lifetime of the local will be one console command. (Each console command is technically a separate chunk).

<a id="multiline-mode"></a>
### Multiline mode

One unique feature of the console is its multiline mode, which facilitates the input of multiple lines of code at once. To enter multiline mode, start your input with "--\[\[" followed by pressing the enter key. This signals the console to expect multiple lines of code. Once you've entered all your lines, close the multiline mode with "]]--".

Here's an example demonstrating the usage of multiline mode:

```
--[[
someTable={1,2,3,4,5}
for key,value in pairs(someTable) do
    print(value)
end
]]--
```

<a id="save-console-output"></a>
### Saving the console output to a file

Sometimes, the output of a command in the console might be too lengthy or complex to analyze effectively within the console interface. In such cases, you might prefer to save the output to a file for easier review. This can be accomplished using a combination of two functions: `Ext.IO.SaveFile(filename, content)` and `Ext.DumpExport(object)`.

Here's how it works:

`Ext.IO.SaveFile(filename, content)`: This function allows you to save content to a file with a specified filename. You need to provide the filename and the content you want to save. If the directory specified in the filename does not exist, `SaveFile` will automatically create it.

`Ext.DumpExport(object)`: This function serializes the given object into a string, which can then be saved to a file using `Ext.IO.SaveFile`.

Here's an example illustrating how to save console output to a file:

```lua
entity=Ext.Entity.Get("0133f2ad-e121-4590-b5f0-a79413919805") --Wither's UUID, for demonstration purposes
Ext.IO.SaveFile("output.json", Ext.DumpExport(entity:GetAllComponents()))
```

The saved file will be located in the Script Extender folder, typically found at `%localappdata%\Larian Studios\Baldur's Gate 3\Script Extender`

This method provides a convenient way to store and analyze complex console output, allowing for easier debugging and analysis outside of the console environment.

<a id="lua-general"></a>
## General SE Lua Rules

<a id="lua-scopes"></a>
### Object Scopes

Previously, a `userdata` (game object passed from the extender to Lua, i.e. `Character`, `Status`, etc.) returned from an API call or passed via a parameter was valid for an infinite duration. This meant that the object could be accessed anytime, potentially well after the actual object in the engine was destroyed, leading to strange crashes.

Example of possible crash:
```lua
local spells = Ext.Entity.Get(...).SpellBook.Spells

Ext.OnNextTick(function (...)
    -- Spell might get deleted beforehand
    -- POSSIBLE CRASH!
    local uuid = spells[2].SpellUUID
end)
```

To fix these issues, most `userdata` types are now bound to their enclosing *extender scope*. Since the engine always deletes game objects at the end of the game loop, it is guaranteed that eg. a Status or Character won't disappear during a Lua call, but they may be gone afterwards. To rectify this, "smuggling" objects outside of listeners is no longer allowed. Example:

```lua
local spellbook = Ext.Entity.Get(...).SpellBook

Ext.OnNextTick(function (...)
    -- Throws "Attempted to read object of type 'SpellBookEntry' whose lifetime has expired"
    local uuid = spellbook.Spells[2].SpellUUID
end)
```

This rule also applies to objects you fetch manually during a listener:

```lua
local spellbook

Ext.Events.SessionLoaded:Subscribe(function (event)
    spellbook = Ext.Entity.Get(...).SpellBook
end)

Ext.OnNextTick(function (...)
    -- Throws "Attempted to read object of type 'SpellBookEntry' whose lifetime has expired"
    local uuid = spellbook.Spells[2].SpellUUID
end)
```

Subproperties inherit the lifetime of their parent object, eg. if you keep a reference to one specific spell (`local spellbook = Ext.Entity.Get(...).SpellBook.Spells[2]`), its lifetime will expire at the same time as the characters'.

This ensures that no potentially deleted objects are not accessed in risky contexts.


<a id="lua-objects"></a>
### Object Behavior

Attempting to read or write properties that don't exist on an object class now lead to a Lua error:
```lua
local spellbook = Ext.Entity.Get(...).SpellBook
-- Object of type 'SpellBookComponent' has no property named 'PreparedSpells'
_P(spellbook.PreparedSpells)
```

The properties and methods of all engine objects can be read using iteration (metatables now support `__pairs`):
```lua
local spell = Ext.Entity.Get(...).SpellBook.Spells[1]
for property, value in pairs(spell) do
    _P(property, value)
end
```

Stringifying an engine object returns its class and instance ID (i.e. metatables now support `__tostring`):
```lua
-- Prints "SpellBookEntry (00000209C32D16F0)"
_P(tostring(Ext.Entity.Get(GetHostCharacter()).SpellBook.Spells[1]))
```

Equality checks on engine objects return whether the two references point to the _same_ object:
```lua
_P(Ext.Entity.Get(GetHostCharacter()) == Ext.Entity.Get(GetHostCharacter()))
```

Array-like engine objects support iteration via `ipairs()` and their length can be read using the `#` operator (i.e. the `__len` and `__pairs` metamethods are now supported):
```lua
local tags = _C().Tag.Tags
_P("Number of tags: ", #tags)
for i, tag in ipairs(tags) do
    _P(i, tag)
end
```

<a id="lua-parameters"></a>
### Parameter Passing

 - Numeric enum values and numeric bitmask values passed to API calls are validated; a Lua error is thrown if an unsupported enum label or bitfield value is passed.

 - All bitmask parameters (eg. `PropertyContext`) support passing numeric values, strings and tables to specify the flags, i.e. the allowed ways to pass bitmasks are:
    - Integer (i.e. `3` means "Target and AoE" for `PropertyContext`)
    - String (i.e. `"Target"`) - note that this only supports passing a single value!
    - Table (i.e. `{"Target", "AoE"}`)

<a id="lua-enumerations"></a>
### Enumerations

Enum values returned from functions and enum properties are returned as `userdata` (lightcppobject) values instead of `string`.

```lua
_D(type(_C().CurrentTemplate.BloodSurfaceType)) -- "userdata"
```

Enum values have `Label`, `Value` and `EnumName` properties that can be queried to fetch the textual name, numeric value and enumeration name respectively.
```lua
local bt = _C().CurrentTemplate.BloodSurfaceType
_D(bt.Label) -- "Blood"
_D(bt.Value) -- 16
_D(bt.EnumName) -- "SurfaceType"
```

Enum values implement `__tostring` for backwards compatibility with old string enums

```lua
print(_C().CurrentTemplate.BloodSurfaceType) -- "Blood"
```

Enum values support comparison with other enum values, enum labels (names) and numeric values:

```lua
local bt = _C().CurrentTemplate.BloodSurfaceType
_D(bt) -- "Blood"
_D(bt == "Blood") -- true
_D(bt == "something else") -- false
_D(bt == 16) -- true
_D(bt == 15) -- false
_D(bt == Ext.Enums.SurfaceType.Blood) -- true
_D(bt == Ext.Enums.SurfaceType.Web) -- false
```

Enum properties support assignment of other enum values, enum labels (names) and numeric values:

```lua
-- assignment by enum label
_C().CurrentTemplate.BloodSurfaceType = "Blood"
-- assignment by enum value
_C().CurrentTemplate.BloodSurfaceType = 16
-- assignment by enum object
_C().CurrentTemplate.BloodSurfaceType = Ext.Enums.SurfaceType.Blood
```

Using enum values as table keys turns them into strings for backwards compatibility reasons:

```lua
local t = { Web = 123 }
_D(t[Ext.Enums.SurfaceType.Web]) -- prints 123
```

JSON serialization turns enum values into their string representation.

```lua
print(Ext.Json.Stringify(Ext.Enums.SurfaceType.Web))
-- "Web"
```

<a id="lua-bitfields"></a>
### Bitfields

Bitfields returned from functions and enum properties are `userdata` (lightcppobject) values instead of `table`.

```lua
_D(type(_C().Stats.AttributeFlags)) -- "userdata"
```

Bitfields have `__Labels`, `__Value` and `__EnumName` properties that can be queried to fetch a table containing all textual names, a numeric value representing all values and the enumeration name respectively.
```lua
local af = _C().Stats.AttributeFlags
_D(af) -- ["SuffocatingImmunity", "BleedingImmunity", "DrunkImmunity"]
_D(af.__Labels) -- ["SuffocatingImmunity", "BleedingImmunity", "DrunkImmunity"]
_D(af.__Value) -- 137440004096
_D(af.__EnumName) -- "StatAttributeFlags"
```

They also support querying the state of each bitfield flag (either by label or by numeric value):
```lua
local af = _C().Stats.AttributeFlags
_D(af.DrunkImmunity) -- true
_D(af.WebImmunity) -- false
```

Bitfields support table-like iteration (i.e. `pairs`/`ipairs`):
```lua
for k,v in pairs(af) do
   print(k,v)
end
-- 1       BleedingImmunity
-- 2       DrunkImmunity
-- 3       SuffocatingImmunity
```

Bitfields implement `__tostring` that returns a string containing the enum type and all labels:

```lua
-- "StatAttributeFlags(SuffocatingImmunity,BleedingImmunity,DrunkImmunity)"
print(_C().Stats.AttributeFlags)
```

Bitfields support the `~` (bitwise negate) unary operator and the `|` (bitwise or), `&` (bitwise and) and `~` (bitwise xor) binary operators. All binary operators support bitfields, string bitfield labels, tables of bitfield labels and numeric values as their second operand:

```lua
local af = _C().Stats.AttributeFlags
_D(af) -- ["SuffocatingImmunity", "BleedingImmunity", "DrunkImmunity"]
_D(~af) -- ["Unstorable", "DisarmedImmunity", "PoisonImmunity", "HastedImmunity", ...]
_D(af & {"DrunkImmunity", "BleedingImmunity"}) -- ["BleedingImmunity", "DrunkImmunity"]
_D(af & Ext.Enums.StatAttributeFlags.DrunkImmunity) -- ["DrunkImmunity"]
_D(af | "FreezeImmunity") -- ["FreezeImmunity", "SuffocatingImmunity", "BleedingImmunity", "DrunkImmunity"]
_D(af ~ 0x802) -- ["SuffocatingImmunity", "DrunkImmunity", "BurnImmunity"]
```


Bitfields support comparison with other bitfields, singular bitfield labels, tables of bitfield labels and numeric values:

```lua
local af = _C().Stats.AttributeFlags
_D(af) -- ["SuffocatingImmunity", "BleedingImmunity", "DrunkImmunity"]
_D(af == {"SuffocatingImmunity", "BleedingImmunity", "DrunkImmunity"}) -- true
_D(af == {"SuffocatingImmunity", "BleedingImmunity"}) -- false
_D(af == 137440004096) -- true
_D(af == 1234) -- false
```

Bitfields support assignment of other enum values, enum labels (names) and numeric values:

```lua
-- assignment by enum label
_C().Stats.AttributeFlags = {"SuffocatingImmunity", "BleedingImmunity"}
-- assignment by enum value
_C().Stats.AttributeFlags = 137440004096
-- assignment by enum object
_C().Stats.AttributeFlags = Ext.Enums.StatAttributeFlags.WebImmunity
-- assigning result of bitfield operation
_C().Stats.AttributeFlags = _C().Stats.AttributeFlags | "WebImmunity"
```

JSON serialization turns bitfields into an array of textual labels.

```lua
print(Ext.Json.Stringify(_C().Stats.AttributeFlags))
-- ["SuffocatingImmunity", "BleedingImmunity", "DrunkImmunity"]
```

It should be noted that bitfields are always passed by value, so appending or removing elements from them like a table is not possible:

```lua
local af = _C().Stats.AttributeFlags
-- throws "attempt to index a userdata value (global 'af')"
af.SuffocatingImmunity = false
-- throws "bad argument #1 to 'insert' (table expected, got light C++ object)"
table.insert(af, "WebImmunity")
```

<a id="lua-events"></a>
### SE Events

Subscribing to engine events can be done through the `Ext.Events` table.

Example:
```lua
Ext.Events.GameStateChanged:Subscribe(function (e)
    _P("State change from " .. e.FromState .. " to " .. e.ToState)
end)
```

The `Subscribe()` method accepts an optional options table that contains additional settings:
```lua
Ext.Events.GameStateChanged:Subscribe(handler, {
    Priority = 50,
    Once = true
})
```

The `Priority` setting determines the order in which subscribers are called; subscribers with lower priority are called first. The default priority is 100.
If the `Once` flag is set, the event is only triggered once and the handler is automatically unsubscribed after the first call.

The `Subscribe()` method returns a handler index that can be used to cancel the subscription later on:
```lua
local handlerId = Ext.Events.GameStateChanged:Subscribe(handler)
...
Ext.Events.GameStateChanged:Unsubscribe(handlerId)
```
<a id="calling-osiris-from-lua"></a>
## Calling Osiris from Lua

Lua server contexts have a special global table called `Osi` that contains every Osiris symbol. In addition, built-in engine functions (calls, queries, events) are also added to the global table.

<a id="o2l_calls"></a>
### Osiris Calls

Simply call the method from Lua with the same parameters:
```lua
-- Built-in functions are in each mod's global table (_G). This will not work in the console!
CharacterResetCooldowns(player)
-- Equivalent to the above
Osi.CharacterResetCooldowns(player)
```

<!-- Implementation detail: Technically, name resolution is only performed when the function is called, since Osiris allows multiple overloads of the same name and the function to call is resolved based on the number of arguments. Because of this, getting any key from the `Osi` table will return an object, even if no function with that name exists. Therefore, `Osi.Something ~= nil` and similar checks cannot be used to determine whether a given Osiris symbol exists. -->

<a id="o2l_queries"></a>
### Osiris Queries

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
### Osiris Events

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
## Calling Lua from Osiris

<a id="l2o_captures"></a>
### Capturing Events/Calls

The `Ext.Osiris.RegisterListener(name, arity, event, handler)` function registers a listener that is called in response to Osiris events.
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
    _P("TurnEnded- " .. characterGuid)
end)
```

## Persistence

The Lua state and all local variables are reset after each game reload. To persist data across multiple play sessions, use ModVars, UserVars, or [MCMVars](https://wiki.bg3.community/Tutorials/Mod-Frameworks/mod-configuration-menu#mcm-api-functions). PersistentVars are deprecated and should not be used; prefer one of the aforementioned alternatives instead.

<a id="user-variables"></a>
### User variables

v10 adds support for attaching custom properties to entities. These properties support automatic network synchronization between server and clients as well as savegame persistence.

To use custom variables, the variable name must first be registered with the variable manager:
```lua
Ext.Vars.RegisterUserVariable("NRD_Whatever", {
    Server = true,
    Client = true,
    SyncToClient = true
})
```

The `RegisterUserVariable` method accepts two parameters, a variable name and an optional list of settings.
The following settings are supported:
| Setting | Default | Meaning |
|-|-|-|
| `Server` | true | Variable is present on server entities |
| `Client` | false | Variable is present on client entities |
| `WriteableOnServer` | true | Variable can be modified on server side |
| `WriteableOnClient` | false | Variable can be modified on client side |
| `Persistent` | true | Variable is written to/restored from savegames |
| `SyncToClient` | false | Server-side changes to the variable are synced to all clients |
| `SyncToServer` | false | Client-side changes to the variable are synced to the server |
| `SyncOnTick` | true | Client-server sync is performed once per game loop tick |
| `SyncOnWrite` | false | Client-server sync is performed immediately when the variable is written. This is disabled by default for performance reasons. |
| `DontCache` | false | Disable Lua caching of variable values (see below) |

Usage notes:
 - Since variable prototypes are used for savegame serialization, network syncing, etc., they must be registered before the savegame is loaded and every time the Lua context is reset; performing the registration when `BootstrapServer.lua` or `BootstrapClient.lua` is loaded is recommended
 - Although the variables registered server-side and client-side can differ, it is recommended to register all variables on both sides (even if they're server-only or client-only) for consistency
 - Variable names, much like Osiris DB names are global; it is recommended to prefix them with your mod name to ensure they're unique
 - Variables must be registered with the same settings on both client and server, otherwise various synchronization issues may occur.
 - Client-only variables cannot be persistent.


After registration, custom variables can be read/written through the `Vars` property on entities:
```lua
_C().Vars.NRD_Whatever = 123
Ext.Print(_C().Vars.NRD_Whatever)
```

<a id="mod-variables"></a>
### Mod variables

Mod variables are the equivalent of user variables for mods; i.e. they store and synchronize a set of variables for each mod. Mod variables are mostly functionally identical to user variables, so only the differences are highlighted here.

To use a mod variable, the variable must first be registered with the variable manager:
```lua
Ext.Vars.RegisterModVariable(ModuleUUID, "VariableName", {
    Server = true, Client = true, SyncToClient = true
})
```

Mod variable registrations are kept separate for each mod UUID, so there is no need to use unique prefixes for variables since a mod registering a variable in its own table will have no effect on other mods.

The variables for a mod can be accessed by calling `Ext.Vars.GetModVariables(ModuleUUID)`:
```lua
local vars = Ext.Vars.GetModVariables(ModuleUUID)
Ext.Print(vars.VariableName)
vars.VariableName = 123
```

`Ext.Vars.SyncModVariables([moduleUuid])` can be called to perform an immediate synchronization of all mod variable changes.

### Synchronization

A variable is only eligible for synchronization if:
 - Both `Server` and `Client` flags are set
 - For server to client synchronization, both `WriteableOnServer` and `SyncToClient` flags are set
 - For client to server synchronization, both `WriteableOnClient` and `SyncToServer` flags are set

For a variable to be synchronized, it must be *dirtied* first. The most straightforward way to perform this is by doing a direct write to the variable:
```lua
_C().Vars.NRD_Whatever = "asd"
```

Note: Writes to subproperties of complex types (i.e. tables etc) will not trigger this mechanism! Example:
```lua
_C().Vars.NRD_Whatever.SomeProperty = 123
```
Since the `__newindex` metamethod of the `Vars` object is not called, the variable manager does not detect that a change was performed. A simple fix is to reassign the property after modifications were made:
```lua
local v = _C().Vars.NRD_Whatever
v.SomeProperty = 123
_C().Vars.NRD_Whatever = v
```

On each tick of the game loop, variables that were changed during the current tick are collected and sent to the client/server in a batch. Unless configured otherwise (i.e. the `SyncOnTick` setting is disabled), this is the default synchronization method.

If a change to a user variable must be visible by the peer before the end of the current tick:
 - The `SyncOnWrite` flag can be enabled which ensures that the write is immediately sent to client/server without additional wait time.
 - `Ext.Vars.SyncUserVariables()` can be called, which synchronizes all user variable changes that were done up to that point


### Caching behavior

The variable manager keeps a Lua copy of table variables for performance reasons. This means that instead of unserializing the table from JSON each time the property is accessed, the cached Lua version is returned after the first access. This means that subsequent accesses to the property will return the same reference and writes to the property.

Example:
```lua
local t1 = _C().Vars.NRD_Whatever
local t2 = _C().Vars.NRD_Whatever
t1.Name = "test"
_D(t2.Name) -- prints "test"
```

Cached variables are serialized to JSON when they are first sent to the client/server or when a savegame is created. This means that all changes to a dirtied variable up to the next synchronization point will be visible to peers despite no explicit write being performed to `Vars`. Example:
```lua
local v = _C().Vars.NRD_Whatever
v.SomeProperty = 123
-- variable is dirtied here
_C().Vars.NRD_Whatever = v
v.SomeProperty = 456
-- client will receive 456
Ext.Vars.SyncUserVariables()

-- client will NOT receive this change since the NRD_Whatever variable is no longer dirtied after sync;
-- another explicit write to Vars.NRD_Whatever must be performed
v.SomeProperty = 789
```

Variable caching can be disabled by passing the `DontCache` flag to `RegisterUserVariable`. Uncached variables are unserialized from JSON each time the property is accessed, so each access returns a different copy:

```lua
local t1 = _C().Vars.NRD_Whatever
local t2 = _C().Vars.NRD_Whatever
t1.Name = "test"
_D(t2.Name) -- prints nil
```

Variables are immediately serialized to JSON when a `Vars` write occurs; this means that changes to the original reference have no effect after assignment.

```lua
local t1 = { Name = "t1" }
_C().Vars.NRD_Whatever = t1
t1.Name = "t2"
_D(_C().Vars.NRD_Whatever.Name) -- prints "t1"
```

This also means that changing the value returned from a `Vars` fetch will not affect the stored value:

```lua
local t1 = _C().Vars.NRD_Whatever
t1.Name = "t1"
_D(_C().Vars.NRD_Whatever.Name) -- prints "t1"
```


<details>
<summary><b>Deprecated: PersistentVars</b></summary>

### Persistent Variables

PersistentVars store in the mod-local table `Mods[ModTable].PersistentVars`. By default the table is `nil`, i.e. a mod should create the table and populate it with data it wishes to store in the savegame. The contents of `PersistentVars` is saved when a savegame is created, and restored before the `SessionLoaded` event is triggered.

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
    _P(PersistentVars['Test'])
end

Ext.Events.SessionLoaded:Subscribe(OnSessionLoaded)
```
<a id="persistent-vars"></a>
</details>

---

<!-- https://github.com/orgs/community/discussions/16925 -->
> [!IMPORTANT]
> The following sections will go over the different modules provided by the Script Extender. Note that this documentation is not exhaustive and may not cover all features.
>
> Please refer to the [ExtIdeHelpers](https://github.com/Norbyte/bg3se/blob/main/BG3Extender/IdeHelpers/ExtIdeHelpers.lua) for a more comprehensive and systematic definition/reference of the API.

## ECS
### TODO - WIP

<a id="entity-class"></a>
## Entity class - `Ext.Entity`

Game objects in BG3 are called entities. Each entity consists of multiple components that describes certain properties or behaviors of the entity.
The Lua `Entity` class is the represntation of an ingame object (eg. character, item, trigger, etc.).

*Technical note:* For a somewhat more detailed description of the ECS system see:
 - [Entities, components and systems](https://medium.com/ingeniouslysimple/entities-components-and-systems-89c31464240d)
 - [The Entity-Component-System - An awesome game-design pattern in C++ ](https://www.gamasutra.com/blogs/TobiasStein/20171122/310172/The_EntityComponentSystem__An_awesome_gamedesign_pattern_in_C_Part_1.php)


### Entity:GetAllComponentNames() : string[]

Returns all engine component types (native C++ class names) that the entity has.

Example:
```lua
local char = Ext.Entity.Get(GetHostCharacter())
_D(char:GetAllComponentNames())
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
local entity = Ext.Entity.Get(GetHostCharacter())
_D(entity:GetAllComponents())
-- Prints:
-- {
--      "ActionResources" :
--      {
--              "Entity" : "Entity (02c0000100000180)",
--              "GetReplicationFlags" : "function: 00007FFDE482D5E0",
-- ...
```


### Entity:GetComponent(name) : Component?

Returns the specified component if it is attached to the entity. If the component is not present the method returns `nil`.

*Note:* This method only returns components whose structure is known to the Script Extender. Components with unknown structure are not returned.

*Note:* Although the type (character, item, etc.) of the entity cannot be determined directly, it can be inferred from the components that are attached to the entity.
Eg. to check if the entity is a character, an `entity:GetComponent("ServerCharacter") ~= nil` check can be used.

Example:
```lua
local entity = Ext.Entity.Get(GetHostCharacter())
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
local entity = Ext.Entity.Get(GetHostCharacter())
-- The two below are equivalent
local displayName = entity:GetComponent("DisplayName")
local displayName = entity.DisplayName
```


### Entity:CreateComponent(name) : Component

Attaches a new empty copy of the specified component type to the entity, if one does not exist. The function returns the newly created component.

*Note:* This method only works for components whose structure is known to the Script Extender. Components with unknown structure are not returned.


### Entity:Replicate(component)
### Entity:SetReplicationFlags(component, flags, word)
### Entity:GetReplicationFlags(component, word) : flags
The following methods allow subscribing to component lifecycle events (creation, destruction, or modification):
### Ext.Entity.Subscribe(componentName, callback, [entity], [order])
### Ext.Entity.Unsubscribe(subscriptionId)
### Ext.Entity.OnChange(...)
### Ext.Entity.OnCreate(...) / OnCreateDeferred(...) / OnCreateOnce(...)
### Ext.Entity.OnDestroy(...) / OnDestroyDeferred(...) / OnDestroyOnce(...)
### Ext.Entity.OnSystemUpdate(...) / OnSystemPostUpdate(...)

TODO - DOCUMENT

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

<a id="helper-functions"></a>
### Helper/aliased functions

Some helper functions were added to aid in development. (Please note that using them in mod code is not recommended, they are designed for developer use only.)

Prints the specified value(s) to the debug console. Works similarly to the built-in Lua `print()`, except that it also logs the printed messages to the editor messages pane.
 - `_D()`: Equivalent to `Ext.Dump()`, an utility function for dumping an expression to console; supports hierarchical dumping of tables and userdata (engine) objects
 - `_P()`: Equivalent to `Ext.Utils.Print()`
 - `_C()`: Equivalent to `Ext.Entity.Get(Osi.GetHostCharacter())`

## Networking

Mods can exchange data between the server and client(s) using the NetChannel API.

Note that there is no external networking capability in the Script Extender. SE mods cannot communicate with external servers or clients.

<a id="net-channel-api"></a>
### NetChannel API

> This section documents the new **NetChannel API**, which supersedes the legacy/deprecated NetMessage approach.

The NetChannel API provides a small, structured abstraction for request/response and message broadcasting and handling between server/client peers. It makes asynchronous requests easier to write and reason about, and can attach message handlers directly to the named channels.

#### Why NetChannel is better than the legacy approach

NetChannel improves ergonomics and safety compared to the deprecated NetMessage API:

- **Structured request/reply semantics** - instead of manually correlating messages and replies with NetMessage subscriptions, `RequestTo*` can accept a callback & receive the reply asynchronously right where you define the request.
- **Handler attachment per-channel** - handlers are registered per channel and are explicit; there are no loose handlers floating around subscriptions.
- **Faster local client requests** - old NetMessages were delayed by 1 frame even if the target was the local client (e.g. in single-player).


#### Quick concepts

* **Channel**: a named communication channel (string identifier).
* **Request / reply**: send a request and receive a response via a callback.
* **Message**: a one-way transmission of payload data to the other context (fire-and-forget).
* **Handlers** assign a *message handler* or *request handler* to a channel; these run when a message/request arrives.
* **Request callbacks** when you `RequestTo*`, you can pass a callback to receive the reply asynchronously.

---

#### API type annotation reference

```lua
--- Sets a handler for incoming messages (fire-and-forget)
---@param callback fun(data:table, user:any)
function NetChannel:SetHandler(callback) end

--- Sets a handler for incoming requests; return a table as the reply
---@param callback fun(data:table, user:any):table
function NetChannel:SetRequestHandler(callback) end

--- Send a request to the server; reply arrives via `replyCallback`
---@param data table
---@param replyCallback fun(data:table)
function NetChannel:RequestToServer(data, replyCallback) end

--- Send a request to a client; optionally specify a user and reply callback
---@param data table
---@param user integer|Guid
---@param replyCallback fun(data:table)
function NetChannel:RequestToClient(data, user, replyCallback) end
```

<a id="net-channel-examples"></a>
#### Usage patterns and examples

This section provides some pseudo-code examples of how to use the NetChannel API under different scenarios.

##### 1) Server-side handler that calls Osiris using data from the payload

```lua
-- Server side: handle requests
Channels = {}
Channels.TemplateAddTo = Net.CreateChannel(ModuleUUID, "TemplateAddTo")

-- Using SetHandler: note there's no reply callback
Channels.TemplateAddTo:SetHandler(function(data, user)
    for _, v in pairs(data.Items) do
        local template, amount = table.unpack(v)
        _P("Adding " .. template .. " to " .. data.Target .. " with amount " .. amount)
        Osi.TemplateAddTo(template, data.Target, amount or 1)
    end
end)

-- Client side: send message to server
Channels.TemplateAddTo:SendToServer({
    Items = { {"item-template-guid-1", 1}, {"item-template-guid-2", 2} },
    Target = someEntityId
})
```

##### 2) Request / reply (client requests some data from the server)

```lua
-- Server side: handle requests
Channels.CanItemBeMoved:SetRequestHandler(function(data, user)
    local item = Ext.Entity.Get(data.Target)
    return { Result = item.ServerItem.CanBeMoved }
end)

-- Client side: send to server and handle reply in the same spot (async)
local itemUuid = ...
Channels.CanItemBeMoved:RequestToServer({ Target = itemUuid }, function(response)
    Log("CanItemBeMoved reply", itemUuid, response.Result)
end)
```

This pattern allows the caller to perform work after the reply arrives without storing temporary state elsewhere, or defining multiple NetMessage channels (old API).

##### 3) Broadcast & sync (server pushes global state)

```lua
-- Client side: message handler for SyncSettings
Channels.SyncSettings = Net.CreateChannel(ModuleUUID, "SyncSettings")
Channels.SyncSettings:SetHandler(function(data, user)
    ModSettings = data.Settings
    _P("Received mod settings sync from server")
end)

-- Server side: push current data to all connected clients
Channels.SyncSettings:Broadcast({ Settings = MCM.GetCurrentSettings() })
```

##### 4) Targeted messages (server → specific client)

```lua
-- Server side: send data to a specific client (determined by clientId)
Channels.ChangeAppearance = Net.CreateChannel(ModuleUUID, "ChangeAppearance")
local clientId = ...
Channels.ChangeAppearance:SendToClient({ CCAData = {...} }, clientId)
```

Only the client of id `clientId` will receive the message.


<details>
<summary><b>Deprecated: NetMessages API</b></summary>

NOTE: The NetMessages API is deprecated; it is strongly recommended to use the NetChannel API instead.

To exchange data between the server and client(s), we use NetMessages. These can be sent and received from either context to facilitate communication. This allows us to share data between the server and the client(s) and vice versa.

<a id="net-send"></a>

### Sending NetMessages
NetMessages can be sent from either the server or client. They consist of a channel and a payload. The channel is a string used to distinguish your messages from others, and the payload is the data being sent. Currently, the payload must be a string. Here are some examples:

**Sending data from the server to the client(s) :**
```lua
--Server context
local channel = "MyModChannel_SomethingSpecific"
local payload = {["somedata"] = somevalue, ["supertable"]={1,2,3,4,5}}

--We need to stringify our payload in this case since it is a table and not a string
payload=Ext.Json.Stringify(payload)

--If we want to send the message to ALL the clients
Ext.ServerNet.BroadcastMessage(channel, payload)

--If we wanted to send the message to a specific userId
local somePeer = 9999
Ext.ServerNet.PostMessageToUser(somePeer, channel, payload)

--If we wanted to send the message to the client controlling a specific character
local someUUID = "c774d764-4a17-48dc-b470-32ace9ce447d" -- Wyll's uuid
Ext.ServerNet.PostMessageToClient(characterUUID, channel, payload)

```

**Sending data from the client to the server :**
```lua
local channel = "MyModChannel_SomethingSpecific"
local payload = "I'm a cute message"
--No need to stringify since we're sending a simple string
Ext.ClientNet.PostMessageToServer(channel, payload)
```



<a id="net-receive"></a>
### Listening for NetMessages

To handle incoming messages, we can listen to a channel on either side and use the received data. If the payload was stringified, use `Ext.Json.Parse` to convert it back into a table; otherwise, it remains a string.

**Listening for a Message from the Server in the Client Context :**

```lua
--Client context
local channel = "MyModChannel_SomethingSpecific"
Ext.Events.NetMessage:Subscribe(function(data)
 if data.Channel == channel then
  --Parse the string back into a table if it was stringified
  local data = Ext.Json.Parse(data.Payload)
  --Do whatever you want with the data in the client context
  someFunction(data)
 end
end)
```

**Alternatively :**
```lua
--wrapper for Ext.Events.NetMessage:Subscribe(function(data) ...end)
--which removes the need to check for the channel
Ext.RegisterNetListener(channel, function(channel, payload, userID)
  --Parse the string back into a table
  local data = Ext.Json.Parse(payload)
  --Do whatever you want with the data in the client context
  someFunction(data)
end)
```

*The code to listen for messages from the client on the server context would be similar.*

Note that the `userId` in these examples is actually a peerId. Osiris functions usually expect a different userId, which is typically `peerId + 1`. Use the following function to convert between the peerId used by network functions and the userId expected by Osiris functions:

```lua
function PeerToUserID(peerID)
 -- usually just userid+1
 return (u & 0xffff0000) | 0x0001
end
--Example usage, Server context, pretend the client just sent something on the whatever channel
--And that we need to get which character they're controlling
Ext.Events.NetMessage:Subscribe(function(data)
 if data.Channel == "whatever" then
  local character = Osi.GetCurrentCharacter(PeerToUserID(data.UserID)) -- returns the character the client was using when the client sent the message
  _P(character ) --Prints the character of the user the message originates from
 end
end)
```
</details>

### Utility functions
<a id="net-utils"></a>

#### Ext.Net.IsHost()

Returns true if the client it was called from is the host, always return true from the server context.

<a id="noesis-ui"></a>
## Noesis UI - `Ext.UI`

<a id="noesis-viewmodels"></a>
### Custom ViewModels

SE supports the creation and modification of Noesis viewmodels. These objects can be bound to UI elements as a DataContext and can participate in data binding.

For details on how Noesis (WPF) data bindings work see: [docs](https://learn.microsoft.com/en-us/dotnet/desktop/wpf/data/)

#### Registering a ViewModel type

To create a new viewmodel, the structure of the viewmodel must be first registered with Noesis. This only needs to be done once (typically during script bootstrapping) and the type can be instantiated any number of times afterwards. Once registered, a type cannot be changed.


To register a type, call the `Ext.UI.RegisterType(typeName, {property1 = {...}, property2 = {...}, ...}, [wrappedTypeName])` function.

Example:
```lua
local changeCallback = function (context, value)
    print("Value changed: ", value)
end

Ext.UI.RegisterType("PREFIX_YourTypeName", {
    MyStringProperty = {Type = "String", WriteCallback = changeCallback},
    CommandName1 = {Type = "Command"},
    SomeCollection = {Type = "Collection"}
})
```

The type name must be unique system-wide, so it's recommended to prefix the type name with the abbreviation of your mods name. When `RegisterType` is called with an already existing type name (eg. on a Lua reset), only changes to the `WriteCallback` property are applied; changes to fields, type names and the parent viewmodel type are not allowed.

*Note 1:* Registered types persist until the application is restarted; the registration does not go away on a Lua reset.
*Note 2:* To aid development, re-registering a type with different properties is allowed in Devel builds.

Property definitions support the following parameters:

- `Type`: Noesis type name; must always be specified
- `Notify`: Determines whether property change notifications are sent when the field is written; this allows Noesis to automatically detect changes to the property and update the UI accordingly
- `WriteCallback`: Function called when the value of the property is updated; this is useful for two-way data bindings when script needs to detect when the value of a UI field was updated.

*Note 1:* Properties can't distinguish between writes from script and writes from Noesis UI; the `WriteCallback` will be called for both cases.
*Note 2:* The `WriteCallback` is not called when the property is updated with the same value that it currently holds

The supported property types are the following:

- `Bool`: equivalent to the Lua `boolean` type
- `Int8`, `Int16`, `Int32`, `Int64`, `UInt8`, `UInt16`, `UInt32`, `UInt64`: integer types with varying sizes; maps to the Lua `integer` (number) type
- `Single`, `Double`: single-width and double-width floating point value; maps to the Lua `number` type
- `String`: equivalent to the Lua `string` type
- `Collection`: an array of Noesis objects; supports all classic Lua table operations (`__index`, `__newindex`, iteration, etc.)
- `Command`: a command that can be invoked by Noesis UI elements; use `command:SetHandler(func)` to set a Lua callback to handle the command
- `Object`: a reference to any Noesis object (either UI object or a viewmodel)
- `Color`, `Vector2`, `Vector3`, `Point`, `Rect`: vector types with varying sizes

The third (optional) parameter contains the type name of the viewmodel your type is wrapping; `nil` means that no viewmodel is being wrapped. When wrapping a viewmodel, all reads and writes to properties of the wrapped type are transparently forwarded to the underlying viewmodel object. This functionality exists to support extending the DataContext of existing UI widgets.

#### Instantiating a ViewModel

The `Ext.UI.Instantiate(type, [wrappedViewModel])` call constructs a new instance of the specified type.

For types without a wrapped viewmodel the second parameter should be omitted:
```lua
local vm = Ext.UI.Instantiate("PREFIX_YourTypeName")
vm.MyStringProperty = "whatever"
```

For types that wrap an existing DataContext, the wrapped object should be passed in the second parameter when constructing; the new object can be used to replace the existing DataContext on the widget:
```lua
local vm = Ext.UI.Instantiate("se::PREFIX_YourTypeName", mainMenu.DataContext)
mainMenu.DataContext = vm
```


#### Example

Example for replacing the main menu start game action:
```lua
-- Register a wrapper type for the main menu DataContext
Ext.UI.RegisterType("SAMPLE_MainMenuCtx", {
    StartGameCommand = {Type = "Command"} -- builtin command to start game
}, "gui::DCMainMenu") -- gui::DCMainMenu is the name of the ingame main menu DataContext

-- Jank sample code for getting a widget, DON'T DO IT LIKE THIS!
local mainMenu = Ext.UI.GetRoot():Find("ContentRoot"):VisualChild(1)

-- Create a wrapper around the original main menu DataContext
local ctx = Ext.UI.Instantiate("se::SAMPLE_MainMenuCtx", mainMenu.DataContext)
ctx.StartGameCommand:SetHandler(function ()
    print("do stuff")
end)

-- Overwrite datacontext with our wrapper
mainMenu.DataContext = ctx
```

### Input & world interaction
TODO:
```lua
    Ext.UI.GetPickingHelper(playerId): Returns data about what is under the cursor (world pos, entities).
    Ext.UI.GetCursorControl(): Access cursor state.
    Ext.UI.GetDragDrop(): Access drag-and-drop state.
```

<a id="stats"></a>
## Stats - `Ext.Stats`

<a id="stats-GetStatEntries"></a>
### Ext.Stats.GetStats(type: string): string[]

Returns a table with the names of all stat entries.
When the optional parameter `type` is specified, it'll only return stats with the specified type.
The following types are supported: `StatusData`, `SpellData`, `PassiveData`, `Armor`,  `Weapon`, `Character`, `Object`, `SpellSet`, `EquipmentSet`, `TreasureTable`, `TreasureCategory`,  `ItemGroup`, `NameGroup`


<a id="stats-objects"></a>
### Stats Objects

The following functions are only usable for Spell, Status, Passive, Interrupt, Armor, Weapon, Character and Object stats entries. Other stats types (eg. ItemGroups, TreasureTables) have their own separate sections in the docs and cannot be manipulated using these functions.


#### Ext.Stats.GetStatsLoadedBefore(modGuid: string, type: string): string[]

Returns a table with the names of all stat entries that were loaded before the specified mod.
This function is useful for retrieving stats that can be overridden by a mod according to the module load order.
When the optional parameter `type` is specified, it'll only return stats with the specified type. (The type of a stat entry is specified in the stat .txt file itself (eg. `type "StatusData"`).

#### Ext.Stats.Create(name: string, type: string, template: string?): StatEntry

Creates a new stats entry.
If a stat object with the same name already exists, the specified modifier type is invalid or the specified template doesn't exist, the function returns `nil`.
After all stat properties were initialized, the stats entry must be synchronized by calling `stat:Sync()`.

 - `name` is the name of stats entry to create; it should be globally unique
 - `type` is the stats entry type (eg. `SkillData`, `StatusData`, `Weapon`, etc.)
 - If the `template` parameter is not null, stats properties are copied from the template entry to the newly created entry
 - If the entry was created on the server, `stat:Sync()` will replicate the stats entry to all clients. If the entry was created on the client, `stat:Sync()` will only update it locally.

Example:
```lua
local stat = Ext.Stats.Create("NRD_Dynamic_Skill", "SkillData", "Rain_Water")
stat.RainEffect = "RS3_FX_Environment_Rain_Fire_01"
stat.SurfaceType = "Fire"
stat:Sync()
```

#### Ext.Stats.Get(stat, [level], [warnOnError]): StatEntry

Returns the specified stats entry as an object for easier manipulation.
If the `level` argument is not specified or is `nil`, the table will contain stat values as specified in the stat entry.
If the `level` argument is not `nil`, the table will contain level-scaled values for the specified level. A `level` value of `-1` will use the level specified in the stat entry.

The behavior of getting a table entry is identical to that of `StatGetAttribute` and setting a table entry is identical to `StatSetAttribute`.

The `StatSetAttribute` example rewritten using `Stats.Get`:
```lua
-- Swap DamageType from Poison to Air on all skills
for i,name in pairs(Ext.Stats.GetStats("SkillData")) do
    local stat = Ext.Stats.Get(name)
    if stat.DamageType == "Poison" then
        stat.DamageType = "Air"
    end
end
```

<a id="reading-writing-stats"></a>
#### Reading stat attributes

Stat attributes can be retrieved by reading the appropriate property of the StatEntry object:

```lua
local spell = Ext.Stats.Get("Shout_FlameBlade")
local useCosts = spell.UseCosts
```

If the stat entry doesn't have the specified attribute or the attribute is not supported, `nil` is returned.
The list of attributes each stat type supports can be found in `Public\Shared\Stats\Generated\Structure\Modifiers.txt`.

*Technical note:* The StatEntry object has an `__index` metamethod that retrieves the stats property; the StatEntry is not a Lua table and shouldn't be treated as such!

#### Writing stat attributes

Stat attributes can be updated using simple table assignment:

```lua
local spell = Ext.Stats.Get("Shout_FlameBlade")
spell.UseCosts = "BonusActionPoint:1;SpellSlot:1:1:2"
```

This essentially allows on-the-fly changing of data loaded from stat .txt files without having to override the whole stat entry.
If the function is called while the module is loading (i.e. from a `ModuleLoading`/`StatsLoaded` listener) no additional synchronization is needed. If the function is called after module load, the stats entry must be synchronized with the client via the `StatEntry:Sync()` call.

*Technical note:* The StatEntry object has a `__newindex` metamethod that performs validation and updates the real stats entry in the background.


Example usage of stats read/write (Disable autocast on all spells):
```lua
for i,name in pairs(Ext.Stats.GetStats("SpellData")) do
    local spell = Ext.Stats.Get(name)
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

#### Stat property type notes

For a list of enumeration types and their possible values see `Public\Shared\Stats\Generated\Structure\Base\ValueLists.txt` or `Enumerations.xml`.

#### Flags

The `AttributeFlags`, `SpellFlagList`, `WeaponFlags`, `ResistanceFlags`, `PassiveFlags`, `ProficiencyGroupFlags`, `StatsFunctorContext`, `StatusEvent`, `StatusPropertyFlags`, `StatusGroupFlags` and `LineOfSightFlags` enumerations are flags; this means that multiple enumeration values may be assigned to a stats property.

Reading flags:
```lua
local spell = Ext.Stats.Get("Shout_ArmorOfAgathys")
_D(spell.SpellFlags)
-- Prints:
-- ["HasSomaticComponent", "HasVerbalComponent", "IsSpell"]
```

Writing flags:
```lua
local spell = Ext.Stats.Get("Shout_ArmorOfAgathys")
spell.SpellFlags = {"HasVerbalComponent", "IsSpell"}
```

##### Requirements

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

##### StatsFunctors

**StatsFunctors are not supported as of v29.**

#### Ext.Stats.ExtraData

`Ext.ExtraData` is an object containing all entries from `Data.txt`.

*Note*: It is possible to add custom `ExtraData` keys by adding a new `Data.txt` to the mod and then retrieve them using Lua.

Example:
```lua
Ext.Utils.Print(Ext.Stats.ExtraData.WisdomTierHigh)
```

<a id="io"></a>
## I/O - `Ext.IO`

TODO.

`Ext.IO.LoadFile(path, [context])`: Read file contents.

`Ext.IO.SaveFile(path, content)`: Write data to a file.

`Ext.IO.AddPathOverride(originalPath, newPath)`

Redirects file access from `originalPath` to `newPath`. This is useful for overriding built-in files or resources that are otherwise not moddable.
Make sure that the override is added as early as possible (preferably in `StatsLoaded`), as adding path overrides after the game has already loaded the resource has no effect.

Example:
```lua
Ext.IO.AddPathOverride("Public/Game/GUI/enemyHealthBar.swf", "Public/YourMod/GUI/enemyHealthBar.swf")
```

<a id="timers"></a>
## Timers - `Ext.Timer`

TODO:
Delayed Execution

`Ext.Timer.WaitFor(ms, callback)`: Uses game clock (pauses when game pauses).

`Ext.Timer.WaitForRealtime(ms, callback)`: Uses OS clock.

Most of the time they are the same, but there are cases when the game timer is paused and time doesn't "progress".
Game timer can also be affected by the tick throttling logic if the framerate drops too low.

`Ext.Timer.WaitForPersistent(ms, name, callback)`: Creates a persistent handle that is written to the savegame so your timer survives a save/reload.

`Ext.Timer.MonotonicTime()`:
Returns a monotonic value representing the current system time in milliseconds. Useful for performance measurements / measuring real world time.
(Note: This value is not synchronized between peers and different clients may report different time values!)

Example:
```lua
local startTime = Ext.Timer.MonotonicTime()
DoLongTask()
local endTime = Ext.Timer.MonotonicTime()
_P("Took: " .. tostring(endTime - startTime) .. " ms")
```

<a id="json-support"></a>
## JSON support - `Ext.Json`

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
_P(json)

local decoded = Ext.Json.Parse(json)
_P(decoded.arr[1])
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

 - The `Stringify` function accepts an optional settings table `Stringify(value, [options])`. `options` is a table that supports the following keys:
   - `Beautify` (bool) - Generate human-readable JSON (i.e. add indents and linebreaks to the output)
   - `StringifyInternalTypes` (bool) - Save engine types (handles, coroutines, etc.) as strings instead of throwing an error
   - `IterateUserdata` (bool) - Dump engine objects similarly to tables instead of throwing an error
      - NOTE: Due to the nature of these objects, neither internal types nor userdata types can be unserialized from a JSON; parsing a JSON with userdata objects will return them as normal tables
    - `AvoidRecursion` (bool) - If an userdata or table is seen multiple times, further instances will be serialized as `"*RECURSION*"`; this is helpful when dumping objects
    - `MaxDepth` (int) - Maximum iteration depth

Example:
```lua
Ext.Json.Stringify(val, {
    Beautify = true,
    MaxDepth = 4
})
```

<a id="mod-info"></a>
## Mod info - `Ext.Mod`

### IsModLoaded(modGuid)

Returns whether the module with the specified GUID is loaded.

Example:
```lua
if (Ext.Mod.IsModLoaded("5cc23efe-f451-c414-117d-b68fbc53d32d"))
    _P("Mod loaded")
end
```

### GetLoadOrder()

Returns the list of loaded module UUIDs in the order they're loaded in.

### GetMod(modGuid)

Returns detailed information about the specified (loaded) module.
Example:
```lua
local loadOrder = Ext.Mod.GetLoadOrder()
for k,uuid in pairs(loadOrder) do
    local mod = Ext.Mod.GetMod(uuid)
    _D(mod)
end
```

### GetModManager(): ModManager
TODO:
Provides access to the engine's internal ModManager.

<a id="utils"></a>
## Utils - `Ext.Utils`
TODO:

`Ext.Utils.GetGlobalSwitches() : GlobalSwitches`
Allows access to engine toggles (e.g., AiEnableSwarm, NrOfAutoSaves).

<a id="audio"></a>
## Audio - `Ext.Audio`
TODO: Document `Ext.Audio` module

Functionality for audio manipulation including Sound Banks, Events, and RTPC (Real-Time Parameter Controls).

<a id="loca"></a>
## Localization - `Ext.Loca`
TODO: Document `Ext.Loca` module

Methods for reading and writing localization entries (loca) at runtime.

<a id="templates"></a>
## Templates - `Ext.Template`
TODO: Document `Ext.Template` module

API for accessing and modifying Character and Item templates.

<a id="static-data"></a>
## Static Data - `Ext.StaticData`
TODO: Document `Ext.StaticData` module

Access to static game resources such as Races, Classes, and other UUID-based engine definitions.

<a id="resources"></a>
## Resources - `Ext.Resource`
TODO: Document `Ext.Resource` module

Access to visual resources including Meshes, Materials, and Textures.

<a id="level"></a>
## Levels, Pathfinding & Physics - `Ext.Level`
TODO: Document `Ext.Level` module

Contains logic for Raycasting, Pathfinding, and checking entity/tile physics data.

<a id="math"></a>
## Math library - `Ext.Math`

The extender math library `Ext.Math` contains following functions:

| Function | Parameters | Return Type | Description |
|----------|-----------|-------------|-------------|
| `Add` | `a: any, b: any` | `any` | Adds two operands. Supports `number`/`vec3`/`vec4`/`mat3x3`/`mat4x4` with mixed types. |
| `Sub` | `a: any, b: any` | `any` | Subtracts two operands. Supports `number`/`vec3`/`vec4`/`mat3x3`/`mat4x4` with mixed types. |
| `Mul` | `a: any, b: any` | `any` | Multiplies two operands. Supports `number`/`vec3`/`vec4`/`mat3x3`/`mat4x4` with mixed types. |
| `Div` | `a: any, b: any` | `any` | Divides two operands. Supports `number`/`vec3`/`vec4`/`mat3x3`/`mat4x4`. |
| `Reflect` | `I: vec3\|vec4, N: vec3\|vec4` | `vec3\|vec4` | Returns reflection direction: `result = I - 2.0 * dot(N, I) * N`. |
| `Angle` | `a: vec3\|vec4, b: vec3\|vec4` | `float` | Returns absolute angle between two normalized vectors. |
| `Cross` | `x: vec3, y: vec3` | `vec3` | Returns the cross product of x and y. |
| `Distance` | `p0: vec3, p1: vec3` | `float` | Returns distance between p0 and p1: `length(p0 - p1)`. |
| `Dot` | `x: vec3, y: vec3` | `float` | Returns the dot product of x and y. |
| `Length` | `x: vec3\|vec4` | `float` | Returns the length of x: `sqrt(x * x)`. |
| `Normalize` | `x: vec3\|vec4` | `vec3\|vec4` | Returns a vector in the same direction as x with length 1. |
| `Determinant` | `x: mat3\|mat4` | `float` | Returns the determinant of a matrix. |
| `Inverse` | `x: mat3\|mat4` | `mat3\|mat4` | Returns the inverse of a matrix. |
| `Transpose` | `x: mat3\|mat4` | `mat3\|mat4` | Returns the transposed matrix of x. |
| `OuterProduct` | `c: vec3\|vec4, r: vec3\|vec4` | `mat` | Treats c as column vector and r as row vector, returns `c * r`. |
| `Rotate` | `m: mat3\|mat4, angle: float, axis: vec4` | `void` | Builds a rotation matrix from axis and angle in radians. |
| `Translate` | `m: mat4, translation: vec3` | `void` | Transforms a matrix with translation. |
| `Scale` | `m: mat4, translation: vec3` | `void` | Transforms a matrix with scale. |
| `BuildRotation4` | `v: vec3, angle: float` | `mat4` | Builds a 4×4 rotation matrix from axis and angle in radians. |
| `BuildRotation3` | `v: vec3, angle: float` | `mat3` | Builds a 3×3 rotation matrix from axis and angle in radians. |
| `BuildTranslation` | `v: vec3` | `mat4` | Builds a 4×4 translation matrix from a vector. |
| `BuildScale` | `v: vec3` | `mat4` | Builds a 4×4 scale matrix from 3 scalars. |
| `ExtractEulerAngles` | `m: mat3\|mat4` | `vec3` | Extracts `(X * Y * Z)` Euler angles from rotation matrix. |
| `BuildFromEulerAngles4` | `angles: vec3` | `mat4` | Creates a 4×4 rotation matrix from `(X * Y * Z)` Euler angles. |
| `BuildFromEulerAngles3` | `angles: vec3` | `mat3` | Creates a 3×3 rotation matrix from `(X * Y * Z)` Euler angles. |
| `Decompose` | `m: mat4, scale: vec3, yawPitchRoll: vec3, translation: vec3` | `void` | Decomposes a model matrix into translation, rotation, and scale. |
| `ExtractAxisAngle` | `m: mat3\|mat4, axis: vec3` | `float` | Extracts axis-angle representation from a matrix. |
| `BuildFromAxisAngle3` | `axis: vec3, angle: float` | `mat3` | Builds a 3×3 rotation matrix from axis and angle. |
| `BuildFromAxisAngle4` | `axis: vec3, angle: float` | `mat4` | Builds a 4×4 rotation matrix from axis and angle. |
| `Perpendicular` | `x: vec3\|vec4, normal: vec3\|vec4` | `vec3\|vec4` | Projects x on a perpendicular axis of normal. |
| `Project` | `x: vec3\|vec4, normal: vec3\|vec4` | `vec3\|vec4` | Projects x on normal. |
| `Fract` | `x: float` | `float` | Returns `x - floor(x)`. |
| `Trunc` | `x: float` | `float` | Returns nearest integer to x with absolute value not larger than x. |
| `Sign` | `x: float` | `float` | Returns `1.0` if `x > 0`, `0.0` if `x == 0`, `-1.0` if `x < 0`. |
| `Clamp` | `val: float, minVal: float, maxVal: float` | `float` | Returns `min(max(val, minVal), maxVal)`. |
| `Lerp` | `x: float, y: float, a: float` | `float` | Returns `x * (1.0 - a) + y * a` (linear blend). |
| `Acos` | `x: float` | `float` | Arc cosine. Returns angle whose cosine is x. |
| `Asin` | `x: float` | `float` | Arc sine. Returns angle whose sine is x. |
| `Atan` | `y_over_x: float` | `float` | Arc tangent. Returns angle whose tangent is `y_over_x`. |
| `Atan2` | `x: float, y: float` | `float` | Arc tangent. Returns angle whose tangent is `y / x`. |

<a id="engine-events"></a>

## Engine and SE Events - `Ext.Events`

You can listen to SE and engine events with `Ext.Events.<EVENT_NAME>:Subscribe(fun)`:

| Event | Description |
|-------|-------------|
| `ModuleLoadStarted` | Thrown when the engine has started loading mods.<br/>Mod data (stats, localization, root templates, etc.) is not yet loaded, so most mod editing functionality (e.g., `Ext.StatSetAttribute`) is inaccessible.<br/>Purpose: Allow adding filesystem-level hooks using `Ext.IO.AddPathOverride` before mod data is loaded. |
| `StatsLoaded` | Thrown after stats entries (weapons, skills, etc.) were cleared and subsequently reloaded.<br/>Stat modifications that are valid for every game session should be applied here. |
| `SessionLoading` | Thrown when the engine has started setting up a game session (i.e., new game, loading a savegame, or joining a multiplayer game). |
| `SessionLoaded` | Thrown when the game session was set up. |
| `ResetCompleted` | Thrown when `Ext.Debug.Reset()` or `reset` console command completes on the client or server.<br/>Indicates that the Lua state was reloaded. |
| `GameStateChanged` | Indicates that the server/client game state changed (e.g., loading save, paused, main menu, etc.). |
| `Tick` | Thrown after each game engine tick on both the client and the server.<br/>Server logic runs at ~30hz, so this event is thrown roughly every 33ms.<br/>Helper: `Ext.OnNextTick(fun)` registers a handler that is only called on the next tick and is unregistered afterwards. |
