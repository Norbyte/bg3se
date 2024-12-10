#include "stdafx.h"
#include "Cache.h"
#include <CoreLib/Crypto.h>
#include <Shlwapi.h>
#include <ZipLib/ZipFile.h>

BEGIN_SE()

CachedResource::CachedResource(std::wstring const& cachePath, Manifest::Resource const& resource, Manifest::ResourceVersion const& version)
    : cachePath_(cachePath), resource_(resource), version_(version)
{}

std::wstring CachedResource::GetResourceLocalPath() const
{
    return cachePath_ + L"\\" + FromStdUTF8(resource_.Name);
}

std::wstring CachedResource::TryCreateLocalResourceCacheDirectory()
{
    auto path = GetResourceLocalPath();
    TryCreateDirectory(path);
    return path;
}

std::wstring CachedResource::GetLocalPath() const
{
    return GetResourceLocalPath() + L"\\" + FromStdUTF8(version_.Version.ToString()) + L"_" + FromStdUTF8(version_.Digest);
}

std::wstring CachedResource::GetLocalPackagePath() const
{
    return GetLocalPath() + L".package";
}

std::wstring CachedResource::TryCreateLocalCacheDirectory()
{
    TryCreateLocalResourceCacheDirectory();
    auto path = GetLocalPath();
    TryCreateDirectory(path);
    return path;
}

bool CachedResource::UpdateLocalPackage(std::vector<uint8_t> const& contents, std::string& reason)
{
    TryCreateLocalResourceCacheDirectory();
    auto packagePath = GetLocalPackagePath();
    DEBUG("Saving update package to: %s", ToStdUTF8(packagePath).c_str());

    // Check if any of the files are currently in use by the game.
    // The shell Zip API won't tell us if it failed to overwrite one of the files, so we need to 
    // check beforehand that the files are writeable.
    if (!AreDllsWriteable()) {
        return false;
    }

    auto tempPath = packagePath + L".tmp";
    if (!SaveFile(tempPath, contents)) {
        DEBUG("Unable to write package temp file: %s", ToStdUTF8(tempPath).c_str());
        reason = "Script Extender update failed:\r\n";
        reason += std::string("Failed to write file ") + ToStdUTF8(tempPath);
        return false;
    }

    if (!CryptoUtils::VerifySignedFile(tempPath, reason)) {
        DEBUG("Unable to verify package signature: %s", reason.c_str());
        return false;
    }

    if (!MoveFileExW(tempPath.c_str(), packagePath.c_str(), MOVEFILE_REPLACE_EXISTING)) {
        DEBUG("Failed to move package file %s", packagePath.c_str());
        reason = "Script Extender update failed:\r\n";
        reason += std::string("Failed to move file ") + ToStdUTF8(packagePath);
        return false;
    }


    std::string unzipReason;
    auto cachePath = TryCreateLocalCacheDirectory();
    DEBUG("Unpacking update to %s", ToStdUTF8(cachePath).c_str());
    if (UnzipPackage(packagePath, cachePath, reason)) {
        return true;
    } else {
        DEBUG("Unzipping failed: %s", reason.c_str());
        DeleteFileW(packagePath.c_str());
        return false;
    }
}

bool CachedResource::RemoveLocalPackage()
{
    TryCreateLocalResourceCacheDirectory();
    auto packagePath = GetLocalPackagePath();
    DEBUG("Removing local package: %s", ToStdUTF8(packagePath).c_str());
    auto localPath = GetLocalPath();
    bool ok = DeleteLocalCacheFromZip(packagePath, localPath);
    DeleteFileW(packagePath.c_str());
    return ok;
}

std::wstring CachedResource::GetAppDllPath()
{
    if (IsInEditor()) {
        return GetLocalPath() + L"\\" + EDITOR_DLL;
    } else {
        return GetLocalPath() + L"\\" + GAME_DLL;
    }
}

bool CachedResource::ExtenderDLLExists()
{
    auto dllPath = GetAppDllPath();
    return PathFileExistsW(dllPath.c_str());
}

