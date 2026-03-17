param($Channel)

if ($Channel -ne "Devel" -And $Channel -ne "Release" -And $Channel -ne "Nightly") {
	Write-Error -Message "Usage: publish.ps1 <Channel>"
	Exit 1
}

$GameChannel = $Channel
$S3Bucket = "nb-stor"
$S3RootPath = "bg3-updater-v3/Channels"
$CloudFrontRootPath = "/Channels"
$CloudFrontRootURL = "https://bg3se-updates.norbyte.dev/Channels"
$CloudFrontDistributionID = "EVZRUV2YONUZF"

$S3ManifestPath = "Manifest.json"

$PublishingBaseDir = "D:\Dev\LS\BG3PublishingRoot"
$PublishingRoot = Join-Path "$PublishingBaseDir" "$Channel"
$SigningKey = Join-Path "$PublishingBaseDir" "package-signer.key"
$CertificationFile = Join-Path "$PublishingBaseDir" "certification.txt"
$BuildRoot = Join-Path "$PublishingRoot" "Build"
$PDBRoot = Join-Path "$PublishingRoot" "PDB"
$UpdaterBuildRoot = Join-Path "$PublishingRoot" "UpdaterBuild"
$RootPath = (Get-Location).Path
$GameManifestPath = Join-Path $PublishingRoot "GameManifest.json"
$GameBuildZipPath = Join-Path $BuildRoot "GameLatest.zip"
$GameOfflineBuildZipPath = Join-Path $BuildRoot "GameLatestOffline.zip"
$GameDebugAdapterBuildZipPath = Join-Path $BuildRoot "DebugAdapter.zip"
$GameDllPath = Join-Path $RootPath "x64\Game Release\BG3ScriptExtender.dll"

$GameBuildDir = Join-Path "$BuildRoot" "TempGameBuild"
$GameOfflineBuildDir = Join-Path "$BuildRoot" "TempGameOfflineBuild"
$GameDebugAdapterBuildDir = Join-Path "$BuildRoot" "TempGameDebugAdapter"
$PDBDir = Join-Path "$PDBRoot" "TempPDB"

$GameMinVersion = "4.69.95.0"
$GameMaxVersion = "-"

function Build-Extender
{
	# Force a build date refresh
	(gci BG3Extender/Extender/Shared/Console.cpp).LastWriteTime = Get-Date

	if ($Channel -eq "Release") {
		$BuildInfo = "#undef SE_IS_DEVELOPER_BUILD`r`n"
	} else {
		$BuildInfo = "#define SE_IS_DEVELOPER_BUILD`r`n"
	}
	
	$BuildInfo += "#define SE_BUILD_CHANNEL `"$Channel`"`r`n"

	if (Test-Path $CertificationFile) {
		$CertificationId = Get-Content -Path $CertificationFile
		$BuildInfo += "#define SE_CERTIFICATION_ID `"$CertificationId`"`r`n"
	} else {
		$BuildInfo += "#undef SE_CERTIFICATION_ID`r`n"
	}
	
	echo $BuildInfo | Out-File -FilePath BG3Extender/Extender/BuildInfo.h
	(gci BG3Extender/Extender/BuildInfo.h).LastWriteTime = Get-Date

	if ($Channel -eq "Release") {
		$CLConfig = "#undef SE_RELEASE_ASSERTS`r`n"
	} else {
		$CLConfig = "#define SE_RELEASE_ASSERTS`r`n"
	}
	
	echo $CLConfig | Out-File -FilePath CoreLib/Config.h
	(gci CoreLib/Config.h).LastWriteTime = Get-Date

	Write-Output " ===== BUILDING GAME EXTENDER ===== "
	msbuild BG3Tools.sln "/p:Configuration=Game Release" /t:Build /m /nologo /verbosity:quiet /consoleloggerparameters:summary
	
	Write-Output " ===== BUILDING SYMTAB ===== "
	x64\Release\SymbolTableGenerator.exe "x64\Game Release\BG3ScriptExtender.pdb" "x64\Game Release\BG3ScriptExtender.symtab"
	
	Write-Output " ===== FIN. ===== "
}

