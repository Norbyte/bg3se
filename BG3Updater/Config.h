#pragma once

#include "GameVersion.h"

BEGIN_SE()

struct UpdaterConfig
{
    std::string ManifestURL;
    std::string ManifestName;
    std::string UpdateChannel;
    std::optional<VersionNumber> TargetVersion;
    std::string TargetResourceDigest;
    std::wstring CachePath;
#if defined(_DEBUG)
    bool Debug{ true };
    // Development option, force loading of BG3ScriptExtender.dll from BG3 bin
    bool DebugLoadSE{ false };
#else
    bool Debug{ false };
    bool DebugLoadSE{ false };
#endif
    bool ValidateSignature{ true };
    bool IPv4Only{ false };
    bool DisableUpdates{ false };
};

void LoadConfigFile(std::wstring const& configPath, UpdaterConfig& config);

END_SE()
