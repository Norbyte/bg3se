#include <Config.h>

#include <CoreLib/ConfigUtils.inl>

BEGIN_SE()

using namespace rapidjson;

std::wstring GetDefaultCachePath()
{
    TCHAR appDataPath[MAX_PATH];
    if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appDataPath)))
    {
        return L"";
    }

    return std::wstring(appDataPath) + L"\\BG3ScriptExtender";
}

void ApplyConfig(UpdaterConfig& config, Document& root)
{
    ConfigGet(root, "ManifestURL", config.ManifestURL);
    ConfigGet(root, "ManifestName", config.ManifestName);
    ConfigGet(root, "UpdateChannel", config.UpdateChannel);
    std::string targetVersion;
    ConfigGet(root, "TargetVersion", targetVersion);
    if (!targetVersion.empty()) {
        config.TargetVersion = VersionNumber::FromString(targetVersion);
    }
    ConfigGet(root, "TargetResourceDigest", config.TargetResourceDigest);
    ConfigGet(root, "CachePath", config.CachePath);
#if defined(HAS_DEBUG_LOGGING)
    ConfigGet(root, "Debug", config.Debug);
#endif
#if defined(_DEBUG)
    ConfigGet(root, "DebugLoadSE", config.DebugLoadSE);
    ConfigGet(root, "ValidateSignature", config.ValidateSignature);
#endif
    ConfigGet(root, "IPv4Only", config.IPv4Only);
    ConfigGet(root, "DisableUpdates", config.DisableUpdates);
}

void LoadConfigFile(std::wstring const& configPath, UpdaterConfig& config)
{
    config.ManifestURL = UPDATER_MANIFEST_URL;
    config.ManifestName = UPDATER_MANIFEST_NAME;
    config.UpdateChannel = UPDATER_CHANNEL;
    config.CachePath = GetDefaultCachePath();

    ApplyConfigFile(configPath, config, &ApplyConfig);
}

END_SE()