bool CachedResource::AreDllsWriteable()
{
    auto dllPath = GetAppDllPath();
    if (PathFileExistsW(dllPath.c_str())) {
        std::ofstream f;
        f.open(dllPath.c_str(), std::ios::out | std::ios::app, _SH_DENYRW);
        if (!f.good()) {
            DEBUG("Extender DLL not writeable, skipping update.");
            return false;
        }
    }

    DEBUG("DLL write check OK");
    return true;
}

bool CachedResource::UnzipPackage(std::wstring const& zipPath, std::wstring const& resourcePath, std::string& reason)
{
    auto archive = ZipFile::Open(zipPath);
    if (!archive) {
        reason = "Script Extender update failed:\r\nUnable to open update package, file possibly corrupted?";
        return false;
    }

    bool failed{ false };

    auto entries = archive->GetEntriesCount();
    for (auto i = 0; i < entries; i++) {
        auto entry = archive->GetEntry(i);

        DEBUG("Extracting: %s", entry->GetFullName().c_str());

        auto outPath = resourcePath + L"\\" + FromStdUTF8(entry->GetFullName());
        auto tempPath = resourcePath + L"\\extract.tmp";
        std::ofstream f(tempPath.c_str(), std::ios::out | std::ios::binary);
        if (!f.good()) {
            DEBUG("Failed to open %s for extraction", entry->GetFullName().c_str());
            reason = "Script Extender update failed:\r\n";
            reason += std::string("Failed to open file ") + entry->GetFullName() + " for extraction";
            failed = true;
            break;
        }

        auto stream = entry->GetDecompressionStream();
        if (!stream) {
            DEBUG("Failed to decompress %s", entry->GetFullName().c_str());
            reason = "Script Extender update failed:\r\n";
            reason += std::string("Failed to decompress file ") + entry->GetFullName();
            failed = true;
            break;
        }

        auto len = entry->GetSize();

        char buf[4096];
        while (len) {
            auto chunkSize = std::min(len, std::size(buf));
            stream->read(buf, chunkSize);
            f.write(buf, chunkSize);
            len -= chunkSize;
        }

        entry->CloseDecompressionStream();
        f.close();

        if (!MoveFileExW(tempPath.c_str(), outPath.c_str(), MOVEFILE_REPLACE_EXISTING)) {
            DEBUG("Failed to move file %s", entry->GetFullName().c_str());
            reason = "Script Extender update failed:\r\n";
            reason += std::string("Failed to update file ") + entry->GetFullName();
            failed = true;
            break;
        }
    }

    if (failed) {
        auto entries = archive->GetEntriesCount();
        for (auto i = 0; i < entries; i++) {
            auto entry = archive->GetEntry(i);
            DEBUG("Removing: %s", entry->GetFullName().c_str());
            auto outPath = resourcePath + L"\\" + FromStdUTF8(entry->GetFullName());
            DeleteFileW(outPath.c_str());
        }
    }

    return !failed;
}

bool CachedResource::DeleteLocalCacheFromZip(std::wstring const& zipPath, std::wstring const& resourcePath)
{
    auto archive = ZipFile::Open(zipPath);
    if (!archive) {
        return false;
    }

    auto entries = archive->GetEntriesCount();
    for (auto i = 0; i < entries; i++) {
        auto entry = archive->GetEntry(i);

        DEBUG("Deleting local file: %s", entry->GetFullName().c_str());
        auto extractedPath = resourcePath + L"\\" + FromStdUTF8(entry->GetFullName());
        DeleteFileW(extractedPath.c_str());
    }

    return true;
}



ResourceCacheRepository::ResourceCacheRepository(UpdaterConfig const& config, std::wstring const& path)
    : config_(config), path_(path)
{
    DEBUG("ResourceCache path: %s", ToStdUTF8(path).c_str());
    LoadManifest(GetCachedManifestPath());
}

std::wstring ResourceCacheRepository::GetCachedManifestPath() const
{
    return path_ + L"\\Manifest-" + FromStdUTF8(config_.UpdateChannel) + L".json";
}

