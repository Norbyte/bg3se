param(
    [string]$WorkspaceFolder = $PSScriptRoot,
    [string]$GameBinFolder = "F:\Games\Baldurs Gate 3\bin",
    [string]$GameLocalFolder = "C:\Users\thier\AppData\Local\BG3ScriptExtender\ScriptExtender\26.0.0.0_4762de214be86ca9b2d5cc59f68afa30bb0c8a10984391340ec26033a9190f3e"
)

Write-Host "Running generate-property-maps.bat..." -ForegroundColor Cyan
& "${WorkspaceFolder}\generate-property-maps.bat"
if ($LASTEXITCODE -ne 0) { Write-Error "generate-property-maps.bat FAILED!"; exit 1 }
Write-Host "generate-property-maps.bat successful." -ForegroundColor Green

Write-Host "Running generate-proto.bat..." -ForegroundColor Cyan
& "${WorkspaceFolder}\generate-proto.bat"
if ($LASTEXITCODE -ne 0) { Write-Error "generate-proto.bat FAILED!"; exit 1 }
Write-Host "generate-proto.bat successful." -ForegroundColor Green

Write-Host "Building SymbolTableGenerator.vcxproj..." -ForegroundColor Cyan
& "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe" "${WorkspaceFolder}\SymbolTableGenerator\SymbolTableGenerator.vcxproj" "/p:Configuration=Release" "/p:Platform=x64" /t:Build /m /nologo /verbosity:quiet /consoleloggerparameters:summary
if ($LASTEXITCODE -ne 0) { Write-Error "MSBuild SymbolTableGenerator.vcxproj FAILED!"; exit 1 }
Write-Host "SymbolTableGenerator.vcxproj build successful." -ForegroundColor Green

Write-Host "Building BG3Tools.sln..." -ForegroundColor Cyan
& "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe" "${WorkspaceFolder}\BG3Tools.sln" "/p:Configuration=Release" /t:Clean,Build /m /nologo /verbosity:quiet /consoleloggerparameters:summary
if ($LASTEXITCODE -ne 0) { Write-Error "MSBuild BG3Tools.sln FAILED!"; exit 1 }
Write-Host "BG3Tools.sln build successful." -ForegroundColor Green

Write-Host "Running SymbolTableGenerator..." -ForegroundColor Cyan
& "${WorkspaceFolder}\x64\Release\SymbolTableGenerator.exe" "${WorkspaceFolder}\x64\Game Release\BG3ScriptExtender.pdb" "${WorkspaceFolder}\BG3Extender\GameHooks\BG3ScriptExtender.symtab"
if ($LASTEXITCODE -ne 0) { Write-Error "SymbolTableGenerator.exe FAILED!"; exit 1 }
Write-Host "SymbolTableGenerator successful." -ForegroundColor Green

Write-Host "Copying Updater DLL to game folder..." -ForegroundColor Cyan
$DestinationDll1 = Join-Path -Path $GameBinFolder -ChildPath "ScriptExtender.dll"
Copy-Item -Path "${WorkspaceFolder}\x64\Release\BG3Updater.dll" -Destination $DestinationDll1 -Force
if ($LASTEXITCODE -ne 0) { Write-Error "Copying DLL FAILED!"; exit 1 }
Write-Host "DLL successfully copied to '$DestinationDll1'!" -ForegroundColor Green

Write-Host "Copying Game DLL to local folder..." -ForegroundColor Cyan
$DestinationDll2 = Join-Path -Path $GameLocalFolder -ChildPath "BG3ScriptExtender.dll"
Copy-Item -Path "${WorkspaceFolder}\x64\Game Release\BG3ScriptExtender.dll" -Destination $DestinationDll2 -Force
if ($LASTEXITCODE -ne 0) { Write-Error "Copying DLL FAILED!"; exit 1 }
Write-Host "DLL successfully copied to '$DestinationDll2'!" -ForegroundColor Green


Write-Host "Build and deploy process completed successfully." -ForegroundColor Green
