#include "stdafx.h"
#include "Cache.h"
#include <CoreLib/Crypto.h>
#include <Shlwapi.h>
#include <ZipLib/ZipFile.h>

BEGIN_SE()

CachedResource::CachedResource(std::wstring const& cachePath, Manifest::Resource const& resource, Manifest::ResourceVersion const& version)
    : cachePath_(cachePath), resourceName_(resource.Name), version_(version)
{}

std::wstring CachedResource::GetResourceLocalPath() const
{
    return cachePath_ + L"\\" + FromStdUTF8(resourceName_);
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

OperationResult CachedResource::UpdateLocalPackage(std::vector<uint8_t> const& contents)
{
    TryCreateLocalResourceCacheDirectory();
    auto packagePath = GetLocalPackagePath();
    DEBUG("Saving update package to: %s", ToStdUTF8(packagePath).c_str());

    // Check if any of the files are currently in use by the game.
    // The shell Zip API won't tell us if it failed to overwrite one of the files, so we need to 
    // check beforehand that the files are writeable.
    if (!AreDllsWriteable()) {
        return ErrorReason{ ErrorCategory::LocalUpdate, std::string("File not writeable: ") + ToStdUTF8(GetAppDllPath()) };
    }

    auto tempPath = packagePath + L".tmp";
    if (!SaveFile(tempPath, contents)) {
        DEBUG("Unable to write package temp file: %s", ToStdUTF8(tempPath).c_str());
        return ErrorReason{ ErrorCategory::LocalUpdate, std::string("Failed to write package temp file: ") + ToStdUTF8(tempPath) };
    }

    std::string reason;
    if (!CryptoUtils::VerifySignedFile(tempPath, reason)) {
        DEBUG("Unable to verify package signature: %s", reason.c_str());
        return ErrorReason{ ErrorCategory::LocalUpdate, reason };
    }

    if (!MoveFileExW(tempPath.c_str(), packagePath.c_str(), MOVEFILE_REPLACE_EXISTING)) {
        DEBUG("Failed to move package file %s", packagePath.c_str());
        return ErrorReason{ ErrorCategory::LocalUpdate, std::string("Failed to move package file: ") + ToStdUTF8(packagePath) };
    }

    auto cachePath = TryCreateLocalCacheDirectory();
    DEBUG("Unpacking update to %s", ToStdUTF8(cachePath).c_str());
    auto result = UnzipPackage(packagePath, cachePath);
    if (!result) {
        DEBUG("Unzipping failed: %s", reason.c_str());
        DeleteFileW(packagePath.c_str());
    }

    return result;
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

OperationResult UnzipFile(ZipArchive::Ptr& archive, ZipArchiveEntry::Ptr& entry, std::wstring const& outputPath)
{
    DEBUG("Extracting: %s", entry->GetFullName().c_str());

    auto tempPath = outputPath + L".tmp";
    std::ofstream f(tempPath.c_str(), std::ios::out | std::ios::binary);
    if (!f.good()) {
        DEBUG("Failed to open %s for extraction", entry->GetFullName().c_str());
        return ErrorReason{ ErrorCategory::LocalUpdate, std::string("Failed to open file ") + entry->GetFullName() + " for extraction" };
    }

    auto stream = entry->GetDecompressionStream();
    if (!stream) {
        DEBUG("Failed to decompress %s", entry->GetFullName().c_str());
        return ErrorReason{ ErrorCategory::LocalUpdate, std::string("Failed to decompress file: ") + entry->GetFullName() };
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

    if (!MoveFileExW(tempPath.c_str(), outputPath.c_str(), MOVEFILE_REPLACE_EXISTING)) {
        DEBUG("Failed to move file %s", entry->GetFullName().c_str());
        DeleteFileW(tempPath.c_str());
        return ErrorReason{ ErrorCategory::LocalUpdate, std::string("Failed to move extracted file: ") + entry->GetFullName() };
    }

    return OperationSuccessful{};
}

OperationResult CachedResource::UnzipPackage(std::wstring const& zipPath, std::wstring const& resourcePath)
{
    auto archive = ZipFile::Open(zipPath);
    if (!archive) {
        return ErrorReason{ ErrorCategory::LocalUpdate, "Unable to read update package, file possibly corrupted?" };
    }

    OperationResult fileResult{ OperationSuccessful{} };

    auto entries = archive->GetEntriesCount();
    for (auto i = 0; i < entries; i++) {
        auto entry = archive->GetEntry(i);
        DEBUG("Extracting: %s", entry->GetFullName().c_str());

        auto outPath = resourcePath + L"\\" + FromStdUTF8(entry->GetFullName());
        fileResult = UnzipFile(archive, entry, outPath);
        if (!fileResult) {
            break;
        }
    }

    if (!fileResult) {
        auto entries = archive->GetEntriesCount();
        for (auto i = 0; i < entries; i++) {
            auto entry = archive->GetEntry(i);
            DEBUG("Removing: %s", entry->GetFullName().c_str());
            auto outPath = resourcePath + L"\\" + FromStdUTF8(entry->GetFullName());
            DeleteFileW(outPath.c_str());
        }
    }

    return fileResult;
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
    if (!LoadManifest(GetCachedManifestPath())) {
        manifestDirty_ = true;
    }
}

std::wstring ResourceCacheRepository::GetCachedManifestPath() const
{
    return path_ + L"\\Manifest-" + FromStdUTF8(config_.UpdateChannel) + L".json";
}

Manifest const& ResourceCacheRepository::GetManifest() const
{
    return manifest_;
}

OperationResult ResourceCacheRepository::LoadManifest(std::wstring const& path)
{
    std::string manifestText;
    DEBUG("Loading cache manifest: %s", ToStdUTF8(path).c_str());
    if (LoadFile(path, manifestText)) {
        ManifestSerializer parser;
        std::string parseError;
        auto result = parser.Parse(manifestText, manifest_);
        if (result) {
            DEBUG("Cache manifest load OK.");
        } else {
            DEBUG("Cache manifest parse failed: %s", result.error().Message.c_str());
            manifest_.Resources.clear();
        }

        return result;
    }

    DEBUG("Cache manifest load failed.");
    return ErrorReason{ ErrorCategory::LocalUpdate, std::string("Failed to load manifest file: ") + ToStdUTF8(path) };
}

bool ResourceCacheRepository::SaveManifestIfNecessary()
{
    if (manifestDirty_) {
        DEBUG("SaveManifestIfNecessary(): Manifest requires rewrite");
        return SaveManifest(GetCachedManifestPath());
    } else {
        return true;
    }
}

bool ResourceCacheRepository::SaveManifest(std::wstring const& path)
{
    manifest_.ManifestVersion = Manifest::CurrentVersion;
    manifest_.ManifestMinorVersion = Manifest::CurrentMinorVersion;

    ManifestSerializer parser;
    std::string manifestText = parser.Stringify(manifest_);

    DEBUG("Saving cache manifest: %s", ToStdUTF8(path).c_str());
    if (SaveFile(path, manifestText)) {
        manifestDirty_ = false;
        return true;
    } else {
        return false;
    }
}

bool ResourceCacheRepository::LocalResourceExists(std::string const& name, Manifest::ResourceVersion const& version) const
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

OperationResult ResourceCacheRepository::UpdateLocalPackage(Manifest::Resource const& resource, Manifest::ResourceVersion const& version, std::vector<uint8_t> const& contents)
{
    DEBUG("Updating local copy of resource %s, digest %s", resource.Name.c_str(), version.Digest.c_str());
    CachedResource res(path_, resource, version);
    auto result = res.UpdateLocalPackage(contents);
    if (result) {
        AddResourceToManifest(resource, version);
        if (!SaveManifestIfNecessary()) {
            result = ErrorReason{ ErrorCategory::LocalUpdate, std::string("Failed to write manifest file: ") + ToStdUTF8(GetCachedManifestPath()) };
        }
    }

    return result;
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
        RemoveLocalResource(*removal.first, *removal.second);
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
        RemoveLocalResource(resource, version);
    } else {
        // Allow retroactive correction of various publishing attributes
        if (verIt->second.Version != version.Version
            || verIt->second.MinGameVersion != version.MinGameVersion
            || verIt->second.MaxGameVersion != version.MaxGameVersion
            || verIt->second.Notice != version.Notice) {
            manifestDirty_ = true;
        }

        verIt->second.Version = version.Version;
        verIt->second.MinGameVersion = version.MinGameVersion;
        verIt->second.MaxGameVersion = version.MaxGameVersion;
        verIt->second.Notice = version.Notice;
    }

    return true;
}

bool ResourceCacheRepository::RemoveLocalResource(Manifest::Resource const& resource, Manifest::ResourceVersion const& version)
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
    manifestDirty_ = true;
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

std::optional<CachedResource> ResourceCacheRepository::FindLoadableResource(std::string const& name, VersionNumber const& gameVersion)
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
        return res;
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
    manifestDirty_ = true;
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

    manifestDirty_ = true;
}


END_SE()
