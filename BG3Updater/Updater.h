#include "stdafx.h"
#include "Cache.h"
#include "HttpFetcher.h"
#include <curl/curl.h>
#include <CoreLib/Wrappers.h>
#include <SDL.h>

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
	ManifestFetcher(HttpFetcher& fetcher, UpdaterConfig const& config);
	bool Fetch(Manifest& manifest, ErrorReason& reason);
	bool Parse(std::string const& manifestStr, Manifest& manifest, ErrorReason& reason);

private:
	UpdaterConfig const& config_;
	HttpFetcher& fetcher_;
};


class ResourceUpdater
{
public:
	ResourceUpdater(HttpFetcher& fetcher, UpdaterConfig const& config, ResourceCacheRepository& cache);
	bool Update(Manifest const& manifest, std::string const& resourceName, VersionNumber const& gameVersion, ErrorReason& reason);
	bool Update(Manifest::Resource const& resource, Manifest::ResourceVersion const& version, ErrorReason& reason);

private:
	UpdaterConfig const& config_;
	ResourceCacheRepository& cache_;
	HttpFetcher& fetcher_;
};

class UpdaterConsole : public Console
{
public:
	void Print(DebugMessageType type, char const* msg) override;
};

class UpdaterUI
{
public:
	void Show();
	void Hide();
	void SetStatusText(std::wstring const& status);

private:
	bool requestShow_{ false };
	HWND progressWindow_{ NULL };
	std::wstring status_;

	void DoShow();

	static HRESULT UICallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LONG_PTR lpRefData);
	static DWORD WINAPI UIThreadMain(LPVOID param);
};

class ScriptExtenderUpdater
{
public:
	void SetGameVersion(int32_t major, int32_t minor, int32_t revision, int32_t build);
	bool TryToUpdate(ErrorReason& reason);
	inline bool IsCompleted() const;
	void InitConsole();
	void InitUI();
	void SetStatusText(std::wstring const& status);
	void RequestCancelUpdate();

	UpdaterConfig& GetConfig()
	{
		return config_;
	}

	std::string const& GetErrorMessage() const
	{
		return errorMessage_;
	}

	std::string& GetLog()
	{
		return log_;
	}

	ResourceCacheRepository* GetCache() const
	{
		return cache_.get();
	}

	VersionNumber GetCachedGameVersion() const
	{
		return gameVersion_;
	}

	bool IsCancellingUpdate() const
	{
		return cancellingUpdate_;
	}

	void RequestClientSuspend(bool suspend)
	{
		needsClientSuspend_ = suspend;
	}

	void Initialize(char const* exeDirOverride);
	void Run();
	void LoadCaches();
	bool FetchUpdates();
	bool LoadExtender();

	enum class SDLInitTag {};

private:
	VersionNumber gameVersion_;
	UpdaterConfig config_;
	HttpFetcher fetcher_;
	std::optional<Manifest> updateManifest_;
	std::wstring exeDir_;
	std::unique_ptr<ResourceCacheRepository> cache_;
	std::unique_ptr<UpdaterUI> ui_;
	std::optional<std::wstring> launchDllPath_;
	std::string errorMessage_;
	bool updated_{ false };
	bool completed_{ false };
	bool cancellingUpdate_{ false };
	std::string log_;
	volatile bool needsClientSuspend_{ false };

	WrappableFunction<SDLInitTag, int (Uint32)> SDLInitHook;

	void UpdatePaths();
	void UpdateExeDir(char const* exeDirOverride);
	void LoadConfig();
	void LoadGameVersion();
	void HookSDL();
	void UnhookSDL();
	void OnSDLInit(Uint32, int);
};

void StartUpdaterThread();

extern std::unique_ptr<ScriptExtenderUpdater> gUpdater;

END_SE()