Manifest const& ResourceCacheRepository::GetManifest() const
{
    return manifest_;
}

bool ResourceCacheRepository::LoadManifest(std::wstring const& path)
{
    std::string manifestText;
    DEBUG("Loading cache manifest: %s", ToStdUTF8(path).c_str());
    if (LoadFile(path, manifestText)) {
        ManifestSerializer parser;
        std::string parseError;
        auto result = parser.Parse(manifestText, manifest_, parseError);
        if (result == ManifestParseResult::Successful) {
            DEBUG("Cache manifest load OK.");
            return true;
        } else {
            DEBUG("Cache manifest parse failed: %d", result);
            manifest_.Resources.clear();
            return false;
        }
    }

    DEBUG("Cache manifest load failed.");
    return false;
}

bool ResourceCacheRepository::SaveManifest(std::wstring const& path)
{
    manifest_.ManifestVersion = Manifest::CurrentVersion;
    manifest_.ManifestMinorVersion = Manifest::CurrentMinorVersion;

    ManifestSerializer parser;
    std::string manifestText = parser.Stringify(manifest_);

    DEBUG("Saving cache manifest: %s", ToStdUTF8(path).c_str());
    return SaveFile(path, manifestText);
}

bool ResourceCacheRepository::ResourceExists(std::string const& name, Manifest::ResourceVersion const& version) const
{
    auto resource = manifest_.Resources.find(name);
    if (resource == manifest_.Resources.end()) {
        return false;
    }

    auto found = resource->second.ResourceVersions.find(version.Digest);
    if (found == resource->second.ResourceVersions.end()) {
        return false;
    }

    return HasLocalCopy(resource->second, found->second);
}

