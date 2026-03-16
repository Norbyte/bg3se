#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <optional>
#include <CoreLib/JsonLibs.h>
#include "GameVersion.h"
#include "Result.h"

BEGIN_SE()

struct UpdaterConfig;

std::optional<VersionNumber> GetGameVersion();
std::optional<VersionNumber> GetModuleVersion(std::wstring_view path);

struct Manifest
{
    static constexpr int32_t CurrentVersion = 1;
    static constexpr int32_t CurrentMinorVersion = 1;

    struct ResourceVersion
    {
        std::string URL;
        std::string Digest;
        VersionNumber Version;
        std::optional<VersionNumber> MinGameVersion, MaxGameVersion;
        uint64_t BuildDate{ 0 };
        bool Revoked{ false };
        std::string Signature;
        std::string Notice;

        bool UpdatePackageMetadata(std::wstring const& path);
        bool UpdateDLLMetadata(std::wstring const& path);

        inline bool operator < (ResourceVersion const& o) const
        {
            if (MinGameVersion && o.MinGameVersion && MinGameVersion != o.MinGameVersion) {
                return MinGameVersion < o.MinGameVersion;
            }

            return BuildDate < o.BuildDate;
        }
    };

    struct Resource
    {
        std::string Name;
        std::unordered_map<std::string, ResourceVersion> ResourceVersions;

        std::optional<Manifest::ResourceVersion> FindResourceVersionWithOverrides(VersionNumber const& gameVersion,
            UpdaterConfig const& config) const;
        std::optional<Manifest::ResourceVersion> FindResourceVersion(VersionNumber const& gameVersion,
            std::optional<VersionNumber> resourceVersion) const;
    };

    int32_t ManifestVersion;
    int32_t ManifestMinorVersion;
    // Indicates that the manifest only contains a subset of all available resources and 
    // should not be used for local resource deletion
    bool Partial;
    std::string Notice;
    std::string NoMatchingVersionNotice;
    std::unordered_map<std::string, Resource> Resources;

    std::optional<Manifest::ResourceVersion> FindResourceVersionWithOverrides(std::string const& resourceName,
        VersionNumber const& gameVersion, UpdaterConfig const& config) const;
};

class ManifestSerializer
{
public:
    OperationResult Parse(std::string_view json, Manifest& manifest);
    std::string Stringify(Manifest const& manifest);

private:
    OperationResult Parse(rapidjson::Value const& node, Manifest& manifest);
    OperationResult ParseResource(rapidjson::Value const& node, Manifest::Resource& resource);
    OperationResult ParseVersion(rapidjson::Value const& node, Manifest::ResourceVersion& version);

    void Stringify(rapidjson::Value& doc, Manifest const& manifest, RAPIDJSON_DEFAULT_ALLOCATOR& alloc);
    void Stringify(rapidjson::Value& resources, Manifest::Resource const& resource, RAPIDJSON_DEFAULT_ALLOCATOR& alloc);
    void Stringify(rapidjson::Value& versions, Manifest::ResourceVersion const& ver, RAPIDJSON_DEFAULT_ALLOCATOR& alloc);
};


std::optional<VersionNumber> GetFileVersion(std::wstring const& path);
std::optional<std::string> GetFileDigest(std::wstring const& path);

END_SE()
