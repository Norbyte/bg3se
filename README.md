# Norbyte's Baldur's Gate 3 Script Extender

# PATCH 4 IS NOT YET SUPPORTED!

[Downloads available here](https://github.com/Norbyte/bg3se/releases)

The Script Extender adds Lua/Osiris scripting support to the game.
[API Documentation](https://github.com/Norbyte/bg3se/blob/master/Docs/API.md)

### Configuration

The following configuration variables can be set in the `ScriptExtenderSettings.json` file:

| Variable | Type | Description |
|--|--|--|
| CreateConsole | Boolean | Creates a console window that logs extender internals. Mainly useful for debugging. |
| EnableLogging | Boolean | Enable logging of Osiris activity (rule evaluation, queries, etc.) to a log file. |
| LogRuntime | Boolean | Log extender console and script output to a log file. |
| LogCompile | Boolean | Log Osiris story compilation to a log file. |
| LogFailedCompile | Boolean | Log errors during Osiris story compilation to a log file. |
| LogDirectory | String | Directory where the generated Osiris logs will be stored. Default is `My Documents\OsirisLogs` |
| EnableExtensions | Boolean | Make the Osiris extension functionality available ingame or in the editor. |
| SendCrashReports | Boolean | Upload minidumps to the crash report collection server after a game crash. |
| DumpNetworkStrings | Boolean | Dumps the NetworkFixedString table to `LogDirectory`. Mainly useful for debugging desync issues. |
| DeveloperMode | Boolean | Enables various debug functionality for development purposes. |
| DisableModValidation | Boolean | Disable module hashing when loading modules. |
| EnableAchievements | Boolean | Re-enable achievements for modded games. |
| EnableDebugger | Boolean | Enables the Osiris debugger interface |
| DebuggerPort | Integer | Port number the Osiris debugger will listen on (default 9999) |
| EnableLuaDebugger | Boolean | Enables the Lua debugger interface |
| LuaDebuggerPort | Integer | Port number the Lua debugger will listen on (default 9998) |
