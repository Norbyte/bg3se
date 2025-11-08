# Release Notes

## v28

- Added support for separate client/server reset to `Ext.Utils.Reset()`
- Added support for custom Noesis viewmodels via `Ext.UI.RegisterType()` and `Ext.UI.Instantiate()`
- Added support for Noesis Command objects
- Fixed `ResetCompleted` event not firing in certain game states
- Fixed Noesis SymbolManager race condition
- Fixed crash when setting a Noesis object property
- Fixed bug where certain types were not serializable
- Fixed bug where savegame load warnings could prevent the game from loading
- Fixed bug where unserializing a hashmap would not preserve values


## v27.1

- Hotfix 34 InputManager compatibility 


## v27

- Added support for photo mode resources
- Added limited support for runtime creation of new guid resources
- Added support for custom properties and functions on C++ objects
- Added reporting for longLua dispatches
- Added Optick profiler
- Added mapping for `Effect`, `Decal`, `AnimatableObject`, `Shape` visual types
- Added IMGUI `SamePosition` flag
- Added `Ext.Types.GetFunctionLocation`
- Added `Ext.Utils.GetMemoryUsage`
- Added blend shape helpers
- IMGUI fonts can now be replaced by a subsequent call to `LoadFont`
- `table.find` now supports userdata
- Changed network and savegame serialization from JSON to binary
- Iteration order for order for property maps is now consistent (lexicographic)
- Improved JSON serializer performance
- Fixed logic for reading AIFlags
- Fixed inline status flags disappearing
- Fixed crash when subscribers are modified during a replication event callback
- Fixed bug where the refcount of the `Cast` string could underflow
- Concurrency fixes
- Various mapping fixes


### v26

- Added support for Hotfix 33
- Fix possible crash when loading level without an SE context
- Fix crash when Noesis event is thrown without a handler present
- Fix unsafe VM access during network variable sync
- Fix crash when all VT tilesets fail to load
- Fix crash when removing ECS components
- Fix reading/writing AIFlags property on stats objects
- Potential fix for texture binding crashes
- Various type mapping fixes


## v25

- Added `Ext.System` for accessing various ECS systems
- Added system pre/post-update events via `Ext.Entity.OnSystemUpdate` and `Ext.Entity.OnSystemPostUpdate`
- Mapped the following game systems: `ServerShapeshift, ServerCastRequest, ServerSpell, ServerCombat, ServerTurnOrder, ServerActionResource, ServerBoost, ServerPassive, ServerRoll, ServerStats, ServerVisual, ServerSpellCooldown, ServerSpellLearning, ServerHit, ServerProgression, ServerInterruptDecision, ServerInterruptRequests, ServerInterruptManagement, ServerSummonSpawn, ServerSummonDespawn, ServerCapabilities, ServerTemplateChange, ServerConcentration, ServerExperience, ServerShortRest, ServerLongRest, ServerFTBZone, ServerStatusRequest, ServerInventoryCanPlace, ServerInventoryReceivalNotification, ServerInventoryEquipment, ServerInventoryInteractionRequest, ServerInventoryInteraction, ServerInventoryLocking, ServerMagicPocketsTracking, ServerInventoryManagement, ServerNewInventoryMember, ServerInventoryStack, ServerTradeBuyback, ServerTreasureGeneration, ServerGod, ServerRestore, ServerBodyType, ServerDualWielding, ServerFalling, ServerDisplayName, ServerGravity, ServerLeader, ServerPartyTeleport, ServerPingRequest, ServerPlatform, ServerRating, ServerAttitude, ServerParty, ClientEquipmentVisuals, ClientVisual, ClientCharacterIconRender, ClientCharacterManager, ClientVisualsVisibilityState, SoundRouting`
- Add option to disable ModCrashSanityCheck
- Upgrade to IMGUI 1.91
- Added IMGUI TextLink support
- Added global UI scaler via Ext.IMGUI.SetUIScaleMultiplier and Ext.IMGUI.SetFontScaleMultiplier
- Added `OnClick` and `OnRightClick` events to every IMGUI object
- Improved detection for situations where client and server states may be used on the same thread
- Fix possible concurrency issue when updating translated keys
- Fix concurrency issue during SDL deferred input event handling
- Fix concurrency issues during Lua VM reset
- Fix crash when Noesis delegates are called on the server thread
- Fix possible crash when binding a non-2D texture to IMGUI
- Fix crash when IMGUI table column count doesnt match column defs
- Fix IMGUI crash on unclean shutdown
- Fix crash when passing null or invalid entity to certain functions
- Fix context assertion on exit
- Fix incorrect handling of stats types `InterruptContext, InterruptContextScope, InterruptDefaultValue, InterruptFlagsList, AuraFlags, AbilityFlags`
- Fix viewport resize not working properly on DX11
- Improve FixedString caching performance
- Various type mapping updates

## v24