function Create-PDBDir
{
	Remove-Item $PDBDir -Recurse -ErrorAction SilentlyContinue
	New-Item $PDBDir -ItemType "directory"

	Copy-Item "x64\Release\CrashReporter.pdb" -Destination $PDBDir\CrashReporter.pdb
	Copy-Item "x64\Game Release\BG3ScriptExtender.pdb" -Destination $PDBDir\BG3ScriptExtender.pdb
	Copy-Item "x64\Release\CrashReporter.exe" -Destination $PDBDir\CrashReporter.exe
	Copy-Item "x64\Game Release\BG3ScriptExtender.dll" -Destination $PDBDir\BG3ScriptExtender.dll
	Copy-Item "x64\Game Release\BG3ScriptExtender.symtab" -Destination $PDBDir\BG3ScriptExtender.symtab
	Copy-Item External\protobuf\bin\libprotobuf-lite.dll -Destination $PDBDir\libprotobuf-lite.dll

	if ($Channel -eq "Devel" -Or $Channel -eq "Nightly") {
		New-Item $PDBDir\DAP -ItemType "directory"
		Copy-Item "LuaDebugger\bin\Release\Google.Protobuf.dll" -Destination $PDBDir\DAP\Google.Protobuf.dll
		Copy-Item "LuaDebugger\bin\Release\Newtonsoft.Json.dll" -Destination $PDBDir\DAP\Newtonsoft.Json.dll
		Copy-Item "LuaDebugger\bin\Release\LuaDebugger.exe" -Destination $PDBDir\DAP\LuaDebugger.exe
		Copy-Item "LuaDebugger\bin\Release\LuaDebugger.exe.config" -Destination $PDBDir\DAP\LuaDebugger.exe.config
	}
}

function Create-Update-Package ($BuildDir, $ZipPath)
{
	Remove-Item $BuildDir -Recurse -ErrorAction SilentlyContinue
	New-Item $BuildDir -ItemType "directory"

	git show -s --format="BG3SE Version: Commit %H, %cD" > $BuildDir\Version.txt

	Copy-Item "x64\Release\CrashReporter.exe" -Destination $BuildDir\CrashReporter.exe
	Copy-Item External\protobuf\bin\libprotobuf-lite.dll -Destination $BuildDir\libprotobuf-lite.dll
	
	Copy-Item "x64\Game Release\BG3ScriptExtender.dll" -Destination $BuildDir\BG3ScriptExtender.dll
	Copy-Item "x64\Game Release\BG3ScriptExtender.symtab" -Destination $BuildDir\BG3ScriptExtender.symtab
	
	Remove-Item $ZipPath -ErrorAction SilentlyContinue
	Compress-Archive -Path $BuildDir\* -DestinationPath $ZipPath -CompressionLevel Optimal -Force
}

function Create-Offline-Package ($BuildDir, $ZipPath)
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

function Build-Package ($BuildDir, $ZipPath, $DllPath, $Channel)
{
	Create-Update-Package $BuildDir $ZipPath

	x64\Release\UpdateSigner.exe sign $SigningKey $ZipPath
	x64\Release\UpdateSigner.exe verify $ZipPath

	x64\Release\UpdateSigner.exe update-manifest "$GameManifestPath" ScriptExtender "$ZipPath" "$DllPath" $GameMinVersion $GameMaxVersion "$CloudFrontRootURL/$Channel/Packages/"
}

function Bundle-Updater ($ZipPath, $DllPath)
{
	Copy-Item BG3Updater/EmbeddedResources/BlankManifest.json -Destination BG3Updater/EmbeddedResources/LocalManifest.json
	Copy-Item "$ZipPath" -Destination BG3Updater/EmbeddedResources/LocalSE.package
	x64\Release\UpdateSigner.exe update-manifest "BG3Updater/EmbeddedResources/LocalManifest.json" ScriptExtender "$ZipPath" "$DllPath" $GameMinVersion $GameMaxVersion "-"
	
	# Force a rebuild
	(gci BG3Updater/dllmain.cpp).LastWriteTime = Get-Date

	Write-Output " ===== BUILDING BUNDLED UPDATER ===== "
	msbuild BG3Updater/BG3Updater.vcxproj /p:Configuration=Release /t:Build /m /nologo /verbosity:quiet /consoleloggerparameters:summary "/property:SolutionDir=$RootPath\"
}

