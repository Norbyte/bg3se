#include "stdafx.h"
#include "Manifest.h"
#include "Result.h"

BEGIN_SE()

class CachedResource
{
public:
    CachedResource(std::wstring const& cachePath, std::string const& resourceName, Manifest::ResourceVersion const& version);
    std::wstring GetResourceLocalPath() const;
    std::wstring TryCreateLocalResourceCacheDirectory();
    std::wstring GetLocalPath() const;
    std::wstring GetLocalPackagePath() const;
    std::wstring TryCreateLocalCacheDirectory();
    OperationResult UpdateLocalPackage(std::string_view contents);
    bool RemoveLocalPackage();
    std::wstring GetAppDllPath();
    bool ExtenderDLLExists();

    Manifest::ResourceVersion const& GetVersion() const
    {
        return version_;
    }

private:
    std::wstring cachePath_;
    std::string resourceName_;
    Manifest::ResourceVersion version_;

    bool AreDllsWriteable();
    OperationResult UnzipPackage(std::wstring const& zipPath, std::wstring const& resourcePath);
    bool DeleteLocalCacheFromZip(std::wstring const& zipPath, std::wstring const& resourcePath);
};


class ResourceCacheRepository
{
public:
    ResourceCacheRepository(UpdaterConfig const& config, std::wstring const& path);
    std::wstring GetCachedManifestPath() const;
    Manifest const& GetManifest() const;
    OperationResult LoadManifest(std::wstring const& path);
    bool SaveManifest(std::wstring const& path);
    bool SaveManifestIfNecessary();
    bool LocalResourceExists(std::string const& name, Manifest::ResourceVersion const& version) const;
    OperationResult UpdateLocalPackage(Manifest::Resource const& resource, Manifest::ResourceVersion const& version, std::string_view contents);
    void UpdateFromManifest(Manifest const& manifest);
    bool UpdateFromLatestMetadata(Manifest::Resource const& resource, Manifest::ResourceVersion const& version);
    bool RemoveLocalResource(Manifest::Resource const& resource, Manifest::ResourceVersion const& version);
    std::optional<Manifest::ResourceVersion> FindResourceVersion(std::string const& name, VersionNumber const& gameVersion);
    std::optional<CachedResource> FindLoadableResource(std::string const& name, VersionNumber const& gameVersion);

private:
    UpdaterConfig const& config_;
    std::wstring path_;
    Manifest manifest_;
    bool manifestDirty_{ false };

    bool HasLocalCopy(Manifest::Resource const& resource, Manifest::ResourceVersion const& version) const;
    void AddResourceToManifest(Manifest::Resource const& resource, Manifest::ResourceVersion const& version);
    void AddVersionToResource(Manifest::Resource& resource, Manifest::ResourceVersion const& version);
};


END_SE()
