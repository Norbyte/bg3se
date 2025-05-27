ON EVERY GIT CLONE:

.\nuget.exe restore BG3Tools.sln

download External

get new imgui version and rename to imgui,
copy/paste SDL2-2.30.1 to SDL2 and keep old too

MY TRACK RECORD:

remove line dx11 in DX11.inl =         //if (!initializationFailed_ && !ImGui_ImplDX11_RenderObjectsInitialized()) { DX11 PROB DOESNT WORK ANYMORE WITHOUT IT?

Need Build Tools 2022 from Microsoft

vs_installer.exe: C++ ATL for latest v143 build tools (x86 & x64) in Visual Studio Installer



run in admin powershell: regsvr32 "C:\Program Files\Microsoft Visual Studio\2022\Community\DIA SDK\bin\amd64\msdia140.dll"

IF NOT DONE ALREADY: remove PostBuild in BG3Extender.vcxproj and BG3Updater.vcxproj and fix UpdateSigner.vcxproj/BG3Extender.vcxproj/BG3Updater.vcxproj/UpdateSigner.vcxproj with correct AdditionalIncludeDirectories

create Generated folder in correct place if needed...

remove pause in .bat

change path MSBuild.exe in build_and_deploy_extender.ps1

run build_and_deploy_extender.ps1