- Support for hotfix 30
- Fixed bug where IMGUI objects were not deleted after a reset
- Fixed crash when IMGUI textures were released while still in use by the renderer
- Fixed JSON serializer not properly detecting recursion for IMGUI objects
- Fixed bug where `RaycastAll()` result would retain hits from previous calls
- Various mapping fixes

## v23 Hotfix 1

- Fixed crash when calling Osi user queries
- Fixed crash when reading child count of a non-visual Noesis object
- Fixed crash when loading IMGUI fonts before the IMGUI backend is initialized
- Fixed bug where GlobalSwitches was inaccessible


## v23

- Add support for Patch 8
- Add Genome mappings
- Add Skeleton mappings
- Add InputManager mappings (accessible via `Ext.Input.GetInputManager()`)
- Add client character input controller mappings (accessible via `entity.ClientCharacter.InputController`)
- Add support for getting entities NetId (via `entity.GetNetId()`) and resolving entities using their NetId (via `Ext.Entity.Get(id)`)
- Add support for reading IMGUI window size and position via `window.Size` and `window.Position` properties
- Add fast draw optimization for large tables (using `table.OptimizedDraw` property)
- Improve performance of cppobject property reads and writes
- Improve performance of accessing Noesis properties
- Fix mapping of audio object ID-s in `Ext.Audio`
- Fix crash when subscribing to ECS events
- Fix physics hit data not being cleared on repeat calls to Ext.Level.RaycastX or Ext.Level.SweepX
- Fix possible crash when a function ref was being deleted
- Fix composite bitfield values (i.e. multiple bits set) not being properly mapped to enum labels
- Fix empty string ID being incompatible with game
- Fix AI path requests not being initialized correctly
- Fix AI path player weight calculation
- Fix possible crash when object has nonexistent modifier list
- Fix debugger not being able to to read properties on IMGUI elements
- Temporary workaround to Noesis refcounting crash in Patch 8

## v22

### Stats
- Added the `ExecuteFunctor` event that fires before a stats functor is executed
- Added `Ext.Stats.ExecuteFunctors`, `ExecuteFunctor` and `PrepareFunctorParams` for programmatic execution of stats functors

### Entity system
- Added `Ext.Entity.Create`, `Destroy`, `RemoveComponent`
- It is now possible to retrieve and enumerate one-frame components that were created in the current frame
 
### Input
- Added support for programmatic triggering of input events via `Ext.Input.InjectKeyPress`, `InjectKeyDown`, `InjectKeyUp`

### Physics
- Mapped `Physics` and `StaticPhysics` components
- Added physics query functions to `Ext.Level`: `RaycastClosest`, `RaycastAny`, `RaycastAll`, `SweepSphereClosest`, `SweepCapsuleClosest`, `SweepBoxClosest`, `SweepCylinderClosest`, `SweepSphereAll`, `SweepCapsuleAll`, `SweepBoxAll`, `SweepCylinderAll`, `TestBox`, `TestSphere`

### Game objects
- Added client-side character statuses and VFX via `entity.ClientCharacter.StatusManager`
- Added mappings for server action, behavior, movement and steering states via `entity.ServerCharacter.AiActionMachine`, `AiBehaviourMachine`, `AiMovementMachine`, `AiSteeringMachine`
- Added server-side Osiris action states via `entity.ServerCharacter.OsirisController`

### Networking
- Added channel-based network communication API via `Ext.Net.CreateChannel()`
- Reduced client-server latency when communicating with local peer via channels

### UI
- Added access to mouse cursor settings via `Ext.UI.GetCursorControl()`
- Added access to drag&drop manager state via `Ext.UI.GetDragDrop()`
- Added support for retrieving current table sorting settings via the `Sorting` property and the `OnSortChanged` event to notify when the sorting is changed
- Allow freezing table rows/cols via `table.FreezeRows`, `table.FreezeCols`
- Added `SetPos`, `SetSize`, `SetSizeConstraints`, `SetContentSize`, `SetCollapsed`, `SetFocus`, `SetScroll`, `SetBgAlpha` support to child windows, tooltips and popups
- Added support for setting whether UI measures (eg. offset, size, etc) are absolute or scaled via the `Scaling` property on windows
- Fixed `Destroy()` not working on top-level UI objects
- Fixed crash when drawing a table with headers that's not in the viewport
- Fixed crash when drawing windows with empty `Label` value
- Fixed properties of Noesis parent classes not being visible

### Notable fixes
- Fixed `ModuleLoadStarted `event not firing
- Compatibility with vanilla `require()` behavior
- Allow writing null bytes to files
- Fixed crash when updating treasure tables
- Improved performance of accessing entity components and cpp object subproperties by ~50%

### Misc
- Added `_C()` helper on client
- Added `Ext.Level.GetActivePathfindingRequests`
- Updated initial pathing settings to more closely resemble ingame ones
- Added AI path player weighting
- Fixed aspk component mappings
- Fixed LightingResource mappings
- Fixed DialogInstance mappings
- Improved Lua type mismatch error messages
- Various component mapping updates
