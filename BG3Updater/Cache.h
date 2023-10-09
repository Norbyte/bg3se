#include "stdafx.h"
#include "Manifest.h"

BEGIN_SE()

class CachedResource
{
public:
	CachedResource(std::wstring const& cachePath, Manifest::Resource const& resource, Manifest::ResourceVersion const& version);
	std::wstring GetResourceLocalPath() const;
	std::wstring TryCreateLocalResourceCacheDirectory();
	std::wstring GetLocalPath() const;
	std::wstring GetLocalPackagePath() const;
	std::wstring TryCreateLocalCacheDirectory();
	bool UpdateLocalPackage(std::vector<uint8_t> const& contents, std::string& reason);
	bool RemoveLocalPackage();
	std::wstring GetAppDllPath();
	bool ExtenderDLLExists();

private:
	std::wstring cachePath_;
	Manifest::Resource const& resource_;
	Manifest::ResourceVersion const& version_;

	bool AreDllsWriteable();
	bool UnzipPackage(std::wstring const& zipPath, std::wstring const& resourcePath, std::string& reason);
	bool DeleteLocalCacheFromZip(std::wstring const& zipPath, std::wstring const& resourcePath);
};


class ResourceCacheRepository
{
public:
	ResourceCacheRepository(UpdaterConfig const& config, std::wstring const& path);
	std::wstring GetCachedManifestPath() const;
	Manifest const& GetManifest() const;
	bool LoadManifest(std::wstring const& path);
	bool SaveManifest(std::wstring const& path);
	bool ResourceExists(std::string const& name, Manifest::ResourceVersion const& version) const;
	bool UpdateLocalPackage(Manifest::Resource const& resource, Manifest::ResourceVersion const& version, std::vector<uint8_t> const& contents, std::string& reason);
	void UpdateFromManifest(Manifest const& manifest);
	bool UpdateFromLatestMetadata(Manifest::Resource const& resource, Manifest::ResourceVersion const& version);
	bool RemoveResource(Manifest::Resource const& resource, Manifest::ResourceVersion const& version);
	std::optional<Manifest::ResourceVersion> FindResourceVersion(std::string const& name, VersionNumber const& gameVersion);
	std::optional<std::wstring> FindResourcePath(std::string const& name, VersionNumber const& gameVersion);
	std::optional<std::wstring> FindResourceDllPath(std::string const& name, VersionNumber const& gameVersion);

private:
	UpdaterConfig const& config_;
	std::wstring path_;
	Manifest manifest_;

	bool HasLocalCopy(Manifest::Resource const& resource, Manifest::ResourceVersion const& version) const;
	void AddResourceToManifest(Manifest::Resource const& resource, Manifest::ResourceVersion const& version);
	void AddVersionToResource(Manifest::Resource& resource, Manifest::ResourceVersion const& version);
};


END_SE()
