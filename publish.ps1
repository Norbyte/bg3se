param($Channel)

if ($Channel -ne "Devel" -And $Channel -ne "Release") {
	Write-Error -Message "Usage: publish.ps1 <Channel>"
	Exit 1
}

$S3Bucket = "nb-stor"
$S3RootPath = "bg3-updater-v2/Channels/$Channel"
$CloudFrontRootPath = "/Channels/$Channel"
$CloudFrontRootURL = "https://dzh4fh4qphx4q.cloudfront.net/Channels/$Channel"
$CloudFrontDistributionID = "E300IVONTDEMJX"

$S3ManifestPath = "Manifest2.json"

$PublishingRoot = Join-Path "PublishingRoot" "$Channel"
$PDBRoot = Join-Path "PDB" "$Channel"
$RootPath = (Get-Location).Path
$ManifestPath = Join-Path $RootPath "$PublishingRoot\Manifest2.json"
$PackagePath = Join-Path $RootPath "$PublishingRoot\Latest.zip"
$DllPath = Join-Path $RootPath "x64\Game Release\BG3ScriptExtender.dll"

$GameMinVersion = "4.36.35.601"
$GameMaxVersion = "-"

function Build-Extender
{
	# Force a build date refresh
	(gci BG3Extender/Extender/Shared/Utils.cpp).LastWriteTime = Get-Date

	# MSVC is broken and sometimes reuses old struct definitions from previous compilations.
	# Force a full recompile each time
	# TODO TEST - maybe fixed in 16.x?
	Remove-Item "x64\Game Release" -Recurse -ErrorAction SilentlyContinue

	Write-Output " ===== BUILDING EXTENDER ===== "
	msbuild BG3Tools.sln "/p:Configuration=Game Release" /t:Build /m /nologo /verbosity:quiet /consoleloggerparameters:summary
}

function Create-Update-Package ($ZipPath)
{
	Remove-Item LatestBuild -Recurse -ErrorAction SilentlyContinue

	New-Item LatestBuild -ItemType "directory"

	git show -s --format="BG3 Extender Version: Commit %H, %cD" > LatestBuild\Version.txt

	Copy-Item "x64\Release\CrashReporter.exe" -Destination LatestBuild\CrashReporter.exe
	Copy-Item "x64\Game Release\BG3ScriptExtender.dll" -Destination LatestBuild\BG3ScriptExtender.dll
	Copy-Item "External\protobuf\bin\libprotobuf-lite.dll" -Destination LatestBuild\libprotobuf-lite.dll

	Remove-Item Latest.zip -ErrorAction SilentlyContinue
	Compress-Archive -Path LatestBuild\* -DestinationPath $ZipPath -CompressionLevel Optimal
}

function Archive-Build ($ArchiveDir)
{
	New-Item $ArchiveDir -ItemType directory -ErrorAction SilentlyContinue
	Copy-Item "x64\Release\CrashReporter.pdb" -Destination "$ArchiveDir\CrashReporter.pdb"
	Copy-Item "x64\Game Release\BG3ScriptExtender.pdb" -Destination "$ArchiveDir\BG3ScriptExtender.pdb"
	Copy-Item "x64\Release\CrashReporter.exe" -Destination "$ArchiveDir\CrashReporter.exe"
	Copy-Item "x64\Game Release\BG3ScriptExtender.dll" -Destination "$ArchiveDir\BG3ScriptExtender.dll"
	Copy-Item "External\protobuf\bin\libprotobuf-lite.dll" -Destination "$ArchiveDir\libprotobuf-lite.dll"
}

function Build-Release
{
	Build-Extender
	Create-Update-Package "Latest.zip"
	Archive-Build "$PDBRoot\Latest"

	Remove-Item $PackagePath -ErrorAction SilentlyContinue
	Move-Item Latest.zip $PackagePath
	
	x64\Release\UpdateSigner.exe sign package-signer.key $PackagePath
	x64\Release\UpdateSigner.exe verify $PackagePath
}

Build-Release

x64\Release\UpdateSigner.exe update-manifest "$ManifestPath" ScriptExtender "$PackagePath" "$DllPath" $GameMinVersion $GameMaxVersion "$CloudFrontRootURL/Packages/"
$DigestPath = (x64\Release\UpdateSigner.exe compute-path "$PackagePath" "$DllPath")

Archive-Build "$PDBRoot\$DigestPath"

Write-Output "Build completed."
Write-Output "."
Write-Output "S3 path: s3://$S3Bucket/$S3RootPath/Packages/$DigestPath"
Write-Output "CloudFront path: $CloudFrontRootPath/Packages/$DigestPath"
Write-Output "URL: $CloudFrontRootURL/Packages/$DigestPath"

Write-Output "*********************************************************************************"
Write-Output ""
Write-Output "                      ABOUT TO PUBLISH ON CHANNEL $Channel!"
Write-Output ""
Write-Output "*********************************************************************************"
Read-Host


Write-Output " ===== UPLOADING PACKAGE ===== "
aws s3 cp $PackagePath "s3://$S3Bucket/$S3RootPath/Packages/$DigestPath"
aws s3api put-object-acl --bucket nb-stor --key "$S3RootPath/Packages/$DigestPath" --acl public-read
aws s3 cp $ManifestPath "s3://$S3Bucket/$S3RootPath/$S3ManifestPath"
aws s3api put-object-acl --bucket nb-stor --key "$S3RootPath/$S3ManifestPath" --acl public-read
aws cloudfront create-invalidation --distribution-id $CloudFrontDistributionID --paths "$CloudFrontRootPath/$S3ManifestPath" "$CloudFrontRootPath/Packages/$DigestPath"