function Publish-Package ($ZipPath, $OfflineZipPath, $DigestPath, $Channel, $ManifestPath)
{
	$OfflineDigestPath = $DigestPath.replace('.package', '-offline.zip')
	$S3Path = "s3://$S3Bucket/$S3RootPath/$Channel/Packages/$DigestPath"
	$OfflineS3Path = "s3://$S3Bucket/$S3RootPath/$Channel/Packages/$OfflineDigestPath"
	
	Write-Output " ==== UPDATER PACKAGE ===== "
	Write-Output "S3 path: $S3Path"
	Write-Output "CloudFront path: $CloudFrontRootPath/$Channel/Packages/$DigestPath"
	Write-Output "URL: $CloudFrontRootURL/$Channel/Packages/$DigestPath"
	Write-Output "Offline URL: $CloudFrontRootURL/$Channel/Packages/$OfflineDigestPath"
	

	aws s3 cp $ZipPath "$S3Path"
	aws s3api put-object-acl --bucket nb-stor --key "$S3RootPath/$Channel/Packages/$DigestPath" --acl public-read

	aws s3 cp $OfflineZipPath "$OfflineS3Path"
	aws s3api put-object-acl --bucket nb-stor --key "$S3RootPath/$Channel/Packages/$OfflineDigestPath" --acl public-read

	aws s3 cp $ManifestPath "s3://$S3Bucket/$S3RootPath/$Channel/$S3ManifestPath"
	aws s3api put-object-acl --bucket nb-stor --key "$S3RootPath/$Channel/$S3ManifestPath" --acl public-read
	aws cloudfront create-invalidation --distribution-id $CloudFrontDistributionID --paths "$CloudFrontRootPath/$Channel/$S3ManifestPath" "$CloudFrontRootPath/$Channel/Packages/$DigestPath"
}

function Publish-Latest-Offline-Package ($ZipPath, $Channel)
{
	Write-Output " ==== LATEST OFFLINE PACKAGE ===== "
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

function Publish-Updater ()
{
	$BuildDate = (Get-Date -Format "yyyyMMdd")
	$BuildDateFmt = (Get-Date -UFormat "%Y %b %d")
	$ZipName = "BG3SE-Updater-$BuildDate.zip"
	$Title = "Script Extender ($BuildDateFmt)"
	Copy-Item x64/Release/BG3Updater.dll -Destination x64/Release/DWrite.dll
	Compress-Archive -Path x64/Release/DWrite.dll -DestinationPath "$UpdaterBuildRoot/$ZipName" -CompressionLevel Optimal -Force
	gh release create updater-test --draft --notes-file GHReleaseNotesTemplate.md --title "$Title" "$UpdaterBuildRoot/$ZipName"
}

Build-Extender

Build-Package $GameBuildDir $GameBuildZipPath $GameDllPath $GameChannel
Bundle-Updater $GameBuildZipPath $GameDllPath
Create-Offline-Package $GameOfflineBuildDir $GameOfflineBuildZipPath
Create-DebugAdapter-Package $GameDebugAdapterBuildDir $GameDebugAdapterBuildZipPath

Create-PDBDir

$GameDigestPath = (x64\Release\UpdateSigner.exe compute-path "$GameBuildZipPath" "$GameDllPath")

Archive-Build $GameDigestPath $GameBuildZipPath

Write-Output "Build completed."
Write-Output "."

Write-Output "*********************************************************************************"
Write-Output ""
Write-Output "                      ABOUT TO PUBLISH ON CHANNEL $Channel!"
Write-Output ""
Write-Output "*********************************************************************************"
Read-Host

Write-Output " ===== UPLOADING PACKAGES ===== "
Publish-Package $GameBuildZipPath $GameOfflineBuildZipPath $GameDigestPath $GameChannel $GameManifestPath
Publish-Latest-Offline-Package $GameOfflineBuildZipPath $GameChannel
Publish-DebugAdapter-Package $GameDebugAdapterBuildZipPath $GameChannel

if ($Channel -eq "Release") {
	Write-Output " ===== PUBLISHING UPDATER ===== "
	Publish-Updater
}
