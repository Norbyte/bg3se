
### BG3SE Lua API v11 Documentation

### Table of Contents  
 - [Getting Started](#getting-started)
    - [Bootstrap Scripts](#bootstrap-scripts)
 - [Client / Server States](#client-server)
    - [Persistent Variables](#persistent-vars)
 - [Console](#console)
    - [Multiline Mode](#multiline-mode)
    - [Saving output to a file](#save-console-output)
 - [Calling Lua from Osiris](#calling-lua-from-osiris)
    * [Capturing Events/Calls](#l2o_captures)
 - [Calling Osiris from Lua](#calling-osiris-from-lua)
    * [Calls](#o2l_calls)
    * [Queries](#o2l_queries)
    * [Events](#o2l_events)
    * [PROCs](#o2l_procs)
    * [User Queries](#o2l_qrys)
    * [Databases](#o2l_dbs)
 - [General Lua Rules](#lua-general)
    * [Object Scopes](#lua-scopes)
    * [Object Behavior](#lua-objects)
    * [Parameter Passing](#lua-parameters)
    * [Enumerations](#lua-enumerations)
    * [Bitfields](#lua-bitfields)
    * [Events](#lua-events)
 - [Stats](#stats)
 - [ECS](#ecs)
 - [Custom Variables](#custom-variables)
 - [Utility functions](#ext-utility)
 - [JSON Support](#json-support)
 - [Mod Info](#mod-info)
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

<a id="bootstrap-scripts"></a>
### Bootstrap Scripts  

If Lua is enabled for the mod, the extender will attempt to load `BootstrapServer.lua` on the server side, and `BootstrapClient.lua` on the client side. These scripts should be created in the `Mods\<YOURMOD>\ScriptExtender\Lua\` folder.
```

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
    _P(PersistentVars['Test'])
end

Ext.Events.SessionLoaded:Subscribe(OnSessionLoaded)
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

Sometimes, the output of a command in the console might be too lengthy or complex to analyze effectively within the console interface. In such cases, you might prefer to save the output to a file for easier review. This can be accomplished using a combination of two functions: Ext.IO.SaveFile(filename, content) and Ext.DumpExport(object).

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
## General Lua Rules

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
## Events

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

<a id="stats"></a>
## Stats (Ext.Stats module)

<a id="stats-GetStatEntries"></a>
### Ext.Stats.GetStats(type: string): string[]

Returns a table with the names of all stat entries.
When the optional parameter `type` is specified, it'll only return stats with the specified type.
The following types are supported: `StatusData`, `SpellData`, `PassiveData`, `Armor`,  `Weapon`, `Character`, `Object`, `SpellSet`, `EquipmentSet`, `TreasureTable`, `TreasureCategory`,  `ItemGroup`, `NameGroup`


<a id="stats-objects"></a>
## Stats Objects

The following functions are only usable for Spell, Status, Passive, Interrupt, Armor, Weapon, Character and Object stats entries. Other stats types (eg. ItemGroups, TreasureTables) have their own separate sections in the docs and cannot be manipulated using these functions.


### Ext.Stats.GetStatsLoadedBefore(modGuid: string, type: string): string[]

Returns a table with the names of all stat entries that were loaded before the specified mod.
This function is useful for retrieving stats that can be overridden by a mod according to the module load order.
When the optional parameter `type` is specified, it'll only return stats with the specified type. (The type of a stat entry is specified in the stat .txt file itself (eg. `type "StatusData"`).

### Ext.Stats.Create(name: string, type: string, template: string?): StatEntry

Creates a new stats entry. 
If a stat object with the same name already exists, the specified modifier type is invalid or the specified template doesn't exist, the function returns `nil`.
After all stat properties were initialized, the stats entry must be synchronized by calling `stat:Sync()`. 

 - `name` is the name of stats entry to create; it should be globally unique
 - `type` is the stats entry type (eg. `SkillData`, `StatusData`, `Weapon`, etc.)
 - If the `template` parameter is not null, stats properties are copied from the template entry to the newly created entry
 - If the entry was created on the server, `stat:Sync()` will replicate the stats entry to all clients. If the entry was created on the client, `stat:Sync()` will only update it locally.

Example:
```lua
local stat = Ext.Stats.CreateStat("NRD_Dynamic_Skill", "SkillData", "Rain_Water")
stat.RainEffect = "RS3_FX_Environment_Rain_Fire_01"
stat.SurfaceType = "Fire"
stat:Sync()
```

### Ext.Stats.Get(stat, [level], [warnOnError]): StatEntry

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

### Reading stat attributes

Stat attributes can be retrieved by reading the appropriate property of the StatEntry object:

```lua
local spell = Ext.Stats.Get("Shout_FlameBlade")
local useCosts = spell.UseCosts
```

If the stat entry doesn't have the specified attribute or the attribute is not supported, `nil` is returned. 
The list of attributes each stat type supports can be found in `Public\Shared\Stats\Generated\Structure\Modifiers.txt`.

*Technical note:* The StatEntry object has an `__index` metamethod that retrieves the stats property; the StatEntry is not a Lua table and shouldn't be treated as such!

### Writing stat attributes

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

### Stat property type notes

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

**StatsFunctors are not supported as of v11.**

### Ext.Stats.ExtraData

`Ext.ExtraData` is an object containing all entries from `Data.txt`.

*Note*: It is possible to add custom `ExtraData` keys by adding a new `Data.txt` to the mod and then retrieve them using Lua.

Example:
```lua
Ext.Utils.Print(Ext.Stats.ExtraData.WisdomTierHigh)
```


## ECS

### TODO - WIP

## Entity class

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

FIXME - DOCUMENT

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


<a id="custom-variables"></a>
## Custom variables

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


<a id="ext-utility"></a>
## Utility functions

#### Ext.Require(path) <sup>R</sup>

The `Ext.Require` function is the extender's version of the Lua built-in `require` function. 
The function checks if the file at `Mods/<ModuleUUID>/ScriptExtender/Lua/<path>` was already loaded; if not, it'll load the file, store the return value of the main chunk and return it to the caller. If the file was already loaded, it'll return the stored return value.
**Note:** `Ext.Require` should only be called during module startup (i.e. when loading `BootstrapClient.lua` or `BoostrapServer.lua`). Loading Lua files after module startup is deprecated.

#### Ext.Utils.Print(...)

Prints the specified value(s) to the debug console. Works similarly to the built-in Lua `print()`, except that it also logs the printed messages to the editor messages pane.

#### Ext.IO.AddPathOverride(originalPath, newPath)

Redirects file access from `originalPath` to `newPath`. This is useful for overriding built-in files or resources that are otherwise not moddable.
Make sure that the override is added as early as possible (preferably in `StatsLoaded`), as adding path overrides after the game has already loaded the resource has no effect.

Example:
```lua
Ext.IO.AddPathOverride("Public/Game/GUI/enemyHealthBar.swf", "Public/YourMod/GUI/enemyHealthBar.swf")
```

#### Ext.Utils.MonotonicTime()

Returns a monotonic value representing the current system time in milliseconds. Useful for performance measurements / measuring real world time.
(Note: This value is not synchronized between peers and different clients may report different time values!)

Example:
```lua
local startTime = Ext.Utils.MonotonicTime()
DoLongTask()
local endTime = Ext.Utils.MonotonicTime()
_P("Took: " .. tostring(endTime - startTime) .. " ms")
```

### Helper functions

Some helper functions were added to aid in development. (Please note that using them in mod code is not recommended, they are designed for developer use only.)

 - `_D()`: Equivalent to `Ext.Dump()`, an utility function for dumping an expression to console; supports hierarchical dumping of tables and userdata (engine) objects
 - `_P()`: Equivalent to `Ext.Utils.Print()`
 - `_C()`: Equivalent to `Ext.Entity.Get(Osi.GetHostCharacter())`


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
## Mod Info

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

### GetModInfo(modGuid)

Returns detailed information about the specified (loaded) module.
Example:
```lua
local loadOrder = Ext.Mod.GetLoadOrder()
for k,uuid in pairs(loadOrder) do
    local mod = Ext.Mod.GetModInfo(uuid)
    _D(mod)
end
```

<a id="math"></a>
## Math library

The extender math library `Ext.Math` contains following functions:

##### Add(a: any, b: any)

Adds the two operands. All math types (number/vec3/vec4/mat3x3/mat4x4) are supported. Mixing different operand types works in if a reasonable implementation is available (eg. `number + vec3`).

##### Sub(a: any, b: any)

Subtracts the two operands. All math types (number/vec3/vec4/mat3x3/mat4x4) are supported. Mixing different operand types works in if a reasonable implementation is available (eg. `vec3 - number`).

##### Mul(a: any, b: any)

Multiplies the two operands. All math types (number/vec3/vec4/mat3x3/mat4x4) are supported. Mixing different operand types works in if a reasonable implementation is available (eg. `mat3x3 * vec3`).

##### Div(a: any, b: any)

Divides the two operands. All math types (number/vec3/vec4/mat3x3/mat4x4) are supported.

##### vec3|vec4 Reflect(I: vec3|vec4, N: vec3|vec4)

For the incident vector `I` and surface orientation `N`, returns the reflection direction: `result = I - 2.0 * dot(N, I) * N`.

##### float Angle(a: vec3|vec4, b: vec3|vec4)

Returns the absolute angle between two vectors. Parameters need to be normalized.

##### vec3 Cross(x: vec3, y: vec3)

Returns the cross product of x and y.

##### float Distance(p0: vec3, p1: vec3)

Returns the distance between p0 and p1, i.e., `length(p0 - p1)`.

##### float Dot(x: vec3, y: vec3)

Returns the dot product of x and y.

##### float Length(x: vec3|vec4)

Returns the length of x, i.e., `sqrt(x * x)`.

##### vec3|vec4 Normalize(x: vec3|vec4)

Returns a vector in the same direction as x but with length of 1.

##### float Determinant(x: mat3|mat4)

Return the determinant of a matrix.

##### mat3|mat4 Inverse(x: mat3|mat4)

Return the inverse of a matrix.

##### mat3|mat4 Transpose(x: mat3|mat4)

Returns the transposed matrix of `x`.

##### mat3|mat4 OuterProduct(c: vec3|vec4, r: vec3|vec4)

Treats the first parameter `c` as a column vector and the second parameter `r` as a row vector and does a linear algebraic matrix multiply `c * r`.

##### void Rotate(m: mat3|mat4, angle: float, axis: vec4)

Builds a rotation matrix created from an axis of 3 scalars and an angle expressed in radians.

##### void Translate(m: mat4, translation: vec3)

Transforms a matrix with a translation 4 * 4 matrix created from a vector of 3 components.

##### void Scale(m: mat4, translation: vec3)

Transforms a matrix with a scale 4 * 4 matrix created from a vector of 3 components.

##### mat4 BuildRotation4(v: vec3, angle: float)

Builds a rotation 4 * 4 matrix created from an axis of 3 scalars and an angle expressed in radians.

##### mat3 BuildRotation3(v: vec3, angle: float)

Builds a rotation 3 * 3 matrix created from an axis of 3 scalars and an angle expressed in radians.

##### mat4 BuildTranslation(v: vec3)

Builds a translation 4 * 4 matrix created from a vector of 3 components.

##### mat4 BuildScale(v: vec3)

Builds a scale 4 * 4 matrix created from 3 scalars.

##### vec3 ExtractEulerAngles(m: mat3|mat4)

Extracts the `(X * Y * Z)` Euler angles from the rotation matrix M.

##### mat4 BuildFromEulerAngles4(angles: vec3)

Creates a 3D 4 * 4 homogeneous rotation matrix from euler angles `(X * Y * Z)`.

##### mat3 BuildFromEulerAngles3(angles: vec3)

Creates a 3D 3 * 3 homogeneous rotation matrix from euler angles `(X * Y * Z)`.

##### void Decompose(m: mat4, scale: vec3, yawPitchRoll: vec3, translation: vec3)

Decomposes a model matrix to translations, rotation and scale components.

##### float ExtractAxisAngle(m: mat3|mat4, axis: vec3)

Decomposes a model matrix to translations, rotation and scale components.

##### mat3 BuildFromAxisAngle3(axis: vec3, angle: float)
##### mat4 BuildFromAxisAngle4(axis: vec3, angle: float)

Build a matrix from axis and angle.

##### vec3|vec4 Perpendicular(x: vec3|vec4, normal: vec3|vec4)

Projects `x` on a perpendicular axis of `normal`.

##### vec3|vec4 Project(x: vec3|vec4, normal: vec3|vec4)

Projects `x` on `normal`.

##### float Fract(x: float)

Return `x - floor(x).`

##### float Trunc(x: float)

Returns a value equal to the nearest integer to x whose absolute value is not larger than the absolute value of x.

##### float Sign(x: float)

Returns 1.0 if `x > 0`, 0.0 if `x == 0`, or -1.0 if `x < 0`.

##### float Clamp(val: float, minVal: float, maxVal: float)

Returns `min(max(x, minVal), maxVal)` for each component in x using the floating-point values minVal and maxVal.

##### float Lerp(x: float, y: float, a: float)

Returns `x * (1.0 - a) + y * a`, i.e., the linear blend of x and y using the floating-point value a.

##### float Acos(x: float)

Arc cosine. Returns an angle whose sine is x.

##### float Asin(x: float)

Arc sine. Returns an angle whose sine is x.

##### float Atan(y_over_x: float)

 Arc tangent. Returns an angle whose tangent is `y_over_x`.

##### float Atan2(x: float, y: float)

Arc tangent. Returns an angle whose tangent is `y / x`. The signs of x and y are used to determine what quadrant the angle is in.

<a id="engine-events"></a>
# Engine Events

<a id="event-load-events"></a>
## Load Events

### ModuleLoadStarted

The `ModuleLoadStarted` event is thrown when the engine has started loading mods. Mod data (stats, localization, root templates, etc.) is not yet loaded when this listener is called, so most mod editing functionality (eg. `Ext.StatSetAttribute`) is inaccessible.
The purpose of this event is to allow adding filesystem-level hooks using `Ext.IO.AddPathOverride` before mod data is loaded.

### StatsLoaded

`StatsLoaded` is thrown after stats entries (weapons, skills, etc.) were cleared and subsequently reloaded. Stat modifications that are valid for every game session should be applied here.

### SessionLoading

`SessionLoading` is thrown when the the engine has started setting up a game session (i.e. new game, loading a savegame or joining a multiplayer game). 

### SessionLoaded

`SessionLoaded` is thrown when the game session was set up.

### ResetCompleted

The `ResetCompleted` event is thrown when a `reset` console command completes on the client or server, indicating that the Lua state was reloaded.

### GameStateChanged

The `GameStateChanged` event indicates that the server/client game state changed (eg. paused, etc).

### Tick

The `Tick` event is thrown after each game engine tick on both the client and the server. Server logic runs at ~30hz, so this event is thrown roughly every 33ms.
    -  The `Ext.OnNextTick(fun)` helper registers a handler that is only called on the next tick and is unregistered afterwards

### OnResetCompleted

Thrown when a console `reset` command or an `NRD_LuaReset` Osiris call completes.
