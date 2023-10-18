param($Channel)

if ($Channel -ne "Devel" -And $Channel -ne "Release" -And $Channel -ne "Nightly") {
	Write-Error -Message "Usage: publish.ps1 <Channel>"
	Exit 1
}

$GameChannel = $Channel
$EditorChannel = $Channel + "Editor"
$S3Bucket = "nb-stor"
$S3RootPath = "bg3-updater-v3/Channels"
$CloudFrontRootPath = "/Channels"
$CloudFrontRootURL = "https://bg3se-updates.norbyte.dev/Channels"
$CloudFrontDistributionID = "EVZRUV2YONUZF"

$S3ManifestPath = "Manifest.json"

$PublishingRoot = Join-Path "C:\Dev\LS\BG3PublishingRoot" "$Channel"
$SigningKey = Join-Path "C:\Dev\LS\BG3PublishingRoot" "package-signer.key"
$BuildRoot = Join-Path "$PublishingRoot" "Build"
$PDBRoot = Join-Path "$PublishingRoot" "PDB"
$RootPath = (Get-Location).Path
$GameManifestPath = Join-Path $PublishingRoot "GameManifest.json"
$EditorManifestPath = Join-Path $PublishingRoot "EditorManifest.json"
$GameBuildZipPath = Join-Path $BuildRoot "GameLatest.zip"
$GameManualBuildZipPath = Join-Path $BuildRoot "GameLatestManual.zip"
$GameDebugAdapterBuildZipPath = Join-Path $BuildRoot "DebugAdapter.zip"
$EditorBuildZipPath = Join-Path $BuildRoot "EditorLatest.zip"
$GameDllPath = Join-Path $RootPath "x64\Game Release\BG3ScriptExtender.dll"
$EditorDllPath = Join-Path $RootPath "x64\Editor Release\BG3EditorScriptExtender.dll"

$GameBuildDir = Join-Path "$BuildRoot" "TempGameBuild"
$GameManualBuildDir = Join-Path "$BuildRoot" "TempGameManualBuild"
$GameDebugAdapterBuildDir = Join-Path "$BuildRoot" "TempGameDebugAdapter"
$EditorBuildDir = Join-Path "$BuildRoot" "TempEditorBuild"
$PDBDir = Join-Path "$PDBRoot" "TempPDB"
	
$EditorMinVersion = "4.37.32.833"
$EditorMaxVersion = "-"

$GameMinVersion = "4.37.32.833"
$GameMaxVersion = "-"

function Build-Extender
{
	# Force a build date refresh
	(gci BG3Extender/Extender/Shared/Console.cpp).LastWriteTime = Get-Date

	Write-Output " ===== BUILDING GAME EXTENDER ===== "
	msbuild BG3Tools.sln "/p:Configuration=Game Release" /t:Build /m /nologo /verbosity:quiet /consoleloggerparameters:summary
	
	x64\Release\SymbolTableGenerator.exe "x64\Game Release\BG3ScriptExtender.pdb" "BG3Extender\GameHooks\BG3ScriptExtender.symtab"
	
	msbuild BG3Tools.sln "/p:Configuration=Game Release" /t:Build /m /nologo /verbosity:quiet /consoleloggerparameters:summary
	
	# Write-Output " ===== BUILDING EDITOR EXTENDER ===== "
	# msbuild BG3Tools.sln "/p:Configuration=Editor Release" /t:Build /m /nologo /verbosity:quiet /consoleloggerparameters:summary
}

function Create-PDBDir
{
	Remove-Item $PDBDir -Recurse -ErrorAction SilentlyContinue
	New-Item $PDBDir -ItemType "directory"

	Copy-Item "x64\Release\CrashReporter.pdb" -Destination $PDBDir\CrashReporter.pdb
	Copy-Item "x64\Game Release\BG3ScriptExtender.pdb" -Destination $PDBDir\BG3ScriptExtender.pdb
	#Copy-Item "x64\Editor Release\BG3EditorScriptExtender.pdb" -Destination $PDBDir\BG3EditorScriptExtender.pdb
	Copy-Item "x64\Release\CrashReporter.exe" -Destination $PDBDir\CrashReporter.exe
	Copy-Item "x64\Game Release\BG3ScriptExtender.dll" -Destination $PDBDir\BG3ScriptExtender.dll
	#Copy-Item "x64\Editor Release\BG3EditorScriptExtender.dll" -Destination $PDBDir\BG3EditorScriptExtender.dll
	Copy-Item External\protobuf\bin\libprotobuf-lite.dll -Destination $PDBDir\libprotobuf-lite.dll

	if ($Channel -eq "Devel" -Or $Channel -ne "Nightly") {
		New-Item $PDBDir\DAP -ItemType "directory"
		Copy-Item "LuaDebugger\bin\Release\Google.Protobuf.dll" -Destination $PDBDir\DAP\Google.Protobuf.dll
		Copy-Item "LuaDebugger\bin\Release\Newtonsoft.Json.dll" -Destination $PDBDir\DAP\Newtonsoft.Json.dll
		Copy-Item "LuaDebugger\bin\Release\LuaDebugger.exe" -Destination $PDBDir\DAP\LuaDebugger.exe
		Copy-Item "LuaDebugger\bin\Release\LuaDebugger.exe.config" -Destination $PDBDir\DAP\LuaDebugger.exe.config
	}
}

