# BG3 Lua Debugger Setup

## Installation

The debugger requires VS Code. Follow these steps to setup VS Code and the required extensions.

- Install VS Code: https://code.visualstudio.com/
- Install the BG3 Lua Debugger extension: [https://bg3se-updates.norbyte.dev/Stuff/bg3-lua-debugger-1.0.0.vsix](https://bg3se-updates.norbyte.dev/Stuff/bg3-lua-debugger-1.0.0.vsix "https://bg3se-updates.norbyte.dev/Stuff/bg3-lua-debugger-1.0.0.vsix")
	- Open File -> Preferences -> Extensions and drag the VSIX file to the extension pane to install
- Download and extract the debug adapter to a standalone folder: https://bg3se-updates.norbyte.dev/Channels/Devel/DebugAdapter.zip

A launch configuration must be set up in the workspace of your current mod:

 - Open the command palette (Ctrl+Shift+P) -> Debug: Add Configuration ... -> BG3 Script Extender - Lua Debug
 - In the generated launch config, replace `"program": "",` with the path to LuaDebugger.exe in the downloaded debug adapter; eg. `"program": "C:\\Whatever\\DebugAdapter\\LuaDebugger.exe",`. Make sure to escape backslashes (i.e. use `\\`, not just `\`).

Remote debugging must be enabled in the script extender configuration:

 - Open the script extender configuration file `ScriptExtenderSettings.json` in the `bin` folder of your game (eg. `C:\Program Files (x86)\Steam\steamapps\common\Baldurs Gate 3\bin\ScriptExtenderSettings.json`)
	 - Add the setting `"EnableLuaDebugger": true`

If done correctly, debugger startup will show up the extender console log:

```
Starting Lua debugger server
Debugger::Debugger(): Starting Lua debugger
Lua debugger listening on 127.0.0.1:9998; DBG protocol version 3
```

## Usage

The debugger will attach to the running game, i.e. you should start the debug session (F5 key) after launching the game. If the game is not running or the extender debugger is not enabled, debug launch will fail with a `Could not connect to Lua debugger backend: No connection could be made [...]` error message.

Supported features:

 - Single-stepping (break, continue, step over, step into, step out functions)
 - Call stack viewer
 - Inspecting local variables and upvalues within any stack frame
 - Watches window
 - Debug break on exception
 - Breakpoints are supported; conditional breakpoints and logpoints are not supported
 - Evaluating Lua code in console and inspecting the result of evaluated code
