#include "stdafx.h"
#include "Config.h"
#include "Cache.h"
#include "Result.h"
#include "HttpFetcher.h"
#include "UI.h"
#include <CoreLib/Wrappers.h>
#include <SDL.h>

BEGIN_SE()

class ManifestFetcher
{
public:
    ManifestFetcher(HttpFetcher& fetcher, UpdaterConfig const& config);
    OperationResult Fetch(Manifest& manifest);
    OperationResult LoadEmbedded(Manifest& manifest);
    OperationResult Parse(std::string_view manifestStr, Manifest& manifest);

private:
    UpdaterConfig const& config_;
    HttpFetcher& fetcher_;
};


class ResourceUpdater
{
public:
    ResourceUpdater(HttpFetcher& fetcher, UpdaterConfig const& config, ResourceCacheRepository& cache);
    OperationResult Update(Manifest const& manifest, std::string const& resourceName, VersionNumber const& gameVersion);
    OperationResult Update(Manifest::Resource const& resource, Manifest::ResourceVersion const& version);

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

class ScriptExtenderUpdater
{
public:
    void SetGameVersion(int32_t major, int32_t minor, int32_t revision, int32_t build);
    OperationResult TryToUpdate();
    void UpdateFromEmbeddedCache();
    inline bool IsCompleted() const;
    void InitConsole();
    void InitUI();
    void SetStatusText(std::wstring const& status);
    void RequestCancelUpdate();

    UpdaterConfig& GetConfig()
    {
        return config_;
    }

    bool ShowError() const
    {
        return showError_;
    }

    bool IsCriticalError() const
    {
        return criticalError_;
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

    void SetAPICapabilities(uint32_t capabilities)
    {
        apiCapabilities_ = capabilities;
    }

    void Initialize(std::string_view exeDirOverride);
    void Run();
    void LoadCaches();
    void FetchUpdates();
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
    uint32_t apiCapabilities_{ 0 };

    std::optional<std::wstring> launchDllPath_;
    std::string launchNotice_;

    std::string errorMessage_;
    bool showError_{ false };
    bool criticalError_{ false };

    OperationResult updateResult_{ OperationSuccessful{} };
    bool completed_{ false };
    bool cancellingUpdate_{ false };
    std::string log_;
    volatile bool needsClientSuspend_{ false };

    WrappableFunction<SDLInitTag, int (Uint32)> SDLInitHook;

    void UpdatePaths();
    void UpdateExeDir(std::string_view exeDirOverride);
    void LoadConfig();
    void LoadGameVersion();
    void HookSDL();
    void UnhookSDL();
    void OnSDLInit(Uint32, int);
    void UpdateErrorText();
    std::wstring GetDebugDllPath();
};

void StartUpdaterThread();

extern std::unique_ptr<ScriptExtenderUpdater> gUpdater;

END_SE()