function Create-Update-Package ($BuildDir, $ZipPath, $HasEditor, $HasGame)
{
	Remove-Item $BuildDir -Recurse -ErrorAction SilentlyContinue
	New-Item $BuildDir -ItemType "directory"

	git show -s --format="BG3SE Version: Commit %H, %cD" > $BuildDir\Version.txt

	Copy-Item "x64\Release\CrashReporter.exe" -Destination $BuildDir\CrashReporter.exe
	Copy-Item External\protobuf\bin\libprotobuf-lite.dll -Destination $BuildDir\libprotobuf-lite.dll
	
	if ($HasEditor)
	{
		Copy-Item "x64\Editor Release\BG3EditorScriptExtender.dll" -Destination $BuildDir\BG3EditorScriptExtender.dll
	}
	
	if ($HasGame)
	{
		Copy-Item "x64\Game Release\BG3ScriptExtender.dll" -Destination $BuildDir\BG3ScriptExtender.dll
	}
	
	Remove-Item $ZipPath -ErrorAction SilentlyContinue
	Compress-Archive -Path $BuildDir\* -DestinationPath $ZipPath -CompressionLevel Optimal -Force
}

function Create-ManualInstall-Package ($BuildDir, $ZipPath)
{
	Remove-Item $BuildDir -Recurse -ErrorAction SilentlyContinue
	New-Item $BuildDir -ItemType "directory"

	Copy-Item External\protobuf\bin\libprotobuf-lite.dll -Destination $BuildDir\libprotobuf-lite.dll
	Copy-Item "x64\Game Release\BG3ScriptExtender.dll" -Destination $BuildDir\DWrite.dll
	
	Remove-Item $ZipPath -ErrorAction SilentlyContinue
	Compress-Archive -Path $BuildDir\* -DestinationPath $ZipPath -CompressionLevel Optimal -Force
}

function Create-DebugAdapter-Package ($BuildDir, $ZipPath)
{
	Remove-Item $BuildDir -Recurse -ErrorAction SilentlyContinue
	New-Item $BuildDir -ItemType "directory"

	Copy-Item "LuaDebugger\bin\Release\Google.Protobuf.dll" -Destination $BuildDir\Google.Protobuf.dll
	Copy-Item "LuaDebugger\bin\Release\Newtonsoft.Json.dll" -Destination $BuildDir\Newtonsoft.Json.dll
	Copy-Item "LuaDebugger\bin\Release\LuaDebugger.exe" -Destination $BuildDir\LuaDebugger.exe
	Copy-Item "LuaDebugger\bin\Release\LuaDebugger.exe.config" -Destination $BuildDir\LuaDebugger.exe.config
	
	Remove-Item $ZipPath -ErrorAction SilentlyContinue
	Compress-Archive -Path $BuildDir\* -DestinationPath $ZipPath -CompressionLevel Optimal -Force
}

function Archive-Build ($Digest, $ZipPath)
{
	Write-Output "Digest: " $Digest
	Write-Output "Zip path: " $ZipPath
	$ArchivePDBDir = Join-Path "$PDBRoot" "$Digest"
	New-Item $ArchivePDBDir -ItemType directory -ErrorAction SilentlyContinue
	Copy-Item $PDBDir\* -Destination $ArchivePDBDir
	Copy-Item $ZipPath -Destination $BuildRoot\$Digest.zip
}

