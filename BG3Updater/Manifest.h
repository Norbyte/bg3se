#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <optional>
#include <CoreLib/JsonLibs.h>
#include "Result.h"

BEGIN_SE()

struct UpdaterConfig;

struct VersionNumber
{
    inline VersionNumber()
        : Major(0), Minor(0), Revision(0), Build(0)
    {}

    inline VersionNumber(int32_t major, int32_t minor, int32_t revision, int32_t build)
        : Major(major), Minor(minor), Revision(revision), Build(build)
    {}

    inline static std::optional<VersionNumber> FromString(char const* versionNumber)
    {
        VersionNumber ver;
        if (sscanf_s(versionNumber, "%d.%d.%d.%d", &ver.Major, &ver.Minor, &ver.Revision, &ver.Build) == 4) {
            return ver;
        } else {
            return {};
        }
    }

    inline std::string ToString() const
    {
        char ver[100];
        sprintf_s(ver, "%d.%d.%d.%d", Major, Minor, Revision, Build);
        return ver;
    }

    inline bool operator > (VersionNumber const& o) const
    {
        return Major > o.Major ||
            (Major == o.Major && Minor > o.Minor) ||
            (Major == o.Major && Minor == o.Minor && Revision > o.Revision) ||
            (Major == o.Major && Minor == o.Minor && Revision == o.Revision && Build > o.Build);
    }

    inline bool operator < (VersionNumber const& o) const
    {
        return Major < o.Major ||
            (Major == o.Major && Minor < o.Minor) ||
            (Major == o.Major && Minor == o.Minor && Revision < o.Revision) ||
            (Major == o.Major && Minor == o.Minor && Revision == o.Revision && Build < o.Build);
    }

    inline bool operator == (VersionNumber const& o) const
    {
        return Major == o.Major 
            && Minor == o.Minor 
            && Revision == o.Revision 
            && Build == o.Build;
    }

    inline bool operator != (VersionNumber const& o) const
    {
        return Major != o.Major 
            || Minor != o.Minor 
            || Revision != o.Revision 
            || Build != o.Build;
    }

    int32_t Major, Minor, Revision, Build;
};

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
    std::string Notice;
    std::string NoMatchingVersionNotice;
    std::unordered_map<std::string, Resource> Resources;
};

class ManifestSerializer
{
public:
    OperationResult Parse(std::string const& json, Manifest& manifest);
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
