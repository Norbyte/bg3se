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
 - [General Lua Rules](#lua-general)
    * [Object Scopes](#lua-scopes)
    * [Object Behavior](#lua-objects)
    * [Parameter Passing](#lua-parameters)
    * [Events](#lua-events)
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

<a id="ext-utility"></a>
## Utility functions

#### Ext.Require(path) <sup>R</sup>

The `Ext.Require` function is the extender's version of the Lua built-in `require` function. 
The function checks if the file at `Mods/<ModuleUUID>/ScriptExtender/Lua/<path>` was already loaded; if not, it'll load the file, store the return value of the main chunk and return it to the caller. If the file was already loaded, it'll return the stored return value.
**Note:** `Ext.Require` should only be called during module startup (i.e. when loading `BootstrapClient.lua` or `BoostrapServer.lua`). Loading Lua files after module startup is deprecated.

#### Ext.Utils.Print(...)

Prints the specified value(s) to the debug console. Works similarly to the built-in Lua `print()`, except that it also logs the printed messages to the editor messages pane.

#### Ext.IO.AddPathOverride(originalPath, newPath)

Redirects file access from `originalPath` to `newPath`. This is useful for overriding built-in files or resources that are otherwise not moddable, eg. UI Flash files.
Make sure that the override is added as early as possible (preferably in `ModuleLoading`), as adding path overrides after the game has already loaded the resource has no effect.

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

### ModuleLoading

`ModuleLoading` is thrown after the stats manager has finished loading; this callback is deprecated and `StatsLoaded` should be used instead.

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
