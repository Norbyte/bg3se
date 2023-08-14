#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <optional>
#include "json/json.h"

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

enum class ManifestParseResult
{
	Successful,
	Failed,
	UpdateRequired
};

struct Manifest
{
	static constexpr int32_t CurrentVersion = 1;

	struct ResourceVersion
	{
		std::string URL;
		std::string Digest;
		VersionNumber Version;
		std::optional<VersionNumber> MinGameVersion, MaxGameVersion;
		uint64_t BuildDate{ 0 };
		bool Revoked{ false };

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
	std::unordered_map<std::string, Resource> Resources;
};

class ManifestSerializer
{
public:
	ManifestParseResult Parse(std::string const& json, Manifest& manifest, std::string& parseError);
	std::string Stringify(Manifest& manifest);

private:
	bool Parse(Json::Value const& node, Manifest& manifest, std::string& parseError);
	bool ParseResource(Json::Value const& node, Manifest::Resource& resource, std::string& parseError);
	bool ParseVersion(Json::Value const& node, Manifest::ResourceVersion& version, std::string& parseError);
};


std::optional<VersionNumber> GetFileVersion(std::wstring const& path);
std::optional<std::string> GetFileDigest(std::wstring const& path);

END_SE()
