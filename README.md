# Norbyte's Divinity Script Extender

Downloads available [>>>HERE<<<](https://github.com/Norbyte/ositools/releases/latest).

The Script Extender toolkit adds previously unavailable features to the scripting language of the game.

Features:
 - Lots of new Osiris calls and queries ([API Documentation](https://github.com/Norbyte/ositools/blob/master/APIDocs.md))
 - Lua scripting ([API Documentation](https://github.com/Norbyte/ositools/blob/master/LuaAPIDocs.md))
 - Real-time debugging of Osiris code ([Usage](https://gist.github.com/Norbyte/8b7eb35cd17f799ea113636b15e1f967))
 - Significant improvements in modded game loading times
 - Debug logging of Osiris rule execution/compilation

### Configuration

The following configuration variables can be set in the `OsirisExtenderSettings.json` file:

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
