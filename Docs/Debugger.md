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

To test your mod(s), place your ScriptExtender/ and meta.lsx files (not .pak'd) in `<Baldurs Gate 3 path>/Data/Mods/<ModName>`. If you're using the [Recommended Mod structure](https://github.com/ShinyHobo/BG3-Modders-Multitool/wiki/Mod-Workspace-Structure), you can create a directory-type symblink to your vscode project's `<project-path>\Mods\<mod-name>` folder.

<details>
<summary>Creating symblink examples</summary>
	
Windows CMD (Not Powershell):
```shell
 mklink /D "<BG3 Install Path>\Data\Mods\<ModName>" "<VSCodes Project Path>\Mods\<ModName>"
```

Unix Shell:
```shell
ln -s <VSCodes Project Path>/Mods/<ModName>/ <BG3 Install Path>/Data/Mods/
```
 
</details>

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

## QOL
You can configure VSCode to launch and kill the bg3.exe process, with arguments, and tie them to the startup and shutdown of your debugging instance via [Tasks](https://code.visualstudio.com/Docs/editor/tasks). This allows you to start and stop BG3 from within VSCode, attaching the debugger at the earliest possible point.
<details>
<summary>.vscode\tasks.json</summary>
	
```json 
{
	// See https://go.microsoft.com/fwlink/?LinkId=733558
	// for the documentation about the tasks.json format
	"version": "2.0.0",
	"tasks": [
		{
			"label": "bg3Continue",
			"type": "shell",
			"windows": {
				"command": "Start-Process",
				"args": [
					"-FilePath",
					"<BG3 Install Path>\\bin\\bg3.exe",
					"-ArgumentList",
					"-continueGame --skip-launcher", // Starts the last game you had loaded automagically
					"-WorkingDirectory",
					"<BG3 Install Path>\\bin"
				],
				"options": {
					"shell": {
						"executable": "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe"
					}
				},
			}
		},
		{
			"label": "bg3Kill",
			"type": "shell",
			"windows": {
				"command": "taskkill",
				"args": [
					"/IM",
					"bg3.exe"
				],
				"options": {
					"shell": {
						"executable": "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe"
					}
				},
			}
		}
	]
}
```
</details>

<details>
<summary>.vscode\launch.json</summary>
<Section hyperlinks don't work inside a details block>
	
Follow process outlined in the [Installation Section](https://github.com/Norbyte/bg3se/blob/main/Docs/Debugger.md#installation) section first, then modify the configurations block to the include the following fields:
	
```json
	...
	"configurations": [
		{
			"preLaunchTask": "bg3Continue",
			"postDebugTask": "bg3Kill",
			"internalConsoleOptions": "openOnSessionStart",
                        ...
```

</details>