bool ResourceCacheRepository::UpdateLocalPackage(Manifest::Resource const& resource, Manifest::ResourceVersion const& version, std::vector<uint8_t> const& contents, std::string& reason)
{
    DEBUG("Updating local copy of resource %s, digest %s", resource.Name.c_str(), version.Digest.c_str());
    CachedResource res(path_, resource, version);
    if (res.UpdateLocalPackage(contents, reason)) {
        AddResourceToManifest(resource, version);
        if (!SaveManifest(GetCachedManifestPath())) {
            reason = "Script Extender update failed:\r\n";
            reason += std::string("Failed to write manifest file ") + ToStdUTF8(GetCachedManifestPath());
            return false;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

void ResourceCacheRepository::UpdateFromManifest(Manifest const& manifest)
{
    bool hasAnyResources{ false };

    for (auto const& res : manifest.Resources) {
        for (auto const& ver : res.second.ResourceVersions) {
            UpdateFromLatestMetadata(res.second, ver.second);
            hasAnyResources = true;
        }
    }

    // Prevent removal of local resources if a bogus manifest was fetched
    if (!hasAnyResources) {
        return;
    }

    // Remove all resource versions that are cached locally but are not present in the manifest
    std::vector<std::pair< Manifest::Resource const*, Manifest::ResourceVersion const*>> removals;
    for (auto const& res : manifest_.Resources) {
        for (auto const& ver : res.second.ResourceVersions) {
            auto resIt = manifest.Resources.find(res.second.Name);
            if (resIt == manifest.Resources.end()) {
                DEBUG("Resource %s not found in update manifest, marked for removal, digest %s", res.second.Name.c_str());
                removals.push_back(std::make_pair(&res.second, &ver.second));
            } else {
                auto verIt = resIt->second.ResourceVersions.find(ver.second.Digest);
                if (verIt == resIt->second.ResourceVersions.end()) {
                    DEBUG("Resource %s, digest %s not found in update manifest, marked for removal", res.second.Name.c_str(), ver.second.Digest.c_str());
                    removals.push_back(std::make_pair(&res.second, &ver.second));
                }
            }
        }
    }

    for (auto const& removal : removals) {
        RemoveResource(*removal.first, *removal.second);
    }
}

bool ResourceCacheRepository::UpdateFromLatestMetadata(Manifest::Resource const& resource, Manifest::ResourceVersion const& version)
{
    auto resIt = manifest_.Resources.find(resource.Name);
    if (resIt == manifest_.Resources.end()) {
        return false;
    }

    auto verIt = resIt->second.ResourceVersions.find(version.Digest);
    if (verIt == resIt->second.ResourceVersions.end()) {
        return false;
    }

    if (version.Revoked) {
        RemoveResource(resource, version);
    } else {
        verIt->second.Notice = version.Notice;
    }

    return true;
}

bool ResourceCacheRepository::RemoveResource(Manifest::Resource const& resource, Manifest::ResourceVersion const& version)
{
    auto resIt = manifest_.Resources.find(resource.Name);
    if (resIt == manifest_.Resources.end()) {
        return false;
    }

    auto verIt = resIt->second.ResourceVersions.find(version.Digest);
    if (verIt == resIt->second.ResourceVersions.end()) {
        return false;
    }

    DEBUG("Removing local copy of resource %s, digest %s", resource.Name.c_str(), version.Digest.c_str());

    CachedResource res(path_, resource, version);
    res.RemoveLocalPackage();
    resIt->second.ResourceVersions.erase(verIt);
    return true;
}

std::optional<Manifest::ResourceVersion> ResourceCacheRepository::FindResourceVersion(std::string const& name, VersionNumber const& gameVersion)
{
    auto resource = manifest_.Resources.find(name);
    if (resource == manifest_.Resources.end()) {
        return {};
    }

    return resource->second.FindResourceVersionWithOverrides(gameVersion, config_);
}

std::optional<std::wstring> ResourceCacheRepository::FindResourcePath(std::string const& name, VersionNumber const& gameVersion)
{
    auto resource = manifest_.Resources.find(name);
    if (resource == manifest_.Resources.end()) {
        return {};
    }

    auto ver = resource->second.FindResourceVersionWithOverrides(gameVersion, config_);
    if (!ver) {
        return {};
    }

    CachedResource res(path_, resource->second, *ver);
    if (res.ExtenderDLLExists()) {
        return res.GetLocalPath();
    } else {
        return {};
    }
}

std::optional<std::wstring> ResourceCacheRepository::FindResourceDllPath(std::string const& name, VersionNumber const& gameVersion)
{
    auto resource = manifest_.Resources.find(name);
    if (resource == manifest_.Resources.end()) {
        return {};
    }

    auto ver = resource->second.FindResourceVersionWithOverrides(gameVersion, config_);
    if (!ver) {
        return {};
    }

    CachedResource res(path_, resource->second, *ver);
    if (res.ExtenderDLLExists()) {
        return res.GetAppDllPath();
    } else {
        return {};
    }
}

bool ResourceCacheRepository::HasLocalCopy(Manifest::Resource const& resource, Manifest::ResourceVersion const& version) const
{
    CachedResource res(path_, resource, version);
    return res.ExtenderDLLExists();
}

void ResourceCacheRepository::AddResourceToManifest(Manifest::Resource const& resource, Manifest::ResourceVersion const& version)
{
    auto resIt = manifest_.Resources.find(resource.Name);
    if (resIt == manifest_.Resources.end()) {
        Manifest::Resource res;
        res.Name = resource.Name;
        manifest_.Resources.insert(std::make_pair(res.Name, res));
        resIt = manifest_.Resources.find(resource.Name);
    }

    AddVersionToResource(resIt->second, version);
}

void ResourceCacheRepository::AddVersionToResource(Manifest::Resource& resource, Manifest::ResourceVersion const& version)
{
    Manifest::ResourceVersion ver{ version };
    ver.URL = "";

    auto it = resource.ResourceVersions.find(version.Digest);
    if (it == resource.ResourceVersions.end()) {
        resource.ResourceVersions.insert(std::make_pair(ver.Digest, ver));
    } else {
        it->second = ver;
    }
}


END_SE()
