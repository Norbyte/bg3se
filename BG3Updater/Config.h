#pragma once

BEGIN_SE()

struct UpdaterConfig
{
    std::string ManifestURL;
    std::string ManifestName;
    std::string UpdateChannel;
    std::string TargetVersion;
    std::string TargetResourceDigest;
    std::wstring CachePath;
#if defined(_DEBUG)
    bool Debug{ true };
#else
    bool Debug{ false };
#endif
    bool ValidateSignature{ true };
    bool IPv4Only{ false };
    bool DisableUpdates{ false };
};

void LoadConfigFile(std::wstring const& configPath, UpdaterConfig& config);

END_SE()