function Build-Package ($BuildDir, $ZipPath, $DllPath, $Channel, $IsGame, $IsEditor)
{
	Create-Update-Package $BuildDir $ZipPath $IsEditor $IsGame

	x64\Release\UpdateSigner.exe sign $SigningKey $ZipPath
	x64\Release\UpdateSigner.exe verify $ZipPath

	if ($IsEditor)
	{
		x64\Release\UpdateSigner.exe update-manifest "$EditorManifestPath" ScriptExtender "$ZipPath" "$DllPath" $EditorMinVersion $EditorMaxVersion "$CloudFrontRootURL/$Channel/Packages/"
	}
	
	if ($IsGame)
	{
		x64\Release\UpdateSigner.exe update-manifest "$GameManifestPath" ScriptExtender "$ZipPath" "$DllPath" $GameMinVersion $GameMaxVersion "$CloudFrontRootURL/$Channel/Packages/"
	}
}

function Publish-Package ($ZipPath, $DigestPath, $Channel, $ManifestPath)
{
	Write-Output "S3 path: s3://$S3Bucket/$S3RootPath/$Channel/Packages/$DigestPath"
	Write-Output "CloudFront path: $CloudFrontRootPath/$Channel/Packages/$DigestPath"
	Write-Output "URL: $CloudFrontRootURL/$Channel/Packages/$DigestPath"

	aws s3 cp $ZipPath "s3://$S3Bucket/$S3RootPath/$Channel/Packages/$DigestPath"
	aws s3api put-object-acl --bucket nb-stor --key "$S3RootPath/$Channel/Packages/$DigestPath" --acl public-read
	aws s3 cp $ManifestPath "s3://$S3Bucket/$S3RootPath/$Channel/$S3ManifestPath"
	aws s3api put-object-acl --bucket nb-stor --key "$S3RootPath/$Channel/$S3ManifestPath" --acl public-read
	aws cloudfront create-invalidation --distribution-id $CloudFrontDistributionID --paths "$CloudFrontRootPath/$Channel/$S3ManifestPath" "$CloudFrontRootPath/$Channel/Packages/$DigestPath"
}

function Publish-Manual-Package ($ZipPath, $Channel)
{
	Write-Output "S3 path: s3://$S3Bucket/$S3RootPath/$Channel/ManualInstall.zip"
	Write-Output "CloudFront path: $CloudFrontRootPath/$Channel/ManualInstall.zip"
	Write-Output "URL: $CloudFrontRootURL/$Channel/ManualInstall.zip"

	aws s3 cp $ZipPath "s3://$S3Bucket/$S3RootPath/$Channel/ManualInstall.zip"
	aws s3api put-object-acl --bucket nb-stor --key "$S3RootPath/$Channel/ManualInstall.zip" --acl public-read
}

function Publish-DebugAdapter-Package ($ZipPath, $Channel)
{
	aws s3 cp $ZipPath "s3://$S3Bucket/$S3RootPath/$Channel/DebugAdapter.zip"
	aws s3api put-object-acl --bucket nb-stor --key "$S3RootPath/$Channel/DebugAdapter.zip" --acl public-read
}

Build-Extender

Build-Package $GameBuildDir $GameBuildZipPath $GameDllPath $GameChannel 1 0
Create-ManualInstall-Package $GameManualBuildDir $GameManualBuildZipPath
Create-DebugAdapter-Package $GameDebugAdapterBuildDir $GameDebugAdapterBuildZipPath
# Build-Package $EditorBuildDir $EditorBuildZipPath $EditorDllPath $EditorChannel 0 1

Create-PDBDir

$GameDigestPath = (x64\Release\UpdateSigner.exe compute-path "$GameBuildZipPath" "$GameDllPath")
# $EditorDigestPath = (x64\Release\UpdateSigner.exe compute-path "$EditorBuildZipPath" "$EditorDllPath")

Archive-Build $GameDigestPath $GameBuildZipPath
# Archive-Build $EditorDigestPath $EditorBuildZipPath

Write-Output "Build completed."
Write-Output "."

Write-Output "*********************************************************************************"
Write-Output ""
Write-Output "                      ABOUT TO PUBLISH ON CHANNEL $Channel!"
Write-Output ""
Write-Output "*********************************************************************************"
Read-Host

Write-Output " ===== UPLOADING PACKAGES ===== "
Publish-Package $GameBuildZipPath $GameDigestPath $GameChannel $GameManifestPath
Publish-Manual-Package $GameManualBuildZipPath $GameChannel
Publish-DebugAdapter-Package $GameDebugAdapterBuildZipPath $GameChannel
# Publish-Package $EditorBuildZipPath $EditorDigestPath $EditorChannel $EditorManifestPath
