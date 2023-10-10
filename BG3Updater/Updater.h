#include "stdafx.h"
#include "Cache.h"
#include <curl/curl.h>

BEGIN_SE()

enum class ErrorCategory
{
	General,
	ManifestFetch,
	UpdateDownload,
	LocalUpdate,
	UpdateRequired
};

struct ErrorReason
{
	std::string Message;
	ErrorCategory Category{ ErrorCategory::General };
	CURLcode CurlResult{ CURLE_OK };

	inline bool IsInternetIssue() const
	{
		return
			CurlResult == CURLE_COULDNT_RESOLVE_PROXY
			|| CurlResult == CURLE_COULDNT_RESOLVE_HOST
			|| CurlResult == CURLE_COULDNT_CONNECT
			|| CurlResult == CURLE_WEIRD_SERVER_REPLY
			|| CurlResult == CURLE_OPERATION_TIMEDOUT
			|| CurlResult == CURLE_SSL_CONNECT_ERROR
			|| CurlResult == CURLE_SEND_ERROR
			|| CurlResult == CURLE_RECV_ERROR;
	}
};

class ManifestFetcher
{
public:
	ManifestFetcher(UpdaterConfig const& config);
	bool Fetch(Manifest& manifest, ErrorReason& reason);
	bool Parse(std::string const& manifestStr, Manifest& manifest, ErrorReason& reason);

private:
	UpdaterConfig const& config_;
};


class ResourceUpdater
{
public:
	ResourceUpdater(UpdaterConfig const& config, ResourceCacheRepository& cache);
	bool Update(Manifest const& manifest, std::string const& resourceName, VersionNumber const& gameVersion, ErrorReason& reason);
	bool Update(Manifest::Resource const& resource, Manifest::ResourceVersion const& version, ErrorReason& reason);

private:
	UpdaterConfig const& config_;
	ResourceCacheRepository& cache_;
};

class ScriptExtenderUpdater
{
public:
	void SetGameVersion(int32_t major, int32_t minor, int32_t revision, int32_t build);
	bool TryToUpdate(ErrorReason& reason);
	inline bool IsCompleted() const;
	void InitConsole();
	void LoadConfig(char const* configPathOverride);

	UpdaterConfig& GetConfig()
	{
		return config_;
	}

	std::string const& GetErrorMessage() const
	{
		return errorMessage_;
	}

	ResourceCacheRepository* GetCache() const
	{
		return cache_.get();
	}

	VersionNumber GetCachedGameVersion() const
	{
		return gameVersion_;
	}

	void Run();
	void LoadCaches();
	bool FetchUpdates();
	bool LoadExtender();

private:
	VersionNumber gameVersion_;
	UpdaterConfig config_;
	std::optional<Manifest> updateManifest_;
	std::wstring exeDir_;
	std::unique_ptr<ResourceCacheRepository> cache_;
	std::optional<std::wstring> launchDllPath_;
	std::string errorMessage_;
	bool updated_{ false };
	bool completed_{ false };

	void UpdatePaths();
};

void StartUpdaterThread();

extern std::unique_ptr<ScriptExtenderUpdater> gUpdater;

END_SE()
